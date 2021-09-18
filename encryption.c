/*
 * encryption.c
 *
 *  Created on: 18 sep 2021
 *      Author: sergio_mndz
 */

#include <RSE_Practica1/encryption.h>

//TODO: Definir si el contexto se declarará en la capa de aplicacion o
//		en este archivo, seguramente en la de aplicación
//struct AES_ctx ctx;

void AES_setup(struct AES_ctx* ctx, const uint8_t* key)
{
	AES_init_ctx(&ctx, key);
}

const char* encrypt_message(struct AES_ctx* ctx, uint8_t* buf)
{
	uint8_t* encrypted;
	strcpy(encrypted, buf);
	AES_ECB_encrypt(&ctx, encrypted);
	return encrypted;
}

const char* decrypt_message(struct AES_ctx* ctx, uint8_t* buf)
{
	uint8_t* decrypted;
	strcpy(decrypted, buf);
	AES_ECB_decrypt(&ctx, decrypted);
	return decrypted;
}
