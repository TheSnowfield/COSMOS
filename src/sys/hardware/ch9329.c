#include <config.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define _CH9329_INTERNAL
#include <hardware/ch9329.h>
#undef _CH9329_INTERNAL

void ch9329_init() {

  HAL_GPIO_Init(USART_PORT, &port_usart1_ctl);
  //HAL_GPIO_WritePin(USART_PORT, USART_PIN_CTL_RST, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(USART_PORT, USART_PIN_CTL_RST, GPIO_PIN_RESET);

	__HAL_RCC_USART1_CLK_ENABLE();
  HAL_GPIO_Init(USART_PORT, &port_usart1);
	HAL_UART_Init(&usart1);

  // ch9329_stat_t stat;
  // ch9329_para_cfg_t* paracfg;
  // if(ch9329_get_para_cfg(&paracfg) == ok) {
  //   int x = paracfg->address;
  //   // printf("CH9329 chip info: %s\n", chipinfo->usb_connected);
  // }
	
	// char* usbstr = NULL;
	// uint8_t length = 0;
	// if(ch9329_get_usb_string(manufacturer, &usbstr, &length)) {
	// usbstr[0] = '1';
	
	// }
	
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
  if(!packet) return; {
    packet->command = command;
    if(!data) return; {
      packet->length = length;
      memcpy(packet->buffer, data, length);
    }
  }
}

void _packet_update_checksum(packet_t *packet) {
  if(!packet) return; {
    uint8_t *ptr = (uint8_t *)packet;
    uint8_t checksum = 0;
    for(uint8_t i = 0; i < PACKET_SIZE(packet) - 1; ++i) {
      checksum += ptr[i];
    }
    packet->buffer[packet->length] = checksum;
  }
}

ch9329_stat_t ch9329_send_packet(uint8_t command, uint8_t *data, uint8_t length,
                            void** recvdata, uint8_t *recvlen) {
  packet_t packet; {
    _packet_init(&packet, 0x00);
    _packet_set_command(&packet, command, data, length);
    _packet_update_checksum(&packet);
  }

  // send data
  HAL_UART_Transmit(&usart1, (uint8_t *)&packet, PACKET_SIZE(&packet), 200);
  HAL_UART_Receive(&usart1, (uint8_t *)&packet, MAX_BUFFER_SIZE, 50);

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
  if(recvdata) *recvdata = packet.buffer;

  return ok;
}

ch9329_stat_t ch9329_get_info(ch9329_chip_info_t** info) {
  return ch9329_send_packet(CH9329_CMD_GET_INFO, NULL, 0, (void **)info, NULL);
}

ch9329_stat_t ch9329_get_para_cfg(ch9329_para_cfg_t** config) {
  return ch9329_send_packet(CH9329_CMD_GET_PARA_CFG, NULL, 0, (void **)config, NULL);
}

ch9329_stat_t ch9329_set_para_cfg(ch9329_para_cfg_t* config) {
  return ch9329_send_packet(CH9329_CMD_SET_PARA_CFG, NULL, 0, NULL, NULL);
}

ch9329_stat_t ch9329_get_usb_string(ch9329_usbstr_t type, char** str, uint8_t *length) {

  ch9329_stat_t ret = ch9329_send_packet(CH9329_CMD_GET_USB_STRING, (uint8_t *)&type, 1, (void **)str, length);
  if(ret != ok) return ret;
  if(*length == 0) return error;

  // 1 byte for type
  // 1 byte for length
  // n bytes for string
  *length = *((*str) + 1);
  *str += 2;

  return ret;
}

ch9329_stat_t ch9329_set_default_cfg() {
  return ch9329_send_packet(CH9329_CMD_SET_DEFAULT_CFG, NULL, 0, NULL, NULL);
}

ch9329_stat_t ch9329_reset() {
  return ch9329_send_packet(CH9329_CMD_RESET, NULL, 0, NULL, NULL);
}

ch9329_stat_t ch9329_send_keys(ch9329_keystat_t *stat) {
  stat->zero = 0;
  return ch9329_send_packet(CH9329_CMD_SEND_KB_GENERAL_DATA, (uint8_t *)stat, sizeof(ch9329_keystat_t), NULL, NULL);
}

ch9329_stat_t ch9329_release_keys() {
  return ch9329_send_keys(&(ch9329_keystat_t) {});
}

// ch9329_stat_t ch9329_set_usb_string(char* data, uint8_t length);

// void ch9329_send_keys(uint8_t *data) {
//   uint8_t recvlen;
//   data_t* recvdata;
//   ch9329_send_packet(CH9329_CMD_SEND_KB_GENERAL_DATA, data, 8, &recvdata, &recvlen);
  
// }

// stat_t ch9329_get_info(ch9329_chip_info_t *info){
//   const char cmd_ver[] = "\x57\xAB\x00\x01\x00\x03";
//   HAL_UART_Transmit(&usart1, (uint8_t *)cmd_ver, sizeof(cmd_rst), 1000);
//   info.xxx
//   info.xxx
//   return ok;
// }
