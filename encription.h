/*
 * encription.h
 *
 *  Created on: 18 sep 2021
 *      Author: sergio_mndz
 */

#ifndef RSE_PRACTICA1_ENCRIPTION_H_
#define RSE_PRACTICA1_ENCRIPTION_H_

#include "aes.h"
#include "tcpecho.h"
#include "lwip/netifapi.h"
#include "lwip/tcpip.h"
#include "netif/ethernet.h"
#include "enet_ethernetif.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_phy.h"

#include "fsl_phyksz8081.h"
#include "fsl_enet_mdio.h"
#include "fsl_device_registers.h"

#include <stdio.h>

void AES_setup(struct AES_ctx* ctx, const uint8_t* key);

const char* encrypt_message(struct AES_ctx* ctx, uint8_t* buf);

const char* decrypt_message(struct AES_ctx* ctx, uint8_t* buf);

#endif /* RSE_PRACTICA1_ENCRIPTION_H_ */
