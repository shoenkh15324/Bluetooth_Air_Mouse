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

#include "cli.h"
#include "mpu6050.h"
#include "button.h"
#include "kalman_filter.h"
#include "complementary_filter.h"
#include "tim.h"



#define TIME_INTERVAL         0.1f            // Changed to seconds. (100ms)




void dataProcessingInit();
void filterInit();
void readData();
bool dataProcessing();

int8_t calculateMouseX();
int8_t calculateMouseY();

void setSCALE_X(int val);
void setSCALE_Y(int val);

#endif /* INC_DATA_PROCESSING_H_ */
