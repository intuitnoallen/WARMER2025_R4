#include "../ui.h"

void ui_BluetoothApp_screen_init(void)
{
    ui_BluetoothApp = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_BluetoothApp, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_BluetoothApp, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BluetoothApp, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelBluetoothAppScreen = lv_obj_create(ui_BluetoothApp);
    lv_obj_set_width(ui_PanelBluetoothAppScreen, 240);
    lv_obj_set_height(ui_PanelBluetoothAppScreen, 240);
    lv_obj_set_align(ui_PanelBluetoothAppScreen, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelBluetoothAppScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_PanelBluetoothAppScreen, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelBluetoothAppScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelBluetoothAppScreen, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImageAppQRcode = lv_image_create(ui_PanelBluetoothAppScreen);
    lv_image_set_src(ui_ImageAppQRcode, &ui_img_qrcodeapp_png);
    lv_obj_set_width(ui_ImageAppQRcode, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ImageAppQRcode, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ImageAppQRcode, 0);
    lv_obj_set_y(ui_ImageAppQRcode, -22);
    lv_obj_set_align(ui_ImageAppQRcode, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageAppQRcode, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_ImageAppQRcode, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelAppQRcode = lv_label_create(ui_PanelBluetoothAppScreen);
    lv_obj_set_width(ui_LabelAppQRcode, 150);   /// 1
    lv_obj_set_height(ui_LabelAppQRcode, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelAppQRcode, 1);
    lv_obj_set_y(ui_LabelAppQRcode, 74);
    lv_obj_set_align(ui_LabelAppQRcode, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelAppQRcode, _("DownloadAppScan"));
    lv_obj_set_style_text_color(ui_LabelAppQRcode, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelAppQRcode, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelAppQRcode, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelAppQRcode, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_PanelBluetoothAppScreen, ui_event_PanelBluetoothAppScreen, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BluetoothApp, ui_event_BluetoothApp, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_BluetoothApp, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED, &ui_BluetoothApp);

}
