#include "gy39.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace gy39 {

static const char *const TAG = "gy39";

void GY39::update() {
  uint8_t data[10];

  // read data
  data[0] = 0x04;
  if (this->write(data, 1) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Write failed!");
    this->mark_failed();
    return;
  }
  if (this->read(data, 10) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Read failed!");
    this->mark_failed();
    return;
  }

  // convert
  int16_t temperature = (data[0] << 8) | data[1];
  int32_t pressure = (data[2] << 24) | (data[3] << 16) | (data[4] << 8) | data[5];
  int16_t humidity = (data[6] << 8) | data[7];
  int16_t altitude = (data[8] << 8) | data[9];

  // is this delay needed?
  delay(1);

  // read data
  data[0] = 0x00;
  if (this->write(data, 1) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Write failed!");
    this->mark_failed();
    return;
  }
  if (this->read(data, 4) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Read failed!");
    this->mark_failed();
    return;
  }

  // convert
  int32_t lux = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];

  // update sensors
  if (this->temperature_sensor_ != nullptr) {
    this->temperature_sensor_->publish_state(temperature / 100.0f);
  }
  if (this->humidity_sensor_ != nullptr) {
    this->humidity_sensor_->publish_state(humidity / 100.0f);
  }
  if (this->pressure_sensor_ != nullptr) {
    this->pressure_sensor_->publish_state(pressure / 100.0f);
  }
  if (this->altitude_sensor_ != nullptr) {
    this->altitude_sensor_->publish_state(altitude);
  }
  if (this->lux_sensor_ != nullptr) {
    this->lux_sensor_->publish_state(lux / 100.0f);
  }
  this->status_clear_warning();
}

void GY39::dump_config() {
  ESP_LOGCONFIG(TAG, "gy39:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Communication with gy39 failed!");
  }
  LOG_SENSOR("  ", "Temperature", this->temperature_sensor_);
  LOG_SENSOR("  ", "Humidity", this->humidity_sensor_);
  LOG_SENSOR("  ", "Pressure", this->pressure_sensor_);
  LOG_SENSOR("  ", "Altitude", this->altitude_sensor_);
  LOG_SENSOR("  ", "Lux", this->lux_sensor_);
}

float GY39::get_setup_priority() const { return setup_priority::DATA; }

}  // namespace gy39
}  // namespace esphome
