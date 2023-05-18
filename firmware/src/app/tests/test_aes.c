#include <string.h>
#include <sys/panic.h>
#include <crypto/aes.h>

void test_aes_appmain() {

  uint8_t key_256[32] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
  };

  uint8_t cbc_iv[16] = {
    0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30,
    0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a
  };

  uint8_t cbc_expected_plaintext[64] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
  };

  uint8_t cbc_expected_ciphertext[64] = {
    0x36, 0x74, 0x69, 0x6D, 0x1A, 0x47, 0x1A, 0x53, 0xAF, 0xEB, 0x8F, 0xD2, 0x17, 0xB6, 0x75, 0xD4,
    0xB4, 0xE9, 0x96, 0xB6, 0x43, 0xF9, 0x90, 0x2E, 0xC5, 0xBD, 0x8C, 0xC0, 0x68, 0x9E, 0x29, 0x2A,
    0x7C, 0x35, 0xD2, 0x7F, 0x86, 0xB7, 0x42, 0xF2, 0x69, 0x00, 0xA5, 0x0B, 0x08, 0xE6, 0x3C, 0x4E,
    0x2D, 0xF1, 0x46, 0x1E, 0x41, 0x1C, 0x15, 0x19, 0xF2, 0x23, 0x78, 0x24, 0x23, 0x52, 0xEB, 0x43
  };

  uint8_t cbc_tmp[64];

  // encrypt test
  crypto_aes256cbc_encrypt(key_256, cbc_iv, cbc_expected_plaintext, cbc_tmp, 64);
  if(memcmp(cbc_expected_ciphertext, cbc_tmp, 64) != 0)
    panic(NULL);

  // decrypt test
  crypto_aes256cbc_decrypt(key_256, cbc_iv, cbc_expected_ciphertext, cbc_tmp, 64);
  if(memcmp(cbc_expected_plaintext, cbc_tmp, 64) != 0)
    panic(NULL);

}
