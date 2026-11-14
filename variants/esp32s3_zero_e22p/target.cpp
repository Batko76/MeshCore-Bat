#include <Arduino.h>
#include "target.h"

ESP32Board board;

static SPIClass spi;
static Module* mod = nullptr;
static CustomSX1262* radio = nullptr;

WRAPPER_CLASS radio_driver(*radio, board);
AutoDiscoverRTCClock rtc_clock;
SensorManager sensors;

bool radio_init() {
  // Запуск аппаратной шины SPI на пинах ESP32-S3-Zero
  spi.begin(P_LORA_SCLK, P_LORA_MISO, P_LORA_MOSI, P_LORA_NSS);
  
  mod = new Module(P_LORA_NSS, P_LORA_DIO_1, P_LORA_RESET, P_LORA_BUSY, spi);
  radio = new CustomSX1262(mod);
  radio_driver = WRAPPER_CLASS(*radio, board);

  // Настройка управления внешним ВЧ-усилителем E22P (пины RXEN и TXEN)
  radio->setRfSwitchPins(SX126X_RXEN, SX126X_TXEN);

  // Базовая инициализация SX1262 с питанием TCXO 1.8В от DIO3
  int state = radio->begin(
    869.618, 62.5, 8, 8, 
    RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 
    22, 16, 
    SX126X_DIO3_TCXO_VOLTAGE, 
    false
  );

  if (state != RADIOLIB_ERR_NONE) {
    return false;
  }

  // Включение режима повышенной чувствительности приема (Rx Boosted Gain)
  radio->setRxBoostedGainMode(true);
  return true;
}

uint32_t radio_get_rng_seed() {
  return radio->random(0x7FFFFFFF);
}

void radio_set_params(float freq, float bw, uint8_t sf, uint8_t cr) {
  radio->setFrequency(freq);
  radio->setBandwidth(bw);
  radio->setSpreadingFactor(sf);
  radio->setCodingRate(cr);
}

void radio_set_tx_power(uint8_t dbm) {
  if (dbm > SX126X_MAX_POWER) {
    dbm = SX126X_MAX_POWER;
  }
  radio->setOutputPower(dbm);
}

mesh::LocalIdentity radio_new_identity() {
  RadioNoiseListener rng(*radio);
  return mesh::LocalIdentity(&rng);
}
