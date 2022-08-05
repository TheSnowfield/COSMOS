#ifndef __SYS_HARDWARE_NORFLASH_H
#define __SYS_HARDWARE_NORFLASH_H

#include <sys/status.h>

typedef enum {
  w25q64jvssiq
} flash_type_t;

typedef struct {
  uint8_t dummy [3];
  uint8_t manufacturer_id;
  uint8_t device_id;
} w25qx_manufacturer_id_t;

typedef struct {
  uint8_t manufacturer_id;
  uint8_t memory_type;
  uint8_t capacity;
} w25qx_jedec_id_t;

typedef struct {

} w25qx_register1_t;

typedef struct {
  uint8_t dummy [4];
  uint8_t unique_sn [8];
} w25qx_unique_id_t;

typedef enum {
  erase_sector,
  erase_block32,
  erase_block64,
  erase_chip,
} erase_type_t;

#ifdef __NORFLASH_INTERNAL

#define W25QX_TIMEOUT 1000

#define W25QX_CMD_WRITE_ENABLE       0x06
#define W25QX_CMD_VOLATILE_WRITE_EN  0x50
#define W25QX_CMD_WRITE_DISABLE      0x04
#define W25QX_CMD_RELEASE_POWER_DOWN 0x05
#define W25QX_CMD_MANUFACTURER_ID    0x90
#define W25QX_CMD_JEDEC_ID           0x9F
#define W25QX_CMD_READ_UNIQUE_ID     0x4B
#define W25QX_CMD_READ_DATA          0x03
#define W25QX_CMD_FAST_READ          0x0B
#define W25QX_CMD_PAGE_PROGRAM       0x02
#define W25QX_CMD_SECTOR_ERASE       0x20
#define W25QX_CMD_BLOCK_ERASE_32K    0x52
#define W25QX_CMD_BLOCK_ERASE_64K    0xD8
#define W25QX_CMD_CHIP_ERASE         0xC7
#define W25QX_CMD_READ_STATUS1       0x05
#define W25QX_CMD_WRITE_STATUS1      0x01
#define W25QX_CMD_READ_STATUS2       0x35
#define W25QX_CMD_WRITE_STATUS2      0x31
#define W25QX_CMD_READ_STATUS3       0x15
#define W25QX_CMD_WRITE_STATUS3      0x11
#define W25QX_CMD_READ_SFDP          0x5A
#define W25QX_CMD_ERASE_SECURITY_REG 0x44
#define W25QX_CMD_PROGRAM_SECURITY_REG     0x42
#define W25QX_CMD_READ_SECURITY_REG        0x48
#define W25QX_CMD_GLOBAL_BLOCK_LOCK        0x7E
#define W25QX_CMD_GLOBAL_BLOCK_UNLOCK      0x98
#define W25QX_CMD_READ_BLOCK_LOCK          0x3D
#define W25QX_CMD_INDIVIDUAL_BLOCK_LOCK    0x36
#define W25QX_CMD_INDIVIDUAL_BLOCK_UNLOCK  0x39
#define W25QX_CMD_ERASE_SUSPEND    0x75
#define W25QX_CMD_ERASE_RESUME     0x7A
#define W25QX_CMD_POWER_DOWN_RESET 0xB9
#define W25QX_CMD_ENABLE_RESET     0x66
#define W25QX_CMD_RESET_DEVICE     0x99

#define w25qx_select() SPI1_PORT->BRR = (uint32_t)SPI1_PIN_CTL_CS
#define w25qx_unselect() SPI1_PORT->BSRR = (uint32_t)SPI1_PIN_CTL_CS
#define w25qx_reset() SPI1_PORT->BRR = (uint32_t)SPI1_PIN_CTL_RST
#define w25qx_unreset() SPI1_PORT->BSRR = (uint32_t)SPI1_PIN_CTL_RST

#endif

/**
 * @brief Init flash device
 */
void w25qx_init();

/**
 * @brief Get the manufacturer ID of the flash.
 *
 * @param id w25qx_manufacturer_id_t* pointer
 * @return if success return ok
 */
status_t w25qx_get_manufacturer_id(w25qx_manufacturer_id_t *id);

/**
 * @brief Get the JEDEC ID of the flash.
 *
 * @param id w25qx_jedec_id_t* pointer
 * @return if success return ok
 */
status_t w25qx_get_jedec_id(w25qx_jedec_id_t *id);

/**
 * @brief Get the unique ID of the flash.
 *
 * @param id w25qx_unique_id_t* pointer
 * @return if success return ok
 */
status_t w25qx_get_unique_id(w25qx_unique_id_t *id);

/**
 * @brief Enable write
 *
 * @return if success return ok
 */
status_t w25qx_write_enable();

/**
 * @brief Disable write
 *
 * @return if success return ok
 */
status_t w25qx_write_disable();

/**
 * @brief Global lock
 *
 * @return if success return ok
 */
status_t w25qx_global_lock();

/**
 * @brief Global unlock
 *
 * @return if success return ok
 */
status_t w25qx_global_unlock();

/**
 * @brief Read flash data
 *
 * @param address 24bit access address
 * @param data data pointer
 * @param length data length
 * @return if success return ok
 */
status_t w25qx_read_data(uint32_t address, void* data, uint32_t length);

/**
 * @brief Read flash data (fast)
 *
 * @param address 24bit access address
 * @param data data pointer
 * @param length data length
 * @return if success return ok
 */
status_t w25qx_fastread(uint32_t address, void* data, uint32_t length);

/**
 * @brief Write flash data
 *
 * @param address 24bit access address
 * @param data data pointer
 * @param length data length
 * @return if success return ok
 */
status_t w25qx_write_data(uint32_t address, void* data, uint32_t length);

/**
 * @brief Erase flash. Write enable/disable instructions
 *        and automatic write disable after erase or program
 *
 * @param type erase type
 * @param address 24bit access address
 * @return if success return ok
 */
status_t w25qx_erase(erase_type_t type, uint32_t address);

#endif /* __SYS_HARDWARE_NORFLASH_H */
