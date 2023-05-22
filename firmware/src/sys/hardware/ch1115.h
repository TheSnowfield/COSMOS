#ifndef _SYS_HARDWARE_CH1115_H
#define _SYS_HARDWARE_CH1115_H

#include <stdbool.h>
#include <sys/status.h>

typedef void (* ch1115_display_ready_t)();

typedef enum {
  clr_black = 0x00,
  clr_white = 0xFF
} ch1115_color_t;

#ifdef _CH1115_INTERNAL
#include <config.h>

#define CH1115_ADDRESS (0x3C << 1)
#define Ch1115_MAX_ARGLEN 16

typedef enum {
  cmd_type_operation = 0x00,
  cmd_type_pixeldata = 0x40
} ch1115_cmd_type_t;

// Fundamental Commands
#define CH1115_REG_ENTIRE_DISPLAY_ON 0xA4 //Forcibly turns the entire display on regardless of the contents of the display data RAM
#define CH1115_REG_ENTIRE_DISPLAY_OFF 0xA5
#define CH1115_REG_DISPLAY_ON 0xAF
#define CH1115_REG_DISPLAY_OFF 0xAE
#define CH1115_REG_DISPLAY_NORMAL 0xA6
#define CH1115_REG_DISPLAY_INVERT 0xA7
#define CH1115_REG_CONTRAST_CONTROL 0x81 //Contrast Control Mode Set
#define CH1115_REG_CONTRAST_DATA_DEFAULT 0x80 // Contrast Data Reg Set 0x00 to 0xFF

// Addressing Setting Commands
#define CH1115_REG_SET_PAGEADD 0xB0 // Set Page Address: (B0H - B7H)
#define CH1115_REG_SET_COLADD_LSB 0x00 // Set Lower Column Address: (00H - 0FH)
#define CH1115_REG_SET_COLADD_MSB 0x10 // Set Higher Column Address: (10H – 1FH)
#define CH1115_REG_SET_DISPLAY_START_LINE 0x40 // Determine the initial display line

// Hardware Configuration Commands
#define CH1115_REG_SET_PUMP_REG 0x30 // Output voltage (V PP ) internal charger pump.
#define CH1115_REG_SET_PUMP_SET   0x01 //  A1:A0[] 01 = 7.4v
#define CH1115_REG_IREF_REG 0x82 // I REF Resistor Set
#define CH1115_REG_IREF_SET 0x00 // I REF Resistor adjust
#define CH1115_REG_SEG_SET_REMAP  0xA0 // Set Segment Re-map: (A0H - A1H)
#define CH1115_REG_SEG_SET_PADS 0xA2 //Set SEG pads hw config: (A2H – A3H)
#define CH1115_REG_MULTIPLEX_MODE_SET 0xA8 //Multiplex Ration Mode Set
#define CH1115_REG_MULTIPLEX_DATA_SET  0x3F //Multiplex Ration Data Set: (00H - 3FH)

#define CH1115_REG_DC_MODE_SET 0xAD // DC-DC Control Mode Set
#define CH1115_REG_DC_ONOFF_SET 0x8B // DC-DC ON/OFF Mode Set (8AH - 8BH) (a=off; b=on)
#define CH1115_REG_COMMON_SCAN_DIR 0xC0 // Set Common Op Scan Dir:C0H - C8H Flip vertical
#define CH1115_REG_OFFSET_MODE_SET 0xD3 //Display Offset Mode Set:
#define CH1115_REG_OFFSET_DATA_SET 0x00 //Display Offset Data Set: (00H~3FH)
#define CH1115_REG_BREATHEFFECT_SET 0x23 // Set Breathing Display Effect:

// Timing & Driving Scheme Setting Commands
#define CH1115_REG_OSC_FREQ_MODE_SET 0xD5 //Divide Ratio/Oscillator Frequency Mode Set: (D5H)
#define CH1115_REG_PRECHARGE_MODE_SET 0xD9 //Pre-charge Period Mode Set:
#define CH1115_REG_COM_LEVEL_MODE_SET 0xDB //COM Deselect Level Mode Set: (DBH)

// HORIZONTAL Scrolling Commands
#define CH1115_REG_HORIZONTAL_A_SCROLL_SETUP 0x24 //  Additional Horizontal Scroll Setup Mode Set: (24H)
#define CH1115_REG_HORIZONTAL_A_SCROLL_SET_SCOL 0x00  // Start Column Position Set: (A7 – A0)
#define CH1115_REG_HORIZONTAL_A_SCROLL_SET_ECOL 0x7F // End Column Position Set: (B7 – B0)
#define CH1115_REG_SCROLL_SETUP  0x26 //Horizontal Scroll Setup: 26 right 27 left (A2 – A0)
#define CH1115_REG_SPAGE_ADR_SET 0x00  // Start Page Address Set 0x00 = page 0
#define CH1115_REG_TIME_SET 0x00 //Time Interval Set: (B2 – B0) 0x00 = 6 frames
#define CH1115_REG_EPAGE_ADR_SET  0x07 //End Page Address Set: (C2 – C0)
#define CH1115_REG_SET_SCROLL_MODE 0x28 // Set Scroll Mode: (28H – 2BH) 0x28 = continuous
#define CH1115_REG_DEACTIVATE_SCROLL  0x2E
#define CH1115_REG_ACTIVATE_SCROLL   0x2F

typedef struct {
  uint8_t cmd;
  uint8_t args[Ch1115_MAX_ARGLEN];
  uint8_t len;
  uint8_t delay;

} ch1115_initcmd_t;

ch1115_initcmd_t ch1115_init_cmd[] = {
  { CH1115_REG_DISPLAY_OFF,             NULL,       0, 150 },
  { CH1115_REG_SET_COLADD_LSB,          NULL,       0, 1   },
  { CH1115_REG_SET_COLADD_MSB,          NULL,       0, 1   },
  { CH1115_REG_SET_DISPLAY_START_LINE,  NULL,       0, 1   },
  { CH1115_REG_SET_PAGEADD,             NULL,       0, 1   },
  { CH1115_REG_CONTRAST_CONTROL,        { 0x4D },   1, 1   },
  { CH1115_REG_IREF_REG,                { 0x00 },   1, 1   },
  { CH1115_REG_SEG_SET_PADS,            NULL,       0, 1   },

  { 0xA4,                               NULL,       0, 1   },
  { CH1115_REG_DISPLAY_NORMAL,          NULL,       0, 1   },
  { 0xA8,                               { 0x1F },   1, 1   },
  // { 0xC8,                               NULL,       0, 1   },
  { CH1115_REG_OFFSET_MODE_SET,         { 0x10 },   1, 1   },
  { CH1115_REG_OSC_FREQ_MODE_SET,       { 0b11110001 },   1, 1   },
  { CH1115_REG_PRECHARGE_MODE_SET,      { 0x62 },   1, 1   },
  { CH1115_REG_COM_LEVEL_MODE_SET,      { 0x3F },   1, 1   },
  { 0x32,                               NULL,       0, 1   },
  { CH1115_REG_DC_MODE_SET,             { 0x8B },   1, 1   },
  { CH1115_REG_COMMON_SCAN_DIR + 8,     NULL,       0, 1   },
  { CH1115_REG_SEG_SET_REMAP   + 0,     NULL,       0, 1   },
  { 0xE3,                               NULL,       0, 150 },
  // { CH1115_REG_BREATHEFFECT_SET,        { 0b10000000 }, 1, 1 },

  // { 0x81,                               {0b11111111},       1, 150 },

  { CH1115_REG_DISPLAY_OFF,              NULL,       0, 150 },
};

#define ch1115_reset_set() I2C1_PORT->BSRR = I2C1_PIN_CTL_RST
#define ch1115_reset_reset() I2C1_PORT->BSRR = (uint32_t)I2C1_PIN_CTL_RST << 16U

#endif

void ch1115_init();

/**
 * @brief turn the display on
 * @return status_t return status
 */
status_t ch1115_light_on(bool breathing);

/**
 * @brief turn the display off
 * @return status_t return status
 */
status_t ch1115_light_off(bool breathing);

status_t ch1115_set_pixel(uint8_t x, uint8_t y, ch1115_color_t color);
status_t ch1115_set_pixel_column(uint8_t bits);
status_t ch1115_set_page(uint8_t page);
status_t ch1115_set_column(uint8_t column);
void ch1115_set_readycb(ch1115_display_ready_t cb);
status_t ch1115_write_cmd(uint8_t cmd, uint8_t* args, uint8_t len);
// status_t ch1115_write_page(uint8_t page, uint8_t column, uint8_t* buf, uint8_t len);
status_t ch1115_write_page(uint8_t page, uint8_t* buf, uint8_t len);
status_t ch1115_write_page_it(uint8_t page, uint8_t* buf, uint8_t len);

#endif /* _SYS_HARDWARE_CH1115_H */
