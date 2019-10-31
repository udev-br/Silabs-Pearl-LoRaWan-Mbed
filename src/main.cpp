/***************************************************************************//**
 * @file
 * @brief Demo app for loraWan stack from mbed-os
 *
 ******************************************************************************/

#include <mbed.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_gpio.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "em_pcnt.h"
#include "rtcdriver.h"

#include "bsp.h"
#include "Event.h"
#include "LoRaWANInterface.h"
#include "LoRaRadio.h"
#include "../mbed-semtech-lora-rf-drivers/SX1272/SX1272_LoRaRadio.h"
#include "../mbed-semtech-lora-rf-drivers/SX126X/SX126X_LoRaRadio.h"
#include "us_ticker_api.h"
#include "retargetserial.h"
#include "sensor.h"
#include "loraWan.h"

/***************************************************************************//**
 * Local defines
 ******************************************************************************/

/** Time (in ms) between periodic updates of rtc. */
#define PERIODIC_RTC_UPDATE_MS      1000

/***************************************************************************//**
 * Local variables
 ******************************************************************************/

/** Timer used for periodic update of the measurements. */
RTCDRV_TimerID_t periodicUpdateTimerId;

int txCounter = 0;
int runTimeCounter = 0;

/***************************************************************************//**
 * Local prototypes
 ******************************************************************************/

static void BoardSetup ( void );
static void PrepareAndSendPayload ( loraWan & loraWan );
static void periodicUpdateCallback (RTCDRV_TimerID_t id, void *user);

/***************************************************************************//**
 * @brief  Main function
 ******************************************************************************/

int main(void) {

  /* Chip errata */
	CHIP_Init();

	BoardSetup();

	sensorInit();

#if 0
	SX1272_LoRaRadio radio ( SX127X_SPI_MOSI,
							 SX127X_SPI_MISO,
							 SX127X_SPI_CLK,
							 SX127X_SPI_CS,
							 SX127X_RESET,
							 SX127X_SPI_DIO0,
							 SX127X_SPI_DIO1,
							 SX127X_SPI_DIO2,
							 SX127X_SPI_DIO3,
							 SX127X_SPI_DIO4,
							 SX127X_SPI_DIO5 );
#endif

#if 1
	SX126X_LoRaRadio radio( SX126X_SPI_MOSI,
							SX126X_SPI_MISO,
							SX126X_SPI_CLK,
							SX126X_SPI_CS,
							SX126X_RESET,
							SX126X_DIO1,
							SX126X_BUSY,
							SX126X_FREQ_SELECT,
							SX126X_DEVICE_SELECT,
							SX126X_CRYSTAL_SELECT,
							SX126X_ANT_SWITCH );
#endif

	events::EventQueue ev_queue;

	LoRaWANInterface lorawanInterface( radio );

	lorawanInterface.initialize(&ev_queue);

	loraWan loraWan( ev_queue, radio, lorawanInterface );

	/* Config stdio to Virtual COM port through USB */
	RETARGET_SerialInit();
	RETARGET_SerialCrLf(1);

	while(1) {

		ev_queue.dispatch( 10 );

	    if ( txCounter >= 10 ) {

	    	txCounter = 0;

	    	PrepareAndSendPayload( loraWan );

	    }

	}

}


static void BoardSetup ( void )
{

	EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_STK_DEFAULT;
	CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_STK_DEFAULT;

	/* Init DCDC regulator and HFXO with kit specific parameters */
	EMU_DCDCInit(&dcdcInit);
	CMU_HFXOInit(&hfxoInit);

	/* Switch HFCLK to HFXO and disable HFRCO */
	CMU_ClockSelectSet((CMU_Clock_TypeDef)cmuClock_HF, (CMU_Select_TypeDef)cmuSelect_HFXO);
	CMU_OscillatorEnable((CMU_Osc_TypeDef)cmuOsc_HFRCO, false, false);

	/* Enable GPIO clock */
	CMU_ClockEnable((CMU_Clock_TypeDef)cmuClock_GPIO, true);

	/* Initalize other peripherals and drivers */
	RTCDRV_Init();

	/* Set up periodic update of the display. */
	RTCDRV_AllocateTimer(&periodicUpdateTimerId);
	RTCDRV_StartTimer(periodicUpdateTimerId, rtcdrvTimerTypePeriodic,
					PERIODIC_RTC_UPDATE_MS, periodicUpdateCallback, 0);

	/* Initialize LED driver */
	BSP_LedsInit();
	BSP_LedClear(0);
	BSP_LedClear(1);

}


static void PrepareAndSendPayload ( loraWan & loraWan ) {

	uint8_t lorawanPayload[8];

	sensorPerformMeasurements();

	memcpy( &lorawanPayload[0], (const void*)&sensorGetDataPointer()->tempData, 4 );
	memcpy( &lorawanPayload[4], (const void*)&sensorGetDataPointer()->rhData, 4 );

	loraWan.send( 1, lorawanPayload, 8, MSG_CONFIRMED_FLAG );

// Use the following code to decode the packet on TTN - Payload Formats
//
//	function Decoder(bytes, port) {
//
//	    var result = {tempC: "", rH: ""};
//
//	    result.tempC = ((bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0])/1000.0;
//
//	    result.rH = ((bytes[7] << 24) | (bytes[6] << 16) | (bytes[5] << 8) | bytes[4])/1000.0;
//
//	    return result;
//
//	}

}


/***************************************************************************//**
 * @brief Callback used to count between measurement updates
 ******************************************************************************/

static void periodicUpdateCallback(RTCDRV_TimerID_t id, void *user)
{
  (void) id;
  (void) user;

  txCounter++;

  BSP_LedToggle(0);

}
