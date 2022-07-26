#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sys/panic.h>
#include <sys/passbook.h>

void assert_note_id(uint16_t id) {
  if (id < 1 || id > 2047)
    panic();
}

void assert_value(var, val) {
  if (var != val)
    panic();
}

uint32_t test_passbook_read_index() {
  uint32_t cnt = read_passbook_cnt();
  if (cnt < 0)
    panic();

  if (cnt > 0) {
    uint16_t *id_list = (uint16_t*)malloc(cnt * 2);
    read_passbook_index(id_list);

    for (uint32_t i = 0; i < cnt; i++)
      assert_note_id(id_list[i]);

    free(id_list);
  }

  return cnt;
}

uint16_t test_passbook_add_note() {
  char *name = "高山";
  char *account = "takayama";
  char *password = "theSnowfield";

  passnote_meta_t meta = {
    .name_len = 7,
    .account_len = 9,
    .password_len = 13,
  };
  passnote_data_t data = {
    .name = name,
    .account = account,
    .password = password,
  };
  uint16_t id = insert_passnote(&meta, &data);
  assert_note_id(id);

  passnote_meta_t meta_saved;
  char name_saved[7];
  char account_saved[9];
  char password_saved[13];
  passnote_data_t data_saved = {
    .name = name_saved,
    .account = account_saved,
    .password = password_saved,
  };

  fetch_passnote_meta(id, &meta_saved);
  assert_value(meta_saved.name_len, 7);
  assert_value(meta_saved.account_len, 9);
  assert_value(meta_saved.password_len, 13);

  fetch_passnote_name(id, name_saved);
  if(memcmp(name_saved, name, 7) != 0)
    panic();

  fetch_passnote_data(id, &data_saved);
  if(memcmp(name_saved, name, 7) != 0)
    panic();
  if(memcmp(account_saved, account, 9) != 0)
    panic();
  if(memcmp(password_saved, password, 13) != 0)
    panic();

  return id;
}

void test_passbook_appmain() {
  uint32_t cnt = test_passbook_read_index();
  uint16_t id = test_passbook_add_note();
  id = test_passbook_add_note();
  if (test_passbook_read_index() - cnt != 2)
    panic();

  cnt += 2;

  delete_passnote(id);
  if (cnt - test_passbook_read_index() != 1)
    panic();
}
