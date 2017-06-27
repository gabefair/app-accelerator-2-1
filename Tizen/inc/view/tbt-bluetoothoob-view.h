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
 * @file	tbt-bluetoothoob-view.h
 * @brief	Bluetooth OOB
 *
 * @credit
 *
 */
#ifndef __TBT_BLUETOOTH_OOB_VIEW_H__
#define __TBT_BLUETOOTH_OOB_VIEW_H__

#include "utils/app_module_config.h"
#ifdef TBT_MODULE_BT_OOB

#include <Evas.h>
#include "model/tbt-list.h"
#include <nfc.h>
#include <bluetooth.h>


/**
* @typedef bluetoothoob_viewer_view
*
* @brief A bluetoothoob_view handle.
*/
typedef struct _bluetoothoob_view bluetoothoob_view;

/**
* @brief Create bluetooth oob
* @param[in] navi Parent naviframe
* @param[in] custom structute to hold tbt application information
* @param[in] item of the main tbt menu
* @return Pointer of bluetoothoob_view on success, otherwise NULL
*/

bluetoothoob_view *bluetoothoob_view_add(Evas_Object *navi, tbt_info *tbt_info, Elm_Object_Item *item); // module's startup function

#endif
#endif // __TBT_BLUETOOTH_OOB_VIEW_H__
