#ifndef PROJECT_CONF_H_

//LOGGING
#define LOG_CONF_LEVEL_RPL LOG_LEVEL_DBG
#define LOG_CONF_LEVEL_TCPIP LOG_LEVEL_WARN
#define LOG_CONF_LEVEL_IPV6 LOG_LEVEL_WARN
#define LOG_CONF_LEVEL_6LOWPAN LOG_LEVEL_DBG
#define LOG_CONF_LEVEL_MAC LOG_LEVEL_DBG
#define LOG_CONF_LEVEL_FRAMER LOG_LEVEL_INFO
#define TSCH_LOG_CONF_PER_SLOT 0

//CHANNEL CONFIGURATION
#define PROJECT_CONF_H_
#undef IEEE802154_CONF_DEFAULT_CHANNEL
#define IEEE802154_CONF_DEFAULT_CHANNEL 16
#undef IEEE802154_CONF_PANID
#define IEEE802154_CONF_PANID 0xABD2


//CLIENT CONFIGURATION
#define CLIENT_SEND_UDP_PACKETS
#define UDP_PORT 3001

//MAC CONFIGURATION
//simple node 1 macc addr: 0012.4b00.1204.cda5
//#define INTERMEDIATE_NODE_MAC_ADDRESS { 0, 18, 75, 0, 18, 4, 205, 165}

//SFA ATTACK
//#define ENABLE_SELECTIVE_FORWARD_ATTACK true

//SECURITY
//#define LLSEC802154_CONF_ENABLED 1
//#define CSMA_CONF_LLSEC_SECURITY_LEVEL 5
//#define CSMA_CONF_LLSEC_KEY_ID_MODE 2

#endif