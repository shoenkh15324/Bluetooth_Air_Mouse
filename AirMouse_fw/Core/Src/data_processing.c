/*
 * data_processing.c
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */

#include <data_processing.h>


int16_t raw_ax = 0, raw_ay = 0, raw_az = 0;   // Raw Accelerometer x, y, z.
int16_t raw_gx = 0, raw_gy = 0, raw_gz = 0;   // Raw Gyrometer x, y, z.
int16_t raw_temp = 0;                         // Raw Temperature.

KalmanFilter kf_z, kf_y;                      // Kalman Filter
ComplementaryFilter cf_z, cf_y;               // Complementary Filter


int SCALE_X = 350;           // X-axis movement sensitivity coefficient
int SCALE_Y = 350;           // Y-axis movement sensitivity coefficient


int16_t wheel_data = 0;


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
  float mouse_x = k_filtered_yaw * SCALE_X * (-1);

  return mouse_x;
}

// Y-axis movement can be obtained with roll.
int8_t calculateMouseY()
{
  // Read MPU6050 Datasheet.
  float ax = (float)raw_ax / 4096;
  float ay = (float)raw_ay / 4096;
  float az = (float)raw_az / 4096;
  float gy = (float)raw_gy / 32.8;

  // Calculate roll (rad)
  float roll = atan2f(ax, sqrtf(ay * ay + az * az));

  // Apply complementary filter
  float c_filtered_roll = ComplementaryFilter_Update(&cf_y, roll, gy, (float)TIME_INTERVAL);

  // Apply kalman filter
  float k_filtered_roll = KalmanFilter_Update(&kf_y, c_filtered_roll);

  // Apply SENSITIVITY
  float mouse_y = k_filtered_roll * SCALE_Y;

  return mouse_y;
}

void readData()
{
  // Read MPU6050 data.
  MPU6050_GetData(&raw_ax, &raw_ay, &raw_az, &raw_gx, &raw_gy, &raw_gz, &raw_temp);

  // Read button data
  buttonRead(LEFT_BTN_GPIO_Port, LEFT_BTN_Pin);
  buttonRead(RIGHT_BTN_GPIO_Port, RIGHT_BTN_Pin);

  // Read Mouse wheel data.
  wheel_data = __HAL_TIM_GET_COUNTER(&htim3);
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

  // Calculate Mouse X, Y Movement.
  HID_report[1] = calculateMouseX();
  HID_report[2] = calculateMouseY();

  // Mouse Left Button Clicked.
  if(isButtonPressed(LEFT_BTN_GPIO_Port, LEFT_BTN_Pin))
  {
    HID_report[0] = 0x01;
  }
  // Mouse Right Button Clicked.
  else if(isButtonPressed(RIGHT_BTN_GPIO_Port, RIGHT_BTN_Pin))
  {
    //HID_report[0] = 0x02;
    setSCALE_X(50);
    setSCALE_Y(50);
  }

  // Mouse Wheel
  HID_report[3] = wheel_data;

  // Data Transmit.
  HAL_UART_Transmit(&huart2, (uint8_t *)HID_report, sizeof(HID_report), 50);

//  cliPrintf("ax: %.2f, ay: %.2f, az: %.2f, gx: %.2f, gy: %.2f, gz: %.2f\n",
//            (float)raw_ax / 4096,
//            (float)raw_ay / 4096,
//            (float)raw_az / 4096,
//            (float)raw_gx / 32.8,
//            (float)raw_gy / 32.8,
//            (float)raw_gz / 32.8);

  // Print HID_report data on cli terminal.
  cliPrintf("%d %d %d %d\n", HID_report[0], HID_report[1], HID_report[2], HID_report[3]);

  // Data Processing delay.
  HAL_Delay((uint32_t)(TIME_INTERVAL * 10));

  return 1;
}

void setSCALE_X(int val)
{
  SCALE_X += val;

  if(SCALE_X > 400)
    SCALE_X = 350;
}

void setSCALE_Y(int val)
{
  SCALE_Y += val;

  if(SCALE_Y > 400)
    SCALE_Y = 350;
}

