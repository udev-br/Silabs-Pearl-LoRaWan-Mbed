/*
 * loraWan.cpp
 *
 *  Created on: 21 Jul 2019
 *      Author: uDEV - www.udev.com.br
 */

#include <loraWan.h>

#include "Event.h"
#include "LoRaWANInterface.h"
#include "LoRaRadio.h"


uint8_t  NwkSKey[16] = MBED_CONF_LORA_NWKSKEY;
uint8_t  AppSKey[16] = MBED_CONF_LORA_APPSKEY;
uint8_t  NwkKey[16]  = MBED_CONF_LORA_NETWORK_KEY;
uint8_t  AppKey[16]  = MBED_CONF_LORA_APPLICATION_KEY;


loraWan::loraWan( events::EventQueue & queue, LoRaRadio & radio, LoRaWANInterface & lorawanInterface ):
		m_eventQueue( queue ),
		m_radio( radio ),
		m_lorawanInterface ( lorawanInterface )
{

	//
	// Important!! Don't forget to modify the preprocessor symbols
	//

	m_lorawanAppCallbacks.events          = mbed::callback( loraWanEventsCallback );
	m_lorawanAppCallbacks.link_check_resp = mbed::callback( loraWanCheckResponseCallback );
	m_lorawanAppCallbacks.battery_level   = NULL;

	m_lorawanInterface.add_app_callbacks( &m_lorawanAppCallbacks );

#if ( MBED_CONF_LORA_OVER_THE_AIR_ACTIVATION == 0 )

	m_connection.connect_type = LORAWAN_CONNECTION_ABP;
	m_connection.connection_u.abp.nwk_id = 1;
	m_connection.connection_u.abp.dev_addr = MBED_CONF_LORA_DEVICE_ADDRESS;
	m_connection.connection_u.abp.app_skey = AppSKey;
	m_connection.connection_u.abp.nwk_skey = NwkSKey;
	m_connection.connection_u.abp.nwk_senckey = NwkSKey;
	m_connection.connection_u.abp.snwk_sintkey = NwkSKey;

#elif ( MBED_CONF_LORA_OVER_THE_AIR_ACTIVATION == 1 )

	m_connection.connect_type = LORAWAN_CONNECTION_OTAA;
	m_connection.connection_u.abp.nwk_id = 1;
	m_connection.connection_u.abp.dev_addr = MBED_CONF_LORA_DEVICE_ADDRESS;
	m_connection.connection_u.abp.app_skey = AppSKey;
	m_connection.connection_u.abp.nwk_skey = NwkKey;
	m_connection.connection_u.abp.nwk_senckey = AppSKey;
	m_connection.connection_u.abp.snwk_sintkey = AppSKey;

#endif

	m_lorawanInterface.disable_adaptive_datarate();

	m_lorawanInterface.set_datarate( DR_2 );

	m_lorawanInterface.set_device_class( CLASS_A );

	m_lorawanInterface.connect( m_connection );

}

loraWan::~loraWan()
{

}

int16_t
loraWan::send ( uint8_t port, const uint8_t *data, uint16_t length, int flags )
{

	return m_lorawanInterface.send( port, data, length, flags );

}

const char * ac_lorawan_event[] = {

   "CONNECTED\r\n",
   "DISCONNECTED\r\n",
   "TX_DONE\r\n",
   "TX_TIMEOUT\r\n",
   "TX_ERROR\r\n",
   "CRYPTO_ERROR\r\n",
   "TX_SCHEDULING_ERROR\r\n",
   "RX_DONE\r\n",
   "RX_TIMEOUT\r\n",
   "RX_ERROR\r\n",
   "JOIN_FAILURE\r\n",
   "UPLINK_REQUIRED\r\n",
   "AUTOMATIC_UPLINK_ERROR\r\n",
   "CLASS_CHANGED\r\n",							//only in Lorawan 1.1 (ch 18.1)
   "SERVER_ACCEPTED_CLASS_IN_USE\r\n",			//only in Lorawan 1.1 (ch 18.1)
   "SERVER_DOES_NOT_SUPPORT_CLASS_IN_USE\r\n",	//only in Lorawan 1.1 (ch 18.1)
   "DEVICE_TIME_SYNCHED\r\n", 					//only in LoRaWAN v1.0.3 and v1.1.x

};


void
loraWan::loraWanEventsCallback ( lorawan_event_t event )
{

	// Treat Events Callback here

   if ( event < ( DEVICE_TIME_SYNCHED ) ) {
	  if ( ac_lorawan_event[event] != NULL )
	  printf("%s", ac_lorawan_event[event] );
   }

	switch (event) {
		case CONNECTED:
			break;
		case DISCONNECTED:
			break;
		case TX_DONE:
			break;
		case TX_TIMEOUT:
			break;
		case TX_ERROR:
			break;
		case CRYPTO_ERROR:
			break;
		case TX_SCHEDULING_ERROR:
			break;
		case RX_DONE:
			break;
		case RX_TIMEOUT:
			break;
		case RX_ERROR:
			break;
		case JOIN_FAILURE:
			break;
		case UPLINK_REQUIRED:
			break;
		case AUTOMATIC_UPLINK_ERROR:
			break;
		case CLASS_CHANGED: //only in Lorawan 1.1 (ch 18.1)
			break;
		case SERVER_ACCEPTED_CLASS_IN_USE: //only in Lorawan 1.1 (ch 18.1)
			break;
		case SERVER_DOES_NOT_SUPPORT_CLASS_IN_USE: //only in Lorawan 1.1 (ch 18.1)
			break;
		case DEVICE_TIME_SYNCHED: // only in LoRaWAN v1.0.3 and v1.1.x
			break;
		default:
			break;
    }

}

void
loraWan::loraWanCheckResponseCallback(uint8_t demod_margin, uint8_t num_gw)
{

// Treat Check Response here

}
