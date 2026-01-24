#pragma once

#include "esphome/core/component.h"
// #include "esphome/components/spi/spi.h"
#include "esphome/components/display/display_buffer.h"

namespace esphome {
namespace xteink_edp {

class XteinkEDP;

using xteink_edp_writer_t = std::function<void(XteinkEDP &)>;

class XteinkEDP : public display::DisplayBuffer {
                    //  public spi::SPIDevice<spi::BIT_ORDER_LSB_FIRST, spi::CLOCK_POLARITY_LOW,
                    //                        spi::CLOCK_PHASE_LEADING, spi::DATA_RATE_40MHZ> {
 public:
  display::DisplayType get_display_type() override { return display::DisplayType::DISPLAY_TYPE_BINARY; }

  enum RefreshMode {
    FULL_REFRESH = 0,
    HALF_REFRESH = 1,
    FAST_REFRESH = 2,
  };

  int get_width_internal() override;
  int get_height_internal() override;
  void draw_absolute_pixel_internal(int x, int y, Color color) override;

  void update() override;
  void setup() override;

  void set_writer(xteink_edp_writer_t &&writer) { this->writer_local_ = writer; }
  void set_refresh_mode(int mode) { this->refresh_mode_ = static_cast<RefreshMode>(mode); }

  uint64_t update_count{0}; // for manually tracking number of updates, can be used in writer function
  optional<xteink_edp_writer_t> writer_local_{};

 private:
  class Impl;
  Impl* impl_;

  RefreshMode refresh_mode_{HALF_REFRESH};
  bool has_drawn_{false}; // has the display been drawn at least once?
};

}
}
