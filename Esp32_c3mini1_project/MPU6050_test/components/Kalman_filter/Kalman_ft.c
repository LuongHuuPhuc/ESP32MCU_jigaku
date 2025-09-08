/** Bo loc Kalman cho cam bien Gyroscope & Accelerometer
 * @file Kalman_ft.c
 * @date 2025/09/01
 * @author Luong Huu Phuc
 */

#ifdef __cplusplus
extern "C" {
#endif 

#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "math.h"
#include "esp_err.h"
#include "Kalman_ft.h"

esp_err_t Kalman_init(Kalman_filter_t *kf, float Q_angle, float Q_bias, float R_measure){
  if(kf == NULL) return ESP_ERR_INVALID_ARG;

  kf->Q_angle = Q_angle;
  kf->Q_bias = Q_bias;
  kf->R_measure = R_measure;
  
  kf->angle = 0.0f;
  kf->bias = 0.0f;
  kf->rate = 0.0f;

  memset(kf->Q, 0, sizeof(&kf->Q));

  kf->sensor_mode = SENSOR_ACCEL_GYRO;
  kf->state = KALMAN_STATE_READY;

  return ESP_OK;
}

float Kalman_Update(Kalman_filter_t *kf, float newAngle){
  //Sai so doi moi (innovation)
  float y = newAngle - kf->angle;

  //S sai so do
  float S = kf->Q[0][0] + kf->R_measure;

  //K: He so Kalman
  float K0 = kf->Q[0][0] / S;
  float K1 = kf->Q[1][0] / S;

  //Cap nhat trang thai
  kf->angle += K0 * y;
  kf->bias += K1 * y;

  //Cap nhat ma tran hiep phuong sai 
  float P00_temp = kf->Q[0][0];
  float P01_temp = kf->Q[0][1];

  kf->Q[0][0] -= K0 * P00_temp;
  kf->Q[0][1] -= K0 * P01_temp;
  kf->Q[1][0] -= K1 * P00_temp;
  kf->Q[1][1] -= K1 * P01_temp;

  return kf->angle;
}

void Kalman_Predict(Kalman_filter_t *kf, float newRate, float dt){
  //Buoc du doan
  kf->rate = newRate - kf->bias; //Toc do goc khi da tru di bias (troi) truoc khi tich phan
  kf->angle += dt * kf->rate; //Tich phan toc do goc -> Tinh ra goc (pitch_gyro, roll_gyro hoac yaw_gyro)

  //Cap nhat phuong trinh ma tran hiep phuong sai
  kf->Q[0][0] += dt * (dt * kf->Q[1][1] - kf->Q[0][1] - kf->Q[1][0] + kf->Q_angle);
  kf->Q[0][1] -= dt * kf->Q[1][1];
  kf->Q[1][0] -= dt * kf->Q[1][1];
  kf->Q[1][1] += kf->Q_bias * dt;
  kf->state = KALMAN_STATE_RUNNING;
}

float Kalman_getAngle(Kalman_filter_t *kf, float newAngle, float newRate, float dt){
  Kalman_Predict(kf, newRate, dt);
  return Kalman_Update(kf, newAngle);
}

esp_err_t Kalman_Reset(Kalman_filter_t *kf){
  if(kf == NULL) return ESP_ERR_INVALID_ARG;

  kf->angle = 0.0f;
  kf->bias = 0.0f;
  kf->rate = 0.0f;
  kf->state = KALMAN_STATE_READY;
  memset(kf->Q, 0, sizeof(&kf->Q));

  return ESP_OK;
}

#ifdef __cplusplus
}
#endif //__cplusplus