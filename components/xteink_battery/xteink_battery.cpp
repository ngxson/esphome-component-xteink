#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "xteink_battery.h"

/*

Copied from https://github.com/open-x4-epaper/community-sdk

---

MIT License

Copyright (c) 2025 Open X4 E-Paper Contributors

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

---

 */


#include <cstdint>

class BatteryMonitor {
public:
    // Optional divider multiplier parameter defaults to 2.0
    explicit BatteryMonitor(uint8_t adcPin, float dividerMultiplier = 2.0f);

    // Read voltage and return percentage (0-100)
    uint16_t readPercentage() const;

    // Read the battery voltage in millivolts (accounts for divider)
    uint16_t readMillivolts() const;

    // Read raw millivolts from ADC (doesn't account for divider)
    uint16_t readRawMillivolts() const;

    // Read the battery voltage in volts (accounts for divider)
    double readVolts() const;

    // Percentage (0-100) from a millivolt value
    static uint16_t percentageFromMillivolts(uint16_t millivolts);

    // Calibrate a raw ADC reading and return millivolts
    static uint16_t millivoltsFromRawAdc(uint16_t adc_raw);

private:
    uint8_t _adcPin;
    float _dividerMultiplier;
};

#include <esp32-hal-adc.h>
#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_cali_scheme.h>

inline float min(const float a, const float b) { return a < b ? a : b; }
inline float max(const float a, const float b) { return a > b ? a : b; }

BatteryMonitor::BatteryMonitor(uint8_t adcPin, float dividerMultiplier)
  : _adcPin(adcPin), _dividerMultiplier(dividerMultiplier)
{
}

uint16_t BatteryMonitor::readPercentage() const
{
    return percentageFromMillivolts(readMillivolts());
}

uint16_t BatteryMonitor::readMillivolts() const
{
    const uint16_t raw = readRawMillivolts();
    const uint32_t mv = millivoltsFromRawAdc(raw);
    return static_cast<uint32_t>(mv * _dividerMultiplier);
}

uint16_t BatteryMonitor::readRawMillivolts() const
{
    const uint16_t raw = analogRead(_adcPin);
    return raw;
}

double BatteryMonitor::readVolts() const
{
    return static_cast<double>(readMillivolts()) / 1000.0;
}

uint16_t BatteryMonitor::percentageFromMillivolts(uint16_t millivolts)
{
    double volts = millivolts / 1000.0;
    // Polynomial derived from LiPo samples
    double y = -144.9390 * volts * volts * volts +
               1655.8629 * volts * volts -
               6158.8520 * volts +
               7501.3202;

    // Clamp to [0,100] and round
    y = max(y, 0.0);
    y = min(y, 100.0);
    y = round(y);
    return static_cast<int>(y);
}

uint16_t BatteryMonitor::millivoltsFromRawAdc(uint16_t adc_raw)
{
    static adc_cali_handle_t cali_handle = nullptr;
    if (cali_handle == nullptr) {
#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
        adc_cali_curve_fitting_config_t cali_config = {
            .unit_id = ADC_UNIT_1,
            .atten = ADC_ATTEN_DB_12,
            .bitwidth = ADC_BITWIDTH_12,
        };
        adc_cali_create_scheme_curve_fitting(&cali_config, &cali_handle);
#elif ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = ADC_UNIT_1,
            .atten = ADC_ATTEN_DB_12,
            .bitwidth = ADC_BITWIDTH_12,
            .default_vref = 1100,
        };
        adc_cali_create_scheme_line_fitting(&cali_config, &cali_handle);
#endif
    }
    int voltage = 0;
    adc_cali_raw_to_voltage(cali_handle, adc_raw, &voltage);
    return static_cast<uint16_t>(voltage);
}








//
// XteinkBattery
//

namespace esphome {
namespace xteink_battery {

static const char *TAG = "xteink_battery";
static constexpr int BATT_PIN = 0; // ADC pin 0

static BatteryMonitor battery = BatteryMonitor(BATT_PIN); // ADC pin 0

void XteinkBattery::setup() {
  pinMode(BATT_PIN, INPUT);
}

void XteinkBattery::dump_config() {
  ESP_LOGCONFIG(TAG, "Xteink Battery Monitor:");
  ESP_LOGCONFIG(TAG, "  ADC Pin: %d", BATT_PIN);
}

void XteinkBattery::update() {
  if (battery_level_ == nullptr) {
    ESP_LOGE(TAG, "Battery level sensor not set!");
    return;
  }

  uint16_t percentage = battery.readPercentage();
  battery_level_->publish_state(percentage);
}

}  // namespace xteink_battery
}  // namespace esphome
