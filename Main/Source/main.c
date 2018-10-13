/*
 */
#include <string.h>         // C 標準ライブラリ用
#include <AppHardwareApi.h>
#include "utils.h"
#include "ToCoNet.h"
#include "serial.h"         // シリアル用
#include "sprintf.h"        // SPRINTF 用
#include "SMBus.h"
#include "adxl345.h"

/*
    パソコン側のシリアル通信の設定（デバッグ用）
    ボーレート      115200
    データ          ８ビット
    パリティ        None(無し)
    ストップビット   1ビット
    フローコントロール None(無し)
*/
#define UART_BAUD       115200      // シリアルのボーレート

static tsFILE sSerStream;           // シリアル用ストリーム
static tsSerialPortSetup sSerPort;  // シリアルポートデスクリプタ

// デバッグメッセージ出力用
#define debug(...) vfPrintf(&sSerStream, LB __VA_ARGS__)

// デバッグ出力用に UART を初期化
static void vSerialInit() {
    static uint8 au8SerialTxBuffer[96];
    static uint8 au8SerialRxBuffer[32];

    sSerPort.pu8SerialRxQueueBuffer = au8SerialRxBuffer;
    sSerPort.pu8SerialTxQueueBuffer = au8SerialTxBuffer;
    sSerPort.u32BaudRate = UART_BAUD;
    sSerPort.u16AHI_UART_RTS_LOW = 0xffff;
    sSerPort.u16AHI_UART_RTS_HIGH = 0xffff;
    sSerPort.u16SerialRxQueueSize = sizeof(au8SerialRxBuffer);
    sSerPort.u16SerialTxQueueSize = sizeof(au8SerialTxBuffer);
    sSerPort.u8SerialPort = E_AHI_UART_0;
    sSerPort.u8RX_FIFO_LEVEL = E_AHI_UART_FIFO_LEVEL_1;
    SERIAL_vInit(&sSerPort);

    sSerStream.bPutChar = SERIAL_bTxChar;
    sSerStream.u8Device = E_AHI_UART_0;
}

// ハードウェア初期化
static void vInitHardware()
{
	// デバッグ出力用
	vSerialInit();
	ToCoNet_vDebugInit(&sSerStream);
	ToCoNet_vDebugLevel(0);

    // IOポート初期化
    //vPortAsOutput(PWR_PIN);
    //vPortSetLo(PWR_PIN);

    // I2C初期化
    vSMBusInit(I2C_CLOCK_100KHZ);
}


// ユーザ定義のイベントハンドラ
static void vProcessEvCore(tsEvent *pEv, teEvent eEvent, uint32 u32evarg)
{
    static int count = 0;

    switch (pEv->eState) {
    // アイドル状態
    case E_STATE_IDLE:
        if (eEvent == E_EVENT_START_UP) { // 起動時
            debug("** ADXL345 Test **");

            if(!ADXL345_Raedy()) {
                debug("ADXL345 not connnected.");

            } else {

                //測定レンジを設定
                //gに設定可能な値は2,4,8,16のいずれか
                ADXL345_SetRange(16);

                //モードを設定
                ADXL345_SetMode(ADXL345_MEASURE);

                // 測定モードへ移行
                ToCoNet_Event_SetState(pEv, E_STATE_RUNNING);
            }
        }
        break;

    // 稼働状態
    case E_STATE_RUNNING:
        if (eEvent == E_EVENT_TICK_TIMER) {
            if (++count > 200) {
                int16_t x,y,z;
                if(!ADXL345_GetValue(&x, &y, &z)) {
                    debug("ADXL345 measuring error.");
                } else {
                    debug("X:%d Y:%d Z:%d", (int)x, (int)y, (int)z);
                }
                count = 0;
            }
        }
        break;

    default:
        break;
    }
}

// メインループ無限ループではなく割り込みなどの発生を起点として呼び出されます
void cbToCoNet_vMain(void)
{
}

// パケット送信完了時
void cbToCoNet_vTxEvent(uint8 u8CbId, uint8 bStatus)
{
}

// パケット受信時
void cbToCoNet_vRxEvent(tsRxDataApp *pRx)
{
}

// 電源オンによるスタート
void cbAppColdStart(bool_t bAfterAhiInit)
{
	if (!bAfterAhiInit) {
	} else {
        // SPRINTF 初期化
        SPRINTF_vInit128();

        // ユーザ定義のイベントハンドラを登録
        ToCoNet_Event_Register_State_Machine(vProcessEvCore);

        // ハードウェア初期化
        vInitHardware();
	}
}

// スリープからの復帰
void cbAppWarmStart(bool_t bAfterAhiInit)
{
}

// ネットワークイベント発生時
void cbToCoNet_vNwkEvent(teEvent eEvent, uint32 u32arg)
{
}


// ハードウェア割り込み発生後（遅延呼び出し）
void cbToCoNet_vHwEvent(uint32 u32DeviceId, uint32 u32ItemBitmap)
{
}

// ハードウェア割り込み発生時
uint8 cbToCoNet_u8HwInt(uint32 u32DeviceId, uint32 u32ItemBitmap)
{
	return FALSE;
}
