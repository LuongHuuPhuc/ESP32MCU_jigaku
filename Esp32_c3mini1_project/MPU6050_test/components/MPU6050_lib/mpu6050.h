/** @file mpu6050.h
 * @date 2025/08/29
 * @author Luong Huu Phuc
 * @brief MPU6050 driver (n time write sensor driver ! try my best !)
 */

#ifndef __INC_MPU6050_H_
#define __INC_MPU6050_h

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#pragma once 
#include "stdio.h"
#include "stdint-gcc.h"
#include "esp_log.h"
#include "esp_err.h"
#include "I2C_dev.h"

// ===== GENERAL CONFIG REGISTER =====

#define MPU6050_CONFIG          0x1A //(Write-Read) Dung de dong bo du lieu voi tin hieu ngoai (FSYNC) va loc tin hieu gyro va accel (DLPF) truoc khi xuat ra (thanh ghi quyet dinh Gyro Output Rate + Filter)
//Gom 2 truong EXT_SYNC_SET[2:0] - dong bo tin hieu va DLPF_CFG[2:0] - quyet dinh tan so noi bo (Gyro Output Rate)
//Neu DLPF_CFG = 0 hoac 7 -> Gyro Output Rate = 8kHz
//Neu DLPF_CFG =  1 den 6 -> Gyro Output Rate = 1kHz

#define MPU6050_WHO_AM_I        0x75 //(Read-only) Thanh ghi de xac minh danh tinh (verify identity) cua cam bien [6:1] - Bit cuoi quyet dinh boi AD0 pin
#define MPU6050_DEFAULT_ADRR    0x68 //(Read-only) Dia chi I2C mac dinh (6-bit dau, khong co bit cuoi)
#define MPU6050_SMPRT_DIV       0x19 //(Write-Read) Dung de tao tan so lay mau cuoi cung tu thanh ghi 0x1A (8-bit) - Sample Rate = Gyro Output Rate / (SMPRT_DIV + 1)
//Trong do Gyro Output Rate phu thuoc vao cau hinh DLPF-CFG o thanh ghi MPU6050_CONFIG (0x1A) de sinh ra Sample Rate cuoi cung


#define MPU6050_GYRO_CONFIG     0x1B //(Write-Read) Dung de trigger self-test (tu kiem tra) va cau hinh dai do gyro
//Self-test: XG_ST, YG_ST, ZG_ST - bit7, bit6, bit5: Dung de kiem tra xem gyro con hoat dong dung khong 
//FG_SEL[1:0]: Chon dai do (scale) cua gyro -> Dai cang lon, do duoc toc do quay nhanh hon nhung do phan giai (LSB/s) giam

#define MPU6050_ACCEL_CONFIG    0x1C //(Write-Read) Dung trigger self-test va cau hinh dai do cua accel. Thanh ghi nay cung dung de cau hih DHPF (Digital High Pass Filter)
//Self-test: XA_ST, YA_ST, ZA_ST - bit7, bit6, bit5: Dung de kiem tra xem accel con hoat dong dung khong 
//AFS_SEL[1:0]: Chon dai do (scale) cua accel -> Dai cang lon, do duoc toc do quay nhanh hon nhung do phan giai (LSB/s) giam


// ===== SELF TEST REGISTER =====

#define MPU6050_SELF_TEST_X     0x0D
#define MPU6050_SELF_TEST_Y     0x0E 
#define MPU6050_SELF_TEST_Z     0x0F
#define MPU6050_SELF_TEST_A     0x10


// ===== FIFO REGISTER =====

#define MPU6050_FIFO_DATA       0x74 //(Write-Read) Thanh ghi dung de doc-ghi data tu FIFO buffer [7:0]
#define MPU6050_FIFO_COUNT_H    0x72 //(Read-only) Thanh ghi de doc so sample hien co trong FIFO buffer (Cao) [15:8] - Doc thanh ghi nay truoc vi sample se update o day (cao->thap)
#define MPU6050_FIFO_COUNT_L    0x73 //(Read-only) Thanh ghi de doc so sample hien co trong FIFO buffer (thap) [7:0] - doc so sample tu FIFO_COUNT_H truoc
#define MPU6050_FIFO_ENABLE     0x23 //(Write-Read) Kich hoat (enable = 1) thi data tu data reigster cua cam bien se duoc day vao FIFO buffer (8-bit)
//Bit7: TEMP_FIFO_EN - Khi set = 1, cho phep ghi TEMP_OUT_H va TEMP_OUT_L vao FIFO buffer
//Bit6: XG_FIFO_EN - Khi set = 1, cho phep ghi GYRO_XOUT_H va GYRO_XOUT_L vao FIFO buffer
//Bit5: YG_FIFO_EN - Khi set = 1, cho phep ghi GYRO_YOUT_H va GYRO_YOUT_L vao FIFO buffer
//Bit4: ZG_FIFO_EN - Khi set = 1, cho phep ghi GYRO_ZOUT_H va GYRO_ZOUT_L vao FIFO buffer
//Bit3: ACCEL_FIFO_EN - Khi set = 1, cho phep ghi cac gia cua ACCEL vao FIFO buffer
//Bit2: SLV2_FIFO_EN
//Bit1: SLV1_FIFO_EN
//Bit0: SLV0_FIFO_EN


// ===== INTERRUPT REGISTER =====

#define MPU6050_INT_PIN_CFG     0x37
#define MPU6050_INT_ENABLE      0x38
#define MPU6050_INT_STATUS      0x3A


// ===== I2C PERIPHERAL REGISTER =====

#define MPU6050_I2C_MST_CTRL        0x24
#define MPU6050_I2C_MST_STATUS      0x36
#define MPU6050_I2C_MST_DELAY_CTRL  0x67

#define MPU6050_I2C_SLV0_ADDR     0x25
#define MPU6050_I2C_SLV0_REG      0x26
#define MPU6050_I2C_SLV0_CTRL     0x27
#define MPU6050_I2C_SLV0_DO       0x63

#define MPU6050_I2C_SLV1_ADDR     0x28
#define MPU6050_I2C_SLV1_REG      0x29
#define MPU6050_I2C_SLV1_CTRL     0x2A
#define MPU6050_I2C_SLV1_DO       0x64

#define MPU6050_I2C_SLV2_ADDR     0x2B
#define MPU6050_I2C_SLV2_REG      0x2C
#define MPU6050_I2C_SLV2_CTRL     0x2D
#define MPU6050_I2C_SLV2_DO       0x65

#define MPU6050_I2C_SLV3_ADDR     0x2E
#define MPU6050_I2C_SLV3_REG      0x2F
#define MPU6050_I2C_SLV3_CTRL     0x30
#define MPU6050_I2C_SLV3_DO       0x66

#define MPU6050_I2C_SLV4_ADDR     0x31
#define MPU6050_I2C_SLV4_REG      0x32
#define MPU6050_I2C_SLV4_DO       0x33
#define MPU6050_I2C_SLV4_CTRL     0x34
#define MPU6050_I2C_SLV4_DI       0x35



// ===== ACCEL & GYRO REGISTER =====

#define MPU6050_ACCEL_XOUT_H        0x3B //(Read-Only)[15:8] byte X cao
#define MPU6050_ACCEL_XOUT_L        0x3C //(Read_Only)[7:0] byte X thap 
#define MPU6050_ACCEL_YOUT_H        0x3D //(Read-Only)[15:8] byte Y cao
#define MPU6050_ACCEL_YOUT_L        0x3E //(Read-Only)[7:0] byte Y thap
#define MPU6050_ACCEL_ZOUT_H        0x3F //(Read-Only)[15:8] byte Z cao
#define MPU6050_ACCEL_ZOUT_L        0x40 //(Read-Only)[7:0] byte Z thap
//Dung de luu cac gia tri Accel - Du lieu cac thanh ghi nay duoc cap nhat o Sample Rate (thanh ghi 0x19)

#define MPU6050_TEMP_OUT_H          0x41 //(Read-Only)Thanh ghi luu byte cao gia tri nhiet do cam bien [15:8]
#define MPU6050_TEMP_OUT_L          0x42 //(Read-Only)Thanh ghi luu byte thap gia tri nhiet do cam bien [7:0]
//Ket hop 2 byte -> tao thanh gia tri 16-bit co dau goi la TEMP_OUT
//Du lieu 2 thanh ghi nay duoc cap nhat o Sample Rate (thanh ghi 0x19 MPU6050_SMPRT_DIV)
//Cong thuc: TEMP = (TEMP_OUT / 340 (he so scale factor)) + 36.53 (offset)

#define MPU6050_GYRO_XOUT_H        0x43
#define MPU6050_GYRO_XOUT_L        0x44
#define MPU6050_GYRO_YOUT_H        0x45
#define MPU6050_GYRO_YOUT_L        0x46
#define MPU6050_GYRO_ZOUT_H        0x47
#define MPU6050_GYRO_ZOUT_L        0x48
//Dung de luu cac gia tri Gyro - Du lieu cac thanh ghi nay duoc cap nhat o Sample Rate (thanh ghi 0x19)


// ===== MISCELLANEOUS REGISTER =====

#define MPU6055_SIGNAL_PATH_RESET  0x68
#define MPU6050_USER_CTRL          0x6A
#define MPU6050_PWR_MGMT_1         0x6B
#define MPU6050_PWR_MGMT_2         0x6C

// ====================== KHOI TAO ======================


/**
 * @brief 
 * 
 * @param
 * 
 * @return
 */
esp_err_t mpu6050_init();

// ====================== CHUC NANG ===================


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__INC_MPU6050_H