#pragma once

// Распиновка шины SPI для модуля E22P-868M30S
#define P_LORA_SCLK     12
#define P_LORA_MISO     13
#define P_LORA_MOSI     11
#define P_LORA_NSS      10

// Линии управления чипом SX1262 и ВЧ-усилителем
#define P_LORA_RESET    4
#define P_LORA_BUSY     5
#define P_LORA_DIO_1    6
#define SX126X_TXEN     7
#define SX126X_RXEN     8

// Параметры работы E22P-868M30S
#define SX126X_DIO2_AS_RF_SWITCH  0
#define SX126X_DIO3_TCXO_VOLTAGE  1.8
#define SX126X_MAX_POWER          22

// Отключение дисплея и лишней периферии
#define HAS_DISPLAY     0
#define HAS_SDCARD      0
#define HAS_GPS         0
