/*
 * comm.h
 *
 *  Created on: 19 sep 2021
 *      Author: sergio_mndz
 */

#ifndef COMM_H_
#define COMM_H_

#include "aes.h"

#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"

#include "stdio.h"

static void tcp_connect(void *arg);

void client_write(int message_id);

void client_recieve();

#endif /* COMM_H_ */
