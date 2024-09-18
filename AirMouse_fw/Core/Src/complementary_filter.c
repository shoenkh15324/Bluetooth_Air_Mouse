/*
 * complementary_filter.c
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */


#include "complementary_filter.h"

void ComplementaryFilter_Init(ComplementaryFilter *cf, float alpha, float initial_angle) {
    cf->alpha = alpha;  // 상보 필터 계수 설정
    cf->angle = initial_angle;  // 초기 각도 값 설정
}

float ComplementaryFilter_Update(ComplementaryFilter *cf,
                                 float accel_angle,
                                 float gyro_rate,
                                 float dt) {
    // 상보 필터 방정식: 필터링된 각도 = (자이로 각속도 기반 각도) + (가속도계 기반 각도)
    cf->angle = cf->alpha * (cf->angle + gyro_rate * dt) + (1.0f - cf->alpha) * accel_angle;
    return cf->angle;
}
