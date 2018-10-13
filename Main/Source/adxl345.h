
#ifndef  ADXL345_H_INCLUDED
#define  ADXL345_INCLUDED

// BME280アドレス関連
#define ADXL345_ADDRESS         (0x53)      // I2Cアドレス SDOﾋﾟﾝ=>GND
#define ADXL345_ID_REG          (0x00)      // ID
#define ADXL345_BW_RATE_REG     (0x2C)      // BAND WIDTH RATE(データレート)
#define ADXL345_POWER_CTL_REG   (0x2D)      // POWER_TCL(節電機能の制御)
#define ADXL345_DATA_FORMAT_REG (0x31)      // データフォーマット
#define ADXL345_DATA_X0         (0x32)      // データの先頭(全6ﾊﾞｲﾄ)
#define ADXL345_FIFO_CTL_REG    (0x38)      // FIFO_CTL(FIFOの制御)

//
#define ADXL345_ID              (0xE5)      // ID (ADXL345ならこの値)

// POWER_TCL(節電機能の制御)
#define ADXL345_PWRCTL_MEASURE  (0x08)      // 測定モード⇔スタンバイモード
#define ADXL345_PWRCTL_SLEEP    (0x04)      // スリープモード
#define ADXL345_PWRCTL_WAKEUP_1HZ (0x03)    // スリープ中のデータ読み出し周波数(ようわからん)
#define ADXL345_PWRCTL_WAKEUP_2HZ (0x02)    //
#define ADXL345_PWRCTL_WAKEUP_4HZ (0x01)    //
#define ADXL345_PWRCTL_WAKEUP_8HZ (0x00)    //
// データフォーマット
#define ADXL345_FORMAT_FULL_RES (0x08)      // 最大分解能モード(13bit?)⇔10bit
#define ADXL345_FORMAT_RANGE_2G (0x00)      // ±2g
#define ADXL345_FORMAT_RANGE_4G (0x01)      // ±4g
#define ADXL345_FORMAT_RANGE_8G (0x02)      // ±8g
#define ADXL345_FORMAT_RANGE_16G (0x03)     // ±16g
// FIFO_CTL(FIFOの制御)
#define ADXL345_FIFOCTL_FIFO_BYPASS  (0x00) // FIFOバイパス
#define ADXL345_FIFOCTL_FIFO_FIFO    (0x40) // FIFOが満杯になるまで取得、満杯でないとき取得
#define ADXL345_FIFOCTL_FIFO_STREAM  (0x80) // FIFOは最新の32個を保持
#define ADXL345_FIFOCTL_FIFO_TRIGGER (0xC0) // TRUGGERビットで開始し、FIFOが満杯になるまで取得
#define ADXL345_FIFOCTL_TRIGGER      (0x20) // TRUGGERビット
//データレート
#define ADXL345_DATA_RATE_800HZ (0x0d)      // 800Hz
#define ADXL345_DATA_RATE_400HZ (0x0c)      // 400Hz
#define ADXL345_DATA_RATE_200HZ (0x0b)      // 200Hz
#define ADXL345_DATA_RATE_100HZ (0x0a)      // 100Hz
#define ADXL345_DATA_RATE_50HZ  (0x09)      // 50Hz
#define ADXL345_DATA_RATE_25HZ  (0x08)      // 25Hz

typedef enum {
    ADXL345_SLEEP,
    ADXL345_STANDBY,
    ADXL345_MEASURE
} ADXL345MODE;


#include "SMBus.h"

//IDを取得しADXL345のものかチェック
bool ADXL345_Raedy();

//測定レンジを設定
//gに設定可能な値は2,4,8,16のいずれか
bool ADXL345_SetRange(uint8 g);

//データレートを設定
//ADXL345_DATA_RATE_xxxHZを渡す
//lowPower=TRUEでLOW_POWERモードになり、400Hz以下で低消費電力になるものの、
//ノイズが若干増大します。
bool ADXL345_SetDataRate(uint8 dataRate, bool_t lowPower = FALSE);

//モードを設定
bool ADXL345_SetMode(ADXL345MODE mode);

//測定開始
bool ADXL345_GetValue(int16_t *x,int16_t *y,int16_t *z);


#endif  /* ADXL345_H_INCLUDED */
