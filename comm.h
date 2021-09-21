/*
 * comm.h
 *
 *  Created on: 19 sep 2021
 *      Author: sergio_mndz
 */

#ifndef COMM_H_
#define COMM_H_

#include "encryption.h"

#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"

#include "stdio.h"
#include "string.h"

#define MESSAGE_1 1
#define MESSAGE_2 2
#define MESSAGE_3 3
#define MESSAGE_4 4

u8_t Tx_message_1[] = {"Hola server"};
u8_t Tx_message_2[] = {"Como estas?"};
u8_t Tx_message_3[] = {"ITESO"};
u8_t Tx_message_4[] = {"Sistemas"};

u8_t Rx_message_1[] = {""};
u8_t Rx_message_2[] = {""};
u8_t Rx_message_3[] = {""};
u8_t Rx_message_4[] = {""};

static void start_encrypted_comm_client(void *arg);

static void start_encrypted_comm_server(void *arg);

void tcpclient_init(void);

#endif /* COMM_H_ */
