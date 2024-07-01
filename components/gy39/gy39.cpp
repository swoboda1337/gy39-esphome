#include "gy39.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace gy39 {

static const char *const TAG = "gy39";

void GY39::update() {
  uint8_t data[14];

  // read data
  data[0] = 14;
  if (this->write(data, 1) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Write failed!");
    this->mark_failed();
    return;
  }
  if (this->read(data, 14) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Read failed!");
    this->mark_failed();
    return;
  }
  ESP_LOGD(TAG, "Raw data: %s", format_hex(data, 14).c_str());

  // convert
  int32_t lux = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
  int16_t temperature = (data[4] << 8) | data[5];
  int32_t pressure = (data[6] << 24) | (data[7] << 16) | (data[8] << 8) | data[9];
  int16_t humidity = (data[10] << 8) | data[11];
  int16_t altitude = (data[12] << 8) | data[13];

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
