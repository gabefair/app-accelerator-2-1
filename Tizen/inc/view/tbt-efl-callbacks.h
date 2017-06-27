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
 * @file	tbt-efl-callbacks.h
 * @brief	Checks eext_object_event_callback_del() and eext_object_event_callback_add() functionality.
 * @since_tizen 2.3
 * @bug		NA
 * @credit
 *
 */

#ifndef __TBT_EFL_CALLBACKS_H__
#define __TBT_EFL_CALLBACKS_H__

#include "utils/app_module_config.h"
#ifdef TBT_MODULE_EFL

#include <Evas.h>
#include "model/tbt-list.h"


/**
 * @typedef efl_callbacks_view
 * @since_tizen 2.3
 */
typedef struct _efl_callbacks_view efl_callbacks_view;

/**
 * @brief Create EFL test view
 * @since_tizen 2.3 
 * @param[in] navi Parent naviframe
 * @param[in] custom structute to hold tbt application information
 * @param[in] item of the main tbt menu
 * @return Pointer of callback on success, otherwise NULL
 */
efl_callbacks_view *efl_callbacks_view_add(Evas_Object *navi, tbt_info *tbt_info, Elm_Object_Item *item);

#endif
#endif // __TBT_LOCAL_VIEW_H__
