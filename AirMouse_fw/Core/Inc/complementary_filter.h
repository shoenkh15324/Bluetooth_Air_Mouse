/*
 * complementary_filter.h
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */

#ifndef INC_COMPLEMENTARY_FILTER_H_
#define INC_COMPLEMENTARY_FILTER_H_


typedef struct {
  double angle;   // 필터링된 각도 값
  double alpha;   // 상보 필터 계수 (0 < alpha < 1)
} ComplementaryFilter;


void ComplementaryFilter_Init(ComplementaryFilter *cf, double alpha, double initial_angle);
double ComplementaryFilter_Update(ComplementaryFilter *cf,
                                  double accel_angle,
                                  double gyro_rate,
                                  double dt);


#endif /* INC_COMPLEMENTARY_FILTER_H_ */
