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

const u8_t Tx_message_1[] = {"Hola server"};
const u8_t Tx_message_2[] = {"Como estas?"};
const u8_t Tx_message_3[] = {"ITESO"};
const u8_t Tx_message_4[] = {"Sistemas"};

const u8_t Rx_message_1[] = {"Hola cliente"};
const u8_t Rx_message_2[] = {"Bien"};
const u8_t Rx_message_3[] = {"DESI"};
const u8_t Rx_message_4[] = {"Embebidos"};

static void start_encrypted_comm_client(void *arg);

static void start_encrypted_comm_server(void *arg);

int check_Tx_msg(char* msg);

void tcpclient_init(void);

#endif /* COMM_H_ */
