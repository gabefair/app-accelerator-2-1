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
 * @since_tizen 2.3
 * @brief   creates the main genlist
 * @bug		no known bug
 * @credit
 *
 */

#ifndef __TBT_LIST_VIEW_H__
#define __TBT_LIST_VIEW_H__

#include <Evas.h>

/**
 * @brief Create tbt list view
 * @since_tizen 2.3 
 * @param[in]   navi    Parent naviframe
 * @return Main layout on success, otherwise NULL
 */
Evas_Object *tbt_list_view_create(Evas_Object *navi);

#endif /* __TBT_LIST_VIEW_H__ */
