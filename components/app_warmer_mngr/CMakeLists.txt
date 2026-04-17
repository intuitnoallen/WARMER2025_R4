# Register the component
idf_component_register(
    SRCS
        # List of source files
        "app_warmer_mngr.c"
    INCLUDE_DIRS
        # List of public include directories
        "."
    PRIV_INCLUDE_DIRS
        # List of private include directories
    REQUIRES
        # List of public required components
        "common"
    PRIV_REQUIRES
        # List of private required components
        "srvc_button"
        "ui"
        # "app_buzzer"
        "srvc_thermistor"
        "srvc_ps_mon"
        "hwa_gpio"
        "services"
        "main"
)
