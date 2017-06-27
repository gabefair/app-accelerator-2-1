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
 * @file	tbt-bluetoothle-view.c
 * @brief	Bluetooth le
 *
 * @credit
 *
 */

#include "utils/app_module_config.h"
#ifdef TBT_MODULE_BLE

#include <Elementary.h>
#include <bluetooth.h>
#include <glib.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils/logger.h"
#include "utils/config.h"
#include "utils/ui-utils.h"
#include "view/tbt-bluetoothle-view.h"
#include "view/tbt-common-view.h"
#include "bluetooth_internal.h"

#define BT_ADAPTER_DEVICE_DISCOVERY_NONE -1

typedef enum
{
SERVICE_LISTED,
SERVICE_SELECTED,
SERVICE_EXPANDED,
SERVICE_REGISTERED,
CHARACTERISTICS_LISTED,
CHARACTERISTIC_SELECTED,
CHARACTERISTIC_READ,
CHARACTERISTIC_WRITTEN,
CHARACTERISTIC_EXPANDED,
DESCRIPTORS_LISTED,
DESCRIPTORS_SELECTED,
DESCRIPTORS_READ,
DESCRIPTORS_WRITTEN
}ctrl_btn_state_e;

typedef struct {
	bt_gatt_h svc;
	bt_gatt_h chr;
	bt_gatt_h desc;
} gatt_handle_t;

bt_gatt_server_h server;
gatt_handle_t battery_h;
bt_advertiser_h advertiser;




struct _bluetoothle_view
{
	Evas_Object* bluetoothle_list;
	Evas_Object* bluetoothle_label;
	Evas_Object *bluetoothle_btn;
	Evas_Object *read_btn;
	Evas_Object *disconnect_btn;
	Evas_Object *write_btn;
	Evas_Object *expand_btn;
	Evas_Object *services_btn;
	Evas_Object *character_btn;
	Evas_Object *bluetoothle_btn2;
	bt_adapter_state_e adapter_state;
	bt_adapter_visibility_mode_e visibility_mode;
	common_view* view;
	char *remote_addr;

	bt_adapter_le_device_scan_result_info_s *scan_info;
	bt_adapter_le_device_discovery_state_e discovery_state;

	Elm_Object_Item *selected_device_item;

	GList *devices_list;
	GList *log_list;
	bt_gatt_client_h client;
	bt_gatt_h gatt_handle;
	bt_gatt_h characterstic_h, service_h, descriptor_h;
	bool is_read_completed;

	bt_gatt_h all_characteristics[1000];
	int n_characteristics;

	bt_gatt_type_e type;
	bool is_int;

};

static void _app_destroy_cb(void* this);
static void init_bluetooth(void* user_data);
static void _adapter_state_changed_cb(int result, bt_adapter_state_e adapter_state, void *user_data);
static void discover_bluetooth_le(void* user_data);
static void  _bt_adapter_le_scan_result_cb(int result, bt_adapter_le_device_scan_result_info_s *info, void *user_data);
static bool is_new_scanned_device_found(bluetoothle_view *this, bt_adapter_le_device_scan_result_info_s *discovery_info);
static gint bluetooth_list_find_func_cb(gconstpointer a, gconstpointer b);
static void bluetooth_list_free_func_cb(gpointer data);
static void update_view_controls(bluetoothle_view *this);
static void discovered_devices_list_show(bluetoothle_view *this);
static void _device_item_selected_cb(void *data, Evas_Object *obj, void *event_info);
static void _sevice_selected_cb(void *data, Evas_Object *obj, void *event_info);
static void _included_sevice_selected_cb(void *data, Evas_Object *obj, void *event_info);
static void _descriptor_selected_cb(void *data, Evas_Object *obj, void *event_info);
static void set_control_btn_state(ctrl_btn_state_e state, void* user_data);
static void _characteristic_selected_cb(void *data, Evas_Object *obj, void *event_info);
static Evas_Object *add_control_layout(bluetoothle_view *this, Evas_Object *parent);
static void _battery_level_change_cb(void *user_data, Evas_Object *obj, void *event_info);
static void _register_button_pressed_cb(void *user_data, Evas_Object *obj, void *event_info);
static void _control_button_pressed_cb(void *user_data, Evas_Object *obj, void *event_info);
static void _read_button_pressed_cb(void *user_data, Evas_Object *obj, void *event_info);
static void _disconnect_button_pressed_cb(void *user_data, Evas_Object *obj, void *event_info);
static void _services_button_pressed_cb(void *user_data, Evas_Object *obj, void *event_info);
static void _write_button_pressed_cb(void *user_data, Evas_Object *obj, void *event_info);
static void _expand_button_pressed_cb(void *user_data, Evas_Object *obj, void *event_info);
static void _bt_gatt_connection_state_changed_cb(int result, bool connected, const char *remote_address, void *user_data);
static char* get_bluetooth_error(bt_error_e error);
static void register_callback(void* user_data);
static bool _bt_gatt_foreach_services_cb(int total, int index, bt_gatt_h gatt_handle, void *user_data);
static bool _bt_gatt_foreach_services_cb2(int total, int index, bt_gatt_h gatt_handle, void *user_data);
static bool _bt_gatt_foreach_included_service(int total, int index, bt_gatt_h gatt_handle, void *user_data);
static bool _bt_gatt_foreach_characterstics_cb(int total, int index, bt_gatt_h gatt_handle, void *user_data);
static bool _bt_gatt_foreach_characterstics_cb2(int total, int index, bt_gatt_h gatt_handle, void *user_data);
static void _bt_gatt_client_write_request_completed_cb(int result, bt_gatt_h request_handle, void *user_data);
static void _bt_gatt_client_read_request_completed_cb(int result, bt_gatt_h request_handle, void *user_data);
static bool _bt_gatt_foreach_descriptors_cb(int total, int index, bt_gatt_h gatt_handle, void *user_data);
static void _bt_gatt_client_characteristic_value_changed_cb(bt_gatt_h characteristic, char *value, int len, void *user_data);
static void log_list_free_func_cb(gpointer data);

int scan_cb_count = 0;

int bt_gatt_get_specification_name(const char *uuid, char **name)
{
	int i;
	int offset = 0;
	static struct {
		const char *uuid;
		const char *specification_name;
	} bt_uuid_name[] = {
		/* GATT Services */
		{"1800", "Generic Access"},
		{"1801", "Generic Attribute"},
		{"1802", "Immediate Alert"},
		{"1803", "Link Loss"},
		{"1804", "Tx Power"},
		{"1805", "Current Time Service"},
		{"1806", "Reference Time Update Service"},
		{"1807", "Next DST Change Service"},
		{"1808", "Glucose"},
		{"1809", "Health Thermometer"},
		{"180A", "Device Information"},
		{"180D", "Heart Rate"},
		{"180e", "Phone Alert Status Service"},
		{"180F", "Battery Service"},
		{"1810", "Blood Pressure"},
		{"1811", "Alert Notification Service"},
		{"1812", "Human Interface Device"},
		{"1813", "Scan Parameters"},
		{"1814", "Running Speed and Cadence"},
		{"1816", "Cycling Speed and Cadence"},
		{"1818", "Cycling Power"},
		{"1819", "Location and Navigation"},

		/* GATT Declarations */
		{"2800", "Primary Service Declaration"},
		{"2801", "Secondary Service Declaration"},
		{"2802", "Include Declaration"},
		{"2803", "Characteristic Declaration"},

		/* GATT Descriptors */
		{"2900", "Characteristic Extended Properties"},
		{"2901", "Characteristic User Description"},
		{"2902", "Client Characteristic Configuration"},
		{"2903", "Server Characteristic Configuration"},
		{"2904", "Characteristic Format"},
		{"2905", "Characteristic Aggregate Formate"},
		{"2906", "Valid Range"},
		{"2907", "External Report Reference"},
		{"2908", "Report Reference"},

		/* GATT Characteristics */

		{"2a43", "Alert Category ID"},
		{"2a42", "Alert Category ID Bit Mask"},
		{"2a06", "Alert Level"},
		{"2a44", "Alert Notification Control Point"},
		{"2a3f", "Alert Status"},
		{"2a01", "Appearance"},
		{"2a19", "Battery Level"},
		{"2a49", "Blood Pressure Feature"},
		{"2a35", "Blood Pressure Measurement"},
		{"2a38", "Body Sensor Location"},
		{"2a22", "Boot Keyboard Input Report"},
		{"2a32", "Boot Keyboard Output Report"},
		{"2a33", "Boot Mouse Input Report"},
		{"2a5c", "CSC Feature"},
		{"2a5b", "CSC Measurement"},
		{"2a2b", "Current Time"},
		{"2a66", "Cycling Power Control Point"},
		{"2a65", "Cycling Power Feature"},
		{"2a63", "Cycling Power Measurement"},
		{"2a64", "Cycling Power Vector"},
		{"2a08", "Date Time"},
		{"2a0a", "Day Date Time"},
		{"2a09", "Day of Week"},
		{"2a00", "Device Name"},
		{"2a0d", "DST Offset"},
		{"2a0c", "Exact Time 256"},
		{"2a26", "Firmware Revision String"},
		{"2a51", "Glucose Feature"},
		{"2a18", "Glucose Measurement"},
		{"2a34", "Glucose Measurement Context"},
		{"2a27", "Hardware Revision String"},
		{"2a39", "Heart Rate Control Point"},
		{"2a37", "Heart Rate Measurement"},
		{"2a4c", "HID Control Point"},
		{"2a4a", "HID Information"},
		{"2a2a", "IEEE 11073-20601 Regulatory Certification Data List"},
		{"2a36", "Intermediate Cuff Pressure"},
		{"2a1e", "Intermediate Temperature"},
		{"2a6b", "LN Control Point"},
		{"2a6a", "LN Feature"},
		{"2a0f", "Local Time Information"},
		{"2a67", "Location and Speed"},
		{"2a29", "Manufacturer Name String"},
		{"2a21", "Measurement Interval"},
		{"2a24", "Model Number String"},
		{"2a68", "Navigation"},
		{"2a46", "New Alert"},
		{"2a04", "Peripheral Preferred Connection Parameters"},
		{"2a02", "Peripheral Privacy Flag"},
		{"2a50", "PnP ID"},
		{"2a69", "Position Quality"},
		{"2a4e", "Protocol Mode"},
		{"2a03", "Reconnection Address"},
		{"2a52", "Record Access Control Point"},
		{"2a14", "Reference Time Information"},
		{"2a4d", "Report"},
		{"2a4b", "Report Map"},
		{"2a40", "Ringer Control Point"},
		{"2a41", "Ringer Setting"},
		{"2a54", "RSC Feature"},
		{"2a53", "RSC Measurement"},
		{"2a55", "SC Control Point"},
		{"2a4f", "Scan Interval Window"},
		{"2a31", "Scan Refresh"},
		{"2a5d", "Sensor Location"},
		{"2a25", "Serial Number String"},
		{"2a05", "Service Changed"},
		{"2a28", "Software Revision String"},
		{"2a47", "Supported New Alert Category"},
		{"2a48", "Supported Unread Alert Category"},
		{"2a23", "System ID"},
		{"2a1c", "Temperature Measurement"},
		{"2a1d", "Temperature Type"},
		{"2a12", "Time Accuracy"},
		{"2a13", "Time Source"},
		{"2a16", "Time Update Control Point"},
		{"2a17", "Time Update State"},
		{"2a11", "Time with DST"},
		{"2a0e", "Time Zone"},
		{"2a07", "Tx Power Level"},
		{"2a45", "Unread Alert Status"},

		{NULL, NULL}
	};
#if 0
	if (strlen(uuid) >= 8)
		offset = 4;

	for (i = 0; bt_uuid_name[i].uuid; i++) {
		if (!g_ascii_strncasecmp(uuid + offset, bt_uuid_name[i].uuid, 4)) {
			*name = g_strdup(bt_uuid_name[i].specification_name);
			return BT_ERROR_NONE;
		}
	}

	*name = g_strdup("Unknown");
#else

	*name = (char *)malloc(sizeof(char)*5);
	*(*name) = *(uuid + 4);
	*(*name + 1) = *(uuid + 5);
	*(*name + 2) = *(uuid + 6);
	*(*name + 3) = *(uuid + 7);
	*(*name + 4) = '\0';

#endif


	return BT_ERROR_NONE;
}


/**
 * @function		bluetoothle_view_add
 * @since_tizen		2.3
 * @description		Bluetoothle View Add
 * @parameter		Evas_Object*: Evas Object Pointer, tbt_info*: Tbt Info Pointer, Elm_Object_Item*: Elm Object Item Pointer
 * @return		bluetoothle_view*
 */
bluetoothle_view *bluetoothle_view_add(Evas_Object *navi, tbt_info *tbt_info, Elm_Object_Item *item)
{
	RETVM_IF(NULL == navi, NULL, "navi is null");

    bluetoothle_view *this = NULL;
    this = calloc(1, sizeof(bluetoothle_view));
    RETVM_IF(!this, NULL, "calloc failed");

    RETVM_IF(NULL == tbt_info, NULL, "tbt_info is NULL");
    RETVM_IF(NULL == item, NULL, "item is NULL");

    this->view = calloc(1, sizeof(common_view));
    RETVM_IF(!this->view, NULL, "calloc failed");

    tbt_info->layout_group = "bluetoothle_viewer";
    tbt_info->layout_file = get_edje_path("bluetooth_le_viewer.edj");

    common_view_add(navi, tbt_info, item, this->view, _app_destroy_cb, this);
    RETVM_IF(!this, NULL, "calloc failed");
	RETVM_IF(NULL == this->view, NULL, "navi is null");

	this->scan_info = NULL;
	this->is_read_completed = true;
	this->is_int = true;

	//Add Label, Button and List
    this->bluetoothle_label = ui_utils_label_add(this->view->layout, "BLE");
	elm_object_part_content_set(this->view->layout, "bluetoothle_text", this->bluetoothle_label);

	this->bluetoothle_list = elm_list_add(this->view->layout);
	RETVM_IF(!this->bluetoothle_list, NULL, "elm_list_add failed");
	evas_object_data_set(this->bluetoothle_list, "bluetooth_view", this);
	elm_object_part_content_set(this->view->layout, "bluetoothle_list", this->bluetoothle_list);

	evas_object_size_hint_weight_set(this->bluetoothle_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	evas_object_size_hint_align_set(this->bluetoothle_list, EVAS_HINT_FILL, EVAS_HINT_FILL);

	Evas_Object *control = add_control_layout(this, this->view->layout);
	elm_object_part_content_set(this->view->layout, "controlr_part", control);


	//Initialize Bluetooth
	init_bluetooth(this);

    return this;
}


/**
 * @function		discover_bluetooth_le
 * @since_tizen		2.3
 * @description		Discover Bluetooth Le
 * @parameter		void*: Void Pointer
 * @return		static void
 */
static void discover_bluetooth_le(void* user_data)
{
	int result;
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");

	g_list_free_full(this->devices_list, bluetooth_list_free_func_cb);
	this->devices_list = NULL;

	ui_utils_label_set_text(this->bluetoothle_label, "Device Discovery Started", "left");
	result = bt_adapter_le_start_scan(_bt_adapter_le_scan_result_cb, this);
	RETM_IF(result != BT_ERROR_NONE, "bt_adapter_le_start_scan failed --> error: %s", get_bluetooth_error(result));
}


/**
 * @function		register_callback
 * @since_tizen		2.3
 * @description		Register Callback
 * @parameter		void*: Void Pointer
 * @return		static void
 */
static void register_callback(void* user_data)
{
	int result;
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");

	result = bt_adapter_set_state_changed_cb(_adapter_state_changed_cb , this);
	RETM_IF(result != BT_ERROR_NONE, "bt_adapter_set_state_changed_cb Failed");

	result = bt_gatt_set_connection_state_changed_cb(_bt_gatt_connection_state_changed_cb, this);
	RETM_IF(result != BT_ERROR_NONE, "bt_gatt_set_connection_state_changed_cb failed --> error: %s", get_bluetooth_error(result));

}


/**
 * @function		init_bluetooth
 * @since_tizen		2.3
 * @description		Init Bluetooth
 * @parameter		void*: Void Pointer
 * @return		static void
 */
static void init_bluetooth(void* user_data)
{
	int result;
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");

	bt_initialize();
	//Check whether the Bluetooth Service is enabled
	result = bt_adapter_get_state(&this->adapter_state);
	RETM_IF(result != BT_ERROR_NONE, "bt_adapter_get_state Failed");

	if(this->adapter_state == BT_ADAPTER_DISABLED)
	{
		elm_object_text_set(this->bluetoothle_label, "Bluetooth Switched off, Turn ON manually");
	}

	register_callback(this);
}


/**
 * @function		_adapter_state_changed_cb
 * @since_tizen		2.3
 * @description		 Adapter State Changed Cb
 * @parameter		int: Int, void*: Void Pointer
 * @return		static void
 */
static void _adapter_state_changed_cb(int result, bt_adapter_state_e adapter_state, void* data)
{
	DBG(" _adapter_state_changed_cb ");

	RETM_IF(NULL == data, "data is NULL");
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)data;
	RETM_IF(NULL == this, "view is NULL");

	if(result != BT_ERROR_NONE)
	{
		DBG("[adapter_state_changed_cb] Failed! result=%d", result);
		return;
	}
	this->adapter_state = adapter_state;
	if(this->adapter_state == BT_ADAPTER_ENABLED)
	{
		elm_object_text_set(this->bluetoothle_label, "Bluetooth ON");
	}
	else
	{
		elm_object_text_set(this->bluetoothle_label, "Bluetooth Switched off, Turn ON manually");
	}
}


/**
 * @function		is_new_scanned_device_found
 * @since_tizen		2.3
 * @description		Is New Scanned Device Found
 * @parameter		bluetoothle_view*: Bluetoothle View Pointer, bt_adapter_le_device_scan_result_info_s*: Bt Adapter Le Device Scan Result Info S Pointer
 * @return		static bool
 */
static bool is_new_scanned_device_found(bluetoothle_view *this, bt_adapter_le_device_scan_result_info_s *discovery_info)
{

	DBG(" is_new_scanned_device_found ");
	RETVM_IF(NULL == this, NULL, "view is NULL");
	RETVM_IF(NULL == discovery_info, NULL, "discovery_info is NULL");

	GList *old_device = NULL;
	int result = false;


	old_device = g_list_find_custom (this->devices_list, discovery_info, bluetooth_list_find_func_cb);

	if(old_device == NULL)
	{
		result = true;
	}

	return result;
}


/**
 * @function		add_control_layout
 * @since_tizen		2.3
 * @description		Add Control Layout
 * @parameter		bluetoothle_view*: Bluetoothle View Pointer, Evas_Object*: Evas Object Pointer
 * @return		static Evas_Object*
 */
static Evas_Object *add_control_layout(bluetoothle_view *this, Evas_Object *parent)
{
	DBG("add_control_layout__");

	RETVM_IF(NULL == this, NULL, "view is NULL");
	RETVM_IF(NULL == parent, NULL, "parent is NULL");

    Evas_Object *table = elm_table_add(parent);

    evas_object_size_hint_weight_set(table, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(table, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(table);

    if(this->view->tbt_info->apptype == TBT_APP_BLE_GATT_CLIENT)
    {
		this->bluetoothle_btn = ui_utils_push_button_add(this, table, "Discover", _control_button_pressed_cb);
		elm_table_pack(table, this->bluetoothle_btn, 0, 0, 2, 1);


//		this->read_btn = ui_utils_push_button_add(this, table, "Read", _read_button_pressed_cb);
//		elm_table_pack(table, this->read_btn, 0, 1, 1, 1);

		this->services_btn = ui_utils_push_button_add(this, table, "Services", _services_button_pressed_cb);
		elm_table_pack(table, this->services_btn, 0, 1, 1, 1);

//		this->write_btn = ui_utils_push_button_add(this, table, "Write", _write_button_pressed_cb);
//		elm_table_pack(table, this->write_btn, 1, 1, 1, 1);

//		this->expand_btn = ui_utils_push_button_add(this, table, "Expand", _expand_button_pressed_cb);
//		elm_table_pack(table, this->expand_btn, 2, 1, 1, 1);

		this->disconnect_btn = ui_utils_push_button_add(this, table, "Disconnect", _disconnect_button_pressed_cb);
		elm_table_pack(table, this->disconnect_btn, 1, 1, 1, 1);

//		set_control_btn_state(SERVICE_LISTED, this);
		elm_object_disabled_set(this->services_btn, EINA_TRUE);
		elm_object_disabled_set(this->disconnect_btn, EINA_TRUE);

    }
	else if(this->view->tbt_info->apptype == TBT_APP_BLE_GATT_SERVER)
    {
#if 0
		this->bluetoothle_btn = ui_utils_push_button_add(this, table, "Discover", _control_button_pressed_cb);
		elm_table_pack(table, this->bluetoothle_btn, 0, 0, 3, 1);

		this->read_btn = ui_utils_push_button_add(this, table, "Read", _read_button_pressed_cb);
		elm_table_pack(table, this->read_btn, 0, 1, 1, 1);

		this->write_btn = ui_utils_push_button_add(this, table, "Write", _write_button_pressed_cb);
		elm_table_pack(table, this->write_btn, 1, 1, 1, 1);

		this->expand_btn = ui_utils_push_button_add(this, table, "Expand", _expand_button_pressed_cb);
		elm_table_pack(table, this->expand_btn, 2, 1, 1, 1);

		set_control_btn_state(SERVICE_LISTED, this);
#endif
		this->bluetoothle_btn = ui_utils_push_button_add(this, table, "Register Battery Service", _register_button_pressed_cb);
		elm_table_pack(table, this->bluetoothle_btn, 0, 0, 3, 1);

		this->read_btn = ui_utils_push_button_add(this, table, "Change Battery Level", _battery_level_change_cb);
		elm_table_pack(table, this->read_btn, 0, 1, 3, 1);

//		this->write_btn = ui_utils_push_button_add(this, table, "Write", _write_button_pressed_cb);
//		elm_table_pack(table, this->write_btn, 0, 2, 1, 1);

//		this->expand_btn = ui_utils_push_button_add(this, table, "Expand", _expand_button_pressed_cb);
//		elm_table_pack(table, this->expand_btn, 0, 3, 1, 1);

		set_control_btn_state(SERVICE_LISTED, this);



    }


    return table;
}


static const char* __bt_get_error_message(bt_error_e err)
{
	const char* err_str = NULL;

	switch (err) {
	case BT_ERROR_NONE:
		err_str = "BT_ERROR_NONE";
		break;
	case BT_ERROR_CANCELLED:
		err_str = "BT_ERROR_CANCELLED";
		break;
	case BT_ERROR_INVALID_PARAMETER:
		err_str = "BT_ERROR_INVALID_PARAMETER";
		break;
	case BT_ERROR_OUT_OF_MEMORY:
		err_str = "BT_ERROR_OUT_OF_MEMORY";
		break;
	case BT_ERROR_RESOURCE_BUSY:
		err_str = "BT_ERROR_RESOURCE_BUSY";
		break;
	case BT_ERROR_TIMED_OUT:
		err_str = "BT_ERROR_TIMED_OUT";
		break;
	case BT_ERROR_NOW_IN_PROGRESS:
		err_str = "BT_ERROR_NOW_IN_PROGRESS";
		break;
	case BT_ERROR_NOT_INITIALIZED:
		err_str = "BT_ERROR_NOT_INITIALIZED";
		break;
	case BT_ERROR_NOT_ENABLED:
		err_str = "BT_ERROR_NOT_ENABLED";
		break;
	case BT_ERROR_ALREADY_DONE:
		err_str = "BT_ERROR_ALREADY_DONE";
		break;
	case BT_ERROR_OPERATION_FAILED:
		err_str = "BT_ERROR_OPERATION_FAILED";
		break;
	case BT_ERROR_NOT_IN_PROGRESS:
		err_str = "BT_ERROR_NOT_IN_PROGRESS";
		break;
	case BT_ERROR_REMOTE_DEVICE_NOT_BONDED:
		err_str = "BT_ERROR_REMOTE_DEVICE_NOT_BONDED";
		break;
	case BT_ERROR_AUTH_REJECTED:
		err_str = "BT_ERROR_AUTH_REJECTED";
		break;
	case BT_ERROR_AUTH_FAILED:
		err_str = "BT_ERROR_AUTH_FAILED";
		break;
	case BT_ERROR_REMOTE_DEVICE_NOT_FOUND:
		err_str = "BT_ERROR_REMOTE_DEVICE_NOT_FOUND";
		break;
	case BT_ERROR_SERVICE_SEARCH_FAILED:
		err_str = "BT_ERROR_SERVICE_SEARCH_FAILED";
		break;
	case BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED:
		err_str = "BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED";
		break;
	case BT_ERROR_PERMISSION_DENIED:
		err_str = "BT_ERROR_PERMISSION_DENIED";
		break;
	case BT_ERROR_SERVICE_NOT_FOUND:
		err_str = "BT_ERROR_SERVICE_NOT_FOUND";
		break;
	case BT_ERROR_NOT_SUPPORTED:
		err_str = "BT_ERROR_NOT_SUPPORTED";
		break;
	default:
		err_str = "NOT Defined";
		break;
	}

	return err_str;
}

void __bt_gatt_server_read_value_requested_cb (char *remote_address,
				int request_id, bt_gatt_server_h server, bt_gatt_h gatt_handle,
				int offset, void *user_data)
{
	char char_value_1[3] = {0, 1, 2};

	DBG("__bt_gatt_server_read_value_requested_cb");
	DBG("remote_address %s", remote_address);
	DBG("req_id %d", request_id);
	DBG("server %s", (char *)server);
	DBG("gatt_handle %s", (char *)gatt_handle);
	DBG("Offset %d", offset);
	/* Get the attribute new values here */
	bt_gatt_server_send_response(request_id, offset, char_value_1, 3 - offset);
}

void __bt_gatt_server_notification_state_change_cb (bool notify,
			bt_gatt_server_h server, bt_gatt_h gatt_handle, void *user_data)
{
	DBG("__bt_gatt_server_notification_state_change_cb");
	DBG("notify %d", notify);
	DBG("server %s", (char *)server);
	DBG("gatt_handle %s", (char *)gatt_handle);
}

static void __bt_adapter_le_advertising_state_changed_cb(int result,
		bt_advertiser_h advertiser, bt_adapter_le_advertising_state_e adv_state, void *user_data)
{
	DBG("Result : %d", result);
	DBG("Advertiser : %p", advertiser);
	DBG("Advertising %s [%d]", adv_state == BT_ADAPTER_LE_ADVERTISING_STARTED ?
				"started" : "stopped", adv_state);
}


static void _battery_level_change_cb(void *user_data, Evas_Object *obj, void *event_info)
{
	DBG("_battery_level_change_cb");

	RETM_IF(NULL == user_data, "data is NULL");
	RETM_IF(NULL == obj, "obj is NULL");
	

	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");

	int ret;
	char char_value[1] = {1 + (rand()%100)};

	if (!server) {
		DBG("bt gatt server is not created");
		return;
	}
	ret = bt_gatt_set_value(battery_h.chr, char_value, 1);
	DBG("Value[%d], returns %s\n", char_value[0], __bt_get_error_message(ret));
	if (ret == BT_ERROR_NONE)
		ui_utils_label_set_text(this->bluetoothle_label, "Set Value", "left");


	/* Create Advertiser */
	bt_adapter_le_advertising_params_s adv_params = { 0, };
	adv_params.interval_min= 500;
	adv_params.interval_max= 500;

	bt_adapter_le_advertising_state_changed_cb cb;
	cb = __bt_adapter_le_advertising_state_changed_cb;



	ret = bt_adapter_le_create_advertiser (&advertiser);
	DBG("create advertiser: %s \n", __bt_get_error_message(ret));
	if (ret == BT_ERROR_NONE)
		ui_utils_label_set_text(this->bluetoothle_label, "Create Advertiser", "left");

	ret = bt_adapter_le_start_advertising(advertiser, &adv_params, cb, NULL);
	DBG("advertising start: %s \n", __bt_get_error_message(ret));
	if (ret == BT_ERROR_NONE)
		ui_utils_label_set_text(this->bluetoothle_label, "Advertising Start", "left");



}



static void _register_button_pressed_cb(void *user_data, Evas_Object *obj, void *event_info)
{
	DBG("_register_button_pressed_cb");

	RETM_IF(NULL == user_data, "data is NULL");
	RETM_IF(NULL == obj, "obj is NULL");
	

	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");

	int ret;
	bt_gatt_h service = NULL;
	bt_gatt_h characteristic = NULL;
	bt_gatt_h descriptor = NULL;
	char *service_uuid = "180f"; // Battery Service
	char *char_uuid = "2a19"; // Battery Level
	char *desc_uuid = "2902";  // CCCD
	char char_value[1] = {80}; // 80%
	char desc_value[2] = {0, 0}; // Notification & Indication disabled
	int permissions = BT_GATT_PERMISSION_READ;
	int properties = BT_GATT_PROPERTY_READ | BT_GATT_PROPERTY_NOTIFY;

	ret = bt_gatt_server_initialize();
	DBG("bt_gatt_server_initialize : %s \n", __bt_get_error_message(ret));

	ret = bt_gatt_server_create(&server);
	DBG("bt_gatt_server_create : %s \n", __bt_get_error_message(ret));
	if (ret == BT_ERROR_NONE)
		ui_utils_label_set_text(this->bluetoothle_label, "Sever Created", "left");


	ret = bt_gatt_service_create(service_uuid, BT_GATT_SERVICE_TYPE_PRIMARY, &service);
	DBG("bt_gatt_service_create : %s \n", __bt_get_error_message(ret));


	ret = bt_gatt_characteristic_create(char_uuid, permissions, properties,
			char_value, sizeof(char_value), &characteristic);
	DBG("bt_gatt_characteristic_create : %s\n", __bt_get_error_message(ret));
	if (ret == BT_ERROR_NONE)
		ui_utils_label_set_text(this->bluetoothle_label, "Characteristic Create", "left");


	bt_gatt_server_set_read_value_requested_cb(characteristic, __bt_gatt_server_read_value_requested_cb, NULL);
	bt_gatt_server_set_notification_state_change_cb(characteristic, __bt_gatt_server_notification_state_change_cb, NULL);
	ret = bt_gatt_service_add_characteristic(service, characteristic);
	DBG("bt_gatt_service_add_characteristic : %s\n", __bt_get_error_message(ret));

	permissions = BT_GATT_PERMISSION_READ | BT_GATT_PERMISSION_WRITE;
	ret = bt_gatt_descriptor_create(desc_uuid, permissions, desc_value, sizeof(desc_value), &descriptor);
	DBG("bt_gatt_descriptor_create : %s\n", __bt_get_error_message(ret));
	if (ret == BT_ERROR_NONE)
		ui_utils_label_set_text(this->bluetoothle_label, "Descriptor Create", "left");


	ret = bt_gatt_characteristic_add_descriptor(characteristic, descriptor);
	DBG("bt_gatt_characteristic_add_descriptor : %s\n", __bt_get_error_message(ret));

	ret = bt_gatt_server_register_service(server, service);
	DBG("bt_gatt_server_register_service : %s\n", __bt_get_error_message(ret));
	if (ret == BT_ERROR_NONE)
	{
		ui_utils_label_set_text(this->bluetoothle_label, "Register Service", "left");
		set_control_btn_state(SERVICE_REGISTERED, this);
	}


	battery_h.svc = service;
	battery_h.chr = characteristic;
	battery_h.desc = descriptor;


}
Ecore_Task_Cb _scan_stop_cb(void *data)
{
	DBG("scan stop callback");
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)data;
	ui_utils_label_set_text(this->bluetoothle_label, "Device Scan Stopped", "left");

	bt_adapter_le_stop_scan();

	return ECORE_CALLBACK_CANCEL;
}


/**
 * @function		_control_button_pressed_cb
 * @since_tizen		2.3
 * @description		 Control Button Pressed Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _control_button_pressed_cb(void *user_data, Evas_Object *obj, void *event_info)
{
	DBG("_control_button_pressed_cb");

//	RETM_IF(NULL == user_data, "data is NULL");
//	RETM_IF(NULL == obj, "obj is NULL");
	

	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
//	RETM_IF(NULL == this, "view is NULL");
	//Discover Bluetooth LE Devices
	discover_bluetooth_le(this);
	ecore_timer_add(20.0, _scan_stop_cb, this);
}

/**
 * @function		_disconnect_button_pressed_cb
 * @since_tizen		2.3
 * @description		 Disconnect Button Pressed Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _disconnect_button_pressed_cb(void *user_data, Evas_Object *obj, void *event_info)
{
	DBG("_disconnect_button_pressed_cb");
	RETM_IF(NULL == user_data, "data is NULL");
	RETM_IF(NULL == obj, "obj is NULL");
	

	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
	int result;
	bt_gatt_type_e type;
	result = bt_gatt_get_type(this->gatt_handle, &type);
	RETM_IF(result != BT_ERROR_NONE, "bt_gatt_get_type error: %s", get_bluetooth_error(result));

	bt_gatt_disconnect(this->remote_addr);
	ui_utils_label_set_text(this->bluetoothle_label, "Device Disconnected", "left");
	elm_list_clear(this->bluetoothle_list);
}

/**
 * @function		_services_button_pressed_cb
 * @since_tizen		2.3
 * @description		 Services Button Pressed Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _services_button_pressed_cb(void *user_data, Evas_Object *obj, void *event_info)
{
	DBG("_services_button_pressed_cb");

	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;

	int result;
	elm_list_clear(this->bluetoothle_list);
	result = bt_gatt_client_foreach_services(this->client, _bt_gatt_foreach_services_cb, this);
}

void __read_complete_cb(int result, bt_gatt_h h, void *data)
{
	int ret;
	char *str = NULL;
	char *value = NULL;
	int len = 0;
	bool hex_format = false;
	int i;

	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)data;

	ret = bt_gatt_get_value(h, &value, &len);
	if (ret != BT_ERROR_NONE) {
		DBG("bt_gatt_get_value is failed : %d", ret);
		return;
	}


	for (i = 0; i < len; i++)
		if (isalnum(value[i]) == 0) {
			hex_format = true;
			break;
		}

	if (hex_format) {
		str = g_malloc0(len * 5 + 1);
		/* Fix : NULL_RETURNS */
		if (!str) {
			DBG("BT_ERROR_OUT_OF_MEMORY");
			goto fail;
		}
		for (i = 0; i < len; i++) {
			if (i > 0)
				str[i * 5 - 1] = ' ';

			snprintf(&str[i * 5], 5, "0x%02X", value[i]);
		}
	} else {
		str = g_malloc0(len + 1);
		/* Fix : NULL_RETURNS */
		if (!str) {
			DBG("BT_ERROR_OUT_OF_MEMORY");
			goto fail;
		}
		memcpy(str, value, len);
	}

	DBG("Value : %s", str);
	str = format_string("Read Value : %s", str);

	Evas_Object *popup;
	popup = ui_utils_popup_add(this->view->navi, "Characteristic Value");
	elm_object_text_set(popup, str);

	g_free(str);
fail:
	g_free(value);

}



/**
 * @function		_read_button_pressed_cb
 * @since_tizen		2.3
 * @description		 Read Button Pressed Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _read_button_pressed_cb(void *user_data, Evas_Object *obj, void *event_info)
{
	DBG("_read_button_pressed_cb");
	RETM_IF(NULL == user_data, "data is NULL");
	RETM_IF(NULL == obj, "obj is NULL");
	
	bluetoothle_view *this;
	this = evas_object_data_get(obj, "bluetooth_view");
	elm_list_clear(this->bluetoothle_list);

	int result;
	bt_gatt_type_e type;
	result = bt_gatt_get_type((bt_gatt_h)user_data, &type);
	RETM_IF(result != BT_ERROR_NONE, "bt_gatt_get_type error: %s", get_bluetooth_error(result));

#if 0
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
	int result;
	bt_gatt_type_e type;
	result = bt_gatt_get_type(this->gatt_handle, &type);
	RETM_IF(result != BT_ERROR_NONE, "bt_gatt_get_type error: %s", get_bluetooth_error(result));
#endif

	if(type == BT_GATT_TYPE_SERVICE)
		DBG("SERVICE type");
	else if(type == BT_GATT_TYPE_CHARACTERISTIC)
	{
		DBG("CHARACTERISTIC type");
		this->characterstic_h = (bt_gatt_h)user_data;


		result = bt_gatt_client_read_value(this->characterstic_h, _bt_gatt_client_read_request_completed_cb, this);
		RETM_IF(result != BT_ERROR_NONE, "bt_gatt_client_set_characteristic_value_changed_cb error: %s", get_bluetooth_error(result));


	}
	else if(type == BT_GATT_TYPE_DESCRIPTOR)
	{
		DBG("DESCRIPTOR type");
		this->descriptor_h = (bt_gatt_h)user_data;

		int value;
		int result;
		char* str;
		float f_val;

		if(this->descriptor_h == NULL){DBG("THIS IS NULL");}
//		if(this->is_int)
		if (0)
		{
			result = bt_gatt_get_int_value(this->descriptor_h, BT_DATA_TYPE_UINT8, 0, &value);
			RETM_IF(result != BT_ERROR_NONE, "bt_gatt_get_int_value failed --> error: %s", get_bluetooth_error(result));

			DBG("DESC, value:%d", value);

			str = format_string("Descriptor value %d", value);
			elm_list_item_append(this->bluetoothle_list, str, NULL, NULL, NULL, NULL);
			SAFE_DELETE(str);

			elm_list_go(this->bluetoothle_list);
			evas_object_show(this->bluetoothle_list);

			set_control_btn_state(DESCRIPTORS_SELECTED, this);
			elm_object_text_set(this->read_btn, "Read float");
			elm_object_text_set(this->write_btn, "Write float");
			this->is_int = false;
		}
		else
		{
			result = bt_gatt_get_float_value(this->descriptor_h, BT_DATA_TYPE_FLOAT, 0, &f_val);
			RETM_IF(result != BT_ERROR_NONE, "bt_gatt_get_float_value error: %s", get_bluetooth_error(result));
			str = format_string("Descriptor float value %f", f_val);
			elm_list_item_append(this->bluetoothle_list, str, NULL, NULL, NULL, NULL);
			SAFE_DELETE(str);

			elm_list_go(this->bluetoothle_list);
			evas_object_show(this->bluetoothle_list);

			set_control_btn_state(DESCRIPTORS_READ, this);
		}

	}

}

static void _read_button_pressed_cb2(void *user_data, Evas_Object *obj, void *event_info)
{
	DBG("_read_button_pressed_cb2");

	bluetoothle_view *this;
	this = evas_object_data_get(obj, "bluetooth_view");

	bt_gatt_h gatt_handle = (bt_gatt_h)user_data;
	bt_gatt_client_read_value(gatt_handle, __read_complete_cb, this);
}

/**
 * @function		_write_button_pressed_cb
 * @since_tizen		2.3
 * @description		 Write Button Pressed Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _write_button_pressed_cb(void *user_data, Evas_Object *obj, void *event_info)
{
	DBG("_write_button_pressed_cb");
	RETM_IF(NULL == user_data, "data is NULL");
	RETM_IF(NULL == obj, "obj is NULL");
	

	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
	int result;
	bt_gatt_type_e type;
	result = bt_gatt_get_type(this->gatt_handle, &type);
	RETM_IF(result != BT_ERROR_NONE, "bt_gatt_get_type error: %s", get_bluetooth_error(result));

	if(type == BT_GATT_TYPE_SERVICE)
	{

	}
	else if(type == BT_GATT_TYPE_CHARACTERISTIC)
	{
		result = bt_gatt_set_value(this->gatt_handle, "Nazib", 5);
		RETM_IF(result != BT_ERROR_NONE, "bt_gatt_set_value error: %s", get_bluetooth_error(result));

		result = bt_gatt_client_write_value(this->gatt_handle, _bt_gatt_client_write_request_completed_cb, this);
		RETM_IF(result != BT_ERROR_NONE, "bt_gatt_client_write_value error: %s", get_bluetooth_error(result));

	}
	else if(type == BT_GATT_TYPE_DESCRIPTOR)
	{
		if(this->is_int)
		{
			result = bt_gatt_set_int_value(this->descriptor_h, BT_DATA_TYPE_UINT8, 25, 0);
			RETM_IF(result != BT_ERROR_NONE, "bt_gatt_set_int_value error: %s", get_bluetooth_error(result));

		}
		else
		{
			result = bt_gatt_set_float_value(this->descriptor_h, BT_DATA_TYPE_FLOAT, 152, 2, 0);
			RETM_IF(result != BT_ERROR_NONE, "bt_gatt_set_int_value error: %s", get_bluetooth_error(result));
		}

		set_control_btn_state(DESCRIPTORS_WRITTEN, this);

	}
}


/**
 * @function		_expand_button_pressed_cb
 * @since_tizen		2.3
 * @description		 Expand Button Pressed Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _expand_button_pressed_cb(void *user_data, Evas_Object *obj, void *event_info)
{
	DBG("_expand_button_pressed_cb");
	RETM_IF(NULL == user_data, "data is NULL");
	RETM_IF(NULL == obj, "obj is NULL");
	

	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
	int result;
	RETM_IF(NULL == this, "view is NULL");

	bt_gatt_type_e type;
	result = bt_gatt_get_type(this->gatt_handle, &type);
	RETM_IF(result != BT_ERROR_NONE, "bt_gatt_get_type error: %s", get_bluetooth_error(result));

	if(type == BT_GATT_TYPE_SERVICE)
	{

	}
	else if(type == BT_GATT_TYPE_CHARACTERISTIC)
	{
		int properties;
		result = bt_gatt_characteristic_get_properties(this->characterstic_h, &properties);
		RETM_IF(result != BT_ERROR_NONE,"bt_gatt_characteristic_get_properties error: %s", get_bluetooth_error(result));
		DBG("properties: %d", properties);

		result =  bt_gatt_characteristic_foreach_descriptors(this->characterstic_h, _bt_gatt_foreach_descriptors_cb, this);
		RETM_IF(result != BT_ERROR_NONE,"bt_gatt_characteristic_foreach_descriptors error: %s", get_bluetooth_error(result));

		set_control_btn_state(CHARACTERISTIC_EXPANDED, this);
	}
	else if(type == BT_GATT_TYPE_DESCRIPTOR)
	{

	}
}


/**
 * @function		set_control_btn_state
 * @since_tizen		2.3
 * @description		Set Control Btn State
 * @parameter		ctrl_btn_state_e: Ctrl Btn State E, void*: Void Pointer
 * @return		static void
 */
static void set_control_btn_state(ctrl_btn_state_e state, void* user_data)
{
	RETM_IF(NULL == user_data, "data is NULL");
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;

	RETM_IF(NULL == this, "view is NULL");

switch(state)
{
		case SERVICE_LISTED:
		{
			elm_object_disabled_set(this->read_btn, EINA_TRUE);
			elm_object_disabled_set(this->write_btn, EINA_TRUE);
			elm_object_disabled_set(this->expand_btn, EINA_TRUE);
			break;
		}
		case SERVICE_REGISTERED:
		{
			elm_object_disabled_set(this->bluetoothle_btn, EINA_TRUE);
			elm_object_disabled_set(this->read_btn, EINA_FALSE);
//			elm_object_disabled_set(this->write_btn, EINA_TRUE);
//			elm_object_disabled_set(this->expand_btn, EINA_TRUE);
			break;
		}
		case SERVICE_EXPANDED:
		{
			elm_object_text_set(this->bluetoothle_label, "Characteristics..");
			elm_object_disabled_set(this->read_btn, EINA_TRUE);
			elm_object_disabled_set(this->write_btn, EINA_TRUE);
			elm_object_disabled_set(this->expand_btn, EINA_TRUE);
			break;
		}
		case SERVICE_SELECTED:
		{
			elm_object_text_set(this->bluetoothle_label, "Services..");
			elm_object_disabled_set(this->read_btn, EINA_TRUE);
			elm_object_disabled_set(this->write_btn, EINA_TRUE);
			elm_object_disabled_set(this->expand_btn, EINA_FALSE);
			break;
		}
		case CHARACTERISTICS_LISTED:
		{	elm_object_text_set(this->bluetoothle_label, "Characteristics..");
			elm_object_disabled_set(this->read_btn, EINA_TRUE);
			elm_object_disabled_set(this->write_btn, EINA_TRUE);
			elm_object_disabled_set(this->expand_btn, EINA_TRUE);
			break;
		}
		case CHARACTERISTIC_SELECTED:
		{
			elm_object_text_set(this->bluetoothle_label, "Characteristics..");
			elm_object_disabled_set(this->read_btn, EINA_TRUE);
			elm_object_disabled_set(this->write_btn, EINA_FALSE);
			elm_object_disabled_set(this->expand_btn, EINA_FALSE);
			break;
		}
		case CHARACTERISTIC_READ:
		{
			elm_object_text_set(this->bluetoothle_label, "Read Successfully");
			elm_object_disabled_set(this->read_btn, EINA_TRUE);
			elm_object_disabled_set(this->write_btn, EINA_TRUE);
			elm_object_disabled_set(this->expand_btn, EINA_FALSE);
			break;
		}
		case CHARACTERISTIC_WRITTEN:
		{	elm_object_text_set(this->bluetoothle_label, "Written Successfully");
			elm_object_disabled_set(this->read_btn, EINA_FALSE);
			elm_object_disabled_set(this->write_btn, EINA_TRUE);
			elm_object_disabled_set(this->expand_btn, EINA_FALSE);
			break;
		}
		case CHARACTERISTIC_EXPANDED:
		{	elm_object_text_set(this->bluetoothle_label, "Descriptors..");
			elm_object_disabled_set(this->read_btn, EINA_TRUE);
			elm_object_disabled_set(this->write_btn, EINA_TRUE);
			elm_object_disabled_set(this->expand_btn, EINA_TRUE);
			break;
		}
		case DESCRIPTORS_LISTED:
		{	elm_object_text_set(this->bluetoothle_label, "Descriptors..");
			elm_object_disabled_set(this->read_btn, EINA_TRUE);
			elm_object_disabled_set(this->write_btn, EINA_TRUE);
			elm_object_disabled_set(this->expand_btn, EINA_TRUE);
			break;
		}
		case DESCRIPTORS_SELECTED:
		{
			elm_object_text_set(this->bluetoothle_label, "Descriptors..");
			elm_object_disabled_set(this->read_btn, EINA_TRUE);
			elm_object_disabled_set(this->write_btn, EINA_FALSE);
			elm_object_disabled_set(this->expand_btn, EINA_TRUE);
			break;
		}
		case DESCRIPTORS_READ:
		{
			elm_object_text_set(this->bluetoothle_label, "Read Successfully");
			elm_object_disabled_set(this->read_btn, EINA_TRUE);
			elm_object_disabled_set(this->write_btn, EINA_TRUE);
			elm_object_disabled_set(this->expand_btn, EINA_TRUE);
			break;
		}
		case DESCRIPTORS_WRITTEN:
		{
			elm_object_text_set(this->bluetoothle_label, "Written Successfully");
			elm_object_disabled_set(this->read_btn, EINA_FALSE);
			elm_object_disabled_set(this->write_btn, EINA_TRUE);
			elm_object_disabled_set(this->expand_btn, EINA_TRUE);
			break;
		}
	}
}


/**
 * @function		bluetooth_list_find_func_cb
 * @since_tizen		2.3
 * @description		Bluetooth List Find Func Cb
 * @parameter		gconstpointer: Gconstpointer
 * @return		static gint
 */
static gint bluetooth_list_find_func_cb(gconstpointer a, gconstpointer b)
{
	DBG(" bluetooth_list_find_func_cb ");

	if(a && b)
	{
		bt_adapter_le_device_scan_result_info_s * device_info_a;
		device_info_a = (bt_adapter_le_device_scan_result_info_s*)a;
		RETVM_IF(NULL == device_info_a, -1, "device_info_a is NULL");

		bt_adapter_le_device_scan_result_info_s * device_info_b;
		device_info_b = (bt_adapter_le_device_scan_result_info_s*)b;
		RETVM_IF(NULL == device_info_b, -1, "device_info_b is NULL");

		RETVM_IF(NULL == device_info_a->remote_address, -1, "device_info_a->remote_address is NULL");
		RETVM_IF(NULL == device_info_b->remote_address, -1, "device_info_b->remote_address is NULL");

		return strcmp(device_info_a->remote_address, device_info_b->remote_address);
	}
	else
	{
		return -1;
	}

}


/**
 * @function		bluetooth_list_free_func_cb
 * @since_tizen		2.3
 * @description		Bluetooth List Free Func Cb
 * @parameter		gpointer: Gpointer
 * @return		static void
 */
static void bluetooth_list_free_func_cb(gpointer data)
{
	if(data)
	{
		free(data);
	}
}


/**
 * @function		log_list_free_func_cb
 * @since_tizen		2.3
 * @description		Log List Free Func Cb
 * @parameter		gpointer: Gpointer
 * @return		static void
 */
static void log_list_free_func_cb(gpointer data)
{
	if(data)
	{
		free(data);
	}
}


/**
 * @function		_bt_adapter_le_scan_result_cb
 * @since_tizen		2.3
 * @description		 Bt Adapter Le Scan Result Cb
 * @parameter		int: Int, bt_adapter_le_device_scan_result_info_s*: Bt Adapter Le Device Scan Result Info S Pointer, void*: Void Pointer
 * @return		static void
 */
static void  _bt_adapter_le_scan_result_cb(int result, bt_adapter_le_device_scan_result_info_s *info, void *user_data)
{
	DBG("Inside device discovery");

//	RETM_IF(NULL == user_data, "data is NULL");
//	RETM_IF(NULL == info, "info is NULL");
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
//	RETM_IF(NULL == this, "view is NULL");

	if (info != NULL)
	{
		this->scan_info = info;

		if (is_new_scanned_device_found(this, info) == true)
		{
			bt_adapter_le_device_scan_result_info_s *device_info;
			device_info = malloc(sizeof(bt_adapter_le_device_scan_result_info_s));
			DBG("BLE remote_address: %s", info->remote_address);
			if (device_info != NULL)
			{
				memcpy(device_info, info,	sizeof(bt_adapter_le_device_scan_result_info_s));
				device_info->remote_address = strdup(info->remote_address);
				device_info->adv_data = strdup(info->adv_data);
				if (info->scan_data)
					device_info->scan_data = strdup(info->scan_data);

				//Hold the last deviceinfo pointer;
				this->devices_list = g_list_append(this->devices_list, (gpointer) device_info);
			}
		}
	}

	update_view_controls(this);
}

/**
 * @function		_bt_gatt_foreach_services_cb
 * @since_tizen		2.3
 * @description		 Bt Gatt Foreach Services Cb
 * @parameter		int: Int, void*: Void Pointer
 * @return		static bool
 */
static bool _bt_gatt_foreach_services_cb(int total, int index, bt_gatt_h gatt_handle, void *user_data)
{
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
	this->gatt_handle = gatt_handle;

	int result;
	const char* uuid;
	result = bt_gatt_get_uuid(gatt_handle, &uuid);
	DBG("uuid: %s", uuid);
	bt_gatt_client_get_service(this->client, uuid, &gatt_handle);

	elm_object_text_set(this->bluetoothle_label, "Services of the Selected Device");
#if 1
	char* str = NULL;
	bt_gatt_get_specification_name(uuid, &str);
	if (str)
		str = format_string("Service ID: %s", str);
	else
		str = format_string("%d. Unknown", index);

	if ((*(uuid+4) == '1') && (*(uuid+5) == '8') && (*(uuid+6) == '0') && (*(uuid+7) == '1'))
		elm_list_item_append(this->bluetoothle_list, str, NULL, NULL, NULL, gatt_handle);
	else
		elm_list_item_append(this->bluetoothle_list, str, NULL, NULL, _sevice_selected_cb, gatt_handle);

	SAFE_DELETE(str);
#else
	elm_list_item_append(this->bluetoothle_list, uuid, NULL, NULL, _sevice_selected_cb, gatt_handle);

#endif
	return true;
}
 
/**
 * @function		_bt_gatt_foreach_services_cb2
 * @since_tizen		2.3
 * @description		 Bt Gatt Foreach Services Cb
 * @parameter		int: Int, void*: Void Pointer
 * @return		static bool
 */
static bool _bt_gatt_foreach_services_cb2(int total, int index, bt_gatt_h gatt_handle, void *user_data)
{
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
	this->gatt_handle = gatt_handle;


	int result;
	const char* uuid;
	bt_gatt_get_uuid(gatt_handle, &uuid);
	DBG("uuid: %s", uuid);

	bt_gatt_client_get_service(this->client, uuid, &(this->service_h));
	bt_gatt_service_foreach_characteristics (this->service_h, _bt_gatt_foreach_characterstics_cb2, this);


	return true;
}
 


/**
 * @function		_bt_gatt_foreach_included_service
 * @since_tizen		2.3
 * @description		 Bt Gatt Foreach Included Service
 * @parameter		int: Int, void*: Void Pointer
 * @return		static bool
 */
static bool _bt_gatt_foreach_included_service(int total, int index, bt_gatt_h gatt_handle, void *user_data)
{
	RETVM_IF(NULL == user_data, false, "data is NULL");
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
	RETVM_IF(NULL == this,false, "view is NULL");

	int result;
	char* uuid;
	result = bt_gatt_get_uuid(gatt_handle, &uuid);
	RETVM_IF(result != BT_ERROR_NONE,false, "bt_gatt_get_uuid error: %s", get_bluetooth_error(result));
	RETVM_IF(NULL == uuid,false, "uuid is null");
	DBG("_bt_gatt_foreach_included_service uuid: %s", uuid);

	char* str;
	str = format_string("Included Service:  %s",uuid);
	elm_list_item_append(this->bluetoothle_list, str, NULL, NULL, _included_sevice_selected_cb, uuid);
	SAFE_DELETE(str);
	SAFE_DELETE(uuid);

	return true;

}

bool __bt_gatt_client_foreach_desc_cb(int total, int index, bt_gatt_h desc_handle, void *data)
{
	DBG("_bt_gatt_foreach_desc_cb");
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)data;

	char *uuid = NULL;
	bt_gatt_get_uuid(desc_handle, &uuid);
	DBG("uuid: %s", uuid);

	char* str = NULL;
	bt_gatt_get_specification_name(uuid, &str);
	if (str)
		str = format_string("  - descriptor ID: %s", str);

	elm_list_item_append(this->bluetoothle_list, str, NULL, NULL, NULL, desc_handle);

	SAFE_DELETE(str);
	return true;
}

/**
 * @function		_bt_gatt_foreach_characterstics_cb
 * @since_tizen		2.3
 * @description		 Bt Gatt Foreach Characterstics Cb
 * @parameter		int: Int, void*: Void Pointer
 * @return		static bool
 */
static bool _bt_gatt_foreach_characterstics_cb(int total, int index, bt_gatt_h gatt_handle, void *user_data)
{
	DBG("_bt_gatt_foreach_characterstics_cb");
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;

	int result;
	this->gatt_handle = gatt_handle;

	char* uuid;
	result = bt_gatt_get_uuid(gatt_handle, &uuid);
	DBG("uuid: %s", uuid);
	bt_gatt_service_get_characteristic(this->service_h, uuid, &gatt_handle);

	ui_utils_label_set_text(this->bluetoothle_label, "Characteristics and Descriptors", "left");

#if 0
	char* str = NULL;
	bt_gatt_get_specification_name(uuid, &str);
	if (str)
		str = format_string("%d. %s", index, str);

	elm_list_item_append(this->bluetoothle_list, str, NULL, NULL, _read_button_pressed_cb2, gatt_handle);
#else
	char* str = NULL;
	bt_gatt_get_specification_name(uuid, &str);
	if (str)
		str = format_string("Characteristic ID: %s", str);

	elm_list_item_append(this->bluetoothle_list, str, NULL, NULL, _read_button_pressed_cb2, gatt_handle);



#endif

	/* for descriptor */
	bt_gatt_characteristic_foreach_descriptors(gatt_handle, __bt_gatt_client_foreach_desc_cb, this);

	SAFE_DELETE(str);
	return true;
}

/**
 * @function		_bt_gatt_foreach_characterstics_cb2
 * @since_tizen		2.3
 * @description		 Bt Gatt Foreach Characterstics Cb
 * @parameter		int: Int, void*: Void Pointer
 * @return		static bool
 */
static bool _bt_gatt_foreach_characterstics_cb2(int total, int index, bt_gatt_h gatt_handle, void *user_data)
{
	DBG("_bt_gatt_foreach_characterstics_cb2");
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;

	char* uuid;
	bt_gatt_get_uuid(gatt_handle, &uuid);
	DBG("uuid: %s", uuid);

	bt_gatt_service_get_characteristic(this->service_h, uuid, &(this->characterstic_h));

	bt_gatt_client_read_value(this->characterstic_h, __read_complete_cb, NULL);





	return true;
}

/**
 * @function		_bt_gatt_client_characteristic_value_changed_cb
 * @since_tizen		2.3
 * @description		 Bt Gatt Client Characteristic Value Changed Cb
 * @parameter		bt_gatt_h: Bt Gatt H, char*: Char Pointer, void*: Void Pointer
 * @return		static void
 */
static void _bt_gatt_client_characteristic_value_changed_cb(bt_gatt_h characteristic, char *value, int len, void *user_data)
{
	DBG("_bt_gatt_foreach_descriptors_cb");
	RETM_IF(NULL == user_data, "data is NULL");
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");

	DBG("value: %s, len %d", value, len);

	char* str;

	str = format_string("characteristics value changed");
	elm_list_item_append(this->bluetoothle_list, str, NULL, NULL, NULL, NULL);
	SAFE_DELETE(str);
	str = format_string("value %s length %d", value, len);
	elm_list_item_append(this->bluetoothle_list, str, NULL, NULL, NULL, NULL);
	SAFE_DELETE(str);

}


/**
 * @function		_bt_gatt_foreach_descriptors_cb
 * @since_tizen		2.3
 * @description		 Bt Gatt Foreach Descriptors Cb
 * @parameter		int: Int, void*: Void Pointer
 * @return		static bool
 */
static bool _bt_gatt_foreach_descriptors_cb(int total, int index, bt_gatt_h gatt_handle, void *user_data)
{
	DBG("_bt_gatt_foreach_descriptors_cb");
	RETVM_IF(NULL == user_data, false, "data is NULL");
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
	RETVM_IF(NULL == this,false, "view is NULL");
	DBG("Total: %d index: %d", total, index);
	elm_list_clear(this->bluetoothle_list);

	this->gatt_handle = gatt_handle;

	char* str;
	int result;
#if 0
	bt_gatt_h characteristic;
	result = bt_gatt_descriptor_get_characteristic(gatt_handle, &characteristic);
	RETVM_IF(result != BT_ERROR_NONE,false, "bt_gatt_descriptor_get_characteristic error: %s", get_bluetooth_error(result));

	char* characteristic_uuid;
	result = bt_gatt_get_uuid(characteristic, &characteristic_uuid);
	RETVM_IF(result != BT_ERROR_NONE,false, "bt_gatt_get_uuid error: %s", get_bluetooth_error(result));
	RETVM_IF(NULL == characteristic_uuid,false, "characteristic_uuid is null");
	DBG("characteristic_uuid: %s", characteristic_uuid);
#endif

	char* descriptor_uuid;
	result = bt_gatt_get_uuid(gatt_handle, &descriptor_uuid);
	RETVM_IF(result != BT_ERROR_NONE,false,"bt_gatt_get_uuid error: %s", get_bluetooth_error(result));
	RETVM_IF(NULL == descriptor_uuid,false, "descriptor_uuid is null");
	DBG("descriptor_uuid: %s", descriptor_uuid);
	this->descriptor_h = gatt_handle;

	str = format_string("Descriptor %d uuid: %s", index, descriptor_uuid);
	elm_list_item_append(this->bluetoothle_list, str, NULL, NULL, _descriptor_selected_cb, descriptor_uuid);
	SAFE_DELETE(str);
//	SAFE_DELETE(characteristic_uuid);
	SAFE_DELETE(descriptor_uuid);
	return true;
}


/**
 * @function		_bt_gatt_client_write_request_completed_cb
 * @since_tizen		2.3
 * @description		 Bt Gatt Client Write Request Completed Cb
 * @parameter		int: Int, void*: Void Pointer
 * @return		static void
 */
static void _bt_gatt_client_write_request_completed_cb(int result, bt_gatt_h request_handle, void *user_data)
{
	DBG("_bt_gatt_client_write_request_completed_cb");
	RETM_IF(NULL == user_data, "data is NULL");
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
	RETM_IF(NULL == this,"view is NULL");

	set_control_btn_state(CHARACTERISTIC_WRITTEN, this);
}


/**
 * @function		_bt_gatt_client_read_request_completed_cb
 * @since_tizen		2.3
 * @description		 Bt Gatt Client Read Request Completed Cb
 * @parameter		int: Int, void*: Void Pointer
 * @return		static void
 */
static void _bt_gatt_client_read_request_completed_cb(int result, bt_gatt_h request_handle, void *user_data)
{


	DBG("_bt_gatt_client_read_request_completed_cb");
//	RETM_IF(NULL == user_data, "data is NULL");
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
//	RETM_IF(NULL == this,"view is NULL");
	this->is_read_completed = true;
	char* value;
	int value_length;
	result = bt_gatt_get_value(this->characterstic_h, &value, &value_length);
//	RETM_IF(result != BT_ERROR_NONE, "bt_gatt_get_value failed --> error: %s", get_bluetooth_error(result));
	DBG("value: %s, value_length: %d", value, value_length);

	char* str;
	str = format_string("value %s len %d", value, value_length);
	elm_list_item_append(this->bluetoothle_list, str, NULL, NULL, NULL, NULL);
	SAFE_DELETE(str);

	DBG("read completed");
//	set_control_btn_state(CHARACTERISTIC_READ, this);
}


/**
 * @function		update_view_controls
 * @since_tizen		2.3
 * @description		Update View Controls
 * @parameter		bluetoothle_view*: Bluetoothle View Pointer
 * @return		static void
 */
static void update_view_controls(bluetoothle_view *this)
{
	DBG("update_view_controls");
	int result;
	scan_cb_count++;

	//Check whether the Bluetooth Service is enabled
	result = bt_adapter_get_state(&this->adapter_state);
	RETM_IF(result != BT_ERROR_NONE, "bt_adapter_get_state fail > Error = %d", result);

	//If the Bluetooth Service is not enabled
	if(this->adapter_state == BT_ADAPTER_DISABLED)
	{
			this->discovery_state = BT_ADAPTER_DEVICE_DISCOVERY_NONE;
			ui_utils_label_set_text(this->bluetoothle_label, "Bluetooth is Switched Off", "left");
	}
	else if(this->adapter_state == BT_ADAPTER_ENABLED)
	{

			ui_utils_label_set_text(this->bluetoothle_label, "Devices Found", "left");
			discovered_devices_list_show(this);
	}
}


/**
 * @function		discovered_devices_list_show
 * @since_tizen		2.3
 * @description		Discovered Devices List Show
 * @parameter		bluetoothle_view*: Bluetoothle View Pointer
 * @return		static void
 */
static void discovered_devices_list_show(bluetoothle_view *this)
{
	DBG(" discovered_devices_list_show ");
	RETM_IF(NULL == this, "view is NULL");

	elm_list_clear(this->bluetoothle_list);

	GList *l;
	bt_adapter_le_device_scan_result_info_s *device_info;

	for(l = this->devices_list; l != NULL; l = l->next)
	{
		device_info = (bt_adapter_le_device_scan_result_info_s*)l->data;

		if(NULL != device_info)
		{
			char* name = NULL;
			bt_adapter_le_get_scan_result_device_name(device_info, BT_ADAPTER_LE_PACKET_ADVERTISING, &name);

			if(name != NULL)
			{
				elm_list_item_append(this->bluetoothle_list, name, NULL, NULL, _device_item_selected_cb, device_info);
			}
			else
			{
				bt_adapter_le_get_scan_result_device_name(device_info, BT_ADAPTER_LE_PACKET_SCAN_RESPONSE, &name);

				if (name != NULL)
					elm_list_item_append(this->bluetoothle_list, name, NULL, NULL, _device_item_selected_cb, device_info);
				else
					elm_list_item_append(this->bluetoothle_list, device_info->remote_address, NULL, NULL, _device_item_selected_cb, device_info);
			}
		}
	}

	elm_list_go(this->bluetoothle_list);
	evas_object_show(this->bluetoothle_list);
}


/**
 * @function		_device_item_selected_cb
 * @since_tizen		2.3
 * @description		 Device Item Selected Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _device_item_selected_cb(void *data, Evas_Object *obj, void *event_info)
{
	DBG(" _device_item_selected_cb ");
	int result;

//    RETM_IF(!data, "data is NULL");
    

    bluetoothle_view *this;
    this = evas_object_data_get(obj, "bluetooth_view");
//    RETM_IF(!this, "view is NULL");

    bt_adapter_le_device_scan_result_info_s *device_info;
    device_info = (bt_adapter_le_device_scan_result_info_s*)data;
//	RETM_IF(NULL == device_info, "bt_adapter_device_le_discovery_info_s is NULL");

	DBG("Device Info remote address: %s", device_info->remote_address);

	Elm_Object_Item *item;
	item = (Elm_Object_Item*)event_info;
	elm_list_item_selected_set(item, EINA_TRUE);
	this->selected_device_item = item;

	result = bt_adapter_le_stop_scan();
//	RETM_IF(result != BT_ERROR_NONE, "bt_adapter_le_stop_scan fail > Error = %d", result);

	this->remote_addr = device_info->remote_address;
	result = bt_gatt_connect(device_info->remote_address, false);
	DBG("bt_gatt_connect %s", get_bluetooth_error(result));
//	RETM_IF(result != BT_ERROR_NONE, "bt_gatt_connect failed --> error: %s", get_bluetooth_error(result));
	elm_object_text_set(this->bluetoothle_label, "Device Connecting");
}


/**
 * @function		_sevice_selected_cb
 * @since_tizen		2.3
 * @description		 Sevice Selected Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _sevice_selected_cb(void *data, Evas_Object *obj, void *event_info)
{
	DBG(" _sevice_selected_cb ");
	int result;

	bluetoothle_view *this;
	this = evas_object_data_get(obj, "bluetooth_view");
	elm_list_clear(this->bluetoothle_list);
	bt_gatt_h service_h = (bt_gatt_h)data;
	this->service_h = service_h;

	DBG("gatt_handle type service");
	result = bt_gatt_service_foreach_characteristics(service_h, _bt_gatt_foreach_characterstics_cb, this);
}

/**
 * @function		_included_sevice_selected_cb
 * @since_tizen		2.3
 * @description		 Included Sevice Selected Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _included_sevice_selected_cb(void *data, Evas_Object *obj, void *event_info)
{
	DBG("_included_sevice_selected_cb");
	int result;

	RETM_IF(!data, "data is NULL");
	

	bluetoothle_view *this;
	this = evas_object_data_get(obj, "bluetooth_view");
	RETM_IF(!this, "view is NULL");


	char* uuid = (char*)data;
	bt_gatt_h included_service;
	result = bt_gatt_service_get_included_service(this->service_h, uuid, &included_service);
	RETM_IF(result != BT_ERROR_NONE, "bt_gatt_service_get_included_service error: %s", get_bluetooth_error(result));

	this->service_h = included_service;

//	set_control_btn_state(SERVICE_LISTED, this);

}


/**
 * @function		_descriptor_selected_cb
 * @since_tizen		2.3
 * @description		 Descriptor Selected Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _descriptor_selected_cb(void *data, Evas_Object *obj, void *event_info)
{
	DBG("_descriptor_selected_cb ");

	RETM_IF(!data, "data is NULL");
	

	bluetoothle_view *this;
	this = evas_object_data_get(obj, "bluetooth_view");
	RETM_IF(!this, "view is NULL");
	char* uuid = (char*)data;
	bt_gatt_h descriptor;
	int result;

	DBG("descriptor uuid asynch: %s", uuid);
	RETM_IF(NULL == this->characterstic_h,"bt_gatt_characteristic_get_descriptor");
	result = bt_gatt_characteristic_get_descriptor(this->characterstic_h, uuid, &descriptor);
	RETM_IF(result != BT_ERROR_NONE,"bt_gatt_characteristic_get_descriptor error: %s", get_bluetooth_error(result));

	this->descriptor_h = descriptor;
	this->is_int = true;

	elm_object_text_set(this->read_btn, "Read int");
	elm_object_text_set(this->write_btn, "Write int");
//	set_control_btn_state(DESCRIPTORS_SELECTED, this);

	SAFE_DELETE(uuid);
}


/**
 * @function		_characteristic_selected_cb
 * @since_tizen		2.3
 * @description		 Characteristic Selected Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _characteristic_selected_cb(void *data, Evas_Object *obj, void *event_info)
{

#if 0
	DBG(" _characteristic_selected_cb ");

	
	int result;

	char *value = NULL;
	int len = 0;

	bluetoothle_view *this;
	this = evas_object_data_get(obj, "bluetooth_view");

	bt_gatt_h character_h = (bt_gatt_h)data;

	char* characteristic_uuid;
	result = bt_gatt_get_uuid(character_h, &characteristic_uuid);

	bt_gatt_get_value(character_h, &value, &len);
	bt_gatt_characteristic_create(characteristic_uuid, permissions, properties, value, len, character_h);
	this->characterstic_h = character_h;

	result = bt_gatt_characteristic_foreach_descriptors(character_h, _bt_gatt_foreach_descriptors_cb, this);
	RETM_IF(result != BT_ERROR_NONE, "bt_gatt_service_foreach_characteristics failed --> error: %s", get_bluetooth_error(result));
#endif
}


/**
 * @function		_bt_gatt_connection_state_changed_cb
 * @since_tizen		2.3
 * @description		 Bt Gatt Connection State Changed Cb
 * @parameter		int: Int, const char*: Const char Pointer, void*: Void Pointer
 * @return		static void
 */
static void _bt_gatt_connection_state_changed_cb(int result, bool connected, const char *remote_address, void *user_data)
{
	DBG("_bt_gatt_connection_state_changed_cb: %d", result);
	RETM_IF(NULL == user_data, "data is NULL");
	bluetoothle_view *this = NULL;
	this = (bluetoothle_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");
	RETM_IF(NULL == remote_address, "remote_address is NULL");

	int ret;

	if(connected)
	{
		elm_object_text_set(this->bluetoothle_label, "Device Connected");
		elm_object_disabled_set(this->disconnect_btn, EINA_FALSE);


		if(this->view->tbt_info->apptype == TBT_APP_BLE_GATT_CLIENT)
		{
			elm_list_clear(this->bluetoothle_list);
			ret = bt_gatt_client_create(remote_address, &this->client);
			RETM_IF(ret != BT_ERROR_NONE, "bt_gatt_client_create error: %s", get_bluetooth_error(ret));

			ret = bt_gatt_client_foreach_services(this->client, _bt_gatt_foreach_services_cb, this);
			RETM_IF(ret != BT_ERROR_NONE, "bt_gatt_client_foreach_services error: %s", get_bluetooth_error(ret));
			elm_object_disabled_set(this->services_btn, EINA_FALSE);

			char* client_remote_address;
			ret = bt_gatt_client_get_remote_address(this->client, &client_remote_address);
			RETM_IF(ret != BT_ERROR_NONE, "bt_gatt_client_get_remote_address error: %s", get_bluetooth_error(ret));
			DBG("client_remote_address: %s", client_remote_address);
		}

	}
	else
	{
		elm_object_text_set(this->bluetoothle_label, "Device Disconnected");
		elm_object_disabled_set(this->services_btn, EINA_TRUE);
		elm_object_disabled_set(this->disconnect_btn, EINA_TRUE);

	}

}


/**
 * @function		_app_destroy_cb
 * @since_tizen		2.3
 * @description		 App Destroy Cb
 * @parameter		void*: Void Pointer
 * @return		void
 */
void _app_destroy_cb(void* this)
{
	bluetoothle_view *view = NULL;
	view = (bluetoothle_view*)this;
	RETM_IF(NULL == view, "view is NULL");

	int result;


	if(view->view->tbt_info->apptype == TBT_APP_BLE_GATT_CLIENT)
	{
		bt_adapter_le_stop_scan();
		result = bt_gatt_client_destroy(view->client);
		RETM_IF(result != BT_ERROR_NONE, "bt_gatt_client_destroy error: %s", get_bluetooth_error(result));
		result = bt_gatt_client_unset_characteristic_value_changed_cb(view->characterstic_h);
		RETM_IF(result != BT_ERROR_NONE, "bt_gatt_client_unset_characteristic_value_changed_cb error: %s", get_bluetooth_error(result));
	}
	result = bt_gatt_unset_connection_state_changed_cb();
	RETM_IF(result != BT_ERROR_NONE, "bt_gatt_unset_connection_state_changed_cb error: %s", get_bluetooth_error(result));

	SAFE_DELETE(view->view);
	SAFE_DELETE(view);
}


/**
 * @function		get_bluetooth_error
 * @since_tizen		2.3
 * @description		Get Bluetooth Error
 * @parameter		bt_error_e: Bt Error E
 * @return		static char*
 */
static char* get_bluetooth_error(bt_error_e error)
{
	char* ret;

switch(error)
{
	case BT_ERROR_NONE:
		ret = "BT_ERROR_NONE";
		break;
	case BT_ERROR_CANCELLED:
		ret = "BT_ERROR_CANCELLED";
		break;
	case BT_ERROR_INVALID_PARAMETER:
		ret = "BT_ERROR_INVALID_PARAMETER";
		break;
	case BT_ERROR_OUT_OF_MEMORY:
		ret = "BT_ERROR_OUT_OF_MEMORY";
		break;
	case BT_ERROR_RESOURCE_BUSY:
		ret = "BT_ERROR_RESOURCE_BUSY";
		break;
	case BT_ERROR_TIMED_OUT:
		ret = "BT_ERROR_TIMED_OUT";
		break;
	case BT_ERROR_NOW_IN_PROGRESS:
		ret = "BT_ERROR_NOW_IN_PROGRESS";
		break;
	case BT_ERROR_NOT_SUPPORTED:
		ret = "BT_ERROR_NOT_SUPPORTED";
		break;
	case BT_ERROR_PERMISSION_DENIED:
		ret = "BT_ERROR_PERMISSION_DENIED";
		break;
	case BT_ERROR_QUOTA_EXCEEDED:
		ret = "BT_ERROR_QUOTA_EXCEEDED";
		break;
	case BT_ERROR_NO_DATA:
		ret = "BT_ERROR_NO_DATA";
		break;
	case BT_ERROR_NOT_INITIALIZED:
		ret = "BT_ERROR_NOT_INITIALIZED";
		break;
	case BT_ERROR_NOT_ENABLED:
		ret = "BT_ERROR_NOT_ENABLED";
		break;
	case BT_ERROR_ALREADY_DONE:
		ret = "BT_ERROR_ALREADY_DONE";
		break;
	case BT_ERROR_OPERATION_FAILED:
		ret = "BT_ERROR_OPERATION_FAILED";
		break;
	case BT_ERROR_NOT_IN_PROGRESS:
		ret = "BT_ERROR_NOT_IN_PROGRESS";
		break;
	case BT_ERROR_REMOTE_DEVICE_NOT_BONDED:
		ret = "BT_ERROR_REMOTE_DEVICE_NOT_BONDED";
		break;
	case BT_ERROR_AUTH_REJECTED:
		ret = "BT_ERROR_AUTH_REJECTED";
		break;
	case BT_ERROR_AUTH_FAILED:
		ret = "BT_ERROR_AUTH_FAILED";
		break;
	case BT_ERROR_REMOTE_DEVICE_NOT_FOUND:
		ret = "BT_ERROR_NONE";
		break;
	case BT_ERROR_SERVICE_SEARCH_FAILED:
		ret = "BT_ERROR_SERVICE_SEARCH_FAILED";
		break;
	case BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED:
		ret = "BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED";
		break;
	case BT_ERROR_AGAIN:
		ret = "BT_ERROR_AGAIN";
		break;
	case BT_ERROR_SERVICE_NOT_FOUND:
		ret = "BT_ERROR_SERVICE_NOT_FOUND";
		break;
	default:
		ret = "Unknown Error";
		break;
	}

	return ret;
}

#endif
