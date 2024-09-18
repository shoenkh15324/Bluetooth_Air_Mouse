/*
 * data_processing.c
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */

#include "data_processing.h"


#define SCALE_X               100.0
#define SCALE_Y               100.0
#define TIME_INTERVAL         0.1f            // Changed to seconds. (100ms)


int16_t raw_ax = 0, raw_ay = 0, raw_az = 0;   // Raw Accelerometer x, y, z.
int16_t raw_gx = 0, raw_gy = 0, raw_gz = 0;   // Raw Gyrometer x, y, z.
int16_t raw_temp = 0;                         // Raw Temperature.

KalmanFilter kf_z, kf_y;                      // Kalman Filter
ComplementaryFilter cf_z, cf_y;               // Complementary Filter

float prev_yaw = 0.0;
float yaw = 0.0;


void initialize_filters()
{
    KalmanFilter_Init(&kf_z, 0.01f, 0.1f, 0.0f);
    KalmanFilter_Init(&kf_y, 0.01f, 0.1f, 0.0f);
    ComplementaryFilter_Init(&cf_z, 0.98f, 0.0f);
    ComplementaryFilter_Init(&cf_y, 0.98f, 0.0f);
}

int8_t calculate_mouse_x()
{
  float gz = (float)raw_gz / 32.8;

  // Calculate yaw (rad)
  yaw = gz * TIME_INTERVAL;

  prev_yaw = yaw;

  // Apply complementary filter
  float c_filtered_yaw = ComplementaryFilter_Update(&cf_z, yaw, gz, TIME_INTERVAL);

  // Apply kalman filter
  float k_filtered_yaw = KalmanFilter_Update(&kf_z, c_filtered_yaw);

  // Apply ACCEL_SENSITIVITY
  float mouse_x = k_filtered_yaw * SCALE_X * (-1);

  return mouse_x;
}

int8_t calculate_mouse_y()
{
  float ay = (float)raw_ay / 4096;
  float ax = (float)raw_ax / 4096;
  float az = (float)raw_az / 4096;
  float gy = (float)raw_gy / 32.8;

  // Calculate roll (rad)
  float roll = atan2f(ax, sqrtf(ay * ay + az * az));

  // Apply complementary filter
  float c_filtered_roll = ComplementaryFilter_Update(&cf_y, roll, gy, (float)TIME_INTERVAL);

  // Apply kalman filter
  float k_filtered_roll = KalmanFilter_Update(&kf_y, c_filtered_roll);

  // Apply ACCEL_SENSITIVITY
  float mouse_y = k_filtered_roll * SCALE_Y;

  return mouse_y;
}

void readData()
{
  MPU6050_GetData(&raw_ax, &raw_ay, &raw_az, &raw_gx, &raw_gy, &raw_gz, &raw_temp);
  buttonDeboucing(LEFT_BTN_GPIO_Port, LEFT_BTN_Pin);
  buttonDeboucing(RIGHT_BTN_GPIO_Port, RIGHT_BTN_Pin);
}

bool dataProcessing()
{
  initialize_filters(); // Initialize filters.

  int8_t report[4];

  report[0] = 0x00; // Mouse Button State (0x01: Left Button Clicked, 0x02: Right Button Clicked)
  report[1] = 0x00; // Mouse X-axis Movement.
  report[2] = 0x00; // Mouse Y-axis Movement.
  report[3] = 0x00; // Mouse Wheel Movement.

  // Calculate Mouse X, Y Movement.
  report[1] = calculate_mouse_x();
  report[2] = calculate_mouse_y();

  // Mouse Left Button Clicked.
  if(isButtonPressed(LEFT_BTN_GPIO_Port, LEFT_BTN_Pin))
  {
    report[0] = 0x01;
  }
  // Mouse Right Button Clicked.
  else if(isButtonPressed(RIGHT_BTN_GPIO_Port, RIGHT_BTN_Pin))
  {
    report[0] = 0x02;
  }

  //snprintf((char*)report_str, sizeof(report_str), "%02X%02X%02X%02X", report[0], report[1], report[2], report[3]);

  // Data Transmit.
  HAL_UART_Transmit(&huart2, (uint8_t *)report, sizeof(report), 50);

//  cliPrintf("ax: %.2f, ay: %.2f, az: %.2f, gx: %.2f, gy: %.2f, gz: %.2f\n",
//            (float)raw_ax / 4096,
//            (float)raw_ay / 4096,
//            (float)raw_az / 4096,
//            (float)raw_gx / 32.8,
//            (float)raw_gy / 32.8,
//            (float)raw_gz / 32.8);

  cliPrintf("%d %d %d %d\n", report[0], report[1], report[2], report[3]);

  HAL_Delay((uint32_t)(TIME_INTERVAL * 1000));

  return 1;
}