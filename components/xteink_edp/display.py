import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display, spi
from esphome.const import CONF_ID, CONF_LAMBDA

# DEPENDENCIES = ["spi"]

xteink_edp_ns = cg.esphome_ns.namespace("xteink_edp")
XteinkEDP = xteink_edp_ns.class_(
    #"XteinkEDP", cg.PollingComponent, spi.SPIDevice, display.DisplayBuffer
    "XteinkEDP", cg.PollingComponent, display.DisplayBuffer
)
XteinkEDPRef = XteinkEDP.operator("ref")

CONFIG_SCHEMA = cv.All(
    display.FULL_DISPLAY_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(XteinkEDP),
        }
    )
    .extend(cv.polling_component_schema("1s"))
)

async def to_code(config):
    rhs = XteinkEDP.new()
    var = cg.Pvariable(config[CONF_ID], rhs, XteinkEDP)

    if CONF_LAMBDA in config:
        lambda_ = await cg.process_lambda(
            config[CONF_LAMBDA], [(XteinkEDPRef, "it")], return_type=cg.void
        )
        cg.add(var.set_writer(lambda_))

    await display.register_display(var, config)
