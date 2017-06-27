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
 *******************************************************************************/

/**
 * @file	ui-utils.h
 * @brief	This file provide necessary Util functions UI.
 * @since_tizen 2.3
 * @bug
 * @credit
 */


#ifndef __UI_UTILS_H__
#define __UI_UTILS_H__

#include "utils/app_module_config.h"

#include <Evas.h>

#ifdef TIZEN_3_0
	#include <efl_extension.h>
#endif
#ifdef TIZEN_2_4
	#include <efl_extension.h>
#endif
#ifdef TIZEN_2_3_1
	#include "efl_extension_events.h"
#endif
#include <stdbool.h>

#define EVENT_CLICKED                   "clicked"
#define EVENT_CHANGED                   "changed"
#define EVENT_UNFOCUSED                 "unfocused"
#define EVENT_DATETIME_CLICKED          "date,btn,clicked"
#define EVENT_PICKER_VALUE_SET          "picker,value,set"

#define SIGNAL_PICKER_ACTION_VALUE_SET  "picker,action,value,set"

#define PART_NAVI_TITLE_RIGHT_BUTTON    "title_right_btn"
#define PART_ENTRY_GUIDE                "elm.guide"
#define PART_POPUP_TITLE                "title,text"
#define PART_TOOLBAR                    "toolbar"
#define PART_TOOLBAR_MORE_BUTTON        "toolbar_more_btn"
#define PART_ENTRY                      "elm.icon.entry"
#define PART_SWALLOW_CONTENT            "elm.swallow.content"
#define PART_SWALLOW_BG                 "elm.swallow.bg"
#define PART_NAVI_ITEM_TABBAR           "tabbar"

#define STYLE_BUTTON_NAVIFRAME          "naviframe/title_text"
#define STYLE_TOOLBAR_TABBAR            "tabbar/item_with_title"
#define STYLE_NAVI_ITEM_TABBAR          "tabbar"

enum
{
    TOOLBAR_ITEM_INDEX_PASS = 0,
    TOOLBAR_ITEM_INDEX_FAIL,
    TOOLBAR_ITEM_INDEX_INFO,
    TOOLBAR_ITEM_INDEX_COUNT_MAX
} TOOLBAR_ITEM_INDEXS;

static const char *caption_ok = "OK";

static const char *toolbar_item_names[TOOLBAR_ITEM_INDEX_COUNT_MAX] = { "Pass",
																	    "Fail",
																	    "Info"
};

static const int toolbar_item_ids[TOOLBAR_ITEM_INDEX_COUNT_MAX] = { TOOLBAR_ITEM_INDEX_PASS,
																	TOOLBAR_ITEM_INDEX_FAIL,
																	TOOLBAR_ITEM_INDEX_INFO
};

Evas_Object *ui_utils_icon_create_new(Evas_Object *parent, const char *icon_name);

void (*_tbt_pause_resume_cb) (bool) ;
void (*_tbt_app_terminate_cb) () ;
/**
* @function		DoesFeatureExixts
* @since_tizen 2.3
* @description		Checks if a particular feature exists or not
* @parameter[IN]	Feature Key
* @return		bool The result of feature search
*/
bool does_feature_exist(char* feature);


/*
 * @brief Add naviframe widget
 * @since_tizen 2.3
 * @param[in]   parent      Parent widget
 * @param[in]   back_cb     Hardware back button callback
 * @param[in]   cb_data     Data to pass to back_cb
 * @return Naviframe on success, otherwise NULL
 */
Evas_Object *ui_utils_icon_create(Evas_Object *parent, const char *icon_name);

/*
 * @brief Add naviframe widget
 * @since_tizen 2.3
 * @param[in]   parent      Parent widget
 * @param[in]   back_cb     Hardware back button callback
 * @param[in]   cb_data     Data to pass to back_cb
 * @return Naviframe on success, otherwise NULL
 */
Evas_Object *ui_utils_create_image(Evas_Object *parent, const char *file_path);

/*
 * @brief Add naviframe widget
 * @since_tizen 2.3
 * @param[in]   parent      Parent widget
 * @param[in]   back_cb     Hardware back button callback
 * @param[in]   cb_data     Data to pass to back_cb
 * @return Naviframe on success, otherwise NULL
 */
Evas_Object *ui_utils_navi_add(Evas_Object *parent, Eext_Event_Cb back_cb, void *cb_data);

/*
 * @brief Add layout widget
 * @since_tizen 2.3
 * @param[in]   parent      Parent widget
 * @param[in]   destroy_cb  Layout destroy callback
 * @param[in]   cb_data     Data to pass to destroy_cb
 * @return Layout on success, otherwise NULL
 */
Evas_Object *ui_utils_layout_add(Evas_Object *parent, Evas_Object_Event_Cb destroy_cb, void *cb_data);

/**
 * @brief Add button to popup
 * @since_tizen 2.3
 * @remarks Added button automatically hides popup when clicked so you don't
 *          need to hide it in your callback and create callbacks for cancel buttons.
 * @param[in]   popup   Popup widget
 * @param[in]   text    Button text
 * @param[in]   func    Button click callback
 * @param[in]   data    Data to pass to func
 * @return true button on success, otherwise NULL
 */
Evas_Object * ui_utils_popup_button_add(Evas_Object *popup, const char *text, Evas_Smart_Cb func, void *data);

/*
 * @brief Add toolbar widget
 * @since_tizen 2.3
 * @param[in]   parent      Parent widget
 * @return Toolbar on success, otherwise NULL
 */
Evas_Object *ui_utils_toolbar_add(Evas_Object *parent);

/*
 * @brief Add toolbar widget
 * @since_tizen 2.3
 * @param[in]   parent      Parent widget
 * @return Toolbar on success, otherwise NULL
 */
Evas_Object *ui_utils_popup_add(Evas_Object *parent, const char *text);

/*
 * @brief Add toolbar widget
 * @since_tizen 2.3
 * @param[in]   parent      Parent widget
 * @return Toolbar on success, otherwise NULL
 */
Evas_Object *ui_utils_push_button_add(void *data, Evas_Object *parent, const char *name, Evas_Smart_Cb cb);

/*
 * @brief Add toolbar widget
 * @since_tizen 2.3
 * @param[in]   parent      Parent widget
 * @return Toolbar on success, otherwise NULL
 */
Evas_Object *ui_utils_check_add(void *data, Evas_Object *parent);

/*
 * @brief Add toolbar widget
 * @since_tizen 2.3
 * @param[in]   parent      Parent widget
 * @return Toolbar on success, otherwise NULL
 */
void ui_utils_label_set_text(Evas_Object *parent, const char *text, char *alignment);

/*
 * @brief Add toolbar widget
 * @since_tizen 2.3
 * @param[in]   parent      Parent widget
 * @return Toolbar on success, otherwise NULL
 */
Evas_Object *ui_utils_label_add(Evas_Object *parent, const char *text);

/*
 * @brief Add toolbar widget
 * @since_tizen 2.3
 * @param[in]   parent      Parent widget
 * @return Toolbar on success, otherwise NULL
 */
Evas_Object *ui_utils_entry_add(void *data, Evas_Object *parent, Elm_Input_Panel_Layout keyboard_layout, const char *text);

/*
 * @brief Add toolbar widget
 * @since_tizen 2.3
 * @param[in]   parent      Parent widget
 * @return Toolbar on success, otherwise NULL
 */
Evas_Object *ui_utils_progressbar_add(void *data, Evas_Object *parent, Evas_Smart_Cb slider_drag_stop, Evas_Smart_Cb slider_changed);

/*
 * @brief Add toast
 * @since_tizen 2.3
 * */
void ui_utils_show_toast(const char* message, const Evas_Object* data, float timeout);
#endif /* __UI_UTILS_H__ */
