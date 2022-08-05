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

uint32_t read_passbook_cnt();
void read_passbook_index(uint16_t *id_list);

uint16_t insert_passnote(passnote_meta_t *meta, passnote_data_t *data);
void update_passnote(uint16_t id, passnote_meta_t *meta, passnote_data_t *data);
void fetch_passnote_meta(uint16_t id, passnote_meta_t *meta);
void fetch_passnote_data(uint16_t id, passnote_data_t *data);
void fetch_passnote_name(uint16_t id, void *name);
void delete_passnote(uint16_t id);

#endif /* _APP_PASSBOOK */
