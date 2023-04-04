#include "contiki.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip-udp-packet.h"

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "RPL BR"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Declare and auto-start this file's process */
PROCESS(contiki_ng_br, "Contiki-NG Border Router");
AUTOSTART_PROCESSES(&contiki_ng_br);

static struct uip_udp_conn *udp_conn1;
static struct uip_udp_conn *udp_conn2;

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(contiki_ng_br, ev, data)
{
  PROCESS_BEGIN();

#if BORDER_ROUTER_CONF_WEBSERVER
  PROCESS_NAME(webserver_nogui_process);
  process_start(&webserver_nogui_process, NULL);
#endif /* BORDER_ROUTER_CONF_WEBSERVER */

  LOG_INFO("Contiki-NG Border Router started\n");

  /* Create a new UDP connection */
  udp_conn1 = udp_new(NULL, UIP_HTONS(UDP_SOURCE_1), NULL);
  udp_conn2 = udp_new(NULL, UIP_HTONS(UDP_SOURCE_2), NULL);
  udp_bind(udp_conn1, UIP_HTONS(UDP_PORT_1));
  udp_bind(udp_conn2, UIP_HTONS(UDP_PORT_2));

  while (1)
  {
    PROCESS_WAIT_EVENT();

    if (ev == tcpip_event)
    {
      printf("--> ENTERING TCP EVENT <--\n");
      if (uip_newdata())
      {
        printf("--> ENTERTING NEW DATA UIP <---\n");
        
        //Check the source port of the incoming packet
        uint16_t src_port = UIP_HTONS(UIP_UDP_BUF->srcport);

        //Log who connected
        if (src_port == UDP_SOURCE_1) {
            printf("++++++++ Received data from NODE1: %.*s\n", uip_datalen(), (char *)uip_appdata);
        } else if (src_port == UDP_SOURCE_2) {
            printf("-------- Received data from NODE2: %.*s\n", uip_datalen(), (char *)uip_appdata);


        }
      }
    }
  }

  PROCESS_END();
}