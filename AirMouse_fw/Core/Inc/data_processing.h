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


#define SCALE_X               100.0           // X-axis movement sensitivity coefficient
#define SCALE_Y               100.0           // Y-axis movement sensitivity coefficient
#define TIME_INTERVAL         0.1f            // Changed to seconds. (100ms)


void filterInit();
void readData();
bool dataProcessing();

int8_t calculateMouseX();
int8_t calculateMouseY();


#endif /* INC_DATA_PROCESSING_H_ */
