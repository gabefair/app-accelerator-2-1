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
 * @file	window.h
 * @brief
 * @since_tizen 2.3
 * @bug		No known bug.
 * @credit	Tizen SDK Version 2.3.0 Sample Example
 *
 */

#ifndef __WINDOW_H__
#define __WINDOW_H__

#include <Evas.h>

typedef struct _window window;

/*
 * @brief Create application main window
 * @since_tizen 2.3
 * @return Window on success, otherwise NULL
 */
window *win_create();

/*
 * @brief return win handle
 * @since_tizen 2.3
 * @return Evas_Object* on success, otherwise NULL
 */
Evas_Object* win_get_win_handle(const window* win);

/*
 * @brief Lower application window to hide application without exiting
 * @since_tizen 2.3
 * @param[in]   obj     Application window
 */
void win_lower(window *obj);

/*
 * @brief Destroy application main window
 * @since_tizen 2.3
 * @param[in]   obj     Application window
 */

void win_destroy(window *obj);

/*
 * @brief Set content to be displayed in window
 * @since_tizen 2.3
 * @param[in]   win     Application window
 * @param[in]   content Window content
 */
void win_set_layout(window *win, Evas_Object *layout);

/*
 * @brief Get window layout to use as a parent for window content
 * @since_tizen 2.3
 * @param[in]   win     Application window
 * @return Window layout
 */
Evas_Object *win_get_host_layout(const window *win);

#endif /* __WINDOW_H__ */
