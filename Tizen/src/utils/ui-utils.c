/*******************************************************************************
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 *
 *  Licensed under the Apache License, Version 2.0 (the License);
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *******************************************************************************/

/**
 * @file	ui-utils.c
 * @brief	This file provide necessary Util functions UI.
 *
 * @bug
 * @credit
 */


#include "utils/config.h"
#include "utils/logger.h"
#include "utils/ui-utils.h"

#include "system_info.h"

#define PART_POPUP_BUTTON1      "button1"
#define PART_POPUP_BUTTON2      "button2"
#define PART_POPUP_BUTTON3      "button3"
#define STYLE_POPUP_BUTTON      "popup_button/default"

/**
* @function		DoesFeatureExixts
* @description		Checks if a particular feature exists or not
* @parameter[IN]	Feature Key
* @return		bool The result of feature search
*/


/**
 * @function		does_feature_exist
 * @since_tizen		2.3
 * @description		Does Feature Exist
 * @parameter		char*: Char Pointer
 * @return		bool
 */
bool does_feature_exist(char* feature)
{
	bool is_feature_available = false;
	DBG("Check feature %s availability \n",feature);
	system_info_get_platform_bool(feature, &is_feature_available);
	DBG("Check feature %s availability value is  %d \n",feature , is_feature_available );
	return is_feature_available;
}


/**
 * @function		ui_utils_toolbar_add
 * @since_tizen		2.3
 * @description		Ui Utils Toolbar Add
 * @parameter		Evas_Object*: Evas Object Pointer
 * @return		Evas_Object*
 */
Evas_Object *ui_utils_toolbar_add(Evas_Object *parent)
{
    Evas_Object *toolbar = elm_toolbar_add(parent);
    RETVM_IF(NULL == toolbar, NULL, "toolbar is null");

    elm_object_style_set(toolbar, "default");
    elm_toolbar_shrink_mode_set(toolbar, ELM_TOOLBAR_SHRINK_EXPAND);
    elm_toolbar_transverse_expanded_set(toolbar, EINA_TRUE);
    elm_toolbar_select_mode_set(toolbar, ELM_OBJECT_SELECT_MODE_NONE);
    evas_object_size_hint_weight_set(toolbar, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

    return toolbar;
}


/**
 * @function		ui_utils_icon_create_new
 * @since_tizen		2.3
 * @description		Ui Utils Icon Create New
 * @parameter		Evas_Object*: Evas Object Pointer, const char*: Const char Pointer
 * @return		Evas_Object*
 */
Evas_Object *ui_utils_icon_create_new(Evas_Object *parent, const char *icon_name)
{
	char buf[255];
	Evas_Object *img = elm_image_add(parent);

	snprintf(buf,sizeof(buf), "%s/%s", app_get_resource_path(), icon_name);
	elm_image_file_set(img, buf, NULL);
	evas_object_color_set(img, 110, 162, 185, 255);
	evas_object_size_hint_align_set(img, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(img, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	return img;
}


/**
 * @function		ui_utils_icon_create
 * @since_tizen		2.3
 * @description		Ui Utils Icon Create
 * @parameter		Evas_Object*: Evas Object Pointer, const char*: Const char Pointer
 * @return		Evas_Object*
 */
Evas_Object *ui_utils_icon_create(Evas_Object *parent, const char *icon_name)
{
	char buf[255];
	Evas_Object *img = elm_image_add(parent);
	snprintf(buf, sizeof(buf), "%s/icon/%s.png", app_get_resource_path(), icon_name);
	elm_image_file_set(img, buf, NULL);
	evas_object_color_set(img, 110, 162, 185, 255);
	evas_object_size_hint_align_set(img, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_weight_set(img, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	return img;
}


/**
 * @function		ui_utils_create_image
 * @since_tizen		2.3
 * @description		Ui Utils Create Image
 * @parameter		Evas_Object*: Evas Object Pointer, const char*: Const char Pointer
 * @return		Evas_Object*
 */
Evas_Object *ui_utils_create_image(Evas_Object *parent, const char *file_path)
{
    RETVM_IF(NULL == parent, NULL, "create_image parent is NULL");

    Evas_Object *image = NULL;
    image = elm_image_add(parent);
    RETVM_IF(NULL == image, NULL, "elm_image_add is NULL");

    bool result = (EINA_TRUE == elm_image_file_set(image, file_path, NULL));
    if(!result)
    {
		ERR("error with elm_image_file_set, image name %s", file_path);
		evas_object_del(image);
		return NULL;
    }

	elm_image_no_scale_set(image, EINA_FALSE);
	elm_image_resizable_set(image, EINA_TRUE, EINA_TRUE);
	elm_image_smooth_set(image, EINA_FALSE);
	elm_image_aspect_fixed_set(image, EINA_TRUE);
	elm_image_editable_set(image, EINA_FALSE);
	evas_object_show(image);

	return image;
}


/**
 * @function		ui_utils_navi_add
 * @since_tizen		2.3
 * @description		Ui Utils Navi Add
 * @parameter		Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		Evas_Object*
 */
Evas_Object *ui_utils_navi_add(Evas_Object *parent, Eext_Event_Cb back_cb, void *cb_data)
{
    RETVM_IF(NULL == parent, NULL, "parent is null");
    Evas_Object *navi = elm_naviframe_add(parent);
    eext_object_event_callback_add(navi, EEXT_CALLBACK_BACK, back_cb, cb_data);
    eext_object_event_callback_add(navi, EEXT_CALLBACK_MORE, eext_naviframe_more_cb, NULL);
    elm_naviframe_prev_btn_auto_pushed_set(navi, EINA_FALSE);
    return navi;
}


/**
 * @function		ui_utils_layout_add
 * @since_tizen		2.3
 * @description		Ui Utils Layout Add
 * @parameter		Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		Evas_Object*
 */
Evas_Object *ui_utils_layout_add(Evas_Object *parent, Evas_Object_Event_Cb destroy_cb, void *cb_data)
{
    Evas_Object *layout = elm_layout_add(parent);
    RETVM_IF(NULL == layout, NULL, "layout is null");
    evas_object_event_callback_add(layout, EVAS_CALLBACK_FREE, destroy_cb, cb_data);
    return layout;
}


/**
 * @function		_eext_popup_back_cb
 * @since_tizen		2.3
 * @description		 Eext Popup Back Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static inline void
 */
static inline void _eext_popup_back_cb(void *data, Evas_Object *obj, void *event_info)
{
   DBG("eext_object_event_callback_add success. Popup back clicked");
   DBG("eext_object_event_callback_del calling...");
   eext_object_event_callback_del(obj, EEXT_CALLBACK_BACK, _eext_popup_back_cb);
   evas_object_del(obj);
}


/**
 * @function		ui_utils_popup_add
 * @since_tizen		2.3
 * @description		Ui Utils Popup Add
 * @parameter		Evas_Object*: Evas Object Pointer, const char*: Const char Pointer
 * @return		Evas_Object*
 */
Evas_Object *ui_utils_popup_add(Evas_Object *parent, const char *text)
{
    RETVM_IF(NULL == parent, NULL, "popup is null");

    Evas_Object *popup = elm_popup_add(parent);
    RETVM_IF(NULL == popup, NULL, "popup is not created");

	elm_object_part_text_set(popup, PART_POPUP_TITLE, text);
	evas_object_show(popup);
	eext_object_event_callback_add(popup, EEXT_CALLBACK_BACK, _eext_popup_back_cb, NULL);

	return popup;
}


/**
 * @function		ui_utils_popup_button_add
 * @since_tizen		2.3
 * @description		Ui Utils Popup Button Add
 * @parameter		Evas_Object*: Evas Object Pointer, const char*: Const char Pointer, void*: Void Pointer
 * @return		Evas_Object*
 */
Evas_Object * ui_utils_popup_button_add(Evas_Object *popup, const char *text, Evas_Smart_Cb func, void *data)
{
    RETVM_IF(NULL == popup, NULL, "popup is null");
    static const char *button_parts[] =
    {
        PART_POPUP_BUTTON1,
        PART_POPUP_BUTTON2,
        PART_POPUP_BUTTON3,
        NULL
    };
    const char **part = button_parts;
    Evas_Object *button = NULL;

    while(*part && elm_object_part_content_get(popup, *part))
    {
        ++part;
    }

    RETVM_IF(NULL == part, NULL, "No empty button parts left");

    button = elm_button_add(popup);
    RETVM_IF(NULL == button, NULL, "Failed to create button");

    elm_object_style_set(button, STYLE_POPUP_BUTTON);
    elm_object_text_set(button, text);
    elm_object_part_content_set(popup, *part, button);
    evas_object_smart_callback_add(button, EVENT_CLICKED, func, data);

    return button;
}


/**
 * @function		ui_utils_push_button_add
 * @since_tizen		2.3
 * @description		Ui Utils Push Button Add
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, const char*: Const char Pointer
 * @return		Evas_Object*
 */
Evas_Object *ui_utils_push_button_add(void *data, Evas_Object *parent, const char *name, Evas_Smart_Cb cb)
{
	RETVM_IF(NULL == data, NULL, "view is NULL");
	RETVM_IF(NULL == parent, NULL, "parent is NULL");

	Evas_Object *push_button = elm_button_add(parent);
    RETVM_IF(NULL == push_button, NULL, "Failed to create button");

    evas_object_show(push_button);
    elm_object_text_set(push_button, name);
    evas_object_smart_callback_add(push_button, "clicked", cb, data);
    evas_object_size_hint_expand_set(push_button, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_fill_set(push_button, EVAS_HINT_FILL, 0.0);

    return push_button;
}


/**
 * @function		ui_utils_check_add
 * @since_tizen		2.3
 * @description		Ui Utils Check Add
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer
 * @return		Evas_Object*
 */
Evas_Object *ui_utils_check_add(void *data, Evas_Object *parent)
{
	RETVM_IF(NULL == data, NULL, "view is NULL");
	RETVM_IF(NULL == parent, NULL, "parent is NULL");

	Evas_Object *check = elm_check_add(parent);
    RETVM_IF(NULL == check, NULL, "Failed to create button");

    evas_object_show(check);
    evas_object_size_hint_expand_set(check, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_fill_set(check, EVAS_HINT_FILL, 0.0);

    return check;
}


/**
 * @function		ui_utils_label_set_text
 * @since_tizen		2.3
 * @description		Ui Utils Label Set Text
 * @parameter		Evas_Object*: Evas Object Pointer, const char*: Const char Pointer, char*: Char Pointer
 * @return		void
 */
void ui_utils_label_set_text(Evas_Object *parent, const char *text, char *alignment)
{
    char buf[MAX_LENGTH] = {'\0'};
    snprintf(buf, MAX_LENGTH, "<p align=%s >%s</p>",alignment, text);
    elm_object_text_set(parent, buf);
}


/**
 * @function		ui_utils_label_add
 * @since_tizen		2.3
 * @description		Ui Utils Label Add
 * @parameter		Evas_Object*: Evas Object Pointer, const char*: Const char Pointer
 * @return		Evas_Object*
 */
Evas_Object *ui_utils_label_add(Evas_Object *parent, const char *text)
{

    Evas_Object *label = elm_label_add(parent);
    RETVM_IF(NULL == label, NULL, "Failed to create button");

    evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(label, 0.5, 0.5);
    evas_object_show(label);

    char buf[MAX_LENGTH] = {'\0'};
    snprintf(buf, MAX_LENGTH, "<p align=center >%s</p>", text);
    elm_object_text_set(label, buf);

    return label;
}


/**
 * @function		ui_utils_entry_add
 * @since_tizen		2.3
 * @description		Ui Utils Entry Add
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, const char*: Const char Pointer
 * @return		Evas_Object*
 */
Evas_Object *ui_utils_entry_add(void *data, Evas_Object *parent, Elm_Input_Panel_Layout keyboard_layout,  const char *text)
{
	RETVM_IF(NULL == parent, NULL, "parent is NULL");

	Evas_Object *entry = elm_entry_add(parent);
    RETVM_IF(NULL == entry, NULL, "Failed to create button");

	elm_entry_editable_set(entry, EINA_TRUE);
	elm_entry_scrollable_set(entry, EINA_TRUE);
	elm_entry_single_line_set(entry, EINA_TRUE);
	elm_entry_line_wrap_set(entry, ELM_WRAP_WORD);
	elm_object_focus_set(entry, EINA_TRUE);


	elm_entry_input_panel_layout_set(entry, keyboard_layout);

	evas_object_show(entry);

	return entry;
}


/**
 * @function		ui_utils_progressbar_add
 * @since_tizen		2.3
 * @description		Ui Utils Progressbar Add
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer
 * @return		Evas_Object*
 */
Evas_Object *ui_utils_progressbar_add(void *data, Evas_Object *parent, Evas_Smart_Cb slider_drag_stop, Evas_Smart_Cb slider_changed)
{
	RETVM_IF(NULL == data, NULL, "view is NULL");
	RETVM_IF(NULL == parent, NULL, "parent is NULL");

	Evas_Object *slider = elm_slider_add(parent);
    RETVM_IF(NULL == slider, NULL, "Failed to create button");

    elm_slider_horizontal_set(slider, EINA_TRUE);
    evas_object_show(slider);
    elm_slider_indicator_format_set(slider, "%1.2f");
    evas_object_size_hint_weight_set(slider, EVAS_HINT_EXPAND, 0);
    evas_object_size_hint_align_set(slider, EVAS_HINT_FILL, 0);
    elm_slider_value_set(slider, 1);
    elm_slider_indicator_show_set(slider, EINA_TRUE);
    evas_object_smart_callback_add(slider, "slider,drag,stop", slider_drag_stop, data);
    evas_object_smart_callback_add(slider, "changed", slider_changed, data);

    return slider;
}


/**
 * @function		ui_utils_show_toast
 * @since_tizen		2.3
 * @description		Ui Utils Show Toast
 * @parameter		const char*: Const char Pointer, const Evas_Object*: Const Evas Object Pointer
 * @return		void
 */
void ui_utils_show_toast(const char* message, const Evas_Object* data, float timeout)
{
	Evas_Object *popup;
	Evas_Object *win = (Evas_Object*)data;

	popup = elm_popup_add(win);
	elm_object_style_set(popup, "toast");
	evas_object_size_hint_weight_set(popup, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_text_set(popup, message);
	elm_popup_timeout_set(popup, timeout);
	evas_object_show(popup);
}
