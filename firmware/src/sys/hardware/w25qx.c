#include <config.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <sys/panic.h>
#include <sys/status.h>

#define __NORFLASH_INTERNAL
#include <hardware/w25qx.h>
#undef __NORFLASH_INTERNAL

void w25qx_init() {

  // reset chip
  w25qx_unselect();
  w25qx_unreset();
  HAL_Delay(10);

  // detect device
  // w25qx_manufacturer_id_t id = {0};
  // w25qx_get_manufacturer_id(&id);
  // HAL_Delay(100);

  // w25qx_jedec_id_t jedecid = {0};
  // w25qx_get_jedec_id(&jedecid);
  // HAL_Delay(100);

  // w25qx_unique_id_t uniqid = {0};
  // w25qx_get_unique_id(&uniqid);
  // HAL_Delay(100);

  // uint8_t bytes[256];

  // // erase sector
  // w25qx_write_enable(); {
  //   w25qx_erase(erase_sector, 0);
  // }

  // // read data
  // w25qx_read_data(0, bytes, sizeof(bytes), false);

  // // write data into
  // w25qx_write_enable(); {
  //   memset(bytes, 0xCC, sizeof(bytes));
  //   w25qx_write_data(0, bytes, sizeof(bytes));
  // }

  // // read data
  // memset(bytes, 0x00, sizeof(bytes));
  // w25qx_read_data(0, bytes, sizeof(bytes), false);

  // // verify data
  // for (uint32_t i = 0; i < sizeof(bytes); i++)
  //   if (bytes[i] != 0xCC) panic();

  // HAL_Delay(100);
}

status_t w25qx_sendrecv(uint8_t cmd, void* recv, uint32_t length, bool cs) {

  HAL_StatusTypeDef ret;

  w25qx_select(); {
    // send command first
    // if command equals 0x00 then it's a read command
    if(cmd != 0x00) {
      ret = HAL_SPI_Transmit(&spi1, &cmd, 1, W25QX_TIMEOUT);
      if(ret != HAL_OK) goto final;
    }

    // send dummy bytes and receive
    if(recv == NULL) goto final;
    ret = HAL_SPI_Receive(&spi1, (uint8_t *)recv, length, W25QX_TIMEOUT);
  }

  final:
  if(cs) w25qx_unselect(); // if the operation does not finished
  return (ret == HAL_OK) ? ok : error;
}

status_t w25qx_write_enable() {
  return w25qx_sendrecv(W25QX_CMD_WRITE_ENABLE, NULL, 0, true);
}

status_t w25qx_write_disable() {
  return w25qx_sendrecv(W25QX_CMD_WRITE_DISABLE, NULL, 0, true);
}

status_t w25qx_get_manufacturer_id(w25qx_manufacturer_id_t *id) {
  return w25qx_sendrecv(W25QX_CMD_MANUFACTURER_ID, id, sizeof(w25qx_manufacturer_id_t), true);
}

status_t w25qx_get_jedec_id(w25qx_jedec_id_t *id) {
  return w25qx_sendrecv(W25QX_CMD_JEDEC_ID, id, sizeof(w25qx_jedec_id_t), true);
}

status_t w25qx_get_unique_id(w25qx_unique_id_t *id) {
  return w25qx_sendrecv(W25QX_CMD_READ_UNIQUE_ID, id, sizeof(w25qx_unique_id_t), true);
}

status_t w25qx_global_lock() {
  return w25qx_sendrecv(W25QX_CMD_GLOBAL_BLOCK_LOCK, NULL, 0, true);
}

status_t w25qx_global_unlock() {
  return w25qx_sendrecv(W25QX_CMD_GLOBAL_BLOCK_UNLOCK, NULL, 0, true);
}

status_t w25qx_read_data(uint32_t address, void* data, uint32_t length) {
  status_t ret;
  
  // send command first
  ret = w25qx_sendrecv(W25QX_CMD_READ_DATA, &address, 3, false);
  if(ret != ok) return ret;

  // send dummy clock and receive data
  ret = w25qx_sendrecv(0, data, length, true);
  return ret;
}

status_t w25qx_fastread(uint32_t address, void* data, uint32_t length) {
  status_t ret;

  // align length to multiple of 2
  length ^= length & 1;

  // remove high 8 bit from address
  address = (address << 8) >> 8;

  // send command first
  ret = w25qx_sendrecv(W25QX_CMD_FAST_READ, &address, 4, false);
  if(ret != ok) return ret;

  // send dummy clock and receive data
  ret = w25qx_sendrecv(0, data, length, true);
  return ret;
}

status_t w25qx_write_data(uint32_t address, void* data, uint32_t length) {
  status_t ret;
  
  // only allow write more than 256 bytes at a time
  if(length < 256) {
    uint8_t padding[256];
    memcpy(padding, data, length);
    data = padding;
    length = 256;
  }

  // send command first
  ret = w25qx_sendrecv(W25QX_CMD_PAGE_PROGRAM, &address, 3, false);
  if(ret != ok) return ret;
  
  // write data
  ret = w25qx_sendrecv(0, data, length, true);
  return ret;
}

status_t w25qx_erase(erase_type_t type, uint32_t address) {

  switch(type) {
    case erase_sector:
      return w25qx_sendrecv(W25QX_CMD_SECTOR_ERASE, &address, 3, true);
    case erase_block32:
      return w25qx_sendrecv(W25QX_CMD_BLOCK_ERASE_32K, &address, 3, true);
    case erase_block64:
      return w25qx_sendrecv(W25QX_CMD_BLOCK_ERASE_64K, &address, 3, true);
    case erase_chip:
      return w25qx_sendrecv(W25QX_CMD_CHIP_ERASE, NULL, 0, true);
  }

  return error;
}

// status_t w25qx_read_status_register1(uint8_t *status) {
//   return w25qx_sendrecv(W25QX_CMD_READ_STATUS1, status, 1);
// }
