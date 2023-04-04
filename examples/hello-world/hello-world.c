#include "contiki.h"
#include "sys/etimer.h"
#include "sys/rtimer.h"
#include "net/ipv6/uip-ds6.h"
#include "net/ipv6/uip.h"
#include "net/ipv6/uip-icmp6.h"
#include "net/ipv6/uip-udp-packet.h"
#include "os/net/ipv6/uip-debug.h"
#include "net/routing/rpl-lite/rpl.h"
#include <stdio.h>
#include <stdint.h>
#include "uip.h"
#include "uip-udp-packet.h"

#define REBOOT_DELAY_SECONDS 5

PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);

#ifdef CLIENT_SEND_UDP_PACKETS
static struct uip_udp_conn *client_conn;
#endif

PROCESS_THREAD(hello_world_process, ev, data)
{
  static struct rtimer reboot_timer;
  static struct etimer timer;
  
  PROCESS_BEGIN();

  etimer_set(&timer, 2 * CLOCK_SECOND);

  while(1) {
    PROCESS_YIELD();

    if(ev == PROCESS_EVENT_TIMER && data == &timer) {

      #ifdef CLIENT_SEND_UDP_PACKETS

      // Create a UDP socket and send a packet to the rpl-border-router
      client_conn = udp_new(NULL, UIP_HTONS(0), NULL);
      // Schedule a reboot of the device if the UDP socket fails after a delay
      if(client_conn == NULL) {
          printf("===> ERROR: creating UDP socket <===\n ");
          rtimer_set(&reboot_timer, RTIMER_NOW() + (REBOOT_DELAY_SECONDS * RTIMER_SECOND), 1,
             (void (*)(struct rtimer *, void *))watchdog_reboot, NULL);
      }

      // Get the IP address of the RPL root node
      uip_ipaddr_t root_ipaddr;
      rpl_dag_t *dag = rpl_get_any_dag();

      if(dag) {
        printf("===> INFO: Got a DAG <=== \n");
        uip_ipaddr_copy(&root_ipaddr, &dag->dag_id);

        printf("#### IP address: ");
        uip_debug_ipaddr_print(&root_ipaddr);
        printf("#### \n");

        printf("++++ Sending packet to router ++++\n");
        udp_bind(client_conn, UIP_HTONS(0));

        // The payload to be sent in the custom echo packet
        char payload[] = "Hello, World!";
        int payload_len = sizeof(payload);
        uip_udp_packet_sendto(client_conn, payload, payload_len, &root_ipaddr, UIP_HTONS(UDP_PORT));
        
        uint8_t icmp_type = ICMP6_ECHO_REQUEST;
        uint8_t icmp_code = 0;

        // Send the ICMPv6 packet 
        uip_icmp6_send(&root_ipaddr, icmp_type, icmp_code, payload_len);

      } else {
        printf("!!!! Not in a DAG yet !!!!!\n");
      }

      #endif
      printf("Hello, world\n");

      etimer_reset(&timer);
    }
  }

  PROCESS_END();
} 