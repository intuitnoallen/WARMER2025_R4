# Register the component
idf_component_register(
    SRCS
        # List of source files
        "app_buzzer.c"
        "app_vibrator.c"
        "app_heater.c"
        "app_motor.c"
        "encoder.c"
        "app_notification.c"
        # "musical_buzzer_rmt.c"
        
    INCLUDE_DIRS
        # List of public include directories
        "."
    PRIV_INCLUDE_DIRS
        # List of private include directories
    REQUIRES
        # List of public required components
        "driver"
        "common"
        "hwa_gpio"
    PRIV_REQUIRES
        # List of private required components
        "app_warmer_mngr"
        
        
)
