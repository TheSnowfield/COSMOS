#ifndef _SYS_CRYPTO_AES_H
#define _SYS_CRYPTO_AES_H

#include <stdint.h>

/**
 * @param key length = 32 bytes (256 bits)
 * @param iv length = 16 bytes
 * @param data_in (length must be a multiple of 16)
 * @param data_out (length = length of data_in)
 * @param len data length without padding (real data length)
 * @example real data length is 15, then the length of `data_in` or `data_out` should be 16
 * @example real data length is 16, then the length of `data_in` or `data_out` should be 32
 */
void crypto_aes256cbc_encrypt(uint8_t *key, void *iv, void *data_in, void *data_out, uint32_t len);

/**
 * @param key length = 32 bytes (256 bits)
 * @param iv length = 16 bytes
 * @param data_in
 * @param data_out
 * @param len data length, must be a multiple of 16
 * @return data length without padding (real data length)
 * @example `len` is 16 and returned length is 15, then real data is `data_out[0:14]`
 * @example `len` is 32 and returned length is 16, then real data is `data_out[0:15]`
 */
uint32_t crypto_aes256cbc_decrypt(uint8_t *key, void *iv, uint8_t *data_in, void *data_out, uint32_t len);

#endif /* _SYS_CRYPTO_AES_H */
