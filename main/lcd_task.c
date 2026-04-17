/*
 * SPDX-FileCopyrightText: 2021-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/lock.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/gpio.h"
#include "esp_sleep.h"
#include "driver/rtc_io.h"
#include "driver/ledc.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"
#include "ui.h"
#include "ui_common.h"
#include "app_notification.h"
#include "esp_lcd_gc9a01.h"
#include "esp_task_wdt.h"
#include "app_warmer_mngr.h"
#include "lcd_task.h"

  

#if CONFIG_LCD_CONTROLLER_ILI9341
#include "esp_lcd_ili9341.h"
#elif CONFIG_LCD_CONTROLLER_GC9A01

#endif

#if CONFIG_LCD_TOUCH_CONTROLLER_STMPE610
#include "esp_lcd_touch_stmpe610.h"
#elif CONFIG_LCD_TOUCH_CONTROLLER_XPT2046
#include "esp_lcd_touch_xpt2046.h"
#endif

static const char *TAG = "TFT GC9A01";


// Using SPI2
#define LCD_HOST  SPI2_HOST

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define LCD_PIXEL_CLOCK_HZ          (20 * 1000 * 1000)
#define LCD_BK_LIGHT_ON_LEVEL       0
#define LCD_BK_LIGHT_OFF_LEVEL      !LCD_BK_LIGHT_ON_LEVEL
#define LCD_PIN_NUM_SCLK            18
#define LCD_PIN_NUM_MOSI            23//19
#define LCD_PIN_NUM_MISO            -1
#define LCD_PIN_NUM_LCD_DC          21//5
#define LCD_PIN_NUM_LCD_RST         22//3
#define LCD_PIN_NUM_LCD_CS          5//4
#define LCD_PIN_NUM_BK_LIGHT        32              //Sleep_aux to enable VCC_AUX
#define LCD_PIN_NUM_TOUCH_CS        -1//10//15

// The pixel number in horizontal and vertical
#define LCD_H_RES                   240
#define LCD_V_RES                   240

// Bit number used to represent command and parameter
#define LCD_CMD_BITS                 8
#define LCD_PARAM_BITS               8

#define LVGL_DRAW_BUF_LINES          40 // number of display lines in each draw buffer
#define LVGL_TICK_PERIOD_MS          5
#define LVGL_TASK_MAX_DELAY_MS       500 //500
#define LVGL_TASK_MIN_DELAY_MS       1000 / CONFIG_FREERTOS_HZ //1000
#define LVGL_TASK_STACK_SIZE         (8 * 1024) //(4 * 1024)
#define LVGL_TASK_PRIORITY            (tskIDLE_PRIORITY + 2)

SemaphoreHandle_t gui_mutex;

lv_indev_t *indev;
lv_group_t *groupMain;
lv_group_t *groupWarming;
lv_group_t *groupMenu;
lv_group_t *groupHeatingModes;
lv_group_t *groupHeatingPower;
lv_group_t *groupTempSetting;
lv_group_t *groupThermometer;
lv_group_t *groupThermometerReading;
lv_group_t *groupOTAupdate;
lv_group_t *groupLanguage;
lv_group_t *groupBottleSize;
lv_group_t *groupKeepWarm;
lv_group_t *groupTempUnitSetting;
lv_group_t *groupNotification;
lv_group_t *groupAbout;
lv_group_t *groupBluetoothApp;
lv_group_t *groupUserInstructions;
lv_group_t *groupError;
lv_group_t *groupDone;

static TaskHandle_t g_x_lcd_task_handle = NULL;
static TickType_t g_x_count_down_timer = 0;

static WARMER_config_t *g_pstru_config = NULL;

static bool g_b_btn_held = false;
static uint32_t g_u32_last_displayed_sec = 0;
static uint32_t c_time = 0; // total time in milliseconds to count from 3 to 1

// static void keypad_init(void);
static void keypad_read(lv_indev_t * indev, lv_indev_data_t * data);
static uint32_t button_get_event(void);
void keypad_enable(void);
void deinit_LCD(void);

static WARMER_btn_event_t enm_prev_btn_event = BTN_EVENT_IDLE;  // Stores last known event

// LVGL library is not thread-safe, calling LVGL APIs from different tasks, so use a mutex to protect it
// static _lock_t lvgl_api_lock;
esp_lcd_panel_handle_t panel_handle = NULL;

lv_obj_t *prev_scr;
lv_obj_t *curr_scr;

lv_group_t *active_group;
lv_obj_t   *focused_obj;

static bool lcd_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    lv_display_t *disp = (lv_display_t *)user_ctx;
    lv_display_flush_ready(disp);
    return false;
}

/* Rotate display and touch, when rotated screen in LVGL. Called when driver parameters are updated. */
static void lcd_lvgl_port_update_callback(lv_display_t *disp)
{
    esp_lcd_panel_handle_t panel_handle = lv_display_get_user_data(disp);
    // lv_display_set_rotation(disp, LV_DISP_ROTATION_180);
    lv_display_rotation_t rotation = lv_display_get_rotation(disp);
    // ESP_LOGI(TAG, "Rotation: %d", rotation  );
    switch (rotation) {
    case LV_DISPLAY_ROTATION_0:
        // Rotate LCD display
        esp_lcd_panel_swap_xy(panel_handle, false);
        esp_lcd_panel_mirror(panel_handle, true, false);
        break;
    case LV_DISPLAY_ROTATION_90:
        // Rotate LCD display
        esp_lcd_panel_swap_xy(panel_handle, true);
        esp_lcd_panel_mirror(panel_handle, true, true);
        break;
    case LV_DISPLAY_ROTATION_180:
        // Rotate LCD display
        esp_lcd_panel_swap_xy(panel_handle, false);
        esp_lcd_panel_mirror(panel_handle, false, true);
        break;
    case LV_DISPLAY_ROTATION_270:
        // Rotate LCD display
        esp_lcd_panel_swap_xy(panel_handle, true);
        esp_lcd_panel_mirror(panel_handle, false, false);
        break;
    }
}

static void lcd_lvgl_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    lcd_lvgl_port_update_callback(disp);
    esp_lcd_panel_handle_t panel_handle = lv_display_get_user_data(disp);
    int offsetx1 = area->x1;
    int offsetx2 = area->x2;
    int offsety1 = area->y1;
    int offsety2 = area->y2;
    // because SPI LCD is big-endian, we need to swap the RGB bytes order
    lv_draw_sw_rgb565_swap(px_map, (offsetx2 + 1 - offsetx1) * (offsety2 + 1 - offsety1));
    // copy a buffer's content to a specific area of the display
    esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, px_map);
}


void keypad_enable(void) {
    if (indev) {
        lv_indev_set_read_cb(indev, keypad_read);
    }
}


static uint32_t button_get_event(void)
{
     WARMER_btn_event_t enm_curr_btn_event = enm_WARMER_Get_Button_Event();
     
    // Only process if the event has changed
    if (enm_curr_btn_event != enm_prev_btn_event)
    {
        enm_prev_btn_event = enm_curr_btn_event; // Update cache
        switch (enm_curr_btn_event)
        {
            case BTN_EVENT_IDLE:
                // Idle, do nothing
                return BTN_EVENT_IDLE;
                break;

            case BTN_EVENT_BACK:
                // ESP_LOGW(TAG, "Button Event: BACK TO PREVIOUS SCREEN");
                // vTaskDelay(pdMS_TO_TICKS(20));
                lv_scr_load(curr_scr);
                vTaskDelay(pdMS_TO_TICKS(20));
                g_b_btn_held = false;
                // return BTN_EVENT_BACK;
                break;
                
             case BTN_EVENT_ON_PRESSED:
                // ESP_LOGW(TAG, "Button Event: ON_BTN_PRESSED");
                // return BTN_EVENT_ON_PRESSED;
                
                break;                

            case BTN_EVENT_ON_HOLD_START:
                // ESP_LOGW(TAG, "Button Event: ON_HOLD_START (Countdown Trigger)");
                if(curr_scr==ui_Main || curr_scr==ui_Warming || (curr_scr==ui_Error && focused_obj!=ui_PanelERR8)){
                    _ui_screen_change(&ui_Selected, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Selected_screen_init);
                    vTaskDelay(pdMS_TO_TICKS(20));
                    lv_obj_add_flag(ui_ImageSelected, LV_OBJ_FLAG_HIDDEN); 
                    lv_obj_add_flag(ui_LabelSelectedInfo, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui_LabelCountDown, LV_OBJ_FLAG_HIDDEN);
                    g_u32_last_displayed_sec = 0;
                    lv_arc_set_value(ui_countDown, 0);
                    lv_label_set_text(ui_LabelCountDown, "0");
                    lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0x5B5B5B), LV_PART_MAIN | LV_STATE_DEFAULT);        //  #5B5B5B
                    lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);        //  #FFFFFF
                    lv_obj_set_style_text_color(ui_LabelCountDown, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
                    c_time = HOLD_ON_SHORT_TIME - HOLD_START_TIME - HOLD_UI_SAFE_TIME;
                    g_b_btn_held = true;
                    TIMER_RESET(g_x_count_down_timer);
                }
                // return BTN_EVENT_ON_HOLD_START;
                break;

            case BTN_EVENT_ON_HOLD_SHORT_PRESSED:
                // ESP_LOGW(TAG, "Button Event: ON_BTN_HOLD_SHORT_PRESSED");
                 if(curr_scr==ui_Main || curr_scr==ui_Warming){
                   app_notify_soft();
                   lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0xC3F73A), LV_PART_MAIN | LV_STATE_DEFAULT);      // #C3F73A
                   lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0xC3F73A), LV_PART_INDICATOR | LV_STATE_DEFAULT);      // #C3F73A
                //    lv_obj_set_style_text_color(ui_LabelCountDown, lv_color_hex(0xC3F73A), LV_PART_MAIN | LV_STATE_DEFAULT);     // #C3F73A
                   lv_obj_add_flag(ui_LabelCountDown, LV_OBJ_FLAG_HIDDEN);

                   lv_image_set_src(ui_ImageSelected, &ui_img_selected_png);
                   lv_obj_set_style_image_recolor(ui_ImageSelected, lv_color_hex(0xC3F73A), LV_PART_MAIN | LV_STATE_DEFAULT);       // #C3F73A
                   lv_obj_clear_flag(ui_ImageSelected, LV_OBJ_FLAG_HIDDEN);
                   lv_obj_clear_flag(ui_LabelSelectedInfo, LV_OBJ_FLAG_HIDDEN); 
                   g_b_btn_held = false;
                 }else if(curr_scr==ui_Error){
                   app_notify_200ms_double();
                   lv_obj_add_flag(ui_LabelCountDown, LV_OBJ_FLAG_HIDDEN);
                   lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0xF70808), LV_PART_MAIN | LV_STATE_DEFAULT);      // #F70808
                   lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0xF70808), LV_PART_INDICATOR | LV_STATE_DEFAULT);      // #F70808
                   lv_image_set_src(ui_ImageSelected, &ui_img_poweroff_png);
                   lv_obj_set_style_image_recolor(ui_ImageSelected, lv_color_hex(0xF70808), LV_PART_MAIN | LV_STATE_DEFAULT);       // #F70808
                   lv_obj_clear_flag(ui_ImageSelected, LV_OBJ_FLAG_HIDDEN); 
                   g_b_btn_held = false;
                 }
               
                // return BTN_EVENT_ON_HOLD_SHORT_PRESSED;
            break;

            case BTN_EVENT_ON_HOLD_LONG_PRESSED: //Not needed yet
                // ESP_LOGW(TAG, "Button Event: ON_BTN_HOLD_LONG_PRESSED");
                if(curr_scr==ui_Main || curr_scr==ui_Warming){
                lv_obj_add_flag(ui_LabelCountDown, LV_OBJ_FLAG_HIDDEN);
                lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0xF70808), LV_PART_MAIN | LV_STATE_DEFAULT);         // #F70808
                lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0xF70808), LV_PART_INDICATOR | LV_STATE_DEFAULT);         // #F70808
                lv_image_set_src(ui_ImageSelected, &ui_img_poweroff_png);
                lv_obj_set_style_image_recolor(ui_ImageSelected, lv_color_hex(0xF70808), LV_PART_MAIN | LV_STATE_DEFAULT);       // #F70808
                lv_obj_clear_flag(ui_ImageSelected, LV_OBJ_FLAG_HIDDEN); 
                lv_obj_add_flag(ui_LabelSelectedInfo, LV_OBJ_FLAG_HIDDEN);  
                app_notify_200ms_double();
                }
                // return BTN_EVENT_ON_HOLD_LONG_PRESSED;
                break; 

            case BTN_EVENT_ON_HOLD_SHORT_RELEASED: // Handled in warming manager for heater start/stop
                // ESP_LOGW(TAG, "Button Event: ON_BTN_HOLD_SHORT_RELEASED");
                lv_obj_add_flag(ui_LabelSelectedInfo, LV_OBJ_FLAG_HIDDEN);
                if(curr_scr==ui_Error){
                     v_WARMER_Shut_Down();
                }
                // return BTN_EVENT_ON_HOLD_SHORT_RELEASED; // Handled in warming manager
                break;

            case BTN_EVENT_ON_HOLD_LONG_RELEASED: // Power OFF not implemented yet
                // ESP_LOGW(TAG, "Button Event: ON_BTN_HOLD_LONG_RELEASED");
                lv_obj_add_flag(ui_LabelSelectedInfo, LV_OBJ_FLAG_HIDDEN);
                if(curr_scr==ui_Main || curr_scr==ui_Warming || curr_scr==ui_Error){
                v_WARMER_Shut_Down();
                }
                break;

            case BTN_EVENT_ON_RELEASED: // Handled
                // ESP_LOGW(TAG, "Button Event: ON_BTN_RELEASED");
                return BTN_EVENT_ON_RELEASED;
                break;   
                
            case BTN_EVENT_UP_PRESSED:
                // ESP_LOGW(TAG, "Button Event: UP_BTN_PRESSED");
                if( curr_scr== ui_TempSetting){
                    v_WARMER_INC_TEMP();
                    app_notify_soft();
                } 
                // return BTN_EVENT_UP_PRESSED;
                break; 


            case BTN_EVENT_UP_HOLD_START:
                // ESP_LOGW(TAG, "Button Event: UP_HOLD_START (Countdown Trigger)");
                if(curr_scr==ui_Error && focused_obj==ui_PanelERR8){
                    //do nothing for E-8 (Already Warm Error) it will disapper after set time, 2sec
                }
                else if(curr_scr==ui_Main || curr_scr==ui_Menu || curr_scr==ui_HeatingModes || curr_scr==ui_HeatingPower || curr_scr==ui_TempUnitSetting || curr_scr==ui_BottleSize
                    || curr_scr==ui_KeepWarm || curr_scr==ui_Notification || curr_scr==ui_Language || curr_scr== ui_Error){
                    _ui_screen_change(&ui_Selected, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_Selected_screen_init);
                    vTaskDelay(pdMS_TO_TICKS(20));
                    lv_obj_add_flag(ui_ImageSelected, LV_OBJ_FLAG_HIDDEN); 
                    lv_obj_add_flag(ui_LabelSelectedInfo, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui_LabelCountDown, LV_OBJ_FLAG_HIDDEN);
                    g_u32_last_displayed_sec = 0;
                    lv_arc_set_value(ui_countDown, 0);
                    lv_label_set_text(ui_LabelCountDown, "0");
                    lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0x5B5B5B), LV_PART_MAIN | LV_STATE_DEFAULT);         //  #5B5B5B
                    lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);        //  #FFFFFF
                    lv_obj_set_style_text_color(ui_LabelCountDown, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
                    c_time = HOLD_UP_SHORT_TIME - HOLD_START_TIME - HOLD_UI_SAFE_TIME;
                    g_b_btn_held = true;
                    TIMER_RESET(g_x_count_down_timer);

                }
                // return BTN_EVENT_UP_HOLD_START;
                break;


            case BTN_EVENT_UP_HOLD_SHORT_PRESSED:
            //    ESP_LOGW(TAG, "Button Event: UP_BTN_HOLD_SHORT_PRESSED");
                if(curr_scr==ui_Main || curr_scr==ui_Menu || curr_scr==ui_HeatingModes || curr_scr==ui_HeatingPower || curr_scr==ui_TempUnitSetting || curr_scr==ui_BottleSize
                    || curr_scr==ui_KeepWarm || curr_scr==ui_Notification || curr_scr==ui_Error ){
                    app_notify_soft();
                    lv_obj_add_flag(ui_LabelCountDown, LV_OBJ_FLAG_HIDDEN);
                    lv_image_set_src(ui_ImageSelected, &ui_img_selected_png);
                   lv_obj_set_style_image_recolor(ui_ImageSelected, lv_color_hex(0xC3F73A), LV_PART_MAIN | LV_STATE_DEFAULT);       // #C3F73A
                   lv_obj_clear_flag(ui_ImageSelected, LV_OBJ_FLAG_HIDDEN);
                //    lv_obj_clear_flag(ui_LabelSelectedInfo, LV_OBJ_FLAG_HIDDEN); 

                    lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0xC3F73A), LV_PART_MAIN | LV_STATE_DEFAULT);              // #C3F73A
                    lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0xC3F73A), LV_PART_INDICATOR | LV_STATE_DEFAULT);              // #C3F73A
                    // lv_obj_set_style_text_color(ui_LabelCountDown, lv_color_hex(0xC3F73A), LV_PART_MAIN | LV_STATE_DEFAULT);
                    g_b_btn_held = false;
                }else if(curr_scr==ui_Language){
                   app_notify_soft();
                   lv_obj_add_flag(ui_LabelCountDown, LV_OBJ_FLAG_HIDDEN);
                   lv_image_set_src(ui_ImageSelected, &ui_img_selected_png);
                   lv_obj_set_style_image_recolor(ui_ImageSelected, lv_color_hex(0xC3F73A), LV_PART_MAIN | LV_STATE_DEFAULT);       // #C3F73A
                   lv_obj_clear_flag(ui_ImageSelected, LV_OBJ_FLAG_HIDDEN);
                   lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0xC3F73A), LV_PART_MAIN | LV_STATE_DEFAULT);              // #C3F73A
                   lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0xC3F73A), LV_PART_INDICATOR | LV_STATE_DEFAULT);         // #C3F73A
                   lv_label_set_text(ui_LabelSelectedInfo, _("LanguageSetReboot")); 
                   lv_obj_set_style_text_color(ui_LabelSelectedInfo, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
                   lv_obj_clear_flag(ui_LabelSelectedInfo, LV_OBJ_FLAG_HIDDEN);
                   vTaskDelay(pdMS_TO_TICKS(20));
                }
               
            //    return BTN_EVENT_UP_HOLD_SHORT_PRESSED;
               break; 
               
             case BTN_EVENT_UP_HOLD_LONG_PRESSED: // Not needed yet
                // ESP_LOGW(TAG, "Button Event: UP_BTN_HOLD_LONG_PRESSED");
                // return BTN_EVENT_UP_HOLD_LONG_PRESSED;
                break;    

             case BTN_EVENT_UP_HOLD_SHORT_RELEASED: //Handled
                // ESP_LOGW(TAG, "Button Event: UP_BTN_HOLD_SHORT_RELEASED");
                return BTN_EVENT_UP_HOLD_SHORT_RELEASED;
                break;

            case BTN_EVENT_UP_HOLD_LONG_RELEASED:   // Not needed yet
                // ESP_LOGW(TAG, "Button Event: UP_BTN_HOLD_LONG_RELEASED");
                // return BTN_EVENT_UP_HOLD_LONG_RELEASED;
                break;

            case BTN_EVENT_UP_RELEASED: //Handled
                // ESP_LOGW(TAG, "Button Event: UP_BTN_RELEASED");
                return BTN_EVENT_UP_RELEASED;
                break;

            case BTN_EVENT_TH_PRESSED:  // Not needed yet
                // ESP_LOGW(TAG, "Button Event: TH_BTN_PRESSED");
                // return BTN_EVENT_TH_PRESSED;
                break; 

            case BTN_EVENT_TH_HOLD_START:
                // ESP_LOGW(TAG, "Button Event: TH_HOLD_START (Countdown Trigger)");
                if(curr_scr==ui_Main && enm_WARMER_Get_State() == WARMER_STATE_IDLE){
                    _ui_screen_change(&ui_Selected, LV_SCR_LOAD_ANIM_FADE_IN, 2, 0, &ui_Selected_screen_init);
                    vTaskDelay(pdMS_TO_TICKS(20));
                    lv_obj_add_flag(ui_ImageSelected, LV_OBJ_FLAG_HIDDEN); 
                    lv_obj_add_flag(ui_LabelSelectedInfo, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_clear_flag(ui_LabelCountDown, LV_OBJ_FLAG_HIDDEN);
                    g_u32_last_displayed_sec = 0;
                    lv_arc_set_value(ui_countDown, 0);
                    lv_label_set_text(ui_LabelCountDown, "0");
                    lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0x5B5B5B), LV_PART_MAIN | LV_STATE_DEFAULT);              // #5B5B5B
                    lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);        //  #FFFFFF
                    lv_obj_set_style_text_color(ui_LabelCountDown, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
                    c_time = HOLD_TH_SHORT_TIME - HOLD_START_TIME - HOLD_UI_SAFE_TIME;
                    g_b_btn_held = true;
                    TIMER_RESET(g_x_count_down_timer);
                }
                // return BTN_EVENT_TH_HOLD_START;
                break;    

             case BTN_EVENT_TH_HOLD_SHORT_PRESSED:
                // ESP_LOGW(TAG, "Button Event: TH_BTN_HOLD_SHORT_PRESSED");
                if(curr_scr==ui_Main && enm_WARMER_Get_State() == WARMER_STATE_IDLE){
                    app_notify_soft();
                    lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0xC3F73A), LV_PART_MAIN | LV_STATE_DEFAULT);      // #C3F73A
                    lv_obj_set_style_arc_color(ui_countDown, lv_color_hex(0xC3F73A), LV_PART_INDICATOR | LV_STATE_DEFAULT);      // #C3F73A

                    lv_obj_add_flag(ui_LabelCountDown, LV_OBJ_FLAG_HIDDEN);
                    lv_image_set_src(ui_ImageSelected, &ui_img_selected_png);
                   lv_obj_set_style_image_recolor(ui_ImageSelected, lv_color_hex(0xC3F73A), LV_PART_MAIN | LV_STATE_DEFAULT);       // #C3F73A
                   lv_obj_clear_flag(ui_ImageSelected, LV_OBJ_FLAG_HIDDEN);
                   g_b_btn_held = false;
                }
                
                // return BTN_EVENT_TH_HOLD_SHORT_PRESSED;
                break;   

            case BTN_EVENT_TH_HOLD_LONG_PRESSED:
                // ESP_LOGW(TAG, "Button Event: TH_BTN_HOLD_LONG_PRESSED");
                // return BTN_EVENT_TH_HOLD_LONG_PRESSED;
                break;
               
            case BTN_EVENT_TH_HOLD_SHORT_RELEASED: 
                // ESP_LOGW(TAG, "Button Event: TH_BTN_HOLD_SHORT_RELEASED");
                if(curr_scr==ui_Main && enm_WARMER_Get_State() == WARMER_STATE_IDLE){
                    // act_key = LV_KEY_LEFT;
                    ToThermometerScreen(NULL);
                    vTaskDelay(pdMS_TO_TICKS(20));
                    // app_notify_soft();
                }
                // return BTN_EVENT_TH_HOLD_SHORT_RELEASED;
                break;
            
            case BTN_EVENT_TH_HOLD_LONG_RELEASED:
                // ESP_LOGW(TAG, "Button Event: TH_BTN_HOLD_LONG_RELEASED");
                // return BTN_EVENT_TH_HOLD_LONG_RELEASED;
                break;
                   
            case BTN_EVENT_TH_RELEASED: // Not needed yet
                // ESP_LOGW(TAG, "Button Event: TH_BTN_RELEASED");
                // return BTN_EVENT_TH_RELEASED;
                break;

           default:
                // ESP_LOGW(TAG, "Button Event: UNKNOWN (%d)", enm_curr_btn_event);
                return BTN_EVENT_IDLE;
                break;
        }
    }
    return BTN_EVENT_IDLE;
}


static void keypad_read(lv_indev_t * indev_drv, lv_indev_data_t * data)
{
    static uint32_t last_key = 0;

    /*Get whether the a key is pressed and save the pressed key*/
    uint32_t act_key = button_get_event();
    active_group = lv_indev_get_group(indev_drv);
    focused_obj = lv_group_get_focused(active_group);


if (g_b_btn_held)
{
    TickType_t elapsed_ticks = TIMER_ELAPSED(g_x_count_down_timer);
    uint32_t elapsed_ms = pdTICKS_TO_MS(elapsed_ticks);

    float progress_ratio = (float)elapsed_ms / c_time;
    if (progress_ratio > 1.0f)
        progress_ratio = 1.0f;

    float count_f = 3.0f - (progress_ratio * 2.0f);
    uint32_t count_value = (uint32_t)(count_f + 0.5f); // rounded

    uint32_t progress = (uint32_t)((1.0f - progress_ratio) * 100.0f);
    if (progress > 100)
        progress = 100;

    lv_arc_set_value(ui_countDown, progress);

    if (count_value != g_u32_last_displayed_sec)
    {
        g_u32_last_displayed_sec = count_value;
        lv_label_set_text_fmt(ui_LabelCountDown, "%d", count_value);
    }
}

    lv_obj_t *new_scr = lv_scr_act(); // get active screen
    if(new_scr != ui_Selected){
    if(new_scr != curr_scr){
        prev_scr = curr_scr;
        curr_scr = new_scr;
    }
    }
    if(act_key != 0) {
        data->state = LV_INDEV_STATE_PRESSED;

        switch(act_key) {

        case BTN_EVENT_UP_RELEASED: {   //Press/Release 3_BAR (UP) 

                if( curr_scr== ui_Main){
                    ToTempSettingScreen(NULL);
                    vTaskDelay(pdMS_TO_TICKS(20));
                    app_notify_soft();
                }else if(curr_scr== ui_Done || curr_scr==ui_TempSetting || curr_scr== ui_Warming || curr_scr== ui_Done || curr_scr==ui_Thermometer ||curr_scr== ui_ThermometerReading ||
                    curr_scr== ui_About || curr_scr== ui_BluetoothApp || curr_scr== ui_UserInstruction || curr_scr==ui_OTA || curr_scr==ui_Error){
                    // do nothing
                }
                else {
                    act_key = LV_KEY_NEXT; //default mapping scroll on other screen 
                    vTaskDelay(pdMS_TO_TICKS(20));
                    app_notify_soft();
                }
                break;
            }

        case BTN_EVENT_UP_HOLD_SHORT_RELEASED: { //Short Hold Press 3_BAR (UP)
                
                if(curr_scr==ui_Main){
                    ToMenuScreen(NULL);
                    vTaskDelay(pdMS_TO_TICKS(20));
                }
                else if(curr_scr==ui_Menu || curr_scr==ui_HeatingModes || curr_scr==ui_HeatingPower || curr_scr==ui_TempUnitSetting || curr_scr==ui_BottleSize
                    || curr_scr==ui_KeepWarm || curr_scr==ui_Notification || curr_scr==ui_Language) {

                        if(focused_obj==ui_PanelKeepWarm){
                            if (ui_KeepWarm == NULL) {
                                ui_KeepWarm_screen_init();
                            }
                            if(device_data.plug.plugdet){
                            lv_label_set_text_fmt(ui_LabelKeepWarm1, "%d",KEEP_WARM_PLUGGED_LEVEL_1);
                            lv_label_set_text_fmt(ui_LabelKeepWarm2, "%d",KEEP_WARM_PLUGGED_LEVEL_2);
                            lv_label_set_text_fmt(ui_LabelKeepWarm3, "%d",KEEP_WARM_PLUGGED_LEVEL_3);
                            lv_label_set_text_fmt(ui_LabelKeepWarm4, "%d",KEEP_WARM_PLUGGED_LEVEL_4);
                            lv_label_set_text_fmt(ui_LabelKeepWarm5, "%d",KEEP_WARM_PLUGGED_LEVEL_5);
                            }else{
                            lv_label_set_text_fmt(ui_LabelKeepWarm1, "%d",KEEP_WARM_LEVEL_1);
                            lv_label_set_text_fmt(ui_LabelKeepWarm2, "%d",KEEP_WARM_LEVEL_2);
                            lv_label_set_text_fmt(ui_LabelKeepWarm3, "%d",KEEP_WARM_LEVEL_3);
                            lv_label_set_text_fmt(ui_LabelKeepWarm4, "%d",KEEP_WARM_LEVEL_4);
                            lv_label_set_text_fmt(ui_LabelKeepWarm5, "%d",KEEP_WARM_LEVEL_5);    
                            }
                        }

                    act_key = LV_KEY_ENTER; //default mapping enter menu on menu screen 
                    vTaskDelay(pdMS_TO_TICKS(20));
                }                
                else if(curr_scr== ui_Error && focused_obj!=ui_PanelERR8){
                    // no action, no beep
                    ErrToMenu(NULL);
                    vTaskDelay(pdMS_TO_TICKS(20));
                }
                break;
            }    

        case BTN_EVENT_ON_RELEASED: {   //Press/Release Press OK (ON)
                
                if( curr_scr== ui_TempSetting || curr_scr==ui_Thermometer ||curr_scr== ui_ThermometerReading ||
                    curr_scr== ui_About || curr_scr== ui_BluetoothApp || curr_scr== ui_UserInstruction){ 
                
                    app_notify_soft();
                    act_key = LV_KEY_ENTER; 
                    vTaskDelay(pdMS_TO_TICKS(20));
                }
                else if(curr_scr== ui_Menu){
                    app_notify_soft();
                    ToMainScreen(NULL);
                    vTaskDelay(pdMS_TO_TICKS(20));
                }
                else if(curr_scr== ui_Done){

                    if(curr_scr==ui_Done && focused_obj==ui_PanelKeepingProgress){
                        lv_label_set_text(ui_LabelstartInfo, _("KeepingProgress")); //set main info label
                        app_notify_soft();
                     }

                    app_notify_soft();
                    act_key = LV_KEY_ENTER; 
                    vTaskDelay(pdMS_TO_TICKS(20));

                } 
                 
                break;
            }
          
        default:
                ESP_LOGW(TAG, "UNKNOWN");
                return BTN_EVENT_IDLE;
                break;
        }
        
        last_key = act_key;
    }
    else {
        data->state = LV_INDEV_STATE_RELEASED;
    }

    data->key = last_key;
    // PRINT_STACK_USAGE(3000); 
}

void my_focus_cb(lv_group_t * group)
{
    lv_obj_t *focused_obj = lv_group_get_focused(group);
    lv_obj_scroll_to_view(focused_obj, LV_ANIM_ON);
}


static void increase_lvgl_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

static void lvgl_port_task(void *arg)
{
    // uint32_t time_till_next_ms = 0;
    while (1) {
        // _lock_acquire(&lvgl_api_lock);
        xSemaphoreTake(gui_mutex, portMAX_DELAY);
        // time_till_next_ms = lv_timer_handler();
        lv_timer_handler();
        // _lock_release(&lvgl_api_lock);
        xSemaphoreGive(gui_mutex);

        // in case of triggering a task watch dog time out
        // time_till_next_ms = MAX(time_till_next_ms, LVGL_TASK_MIN_DELAY_MS);
        // in case of lvgl display not ready yet
        // time_till_next_ms = MIN(time_till_next_ms, LVGL_TASK_MAX_DELAY_MS);
        // usleep(1000 * time_till_next_ms);
        esp_task_wdt_reset();
        vTaskDelay(pdMS_TO_TICKS(20));
        //  PRINT_STACK_USAGE (5000); 
    }
}

void deinit_LCD(void){
     ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
}

void lcd_init(void)
{
    gui_mutex = xSemaphoreCreateMutex();

    LOGI("Initialize SPI bus");
    spi_bus_config_t buscfg = {
        .sclk_io_num = LCD_PIN_NUM_SCLK,
        .mosi_io_num = LCD_PIN_NUM_MOSI,
        .miso_io_num = LCD_PIN_NUM_MISO,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = LCD_H_RES * 80 * sizeof(uint16_t),
    };
    ESP_ERROR_CHECK(spi_bus_initialize(LCD_HOST, &buscfg, SPI_DMA_CH_AUTO));

    // ESP_LOGI(TAG, "Install panel IO");
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = LCD_PIN_NUM_LCD_DC,
        .cs_gpio_num = LCD_PIN_NUM_LCD_CS,
        .pclk_hz = LCD_PIXEL_CLOCK_HZ,
        .lcd_cmd_bits = LCD_CMD_BITS,
        .lcd_param_bits = LCD_PARAM_BITS,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };
    // Attach the LCD to the SPI bus
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &io_config, &io_handle));

    esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = LCD_PIN_NUM_LCD_RST,
        .color_space = ESP_LCD_COLOR_SPACE_BGR,
        .bits_per_pixel = 16,
    };
#if CONFIG_LCD_CONTROLLER_ILI9341
    ESP_LOGI(TAG, "Install ILI9341 panel driver");
    ESP_ERROR_CHECK(esp_lcd_new_panel_ili9341(io_handle, &panel_config, &panel_handle));
#elif CONFIG_LCD_CONTROLLER_GC9A01
    ESP_LOGI(TAG, "Install GC9A01 panel driver");
    ESP_ERROR_CHECK(esp_lcd_new_panel_gc9a01(io_handle, &panel_config, &panel_handle));
#endif

    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
#if CONFIG_LCD_CONTROLLER_GC9A01
    ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
#endif
    ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle, true, false));

    g_pstru_config = pstru_WARMER_Get_Config ();
    lv_init();
    lv_i18n_init(lv_i18n_language_pack);
    // lv_i18n_set_locale("zh-SG");
    (g_pstru_config->enm_device_language== DEVICE_LANGUAGE_EN_US) ? lv_i18n_set_locale("en-US") :
    (g_pstru_config->enm_device_language== DEVICE_LANGUAGE_ZH_SG) ? lv_i18n_set_locale("zh-SG") :
    (g_pstru_config->enm_device_language== DEVICE_LANGUAGE_JA_JP) ? lv_i18n_set_locale("ja-JP") :
    (g_pstru_config->enm_device_language== DEVICE_LANGUAGE_KO_KR) ? lv_i18n_set_locale("ko-KR") : lv_i18n_set_locale("en-US");

    // create a lvgl display
    lv_display_t *display = lv_display_create(LCD_H_RES, LCD_V_RES);

    // alloc draw buffers used by LVGL, it's recommended to choose the size of the draw buffer(s) to be at least 1/10 screen sized
    size_t draw_buffer_sz = LCD_H_RES * LVGL_DRAW_BUF_LINES * sizeof(lv_color16_t);
    // void *buf1 = spi_bus_dma_memory_alloc(LCD_HOST, draw_buffer_sz, 0);
    // void *buf2 = spi_bus_dma_memory_alloc(LCD_HOST, draw_buffer_sz, 0);
    void *buf1 = heap_caps_malloc(draw_buffer_sz, MALLOC_CAP_DMA);
    assert(buf1);
    void *buf2 = heap_caps_malloc(draw_buffer_sz, MALLOC_CAP_DMA);
    assert(buf2);
    
    lv_display_set_buffers(display, buf1, buf2, draw_buffer_sz, LV_DISPLAY_RENDER_MODE_PARTIAL);        // initialize LVGL draw buffers
    // associate the mipi panel handle to the display
    lv_display_set_user_data(display, panel_handle);
    lv_display_set_color_format(display, LV_COLOR_FORMAT_RGB565);
    // set the callback which can copy the rendered image to an area of the display
    lv_display_set_flush_cb(display, lcd_lvgl_flush_cb);
    lv_display_set_rotation(display, LV_DISPLAY_ROTATION_180);
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &increase_lvgl_tick,
        .name = "lvgl_tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_PERIOD_MS * 1000));

    // ESP_LOGI(TAG, "Register io panel event callback for LVGL flush ready notification");
    const esp_lcd_panel_io_callbacks_t cbs = {
        .on_color_trans_done = lcd_notify_lvgl_flush_ready,
    };
    /* Register done callback */
    ESP_ERROR_CHECK(esp_lcd_panel_io_register_event_callbacks(io_handle, &cbs, display));

#if CONFIG_LCD_TOUCH_ENABLED
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    esp_lcd_panel_io_spi_config_t tp_io_config =
#ifdef CONFIG_LCD_TOUCH_CONTROLLER_STMPE610
        ESP_LCD_TOUCH_IO_SPI_STMPE610_CONFIG(LCD_PIN_NUM_TOUCH_CS);
#elif CONFIG_LCD_TOUCH_CONTROLLER_XPT2046
        ESP_LCD_TOUCH_IO_SPI_XPT2046_CONFIG(LCD_PIN_NUM_TOUCH_CS);
#endif
    // Attach the TOUCH to the SPI bus
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)LCD_HOST, &tp_io_config, &tp_io_handle));

    esp_lcd_touch_config_t tp_cfg = {
        .x_max = X_RES,
        .y_max = CD_V_RES,
        .rst_gpio_num = -1,
        .int_gpio_num = -1,
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = CONFIG_LCD_MIRROR_Y,
        },
    };
    esp_lcd_touch_handle_t tp = NULL;

#if CONFIG_LCD_TOUCH_CONTROLLER_STMPE610
    ESP_LOGI(TAG, "Initialize touch controller STMPE610");
    ESP_ERROR_CHECK(esp_lcd_touch_new_spi_stmpe610(tp_io_handle, &tp_cfg, &tp));
#elif CONFIG_LCD_TOUCH_CONTROLLER_XPT2046
    ESP_LOGI(TAG, "Initialize touch controller XPT2046");
    ESP_ERROR_CHECK(esp_lcd_touch_new_spi_xpt2046(tp_io_handle, &tp_cfg, &tp));
#endif

    static lv_indev_t *indev;
    indev = lv_indev_create(); // Input device driver (Touch)
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_display(indev, display);
    lv_indev_set_user_data(indev, tp);
    lv_indev_set_read_cb(indev, _lvgl_touch_cb);
#endif

     xTaskCreate(lvgl_port_task, "LVGL", LVGL_TASK_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, g_x_lcd_task_handle);
    // xTaskCreatePinnedToCore(lvgl_port_task, "LVGL", LVGL_TASK_STACK_SIZE, NULL, LVGL_TASK_PRIORITY, g_x_lcd_task_handle,1);

    indev = lv_indev_create(); // Input device
    lv_indev_set_type(indev, LV_INDEV_TYPE_KEYPAD);

    // Create a group for the input device
    groupMain = lv_group_create();
    groupWarming = lv_group_create();
    groupMenu = lv_group_create();
    groupHeatingModes = lv_group_create();
    groupHeatingPower = lv_group_create();
    groupLanguage = lv_group_create();
    groupBottleSize = lv_group_create();
    groupKeepWarm = lv_group_create();
    groupTempUnitSetting = lv_group_create();
    groupThermometer = lv_group_create();
    groupThermometerReading = lv_group_create();
    groupOTAupdate = lv_group_create();
    groupNotification = lv_group_create();
    groupTempSetting = lv_group_create();
    groupAbout = lv_group_create();
    groupBluetoothApp = lv_group_create();
    groupUserInstructions = lv_group_create();
    groupError = lv_group_create();
    groupDone = lv_group_create();
    
    // Lock the mutex due to the LVGL APIs are not thread-safe
    // _lock_acquire(&lvgl_api_lock);
    xSemaphoreTake(gui_mutex, portMAX_DELAY);
    ui_init();
    xSemaphoreGive(gui_mutex);
    // _lock_release(&lvgl_api_lock);
    vTaskDelay(pdMS_TO_TICKS(100));

    
    lv_group_add_obj(groupMain, ui_PanelMain);   

    lv_group_add_obj(groupWarming, ui_PanelWarming);

    lv_group_add_obj(groupMenu, ui_PanelModes);
    lv_group_add_obj(groupMenu, ui_PanelHeatingPower);   
    lv_group_add_obj(groupMenu, ui_PanelTempUnit);
    lv_group_add_obj(groupMenu, ui_PanelBottleSize);
    lv_group_add_obj(groupMenu, ui_PanelKeepWarm);
    lv_group_add_obj(groupMenu, ui_PanelNotification);
    lv_group_add_obj(groupMenu, ui_PanelLanguage);
    lv_group_add_obj(groupMenu, ui_PanelBluetoothApp);
    lv_group_add_obj(groupMenu, ui_PanelInstructions);
    lv_group_add_obj(groupMenu, ui_PanelAbout);
           
    lv_group_add_obj(groupOTAupdate, ui_PanelOTAProgress);
    lv_group_add_obj(groupOTAupdate, ui_PanelOTAUpdated);
    lv_group_add_obj(groupOTAupdate, ui_PanelOTAFailed);

    lv_group_add_obj(groupThermometerReading, ui_PanelTempMeasured);

    lv_group_add_obj(groupTempSetting, ui_PanelSettingTemp);

    lv_group_add_obj(groupError, ui_PanelERR1);
    lv_group_add_obj(groupError, ui_PanelERR2);
    lv_group_add_obj(groupError, ui_PanelERR3);
    lv_group_add_obj(groupError, ui_PanelERR4);
    lv_group_add_obj(groupError, ui_PanelERR5);
    lv_group_add_obj(groupError, ui_PanelERR7);
    lv_group_add_obj(groupError, ui_PanelERR8);
    lv_group_add_obj(groupError, ui_PanelERR25);

    lv_group_add_obj(groupDone, ui_PanelDone);
    lv_group_add_obj(groupDone, ui_PanelKeepingProgress);
    lv_group_add_obj(groupDone, ui_PanelChargeProgress);
    lv_group_add_obj(groupDone, ui_PanelChargingDone);
  
    Sleep_Cut_Disable();
    
    curr_scr = lv_scr_act(); // get active screen
    prev_scr = curr_scr;
    // app_notify_200ms();
    // user can flush pre-defined pattern to the screen before we turn on the screen or backlight
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));
   
}