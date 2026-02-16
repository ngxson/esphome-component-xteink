#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace xteink_battery {

class XteinkBattery : public PollingComponent {
 public:
  void setup() override;
  void dump_config() override;
  void update() override;

  void set_battery_level(sensor::Sensor *battery_level) { battery_level_ = battery_level; }

 protected:
  sensor::Sensor *battery_level_{nullptr};
};

}  // namespace xteink_battery
}  // namespace esphome
