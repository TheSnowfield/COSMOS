#ifndef _APP_PASSBOOK
#define _APP_PASSBOOK

#include <stdint.h>

typedef struct {
  uint32_t name_len;
  uint32_t account_len;
  uint32_t password_len;
} passnote_meta_t;

typedef struct {
  void *name;
  void *account;
  void *password;
} passnote_data_t;

uint32_t get_passnote_cnt();
void get_passnote_index(uint16_t *note_id_list, uint32_t cnt);

uint16_t add_passnote(passnote_meta_t *meta, passnote_data_t *data);
void set_passnote(uint16_t id, passnote_meta_t *meta, passnote_data_t *data);
void get_passnote_meta(uint16_t id, passnote_meta_t *meta);
void get_passnote_data(uint16_t id, passnote_data_t *data);
void get_passnote_name(uint16_t id, void *name);
void del_passnote(uint16_t id);

#endif /* _APP_PASSBOOK */
