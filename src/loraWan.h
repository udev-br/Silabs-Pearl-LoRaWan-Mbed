/*
 * loraWan.h
 *
 *  Created on: 21 Jul 2019
 *      Author: uDEV - www.udev.com.br
 */

#ifndef LORAWAN_H_
#define LORAWAN_H_

#include <mbed.h>
#include "LoRaWANInterface.h"
#include "LoRaRadio.h"
#include "us_ticker_api.h"
#include "Event.h"
#include "EventQueue.h"


class loraWan
{

public:
	loraWan( events::EventQueue & queue, LoRaRadio & radio, LoRaWANInterface & m_lorawanInterface );
	virtual ~loraWan();

	int16_t send ( uint8_t port, const uint8_t *data, uint16_t length, int flags );

private:
	static void loraWanEventsCallback ( lorawan_event_t event );
	static void loraWanCheckResponseCallback( uint8_t demod_margin, uint8_t num_gw );

private:
	events::EventQueue & m_eventQueue;

	LoRaRadio & m_radio;

	LoRaWANInterface & m_lorawanInterface;

	lorawan_app_callbacks_t m_lorawanAppCallbacks;

	lorawan_connect_t m_connection;

};

#endif /* LORAWAN_H_ */
