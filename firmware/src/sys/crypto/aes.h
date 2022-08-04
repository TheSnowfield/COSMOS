#ifndef _SYS_CRYPTO_AES_H
#define _SYS_CRYPTO_AES_H

#include <stdint.h>
#include <stddef.h>

#define AES128_KEY_ROUNDS 10
#define AES192_KEY_ROUNDS 12
#define AES256_KEY_ROUNDS 14

/**
 * @param key length = 32 bytes (256 bits)
 * @param iv length = 16 bytes
 * @param len data length, must be a multiple of 16
 */
void crypto_aes256cbc_encrypt(uint8_t *key, uint8_t *iv, uint8_t *data_in, uint8_t *data_out, size_t len);

/**
 * @param key length = 32 bytes (256 bits)
 * @param iv length = 16 bytes
 * @param len data length, must be a multiple of 16
 */
void crypto_aes256cbc_decrypt(uint8_t *key, uint8_t *iv, uint8_t *data_in, uint8_t *data_out, size_t len);

#endif /* _SYS_CRYPTO_AES_H */
