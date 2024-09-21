/*
 * kalman_filter.h
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */

#ifndef INC_KALMAN_FILTER_H_
#define INC_KALMAN_FILTER_H_


typedef struct {
    float q;  // 프로세스 노이즈 공분산
    float r;  // 측정 노이즈 공분산
    float x;  // 값 추정치
    float p;  // 오류 공분산 추정치
    float k;  // 칼만 이득
} KalmanFilter;


void KalmanFilter_Init(KalmanFilter *kf, float process_noise, float measurement_noise, float initial_value);
float KalmanFilter_Update(KalmanFilter *kf, float measurement);


#endif /* INC_KALMAN_FILTER_H_ */
