# Register the component
idf_component_register(
    SRCS
        # List of source files
        "app_main.c"
        # "led_task.c"
        "app_pm.c"
        "app_ble_4.c"
        "app_ble_ota.c"

        "lcd_task.c"
    #     ui/screens/ui_Splash.c
    # ui/ui.c
    # ui/components/ui_comp_hook.c
    # ui/ui_helpers.c
    # ui/ui_events.c
    # ui/images/ui_img_fc_240_png.c
    # ui/fonts/ui_font_Segoe_UI_B20.c
    # ui/fonts/ui_font_Segoe_UI_B25.c
    # ui/fonts/ui_font_Segoe_UI_B95.c
    # ui/fonts/ui_font_Segoe_UI_SB14.c
    # ui/fonts/ui_font_Segoe_UI_SB50.c
    # ui/fonts/ui_font_Segoe_UI_SB65.c
     INCLUDE_DIRS
        # List of public include directories
        "."
     REQUIRES
     "ui"   
     "services"

)
