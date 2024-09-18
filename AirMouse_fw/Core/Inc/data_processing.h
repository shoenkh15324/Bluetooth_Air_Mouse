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


void readData();
bool dataProcessing();

void initialize_kalman_filters();
void initialize_complementary_filters();

int8_t calculate_mouse_x();
int8_t calculate_mouse_y();


#endif /* INC_DATA_PROCESSING_H_ */
