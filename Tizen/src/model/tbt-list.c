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
 * @file	tbt-list.c
 * @brief	This file Handles main List of Application.
 *
 * @bug
 * @credit
 */

#include "utils/config.h"
#include "utils/logger.h"
#include "model/tbt-list.h"


static const char *axes[] = { "X", "Y", "Z" };

#include "utils/xml-util.h"


static tbt_info tbtapps[] =
{

#ifdef TBT_MODULE_BLE
	#ifdef TBT_MODULE_BLE_GATT_CLIENT

		//Bluetooth LE Gatt Client
		{
			.name = "Gatt Client",
			.parent = "Bluetooth LE [ver 0.1]",
			.apptype = TBT_APP_BLE_GATT_CLIENT,
			.icon_name = "dummy",
			.info = "Bluetooth LE Gatt Client",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16}
		},
	#endif

	#ifdef TBT_MODULE_BLE_GATT_SERVER
		{
			.name = "Gatt Server",
			.parent = "Bluetooth LE",
			.apptype = TBT_APP_BLE_GATT_SERVER,
			.icon_name = "dummy",
			.info = "Bluetooth LE Gatt Server",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16}
		},

	#endif

#endif

#ifdef TBT_MODULE_BT
	#ifdef TBT_MODULE_APP_BT_CLIENT_PAIRED

		//Bluetooth Client
		{
			.name = "Client Paired",
			.parent = "Bluetooth",
			.apptype = TBT_APP_BT_CLIENT_PAIRED,
			.icon_name = "dummy",
			.info = "Pair with bluetooth server",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16}
		},
	#endif

	#ifdef TBT_MODULE_APP_BT_CLIENT_CANCEL_PAIRED


		//Bluetooth Client Pair Cancel
		{
			.name = "Client Cancel Pair",
			.parent = "Bluetooth",
			.apptype = TBT_APP_BT_CLIENT_PAIRED,
			.icon_name = "dummy",
			.info = "Pair with bluetooth server and then cancel it.",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16}
		},
	#endif

	#ifdef TBT_MODULE_APP_BT_SERVER_PAIRED

		// Bluetooth Server
		{
			.name = "Server Paired",
			.parent = "Bluetooth",
			.apptype = TBT_APP_BT_SERVER_PAIRED,
			.icon_name = "dummy",
			.info = "Pair with bluetooth client",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16}
		},
	#endif

	#ifdef TBT_MODULE_APP_BT_OPP_SERVER

		// Bluetooth OPP Server
		{
			.name = "OPP Server",
			.parent = "Bluetooth",
			.apptype = TBT_APP_BT_OPP_SERVER,
			.icon_name = "dummy",
			.info = "Pair with bluetooth OPP client",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16}
		},
	#endif

	#ifdef TBT_MODULE_APP_BT_OPP_CLIENT

		// OPP Client
		{
			.name = "OPP Client",
			.parent = "Bluetooth",
			.apptype = TBT_APP_BT_OPP_CLIENT,
			.icon_name = "dummy",
			.info = "Transfer data using bluetooth OPP",
			.result = 0,
			.required_features_count = 1,
			.file_name = "images/giraffe_800_1179.jpg",
			.features_required = {16}
		},
	#endif

	#ifdef TBT_MODULE_APP_BT_AUDIO_CONNECT_HANDSFREE

		// Handsfree
		{
			.name = "Handsfree",
			.parent = "Bluetooth",
			.apptype = TBT_APP_BT_AUDIO_CONNECT_HANDSFREE,
			.icon_name = "dummy",
			.file_name = "sounds/SampleMP3.mp3",
			.info = "Connect to a bluetooth<br/>audio profile using handsfree service.",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16}

		},
	#endif

	#ifdef TBT_MODULE_APP_BT_AUDIO_CONNECT

		// Audio Connect
		{
			.name = "Audio Connect",
			.parent = "Bluetooth",
			.apptype = TBT_APP_BT_AUDIO_CONNECT,
			.icon_name = "dummy",
			.file_name = "sounds/SampleMP3.mp3",
			.info = "Connect to a bluetooth<br/>audio profile using a2dp service.",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16}

		},
	#endif

	#ifdef TBT_MODULE_APP_BT_AUTHORIZATION

		// Authorization
		{
			.name = "Authorization",
			.parent = "Bluetooth",
			.apptype = TBT_APP_BT_AUTHORIZATION,
			.icon_name = "dummy",
			.info = "Authorize a paired<br/>bluetooth device",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16}
		},
	#endif

	#ifdef TBT_MODULE_APP_BT_PROFILES

		// Bluetooth profiles
		{
			.name = "Bluetooth Profiles",
			.parent = "Bluetooth",
			.apptype = TBT_APP_BT_PROFILES,
			.icon_name = "dummy",
			.info = "Find profile of an audio <br/>connected paired device",
			.file_name = "sounds/SampleMP3.mp3",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16}
		},
	#endif

	#ifdef TBT_MODULE_APP_BT_SDP

		// Bluetooth SDP
		{
			.name = "Bluetooth SDP",
			.parent = "Bluetooth",
			.apptype = TBT_APP_BT_SDP,
			.icon_name = "dummy",
			.info = "Find Service provided by bluetooth device",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16}
		},
	#endif

	#ifdef TBT_MODULE_APP_BT_CLIENT_SOCKET

		// Client socket
		{
			.name = "Client Socket",
			.parent = "Bluetooth",
			.apptype = TBT_APP_BT_CLIENT_SOCKET,
			.icon_name = "dummy",
			.info = "Pair with bluetooth socket server",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16}
		},
	#endif

	#ifdef TBT_MODULE_APP_BT_SERVER_SOCKET

		// Server socket
		{
			.name = "Server Socket",
			.parent = "Bluetooth",
			.apptype = TBT_APP_BT_SERVER_SOCKET,
			.icon_name = "dummy",
			.info = "Pair with bluetooth socket client",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16}
		},
	#endif

	#ifdef TBT_MODULE_APP_BT_HID_CONNECT

		// Bluetooh HID connect
		{
			.name = "HID Connect",
			.parent = "Bluetooth",
			.apptype = TBT_APP_BT_HID_CONNECT,
			.icon_name = "dummy",
			.info = "Connect with HID device",
			.result = 0,
			.required_features_count = 2,
			.features_required = {16,72},
		},
	#endif

	#ifdef TBT_MODULE_APP_BT_AVRCP

		// Bluetooh avrcp
		{
			.name = "AVRCP Connect",
			.parent = "Bluetooth",
			.apptype = TBT_APP_BT_AVRCP,
			.icon_name = "dummy",
			.info = "Connect with AVRCP device",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16},
		},
	#endif


	#ifdef TBT_MODULE_APP_BT_AG

		// Bluetooh avrcp
		{
			.name = "Bluetooth AG",
			.parent = "Bluetooth",
			.apptype = TBT_APP_BT_AG,
			.icon_name = "dummy",
			.info = "Connect using AG profile",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16},
		},
	#endif

	#ifdef TBT_MODULE_APP_BT_HDP
		//Bluetoot HDP
		{
			.name = "HDP Connect",
			.parent = "Bluetooth",
			.apptype = TBT_APP_BT_HDP,
			.icon_name = "dummy",
			.info = "Bluetooth HDP",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16}
		},
	#endif

#endif

#ifdef TBT_MODULE_BT_OOB
	#ifdef TBT_MODULE_APP_BT_OOB_DEVICE

		//OOB device
		{
			.name = "OOB Device",
			.parent = "Bluetooth OOB",
			.apptype = TBT_APP_BT_OOB_DEVICE,
			.icon_name = "dummy",
			.info = "oob device",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16}
		},
	#endif

	#ifdef TBT_MODULE_APP_BT_OOB_PERIPHERAL

		// OOB Peripheral
		{
			.name = "OOB Peripheral",
			.parent = "Bluetooth OOB",
			.apptype = TBT_APP_BT_OOB_PERIPHERAL,
			.icon_name = "dummy",
			.info = "oob device",
			.result = 0,
			.required_features_count = 1,
			.features_required = {16}
		},
	#endif
#endif

#ifdef TBT_MODULE_UI
	#ifdef TBT_MODULE_APP_UI_RESIZE

		//UI Test
		{
			.name = "Resize Drawable",
			.parent = "UI Test",
			.apptype = TBT_APP_UI_RESIZE,
			.icon_name = "dummy",
			.file_name = "images/pebble_689_517.jpg",
			.info = "Resize image and move to random positions",
			.result = 0
		},
	#endif

	#ifdef TBT_MODULE_APP_UI_SHAPE

		// Shape
		{
			.name = "Shape",
			.parent = "UI Test",
			.apptype = TBT_APP_UI_SHAPE,
			.icon_name = "dummy",
			.info = "Resize shape and move<br/> to random positions",
			.result = 0
		},
	#endif

	#ifdef TBT_MODULE_APP_UI_ROTATE

		// Rotate
		{
			.name = "Rotate",
			.parent = "UI Test",
			.apptype = TBT_APP_UI_ROTATE,
			.icon_name = "dummy",
			.info = "Rotate rectangle",
			.result = 0
		},
	#endif

	#ifdef TBT_MODULE_APP_UI_FONT

		// Font
		{
			.name = "Font",
			.parent = "UI Test",
			.apptype = TBT_APP_UI_FONT,
			.icon_name = "dummy",
			.info = "Resize font and <br/>change color",
			.result = 0
		},
	#endif

	#ifdef TBT_MODULE_APP_UI_LINE

		// Line drawable
		{
			.name = "Line Drawable",
			.parent = "UI Test",
			.apptype = TBT_APP_UI_LINE,
			.icon_name = "dummy",
			.info = "Draw line in random <br/>position and length",
			.result = 0
		},
	#endif

	#ifdef TBT_MODULE_APP_UI_ALPHA

		// Alpha Drawable
		{
			.name = "Alpha Drawable",
			.parent = "UI Test",
			.apptype = TBT_APP_UI_ALPHA,
			.icon_name = "dummy",
			.info = "Change alpha value",
			.result = 0
		},
	#endif
#endif

#if 0
#ifdef TBT_MODULE_EFL
	#ifdef TBT_MODULE_APP_EFL_CALLBACK

		//EFL Test
		{
			.name = "EFL Callback",
			.parent = "EFL/Event",
			.apptype = TBT_APP_EFL_CALLBACK,
			.icon_name = "dummy",
			.info = "Check callback add/delete.<br/>Press HW Back button<br/>to check whether it works<br/>or not.",
			.result = 0,
			.required_features_count = 0
		},
	#endif
#endif
#endif

#ifdef TBT_MODULE_SHORTCUT
	#ifdef TBT_MODULE_APP_SHORTCUT

		// Shortcut
		{
			.name = "Shortcut",
			.parent = "EFL/Event",
			.apptype = TBT_APP_SHORTCUT,
			.icon_name = "dummy",
			.info = "Create shortcut in <br/>home screen.<br/> Check SHORTCUT API",
			.result = 0,
			.required_features_count = 0
		},
	#endif
#endif


};



/**
 * @function		get_sensor_error
 * @since_tizen		2.3
 * @description		Get Sensor Error
 * @parameter		int: Int
 * @return		char*
 */
char* get_sensor_error(int ret)
{

switch(ret)
{
	case SENSOR_ERROR_NONE:
		return "SENSOR_ERROR_NONE";
	case SENSOR_ERROR_IO_ERROR:
		return "SENSOR_ERROR_IO_ERROR";
	case SENSOR_ERROR_INVALID_PARAMETER:
		return "SENSOR_ERROR_INVALID_PARAMETER";
	case SENSOR_ERROR_NOT_SUPPORTED:
		return "SENSOR_ERROR_NOT_SUPPORTED";
	case SENSOR_ERROR_PERMISSION_DENIED:
		return "SENSOR_ERROR_PERMISSION_DENIED";
	case SENSOR_ERROR_OUT_OF_MEMORY:
		return "SENSOR_ERROR_OUT_OF_MEMORY";
	case SENSOR_ERROR_NOT_NEED_CALIBRATION:
		return "SENSOR_ERROR_NOT_NEED_CALIBRATION";
	case SENSOR_ERROR_OPERATION_FAILED:
		return "SENSOR_ERROR_OPERATION_FAILED";
	}
	return NULL;
}

static unsigned tbtapps_count = sizeof(tbtapps) / sizeof(tbtapps[0]);


/**
 * @function		tbt_list_init
 * @since_tizen		2.3
 * @description		Tbt List Init
 * @parameter		NA
 * @return		void
 */
void tbt_list_init()
{
	DBG("tbt_list_init called");
    tbt_info *info = tbtapps;
    tbt_info *end = info + tbtapps_count;

    for(; info != end; ++info)
    {
        info->result = 0;
    }

    DBG("__REPORT Reading report...");
    xmlUtil_readResults(tbtapps, tbtapps_count);
    DBG("__REPORT Reading report done.");
}


/**
 * @function		tbt_save_report
 * @since_tizen		2.3
 * @description		Tbt Save Report
 * @parameter		NA
 * @return		void
 */
void tbt_save_report()
{
	DBG("__REPORT Going to Write");
	xmlUtil_writeResults(XML_REPORT_FILE_PATH, tbtapps, tbtapps_count);
	DBG("__REPORT Report wrote.");
}


/**
 * @function		tbt_list_get
 * @since_tizen		2.3
 * @description		Tbt List Get
 * @parameter		unsigned*: Unsigned Pointer
 * @return		const tbt_info*
 */
const tbt_info *tbt_list_get(unsigned *count)
{
    if(count)
    {
        *count = tbtapps_count;
    }

    return tbtapps;
}


/**
 * @function		tbt_info_count
 * @since_tizen		2.3
 * @description		Tbt Info Count
 * @parameter		NA
 * @return		const int
 */
const int tbt_info_count()
{
	return tbtapps_count;
}


/**
 * @function		tbt_list_item_get
 * @since_tizen		2.3
 * @description		Tbt List Item Get
 * @parameter		int: Int
 * @return		tbt_info*
 */
tbt_info *tbt_list_item_get(int index)
{
    if(index < tbtapps_count)
    {
    	return &tbtapps[index];
    }

    return NULL;
}


/**
 * @function		tbt_list_item_index_get_by_app_type
 * @since_tizen		2.3
 * @description		Tbt List Item Index Get By App Type
 * @parameter		int: Int
 * @return		const int
 */
const int tbt_list_item_index_get_by_app_type(int app_type)
{
	int loop_count;
	for(loop_count = 0; loop_count<tbtapps_count; loop_count++)
	{
		if(tbtapps[loop_count].apptype == app_type)
		{
			return loop_count;
		}
	}

	return -1;
}


/**
 * @function		tbt_list_item_count
 * @since_tizen		2.3
 * @description		Tbt List Item Count
 * @parameter		int: Int
 * @return		const int
 */
const int tbt_list_item_count(int index)
{
	int count = -1;

	if(index < tbtapps_count)
	{
		count = index;

		const tbt_info *item = NULL;

		item = tbt_list_item_get(index);
		RETVM_IF(item == NULL, 0, "item is NULL");

		char parent[MAX_LENGTH] = {'\0'};

		snprintf(parent,sizeof(parent), "%s", item->parent);
		DBG("Parent :%s",parent);

		while(index >= 0)
		{
			if(&parent[0] != NULL && strcmp(parent, item->parent) == 0)
			{
				count--;
			}
			else
			{
				break;
			}
			index--;
			item--;
		}
	}
	return (count+1);
}


