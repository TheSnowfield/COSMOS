#ifndef __SYS_HARDWARE_NORFLASH_H
#define __SYS_HARDWARE_NORFLASH_H

typedef enum {
  w25q64jvssiq
} flash_type_t;

#ifdef __NORFLASH_INTERNAL
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

void w25qx_init();
void w25qx_read(uint32_t address, uint8_t *data, uint32_t length);
void w25qx_write(uint32_t address, uint8_t *data, uint32_t length);
void w25qx_erase(uint32_t address, uint32_t length);

#endif /* __SYS_HARDWARE_NORFLASH_H */
