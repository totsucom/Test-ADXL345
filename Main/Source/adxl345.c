#include "adxl345.h"
#include "SMBus.h"

//IDを取得しADXL345のものかチェック
bool ADXL345_Raedy()
{
    uint8 reg;

    if(!bSMBusWrite(ADXL345_ADDRESS, ADXL345_ID_REG, 0, NULL)) return FALSE;
    if(!bSMBusSequentialRead(ADXL345_ADDRESS, 1, &reg)) return FALSE;

    return (reg == ADXL345_ID);
}

//測定レンジを設定
//gに設定可能な値は2,4,8,16のいずれか
bool ADXL345_SetRange(uint8 g)
{
    uint8 reg = ADXL345_FORMAT_FULL_RES;
    switch(g) {
        case 2: reg |= ADXL345_FORMAT_RANGE_2G; break;
        case 4: reg |= ADXL345_FORMAT_RANGE_4G; break;
        case 8: reg |= ADXL345_FORMAT_RANGE_8G; break;
        case 16: reg |= ADXL345_FORMAT_RANGE_16G; break;
        default: return FALSE;
    }
    return bSMBusWrite(ADXL345_ADDRESS, ADXL345_DATA_FORMAT_REG, 1, &reg);
}

//データレートを設定
//ADXL345_DATA_RATE_xxxHZを渡す
//lowPower=TRUEでLOW_POWERモードになり、400Hz以下で低消費電力になるものの、
//ノイズが若干増大します。
bool ADXL345_SetDataRate(uint8 dataRate, bool_t lowPower)
{
    uint8 reg = dataRate;
    if(lowPower) reg |= 16;
    return bSMBusWrite(ADXL345_ADDRESS, ADXL345_BW_RATE_REG, 1, &reg);
}

//モードを設定
bool ADXL345_SetMode(ADXL345MODE mode)
{
    uint8 reg = ADXL345_PWRCTL_WAKEUP_8HZ; //ようわからん
    switch(mode) {
        case ADXL345_SLEEP: reg |= ADXL345_PWRCTL_SLEEP; break;
        case ADXL345_MEASURE: reg |= ADXL345_PWRCTL_MEASURE; break;
        case ADXL345_STANDBY:  break;
        default: return FALSE;
    }
    return bSMBusWrite(ADXL345_ADDRESS, ADXL345_POWER_CTL_REG, 1, &reg);
}

//測定開始
bool ADXL345_GetValue(int16_t *x,int16_t *y,int16_t *z)
{
    uint8 reg[6];
    if(!bSMBusWrite(ADXL345_ADDRESS, ADXL345_DATA_X0, 0, NULL)) return FALSE;
    if(!bSMBusSequentialRead(ADXL345_ADDRESS, 6, reg)) return FALSE;

    // データを各XYZの値に変換する(LSB単位)
    *x = (((int16_t)reg[1]) << 8) | reg[0];
    *y = (((int16_t)reg[3]) << 8) | reg[2];
    *z = (((int16_t)reg[5]) << 8) | reg[4];
    return TRUE;
}

