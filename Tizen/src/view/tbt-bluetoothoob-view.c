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
 * @file	tbt-bluetoothoob-view.c
 * @brief	Bluetooth OOB
 *
 * @credit
 *
 */
#include "utils/app_module_config.h"
#ifdef TBT_MODULE_BT_OOB

#include <Elementary.h>
#include "utils/logger.h"
#include "utils/config.h"
#include "utils/ui-utils.h"
#include "view/tbt-bluetoothoob-view.h"
#include "view/tbt-common-view.h"

struct _bluetoothoob_view
{
	common_view* view;
	Evas_Object *bluetoothoob_label;
	Evas_Object *btn_make_pair;

	char* remote_address;
	char* remote_hash;
	char* remote_randomizer;

	char* local_address;
	char* local_hash;
	char* local_randomizer;


};

static void _app_destroy_cb(void* this);
static char* get_bt_adapter_error(int result);
static char* get_nfc_error(nfc_error_e error_code);

static void on_make_pair_button_pressed_cb(void *data, Evas_Object *obj, void *event_info);

static void initialize_nfc(bluetoothoob_view *this);
static int nfc_onoff_operation(void);
static void _nfc_activation_changed_cb(bool activated , void *user_data);
static void _nfc_ndef_discovered_cb(nfc_ndef_message_h message, void *user_data);
static void _nfc_p2p_target_discovered_cb(nfc_discovered_type_e type, nfc_p2p_target_h target, void *user_data);
static void _nfc_tag_discovered_cb(nfc_discovered_type_e type, nfc_tag_h tag, void *user_data);
static void _tag_write_completed_cb(nfc_error_e result, void *user_data);
static void _nfc_tag_read_completed_cb(nfc_error_e result, nfc_ndef_message_h message, void *user_data);
static void _adapter_device_bond_created_cb(int result, bt_device_info_s *device_info, void *data);
static nfc_ndef_message_h create_ndef_message();
static void nfc_ndef_message_read(nfc_ndef_message_h message, void *user_data);


/**
 * @function		bluetoothoob_view_add
 * @since_tizen		2.3
 * @description		Bluetoothoob View Add
 * @parameter		Evas_Object*: Evas Object Pointer, tbt_info*: Tbt Info Pointer, Elm_Object_Item*: Elm Object Item Pointer
 * @return		bluetoothoob_view*
 */
bluetoothoob_view *bluetoothoob_view_add(Evas_Object *navi, tbt_info *tbt_info, Elm_Object_Item *item)
{
	RETVM_IF(NULL == navi, NULL, "navi is null");
	int result;

	bluetoothoob_view *this = NULL;
	this = calloc(1, sizeof(bluetoothoob_view));

	this->view = calloc(1, sizeof(common_view));
	RETVM_IF(!this->view, NULL, "calloc failed");

	this->remote_address = NULL;
	this->remote_hash = NULL;
	this->remote_randomizer = NULL;

	tbt_info->layout_group = "bluetooth_oob_viewer";
	tbt_info->layout_file = get_edje_path("bluetooth_oob_viewer.edj");

	common_view_add(navi, tbt_info, item, this->view, _app_destroy_cb, this);
	RETVM_IF(!this, NULL, "calloc failed");
	RETVM_IF(NULL == this->view, NULL, "navi is null");

	if(this->view->tbt_info->apptype == TBT_APP_BT_OOB_DEVICE)
	{
		Evas_Object *table = elm_table_add(this->view->layout);
		this->btn_make_pair = ui_utils_push_button_add(this, table, "Make Pair", on_make_pair_button_pressed_cb);
		elm_table_pack(table, this->btn_make_pair, 1, 0, 1, 1);
		elm_object_part_content_set(this->view->layout, "controlr_part", table);
		evas_object_show(table);
	}

	bt_initialize();

	result = bt_device_set_bond_created_cb(_adapter_device_bond_created_cb, this);
	RETVM_IF(result != BT_ERROR_NONE, NULL, "bt_device_set_bond_created_cb Failed");

	initialize_nfc(this);

	return this;
}


/**
 * @function		_app_destroy_cb
 * @since_tizen		2.3
 * @description		 App Destroy Cb
 * @parameter		void*: Void Pointer
 * @return		static void
 */
static void _app_destroy_cb(void* view)
{
	RETM_IF(NULL == view, "data is NULL");
	int result;

	bluetoothoob_view *this = NULL;
	this = (bluetoothoob_view*)view;
	RETM_IF(NULL == this, "view is NULL");

	result = bt_device_unset_bond_created_cb();
	RETM_IF(result != BT_ERROR_NONE, "bt_device_uset_bond_created_cb Failed");


	if(this->view->tbt_info->apptype == TBT_APP_BT_OOB_DEVICE)
	{
		result = bt_adapter_remove_remote_oob_data(this->remote_address);
		RETM_IF(result != BT_ERROR_NONE, "bt_adapter_remove_remote_oob_data Failed");
	}

	nfc_manager_unset_activation_changed_cb();

	nfc_manager_unset_ndef_discovered_cb();

	nfc_manager_unset_p2p_target_discovered_cb();

	nfc_manager_unset_tag_discovered_cb();

	SAFE_DELETE(this->remote_address);
	SAFE_DELETE(this->remote_hash);
	SAFE_DELETE(this->remote_randomizer);
	SAFE_DELETE(this->view);
	SAFE_DELETE(this);

}


/**
 * @function		get_bt_adapter_error
 * @since_tizen		2.3
 * @description		Get Bt Adapter Error
 * @parameter		int: Int
 * @return		static char*
 */
static char* get_bt_adapter_error(int result)
{
	char* err = NULL;

	if(result == BT_ERROR_NONE)
	{
		err = "BT_ERROR_NONE";
	}
	else if(result == BT_ERROR_CANCELLED)
	{
		err = "BT_ERROR_CANCELLED";
	}
	else if(result == BT_ERROR_INVALID_PARAMETER)
	{
		err = "BT_ERROR_INVALID_PARAMETER";
	}
	else if(result == BT_ERROR_OUT_OF_MEMORY)
	{
		err = "BT_ERROR_OUT_OF_MEMORY";
	}
	else if(result == BT_ERROR_RESOURCE_BUSY)
	{
		err = "BT_ERROR_RESOURCE_BUSY";
	}
	else if(result == BT_ERROR_TIMED_OUT)
	{
		err = "BT_ERROR_TIMED_OUT";
	}
	else if(result == BT_ERROR_NOW_IN_PROGRESS)
	{
		err = "BT_ERROR_NOW_IN_PROGRESS";
	}
	else if(result == BT_ERROR_PERMISSION_DENIED)
	{
		err = "BT_ERROR_PERMISSION_DENIED";
	}
	else if(result == BT_ERROR_QUOTA_EXCEEDED)
	{
		err = "BT_ERROR_QUOTA_EXCEEDED";
	}
	else if(result == BT_ERROR_NOT_INITIALIZED)
	{
		err = "BT_ERROR_NOT_INITIALIZED";
	}
	else if(result == BT_ERROR_NOT_ENABLED)
	{
		err = "BT_ERROR_NOT_ENABLED";
	}
	else if(result == BT_ERROR_ALREADY_DONE)
	{
		err = "BT_ERROR_ALREADY_DONE";
	}
	else if(result == BT_ERROR_OPERATION_FAILED)
	{
		err = "BT_ERROR_OPERATION_FAILED";
	}
	else if(result == BT_ERROR_NOT_IN_PROGRESS)
	{
		err = "BT_ERROR_NOT_IN_PROGRESS";
	}
	else if(result == BT_ERROR_REMOTE_DEVICE_NOT_BONDED)
	{
		err = "BT_ERROR_REMOTE_DEVICE_NOT_BONDED";
	}
	else if(result == BT_ERROR_AUTH_REJECTED)
	{
		err = "BT_ERROR_AUTH_REJECTED";
	}
	else if(result == BT_ERROR_AUTH_FAILED)
	{
		err = "BT_ERROR_AUTH_FAILED";
	}
	else if(result == BT_ERROR_REMOTE_DEVICE_NOT_FOUND)
	{
		err = "BT_ERROR_REMOTE_DEVICE_NOT_FOUND";
	}
	else if(result == BT_ERROR_SERVICE_SEARCH_FAILED)
	{
		err = "BT_ERROR_SERVICE_SEARCH_FAILED";
	}
	else if(result == BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED)
	{
		err = "BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED";
	}
	else if(result == BT_ERROR_AGAIN)
	{
		err = "BT_ERROR_AGAIN";
	}
	else if(result == BT_ERROR_SERVICE_NOT_FOUND)
	{
		err = "BT_ERROR_SERVICE_NOT_FOUND";
	}
	else
	{
		err = "UNKNOWN_ERROR";
	}

	return err;

}


/**
 * @function		get_nfc_error
 * @since_tizen		2.3
 * @description		Get Nfc Error
 * @parameter		nfc_error_e: Nfc Error E
 * @return		static char*
 */
static char* get_nfc_error(nfc_error_e error_code)
{
	char* error;

switch(error_code)
{
	case NFC_ERROR_NONE:
		error = "NFC_ERROR_NONE";
		break;
	case NFC_ERROR_OUT_OF_MEMORY:
		error = "NFC_ERROR_OUT_OF_MEMORY";
		break;
	case NFC_ERROR_OPERATION_FAILED:
		error = "NFC_ERROR_OPERATION_FAILED";
		break;
	case NFC_ERROR_INVALID_PARAMETER:
		error = "NFC_ERROR_INVALID_PARAMETER";
		break;
	case NFC_ERROR_INVALID_NDEF_MESSAGE:
		error = "NFC_ERROR_INVALID_NDEF_MESSAGE";
		break;
	case NFC_ERROR_INVALID_RECORD_TYPE:
		error = "NFC_ERROR_INVALID_RECORD_TYPE";
		break;
	case NFC_ERROR_TIMED_OUT:
		error = "NFC_ERROR_TIMED_OUT";
		break;
	case NFC_ERROR_DEVICE_BUSY:
		error = "NFC_ERROR_DEVICE_BUSY";
		break;
	case NFC_ERROR_NO_DEVICE:
		error = "NFC_ERROR_NO_DEVICE";
		break;
	case NFC_ERROR_NOT_ACTIVATED:
		error = "NFC_ERROR_NOT_ACTIVATED";
		break;
	case NFC_ERROR_NOT_SUPPORTED:
		error = "NFC_ERROR_NOT_SUPPORTED";
		break;
	case NFC_ERROR_ALREADY_ACTIVATED:
		error = "NFC_ERROR_ALREADY_ACTIVATED";
		break;
	case NFC_ERROR_ALREADY_DEACTIVATED:
		error = "NFC_ERROR_ALREADY_DEACTIVATED";
		break;
	case NFC_ERROR_READ_ONLY_NDEF:
		error = "NFC_ERROR_READ_ONLY_NDEF";
		break;
	case NFC_ERROR_NO_SPACE_ON_NDEF:
		error = "NFC_ERROR_NO_SPACE_ON_NDEF";
		break;
	case NFC_ERROR_NO_NDEF_MESSAGE:
		error = "NFC_ERROR_NO_NDEF_MESSAGE";
		break;
	case NFC_ERROR_NOT_NDEF_FORMAT:
		error = "NFC_ERROR_NOT_NDEF_FORMAT";
		break;
	case NFC_ERROR_SECURITY_RESTRICTED:
		error = "NFC_ERROR_SECURITY_RESTRICTED";
		break;
	case NFC_ERROR_PERMISSION_DENIED:
		error = "NFC_ERROR_PERMISSION_DENIED";
		break;
	case NFC_ERROR_ILLEGAL_STATE:
		error = "NFC_ERROR_ILLEGAL_STATE";
		break;
	case NFC_ERROR_NOT_INITIALIZED:
		error = "NFC_ERROR_NOT_INITIALIZED";
		break;
	case NFC_ERROR_TAG_NOT_SUPPORTED:
		error = "NFC_ERROR_TAG_NOT_SUPPORTED";
		break;
	}

	return error;
}


/**
 * @function		on_make_pair_button_pressed_cb
 * @since_tizen		2.3
 * @description		On Make Pair Button Pressed Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void on_make_pair_button_pressed_cb(void *user_data, Evas_Object *obj, void *event_info)
{
	DBG("on_make_pair_button_pressed_cb");
	RETM_IF(NULL == user_data, "data is NULL");
	int result;

	bluetoothoob_view *this = NULL;
	this = (bluetoothoob_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");

	if(this->remote_address != NULL && this->remote_hash != NULL && this->remote_randomizer != NULL)
	{
		result = bt_adapter_set_remote_oob_data(this->remote_address,this->remote_hash,this->remote_randomizer, sizeof(this->remote_hash),sizeof(this->remote_randomizer));
		RETM_IF(result != BT_ERROR_NONE, "bt_adapter_set_remote_oob_data failed %s",get_bt_adapter_error(result));
		result = bt_device_create_bond(this->remote_address);
		RETM_IF(result != BT_ERROR_NONE, "bt_device_create_bond failed %s",get_bt_adapter_error(result));
	}
}


/**
 * @function		initialize_nfc
 * @since_tizen		2.3
 * @description		Initialize Nfc
 * @parameter		bluetoothoob_view*: Bluetoothoob View Pointer
 * @return		static void
 */
static void initialize_nfc(bluetoothoob_view *this)
{
	DBG("Inside initialize_nfc");
	RETM_IF(NULL == this, "view is NULL");

	int result = NFC_ERROR_NONE;

	if(nfc_manager_is_activated() == false)
	{
		result = nfc_onoff_operation();
		RETM_IF(result != 0, "nfc_onoff_operation failed %d", result);
	}
	result = nfc_manager_initialize();
	RETM_IF(result != NFC_ERROR_NONE, "nfc_manager_initialize failed %s",get_nfc_error(result));


	result = nfc_manager_set_activation_changed_cb(_nfc_activation_changed_cb, this);
	RETM_IF(result != NFC_ERROR_NONE, "nfc_manager_set_activation_changed_cb failed %s",get_nfc_error(result));


	result = nfc_manager_set_ndef_discovered_cb(_nfc_ndef_discovered_cb, this);
	RETM_IF(result != NFC_ERROR_NONE, "nfc_manager_set_ndef_discovered_cb failed %s",get_nfc_error(result));


	result = nfc_manager_set_p2p_target_discovered_cb(_nfc_p2p_target_discovered_cb,this);
	RETM_IF(result != NFC_ERROR_NONE, "nfc_manager_set_p2p_target_discovered_cb failed %s",get_nfc_error(result));


	result = nfc_manager_set_tag_discovered_cb(_nfc_tag_discovered_cb, this);
	RETM_IF(result != NFC_ERROR_NONE, "nfc_manager_set_tag_discovered_cb failed %s",get_nfc_error(result));


}


/**
 * @function		nfc_onoff_operation
 * @since_tizen		2.3
 * @description		Nfc Onoff Operation
 * @parameter		NA
 * @return		static int
 */
static int nfc_onoff_operation(void)
{
	int ret = 0;
	app_control_h service = NULL;
	ret = app_control_create(&service);
	RETVM_IF(ret != APP_CONTROL_ERROR_NONE, -1,  "app_control_create failed");
	if (service == NULL)
	{
	  DBG("service_create failed!");
	  return -1;
	}
	ret = app_control_set_operation(service,  "http://tizen.org/appcontrol/operation/setting/nfc");
	RETVM_IF(ret != APP_CONTROL_ERROR_NONE, -1,  "app_control_set_operation nfc failed");
	ret = app_control_add_extra_data(service,  "type", "nfc");
	RETVM_IF(ret != APP_CONTROL_ERROR_NONE, -1,  "app_control_add_extra_data nfc failed");
	ret = app_control_send_launch_request(service, NULL, NULL);
	RETVM_IF(ret != APP_CONTROL_ERROR_NONE, -1,  "app_control_send_launch_request failed");

	ret = app_control_destroy(service);
	RETVM_IF(ret != APP_CONTROL_ERROR_NONE, -1,  "app_control_destroy failed");
	if (ret == APP_CONTROL_ERROR_NONE)
	{
	  DBG("Succeeded to NFC On/Off app!");
	  return 0;
	}
	else
	{
	  DBG("Failed to relaunch NFC On/Off app!");
	  return -1;
	}
	return 0;
}


/**
 * @function		_nfc_activation_changed_cb
 * @since_tizen		2.3
 * @description		 Nfc Activation Changed Cb
 * @parameter		bool: Bool, void*: Void Pointer
 * @return		static void
 */
static void _nfc_activation_changed_cb(bool activated , void *user_data)
{
	DBG("_nfc_activation_changed_cb");
	RETM_IF(NULL == user_data, "data is NULL");

	bluetoothoob_view *this = NULL;
	this = (bluetoothoob_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");


	if (activated)
	{
	  DBG("NFC activated");
	}
	else
	{
	  DBG("NFC deactivated");
	}
}


/**
 * @function		_nfc_ndef_discovered_cb
 * @since_tizen		2.3
 * @description		 Nfc Ndef Discovered Cb
 * @parameter		nfc_ndef_message_h: Nfc Ndef Message H, void*: Void Pointer
 * @return		static void
 */
static void _nfc_ndef_discovered_cb(nfc_ndef_message_h message, void *user_data)
{
	DBG("Inside _nfc_ndef_discovered_cb");

	bluetoothoob_view *this = NULL;
	this = (bluetoothoob_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");

}


/**
 * @function		_nfc_p2p_target_discovered_cb
 * @since_tizen		2.3
 * @description		 Nfc P2p Target Discovered Cb
 * @parameter		nfc_discovered_type_e: Nfc Discovered Type E, void*: Void Pointer
 * @return		static void
 */
static void _nfc_p2p_target_discovered_cb(nfc_discovered_type_e type, nfc_p2p_target_h target, void *user_data)
{
	DBG("Inside _nfc_p2p_target_discovered_cb");
	RETM_IF(NULL == user_data, "data is NULL");

	bluetoothoob_view *this = NULL;
	this = (bluetoothoob_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");

}


/**
 * @function		create_ndef_message
 * @since_tizen		2.3
 * @description		Create Ndef Message
 * @parameter		NA
 * @return		static nfc_ndef_message_h
 */
static nfc_ndef_message_h create_ndef_message()
{

	int result;
	nfc_ndef_message_h msg;
	result = nfc_ndef_message_create(&msg);
	RETVM_IF(result != NFC_ERROR_NONE, NULL,  "nfc_ndef_message_create %s", get_nfc_error(result));
	RETVM_IF(NULL == msg, NULL,  "nfc_ndef_message_h create failed");

	bt_adapter_state_e adapter_state;

	result = bt_adapter_get_state (&adapter_state);
	RETVM_IF(result != BT_ERROR_NONE, NULL,  "bt_adapter_get_state %s", get_bt_adapter_error(result));

	if(adapter_state == BT_ADAPTER_ENABLED)
	{
		int 	hash_len;
		int  	randomizer_len;
		unsigned char* 	hash = NULL;
		unsigned char* 	randomizer = NULL;
		char* address = NULL;

		nfc_ndef_record_h record_address = NULL;
		nfc_ndef_record_h record_hash = NULL;
		nfc_ndef_record_h record_randomizer = NULL;

		result = bt_adapter_get_address(&address);
		RETVM_IF(result != BT_ERROR_NONE, NULL,  "bt_adapter_get_address %s", get_bt_adapter_error(result));

		result = bt_adapter_get_local_oob_data(&hash, &randomizer, &hash_len, &randomizer_len);
		RETVM_IF(result != BT_ERROR_NONE, NULL,  "bt_adapter_get_local_oob_data %s", get_bt_adapter_error(result));

		DBG("Return Value # %d",result);
		DBG("hash_len # %d", hash_len);
		DBG("randomizer_len # %d", randomizer_len);

		result = nfc_ndef_record_create_text(&record_address, address, "en-US", NFC_ENCODE_UTF_8);
		RETVM_IF(result != NFC_ERROR_NONE, NULL,  "nfc_ndef_record_create_text %s", get_nfc_error(result));
		result = nfc_ndef_record_create_text(&record_hash, hash, "en-US", NFC_ENCODE_UTF_8);
		RETVM_IF(result != NFC_ERROR_NONE, NULL,  "nfc_ndef_record_create_text %s", get_nfc_error(result));
		result = nfc_ndef_record_create_text(&record_randomizer, randomizer, "en-US", NFC_ENCODE_UTF_8);
		RETVM_IF(result != NFC_ERROR_NONE, NULL,  "nfc_ndef_record_create_text %s", get_nfc_error(result));

		result = nfc_ndef_message_append_record(msg, record_address);
		RETVM_IF(result != NFC_ERROR_NONE, NULL,  "nfc_ndef_message_append_record %s", get_nfc_error(result));
		result = nfc_ndef_message_append_record(msg, record_hash);
		RETVM_IF(result != NFC_ERROR_NONE, NULL,  "nfc_ndef_message_append_record %s", get_nfc_error(result));
		result = nfc_ndef_message_append_record(msg, record_randomizer);
		RETVM_IF(result != NFC_ERROR_NONE, NULL,  "nfc_ndef_message_append_record %s", get_nfc_error(result));
	}

	return msg;
}


/**
 * @function		nfc_ndef_message_read
 * @since_tizen		2.3
 * @description		Nfc Ndef Message Read
 * @parameter		nfc_ndef_message_h: Nfc Ndef Message H, void*: Void Pointer
 * @return		static void
 */
static void nfc_ndef_message_read(nfc_ndef_message_h message, void *user_data)
{
	DBG("Inside nfc_ndef_message_read");
	int result;
	int count;
	nfc_ndef_record_h record;

	RETM_IF(NULL == user_data, "data is NULL");
	RETM_IF(NULL == &message, "message is NULL");

	bluetoothoob_view *this = NULL;
	this = (bluetoothoob_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");

	char* address = NULL;
	unsigned char* 	hash = NULL;
	unsigned char* 	randomizer = NULL;

	result = nfc_ndef_message_get_record(message, 0, &record);
	RETM_IF(result != NFC_ERROR_NONE, "nfc_ndef_message_get_record: %s", get_nfc_error(result));

	result = nfc_ndef_record_get_text(record, &address);
	RETM_IF(result != NFC_ERROR_NONE, "nfc_ndef_record_get_text: %s", get_nfc_error(result));

	result = nfc_ndef_message_get_record(message, 1, &record);
	RETM_IF(result != NFC_ERROR_NONE, "nfc_ndef_message_get_record: %s", get_nfc_error(result));

	result = nfc_ndef_record_get_text(record, &hash);
	RETM_IF(result != NFC_ERROR_NONE, "nfc_ndef_record_get_text: %s", get_nfc_error(result));

	result = nfc_ndef_message_get_record(message, 2, &record);
	RETM_IF(result != NFC_ERROR_NONE, "nfc_ndef_message_get_record: %s", get_nfc_error(result));

	result = nfc_ndef_record_get_text(record, &randomizer);
	RETM_IF(result != NFC_ERROR_NONE, "nfc_ndef_record_get_text: %s", get_nfc_error(result));

	this->remote_address = address;
	this->remote_hash = hash;
	this->remote_randomizer = randomizer;

}


/**
 * @function		_nfc_tag_discovered_cb
 * @since_tizen		2.3
 * @description		 Nfc Tag Discovered Cb
 * @parameter		nfc_discovered_type_e: Nfc Discovered Type E, void*: Void Pointer
 * @return		static void
 */
static void _nfc_tag_discovered_cb(nfc_discovered_type_e type, nfc_tag_h tag, void *user_data)
{
	DBG("Inside _nfc_tag_discovered_cb");
	int result;

	RETM_IF(NULL == user_data, "data is NULL");
	RETM_IF(NULL == tag, "tag is NULL");

	bluetoothoob_view *this = NULL;
	this = (bluetoothoob_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");

	if(this->view->tbt_info->apptype == TBT_APP_BT_OOB_DEVICE)
	{
		//read
		result = nfc_tag_read_ndef(tag, _nfc_tag_read_completed_cb, this);
		RETM_IF(result != NFC_ERROR_NONE, "nfc_tag_read_ndef: %s", get_nfc_error(result));

	}

	else if(this->view->tbt_info->apptype == TBT_APP_BT_OOB_PERIPHERAL)
	{
		//write
		nfc_ndef_message_h info_msg;
		info_msg = create_ndef_message();

		result = nfc_tag_write_ndef(tag, info_msg, _tag_write_completed_cb, this);
		RETM_IF(result != NFC_ERROR_NONE, "nfc_tag_write_ndef: %s", get_nfc_error(result));
	}

}


/**
 * @function		_tag_write_completed_cb
 * @since_tizen		2.3
 * @description		 Tag Write Completed Cb
 * @parameter		nfc_error_e: Nfc Error E, void*: Void Pointer
 * @return		static void
 */
static void _tag_write_completed_cb(nfc_error_e result, void *user_data)
{
	DBG("_tag_write_completed_cb %s", get_nfc_error(result));
	RETM_IF(NULL == user_data, "data is NULL");

	bluetoothoob_view *this = NULL;
	this = (bluetoothoob_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");

	if(result == NFC_ERROR_NONE)
	{
		DBG("Write Completed Successfully");
	}
	else
	{
		DBG("Write Error");
	}
}


/**
 * @function		_nfc_tag_read_completed_cb
 * @since_tizen		2.3
 * @description		 Nfc Tag Read Completed Cb
 * @parameter		nfc_error_e: Nfc Error E, void*: Void Pointer
 * @return		static void
 */
static void _nfc_tag_read_completed_cb(nfc_error_e result, nfc_ndef_message_h message, void *user_data)
{
	DBG("_nfc_tag_read_completed_cb");
	RETM_IF(NULL == user_data, "data is NULL");
	RETM_IF(NULL == message, "message is NULL");

	bluetoothoob_view *this = NULL;
	this = (bluetoothoob_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");

	nfc_ndef_message_read(message, this);
}


/**
 * @function		_adapter_device_bond_created_cb
 * @since_tizen		2.3
 * @description		 Adapter Device Bond Created Cb
 * @parameter		int: Int, bt_device_info_s*: Bt Device Info S Pointer, void*: Void Pointer
 * @return		static void
 */
static void _adapter_device_bond_created_cb(int result, bt_device_info_s *device_info, void *data)
{
	DBG("_adapter_device_bond_created_cb %s", get_bt_adapter_error(result));
	RETM_IF(NULL == data, "data is NULL");

	bluetoothoob_view *this = NULL;
	this = (bluetoothoob_view*)data;
	RETM_IF(NULL == this, "view is NULL");

	if(result == BT_ERROR_NONE)
	{
		DBG("Bond Test Created Successfully");
	}
}

#endif
