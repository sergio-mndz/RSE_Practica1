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

u8_t message_1[] = {"Hola server"};
u8_t message_2[] = {"Como estas?"};
u8_t message_3[] = {"ITESO"};
u8_t message_4[] = {"Sistemas"};

static void start_encrypted_comm_client(int msg_id);

#endif /* COMM_H_ */
