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
 * @file	tbt-ui-view.c
 * @brief	This view provide the apps to taste the UI resize, shape, drawable, alpha and font features of the elm APIs
 *
 * @bug		No known bug.
 * @credit	Tizen SDK Version 2.3.0 Sample Example
 *
 */
#include "utils/app_module_config.h"
#ifdef TBT_MODULE_UI

#include <Elementary.h>
#include <system_info.h>

#include "utils/logger.h"
#include "utils/config.h"
#include "utils/ui-utils.h"
#include "view/tbt-ui-view.h"
#include "view/tbt-common-view.h"

struct _ui_view
{
	common_view* view;

    int x;
    int y;

    int alpha;
    int width;
	int height;
	int rotation;

	int x_pad;
	int y_pad;
	int screen_width;
	int screen_height;
	int toolbar_height;

    Ecore_Timer	*timer;
    Evas_Object *ui_element;
    Evas_Object* image_rect;
};

static void set_rotation(ui_view *this);
static void calc_coordinate(ui_view *this);
static void get_screen_resolution(ui_view *this);


static void _app_destroy_cb(void* this);
static Eina_Bool _ui_view_timer_cb(void *data);


/**
 * @function		ui_view_add
 * @since_tizen		2.3
 * @description		Ui View Add
 * @parameter		Evas_Object*: Evas Object Pointer, tbt_info*: Tbt Info Pointer, Elm_Object_Item*: Elm Object Item Pointer
 * @return		ui_view*
 */
ui_view *ui_view_add(Evas_Object *navi, tbt_info *tbt_info, Elm_Object_Item *item)
{
	RETVM_IF(NULL == navi, NULL, "navi is null");
	RETVM_IF(NULL == tbt_info, NULL, "tbt_info is null");
    ui_view *this = NULL;
    this = calloc(1, sizeof(ui_view));
    RETVM_IF(!this, NULL, "calloc failed");

    this->view = calloc(1, sizeof(common_view));
    RETVM_IF(!this->view, NULL, "calloc failed");

    tbt_info->layout_group = "ui_viewer";
	tbt_info->layout_file = get_edje_path("ui_viewer.edj");

	common_view* view = common_view_add(navi, tbt_info, item, this->view, _app_destroy_cb, this);
	RETVM_IF(!view, NULL, "common_view_add failed");

	RETVM_IF(!this, NULL, "calloc failed");
	RETVM_IF(NULL == this->view, NULL, "navi is null");

	this->x = 0;
	this->y = 0;

	this->alpha = 255;
	this->width = 0;
	this->height = 0;
	this->rotation = 0;

	this->x_pad = 50;
	this->y_pad = 50;
	this->toolbar_height = NAVI_TOOLBAR_HEIGHT;

	get_screen_resolution(this);

	this->screen_height = this->screen_height - 2 * this->toolbar_height;

	if(this->view->tbt_info->apptype == TBT_APP_UI_RESIZE)
	{
		calc_coordinate(this);

		this->ui_element = elm_image_add(this->view->layout);
		elm_image_file_set(this->ui_element, get_resource_path(this->view->tbt_info->file_name), NULL);
		elm_image_no_scale_set(this->ui_element, EINA_TRUE);
		elm_image_resizable_set(this->ui_element, EINA_TRUE, EINA_TRUE);
		elm_image_smooth_set(this->ui_element, EINA_FALSE);
		elm_image_aspect_fixed_set(this->ui_element, EINA_FALSE);
		elm_image_editable_set(this->ui_element, EINA_FALSE);
		evas_object_move(this->ui_element, this->x, this->y);
		evas_object_resize(this->ui_element, this->width, this->height);
	}
	else if(this->view->tbt_info->apptype == TBT_APP_UI_SHAPE)
	{
		int r, g, b, a;

		r = 255;
		g = rand() % 100  + 50;
		b = rand() % 100  + 50;
		a = 255;

		calc_coordinate(this);

		Evas *evas = evas_object_evas_get(this->view->layout);
		this->ui_element = evas_object_rectangle_add(evas);

		evas_object_move(this->ui_element, this->x, this->y);
		evas_object_resize(this->ui_element, this->width, this->height);
		evas_object_color_set(this->ui_element, r, g, b, a);
		evas_object_show(this->ui_element);
	}
	else if(this->view->tbt_info->apptype == TBT_APP_UI_ROTATE)
	{
		int r, g, b, a;

		r = 255;
		g = 0;
		b = 0;
		a = 250;

		int offset;

		#ifdef DEVICE_TYPE_MOBILE
			offset = 100;
		#else
			offset = 50;
		#endif
		this->x = this->screen_width / 2 - offset;
		this->y = this->screen_height / 2 - offset + this->toolbar_height;

		Evas *evas = evas_object_evas_get(this->view->layout);
		this->ui_element = evas_object_rectangle_add(evas);

		evas_object_move(this->ui_element, this->x, this->y);
		evas_object_resize(this->ui_element, 2*offset, 2*offset);
		evas_object_color_set(this->ui_element, r, g, b, a);
		evas_object_show(this->ui_element);
	}
	else if(this->view->tbt_info->apptype == TBT_APP_UI_FONT)
	{
		this->x = this->x_pad;
		this->y = this->toolbar_height;

		Evas *evas = evas_object_evas_get(this->view->layout);
		this->ui_element = evas_object_text_add(evas);

		evas_object_text_style_set(this->ui_element, EVAS_TEXT_STYLE_PLAIN);
		evas_object_color_set(this->ui_element, 255,0,0,255);
		evas_object_text_glow_color_set(this->ui_element, 0,255,0,255);
		evas_object_text_glow2_color_set(this->ui_element, 0,0,255,255);
		evas_object_text_outline_color_set(this->ui_element, 255,255,255,255);
		evas_object_text_shadow_color_set(this->ui_element, 255,255,0,255);
		evas_object_text_font_set(this->ui_element, "Courier", 30);
		evas_object_text_text_set(this->ui_element, "Sample Text ! ");
		evas_object_move(this->ui_element,this->x, this->y);
		evas_object_resize(this->ui_element,  700, 500);
		evas_object_show(this->ui_element);
	}
	else if(this->view->tbt_info->apptype == TBT_APP_UI_LINE)
	{
		int r, g, b, a;

		r = 255;
		g = rand() % 100  + 50;
		b = rand() % 100  + 50;
		a = 255;

		calc_coordinate(this);

		Evas *evas = evas_object_evas_get(this->view->layout);
		this->ui_element = evas_object_line_add(evas);

		evas_object_color_set(this->ui_element, r, g, b, a);
		evas_object_line_xy_set(this->ui_element, this->x, this->y, (this->x + this->width), (this->y + this->height));
		evas_object_show(this->ui_element);
	}
	else if(this->view->tbt_info->apptype == TBT_APP_UI_ALPHA)
	{
		int r, g, b;
		r = 255;
		g = 0;
		b = 0;
		this->alpha = 100;

		int offset;

		#ifdef DEVICE_TYPE_MOBILE
			offset = 100;
		#else
			offset = 50;
		#endif

		this->x = this->screen_width / 2 - offset;
		this->y = this->screen_height / 2 - offset + this->toolbar_height;

		Evas *evas = evas_object_evas_get(this->view->layout);
		this->ui_element = evas_object_rectangle_add(evas);

		evas_object_move(this->ui_element, this->x, this->y);
		evas_object_resize(this->ui_element, 2*offset, 2*offset);
		evas_object_color_set(this->ui_element, r, g, b, this->alpha);
		evas_object_show(this->ui_element);
	}

    this->timer = ecore_timer_add(1, _ui_view_timer_cb, this);

    return this;
}


/**
 * @function		calc_coordinate
 * @since_tizen		2.3
 * @description		Calc Coordinate
 * @parameter		ui_view*: Ui View Pointer
 * @return		static void
 */
static void calc_coordinate(ui_view *this)
{
	RETM_IF(NULL == this, "data is NULL");

	this->x = rand() % (this->screen_width / 2 - this->x_pad) + this->x_pad;
	this->y = rand() % (this->screen_height / 2 - this->y_pad) + this->y_pad;

	this->width = rand() % (this->screen_width - this->x - this->x_pad);
	this->height = rand() % (this->screen_height - this->y - this->y_pad);

	this->y +=  this->toolbar_height;
}


/**
 * @function		set_rotation
 * @since_tizen		2.3
 * @description		Set Rotation
 * @parameter		ui_view*: Ui View Pointer
 * @return		static void
 */
static void set_rotation(ui_view *this)
{
	int x,y,w,h;

	Evas_Object *o;
	o = this->ui_element;

	if(this->rotation < 0) this->rotation=-this->rotation;
	if(this->rotation>=360) this->rotation=this->rotation%360;

	evas_object_geometry_get(o, &x, &y, &w, &h);
	Evas_Map *m = evas_map_new(4);
	evas_map_util_points_populate_from_object(m, o);
	evas_map_util_rotate(m, this->rotation, x + (w / 2), y + (h / 2));
	evas_object_map_set(o, m);
	evas_object_map_enable_set(o, EINA_TRUE);
	evas_map_free(m);
}


/**
 * @function		get_screen_resolution
 * @since_tizen		2.3
 * @description		Get Screen Resolution
 * @parameter		ui_view*: Ui View Pointer
 * @return		static void
 */
static void get_screen_resolution(ui_view *this)
{
	int result;
	result = system_info_get_platform_int("tizen.org/feature/screen.width", &this->screen_width);
	RETM_IF(result != 0, "tizen.org/feature/screen.width");
	RETM_IF(this->width < 0, "width cannot be less than zero");

	result = system_info_get_platform_int("tizen.org/feature/screen.height", &this->screen_height);
	RETM_IF(result != 0, "tizen.org/feature/screen.height");
	RETM_IF(this->height < 0, "height cannot be less than zero");
}


/**
 * @function		_ui_view_timer_cb
 * @since_tizen		2.3
 * @description		 Ui View Timer Cb
 * @parameter		void*: Void Pointer
 * @return		static Eina_Bool
 */
static Eina_Bool _ui_view_timer_cb(void *data)
{
	RETVM_IF(NULL == data, false,"data is NULL");

	ui_view *this = NULL;
	this = (ui_view*)data;
	RETVM_IF(NULL == this, false,"view is NULL");

	if(this->view->tbt_info->apptype == TBT_APP_UI_RESIZE)
	{
		calc_coordinate(this);

		evas_object_move(this->ui_element, this->x, this->y);
		evas_object_resize(this->ui_element, this->width, this->height);
		evas_object_show(this->ui_element);
	}
	else if(this->view->tbt_info->apptype == TBT_APP_UI_SHAPE)
	{
		int r, g, b, a;

		r = 255;
		g = rand() % 100  + 50;
		b = rand() % 100  + 50;
		a = 255;

		calc_coordinate(this);

		evas_object_move(this->ui_element, this->x, this->y);
		evas_object_resize(this->ui_element, this->width, this->height);
		evas_object_color_set(this->ui_element, r, g, b, a);
		evas_object_show(this->ui_element);
	}
	else if(this->view->tbt_info->apptype == TBT_APP_UI_ROTATE)
	{
		set_rotation(this);
		this->rotation += 36;
	}
	else if(this->view->tbt_info->apptype == TBT_APP_UI_FONT)
	{
		evas_object_color_set(this->ui_element, rand()%256,rand()%256,rand()%256, 255);
		evas_object_text_glow_color_set(this->ui_element, rand()%256, rand()%256, rand()%256, 255);
		evas_object_text_glow2_color_set(this->ui_element, rand()%256, rand()%256, rand()%256, 255);

		// Set Outline color always white!
		evas_object_text_outline_color_set(this->ui_element, 255,255,255,255);

		// Shadow color always yellow!svn
		evas_object_text_shadow_color_set(this->ui_element, 255,255,0,255);
		evas_object_text_font_set(this->ui_element, "Courier", rand() % 60 + 10);
		evas_object_text_text_set(this->ui_element, "Sample Text ! ");

		evas_object_show(this->ui_element);
	}
	else if(this->view->tbt_info->apptype == TBT_APP_UI_LINE)
	{
		int r, g, b, a;

		r = 255;
		g = rand() % 100  + 50;
		b = rand() % 100  + 50;
		a = 255;

		calc_coordinate(this);

		evas_object_line_xy_set(this->ui_element, this->x, this->y, (this->x + this->width), (this->y + this->height));
		evas_object_color_set(this->ui_element, r, g, b, a);
		evas_object_show(this->ui_element);
	}
	else if(this->view->tbt_info->apptype == TBT_APP_UI_ALPHA)
	{
		this->alpha = (this->alpha > 255) ? 0 : this->alpha + 20;
		evas_object_color_set(this->ui_element, 255, 0, 0, this->alpha);
	}


	return true;
}


/**
 * @function		_app_destroy_cb
 * @since_tizen		2.3
 * @description		 App Destroy Cb
 * @parameter		void*: Void Pointer
 * @return		static void
 */
static void _app_destroy_cb(void* this)
{
	RETM_IF(NULL == this, "data is NULL");

	ui_view *view = NULL;
	view = (ui_view*)this;
	RETM_IF(NULL == view, "view is NULL");

	ecore_timer_del(view->timer);
	evas_object_del(view->ui_element);

	SAFE_DELETE(view->view);
	SAFE_DELETE(view);
}

#endif
