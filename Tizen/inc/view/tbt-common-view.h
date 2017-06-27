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
 * @file	tbt-common-view.h
 * @since_tizen 2.3
 * @brief	A common view defination used by all other view
 * @bug		camera preview is shown in rotated manner
 * @credit	Tizen SDK Version 2.3.0 Sample Example
 */

#ifndef __TBT_COMMON_VIEW_H__
#define __TBT_COMMON_VIEW_H__

#include <Evas.h>
#include "model/tbt-list.h"
#include "system_info.h"

struct _common_view
{
    Evas_Object *navi;
    Elm_Object_Item *navi_item;
    tbt_info *tbt_info;

    Evas_Object *layout;
    Evas_Object *grid;

    Evas_Object *label;
    Evas_Object *text_widget;
    Elm_Object_Item *item;

    Evas_Object *toolbar;
    Elm_Object_Item* toolbar_items[TOOLBAR_ITEM_INDEX_COUNT_MAX];

    unsigned int image_id;

    void* app_view;
    void (*_app_destroy_cb)(void*);
    void (*_app_destroy_before_layout_destroy_cb)(void*);
};


/**
 * @typedef common_view
 * @since_tizen 2.3
 * @brief A common view handle.
 */
typedef struct _common_view common_view;

/**
 * @brief Add commonr view
 * @since_tizen 2.3 
 * @param[in] app The pointer to _app_data instance
 * @param[in] navi The naviframe object
 * @return common view data
 */
common_view *common_view_add(Evas_Object *navi, tbt_info *tbt_info, Elm_Object_Item *item, common_view *view, void (*_app_destroy_cb)(void*), void* app_view);
void common_view_set_appdestroy_before_layout_destroy_cb(common_view *view,void (*_app_destroy_before_layout_destroy_cb)(void*));
#endif // __TBT_COMMON_VIEW_H__
