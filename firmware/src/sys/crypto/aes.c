#include <stdint.h>
#include <aes/target/CM0.h>

#define AES128_KEY_ROUNDS 10
#define AES192_KEY_ROUNDS 12
#define AES256_KEY_ROUNDS 14

void crypto_aes256cbc_encrypt(uint8_t *key, void *iv, void *data_in, void *data_out, uint32_t len) {

  // add padding
  uint8_t pad_len = 16 - len % 16;
  for (uint32_t i = len + (uint32_t)pad_len - 1; i >= len; --i)
    ((uint8_t*)data_in)[i] = pad_len;

  uint8_t round_key[(AES256_KEY_ROUNDS + 1) * 16];
  CM0_sBOX_AES_256_keyschedule_enc(round_key, key);
  uint32_t blocks_cnt = (len + 15) / 16;

  uint32_t* data_in_p = (uint32_t*)data_in;
  uint32_t* data_out_p = (uint32_t*)data_out;
  uint32_t* iv_p = (uint32_t*)iv;

  for(uint32_t i = 0; i < blocks_cnt; i++) {
    data_out_p[0] = data_in_p[0] ^ iv_p[0];
    data_out_p[1] = data_in_p[1] ^ iv_p[1];
    data_out_p[2] = data_in_p[2] ^ iv_p[2];
    data_out_p[3] = data_in_p[3] ^ iv_p[3];
    CM0_sBOX_AES_encrypt(round_key, (uint8_t*)data_out_p, (uint8_t*)data_out_p, AES256_KEY_ROUNDS);

    iv_p = data_out_p;
    data_out_p += 4;
    data_in_p += 4;
  }
}

uint32_t crypto_aes256cbc_decrypt(uint8_t *key, void *iv, uint8_t *data_in, void *data_out, uint32_t len) {
  uint8_t round_key[(AES256_KEY_ROUNDS + 1) * 16];
  CM0_sBOX_AES_256_keyschedule_enc(round_key, key);
  uint32_t blocks_cnt = (len + 15) / 16;

  uint32_t* data_out_p = (uint32_t*)data_out;
  uint32_t* iv_p = (uint32_t*)iv;

  for(uint32_t i = 0; i < blocks_cnt; i++) {
    CM0_sBOX_AES_decrypt(round_key, data_in, (uint8_t*)data_out_p, AES256_KEY_ROUNDS);
    data_out_p[0] ^= iv_p[0];
    data_out_p[1] ^= iv_p[1];
    data_out_p[2] ^= iv_p[2];
    data_out_p[3] ^= iv_p[3];

    iv_p = (uint32_t*)data_in;
    data_out_p += 4;
    data_in += 16;
  }

  // remove padding
  return len - (uint32_t)(((uint8_t*)data_out)[len - 1]);
}
