/*
 * complementary_filter.h
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */

#ifndef INC_COMPLEMENTARY_FILTER_H_
#define INC_COMPLEMENTARY_FILTER_H_


typedef struct {
  float angle;   // 필터링된 각도 값
  float alpha;   // 상보 필터 계수 (0 < alpha < 1)
} ComplementaryFilter;


void ComplementaryFilter_Init(ComplementaryFilter *cf, float alpha, float initial_angle);
float ComplementaryFilter_Update(ComplementaryFilter *cf,
                                 float accel_angle,
                                 float gyro_rate,
                                 float dt);


#endif /* INC_COMPLEMENTARY_FILTER_H_ */
