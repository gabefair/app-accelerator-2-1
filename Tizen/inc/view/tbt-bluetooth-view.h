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
 * @file	tbt-bluetooth-view.h
 * @brief	performs all the bluetooth related operations
 *			performs operations related to bluetooth adapter, bonding and pairing
 * @since_tizen 2.3
 * @bug		1.bluetooth starting application does not work
 * @credit
 */

#ifndef __TBT_BLUETOOTH_VIEW_H__
#define __TBT_BLUETOOTH_VIEW_H__

#include "utils/app_module_config.h"
#ifdef TBT_MODULE_BT

#include <Evas.h>
#include "model/tbt-list.h"


/**
 * @typedef bluetooth_view
 * @since_tizen 2.3
 * @brief A bluetooth view handle.
 */
typedef struct _bluetooth_view bluetooth_view;

/**
 * @brief Create tbt bluetooth view
 * @since_tizen 2.3 
 * @param[in] navi Parent naviframe
 * @param[in] custom structute to hold tbt application information
 * @param[in] item of the main tbt menu
 * @return Pointer of bluetooth on success, otherwise NULL
 */
bluetooth_view *bluetooth_view_add(Evas_Object *navi, tbt_info *tbt_info, Elm_Object_Item *item);

#endif
#endif // __TBT_BLUETOOTH_VIEW_H__
