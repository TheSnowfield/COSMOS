#include <config.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define _CH9329_INTERNAL
#include <hardware/ch9329.h>
#undef _CH9329_INTERNAL

void ch9329_init() {
}

void _packet_init(packet_t *packet, uint8_t address) {
  if(!packet) return;
  memset(packet, 0, sizeof(packet_t)); {
    packet->magic   = 0xAB57;
    packet->address = address;
    packet->length  = 0;
  }
}

void _packet_set_command(packet_t *packet, uint8_t command,
                         uint8_t *data, uint8_t length) {
  if(!packet) return;
  packet->command = command;
  
  if(!data) return;
  packet->length = length;
  memcpy(packet->buffer, data, length);
}

void _packet_update_checksum(packet_t* packet) {
  if(!packet) return;
  uint8_t checksum = 0;
  uint8_t* ptr = (uint8_t *)packet;
  for(uint8_t i = 0; i < PACKET_SIZE(packet) - 1; ++i) checksum += ptr[i];
  packet->buffer[packet->length] = checksum;
}

status_t ch9329_send_packet(uint8_t command, uint8_t* data, uint8_t length,
                            void* recvdata, uint8_t* recvlen) {

  // init a packet
  packet_t packet; {
    _packet_init(&packet, 0x00);
    _packet_set_command(&packet, command, data, length);
    _packet_update_checksum(&packet);
  }

  // send data
  HAL_UART_Transmit(&usart1, (uint8_t *)&packet, PACKET_SIZE(&packet), 200);
  HAL_UART_Receive(&usart1, (uint8_t *)&packet, MAX_BUFFER_SIZE, 500);

  // check length
  if(packet.length > 64) return error;

  // check checksum
  uint8_t checksum = packet.buffer[packet.length]; {
    _packet_update_checksum(&packet);
    if(checksum != packet.buffer[packet.length]) return error;
  }

  // check address
  // if(packet.address != 0x00) return error;

  // set output data
  if(recvlen) *recvlen = packet.length;
  if(recvdata) memcpy(recvdata, packet.buffer, packet.length);

  return ok;
}

status_t ch9329_get_info(ch9329_chip_info_t* info) {
  return ch9329_send_packet(CH9329_CMD_GET_INFO, NULL, 0, (void **)info, NULL);
}

status_t ch9329_get_para_cfg(ch9329_para_cfg_t* config) {
  return ch9329_send_packet(CH9329_CMD_GET_PARA_CFG, NULL, 0, config, NULL);
}

status_t ch9329_set_para_cfg(ch9329_para_cfg_t* config) {
  return ch9329_send_packet(CH9329_CMD_SET_PARA_CFG, NULL, 0, NULL, NULL);
}

status_t ch9329_get_usb_string(ch9329_usbstr_t type, char* str, uint8_t *length) {

  uint8_t buff[32];
  uint8_t bufflen = sizeof(buff);
  status_t ret = ch9329_send_packet(CH9329_CMD_GET_USB_STRING, (uint8_t *)&type, 1, buff, &bufflen);
  if(ret != ok) return ret;
  if(*length == 0) return error;

  // // 1 byte for type
  // // 1 byte for length
  // // n bytes for string
  *length = bufflen - 2;
  strncpy(str, (char *)buff + 2, *length);

  return ok;
}

status_t ch9329_set_usb_string(ch9329_usbstr_t type, char* data) {

  uint8_t buff[strlen(data) + 2]; {
    buff[0] = type;
    buff[1] = strlen(data);
    memcpy(buff + 2, data, buff[1]);
  }

  return ch9329_send_packet(CH9329_CMD_SET_USB_STRING, buff, sizeof(buff), NULL, NULL);
}

status_t ch9329_set_default_cfg() {
  return ch9329_send_packet(CH9329_CMD_SET_DEFAULT_CFG, NULL, 0, NULL, NULL);
}

status_t ch9329_reset() {
  return ch9329_send_packet(CH9329_CMD_RESET, NULL, 0, NULL, NULL);
}

status_t ch9329_send_keys(ch9329_keystat_t *stat) {
  stat->zero = 0;
  return ch9329_send_packet(CH9329_CMD_SEND_KB_GENERAL_DATA, (uint8_t *)stat, sizeof(ch9329_keystat_t), NULL, NULL);
}

status_t ch9329_release_keys() {
  return ch9329_send_keys(&(ch9329_keystat_t) {});
}

status_t ch9329_soft_reset() {
  return ch9329_send_packet(CH9329_CMD_RESET, NULL, 0, NULL, NULL);
}
