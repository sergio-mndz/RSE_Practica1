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

static void init_encrypted_comm()
{
	struct AES_ctx ctx;
	uint8_t key[] = "123 key";
	tcp_connect(NULL);
	AES_setup(&ctx, key);
}

void client_write(int message_id)
{
	struct netconn *conn;
	u8_t *Tx_msg;
	u8_t *Tx_msg_encrypted;
	u16_t len;
	u8_t *Rx_msg;
	struct netbuf *buf;

	ip4_addr_t netif_ServerIP;
	IP4_ADDR(&netif_ServerIP, 148, 201, 186, 47);
	conn = netconn_new(NETCONN_TCP);

	switch(message_id)
	{
	case MESSAGE_1:
		strcpy(Tx_msg, message_1);
		break;
	case MESSAGE_2:
		strcpy(Tx_msg, message_2);
		break;
	case MESSAGE_3:
		strcpy(Tx_msg, message_3);
		break;
	case MESSAGE_4:
		strcpy(Tx_msg, message_4);
		break;
	default:
		strcpy(Tx_msg, message_1);
	}

	&Rx_msg = encrypt_message();

	if(ERR_OK == (netconn_connect(conn, &netif_ServerIP, 7))){
		(void)netconn_write(conn, (void*)Tx_msg, strlen(Tx_msg), NETCONN_COPY);
		while(ERR_OK != (netconn_recv(conn, &buf))){}
		do {
			netbuf_data(buf, (void*)Rx_msg, &len);
			printf("%s", (u8_t*)Rx_msg);
		} while (netbuf_next(buf) >= 0);
		netconn_close(conn);
	} else {
		//Nothing
	}
}
