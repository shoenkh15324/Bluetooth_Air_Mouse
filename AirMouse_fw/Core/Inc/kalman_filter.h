/*
 * kalman_filter.h
 *
 *  Created on: Sep 17, 2024
 *      Author: mok07
 */

#ifndef INC_KALMAN_FILTER_H_
#define INC_KALMAN_FILTER_H_


typedef struct {
  double q;  // 프로세스 노이즈 공분산
  double r;  // 측정 노이즈 공분산
  double x;  // 값 추정치
  double p;  // 오류 공분산 추정치
  double k;  // 칼만 이득
} KalmanFilter;


void KalmanFilter_Init(KalmanFilter *kf, double process_noise, double measurement_noise, double initial_value);
double KalmanFilter_Update(KalmanFilter *kf, double measurement);


#endif /* INC_KALMAN_FILTER_H_ */
