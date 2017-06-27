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


#include "window.h"
#include "utils/config.h"
#include "utils/logger.h"
#include "utils/ui-utils.h"

#include <Elementary.h>

#define APP_NAME "tbtcoreapp"

struct _window
{
	Evas_Object *win;
	Evas_Object *bg;
	Evas_Object *layout;
	Evas_Object *conform;
};


/**
 * @function		win_get_win_handle
 * @since_tizen		2.3
 * @description		Win Get Win Handle
 * @parameter		const window*: Const window Pointer
 * @return		Evas_Object*
 */
Evas_Object* win_get_win_handle(const window* win)
{
	RETVM_IF(NULL == win, NULL, "win_get_win_handle win is null");
	return win->win;
}


/**
 * @function		win_create
 * @since_tizen		2.3
 * @description		Win Create
 * @parameter		NA
 * @return		window*
 */
window *win_create()
{
    window *win = calloc(1, sizeof(window));
    RETVM_IF(NULL == win, NULL, "window is null");

	#ifdef TIZEN_2_3_1
		elm_config_preferred_engine_set(WINDOW_PREFERED_ENGINE);
	#else
		elm_config_accel_preference_set(WINDOW_PREFERED_ENGINE);
	#endif

	win->win = elm_win_add(NULL, APP_NAME, ELM_WIN_BASIC);
	elm_win_indicator_mode_set(win->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_conformant_set(win->win, EINA_TRUE);
	evas_object_show(win->win);


    win->conform = elm_conformant_add(win->win);
    evas_object_size_hint_weight_set(win->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(win->win, win->conform);
    evas_object_show(win->conform);

    win->bg = elm_bg_add(win->conform);
    elm_object_part_content_set(win->conform, PART_SWALLOW_BG, win->bg);

    return win;
}


/**
 * @function		win_destroy
 * @since_tizen		2.3
 * @description		Win Destroy
 * @parameter		window*: Window Pointer
 * @return		void
 */
void win_destroy(window *win)
{
    RETM_IF(NULL == win, "win is null");
    evas_object_del(win->win);
    free(win);
}


/**
 * @function		win_lower
 * @since_tizen		2.3
 * @description		Win Lower
 * @parameter		window*: Window Pointer
 * @return		void
 */
void win_lower(window *win)
{
    RETM_IF(NULL == win, "win is null");
    elm_win_lower(win->win);
}


/**
 * @function		win_set_layout
 * @since_tizen		2.3
 * @description		Win Set Layout
 * @parameter		window*: Window Pointer, Evas_Object*: Evas Object Pointer
 * @return		void
 */
void win_set_layout(window *win, Evas_Object *layout)
{
    RETM_IF(NULL == win, "win is null");
    RETM_IF(NULL == layout, "layout is null");
    elm_object_part_content_set(win->conform, PART_SWALLOW_CONTENT, layout);
    win->layout = layout;
}


/**
 * @function		win_get_host_layout
 * @since_tizen		2.3
 * @description		Win Get Host Layout
 * @parameter		const window*: Const window Pointer
 * @return		Evas_Object*
 */
Evas_Object *win_get_host_layout(const window *win)
{
    RETVM_IF(NULL == win, NULL, "win is null");
    return win->conform;
}
