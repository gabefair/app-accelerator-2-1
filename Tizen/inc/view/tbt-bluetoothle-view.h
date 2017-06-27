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
 * @file	tbt-bluetoothle-view.h
 * @brief	covers location retrieval
 *			starts location service using bluetoothle method and continuously updates the current location
 * @since_tizen 2.3
 * @bug		location manager does not start in device
 * @credit
 *
 */

#ifndef __TBT_BLE_VIEW_H__
#define __TBT_BLE_VIEW_H__

#include "utils/app_module_config.h"
#ifdef TBT_MODULE_BLE

#include <Evas.h>
#include "model/tbt-list.h"


/**
 * @typedef bluetoothle_viewer_view
 * @since_tizen 2.3
 * @brief A bluetoothle viewer view handle.
 */
typedef struct _bluetoothle_view bluetoothle_view;

/**
 * @brief Create bluetoothle view
 * @since_tizen 2.3 
 * @param[in] navi Parent naviframe
 * @param[in] custom structute to hold tbt application information
 * @param[in] item of the main tbt menu
 * @return Pointer of bluetoothle on success, otherwise NULL
 */
bluetoothle_view *bluetoothle_view_add(Evas_Object *navi, tbt_info *tbt_info, Elm_Object_Item *item);

#endif
#endif // __TBT_BLE_VIEW_H__
