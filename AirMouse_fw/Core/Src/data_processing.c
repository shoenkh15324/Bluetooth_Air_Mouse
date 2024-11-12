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

float x_sensitivity = 10.0; // sensitivity coefficient
float y_sensitivity = 10.0;

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

  // Apply SENSITIVITY
  int mouse_x = (int)(k_filtered_yaw * x_sensitivity * (-1));

  return mouse_x;
}

// Y-axis movement can be obtained with roll.
int8_t calculateMouseY()
{
  // Read MPU6050 Datasheet.
  float ax = (float)raw_ax / 4096;
  float ay = (float)raw_ay / 4096;
  float az = (float)raw_az / 4096;
  float gy = ((float)raw_gy / 32.8) - gy_bias;

  // Calculate roll (rad)
  float roll = atan2f(ax, sqrtf(ay * ay + az * az));

  static float roll_gyro = 0;
  roll_gyro += gy * TIME_INTERVAL;

  // Apply complementary filter
  float c_filtered_roll = 0.98 * roll_gyro + 0.02 * roll;

  // Apply kalman filter
  float k_filtered_roll = KalmanFilter_Update(&kf_y, c_filtered_roll);

  // Apply SENSITIVITY
  int mouse_y = (int)(k_filtered_roll * y_sensitivity);

  if (abs(mouse_y) < 1.5)
    mouse_y = 0;

  return mouse_y;
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

bool dataProcessing()
{
  // USB HID Data
  int8_t HID_report[4];

  // Initialize USB HID Data.
  HID_report[0] = 0x00; // Mouse Button State (0x01: Left Button Clicked, 0x02: Right Button Clicked)
  HID_report[1] = 0x00; // Mouse X-axis Movement.
  HID_report[2] = 0x00; // Mouse Y-axis Movement.
  HID_report[3] = 0x00; // Mouse Wheel Movement.

  // Mouse Left Button Clicked.
  if (isButtonPressed(LEFT_BTN_GPIO_Port, LEFT_BTN_Pin))
  {
    HID_report[0] = 0x01;
  }
  // Mouse Right Button Clicked.
  else if (isButtonPressed(RIGHT_BTN_GPIO_Port, RIGHT_BTN_Pin))
  {
    HID_report[0] = 0x02;
  }

  // Mouse Wheel
  if (getIsSwitch() == 1)
  {
    HID_report[3] = 0;
    changeDPI();
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
  }
  else
  {
    HID_report[3] = calculateMouseWheel();
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
  }

  // Calculate Mouse X, Y Movement.
  HID_report[1] = calculateMouseX();
  HID_report[2] = calculateMouseY();

  // Data Transmit.
  HAL_UART_Transmit(&huart2, (uint8_t *)HID_report, sizeof(HID_report), 10);

  // Print HID_report data on cli terminal.
  // cliPrintf("%d %d %d %d (encoder : %d)\n", HID_report[0], HID_report[1], HID_report[2], HID_report[3], prev_wheel);

  return 1;
}
