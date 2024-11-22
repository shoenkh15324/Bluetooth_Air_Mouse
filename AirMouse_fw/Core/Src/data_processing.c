/*
 * data_processing.c
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */

#include "data_processing.h"

int16_t raw_ax = 0, raw_ay = 0, raw_az = 0; // Raw Accelerometer x, y, z.
int16_t raw_gx = 0, raw_gy = 0, raw_gz = 0; // Raw Gyrometer x, y, z.
int16_t raw_temp = 0;                       // Raw Temperature.

KalmanFilter kf_z, kf_y;        // Kalman Filter
ComplementaryFilter cf_z, cf_y; // Complementary Filter

int32_t prev_wheel = 0;

float gy_bias = 0.0;

// sensitivity coefficient
float x_sensitivity = 5.0;
float y_sensitivity = 5.0;

void dataProcessingInit()
{
  // Initialize filters.
  filterInit();
}

// Initialize kalman filter and complementary filter.
void filterInit()
{
  KalmanFilter_Init(&kf_z, 0.01f, 0.1f, 0.0f);
  KalmanFilter_Init(&kf_y, 0.01f, 0.1f, 0.0f);
  ComplementaryFilter_Init(&cf_z, 0.98f, 0.0f);
  ComplementaryFilter_Init(&cf_y, 0.98f, 0.0f);
}

// X-axis movement can be obtained wity yaw.
int8_t calculateMouseX()
{
  // Read MPU6050 Datasheet.
  float gz = (float)raw_gz / 32.8;

  // Calculate yaw (rad)
  float yaw = gz * TIME_INTERVAL;

  // Apply complementary filter
  float c_filtered_yaw = ComplementaryFilter_Update(&cf_z, yaw, gz, TIME_INTERVAL);

  // Apply kalman filter
  float k_filtered_yaw = KalmanFilter_Update(&kf_z, c_filtered_yaw);

  float threshold_x = 0.2 * x_sensitivity;

  // Apply SENSITIVITY
  int mouse_x = (int)(k_filtered_yaw * x_sensitivity * (-1));

  if (mouse_x < threshold_x && mouse_x > 0)
  {
    mouse_x = 0.0;
  }

  return mouse_x;
}

// Y-axis movement can be obtained with pitch.
int8_t calculateMouseY()
{
  // Read MPU6050 Datasheet.
  float ax = (float)raw_ax / 4096;
  float ay = (float)raw_ay / 4096;
  float az = ((float)raw_az / 4096) - 1;
  float gx = ((float)raw_gx / 32.8);

  // Calculate roll (rad)
  float roll = atan2f(ay, sqrtf(ax * ax + az * az));

  // Apply complementary filter
  float c_filtered_roll = ComplementaryFilter_Update(&cf_y, roll, gx, TIME_INTERVAL);

  // Apply kalman filter
  float k_filtered_roll = KalmanFilter_Update(&kf_y, c_filtered_roll);

  // Apply SENSITIVITY
  float mouse_y = k_filtered_roll * y_sensitivity * (-1);

  float threshold_y = 0.4 * y_sensitivity;

  if (mouse_y < threshold_y && mouse_y > 0)
  {
    mouse_y = 0.0;
  }

  return (int)mouse_y;
}

int8_t calculateMouseWheel()
{
  int16_t curr_wheel = __HAL_TIM_GET_COUNTER(&htim3);

  // wrap-around 처리 (128 -> 0 or 0 -> 128)
  if (curr_wheel == 0 && prev_wheel == 128)
  {
    prev_wheel = curr_wheel;
    return 1; // 시계 방향 회전, 휠 위로 스크롤
  }
  else if (curr_wheel == 128 && prev_wheel == 0)
  {
    prev_wheel = curr_wheel;
    return -1; // 반시계 방향 회전, 휠 아래로 스크롤
  }

  // 일반적인 값 증가/감소 처리
  if (curr_wheel > prev_wheel)
  {
    prev_wheel = curr_wheel;
    return 1; // 휠 위로 스크롤
  }
  else if (curr_wheel < prev_wheel)
  {
    prev_wheel = curr_wheel;
    return -1; // 휠 아래로 스크롤
  }

  // 변화가 없을 때
  return 0;
}

void changeDPI()
{
  int16_t curr_wheel = __HAL_TIM_GET_COUNTER(&htim3);

  // wrap-around 처리 (128 -> 0 or 0 -> 128)
  if (curr_wheel == 0 && prev_wheel == 128)
  {
    prev_wheel = curr_wheel;
    increaseSensitivity();
  }
  else if (curr_wheel == 128 && prev_wheel == 0)
  {
    prev_wheel = curr_wheel;
    decreaseSensitivity();
  }

  // 일반적인 값 증가/감소 처리
  if (curr_wheel > prev_wheel)
  {
    prev_wheel = curr_wheel;
    increaseSensitivity();
  }
  else if (curr_wheel < prev_wheel)
  {
    prev_wheel = curr_wheel;
    decreaseSensitivity();
  }
}

void increaseSensitivity()
{
  if (x_sensitivity < 100.0)
    x_sensitivity += X_SENSITIVITY_WEIGHT;
  if (y_sensitivity < 100.0)
    y_sensitivity += Y_SENSITIVITY_WEIGHT;
}

void decreaseSensitivity()
{
  if (x_sensitivity > 1.0)
    x_sensitivity -= X_SENSITIVITY_WEIGHT;
  if (y_sensitivity > 1.0)
    y_sensitivity -= Y_SENSITIVITY_WEIGHT;
}

void readData()
{
  // Read MPU6050 data.
  MPU6050_GetData(&raw_ax, &raw_ay, &raw_az, &raw_gx, &raw_gy, &raw_gz, &raw_temp);

  // Read button data
  buttonRead(LEFT_BTN_GPIO_Port, LEFT_BTN_Pin);
  buttonRead(RIGHT_BTN_GPIO_Port, RIGHT_BTN_Pin);

  // Read Encoder Switch
  readEncoderSwitch(ENCODER_OTS_GPIO_Port, ENCODER_OTS_Pin);
}

uint32_t dataPadding(int8_t *HID_report)
{
  uint32_t value = 0;

  value |= ((uint32_t)HID_report[1] << 24);
  value |= ((uint32_t)HID_report[2] << 16);
  value |= ((uint32_t)HID_report[3] << 8);
  value |= ((uint32_t)HID_report[4] << 0);

  return value;
}

bool dataProcessing()
{
  // USB HID Data
  int8_t HID_report[7];

  // Initialize USB HID Data.
  HID_report[0] = 0x0B; // Start bit.
  HID_report[1] = 0x00; // Mouse Button State.
  HID_report[2] = 0x00; // Mouse X-axis Movement.
  HID_report[3] = 0x00; // Mouse Y-axis Movement.
  HID_report[4] = 0x00; // Mouse Wheel Movement.
  HID_report[5] = 0x00; // CRC
  HID_report[6] = 0x7F; // End bit.

  // Mouse Left Button Clicked.
  if (isButtonPressed(LEFT_BTN_GPIO_Port, LEFT_BTN_Pin))
  {
    HID_report[1] = 0x01;
  }
  // Mouse Right Button Clicked.
  else if (isButtonPressed(RIGHT_BTN_GPIO_Port, RIGHT_BTN_Pin))
  {
    HID_report[1] = 0x02;
  }

  // Mouse Wheel
  if (getIsSwitch() == 1)
  {
    HID_report[4] = 0;
    changeDPI();
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
  }
  else
  {
    HID_report[4] = calculateMouseWheel();
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
  }

  // Calculate Mouse X, Y Movement.
  HID_report[2] = calculateMouseX();
  HID_report[3] = calculateMouseY();

  // Calculate CRC
  uint32_t padded_value = dataPadding(&HID_report);
  HID_report[5] = HAL_CRC_Calculate(&hcrc, (uint32_t *)&padded_value, 1);

  // Data Transmit.
  HAL_UART_Transmit(&huart2, (uint8_t *)HID_report, sizeof(HID_report), 10);

  return 1;
}
