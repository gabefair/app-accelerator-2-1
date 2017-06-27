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
 * @file	tbt-efl-callbacks.c
 * @brief	Checks eext_object_event_callback_del() and eext_object_event_callback_add() functionality.
 *
 * @bug		NA
 * @credit
 *
 */

#include "utils/app_module_config.h"
#ifdef TBT_MODULE_EFL

#include <Elementary.h>
#include <locations.h>

#include "utils/logger.h"
#include "utils/config.h"
#include "utils/ui-utils.h"
#include "view/tbt-efl-callbacks.h"
#include "view/tbt-common-view.h"
#include "utils/config.h"

bool __is_efl_test_running = false;

struct _efl_callbacks_view {
	common_view* view;
	location_manager_h manager;

	Evas_Object *lat_label;
	Evas_Object *lon_label;
	Evas_Object *action_btn;
	bool callbackAdded;
};

static void _app_destroy_cb(void* this);
static Evas_Object *add_control_layout(efl_callbacks_view *this,
		Evas_Object *parent);
static void _control_button_pressed_cb(void *data, Evas_Object *obj,
		void *event_info);
static void update(efl_callbacks_view *this);
static void on_hardware_back_button_pressed(void *data, Evas_Object *obj,
		void *event_info);


/**
 * @function		efl_callbacks_view_add
 * @since_tizen		2.3
 * @description		Efl Callbacks View Add
 * @parameter		Evas_Object*: Evas Object Pointer, tbt_info*: Tbt Info Pointer, Elm_Object_Item*: Elm Object Item Pointer
 * @return		efl_callbacks_view*
 */
efl_callbacks_view *efl_callbacks_view_add(Evas_Object *navi, 		tbt_info *tbt_info, Elm_Object_Item *item)
{
	RETVM_IF(NULL == navi, NULL, "navi is null");

	efl_callbacks_view *this = NULL;
	this = calloc(1, sizeof(efl_callbacks_view));
	RETVM_IF(!this, NULL, "calloc failed");
	this->view = calloc(1, sizeof(common_view));
	RETVM_IF(!this->view, NULL, "calloc failed");

	tbt_info->layout_group = "efl_test";
	tbt_info->layout_file = get_edje_path("efl_callback_viewer.edj");

	common_view_add(navi, tbt_info, item, this->view, _app_destroy_cb, this);

	RETVM_IF(NULL == this->view, NULL, "navi is null");

	this->lat_label = ui_utils_label_add(this->view->layout, "???");
	this->lon_label = ui_utils_label_add(this->view->layout, "???");

	elm_label_line_wrap_set(this->lat_label, ELM_WRAP_CHAR);
	elm_label_line_wrap_set(this->lat_label, ELM_WRAP_CHAR);

	ui_utils_label_set_text(this->lat_label, "???", "left");
	ui_utils_label_set_text(this->lon_label, "???", "left");

	elm_object_part_content_set(this->view->layout, "_title", this->lat_label);
	elm_object_part_content_set(this->view->layout, "_callback",
			this->lon_label);

	Evas_Object *table = add_control_layout(this, this->view->layout);
	elm_object_part_content_set(this->view->layout, "controlr_part", table);

	__is_efl_test_running = true;

	this->callbackAdded = true;
	eext_object_event_callback_add(this->action_btn, EEXT_CALLBACK_BACK,
			on_hardware_back_button_pressed, this);
	update(this);

	return this;
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

	efl_callbacks_view *view = NULL;
	view = (efl_callbacks_view*) this;
	RETM_IF(NULL == view, "view is NULL");

	__is_efl_test_running = false;

	SAFE_DELETE(view->view);
	SAFE_DELETE(view);
}


/**
 * @function		add_control_layout
 * @since_tizen		2.3
 * @description		Add Control Layout
 * @parameter		efl_callbacks_view*: Efl Callbacks View Pointer, Evas_Object*: Evas Object Pointer
 * @return		static Evas_Object*
 */
static Evas_Object *add_control_layout(efl_callbacks_view *this, 		Evas_Object *parent)
{
	RETVM_IF(NULL == this, NULL, "view is NULL");

	Evas_Object *table = elm_table_add(parent);

	evas_object_size_hint_weight_set(table, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(table, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_show(table);

	elm_table_padding_set(table, TABLE_PADDING_HORIZONTAL,
			TABLE_PADDING_VERTICAL);

	this->action_btn = ui_utils_push_button_add(this, table, "Button",
			_control_button_pressed_cb);
	elm_table_pack(table, this->action_btn, 0, 0, 1, 1);

	return table;
}


/**
 * @function		_control_button_pressed_cb
 * @since_tizen		2.3
 * @description		 Control Button Pressed Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _control_button_pressed_cb(void *data, Evas_Object *obj, 		void *event_info)
{
	efl_callbacks_view *this = (efl_callbacks_view*) data;
	RETM_IF(NULL == this, "efl_callbacks_view is null");

	if (this->callbackAdded)
	{
		this->callbackAdded = false;
		eext_object_event_callback_del(this->action_btn, EEXT_CALLBACK_BACK,
				on_hardware_back_button_pressed);
	}
	else
	{
		this->callbackAdded = true;
		eext_object_event_callback_add(this->action_btn, EEXT_CALLBACK_BACK,
				on_hardware_back_button_pressed, this);
	}

	update(this);

}


/**
 * @function		update
 * @since_tizen		2.3
 * @description		Update
 * @parameter		efl_callbacks_view*: Efl Callbacks View Pointer
 * @return		static void
 */
static void update(efl_callbacks_view *this)
{
	RETM_IF(NULL == this, "efl_callbacks_view is null");

	if (this->callbackAdded)
	{
		ui_utils_label_set_text(this->lat_label, "Callback added.", "left");
		elm_object_text_set(this->action_btn, "Delete Callback");
		ui_utils_label_set_text(this->lon_label, "Waiting for callback...",
				"left");
	}
	else
	{
		ui_utils_label_set_text(this->lat_label, "Callback deleted.", "left");
		elm_object_text_set(this->action_btn, "Add Callback");
		ui_utils_label_set_text(this->lon_label, "Waiting for callback...",
				"left");
	}
}


/**
 * @function		on_hardware_back_button_pressed
 * @since_tizen		2.3
 * @description		On Hardware Back Button Pressed
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void on_hardware_back_button_pressed(void *data, Evas_Object *obj, 		void *event_info)
{
	efl_callbacks_view *this = (efl_callbacks_view*) data;
	RETM_IF(NULL == this, "efl_callbacks_view is null");

	ui_utils_label_set_text(this->lon_label, " BACK BUTTON PRESS DETECTED ",
			"left");
}

#endif
