#include "../ui.h"

void ui_UserInstruction_screen_init(void)
{
    ui_UserInstruction = lv_obj_create(NULL);
    lv_obj_remove_flag(ui_UserInstruction, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_UserInstruction, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_UserInstruction, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_PanelUserInstruction = lv_obj_create(ui_UserInstruction);
    lv_obj_set_width(ui_PanelUserInstruction, 240);
    lv_obj_set_height(ui_PanelUserInstruction, 240);
    lv_obj_set_align(ui_PanelUserInstruction, LV_ALIGN_CENTER);
    lv_obj_remove_flag(ui_PanelUserInstruction, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_PanelUserInstruction, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_PanelUserInstruction, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_PanelUserInstruction, LV_BORDER_SIDE_NONE, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_ImageUserInstruction = lv_image_create(ui_PanelUserInstruction);
    lv_image_set_src(ui_ImageUserInstruction, &ui_img_qrcodeavoidleaking_png);
    lv_obj_set_width(ui_ImageUserInstruction, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_ImageUserInstruction, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_ImageUserInstruction, 0);
    lv_obj_set_y(ui_ImageUserInstruction, -22);
    lv_obj_set_align(ui_ImageUserInstruction, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_ImageUserInstruction, LV_OBJ_FLAG_CLICKABLE);     /// Flags
    lv_obj_remove_flag(ui_ImageUserInstruction, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_LabelUserInstruction = lv_label_create(ui_PanelUserInstruction);
    lv_obj_set_width(ui_LabelUserInstruction, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_LabelUserInstruction, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_LabelUserInstruction, 1);
    lv_obj_set_y(ui_LabelUserInstruction, 74);
    lv_obj_set_align(ui_LabelUserInstruction, LV_ALIGN_CENTER);
    lv_label_set_text(ui_LabelUserInstruction, _("AvoidLeaking"));
    lv_obj_set_style_text_color(ui_LabelUserInstruction, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelUserInstruction, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_LabelUserInstruction, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelUserInstruction, &ui_font_WIGG_B20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_PanelUserInstruction, ui_event_PanelUserInstruction, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_UserInstruction, ui_event_UserInstruction, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_UserInstruction, scr_unloaded_delete_cb, LV_EVENT_SCREEN_UNLOADED, &ui_UserInstruction);

}
