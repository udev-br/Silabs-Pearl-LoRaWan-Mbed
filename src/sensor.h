/*
 * sensor.h
 *
 *  Created on: 19 Jul 2019
 *      Author: uDEV - www.udev.com.br
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {

	bool		si7013_status;
	uint32_t	rhData;
	int32_t		tempData;

} st_sensor_data_t;

void sensorInit( void );
void sensorPerformMeasurements ( void );
st_sensor_data_t * sensorGetDataPointer ( void );

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_H_ */
