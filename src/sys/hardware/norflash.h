#ifndef __SYS_HARDWARE_NORFLASH_H
#define __SYS_HARDWARE_NORFLASH_H

void norflash_init();
void norflash_read(uint32_t address, uint8_t *data, uint32_t length);
void norflash_write(uint32_t address, uint8_t *data, uint32_t length);
void norflash_erase(uint32_t address, uint32_t length);

#endif /* __SYS_HARDWARE_NORFLASH_H */
