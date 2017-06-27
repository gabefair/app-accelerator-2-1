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
 * @file	tbt-ui-view.h
 * @brief	This view provide the apps to taste the UI resize, shape, drawable, alpha and font features of the elm APIs
 * @since_tizen 2.3
 * @bug		No known bug.
 * @credit	Tizen SDK Version 2.3.0 Sample Example
 *
 */

#ifndef __TBT_UI_VIEW_H__
#define __TBT_UI_VIEW_H__

#include "utils/app_module_config.h"
#ifdef TBT_MODULE_UI

#include <Evas.h>
#include "model/tbt-list.h"


/**
 * @typedef ui_view
 * @since_tizen 2.3
 * @brief An ui view handle.
 */
typedef struct _ui_view ui_view;

/**
 * @brief Create UI view
 * @since_tizen 2.3
 * @param[in] navi Parent naviframe
 * @param[in] custom structute to hold tbt application information
 * @param[in] item of the main tbt menu
 * @return Pointer of UI on success, otherwise NULL
 */
ui_view *ui_view_add(Evas_Object *navi, tbt_info *tbt_info, Elm_Object_Item *item);

#endif
#endif // __TBT_UI_VIEW_H__
