/*
 * comm.c
 *
 *  Created on: 19 sep 2021
 *      Author: sergio_mndz
 */

#include "comm.h"

#define MAX_STRING_SIZE 16

u8_t Tx_message_1[] = "Hola server";
u8_t Tx_message_2[] = "Como estas?";
u8_t Tx_message_3[] = "ITESO";
u8_t Tx_message_4[] = "Sistemas";

u8_t Rx_message_1[] = "Hola cliente";
u8_t Rx_message_2[] = "Bien";
u8_t Rx_message_3[] = "DESI";
u8_t Rx_message_4[] = "Embebidos";

void stack_init(void *arg)
{
    static struct netif netif;
    ip4_addr_t netif_ipaddr, netif_netmask, netif_gw;
    ethernetif_config_t enet_config = {
        .phyHandle  = &phyHandle,
        .macAddress = configMAC_ADDR,
    };

    LWIP_UNUSED_ARG(arg);

    mdioHandle.resource.csrClock_Hz = EXAMPLE_CLOCK_FREQ;

    IP4_ADDR(&netif_ipaddr, configIP_ADDR0, configIP_ADDR1, configIP_ADDR2, configIP_ADDR3);
    IP4_ADDR(&netif_netmask, configNET_MASK0, configNET_MASK1, configNET_MASK2, configNET_MASK3);
    IP4_ADDR(&netif_gw, configGW_ADDR0, configGW_ADDR1, configGW_ADDR2, configGW_ADDR3);

    tcpip_init(NULL, NULL);

    netifapi_netif_add(&netif, &netif_ipaddr, &netif_netmask, &netif_gw, &enet_config, EXAMPLE_NETIF_INIT_FN,
                       tcpip_input);
    netifapi_netif_set_default(&netif);
    netifapi_netif_set_up(&netif);

    PRINTF("\r\n************************************************\r\n");
    PRINTF(" TCP Echo example\r\n");
    PRINTF("************************************************\r\n");
    PRINTF(" IPv4 Address     : %u.%u.%u.%u\r\n", ((u8_t *)&netif_ipaddr)[0], ((u8_t *)&netif_ipaddr)[1],
           ((u8_t *)&netif_ipaddr)[2], ((u8_t *)&netif_ipaddr)[3]);
    PRINTF(" IPv4 Subnet mask : %u.%u.%u.%u\r\n", ((u8_t *)&netif_netmask)[0], ((u8_t *)&netif_netmask)[1],
           ((u8_t *)&netif_netmask)[2], ((u8_t *)&netif_netmask)[3]);
    PRINTF(" IPv4 Gateway     : %u.%u.%u.%u\r\n", ((u8_t *)&netif_gw)[0], ((u8_t *)&netif_gw)[1],
           ((u8_t *)&netif_gw)[2], ((u8_t *)&netif_gw)[3]);
    PRINTF("************************************************\r\n");

    tcpclient_init();
    //tcpserver_init();

    vTaskDelete(NULL);
}


static void start_encrypted_comm_client(void *arg)
{
	struct netconn *conn, *newconn;
	err_t err;
	ip4_addr_t netif_ServerIP;
	LWIP_UNUSED_ARG(arg);
	u8_t Tx_msg[MAX_STRING_SIZE];
	u8_t *Tx_msg_encrypted;
	u16_t len;
	u8_t *Rx_msg;
	u8_t *Rx_msg_encrypted;
	struct netbuf *buf;
	int msg_id = MESSAGE_3;

	struct AES_ctx ctx;
	uint8_t key[] = "123 key";

	AES_init_ctx(&ctx, key);

	IP4_ADDR(&netif_ServerIP, 192, 168, 0, 105);

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
	while (1) {
		if(ERR_OK == (netconn_connect(conn, &netif_ServerIP, 7))){
			printf("*** Conexión exitosa ***\n");
			switch(msg_id)
			{
			case MESSAGE_1:
				strcpy(Tx_msg, Tx_message_1);
				break;
			case MESSAGE_2:
				strcpy(Tx_msg, Tx_message_2);
				break;
			case MESSAGE_3:
				strcpy(Tx_msg, Tx_message_3);
				break;
			case MESSAGE_4:
				strcpy(Tx_msg, Tx_message_4);
				break;
			default:
				strcpy(Tx_msg, Tx_message_1);
			}
			printf("Mensaje a enviar: %s\n", Tx_msg);

			Tx_msg_encrypted = encrypt_message(&ctx, Tx_msg);
			printf("Mensaje ecriptado a enviar: %s\n", Tx_msg_encrypted);
			(void)netconn_write(conn, (void*)Tx_msg_encrypted, strlen(Tx_msg_encrypted), NETCONN_COPY);
			printf("*** Mensaje enviado ***\n");
			while(ERR_OK != (netconn_recv(conn, &buf))){}
			do {
				  netbuf_data(buf, (void*)(&Rx_msg_encrypted), &len);
				  printf("Mensaje encriptado recibido: %s\n", (u8_t*)Rx_msg_encrypted);
				  Rx_msg = decrypt_message(&ctx, Rx_msg_encrypted);
				  printf("Mensaje recibido: %s\n", (u8_t*)Rx_msg);
			} while (netbuf_next(buf) >= 0);
			netconn_close(conn);
		} else {
//			printf("tcpecho: netconn_connect: error \"%s\"\n", lwip_strerr(err));
		}
	}
}

static void start_encrypted_comm_server(void *arg)
{
	struct netconn *conn, *newconn;
	err_t err;
	LWIP_UNUSED_ARG(arg);
	u8_t* Tx_msg;
	u8_t* Tx_msg_encrypted;
	u16_t len;
	u8_t Rx_msg[MAX_STRING_SIZE];
	u8_t* Rx_msg_encrypted;
	int Tx_msg_num;
	struct netbuf *buf;

	struct AES_ctx ctx;
	uint8_t key[] = "123 key";

	AES_init_ctx(&ctx, key);

	/* Create a new connection identifier. */
	/* Bind connection to well known port number 7. */
	#if LWIP_IPV6
	  conn = netconn_new(NETCONN_TCP_IPV6);
	  netconn_bind(conn, IP6_ADDR_ANY, 7);
	#else /* LWIP_IPV6 */
	  conn = netconn_new(NETCONN_TCP);
	  netconn_bind(conn, IP_ADDR_ANY, 7);
	#endif /* LWIP_IPV6 */

	LWIP_ERROR("tcpecho: invalid conn", (conn != NULL), return;);

	netconn_listen(conn);

	while(1)
	{
		err = netconn_accept(conn, &newconn);
		if (err == ERR_OK) {
			printf("Acepto una connexion\n");
			if(ERR_OK == (netconn_recv(newconn, &buf)))
			{
				do{
					netbuf_data(buf, (void *)(&Tx_msg_encrypted), &len);
					printf("Cadena encriptada recibida:\t%s\n", (u8_t*)Tx_msg_encrypted);
					Tx_msg = decrypt_message(&ctx, Tx_msg_encrypted);
					printf("Cadena desencriptada recibida: \t%s\n", (u8_t*)Tx_msg);
					Tx_msg_num = check_Tx_msg(Tx_msg);
					switch(Tx_msg_num)
					{
					case MESSAGE_1:
						strcpy(Rx_msg, Rx_message_1);
						break;
					case MESSAGE_2:
						strcpy(Rx_msg, Rx_message_2);
						break;
					case MESSAGE_3:
						strcpy(Rx_msg, Rx_message_3);
						break;
					case MESSAGE_4:
						strcpy(Rx_msg, Rx_message_4);
						break;
					default:
						strcpy(Rx_msg, Rx_message_1);
					}
					printf("Cadena a enviar:\t\t%s\n", (u8_t*)Rx_msg);
					Rx_msg_encrypted = encrypt_message(&ctx, Rx_msg);
					printf("Cadena encriptada a enviar:\t%s\n", (u8_t*)Rx_msg_encrypted);
					err = netconn_write(newconn, (void*)Rx_msg_encrypted, strlen(Rx_msg_encrypted), NETCONN_COPY);
					if (err != ERR_OK) {
						printf("tcpecho: netconn_write: error \"%s\"\n", lwip_strerr(err));
					}
				}while(netbuf_next(buf) >= 0);
				netbuf_delete(buf);
				netconn_close(newconn);
				netconn_delete(newconn);
			}
			else
			{
				printf("tcpecho: netconn_write: error \"%s\"\n", lwip_strerr(err));
			}
		}
		else{
			printf("tcpecho: accpet: error \"%s\"\n", lwip_strerr(err));
		}
	}
}

int check_Tx_msg(char* msg){
	if(strcmp(msg, Tx_message_1)==0)
	{
		return MESSAGE_1;
	}
	else if(strcmp(msg, Tx_message_2)==0)
	{
		return MESSAGE_2;
	}
	else if(strcmp(msg, Tx_message_3)==0)
	{
		return MESSAGE_3;
	}
	else if(strcmp(msg, Tx_message_4)==0)
	{
		return MESSAGE_4;
	}
	else
	{
		return MESSAGE_1;
	}
}

void tcpclient_init(void)
{
	sys_thread_new("start_encrypted_comm_client", start_encrypted_comm_client, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
}

void tcpserver_init(void)
{
	sys_thread_new("start_encrypted_comm_server", start_encrypted_comm_server, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
}
