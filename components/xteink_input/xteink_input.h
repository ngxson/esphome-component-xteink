#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace xteink_input {

class XteinkInput : public PollingComponent {
 public:
  void setup() override;
  void dump_config() override;
  void update() override;

  void set_button(int idx, binary_sensor::BinarySensor *button);

 protected:
  binary_sensor::BinarySensor *btn_1_{nullptr};
  binary_sensor::BinarySensor *btn_2_{nullptr};
  binary_sensor::BinarySensor *btn_3_{nullptr};
  binary_sensor::BinarySensor *btn_4_{nullptr};
  binary_sensor::BinarySensor *btn_up_{nullptr};
  binary_sensor::BinarySensor *btn_down_{nullptr};
  binary_sensor::BinarySensor *btn_pwr_{nullptr};
};

}  // namespace xteink_input
}  // namespace esphome
