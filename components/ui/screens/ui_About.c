#include "../ui.h"


void ui_About_screen_init(void)
{
    ui_About = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_About, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_About, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_About, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelAboutInfo = lv_obj_create(ui_About);
    lv_obj_set_width(ui_PanelAboutInfo, 240);
    lv_obj_set_height(ui_PanelAboutInfo, 240);
    lv_obj_set_align(ui_PanelAboutInfo, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelAboutInfo, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_PanelAboutInfo, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelAboutInfo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelAboutInfo, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImageCompanyLogo = lv_image_create(ui_PanelAboutInfo);
    lv_image_set_src(ui_ImageCompanyLogo, &ui_img_fc_wigg_180_png);
    lv_obj_set_width(ui_ImageCompanyLogo, LV_SIZE_CONTENT);   /// 81
    lv_obj_set_height(ui_ImageCompanyLogo, LV_SIZE_CONTENT);    /// 50
    lv_obj_set_x(ui_ImageCompanyLogo, 2);
    lv_obj_set_y(ui_ImageCompanyLogo, -31);
    lv_obj_set_align(ui_ImageCompanyLogo, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageCompanyLogo, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_ImageCompanyLogo, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_image_set_scale(ui_ImageCompanyLogo, 255);

    ui_LabelFWversion = lv_label_create(ui_PanelAboutInfo);
    lv_obj_set_height(ui_LabelFWversion, 10);
    lv_obj_set_width(ui_LabelFWversion, LV_SIZE_CONTENT);   /// 120
    lv_obj_set_x(ui_LabelFWversion, 70);
    lv_obj_set_y(ui_LabelFWversion, 32);
    lv_obj_set_align(ui_LabelFWversion, LV_ALIGN_LEFT_MID);

    lv_label_set_text(ui_LabelFWversion, "FW:   v4.1.1");
    lv_obj_set_style_text_color(ui_LabelFWversion, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelFWversion, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelFWversion, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelFWversion, &ui_font_Segoe_UI_SB14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelHWversion = lv_label_create(ui_PanelAboutInfo);
    lv_obj_set_width(ui_LabelHWversion, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelHWversion, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelHWversion, 70);
    lv_obj_set_y(ui_LabelHWversion, 51);
    lv_obj_set_align(ui_LabelHWversion, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_LabelHWversion, "HW:  R4");
    lv_obj_set_style_text_color(ui_LabelHWversion, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelHWversion, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelHWversion, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelHWversion, &ui_font_Segoe_UI_SB14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_LabelManufacture = lv_label_create(ui_PanelAboutInfo);
    lv_obj_set_width(ui_LabelManufacture, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelManufacture, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelManufacture, 0);
    lv_obj_set_y(ui_LabelManufacture, 86);
    lv_obj_set_align(ui_LabelManufacture, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelManufacture, _("Manufacture"));
    lv_obj_set_style_text_color(ui_LabelManufacture, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelManufacture, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelManufacture, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelManufacture, &ui_font_Segoe_UI_SB14, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_PanelAboutInfo, ui_event_PanelAboutInfo, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_About, ui_event_About, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_About, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED, &ui_About);

}
