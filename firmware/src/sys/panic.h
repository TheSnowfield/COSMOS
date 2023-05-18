#ifndef _SYS_PANIC_H

#include <stdint.h>

void panic(const char *msg);
void assert(uint8_t condition, const char *msg);
void reset();

#endif /* _SYS_PANIC_H */
