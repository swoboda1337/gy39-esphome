#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace gy39 {

class GY39 : public PollingComponent, public i2c::I2CDevice {
 public:
  void dump_config() override;
  void update() override;
  float get_setup_priority() const override;

  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { this->temperature_sensor_ = temperature_sensor; }
  void set_humidity_sensor(sensor::Sensor *humidity_sensor) { this->humidity_sensor_ = humidity_sensor; }
  void set_pressure_sensor(sensor::Sensor *pressure_sensor) { this->pressure_sensor_ = pressure_sensor; }
  void set_altitude_sensor(sensor::Sensor *altitude_sensor) { this->altitude_sensor_ = altitude_sensor; }
  void set_lux_sensor(sensor::Sensor *lux_sensor) { this->lux_sensor_ = lux_sensor; }

 protected:
  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};
  sensor::Sensor *pressure_sensor_{nullptr};
  sensor::Sensor *altitude_sensor_{nullptr};
  sensor::Sensor *lux_sensor_{nullptr};
};

}  // namespace gy39
}  // namespace esphome
