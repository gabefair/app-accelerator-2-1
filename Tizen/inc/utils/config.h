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
 * @file	config.h
 * @brief	This file Configures the Application.
 * @since_tizen 2.3
 * @bug
 * @credit
 */
#ifndef CONFIG_H_
#define CONFIG_H_

#include "utils/app_module_config.h"
#include <app.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "utils/logger.h"
#include <system_info.h>
#include <storage.h>

#define MAX_LENGTH 1024

#define EDJPATH "edje"
#define AUDIOPATH "sounds"
#define VIDEOPATH "video"
#define IMAGEPATH "images"

extern bool __is_efl_test_running;

//Push:
#define PUSH_APP_ID "da58f09b7dcd898f"
#define PUSH_APP_SECRET "E55A746761233098DDD1114F652AFE51"


// *_X_POS, *_Y_POS, *_WIDTH, *_HEIGHT are virtual (relative) coords of grid widget
#define MAX_WIDTH 100
#define MAX_HEIGHT 100

#define IMAGE_X_POS 0
#define IMAGE_Y_POS 0
#define IMAGE_WIDTH MAX_WIDTH
#define IMAGE_HEIGHT 40

#define LABEL_X_POS 0
#define LABEL_Y_POS IMAGE_HEIGHT
#define LABEL_WIDTH MAX_WIDTH
#define LABEL_HEIGHT MAX_WIDTH-IMAGE_HEIGHT

#define LABEL_MAX_LEN 500

#define IMAGES_REL_FOLDER "images"
#define IMAGE_REL_PATH_TEMPLATE "%s/%s"

#define TOOLBAR_BUTTON_DATA_KEY "button_id"

#define LABEL_REL_PATH "res/"IMAGES_REL_FOLDER"/"
#define LABEL_TEMPLATE "%s%s<br/>Dimension = %u x %u<br/>Result:E_SUCCESS"

#define TABLE_PADDING_VERTICAL 20
#define TABLE_PADDING_HORIZONTAL 20

#define NAVI_TOOLBAR_HEIGHT	150

#ifdef TIZEN_2_3_1
	#define WINDOW_PREFERED_ENGINE "opengl_x11"
#else
	#define WINDOW_PREFERED_ENGINE "3d"
#endif


#define PICK_VIEW_APP_PACKAGE_NAME "org.tizen.coretbt_uiapp";
#define PICK_VIEW_SERVICEAPP_PACKAGE_NAME "org.tizen.coretbt_serviceapp";

#define WIDGET_VIEW_APPID "org.tizen.coretbtwidgetapp";
#define VIEWER_VIEW_APP_PACKAGE_NAME "org.tizen.widget_viewer_sdk";

#define DATA_CONTROL_PROVIDER_MAP_ID "MapRepository";
#define DATA_CONTROL_PROVIDER_DATA_ID "provider";

#define DATA_CONTROL_PROVIDER_ID "http://coretbt_dataprovider.com/datacontrol/provider/coretbt_dataprovider";
#define DATA_CONTROL_MAP_PROVIDER_ID "http://coretbt_dataprovider.com/datacontrol/provider/coretbt_dataprovider_map";


#define BLUETOOTH_SOCKET_UUID "00001101-0000-1000-8000-00805F9B34FB"

#define DEFINED_WIDTH 480
#define DEFINED_HEIGHT 800

char *TBT_LOGGING_DIR;
char *TBT_MEDIA_DIR;
char XML_REPORT_FILE_PATH[128];
char MEDIA_FILE_PATH[128];


static inline const char *get_data_path(const char * file_path);
static inline const char *get_resource_path(const char * file_path);
static inline const char *get_shared_resource_path(const char *file_path);


typedef enum {
	 DEVICE_WEARABLE,
	 DEVICE_MOBILE,
	 DEVICE_WEARABLE_360_X_480,
	 DEVICE_WEARABLE_320_X_320,
	 DEVICE_WEARABLE_CIRCLE,

	 DEVICE_NONE,
	 DEVICE_MAX
}device_type_e;

static inline device_type_e get_device_type();

typedef enum
{
	CURSOR_NONE_PRESSED = -1,
	CURSOR_START_PRESSED ,
	CURSOR_FIRST_PRESSED ,
	CURSOR_NEXT_PRESSED ,
	CURSOR_PREVIOUS_PRESSED ,
	CURSOR_LAST_PRESSED
}_Data_Control_Cursor_State;

static int _supportedStorageID;
static bool _storage_device_supported_cb(int storage_id, storage_type_e type, storage_state_e state, const char *path, void *user_data)
{
	if(type == STORAGE_TYPE_INTERNAL)
	{
		// save storage id
		_supportedStorageID=storage_id;
		return false;
	}
	return true;
}

static void
app_get_resource(const char *edj_file_in, char *edj_path_out, int edj_path_max)
{
	char *res_path = app_get_resource_path();
	if (res_path) {
		snprintf(edj_path_out, edj_path_max, "%s%s", res_path, edj_file_in);
	}
}

static void
app_get_shared_resource(const char *edj_file_in, char *edj_path_out, int edj_path_max)
{
	char *res_path = app_get_shared_resource_path();
	if (res_path) {
		snprintf(edj_path_out, edj_path_max, "%s%s", res_path, edj_file_in);
	}
}

static inline const char *get_edje_path(const char *file_name)
{
char absolute_path[MAX_LENGTH] = {'\0'};
char file_path[MAX_LENGTH] = {'\0'};
#ifdef TIZEN_2_3_1
	#ifdef DEVICE_TYPE_MOBILE
		strncat(file_path, "edje/2.3.1/mobile/", sizeof("edje/2.3.1/mobile/"));
	#endif
	#ifdef DEVICE_TYPE_WEARABLE
		if(get_device_type() == DEVICE_WEARABLE)
		{
			strncat(file_path, "edje/2.3.1/wearable/", sizeof("edje/2.3.1/wearable/"));
		}
		else if(get_device_type() == DEVICE_WEARABLE_360_X_480)
		{
			strncat(file_path, "edje/2.3.1/wearable_360_x_480/", sizeof("edje/2.3.1/wearable_360_x_480/"));
		}
		else if(get_device_type() == DEVICE_WEARABLE_320_X_320)
		{
			strncat(file_path, "edje/2.3.1/wearable_320_x_320/", sizeof("edje/2.3.1/wearable_320_x_320/"));
		}
		else if(get_device_type() == DEVICE_WEARABLE_CIRCLE)
		{
			strncat(file_path, "edje/2.3.1/wearable_circle/", sizeof("edje/2.3.1/wearable_circle/"));
		}
	#endif
#endif



#ifdef TIZEN_2_4
	#ifdef DEVICE_TYPE_MOBILE
		strncat(file_path, "edje/2.4/mobile/", sizeof("edje/2.4/mobile/"));
	#endif
	#ifdef  DEVICE_TYPE_WEARABLE
		if(get_device_type() == DEVICE_WEARABLE)
		{
			strncat(file_path, "edje/2.4/wearable/", sizeof("edje/2.4/wearable/"));
		}
		else if(get_device_type() == DEVICE_WEARABLE_360_X_480)
		{
			strncat(file_path, "edje/2.4/wearable_360_x_480/", sizeof("edje/2.4/wearable_360_x_480/"));
		}
		else if(get_device_type() == DEVICE_WEARABLE_320_X_320)
		{
			strncat(file_path, "edje/2.4/wearable_320_x_320/", sizeof("edje/2.4/wearable_320_x_320/"));
		}
		else if(get_device_type() == DEVICE_WEARABLE_CIRCLE)
		{
			strncat(file_path, "edje/2.4/wearable_circle/", sizeof("edje/2.4/wearable_circle/"));
		}
	#endif
#endif



#ifdef TIZEN_3_0
	#ifdef DEVICE_TYPE_MOBILE
		strncat(file_path, "edje/3.0/mobile/", sizeof("edje/3.0/mobile/"));
	#endif
	#ifdef  DEVICE_TYPE_WEARABLE
		if(get_device_type() == DEVICE_WEARABLE)
		{
			strncat(file_path, "edje/3.0/wearable/", sizeof("edje/3.0/wearable/"));
		}
		else if(get_device_type() == DEVICE_WEARABLE_360_X_480)
		{
			strncat(file_path, "edje/3.0/wearable_360_x_480/", sizeof("edje/3.0/wearable_360_x_480/"));
		}
		else if(get_device_type() == DEVICE_WEARABLE_320_X_320)
		{
			strncat(file_path, "edje/3.0/wearable_320_x_320/", sizeof("edje/3.0/wearable_320_x_320/"));
		}
		else if(get_device_type() == DEVICE_WEARABLE_CIRCLE)
		{
			strncat(file_path, "edje/3.0/wearable_circle/", sizeof("edje/3.0/wearable_circle/"));
		}
	#endif
#endif

	int num = strlen(file_name);
	if(strlen(file_path)+strlen(file_name) <= MAX_LENGTH)
	{
		num = strlen(file_name);
	}
	else
	{
		num = MAX_LENGTH-strlen(file_path);
	}

	strncat(file_path,file_name, num);
	app_get_resource(file_path, absolute_path, (int)MAX_LENGTH);
	return strdup(absolute_path);

}
static inline const char *get_resource_path(const char *file_path)
{
    static char absolute_path[MAX_LENGTH] = {'\0'};
    app_get_resource(file_path, absolute_path, (int)MAX_LENGTH);
    return strdup(absolute_path);
}

static inline const char *get_shared_resource_path(const char *file_path)
{
    static char absolute_path[MAX_LENGTH] = {'\0'};
    app_get_shared_resource(file_path, absolute_path, (int)MAX_LENGTH);
    return strdup(absolute_path);
}

static inline const char *get_storage_image_dir_path()
{
	static char *absolute_path;
    // lets set storage devices
    storage_foreach_device_supported(_storage_device_supported_cb,NULL);
    storage_get_directory(_supportedStorageID,STORAGE_DIRECTORY_IMAGES,&absolute_path);
    DBG("Logging_Dir set = %s",absolute_path);

	return strdup(absolute_path);
}

static inline const char *get_data_path(const char *file_path)
{
    static char* absolute_path = NULL;
    static char result[MAX_LENGTH];
	absolute_path = strdup(app_get_data_path());
    strcpy(result, absolute_path);
    strcat(result, "/");
    strcat(result, file_path);
    DBG("Data Path is %s", result);
    return result;
}

static inline char *gen_data_path(const char *file_path)
{
	char path[128];
    snprintf(path, sizeof(path), "%s%s",app_get_data_path(),file_path);

    return strdup(path);
}

static inline char *format_string(const char *format, ...)
{
	va_list arg;
	char* chTemp;
	chTemp= (char*)malloc(1024 * sizeof(char));
	RETVM_IF(!chTemp, NULL,"malloc failed.");
	va_start(arg,format);
 	vsnprintf(chTemp,1024,format,arg);
	va_end(arg);
	return chTemp;
}

static inline device_type_e get_device_type()
{
	int  width, height;
	device_type_e device_type = DEVICE_NONE;
	system_info_get_platform_int("tizen.org/feature/screen.width", &width);
	system_info_get_platform_int("tizen.org/feature/screen.height", &height);

	if(width == 360 && height == 360)
	{
		//circle
		device_type = DEVICE_WEARABLE_CIRCLE;
	}
	else if(width == 360 && height == 480)
	{
		device_type = DEVICE_WEARABLE_360_X_480;
	}
	else if(width == 320 && height == 320)
	{
		device_type = DEVICE_WEARABLE_320_X_320;
	}
	else
	{
		device_type = DEVICE_MOBILE;
	}

return device_type;

}


#endif /* CONFIG_H_ */
