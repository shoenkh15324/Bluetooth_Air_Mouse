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


#define SCALE_X               10.0            // X-axis movement sensitivity coefficient
#define SCALE_Y               10.0            // Y-axis movement sensitivity coeffici
#define TIME_INTERVAL         0.14f           // Yaw integration time-interval


void dataProcessingInit();
void filterInit();
void readData();
bool dataProcessing();

int8_t calculateMouseX();
int8_t calculateMouseY();
int8_t calculateMouseWheel();


#endif /* INC_DATA_PROCESSING_H_ */
