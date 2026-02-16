# Xteink X4 ESPHome Component

ESPHome components for Xteink X4 e-reader (ESP32-C3).

Currently, this repo only contains the display component. Button components will be added soon.

## Usage

See [./example_xteink_full.yml](example_xteink_full.yml) for a complete example.

Requires `partitions.csv` and `package_xteink_x4.yml` from this repo:

```yaml
external_components:
  - source: github://ngxson/esphome-component-xteink
    components:
      - xteink_edp
      - xteink_battery
      - xteink_input

# IMPORANT: this contains the hardware definitions of Xteink X4
packages:
  common: !include
    file: package_xteink_x4.yml

# Battery level
sensor:
  - platform: xteink_battery
    battery_level:
      id: bat_lvl
      name: "Battery level"
      # You can add any other sensor options here

# Buttons
# Note: 4 front buttons are mapped to 1, 2, 3, 4
binary_sensor:
  - platform: xteink_input
    button_1:
      id: btn_1
      # You can add any other binary_sensor options here
    button_2:
      id: btn_2
    button_3:
      id: btn_3
    button_4:
      id: btn_4
    button_up:
      id: btn_up
    button_down:
      id: btn_down
    button_pwr:
      id: btn_pwr

font:
  - file: "gfonts://Roboto"
    id: roboto_20
    size: 20

display:
  - platform: xteink_edp
    # ... See example_xteink_edp.yml for the complete code
```

This outputs:

![demo](demo.jpeg)

Advanced options:

```yaml
# EXAMPLE: custom refresh mode
display:
  - platform: xteink_edp
    lambda: |-
      // draw your content, then set the refresh mode
      it.set_refresh_mode(1);

      // available modes:
      // - 0 (FULL_REFRESH): Full refresh with complete waveform
      // - 1 (HALF_REFRESH): Half refresh (1720ms) - balanced quality and speed
      // - 2 (FAST_REFRESH): Fast refresh (partial update)

      // you can also manually track the display update count
      // for example, force full update after 10 updates
      if (it.update_count % 10 == 0) {
        it.set_refresh_mode(1); // half update
      } else {
        it.set_refresh_mode(2); // fast update
      }
```

When pressing any buttons, you can see in the log:

```
[14:13:43.006][D][binary_sensor:047]: 'btn_pwr' >> ON
[14:13:43.270][D][binary_sensor:047]: 'btn_pwr' >> OFF
[14:13:43.747][D][binary_sensor:047]: 'btn_4' >> ON
[14:13:44.172][D][binary_sensor:047]: 'btn_4' >> OFF
[14:13:44.278][D][binary_sensor:047]: 'btn_4' >> ON
[14:13:44.384][D][binary_sensor:047]: 'btn_4' >> OFF
[14:13:44.543][D][binary_sensor:047]: 'btn_4' >> ON
[14:13:44.648][D][binary_sensor:047]: 'btn_4' >> OFF
[14:13:44.755][D][binary_sensor:047]: 'btn_1' >> ON
[14:13:44.966][D][binary_sensor:047]: 'btn_1' >> OFF
[14:13:45.125][D][binary_sensor:047]: 'btn_3' >> ON
[14:13:45.338][D][binary_sensor:047]: 'btn_2' >> ON
[14:13:45.338][D][binary_sensor:047]: 'btn_3' >> OFF
[14:13:45.551][D][binary_sensor:047]: 'btn_2' >> OFF
[14:13:45.551][D][binary_sensor:047]: 'btn_4' >> ON
[14:13:45.711][D][binary_sensor:047]: 'btn_1' >> ON
[14:13:45.711][D][binary_sensor:047]: 'btn_4' >> OFF
[14:13:45.870][D][binary_sensor:047]: 'btn_1' >> OFF
[14:13:46.188][D][binary_sensor:047]: 'btn_down' >> ON
[14:13:46.400][D][binary_sensor:047]: 'btn_down' >> OFF
[14:13:46.559][D][binary_sensor:047]: 'btn_up' >> ON
[14:13:46.719][D][binary_sensor:047]: 'btn_up' >> OFF
```
