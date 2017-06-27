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
 * @file	tbt-list.h
 * @brief	This file Handles main List of Application.
 * @since_tizen 2.3
 * @bug
 * @credit
 */

#ifndef __TBT_LIST_H__
#define __TBT_LIST_H__

#include "tbt-info.h"

/**
 * @brief Initialize sensor info list
 * @since_tizen 2.3
 */
void tbt_list_init();

/**
 * Save xml report file for future use
 * @since_tizen 2.3
 */
void tbt_save_report();

/**
 * @brief Get list of tbt list supported by application
 * @since_tizen 2.3
 * @param[out]  count  tbts list count
 * @return Sensor list
 */
const tbt_info *tbt_list_get(unsigned *count);

/**
 * Returns total count of tbt_info;
 * @since_tizen 2.3
 */
const int tbt_info_count();

/**
 * @brief Get list of tbt list supported by application
 * @since_tizen 2.3
 * @param[out]  count  tbts list count
 * @return Sensor list
 */
tbt_info *tbt_list_item_get(int index);

/**
 * @brief Get list item index of tbt list supported by application
 * @since_tizen 2.3
 * @param[out]  count  tbts list count
 * @return Sensor list
 */
const int tbt_list_item_index_get_by_app_type(int app_type);
/**
 * @brief Get list item count tbt list supported by application
 * @since_tizen 2.3
 * @param[out]  count  tbts list count
 * @return Sensor list
 */
const int tbt_list_item_count(int index);

#endif /* __TBT_LIST_H__ */
