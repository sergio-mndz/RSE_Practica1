/*
 * comm.c
 *
 *  Created on: 19 sep 2021
 *      Author: sergio_mndz
 */

#include "comm.h"

static void tcp_connect(void *arg)
{
	struct netconn *conn, *newconn;
	err_t err;
	LWIP_UNUSED_ARG(arg);
	ip4_addr_t netif_ServerIP;
	struct netbuf *buf;
	u16_t len;

	IP4_ADDR(&netif_ServerIP, 148, 201, 186, 47);

	/* Create a new connection identifier. */
	/* Bind connection to well known port number 7. */
	#if LWIP_IPV6
	  conn = netconn_new(NETCONN_TCP_IPV6);
	  netconn_bind(conn, IP6_ADDR_ANY, 7);
	#else /* LWIP_IPV6 */
	  conn = netconn_new(NETCONN_TCP);
	//  netconn_bind(conn, IP_ADDR_ANY, 7);
	#endif /* LWIP_IPV6 */

	LWIP_ERROR("tcpecho: invalid conn", (conn != NULL), return;);
}


