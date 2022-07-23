#ifndef __SYS_HARDWARE_CH9329_H
#define __SYS_HARDWARE_CH9329_H

#include <stdbool.h>
#define packed __attribute__((packed))

typedef enum {
  ok,
  error
} ch9329_stat_t;

typedef enum {
  v1_0 = 0x30,
  v1_1,
  v1_2,
  v1_3,
  v1_4,
} ch9329_chip_version_t;

typedef struct {
  uint8_t num_lock    : 1;
  uint8_t caps_lock   : 1;
  uint8_t scroll_lock : 1;
  uint8_t reserved3 : 1;
  uint8_t reserved4 : 1;
  uint8_t reserved5 : 1;
  uint8_t reserved6 : 1;
  uint8_t reserved7 : 1;
} keyboard_stat_t;

typedef struct {
	ch9329_chip_version_t version;
	bool usb_connected;
	keyboard_stat_t keyboard_stat;
  uint8_t reserved [5];
} ch9329_chip_info_t;

typedef enum {
  sw0_general_kbd_mouse = 0x00,
  sw1_general_kbd = 0x01,
  sw2_general_custom_usb_hid = 0x03,
  hw0_general_kbd_mouse = 0x80,
  hw1_general_kbd = 0x81,
  hw2_general_mouse = 0x83,
  hw3_general_custom_usb_hid = 0x83,
} packed ch9329_usb_mode;

typedef enum {
  sw0_protocol,
  sw1_ascii,
  sw2_proxy,
  hw0_protocol,
  hw1_ascii,
  hw2_proxy,
} packed ch9329_serial_mode;

typedef enum {
  manufacturer,
  product_desc,
  serial_number,
} ch9329_usbstr_t;

typedef struct {
  uint8_t enable_custom_sn : 1;
  uint8_t enable_custom_product : 1;
  uint8_t enable_custom_manufacturer : 1;
  uint8_t reserved0 : 1;
  uint8_t reserved1 : 1;
  uint8_t reserved2 : 1;
  uint8_t reserved3 : 1;
  uint8_t enable_custom_string : 1;
} ch9329_vendor_cfg_t;

typedef struct {
  ch9329_usb_mode usb_mode;
  ch9329_serial_mode serial_mode;
  uint8_t address;
  uint32_t baudrate;
  uint16_t reserved0;
  uint16_t packet_interval;
  uint16_t usb_vid;
  uint16_t usb_pid;
  uint16_t usb_kbd_report_interval;
  uint16_t usb_kbd_release_delay;
  bool insert_newline;
  unsigned char usb_kbd_newline_character[8];
  unsigned char usb_kbd_filter[8];
  ch9329_vendor_cfg_t usb_vendor_flag;
  bool usb_quick_report;
  uint32_t reserved1;
  uint32_t reserved2;
  uint32_t reserved3;
} packed ch9329_para_cfg_t;

typedef struct {
  uint8_t control_left  : 1;
  uint8_t shift_left    : 1;
  uint8_t alt_left      : 1;
  uint8_t winkey_left   : 1;
  uint8_t control_right : 1;
  uint8_t shift_right   : 1;
  uint8_t alt_right     : 1;
  uint8_t winkey_right  : 1;
} ch9329_function_keys_t;

typedef enum {
  key_acute = 0x35,
  key_1 = 0x1E,
  key_2 = 0x1F,
  key_3 = 0x20,
  key_4 = 0x21,
  key_5 = 0x22,
  key_6 = 0x23,
  key_7 = 0x24,
  key_8 = 0x25,
  key_9 = 0x26,
  key_0 = 0x27,
  key_minus = 0x2D,
  key_equal = 0x2E,
  key_back_space = 0x2A,
  key_tab = 0x2B,
  key_q = 0x14,
  key_w = 0x1A,
  key_e = 0x08,
  key_r = 0x15,
  key_t = 0x17,
  key_y = 0x1C,
  key_u = 0x18,
  key_i = 0x0C,
  key_o = 0x12,
  key_p = 0x13,
  key_left_bracket = 0x2F,
  key_right_bracket = 0x30,
  key_backslash_104 = 0x31,
  key_capslock = 0x39,
  key_a = 0x04,
  key_s = 0x16,
  key_d = 0x07,
  key_f = 0x09,
  key_g = 0x0A,
  key_h = 0x0B,
  key_j = 0x0D,
  key_k = 0x0E,
  key_l = 0x0F,
  key_semicolon = 0x33,
  key_quote = 0x34,
  key_backslash = 0x32,
  key_enter = 0x28,
  key_left_shift = 0xE1,
  key_z = 0x1D,
  key_x = 0x1B,
  key_c = 0x06,
  key_v = 0x19,
  key_b = 0x05,
  key_n = 0x11,
  key_m = 0x10,
  key_comma = 0x36,
  key_dot = 0x37,
  key_slash = 0x38,
  key_right_shift = 0xE5,
  key_left_ctrl = 0xE0,
  key_left_alt = 0xE2,
  key_space = 0x2C,
  key_right_alt = 0xE6,
  key_right_ctrl = 0xE4,
  key_insert = 0x49,
  key_delete = 0x4C,
  key_left_arrow = 0x50,
  key_home = 0x4A,
  key_end = 0x4D,
  key_up_arrow = 0x52,
  key_down_arrow = 0x51,
  key_page_up = 0x4B,
  key_page_down = 0x4E,
  key_right_arrow = 0x4F,
  key_num_lock = 0x53,
  key_numpad_7 = 0x5F,
  key_numpad_8 = 0x60,
  key_numpad_9 = 0x61,
  key_numpad_minus = 0x56,
  key_numpad_4 = 0x5C,
  key_numpad_5 = 0x5D,
  key_numpad_6 = 0x5E,
  key_numpad_plus = 0x57,
  key_numpad_1 = 0x59,
  key_numpad_2 = 0x5A,
  key_numpad_3 = 0x5B,
  key_numpad_0 = 0x62,
  key_numpad_dot = 0x63,
  key_numpad_enter = 0x58,
  key_esc = 0x29,
  key_f1 = 0x3A,
  key_f2 = 0x3B,
  key_f3 = 0x3C,
  key_f4 = 0x3D,
  key_f5 = 0x3E,
  key_f6 = 0x3F,
  key_f7 = 0x40,
  key_f8 = 0x41,
  key_f9 = 0x42,
  key_f10 = 0x43,
  key_f11 = 0x44,
  key_f12 = 0x45,
  key_print_screen = 0x46,
  key_scroll_lock = 0x47,
  key_pause = 0x48,
  key_left_win = 0xE3,
  key_right_win = 0xE7,
  key_app = 0x65,
} ch9329_keycode_t;

typedef struct {
  ch9329_function_keys_t func;
  uint8_t zero;
  ch9329_keycode_t key1;
  ch9329_keycode_t key2;
  ch9329_keycode_t key3;
  ch9329_keycode_t key4;
  ch9329_keycode_t key5;
  ch9329_keycode_t key6;
} packed ch9329_keystat_t;

#ifdef _CH9329_INTERNAL

#define CH9329_FRAME_HEAD_0 0x57
#define CH9329_FRAME_HEAD_1 0xAB

#define CH9329_CMD_GET_INFO             0x01
#define CH9329_CMD_SEND_KB_GENERAL_DATA 0x02
#define CH9329_CMD_SEND_KB_MEDIA_DATA   0x03
// #define CH9329_CMD_SEND_MS_ABS_DATA     0x04
// #define CH9329_CMD_SEND_MS_REL_DATA     0x05
#define CH9329_CMD_SEND_MY_HID_DATA     0x06
#define CH9329_CMD_READ_MY_HID_DATA     0x87
#define CH9329_CMD_GET_PARA_CFG         0x08
#define CH9329_CMD_SET_PARA_CFG         0x09
#define CH9329_CMD_GET_USB_STRING       0x0A
#define CH9329_CMD_SET_USB_STRING       0x0B
#define CH9329_CMD_SET_DEFAULT_CFG      0x0C
#define CH9329_CMD_RESET                0x0F

// buffer used to send or recv packet
#define MAX_BUFFER_SIZE (64)
#define MAX_PACKET_SIZE (2 + 1 + 1 + 1 + MAX_BUFFER_SIZE + 1)
#define PACKET_SIZE(pkt) (2 + 1 + 1 + 1 + (pkt)->length + 1)

typedef struct {
  uint16_t magic;
  uint8_t  address;
  uint8_t  command;
  uint8_t  length;
  uint8_t  buffer[MAX_BUFFER_SIZE];
} packet_t;

#endif

void ch9329_init();

ch9329_stat_t ch9329_get_info(ch9329_chip_info_t **info);

// ch9329_stat_t ch9329_send_kb_media_data(uint8_t key1, uint8_t key2);
// void ch9329_send_ms_abs_data();
// void ch9329_send_ms_rel_data();
ch9329_stat_t ch9329_send_my_hid_data(char* data, uint8_t length); //size:0~64
// void ch9329_read_my_hid_data();

ch9329_stat_t ch9329_get_para_cfg(ch9329_para_cfg_t** config);
ch9329_stat_t ch9329_set_para_cfg(ch9329_para_cfg_t* config);

ch9329_stat_t ch9329_get_usb_string(ch9329_usbstr_t type, char** str, uint8_t *length);
ch9329_stat_t ch9329_set_usb_string(char* data, uint8_t length);

ch9329_stat_t ch9329_set_default_cfg();

ch9329_stat_t ch9329_send_keys(ch9329_keystat_t *stat);
ch9329_stat_t ch9329_release_keys();

ch9329_stat_t ch9329_reset();

#endif /* __SYS_HARDWARE_CH9329_H */
