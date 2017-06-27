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
 * @file	tbt-list-view.h
 * @brief   creates the main genlist
 * @bug		no known bug
 * @credit
 *
 */
#include <assert.h>
#include <stdbool.h>
#include <Elementary.h>

#include "utils/logger.h"
#include "model/tbt-list.h"
#include "view/tbt-list-view.h"


typedef struct
{
	Evas_Object *list;
	Evas_Object *navi;
    Elm_Object_Item *navi_item;

}list_view;


static void tbt_list_view_fill(list_view *view);
static void _list_view_sel_cb(void *data, Evas_Object *obj, void *event_info);
static void _list_view_destroy_cb(void *data, Evas *e, Evas_Object *obj, void *event_info);


/**
 * @function		tbt_list_view_create
 * @since_tizen		2.3
 * @description		Tbt List View Create
 * @parameter		Evas_Object*: Evas Object Pointer
 * @return		Evas_Object*
 */
Evas_Object *tbt_list_view_create(Evas_Object *navi)
{
	RETVM_IF(NULL == navi, NULL, "navi is null");

    list_view *view = NULL;
    view = calloc(1, sizeof(list_view));
    RETVM_IF(!view, NULL, "calloc failed");

    view->navi = navi;
    view->list = elm_list_add(view->navi);
    if(!view->list)
    {
    	ERR("elm_list_add failed");
        SAFE_DELETE(view);
        return NULL;
    }

    tbt_list_init();
    tbt_list_view_fill(view);

    evas_object_data_set(view->list, "view_data", view);
    evas_object_event_callback_add(view->list, EVAS_CALLBACK_FREE, _list_view_destroy_cb, view);

    view->navi_item = elm_naviframe_item_push(view->navi, "TBT", NULL, NULL, view->list, NULL);
    RETVM_IF(!view->navi_item, NULL, "elm_naviframe_item_push failed");
    return view->list;
}


/**
 * @function		tbt_list_view_fill
 * @since_tizen		2.3
 * @description		Tbt List View Fill
 * @parameter		list_view*: List View Pointer
 * @return		static void
 */
static void tbt_list_view_fill(list_view *view)
{
    unsigned count = 0;
    const tbt_info *end;
    const tbt_info *item;

    item = tbt_list_get(&count);
    end = item + count;

    RETM_IF(!item, "item is NULL");

    for(; item != end; ++item)
    {
    	if(item->apptype == TBT_APP_CAMERA_CAPTURE ||
    	   item->apptype == TBT_APP_IMAGEVIEW_JPEG)
    	{
    		elm_list_item_append(view->list, item->name, NULL, NULL, _list_view_sel_cb, item);
    	}
    	else if(item->apptype == TBT_APP_SENSOR_ACCELEROMETER)
    	{
    		bool is_supported = true;
    		sensor_is_supported(item->sensortype, &is_supported);

    		if(is_supported)
    		{
    			elm_list_item_append(view->list, item->name, NULL, NULL, _list_view_sel_cb, item);
    		}
    	}
    }
}


/**
 * @function		_list_view_sel_cb
 * @since_tizen		2.3
 * @description		 List View Sel Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _list_view_sel_cb(void *data, Evas_Object *obj, void *event_info)
{
    RETM_IF(!data, "data is NULL");
    

    elm_list_item_selected_set(event_info, EINA_FALSE);

}


/**
 * @function		_list_view_destroy_cb
 * @since_tizen		2.3
 * @description		 List View Destroy Cb
 * @parameter		void*: Void Pointer, Evas*: Evas Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _list_view_destroy_cb(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
    SAFE_DELETE(data);
}
