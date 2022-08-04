#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <hardware/w25qx.h>
#include <passbook.h>

#define FLASH_TOTAL_SIZE 8 * 1024 * 1024
#define FLASH_PAGE_SIZE 256
#define FLASH_SECTOR_SIZE 4096
#define FLASH_PAGE_CNT 32768
#define FLASH_SECTOR_CNT 2048

uint32_t get_passnote_cnt() {
  uint32_t cnt;
  w25qx_read_data(0, &cnt, 4);
  if (cnt == 0xffffffff)
    cnt = 0;
  return cnt;
}

void get_passnote_index(uint16_t *id_list) {
  uint32_t cnt = get_passnote_cnt();
  if (cnt > 0)
    w25qx_read_data(256, id_list, get_passnote_cnt() * 2);
}

uint16_t add_passnote(passnote_meta_t *meta, passnote_data_t *data) {
  uint32_t cnt = get_passnote_cnt();
  uint16_t *id_list = (uint16_t*)malloc((cnt + 1) * 2);
  get_passnote_index(id_list);
  uint16_t id = 0;
  for (uint32_t i = 0; i <= cnt; i++) {
    id = i + 1;
    if (i < cnt && id_list[i] > id)
      break;
  }
  set_passnote(id, meta, data);
  if (id > cnt) {
    memcpy(id_list + cnt, &id, 2);
  } else {
    memcpy(id_list + id, id_list + id - 1, (cnt - id - 1) * 2);
    memcpy(id_list + id - 1, &id, 2);
  }
  cnt++;
  w25qx_erase(erase_sector, 0);
  w25qx_write_data(0, &cnt, 4);
  w25qx_write_data(256, id_list, cnt * 2);
  free(id_list);
}
// 1 2 3 5 7
// 1 2 3 4 5 7
// 1 2 3
// 1 2 3 4

void set_passnote(uint16_t id, passnote_meta_t *meta, passnote_data_t *data) {
  uint32_t address = id * FLASH_SECTOR_SIZE;
  w25qx_erase(erase_sector, address);
  w25qx_write_data(address, meta, sizeof(*meta));
  address += FLASH_PAGE_SIZE;
  w25qx_write_data(address, data->name, meta->name_len);
  address += FLASH_PAGE_SIZE;
  w25qx_write_data(address, data->account, meta->account_len);
  address += FLASH_PAGE_SIZE * 2;
  w25qx_write_data(address, data->password, meta->password_len);
}

void get_passnote_meta(uint16_t id, passnote_meta_t *meta) {
  w25qx_read_data(id * FLASH_SECTOR_SIZE, meta, sizeof(*meta));
}

void get_passnote_data(uint16_t id, passnote_data_t *data) {
  passnote_meta_t meta;
  get_passnote_meta(id, &meta);
  uint32_t address = id * FLASH_SECTOR_SIZE + FLASH_PAGE_SIZE;
  w25qx_read_data(address, data->name, meta->name_len);
  address += FLASH_PAGE_SIZE;
  w25qx_read_data(address, data->account, meta->account_len);
  address += FLASH_PAGE_SIZE * 2;
  w25qx_read_data(address, data->password, meta->password_len);
}

void get_passnote_name(uint16_t id, void *name) {
  passnote_meta_t meta;
  get_passnote_meta(id, &meta);
  uint32_t address = id * FLASH_SECTOR_SIZE + FLASH_PAGE_SIZE;
  w25qx_read_data(address, name, meta->name_len);
}

void del_passnote(uint16_t id) {
  w25qx_erase(erase_sector, id * FLASH_SECTOR_SIZE);
  uint32_t cnt = get_passnote_cnt();
  uint16_t id_list[cnt];
  get_passnote_index(id_list, cnt);
  for (uint32_t i = 0; i < cnt; i++) {
    if (id_list[i] == id) {
      memcpy(id_list + i, id_list + i + 1, (cnt - i - 1) * 2);
      break;
    }
  }
  cnt--;
  w25qx_erase(erase_sector, 0);
  w25qx_write_data(0, &cnt, 4);
  w25qx_write_data(256, id_list, cnt * 2);
}
