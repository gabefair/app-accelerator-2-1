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
 * @file	tbt-common-view.h
 * @brief	A common view defination used by all other view
 * @bug		camera preview is shown in rotated manner
 * @credit	Tizen SDK Version 2.3.0 Sample Example
 */
#include <player.h>
#include <Elementary.h>

#include "utils/logger.h"
#include "utils/config.h"
#include "utils/ui-utils.h"
#include "view/tbt-common-view.h"




static void update_view_controls(common_view *view);
static bool create_layout_elements(common_view *view);
static Evas_Object *create_toolbar(common_view *view);

#ifndef DEVICE_TYPE_MOBILE
	static Evas_Object *create_wearable_toolbar(common_view *view);
#endif

static void toolbar_item_info_popup(common_view *view);
static void toolbar_item_pass_pressed(common_view *view);
static void toolbar_item_fail_pressed(common_view *view);
static void _toolbar_button_clicked_cb(void *data, Evas_Object *obj, void *event_info);
static void _toolbar_item_popup_destroy_cb(void *data, Evas_Object *obj, void *event_info);
static void _main_layout_destroy_cb(void *data, Evas *e, Evas_Object *obj, void *event_info);

#ifndef DEVICE_TYPE_MOBILE
static void wearable_toolbar_pass_pressed(void *data, Evas_Object *obj, void *event_info);
static void wearable_toolbar_fail_pressed(void *data, Evas_Object *obj, void *event_info);
static void wearable_toolbar_info_pressed(void *data, Evas_Object *obj, void *event_info);
#endif


common_view *common_view_add(Evas_Object *navi, tbt_info *tbt_info, Elm_Object_Item *item, common_view *view, void (*_app_destroy_cb)(void*), void* app_view)
{
	RETVM_IF(NULL == navi, NULL, "navi is null");
	RETVM_IF(NULL == view, NULL, "view is null");
	RETVM_IF(NULL == tbt_info, NULL, "tbt_info is null");
	RETVM_IF(NULL == item, NULL, "item is null");

	view->navi = navi;
    view->item = item;
    view->app_view = app_view;
    view->tbt_info = tbt_info;
    view->layout = ui_utils_layout_add(navi, _main_layout_destroy_cb, view);
    view->_app_destroy_cb = _app_destroy_cb;

	if(NULL == view->layout)
	{
		RETVM_IF(!view->layout, NULL, "ui_utils_layout_add failed");
		SAFE_DELETE(view);
		return NULL;
	}

	if(false == create_layout_elements(view))
	{
		evas_object_del(view->layout);
		SAFE_DELETE(view);
		return NULL;
	}

    view->navi_item = elm_naviframe_item_push(view->navi, view->tbt_info->name, NULL, NULL, view->layout, NULL);
    #ifdef DEVICE_TYPE_MOBILE
    	elm_object_item_part_content_set(elm_naviframe_top_item_get(view->navi), "toolbar", view->toolbar);
    #endif

    update_view_controls(view);

    return view;
}


/**
 * @function		update_view_controls
 * @since_tizen		2.3
 * @description		Update View Controls
 * @parameter		common_view*: Common View Pointer
 * @return		static void
 */
static void update_view_controls(common_view *view)
{
	RETM_IF(NULL == view, "view is NULL");

}


/**
 * @function		create_layout_elements
 * @since_tizen		2.3
 * @description		Create Layout Elements
 * @parameter		common_view*: Common View Pointer
 * @return		static bool
 */
static bool create_layout_elements(common_view *view)
{
	RETVM_IF(NULL == view, false, "create of layout elements failed");

	elm_layout_file_set(view->layout, view->tbt_info->layout_file, view->tbt_info->layout_group);

	evas_object_size_hint_weight_set(view->layout, EVAS_HINT_FILL, EVAS_HINT_FILL);
	evas_object_size_hint_align_set(view->layout, EVAS_HINT_FILL, EVAS_HINT_FILL);

	int x,y,w,h;
	evas_object_geometry_get(view->layout, &x, &y, &w, &h);
	DBG("layout Dim: %d %d %d %d", x, y, w, h);


	evas_object_show(view->layout);

	#ifdef DEVICE_TYPE_MOBILE
//		view->toolbar = create_toolbar(view);
	#else
//		view->toolbar = create_wearable_toolbar(view);
	#endif

//	RETVM_IF(NULL == view->toolbar, false, "create of layout toolbar elements failed");

    return true;
}

#ifndef DEVICE_TYPE_MOBILE

/**
 * @function		create_wearable_toolbar
 * @since_tizen		2.3
 * @description		Create Wearable Toolbar
 * @parameter		common_view*: Common View Pointer
 * @return		static Evas_Object*
 */
static Evas_Object *create_wearable_toolbar(common_view *view)
{
	DBG("Inside create_wearable_toolbar");
	RETVM_IF(NULL == view, NULL, "view is null");
	Evas_Object *table = elm_table_add(view->layout);

	evas_object_size_hint_weight_set(table, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(table, EVAS_HINT_FILL, EVAS_HINT_FILL);

	Evas_Object* pass_btn = ui_utils_push_button_add(view, table, "Pass", wearable_toolbar_pass_pressed);
	Evas_Object* fail_btn = ui_utils_push_button_add(view, table, "Fail", wearable_toolbar_fail_pressed);
	Evas_Object* info_btn = ui_utils_push_button_add(view, table, "Info", wearable_toolbar_info_pressed);

	evas_object_show(table);
	elm_table_pack(table, pass_btn, 0, 0, 1, 1);
	elm_table_pack(table, fail_btn, 1, 0, 1, 1);
	elm_table_pack(table, info_btn, 2, 0, 1, 1);
	elm_object_part_content_set(view->layout, "wearable_toolbar", table);
	return table;
}
#endif

#ifndef DEVICE_TYPE_MOBILE

/**
 * @function		wearable_toolbar_pass_pressed
 * @since_tizen		2.3
 * @description		Wearable Toolbar Pass Pressed
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void wearable_toolbar_pass_pressed(void *data, Evas_Object *obj, void *event_info)
{
	RETM_IF(NULL == data, "data is null");
	RETM_IF(NULL == obj, "obj is null");
	

	common_view *view =(common_view*) data;
	toolbar_item_pass_pressed(view);
}


/**
 * @function		wearable_toolbar_fail_pressed
 * @since_tizen		2.3
 * @description		Wearable Toolbar Fail Pressed
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void wearable_toolbar_fail_pressed(void *data, Evas_Object *obj, void *event_info)
{
	RETM_IF(NULL == data, "data is null");
	RETM_IF(NULL == obj, "obj is null");
	

	common_view *view =(common_view*) data;
	toolbar_item_fail_pressed(view);
}


/**
 * @function		wearable_toolbar_info_pressed
 * @since_tizen		2.3
 * @description		Wearable Toolbar Info Pressed
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void wearable_toolbar_info_pressed(void *data, Evas_Object *obj, void *event_info)
{
	RETM_IF(NULL == data, "data is null");
	RETM_IF(NULL == obj, "obj is null");
	

	common_view *view =(common_view*) data;
	toolbar_item_info_popup(view);
}

#endif



/**
 * @function		create_toolbar
 * @since_tizen		2.3
 * @description		Create Toolbar
 * @parameter		common_view*: Common View Pointer
 * @return		static Evas_Object*
 */
static Evas_Object *create_toolbar(common_view *view)
{
    RETVM_IF(NULL == view, NULL, "view is null");

    Evas_Object *toolbar = NULL;
    toolbar = ui_utils_toolbar_add(view->navi);
    RETVM_IF(NULL == toolbar, NULL, "toolbar is null");
    elm_toolbar_select_mode_set(toolbar, ELM_OBJECT_SELECT_MODE_NONE);

    unsigned int i;
    for(i = 0; i < TOOLBAR_ITEM_INDEX_COUNT_MAX; ++i)
    {
        view->toolbar_items[i] = elm_toolbar_item_append(toolbar, NULL, toolbar_item_names[i], _toolbar_button_clicked_cb, view);
        evas_object_data_set(elm_toolbar_item_object_get(view->toolbar_items[i]), TOOLBAR_BUTTON_DATA_KEY, (void*)(&toolbar_item_ids[i]));
    }

    return toolbar;
}


/**
 * @function		toolbar_item_pass_pressed
 * @since_tizen		2.3
 * @description		Toolbar Item Pass Pressed
 * @parameter		common_view*: Common View Pointer
 * @return		static void
 */
static void toolbar_item_pass_pressed(common_view *view)
{
    RETM_IF(NULL == view, "view is NULL");

    view->tbt_info->result = TBT_INFO_RESULT_PASS;
    elm_genlist_item_update(view->item);
    elm_naviframe_item_pop(view->navi);
}


/**
 * @function		toolbar_item_fail_pressed
 * @since_tizen		2.3
 * @description		Toolbar Item Fail Pressed
 * @parameter		common_view*: Common View Pointer
 * @return		static void
 */
static void toolbar_item_fail_pressed(common_view *view)
{
    RETM_IF(NULL == view, "view is NULL");

    view->tbt_info->result = TBT_INFO_RESULT_FAIL;
    elm_genlist_item_update(view->item);
    elm_naviframe_item_pop(view->navi);
}


/**
 * @function		_toolbar_button_clicked_cb
 * @since_tizen		2.3
 * @description		 Toolbar Button Clicked Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _toolbar_button_clicked_cb(void *data, Evas_Object *obj, void *event_info)
{
    RETM_IF(NULL == data, "data is NULL");
    

    common_view *view = NULL;
    view = (common_view*)data;

    void *item_data = evas_object_data_get(elm_toolbar_item_object_get(event_info), TOOLBAR_BUTTON_DATA_KEY);
    RETM_IF(NULL == item_data, "Toolbar item data is NULL");
    int item_index = *((int*)item_data);

switch(item_index)
{
        case TOOLBAR_ITEM_INDEX_PASS:
        	toolbar_item_pass_pressed(view);
            break;
        case TOOLBAR_ITEM_INDEX_FAIL:
        	toolbar_item_fail_pressed(view);
        	break;
        case TOOLBAR_ITEM_INDEX_INFO:
        	toolbar_item_info_popup(view);
            break;
        default:
            ERR("Not supported button");
            break;
    }
}


/**
 * @function		toolbar_item_info_popup
 * @since_tizen		2.3
 * @description		Toolbar Item Info Popup
 * @parameter		common_view*: Common View Pointer
 * @return		static void
 */
static void toolbar_item_info_popup(common_view *view)
{
    RETM_IF(NULL == view, "view is NULL");

    Evas_Object *popup = ui_utils_popup_add(view->navi, toolbar_item_names[TOOLBAR_ITEM_INDEX_INFO]);
    RETM_IF(NULL == popup, "popup is not created");
    elm_object_text_set(popup, view->tbt_info->info);

    ui_utils_popup_button_add(popup, caption_ok, _toolbar_item_popup_destroy_cb, popup);
}


/**
 * @function		_toolbar_item_popup_destroy_cb
 * @since_tizen		2.3
 * @description		 Toolbar Item Popup Destroy Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _toolbar_item_popup_destroy_cb(void *data, Evas_Object *obj, void *event_info)
{
    RETM_IF(NULL == data, "data is null");

    Evas_Object *obj_to_del = data;
    evas_object_del(obj_to_del);

    if(event_info)
    {
        Elm_Object_Item *item = event_info;
        elm_genlist_item_selected_set(item, EINA_FALSE);
    }
}


/**
 * @function		_main_layout_destroy_cb
 * @since_tizen		2.3
 * @description		 Main Layout Destroy Cb
 * @parameter		void*: Void Pointer, Evas*: Evas Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _main_layout_destroy_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
    RETM_IF(NULL == data, "data is NULL");

    common_view *view = NULL;
    view = (common_view*)data;
    RETM_IF(NULL == view, "view is NULL");

    evas_object_del(view->layout);

    (*view->_app_destroy_cb)(view->app_view);
}
