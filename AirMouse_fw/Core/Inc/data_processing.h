/*
 * data_processing.h
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */

#ifndef INC_DATA_PROCESSING_H_
#define INC_DATA_PROCESSING_H_

#include <math.h>
#include <stdbool.h>
#include <string.h>

#include "mpu6050.h"
#include "button.h"
#include "kalman_filter.h"
#include "complementary_filter.h"
#include "tim.h"
#include "encoder.h"

#define TIME_INTERVAL 0.02f // Yaw integration time-interval
#define X_SENSITIVITY_WEIGHT 0.25
#define Y_SENSITIVITY_WEIGHT 0.25

void dataProcessingInit();
void filterInit();
void readData();
bool dataProcessing();

int8_t calculateMouseX();
int8_t calculateMouseY();
int8_t calculateMouseWheel();

void increaseSensitivity();
void decreaseSensitivity();

void changeDPI();

#endif /* INC_DATA_PROCESSING_H_ */
