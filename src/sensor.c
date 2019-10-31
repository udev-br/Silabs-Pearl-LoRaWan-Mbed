/*
 * sensor.c
 *
 *  Created on: 19 Jul 2019
 *      Author: uDEV - www.udev.com.br
 */


#include <stddef.h>
#include "em_device.h"
#include "em_gpio.h"
#include "cpt112s_config.h"
#include "i2cspm.h"
#include "si7013.h"

#include "sensor.h"

I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;

st_sensor_data_t st_sensor_data;

void sensorInit( void )
{

	/* Enable Si7021 sensor isolation switch */
	GPIO_PinModeSet(CS0_SENSOR_EN_PORT, CS0_SENSOR_EN_PIN, gpioModePushPull, 1);

	/* Initialize I2C driver, using standard rate. */
	I2CSPM_Init(&i2cInit);

	/* Checks if a Si7013 is present on the I2C bus or not. */
	st_sensor_data.si7013_status = Si7013_Detect(i2cInit.port, SI7021_ADDR, NULL);

}

void sensorPerformMeasurements ( void )
{

	Si7013_MeasureRHAndTemp(i2cInit.port, SI7021_ADDR, &st_sensor_data.rhData, &st_sensor_data.tempData);

}

st_sensor_data_t * sensorGetDataPointer ( void )
{
	return &st_sensor_data;

}
