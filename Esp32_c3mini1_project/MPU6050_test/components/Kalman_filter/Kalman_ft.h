/** Bo loc Kalman cho cam bien Gyroscope & Accelerometer
 * @file Kalman_ft.h
 * @date 2025/09/01
 * @author Luong Huu Phuc
 */

#ifndef __INC_KALMAN_FT_H
#define __INC_KALMAN_FT_H

#ifdef __cplusplus
extern "C" {
#endif 

#pragma once 

#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdbool.h"
#include "esp_err.h"

typedef enum {
  KALMAN_STATE_UNINITIALIZED = 0,
  KALMAN_STATE_READY,
  KALMAN_STATE_RUNNING
} Kalman_state_t;

typedef enum {
  SENSOR_ACCEL_ONLY = 0,
  SENSOR_GYRO_ONLY,
  SENSOR_ACCEL_GYRO
} Sensor_mode_t;

typedef struct __Kalman_filter_t{
  float angle; //Goc uoc luong (deg hoac rad)
  float bias; //Uoc luong bias cua gyro
  float rate; //Toc do goc da loai bias

  float P[2][2]; //Ma tran hiep phuong sai 2x2 (Do khong chac chan) 

  float Q_angle; //Nhieu qua trinh do goc (Neu Q_angle lon -> Bo loc tin vao Gyro, nguoc lai -> Bo loc tin vao Accel)
  float Q_bias; //Nhieu qua trinh cho bias cho Gyro
  float R_measure; //Nhieu do cua accelerometer

  Sensor_mode_t sensor_mode;
  Kalman_state_t state;
} Kalman_filter_t;

// =============== CHUC NANG =================

/**
 * @brief Khoi tao bo loc Kalman 
 * 
 * @param kf Con tro toi struct 
 * @param Q_angle Nhieu qua trinh cho goc
 * @param Q_bias Nhieu qua trinh cho bias
 * @param R_measure Nhieu do luong tu cam bien (Accel)
 * @return Success = ESP_OK
 */
esp_err_t Kalman_init(Kalman_filter_t *kf, float Q_angle, float Q_bias, float R_measure);

/**
 * @brief Buoc cap nhat (update) Kalman filter voi goc do tu Accel
 * 
 * @param kf Con tro toi struct
 * @param newAngle Goc do duoc tu Accelerometer
 * @return Goc sau khi duoc uoc luong boi Kalman Filter
 */
float Kalman_Update(Kalman_filter_t *kf, float newAngle);

/**
 * @brief Buoc du doan (Predict) cua Kalman Filter voi toc do goc Gyro
 * 
 * @param kf Con tro toi struct
 * @param newRate Toc do goc do duoc tu Gyro
 * @param dt Khoang thoi gian lay mau (s)
 */
void Kalman_Predict(Kalman_filter_t *kf, float newRate, float dt);

/**
 * @brief Ham tich hop ca predict + update
 * \brief - Dung khi co ca du lieu Gyro (rate) va Accel (Angle)
 * 
 * @param kf Con tro toi struct
 * @param newAngle Goc do duoc tu Accelerometer
 * @param newRate Toc do goc do tu Gyro 
 * @param dt Thoi gian lay mau (s)
 * @return Goc sau khi loc
 */
float Kalman_getAngle(Kalman_filter_t *kf, float newAngle, float newRate, float dt);

/**
 * @brief Reset Kalman Filter ve trang thai ban dau 
 * 
 * @param kf Con tro toi struct
 * @return Success = ESP_OK
 */
esp_err_t Kalman_Reset(Kalman_filter_t *kf);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif // __INC_KALMAN_FT_H

/** NOTE 
 * P[2][2]: Ma tran hiep phuong sai nhieu du doan 2x2 (Dung de quan ly do khong chac chan trong uoc luong cua Kalman)
 * - P thay doi theo thoi gian, tang khi du doan (vi ta it chac chan hon) va giam khi cap nhat voi do luong (vi ta them thong tin moi)
 * - No duoc update moi vong lap Kalman de can bang giua du doan va do luong
 * 
 * angle: 
 * - Gia tri goc duoc uoc luong cuoi cung (output cua Kalman)
 * - Day la gia tri ta dung cho dieu khien (vd: giu can bang xe)
 * - Duoc cap nhat lien tuc qua 2 giai doan: Predict (Du doan tu Gyro) va Update (Cap nhat tu Accel)
 * 
 * bias: 
 * - Sai lenh/do troi(drift) ma Kalman uoc luong cho cam bien Gyro
 * - Kalman tru dan bias nay de Gyro khong bi lech ve lau dai
 * 
 * rate: 
 * - Toc do goc da duoc bu bias: rate = gyro_rate - bias
 * 
 * Q: Nhieu qua trinh do (Process Noise) trong giai doan Predict
 *
 * Q_angle: la tham so "Process Noise Convariance" - hiep phuong sai nhieu qua trinh/nhieu do cho trang thai goc
 * - No the hien muc do "tin tuong" vao mo hinh toan hoc cua he thong khi du doan goc
 * - Neu Q_angle qua nho -> Bo loc tin tuong manh vao mo hinh -> Phan ung cham voi thay doi dot ngot
 * - Neu Q_angle qua lon -> Bo loc coi mo hinh kem tin cay -> Phan ung nhanh nhung nhieu nhieu hon
 * 
 * Q_bias: la tham so "Process Noise Convariance" - hiep phuong sai nhieu qua trinh cho trang thai bias (sai lenh cua gyro theo thoi gian)
 * - Gyro thuong co xu huong troi (bias/drift), vi vay Kalman can uoc luong khoang bias va cap nhat no
 * - Neu Q_bias lon -> Bo loc thay doi bias nhanh chong
 * - Neu Q_bias nho -> Bo lon giu bias on dinh, it thay doi
 * 
 * R_measure: la tham so "Measure Noise Convariance" - nhieu do luong truc tiep tu cam bien (thuong la accel)
 * - Xac dinh muc do tin tuong vao du lieu do
 * - Neu R_measure nho -> coi Accel rat chinh xac -> Bo loc "di theo" accel
 * - Neu R_measure lon -> coi Accel nhieu -> bo loc tin tuong gyro hon
 * 
 * => Q_angle, Q_bias dieu chinh muc tin tuong vao mo hinh dong hoc
 * => R_measure dieu chinh muc tin tuong vao so do cam bien
 */