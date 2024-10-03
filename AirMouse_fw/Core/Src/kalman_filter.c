/*
 * kalman_filter.c
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */


#include "kalman_filter.h"


void KalmanFilter_Init(KalmanFilter *kf,
                       double process_noise,
                       double measurement_noise,
                       double initial_value)
{
    kf->q = process_noise;
    kf->r = measurement_noise;
    kf->x = initial_value;
    kf->p = 1.0f;  // 초기 공분산 값을 1로 설정
    kf->k = 0.0f;  // 초기 칼만 이득 값
}

double KalmanFilter_Update(KalmanFilter *kf, double measurement)
{
    // 1. 예측 단계
    kf->p += kf->q;

    // 2. 칼만 이득 계산
    kf->k = kf->p / (kf->p + kf->r);

    // 3. 값 업데이트
    kf->x += kf->k * (measurement - kf->x);

    // 4. 공분산 갱신
    kf->p *= (1.0f - kf->k);

    return kf->x;
}
