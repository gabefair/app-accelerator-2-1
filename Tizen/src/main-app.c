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


#include "main-app.h"
#include "window.h"
#include "utils/logger.h"
#include "utils/ui-utils.h"
#include "utils/config.h"
#include "view/tbt-list-view.h"
#include "view/tbt-genlist-view.h"
#include "model/tbt-list.h"

#include "model/tbt-list.h"

#include <app.h>
#include <Elementary.h>
#include <efl_extension.h>
#include <storage.h>
#include <badge.h>


struct _app_data
{
	window *win;
	Evas_Object *navi;
	Evas_Object* list;
	Evas_Object* genlist;

};


static bool _app_create_cb(void *user_data);
static void _app_pause_cb(void *user_data);
static void _app_resume_cb(void *user_data);
static void _app_terminate_cb(void *user_data);
static void _app_control(app_control_h app_control, void *user_data);
static void _app_navi_back_cb(void *data, Evas_Object *obj, void *event_info);


/**
 * @function		get_storage_error
 * @since_tizen		2.3
 * @description		Get Storage Error
 * @parameter		int: Int
 * @return		char*
 */
char* get_storage_error(int ret)
{

switch(ret)
{
	case STORAGE_ERROR_NONE:
		return "STORAGE_ERROR_NONE";
	case STORAGE_ERROR_INVALID_PARAMETER:
		return "STORAGE_ERROR_INVALID_PARAMETER";
	case STORAGE_ERROR_OUT_OF_MEMORY:
		return "STORAGE_ERROR_OUT_OF_MEMORY";
	case STORAGE_ERROR_NOT_SUPPORTED:
		return "STORAGE_ERROR_NOT_SUPPORTED";
	case STORAGE_ERROR_OPERATION_FAILED:
		return "STORAGE_ERROR_OPERATION_FAILED";
	}
	return NULL;
}


/**
 * @function		app_create
 * @since_tizen		2.3
 * @description		App Create
 * @parameter		NA
 * @return		app_data*
 */
app_data *app_create()
{
	app_data *ad = NULL;
	ad = calloc(1, sizeof(app_data));
    RETVM_IF(!ad, NULL, "ad in app_create is NULL");


    // lets set storage devices
    int ret = storage_foreach_device_supported(_storage_device_supported_cb,NULL);
    RETVM_IF(ret<0, NULL,"storage_foreach_device_supported failed : %s",get_storage_error(ret));

    ret = storage_get_directory(_supportedStorageID,STORAGE_DIRECTORY_OTHERS,&TBT_LOGGING_DIR);
    RETVM_IF(ret<0, NULL,"storage_foreach_device_supported failed : %s",get_storage_error(ret));

    ret = storage_get_directory(_supportedStorageID,STORAGE_DIRECTORY_VIDEOS, &TBT_MEDIA_DIR);
    RETVM_IF(ret<0, NULL,"storage_foreach_device_supported failed : %s",get_storage_error(ret));

    DBG("Logging_Dir set = %s",TBT_LOGGING_DIR);
    snprintf(XML_REPORT_FILE_PATH, sizeof(XML_REPORT_FILE_PATH), "%s/%s",TBT_LOGGING_DIR,"tbt-report.xml");
    DBG("Reporting file set = %s",XML_REPORT_FILE_PATH);

	return ad;
}


/**
 * @function		app_destroy
 * @since_tizen		2.3
 * @description		App Destroy
 * @parameter		app_data*: App Data Pointer
 * @return		void
 */
void app_destroy(app_data *ad)
{
	if(ad)
	{
		SAFE_DELETE(ad);
	}
}


/**
 * @function		app_run
 * @since_tizen		2.3
 * @description		App Run
 * @parameter		app_data*: App Data Pointer
 * @return		int
 */
int app_run(app_data *app, int argc, char **argv)
{
	RETVM_IF(!app, -1, "app is NULL");
	_tbt_pause_resume_cb = NULL;

	_tbt_app_terminate_cb = NULL;
	app_error_e result;
	ui_app_lifecycle_callback_s cbs =
    {
        .create = _app_create_cb,
        .terminate = _app_terminate_cb,
        .app_control = _app_control,
        .pause = _app_pause_cb,
        .resume = _app_resume_cb

    };

	result = ui_app_main(argc, argv, &cbs, app);
	RETVM_IF(result != APP_ERROR_NONE, result, "app_main fail > Error = %d", result);

	return result;
}


/**
 * @function		app_navi_add
 * @since_tizen		2.3
 * @description		App Navi Add
 * @parameter		app_data*: App Data Pointer
 * @return		static Evas_Object*
 */
static Evas_Object *app_navi_add(app_data *ad)
{
    Evas_Object *navi = NULL;
    Evas_Object *parent = NULL;

    parent = win_get_host_layout(ad->win);
    RETVM_IF(!parent, NULL, "app_navi_add failed");
	navi = ui_utils_navi_add(parent, _app_navi_back_cb, ad);
	RETVM_IF(!navi, NULL, "ui_utils_navi_add failed");
	win_set_layout(ad->win, navi);

    return navi;
}


/**
 * @function		_app_pause_cb
 * @since_tizen		2.3
 * @description		 App Pause Cb
 * @parameter		void*: Void Pointer
 * @return		static void
 */
static void _app_pause_cb(void *user_data)
{
	DBG("@#@ INside main pause cb");
    if(NULL != _tbt_pause_resume_cb)
    {

    	(*_tbt_pause_resume_cb)(false);
    }
}


/**
 * @function		_app_resume_cb
 * @since_tizen		2.3
 * @description		 App Resume Cb
 * @parameter		void*: Void Pointer
 * @return		static void
 */
static void _app_resume_cb(void *user_data)
{
	DBG("INside main resume cb");
    if(NULL != _tbt_pause_resume_cb)
    {
    	(*_tbt_pause_resume_cb)(true);
    }
}


/**
 * @function		_app_create_cb
 * @since_tizen		2.3
 * @description		 App Create Cb
 * @parameter		void*: Void Pointer
 * @return		static bool
 */
static bool _app_create_cb(void *user_data)
{


	app_data *ad = user_data;
	RETVM_IF(NULL == ad, false, "ad is null");

	ad->win = win_create();
	RETVM_IF(NULL == ad->win, false, "ad->win is null");

	ad->navi = app_navi_add(ad);
	RETVM_IF(NULL == ad->navi, false, "ad->navi is null");


	#ifdef DEVICE_TYPE_WEARABLE
		if(get_device_type() == DEVICE_WEARABLE_CIRCLE)
		{
			elm_theme_overlay_add(NULL, get_edje_path("custom_button.edj"));
		}
	#endif


	ad->genlist = tbt_genlist_view_create(ad->navi);
	RETVM_IF(NULL == ad->genlist, false, "getlist creation is failed");

	return true;
}


/**
 * @function		_app_terminate_cb
 * @since_tizen		2.3
 * @description		 App Terminate Cb
 * @parameter		void*: Void Pointer
 * @return		static void
 */
static void _app_terminate_cb(void *user_data)
{
	app_data *ad = user_data;
	RETM_IF(NULL == ad, "app is null");
	if(_tbt_app_terminate_cb != NULL)
	{
		(*_tbt_app_terminate_cb)();
	}

	#ifdef DEVICE_TYPE_WEARABLE
		if(get_device_type() == DEVICE_WEARABLE_CIRCLE)
		{
			elm_theme_overlay_del(NULL, get_edje_path("custom_button.edj"));
		}
	#endif
	win_destroy(ad->win);
	ad->win = NULL;
}


/**
 * @function		_app_control
 * @since_tizen		2.3
 * @description		 App Control
 * @parameter		app_control_h: App Control H, void*: Void Pointer
 * @return		static void
 */
static void _app_control(app_control_h app_control, void *user_data)
{

	app_data *ad = user_data;
	RETM_IF(NULL == ad, "app is null");


	int result;
	char *operation=NULL;
	result = app_control_get_operation (app_control, &operation);
	RETM_IF(result !=APP_CONTROL_ERROR_NONE, "app_control_get_operation failed! %s",get_error_message(result));
	DBG("Operation = %s",operation);

	char *uri=NULL;
	result = app_control_get_uri (app_control, &uri);
	RETM_IF(result !=APP_CONTROL_ERROR_NONE, "app_control_get_uri failed! %s", get_error_message(result));
	if(uri==NULL)DBG("URI = NULL");
	else DBG("URI = %s",uri);


	if(strcmp(operation, "libpush.operation.handle") == 0)
	{
	}
	else if(uri!=NULL)
	{
		char msg[512];

		ui_utils_show_toast(msg, win_get_win_handle(ad->win), 3);
	}
	else
	{
		//bellow line of codes give a pop/toast in the screen showing in what operaration TBT launching, but in 2.3 these lines causes launching problem
//		sprintf(msg,"Operation:'%s' and URI:'%s'",operation,uri);
//		char msg[512];
//		sprintf(msg,"Operation:'%s'",operation);
//		ui_utils_show_toast(msg, win_get_win_handle(ad->win), 3);
	}



	SAFE_DELETE(operation);
	SAFE_DELETE(uri);
}


/**
 * @function		_app_navi_back_cb
 * @since_tizen		2.3
 * @description		 App Navi Back Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _app_navi_back_cb(void *data, Evas_Object *obj, void *event_info)
{
    app_data *ad = (app_data*)data;
    RETM_IF(!data, "ad in _app_navi_back_cb is NULL");

    if(elm_naviframe_top_item_get(obj) == elm_naviframe_bottom_item_get(obj))
    {
        if(ad)
        {
        	tbt_save_report();


            win_lower(ad->win);
        }
    }
    else
    {
    	if(__is_efl_test_running==false)
    		elm_naviframe_item_pop(obj);
    	else DBG("EFL test running. Main back function disabled");
    }
}
