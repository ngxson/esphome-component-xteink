import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_BUTTON,
)

CONF_BUTTON_1 = "button_1"
CONF_BUTTON_2 = "button_2"
CONF_BUTTON_3 = "button_3"
CONF_BUTTON_4 = "button_4"
CONF_BUTTON_UP = "button_up"
CONF_BUTTON_DOWN = "button_down"
CONF_BUTTON_PWR = "button_pwr"

xteink_input_ns = cg.esphome_ns.namespace("xteink_input")
XteinkInput = xteink_input_ns.class_(
    "XteinkInput", cg.PollingComponent, cg.Component
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(XteinkInput),
    }
).extend(
    {
        cv.Optional(button): binary_sensor.binary_sensor_schema() \
        for button in [
            CONF_BUTTON_1,
            CONF_BUTTON_2,
            CONF_BUTTON_3,
            CONF_BUTTON_4,
            CONF_BUTTON_UP,
            CONF_BUTTON_DOWN,
            CONF_BUTTON_PWR,
        ]
    }
).extend(cv.polling_component_schema("50ms"))

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    for i, button in enumerate([
        CONF_BUTTON_1,
        CONF_BUTTON_2,
        CONF_BUTTON_3,
        CONF_BUTTON_4,
        CONF_BUTTON_UP,
        CONF_BUTTON_DOWN,
        CONF_BUTTON_PWR,
    ]):
        if button in config:
            sens = await binary_sensor.new_binary_sensor(config[button])
            cg.add(var.set_button(i, sens))
