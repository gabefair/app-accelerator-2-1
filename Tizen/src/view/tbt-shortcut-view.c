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
 * @file	tbt-shortcut-view.c
 * @brief	create shortcut of application in home screen
 *
 *
 * @bug		Do not show icon and name of shortcut
 * @credit
 *
 */
#include "utils/app_module_config.h"
#ifdef TBT_MODULE_SHORTCUT

#include <glib.h>
#include <Elementary.h>

#include "utils/logger.h"
#include "utils/config.h"
#include "utils/ui-utils.h"
#include "view/tbt-shortcut-view.h"
#include "view/tbt-common-view.h"

struct _shortcut_view
{
	common_view* view;

	GList *devices_list;

	Evas_Object *action_btn;
	Evas_Object *shortcut_label;
	Evas_Object *cBox_label;
	Evas_Object *cBox;
	Evas_Object *cBox2;
	Eina_Bool state;

};

static Evas_Object *add_control_layout(shortcut_view *this, Evas_Object *parent);
static void _app_destroy_cb(void* this);
static void _control_button_pressed_cb(void *data, Evas_Object *obj, void *event_info);
static int _result_cb(int ret, void *data);
char* get_shortcut_error(int err);


/**
 * @function		shortcut_view_add
 * @since_tizen		2.3
 * @description		Shortcut View Add
 * @parameter		Evas_Object*: Evas Object Pointer, tbt_info*: Tbt Info Pointer, Elm_Object_Item*: Elm Object Item Pointer
 * @return		shortcut_view*
 */
shortcut_view *shortcut_view_add(Evas_Object *navi, tbt_info *tbt_info, Elm_Object_Item *item)
{


	RETVM_IF(NULL == navi, NULL, "navi is null");

	shortcut_view *this = NULL;
    this = calloc(1, sizeof(shortcut_view));
    RETVM_IF(!this, NULL, "calloc failed");

    this->view = calloc(1, sizeof(common_view));
    RETVM_IF(!this->view, NULL, "calloc failed");

    tbt_info->layout_group = "shortcut_viewer";
    tbt_info->layout_file = get_edje_path("shortcut_viewer.edj");

    common_view_add(navi, tbt_info, item, this->view, _app_destroy_cb, this);

	RETVM_IF(NULL == this->view, NULL, "navi is null");

	this->shortcut_label = ui_utils_label_add(this->view->layout, "");
	elm_object_part_content_set(this->view->layout, "shortcut_text", this->shortcut_label);

	this->cBox_label = ui_utils_label_add(this->view->layout, "Allow duplicate");
	elm_object_part_content_set(this->view->layout, "shortcut_body", this->cBox_label);

	this->cBox = elm_check_add(this->view->layout);
	evas_object_resize(this->cBox, 20, 20);
	elm_check_state_set(this->cBox, EINA_FALSE);
	elm_object_text_set(this->cBox , "Allow duplicate");
	elm_check_state_pointer_set(this->cBox, NULL);
	elm_object_part_content_set(this->view->layout, "shortcut_body2", this->cBox);
	this->state = elm_check_state_get(this->cBox);

	Evas_Object *control = add_control_layout(this, this->view->layout);
	elm_object_part_content_set(this->view->layout, "controlr_part", control);

    return this;
}


/**
 * @function		add_control_layout
 * @since_tizen		2.3
 * @description		Add Control Layout
 * @parameter		shortcut_view*: Shortcut View Pointer, Evas_Object*: Evas Object Pointer
 * @return		static Evas_Object*
 */
static Evas_Object *add_control_layout(shortcut_view *this, Evas_Object *parent)
{
	RETVM_IF(NULL == this, NULL, "view is NULL");

    Evas_Object *table = elm_table_add(parent);

    evas_object_size_hint_weight_set(table, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(table, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(table);

    elm_table_padding_set(table, TABLE_PADDING_HORIZONTAL, TABLE_PADDING_VERTICAL);

	this->action_btn = ui_utils_push_button_add(this, table, "Add shortcut", _control_button_pressed_cb);
	elm_table_pack(table, this->action_btn, 1, 0, 1, 1);

    return table;
}


/**
 * @function		_app_destroy_cb
 * @since_tizen		2.3
 * @description		 App Destroy Cb
 * @parameter		void*: Void Pointer
 * @return		static void
 */
static void _app_destroy_cb(void* this)
{
	RETM_IF(NULL == this, "data is NULL");

	shortcut_view *view = NULL;
	view = (shortcut_view*)this;
	RETM_IF(NULL == view, "view is NULL");

	SAFE_DELETE(view->view);
	SAFE_DELETE(view);
}


/**
 * @function		_control_button_pressed_cb
 * @since_tizen		2.3
 * @description		 Control Button Pressed Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _control_button_pressed_cb(void *data, Evas_Object *obj, void *event_info)
{
	DBG("Inside _control_button_pressed_cb");

	RETM_IF(NULL == data, "data is NULL");

	shortcut_view *this = NULL;
	this = (shortcut_view*)data;
	RETM_IF(NULL == this, "view is NULL");

	int result;
	char* name = "new shortcut";
	char* icon = (char*)get_shared_resource_path("tbtcoreapp.png");
	int allow_duplicate;

	if(elm_check_state_get(this->cBox) == EINA_TRUE){
		allow_duplicate = 1;
	}
	else
		allow_duplicate = 0;

	result = shortcut_add_to_home(name,  LAUNCH_BY_APP, NULL, icon, allow_duplicate, _result_cb, this);

	if(result == 0)
	{
		ui_utils_label_set_text(this->shortcut_label, "Creating shortcut...", "left");
		DBG("2");
	}
	else
		ui_utils_label_set_text(this->shortcut_label, "Shortcut api failed!", "left");

	DBG("icon path = %s",icon);
	DBG("Inside shortcut_add_to_home %s, allow_duplicate = %d, result = %d, %s", get_shortcut_error(result), allow_duplicate, result, get_error_message(result));
}

char* get_shortcut_error(int err){

	char* error;

	switch(err){

	case SHORTCUT_ERROR_NONE:
		error = "SHORTCUT_ERROR_NONE";
		break;
	case SHORTCUT_ERROR_INVALID_PARAMETER:
		error = "SHORTCUT_ERROR_INVALID_PARAMETER";
		break;
	case SHORTCUT_ERROR_OUT_OF_MEMORY:
		error = "SHORTCUT_ERROR_OUT_OF_MEMORY";
		break;
	case SHORTCUT_ERROR_IO_ERROR:
		error = "SHORTCUT_ERROR_IO_ERROR";
		break;
	case SHORTCUT_ERROR_PERMISSION_DENIED:
		error = "SHORTCUT_ERROR_PERMISSION_DENIED";
		break;
	case SHORTCUT_ERROR_NOT_SUPPORTED:
		error = "SHORTCUT_ERROR_NOT_SUPPORTED";
		break;
	case SHORTCUT_ERROR_RESOURCE_BUSY:
		error = "SHORTCUT_ERROR_RESOURCE_BUSY";
		break;
	case SHORTCUT_ERROR_NO_SPACE:
		error = "SHORTCUT_ERROR_NO_SPACE";
		break;
	case SHORTCUT_ERROR_EXIST:
		error = "SHORTCUT_ERROR_EXIST";
		break;
	case SHORTCUT_ERROR_FAULT:
		error = "SHORTCUT_ERROR_FAULT";
		break;
	case SHORTCUT_ERROR_COMM:
		error = "SHORTCUT_ERROR_COMM";
		break;
	default:
		error = "UNKNOWN";
		break;
	}
	return error;
}


/**
 * @function		_result_cb
 * @since_tizen		2.3
 * @description		 Result Cb
 * @parameter		int: Int, void*: Void Pointer
 * @return		static int
 */
static int _result_cb(int ret, void *data)
{
	DBG("Inside _result_cb");
	DBG("result is %d %s", ret,get_error_message(ret));

	shortcut_view *this = NULL;
	this = (shortcut_view*)data;

	if(ret == 0)
	{
		ui_utils_label_set_text(this->shortcut_label, "Shortcut Created", "left");
		DBG("1");
	}
	else
		ui_utils_label_set_text(this->shortcut_label, "Unable to create shortcut", "left");

	return ret;

	}

#endif
