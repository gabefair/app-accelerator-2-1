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
 * @file	tbt-bluetooth-view.c
 * @brief	performs all the bluetooth related operations
 *			performs operations related to bluetooth adapter, bonding and pairing
 *
 * @bug     none
 * @credit
 *
 */

#include "utils/app_module_config.h"
#ifdef TBT_MODULE_BT

#include <glib.h>
#include <bluetooth.h>
#include <player.h>
#include <Elementary.h>

#include "utils/logger.h"
#include "utils/config.h"
#include "utils/ui-utils.h"
#include "utils/util_state_object.h"
#include "view/tbt-bluetooth-view.h"
#include "view/tbt-common-view.h"

#define IS_ON_OFF_APP_AVAILABLE false
#define BT_PUSH_FILE_NAME "dial.png"




typedef enum{
	BT_DEACTIVATED=0,
	BT_ACTIVATED,
	BT_ACTIVATED_CLIENT,
	BT_ACTIVATION_ATTEMPTED,
	BT_ACTIVATION_BY_APP_FAILED,
	BT_DISCOVERY_START_REQUESTED,
	BT_PAIR_REQUEST_ATTEMPTED,
	BT_UNPAIR_REQUEST_ATTEMPTED,
	BT_BOND_CANCELLED,
	BT_BOND_AUTH_FAILED,
	BT_BOND_TIMED_OUT,
	BT_BOND_REMOTE_DEVICE_NOT_FOUND,
	BT_DISCOVERED_DEVICE_CLICKED,
	BT_BOND_CLIENT_SUCCESSFULLY_CREATED,
	BT_SUCCESSFULLY_UNPAIRED,

} bt_state_machine_states;

typedef enum{
	BT_ON_ATTEMPT=0,
	BT_APP_CONTROL_SUCCESSFULL_ACTIVATION,
	BT_APP_CONTROL_ACTIVATION_FAILURE,
	BT_ADAPTER_ON,
	BT_ADAPTER_ON_CLIENT,
	BT_START_DISCOVERY_REQUEST,
	BT_SELECT_DISCOVERED_DEVICE,
	BT_ADAPTER_ATTEMP_PAIR,
	BT_ADAPTER_ATTEMP_UNPAIR,
	BT_BOND_SUCCESS,
	BT_BOND_AUTH_FAIL,
	BT_BOND_TIME_OUT,
	BT_BOND_DEVICE_LOST,
	BT_BOND_CANCEL,
	BT_UNPAIR_SUCCESS,

} bt_state_machine_feed_values;

struct _bluetooth_view
{
	common_view* view;

	GList *devices_list;
	GList *bonded_devices_list;
	GList *selected_device_profile_list;
	GList *service_list;

    Evas_Object *action_btn;
	Evas_Object *bluetooth_btn;
    Evas_Object *bluetooth_list;
    Evas_Object *bluetooth_label;
    Evas_Object *bluetooth_check;
    Evas_Object *bluetooth_check_label;
    Evas_Object *bluetooth_check_button;

	/* LHtest */
	Evas_Object *popup;
	int popup_state;

    Elm_Object_Item *selected_device_item;

    util_state_object *state_object_bt_server_pair, *state_object_bt_client_pair;

    bt_adapter_state_e adapter_state;
    bt_adapter_visibility_mode_e visibility_mode;
    bt_adapter_device_discovery_state_e discovery_state;
    bool is_discovering;

    bt_adapter_device_discovery_info_s  *selected_device_info;
    int no_of_profiles;

    int server_socket_fd;
    bool rfcomm_created;
    bool rfcomm_client_connected;
    int opp_transfer_id;
    bool is_opp_server_accecpt;
    bool is_opp_server_push_progress;
    bool is_opp_push_cancelled;
    char* hdp_app_id;
    unsigned int hdp_channel;
    bool hdp_send_data_flag;

    player_h bt_player;
};

#define BT_ADAPTER_DEVICE_DISCOVERY_NONE -1

bool is_bluetooth_app_exist = false;

static void adapter_device_error(int result);
static int adapter_on_off_request_send(bluetooth_view *this);
static int send_rfcomm_send_client_data(bluetooth_view *this);
static int create_rfcomm_server(bluetooth_view *this);
static Evas_Object *add_control_layout(bluetooth_view *this, Evas_Object *parent);
static void update_view_controls(bluetooth_view *this);
static void discovered_devices_list_show(bluetooth_view *this);
static bool is_new_device_found(bluetooth_view *this, bt_adapter_device_discovery_info_s *discovery_info);
static void set_connected_profiles(bluetooth_view *this);
static int update_bonded_devices(bluetooth_view *this);

static gint bluetooth_list_find_func_cb(gconstpointer a, gconstpointer b);
static void bluetooth_list_free_func_cb(gpointer data);

static void _app_destroy_cb(void* this);
static void _device_item_selected_cb(void *data, Evas_Object *obj, void *event_info);

static void _control_button_pressed_cb(void *data, Evas_Object *obj, void *event_info);
static void _control_button_bluetooth_pressed_cb(void *data, Evas_Object *obj, void *event_info);
static void _cancel_button_pressed_cb(void *data, Evas_Object *obj, void *event_info);

static void _adapter_state_changed_cb(int result, bt_adapter_state_e adapter_state, void* data);
static void _adapter_device_discovery_state_changed_cb(int result, bt_adapter_device_discovery_state_e discovery_state, bt_adapter_device_discovery_info_s *discovery_info, void* data);
static void _adapter_device_bond_destroyed_cb(int result, char *remote_address, void *data);
static void _adapter_device_bond_created_cb(int result, bt_device_info_s *device_info, void *data);
static void _adapter_socket_data_received_cb(bt_socket_received_data_s *socket_data, void *data);
static void _adapter_socket_connection_state_changed_cb(int result, bt_socket_connection_state_e connection_state, bt_socket_connection_s *connection, void *data);
static bool _adapter_bonded_device_cb(bt_device_info_s *device_info, void *data);
static void _adapter_visibility_mode_changed_cb(int result, bt_adapter_visibility_mode_e visibility_mode, void *data);
static void _adapter_authorization_changed_cb(bt_device_authorization_e authorization, char *remote_address, void *data);
static void _adapter_visibility_duration_changed_cb(int duration, void *data);
static void _adapter_opp_client_push_finished_cb(int result, const char *remote_address, void *data);
static void _adapter_opp_client_push_responded_cb(int result, const char *remote_address, void *data);
static void _adapter_opp_client_push_progress_cb(const char *file, long long size, int percent, void *data);
static bool _adapter_device_connected_profile_cb(bt_profile_e profile, void *data);
static void _adapter_bt_audio_connection_state_changed_cb(int result, bool connected, const char *remote_address, bt_audio_profile_type_e type, void *user_data);
static void _adapter_bt_device_service_searched_cb(int result, bt_device_sdp_info_s *sdp_info, void *user_data);
static void _adpter_bt_opp_server_connection_requested_cb(const char *remote_address, void *user_data);
static void _adpter_bt_opp_server_transfer_progress_cb_for_opp(const char *file, long long size, int percent, void *user_data);
static void _adapter_bt_opp_server_transfer_finished_cb_for_opp(int result, const char *file, long long size, void *user_data);

static GList* bt_get_service_name(bt_service_class_t service_enum);
static char* get_bluetooth_error(bt_error_e error);

static char* get_bt_profile_name(bt_profile_e profile);

static void setup_state_machine(bluetooth_view *this);
static void _bt_util_state_object_state_changed_cb_server_pair(int previous,int now,int feed,void *user_data,void *feed_data);
static void _bt_util_state_object_state_changed_cb_client_pair(int previous,int now,int feed,void *user_data,void *feed_data);
static void do_bt_state_common_check(bluetooth_view *this, int now, void *feed_data);
static void do_bt_common_feed(bluetooth_view *this, int feed_value, void* feed_data);
static void _bt_hid_host_connection_state_changed_cb(int result, bool connected, const char *remote_address, void *user_data);
static void _bt_avrcp_target_connection_state_changed_cb(bool connected, const char *remote_address, void *user_data);
static void _bt_avrcp_equalizer_state_changed_cb(bt_avrcp_equalizer_state_e equalizer, void *user_data);
static void _bt_avrcp_repeat_mode_changed_cb(bt_avrcp_repeat_mode_e repeat, void *user_data);
static void _bt_avrcp_shuffle_mode_changed_cb(bt_avrcp_shuffle_mode_e shuffle, void *user_data);
static void _bt_avrcp_scan_mode_changed_cb(bt_avrcp_scan_mode_e scan, void *user_data);

static void _bt_hdp_connected_cb(int result, const char *remote_address, const char *app_id,bt_hdp_channel_type_e type, unsigned int channel, void *user_data);
static void _bt_hdp_disconnected_cb(int result, const char *remote_address, unsigned int channel, void *user_data);
static void _bt_hdp_data_received_cb(unsigned int channel, const char *data, unsigned int size, void *user_data);


/**
 * @function		bluetooth_view_add
 * @since_tizen		2.3
 * @description		Bluetooth View Add
 * @parameter		Evas_Object*: Evas Object Pointer, tbt_info*: Tbt Info Pointer, Elm_Object_Item*: Elm Object Item Pointer
 * @return		bluetooth_view*
 */
bluetooth_view *bluetooth_view_add(Evas_Object *navi, tbt_info *tbt_info, Elm_Object_Item *item)
{
	RETVM_IF(NULL == navi, NULL, "navi is null");
	RETVM_IF(NULL == tbt_info, NULL, "tbt_info is null");

	int result;
    bluetooth_view *this = NULL;
    this = calloc(1, sizeof(bluetooth_view));
    RETVM_IF(!this, NULL, "calloc failed");

    this->view = calloc(1, sizeof(common_view));
    RETVM_IF(!this->view, NULL, "calloc failed");

    this->devices_list = NULL;
    this->bonded_devices_list = NULL;
    this->selected_device_profile_list = NULL;
    this->service_list = NULL;
    this->discovery_state = BT_ADAPTER_DEVICE_DISCOVERY_NONE;
    this->rfcomm_created = false;
    this->rfcomm_client_connected = false;
    this->no_of_profiles = 0;
    this->hdp_app_id = NULL;
    this->hdp_channel = 0;
    this->hdp_send_data_flag = false;
    this->is_opp_push_cancelled = false;

    #ifdef DEVICE_TYPE_WEARABLE
		if(get_device_type() == DEVICE_WEARABLE_320_X_320)
		{
			elm_theme_overlay_add(NULL, get_edje_path("custom_button.edj"));
		}
	#endif

    if(tbt_info->apptype == TBT_APP_BT_CLIENT_PAIRED || tbt_info->apptype == TBT_APP_BT_OPP_CLIENT)
    {
    	tbt_info->layout_group = "bluetooth_viewer_cancel_check";
    }
    else
    {
    	tbt_info->layout_group = "bluetooth_viewer";
    }
    tbt_info->layout_file = get_edje_path("bluetooth_viewer.edj");


    common_view_add(navi, tbt_info, item, this->view, _app_destroy_cb, this);
    RETVM_IF(NULL == this->view, NULL, "view is null");

    this->bluetooth_label = ui_utils_label_add(this->view->layout, "Bluetooth");
	elm_object_part_content_set(this->view->layout, "bluetooth_text", this->bluetooth_label);

	this->bluetooth_list = elm_list_add(this->view->layout);
	RETVM_IF(!this->bluetooth_list, NULL, "elm_list_add failed");
	evas_object_data_set(this->bluetooth_list, "bluetooth_view", this);
	elm_object_part_content_set(this->view->layout, "bluetooth_list", this->bluetooth_list);

	if(strcmp(tbt_info->layout_group, "bluetooth_viewer_cancel_check") == 0)
	{
		Evas_Object *table = elm_table_add(this->view->layout);
		if(tbt_info->apptype == TBT_APP_BT_CLIENT_PAIRED)
		{
			this->bluetooth_check_label = ui_utils_label_add(table, "Cancel Pair");
			this->bluetooth_check = ui_utils_check_add(this, table);
			elm_table_pack(table, this->bluetooth_check_label, 1, 0, 1, 1);
			elm_table_pack(table, this->bluetooth_check, 2, 0, 1, 1);
			elm_check_state_set (this->bluetooth_check, EINA_FALSE);


		}
		else if(tbt_info->apptype == TBT_APP_BT_OPP_CLIENT)
		{
			DBG("CANCEL BUTTON!!");
			this->bluetooth_check_button = ui_utils_push_button_add(this, table, "Cancel Transfer", _cancel_button_pressed_cb);
			elm_table_pack(table, this->bluetooth_check_button, 1, 0, 1, 1);
			elm_object_disabled_set(this->bluetooth_check_button, EINA_TRUE);

		}
		elm_object_part_content_set(this->view->layout, "check_part", table);
		evas_object_show(table);
	}


	Evas_Object *control = add_control_layout(this, this->view->layout);
	elm_object_part_content_set(this->view->layout, "controlr_part", control);

	setup_state_machine(this);

	bt_initialize();

	//Check whether the Bluetooth Service is enabled
	result = bt_adapter_get_state(&this->adapter_state);
	RETVM_IF(result != BT_ERROR_NONE, NULL, "bt_adapter_get_state Failed");

	result = bt_adapter_set_state_changed_cb(_adapter_state_changed_cb , this);
	RETVM_IF(result != BT_ERROR_NONE, NULL, "bt_adapter_set_state_changed_cb Failed");

	result = bt_adapter_set_device_discovery_state_changed_cb(_adapter_device_discovery_state_changed_cb, this);
	RETVM_IF(result != BT_ERROR_NONE, NULL, "bt_adapter_set_device_discovery_state_changed_cb Failed");

	result = bt_device_set_bond_created_cb(_adapter_device_bond_created_cb, this);
	RETVM_IF(result != BT_ERROR_NONE, NULL, "bt_device_set_bond_created_cb Failed");

	result = bt_device_set_bond_destroyed_cb(_adapter_device_bond_destroyed_cb, this);
	RETVM_IF(result != BT_ERROR_NONE, NULL, "bt_device_set_bond_destroyed_cb Failed");

	result = bt_adapter_set_visibility_mode_changed_cb(_adapter_visibility_mode_changed_cb, this);
	RETVM_IF(result != BT_ERROR_NONE, NULL, "bt_adapter_set_visibility_mode_changed_cb Failed");

	result = bt_adapter_set_visibility_duration_changed_cb(_adapter_visibility_duration_changed_cb, this);
	RETVM_IF(result != BT_ERROR_NONE, NULL, "bt_adapter_set_visibility_duration_changed_cb Failed");

	result = bt_device_set_authorization_changed_cb (_adapter_authorization_changed_cb, this);
	RETVM_IF(result != BT_ERROR_NONE, NULL, "bt_device_set_authorization_changed_cb Failed");

	result = bt_socket_set_data_received_cb(_adapter_socket_data_received_cb, this);
	RETVM_IF(result != BT_ERROR_NONE, NULL, "bt_device_set_authorization_changed_cb Failed");

	result = bt_socket_set_connection_state_changed_cb(_adapter_socket_connection_state_changed_cb, this);
	RETVM_IF(result != BT_ERROR_NONE, NULL, "bt_socket_set_connection_state_changed_cb Failed");



	if(this->view->tbt_info->apptype == TBT_APP_BT_HDP)
	{
		result = bt_hdp_set_connection_state_changed_cb(_bt_hdp_connected_cb, _bt_hdp_disconnected_cb, this);
		RETVM_IF(result != BT_ERROR_NONE, NULL, "bt_hdp_set_connection_state_changed_cb error: %s", get_bluetooth_error(result));
		result = bt_hdp_set_data_received_cb(_bt_hdp_data_received_cb, this);
		RETVM_IF(result != BT_ERROR_NONE, NULL, "bt_hdp_set_data_received_cb error: %s", get_bluetooth_error(result));
	}
	else if(this->view->tbt_info->apptype == TBT_APP_BT_SDP)
	{
		result = bt_device_set_service_searched_cb(_adapter_bt_device_service_searched_cb, this);
		RETVM_IF(result != BT_ERROR_NONE, NULL, "bt_device_set_service_searched_cb Failed");
	}

	update_view_controls(this);

	return this;
}


/**
 * @function		setup_state_machine
 * @since_tizen		2.3
 * @description		Setup State Machine
 * @parameter		bluetooth_view*: Bluetooth View Pointer
 * @return		static void
 */
static void setup_state_machine(bluetooth_view *this)
{
	RETM_IF(NULL == this, "setup_state_machine Failed");

	if(this->view->tbt_info->apptype == TBT_APP_BT_SERVER_PAIRED)
	{
		this->state_object_bt_server_pair = util_state_object_create("bt_server_pair");

		util_state_object_add_state(this->state_object_bt_server_pair,"BT_DEACTIVATED",BT_DEACTIVATED);
		util_state_object_add_state(this->state_object_bt_server_pair,"BT_ACTIVATION_ATTEMPTED",BT_ACTIVATION_ATTEMPTED);
		util_state_object_add_state(this->state_object_bt_server_pair,"BT_ACTIVATED",BT_ACTIVATED);
		util_state_object_add_state(this->state_object_bt_server_pair,"BT_ACTIVATION_BY_APP_FAILED",BT_ACTIVATION_BY_APP_FAILED);

		util_state_object_add_transition(this->state_object_bt_server_pair,BT_DEACTIVATED, BT_ON_ATTEMPT, BT_ACTIVATION_ATTEMPTED);
		util_state_object_add_transition(this->state_object_bt_server_pair,BT_DEACTIVATED, BT_ADAPTER_ON, BT_ACTIVATED);
		util_state_object_add_transition(this->state_object_bt_server_pair,BT_ACTIVATION_ATTEMPTED, BT_APP_CONTROL_ACTIVATION_FAILURE, BT_ACTIVATION_BY_APP_FAILED);
		util_state_object_add_transition(this->state_object_bt_server_pair,BT_ACTIVATION_BY_APP_FAILED, BT_ADAPTER_ON, BT_ACTIVATED);
		util_state_object_add_transition(this->state_object_bt_server_pair,BT_ACTIVATION_ATTEMPTED, BT_APP_CONTROL_SUCCESSFULL_ACTIVATION, BT_ACTIVATED);

		util_state_object_init(this->state_object_bt_server_pair, BT_DEACTIVATED,_bt_util_state_object_state_changed_cb_server_pair,this);
		util_state_object_info(this->state_object_bt_server_pair);
	}

	if(this->view->tbt_info->apptype == TBT_APP_BT_OPP_CLIENT ||
	this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT ||
	this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT_HANDSFREE ||
	this->view->tbt_info->apptype == TBT_APP_BT_AUTHORIZATION ||
	this->view->tbt_info->apptype == TBT_APP_BT_PROFILES ||
	this->view->tbt_info->apptype == TBT_APP_BT_CLIENT_SOCKET ||
	this->view->tbt_info->apptype == TBT_APP_BT_CLIENT_PAIRED ||
	this->view->tbt_info->apptype == TBT_APP_BT_SDP ||
	this->view->tbt_info->apptype == TBT_APP_BT_HID_CONNECT ||
	this->view->tbt_info->apptype == TBT_APP_BT_AVRCP ||
	this->view->tbt_info->apptype == TBT_APP_BT_AG ||
	this->view->tbt_info->apptype == TBT_APP_BT_HDP
	)
	{
		this->state_object_bt_client_pair = util_state_object_create("bt_client_pair");

		util_state_object_add_state(this->state_object_bt_client_pair,"BT_DEACTIVATED",BT_DEACTIVATED);
		util_state_object_add_state(this->state_object_bt_client_pair,"BT_ACTIVATION_ATTEMPTED",BT_ACTIVATION_ATTEMPTED);
		util_state_object_add_state(this->state_object_bt_client_pair,"BT_ACTIVATED_CLIENT",BT_ACTIVATED_CLIENT);
		util_state_object_add_state(this->state_object_bt_client_pair,"BT_ACTIVATION_BY_APP_FAILED",BT_ACTIVATION_BY_APP_FAILED);
		util_state_object_add_state(this->state_object_bt_client_pair,"BT_DISCOVERY_START_REQUESTED",BT_DISCOVERY_START_REQUESTED);
		util_state_object_add_state(this->state_object_bt_client_pair,"BT_PAIR_REQUEST_ATTEMPTED",BT_PAIR_REQUEST_ATTEMPTED);
		util_state_object_add_state(this->state_object_bt_client_pair,"BT_UNPAIR_REQUEST_ATTEMPTED",BT_UNPAIR_REQUEST_ATTEMPTED);
		util_state_object_add_state(this->state_object_bt_client_pair,"BT_DISCOVERED_DEVICE_CLICKED",BT_DISCOVERED_DEVICE_CLICKED);
		util_state_object_add_state(this->state_object_bt_client_pair,"BT_SUCCESSFULLY_UNPAIRED",BT_SUCCESSFULLY_UNPAIRED);

		util_state_object_add_state(this->state_object_bt_client_pair,"BT_BOND_SUCCESSFULLY_CREATED",BT_BOND_CLIENT_SUCCESSFULLY_CREATED);
		util_state_object_add_state(this->state_object_bt_client_pair,"BT_BOND_ERROR_CANCELLED",BT_BOND_CANCELLED);
		util_state_object_add_state(this->state_object_bt_client_pair,"BT_BOND_ERROR_AUTH_FAILED",BT_BOND_AUTH_FAILED);
		util_state_object_add_state(this->state_object_bt_client_pair,"BT_BOND_ERROR_TIMED_OUT",BT_BOND_TIMED_OUT);
		util_state_object_add_state(this->state_object_bt_client_pair,"BT_BOND_ERROR_REMOTE_DEVICE_NOT_FOUND",BT_BOND_REMOTE_DEVICE_NOT_FOUND);

		util_state_object_add_transition(this->state_object_bt_client_pair,BT_DEACTIVATED, BT_ON_ATTEMPT, BT_ACTIVATION_ATTEMPTED);
		util_state_object_add_transition(this->state_object_bt_client_pair,BT_DEACTIVATED, BT_ADAPTER_ON_CLIENT, BT_ACTIVATED_CLIENT);
		util_state_object_add_transition(this->state_object_bt_client_pair,BT_ACTIVATION_ATTEMPTED, BT_APP_CONTROL_ACTIVATION_FAILURE, BT_ACTIVATION_BY_APP_FAILED);
		util_state_object_add_transition(this->state_object_bt_client_pair,BT_ACTIVATION_BY_APP_FAILED, BT_ADAPTER_ON_CLIENT, BT_ACTIVATED_CLIENT);
		util_state_object_add_transition(this->state_object_bt_client_pair,BT_ACTIVATION_ATTEMPTED, BT_APP_CONTROL_SUCCESSFULL_ACTIVATION, BT_ACTIVATED_CLIENT);
		util_state_object_add_transition(this->state_object_bt_client_pair,BT_ACTIVATED_CLIENT, BT_START_DISCOVERY_REQUEST, BT_DISCOVERY_START_REQUESTED);
		util_state_object_add_transition(this->state_object_bt_client_pair,BT_DISCOVERY_START_REQUESTED, BT_SELECT_DISCOVERED_DEVICE, BT_DISCOVERED_DEVICE_CLICKED);

		util_state_object_add_transition(this->state_object_bt_client_pair,BT_DISCOVERED_DEVICE_CLICKED, BT_ADAPTER_ATTEMP_PAIR, BT_PAIR_REQUEST_ATTEMPTED);
		util_state_object_add_transition(this->state_object_bt_client_pair,BT_DISCOVERED_DEVICE_CLICKED, BT_ADAPTER_ATTEMP_UNPAIR, BT_UNPAIR_REQUEST_ATTEMPTED);

		util_state_object_add_transition(this->state_object_bt_client_pair,BT_PAIR_REQUEST_ATTEMPTED, BT_BOND_SUCCESS, BT_BOND_CLIENT_SUCCESSFULLY_CREATED);
		util_state_object_add_transition(this->state_object_bt_client_pair,BT_PAIR_REQUEST_ATTEMPTED, BT_BOND_CANCEL, BT_BOND_CANCELLED);
		util_state_object_add_transition(this->state_object_bt_client_pair,BT_PAIR_REQUEST_ATTEMPTED, BT_BOND_AUTH_FAIL, BT_BOND_AUTH_FAILED);
		util_state_object_add_transition(this->state_object_bt_client_pair,BT_PAIR_REQUEST_ATTEMPTED, BT_BOND_TIME_OUT, BT_BOND_TIMED_OUT);
		util_state_object_add_transition(this->state_object_bt_client_pair,BT_PAIR_REQUEST_ATTEMPTED, BT_BOND_DEVICE_LOST, BT_BOND_REMOTE_DEVICE_NOT_FOUND);

		util_state_object_add_transition(this->state_object_bt_client_pair,BT_BOND_CLIENT_SUCCESSFULLY_CREATED, BT_SELECT_DISCOVERED_DEVICE, BT_DISCOVERED_DEVICE_CLICKED);
		util_state_object_add_transition(this->state_object_bt_client_pair,BT_UNPAIR_REQUEST_ATTEMPTED, BT_UNPAIR_SUCCESS, BT_SUCCESSFULLY_UNPAIRED);

		util_state_object_add_transition(this->state_object_bt_client_pair,BT_SUCCESSFULLY_UNPAIRED, BT_SELECT_DISCOVERED_DEVICE, BT_DISCOVERED_DEVICE_CLICKED);


		util_state_object_init(this->state_object_bt_client_pair, BT_DEACTIVATED,_bt_util_state_object_state_changed_cb_client_pair,this);
		util_state_object_info(this->state_object_bt_client_pair);
	}

}


/**
 * @function		update_view_controls
 * @since_tizen		2.3
 * @description		Update View Controls
 * @parameter		bluetooth_view*: Bluetooth View Pointer
 * @return		static void
 */
static void update_view_controls(bluetooth_view *this)
{
	DBG("update_view_controls");
	RETM_IF(NULL == this, "update_view_controls Failed");
	int result;

	//Check whether the Bluetooth Service is enabled
	result = bt_adapter_get_state(&this->adapter_state);
	RETM_IF(result != BT_ERROR_NONE, "bt_adapter_get_state fail > Error = %d", result);

	//If the Bluetooth Service is not enabled
	if(this->adapter_state == BT_ADAPTER_DISABLED)
	{
			DBG("Adapter state Disabled");
			this->discovery_state = BT_ADAPTER_DEVICE_DISCOVERY_NONE;
			elm_object_disabled_set(this->action_btn, EINA_TRUE);
			elm_object_disabled_set(this->bluetooth_btn, EINA_FALSE);
			elm_object_text_set(this->bluetooth_btn, "Bluetooth On");
			ui_utils_label_set_text(this->bluetooth_label, "Bluetooth is Switched Off", "left");
	}
	else if(this->adapter_state == BT_ADAPTER_ENABLED)
	{
		if(this->state_object_bt_client_pair)
		{
			if(this->discovery_state == BT_ADAPTER_DEVICE_DISCOVERY_STARTED)
			{
				if(this->view->tbt_info->apptype == TBT_APP_BT_OPP_CLIENT ||
				   this->view->tbt_info->apptype == TBT_APP_BT_AUTHORIZATION ||
				   this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT ||
				   this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT_HANDSFREE ||
				   this->view->tbt_info->apptype == TBT_APP_BT_HID_CONNECT ||
				   this->view->tbt_info->apptype == TBT_APP_BT_AVRCP ||
				   this->view->tbt_info->apptype == TBT_APP_BT_SDP ||
				   this->view->tbt_info->apptype == TBT_APP_BT_AG ||
				   this->view->tbt_info->apptype == TBT_APP_BT_HDP
				   )

				{
					elm_object_disabled_set(this->action_btn, EINA_TRUE);
				}

				elm_object_disabled_set(this->bluetooth_btn, EINA_TRUE);
				elm_object_text_set(this->bluetooth_btn, "Discover");
				ui_utils_label_set_text(this->bluetooth_label, "Discovery Started...", "left");
				discovered_devices_list_show(this);
			}
			else if(this->discovery_state == BT_ADAPTER_DEVICE_DISCOVERY_FOUND)
			{
				elm_object_disabled_set(this->bluetooth_btn, EINA_TRUE);
				elm_object_text_set(this->bluetooth_btn, "Discover");
				ui_utils_label_set_text(this->bluetooth_label, "Discovery Found...", "left");
				discovered_devices_list_show(this);
			}
			else if(this->discovery_state == BT_ADAPTER_DEVICE_DISCOVERY_FINISHED)
			{
//				result = bt_adapter_stop_device_discovery();
//				RETM_IF(result != BT_ERROR_NONE, "bt_adapter_stop_device_discovery fail > Error = %s", get_bluetooth_error(result));
				this->discovery_state = BT_ADAPTER_DEVICE_DISCOVERY_NONE;

				elm_object_disabled_set(this->bluetooth_btn, EINA_FALSE);
				elm_object_text_set(this->bluetooth_btn, "Discover");
				ui_utils_label_set_text(this->bluetooth_label, "Discovery Finished...", "left");
			}
			else
			{
				DBG("I am here !! and current state is %d", util_state_object_current_state(this->state_object_bt_client_pair));
				do_bt_common_feed(this, BT_ADAPTER_ON_CLIENT, NULL);
				return;
			}
		}
		else if(this->view->tbt_info->apptype == TBT_APP_BT_SERVER_PAIRED ||
				this->view->tbt_info->apptype == TBT_APP_BT_SERVER_SOCKET ||
				this->view->tbt_info->apptype == TBT_APP_BT_OPP_SERVER)
		{
			int duration;
			bt_adapter_visibility_mode_e mode;

			result = bt_adapter_get_visibility(&mode, &duration);
			DBG("bt_adapter_get_visibility visibility duration %d", duration);
			RETM_IF(result != BT_ERROR_NONE, "bt_adapter_get_visibility fail > Error = %d", result);

				if(mode == BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE )
				{
					if(this->is_opp_server_accecpt == EINA_FALSE)
					{
						elm_object_disabled_set(this->bluetooth_btn, TRUE);
						elm_object_text_set(this->bluetooth_btn, "Set Visible");
						ui_utils_label_set_text(this->bluetooth_label, "Device is not visible", "left");
					}
				}
				else
				{
					if(this->is_opp_server_accecpt == EINA_FALSE)
					{
						elm_object_disabled_set(this->bluetooth_btn, EINA_TRUE);
						elm_object_text_set(this->bluetooth_btn, "Set Visible");
						char *local_name;
						result = bt_adapter_get_name(&local_name);
						RETM_IF(result != APP_ERROR_NONE, "bt_adapter_get_name fail > Error = %d", result);
						char* str = format_string("'%s' is visible",local_name);
						ui_utils_label_set_text(this->bluetooth_label, str, "left");
						SAFE_DELETE(str);
						SAFE_DELETE(local_name);
					}
				}

			if(this->view->tbt_info->apptype == TBT_APP_BT_SERVER_SOCKET)
			{
				DBG("TBT_APP_BT_SERVER_SOCKET");
				result = create_rfcomm_server(this);
				RETM_IF(result != BT_ERROR_NONE,"create_rfcomm_server fail > Error = %d", result);
			}
			else if(this->view->tbt_info->apptype == TBT_APP_BT_OPP_SERVER && this->is_opp_server_accecpt == EINA_FALSE)
			{
				DBG("TBT_APP_BT_OPP_SERVER");
				result = bt_opp_server_initialize_by_connection_request(get_storage_image_dir_path(), _adpter_bt_opp_server_connection_requested_cb,this);
				DBG("bt_opp_server_initialize_by_connection_request %s %s ", get_bluetooth_error(result), app_get_resource_path());
				RETM_IF(result != BT_ERROR_NONE, "[bt_opp_server_initialize_by_connection_request] Failed. > Error = %s", get_bluetooth_error(result));
			}
		}
		else if(this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT ||
				this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT_HANDSFREE ||
				this->view->tbt_info->apptype == TBT_APP_BT_HID_CONNECT ||
				this->view->tbt_info->apptype == TBT_APP_BT_AVRCP ||
				this->view->tbt_info->apptype == TBT_APP_BT_AG ||
				this->view->tbt_info->apptype == TBT_APP_BT_SDP ||
				this->view->tbt_info->apptype == TBT_APP_BT_HDP)
		{
			 elm_object_text_set(this->bluetooth_btn, "Bluetooth Off");
			 ui_utils_label_set_text(this->bluetooth_label, "Bluetooth is Switched On", "left");
		}
		else if(this->view->tbt_info->apptype == TBT_APP_BT_AUTHORIZATION)
		{
			 elm_object_text_set(this->bluetooth_btn, "Bluetooth Off");
			 ui_utils_label_set_text(this->bluetooth_label, "Bluetooth is Switched On", "left");
		}
	}
}


/**
 * @function		add_control_layout
 * @since_tizen		2.3
 * @description		Add Control Layout
 * @parameter		bluetooth_view*: Bluetooth View Pointer, Evas_Object*: Evas Object Pointer
 * @return		static Evas_Object*
 */
static Evas_Object *add_control_layout(bluetooth_view *this, Evas_Object *parent)
{
	DBG("add_control_layout__");

	RETVM_IF(NULL == this, NULL, "view is NULL");
	RETVM_IF(NULL == parent, NULL, "parent is NULL");

    Evas_Object *table = elm_table_add(parent);

    evas_object_size_hint_weight_set(table, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(table, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(table);


    this->bluetooth_btn = ui_utils_push_button_add(this, table, "Bluetooth", _control_button_bluetooth_pressed_cb);
    RETVM_IF(NULL == this->bluetooth_btn, NULL, "this->bluetooth_btn is NULL");
    elm_table_pack(table, this->bluetooth_btn, 0, 0, 1, 1);

    if(this->view->tbt_info->apptype == TBT_APP_BT_OPP_CLIENT)
	{
    	this->action_btn = ui_utils_push_button_add(this, table, "Send File", _control_button_pressed_cb);
    	RETVM_IF(NULL == this->action_btn, NULL, "this->action_btn is NULL");
    	elm_table_pack(table, this->action_btn, 1, 0, 1, 1);
	}
    else if(this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT)
	{
    	this->action_btn = ui_utils_push_button_add(this, table, "Connect Audio", _control_button_pressed_cb);
    	RETVM_IF(NULL == this->action_btn, NULL, "this->action_btn is NULL");
    	elm_table_pack(table, this->action_btn, 1, 0, 1, 1);
	}
	else if(this->view->tbt_info->apptype == TBT_APP_BT_HDP)
	{
		this->action_btn = ui_utils_push_button_add(this, table, "Connect HDP", _control_button_pressed_cb);
    	RETVM_IF(NULL == this->action_btn, NULL, "this->action_btn is NULL");
		elm_table_pack(table, this->action_btn, 1, 0, 1, 1);
	}
    else if(this->view->tbt_info->apptype == TBT_APP_BT_HID_CONNECT)
	{
    	this->action_btn = ui_utils_push_button_add(this, table, "Connect HID", _control_button_pressed_cb);
    	RETVM_IF(NULL == this->action_btn, NULL, "this->action_btn is NULL");
    	elm_table_pack(table, this->action_btn, 1, 0, 1, 1);
	}

    else if(this->view->tbt_info->apptype == TBT_APP_BT_SDP)
   	{
       	this->action_btn = ui_utils_push_button_add(this, table, "Connected Services", _control_button_pressed_cb);
       	RETVM_IF(NULL == this->action_btn, NULL, "this->action_btn is NULL");
       	elm_table_pack(table, this->action_btn, 1, 0, 1, 1);
   	}

    else if(this->view->tbt_info->apptype == TBT_APP_BT_AVRCP)
	{
		this->action_btn = ui_utils_push_button_add(this, table, "Connect AVRCP", _control_button_pressed_cb);
		RETVM_IF(NULL == this->action_btn, NULL, "this->action_btn is NULL");
		elm_table_pack(table, this->action_btn, 1, 0, 1, 1);
	}
    else if(this->view->tbt_info->apptype == TBT_APP_BT_AG)
	{
		this->action_btn = ui_utils_push_button_add(this, table, "Connect AG", _control_button_pressed_cb);
		RETVM_IF(NULL == this->action_btn, NULL, "this->action_btn is NULL");
		elm_table_pack(table, this->action_btn, 1, 0, 1, 1);
	}
    else if(this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT_HANDSFREE)
   	{
       	this->action_btn = ui_utils_push_button_add(this, table, "Handsfree", _control_button_pressed_cb);
       	RETVM_IF(NULL == this->action_btn, NULL, "this->action_btn is NULL");
       	elm_table_pack(table, this->action_btn, 1, 0, 1, 1);
   	}
    else if(this->view->tbt_info->apptype == TBT_APP_BT_AUTHORIZATION)
   	{
       	this->action_btn = ui_utils_push_button_add(this, table, "Authorize", _control_button_pressed_cb);
       	RETVM_IF(NULL == this->action_btn, NULL, "this->action_btn is NULL");
       	elm_table_pack(table, this->action_btn, 1, 0, 1, 1);
   	}
    else if(this->view->tbt_info->apptype == TBT_APP_BT_CLIENT_SOCKET)
	{
		this->action_btn = ui_utils_push_button_add(this, table, "Send Data", _control_button_pressed_cb);
		RETVM_IF(NULL == this->action_btn, NULL, "this->action_btn is NULL");
		elm_table_pack(table, this->action_btn, 1, 0, 1, 1);
	}
    else if(this->view->tbt_info->apptype == TBT_APP_BT_OPP_SERVER)
    {
		this->action_btn = ui_utils_push_button_add(this, table, "Cancel", _control_button_pressed_cb);
		RETVM_IF(NULL == this->action_btn, NULL, "this->action_btn is NULL");
		elm_table_pack(table, this->action_btn, 1, 0, 1, 1);
		elm_object_disabled_set(this->action_btn, EINA_TRUE);
		this->is_opp_server_accecpt = EINA_FALSE;
		this->is_opp_server_push_progress = EINA_FALSE;
    }

    return table;
}


/**
 * @function		adapter_on_off_request_send
 * @since_tizen		2.3
 * @description		Adapter On Off Request Send
 * @parameter		bluetooth_view*: Bluetooth View Pointer
 * @return		static int
 */
static int adapter_on_off_request_send(bluetooth_view *this)
{
   DBG(" adapter_on_off_request_send ");
   RETVM_IF(NULL == this, -1, "view is NULL");
   int result = 0;
   app_control_h service = NULL;

   result = app_control_create(&service);
   RETVM_IF(result != APP_CONTROL_ERROR_NONE, 0, "app_control_create fail > Error = %d", result);
   if(service == NULL)
   {
      DBG("service_create failed!\n");
      return -1;
   }

   result = app_control_set_operation(service, "http://tizen.org/appcontrol/operation/configure/bluetooth/onoff");
   DBG("app_control_set_operation is %d", result);
   RETVM_IF(result != APP_CONTROL_ERROR_NONE, 0, "app_control_set_operation fail > Error = %d", result);

   result = app_control_send_launch_request(service, NULL, NULL);
   if(result != APP_CONTROL_ERROR_NONE)
	{
	   do_bt_common_feed(this, BT_APP_CONTROL_ACTIVATION_FAILURE, NULL);
	   return 0 ;
	}

   is_bluetooth_app_exist = true;
   result = app_control_destroy(service);
   RETVM_IF(result != APP_CONTROL_ERROR_NONE, 0, "app_control_destroy fail > Error = %d", result);

   return 0;
}


/**
 * @function		send_rfcomm_send_client_data
 * @since_tizen		2.3
 * @description		Send Rfcomm Send Client Data
 * @parameter		bluetooth_view*: Bluetooth View Pointer
 * @return		static int
 */
static int send_rfcomm_send_client_data(bluetooth_view *this)
{
	DBG(" send_rfcomm_send_client_data ");
	RETVM_IF(NULL == this, -1, "bluetooth_view is NULL");
	int result;

	result = bt_adapter_get_state(&this->adapter_state);
	RETVM_IF(result != BT_ERROR_NONE, result,"bt_adapter_get_state fail > Error = %d", result);

	if(this->view->tbt_info->apptype == TBT_APP_BT_CLIENT_SOCKET && this->adapter_state == BT_ADAPTER_ENABLED)
	{
		{
			DBG("Creating rfcomm connection with: %s, UUID: %s", this->selected_device_info->remote_address, BLUETOOTH_SOCKET_UUID);
			result = bt_socket_connect_rfcomm (this->selected_device_info->remote_address,BLUETOOTH_SOCKET_UUID);
			RETVM_IF(result != BT_ERROR_NONE, result,"bt_socket_connect_rfcomm fail > Error = %d", result);
			this->rfcomm_client_connected = true;
		}
	}

	return result;
}


/**
 * @function		create_rfcomm_server
 * @since_tizen		2.3
 * @description		Create Rfcomm Server
 * @parameter		bluetooth_view*: Bluetooth View Pointer
 * @return		static int
 */
static int create_rfcomm_server(bluetooth_view *this)
{
	DBG("create_rfcomm_server__");
	RETVM_IF(NULL == this, -1, "bluetooth_view is NULL");
	int result;
	result = bt_adapter_get_state(&this->adapter_state);
	RETVM_IF(result != BT_ERROR_NONE, result,"bt_adapter_get_state fail > Error = %d", result);

	if(this->view->tbt_info->apptype == TBT_APP_BT_SERVER_SOCKET && this->adapter_state == BT_ADAPTER_ENABLED)
	{
		if(this->rfcomm_created == true)
		{
			DBG("Destroying rfcomm server");
			result = bt_socket_destroy_rfcomm(this->server_socket_fd);
			RETVM_IF(result != BT_ERROR_NONE, result,"[bt_socket_destroy_rfcomm] Failed. > Error = %d", result);
			this->rfcomm_created = true;
		}

		char* local_address = NULL;
		bt_adapter_get_address(&local_address);
		DBG("Creating rfcomm server on device %s ",local_address);
		result = bt_socket_create_rfcomm(BLUETOOTH_SOCKET_UUID, &this->server_socket_fd);
		DBG("bt_socket_create_rfcomm return value is %d", result);
		RETVM_IF(result != BT_ERROR_NONE, result,"[bt_socket_create_rfcomm] Failed. > Error = %d", result);
		DBG("Created rfcomm server server_socket_fd: %d", this->server_socket_fd);

		result = bt_socket_listen_and_accept_rfcomm (this->server_socket_fd, 32);
		DBG("bt_socket_listen_and_accept_rfcomm return value is %d", result);
		RETVM_IF(result != BT_ERROR_NONE, result,"[bt_socket_listen_and_accept_rfcomm] Failed. > Error = %d", result);

		SAFE_DELETE(local_address);
	}

	return result;

}


/**
 * @function		is_new_device_found
 * @since_tizen		2.3
 * @description		Is New Device Found
 * @parameter		bluetooth_view*: Bluetooth View Pointer, bt_adapter_device_discovery_info_s*: Bt Adapter Device Discovery Info S Pointer
 * @return		static bool
 */
static bool is_new_device_found(bluetooth_view *this, bt_adapter_device_discovery_info_s *discovery_info)
{
	DBG(" is_new_device_found ");
	RETVM_IF(NULL == this, false, "view is NULL");
	RETVM_IF(NULL == discovery_info, false, "discovery_info is NULL");
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
 * @function		discovered_devices_list_show
 * @since_tizen		2.3
 * @description		Discovered Devices List Show
 * @parameter		bluetooth_view*: Bluetooth View Pointer
 * @return		static void
 */
static void discovered_devices_list_show(bluetooth_view *this)
{
	DBG(" discovered_devices_list_show ");
	RETM_IF(NULL == this, "view is NULL");

	elm_list_clear(this->bluetooth_list);

	int ret;
	GList *l;
	bt_adapter_device_discovery_info_s *device_info;

	for(l = this->devices_list; l != NULL; l = l->next)
	{
		device_info = (bt_adapter_device_discovery_info_s*)l->data;

		if(NULL != device_info)
		{
			if(device_info->is_bonded == false)
			{
				elm_list_item_append(this->bluetooth_list, device_info->remote_name, NULL, NULL, _device_item_selected_cb, device_info);
			}
			else
			{
				elm_list_item_append(this->bluetooth_list, format_string("%s(Paired)", device_info->remote_name), NULL, NULL, _device_item_selected_cb, device_info);
				elm_object_disabled_set(this->action_btn, EINA_FALSE);

			}
		}
	}

	elm_list_go(this->bluetooth_list);
	evas_object_show(this->bluetooth_list);
}


/**
 * @function		set_connected_profiles
 * @since_tizen		2.3
 * @description		Set Connected Profiles
 * @parameter		bluetooth_view*: Bluetooth View Pointer
 * @return		static void
 */
static void set_connected_profiles(bluetooth_view *this)
{
	DBG(" set_connected_profiles ");
	RETM_IF(NULL == this, "view is NULL");

	int result = 0;

	DBG("set_connected_profiles called");
	this->no_of_profiles = 0;

	if(this->view->tbt_info->apptype == TBT_APP_BT_PROFILES && this->selected_device_info != NULL && this->selected_device_info->is_bonded == true)
	{
		result = bt_audio_initialize();
		adapter_device_error(result);
		RETM_IF(result != BT_ERROR_NONE, "bt_audio_initialize fail > Error = %d", result);

		result = bt_audio_set_connection_state_changed_cb(_adapter_bt_audio_connection_state_changed_cb, this);

		RETM_IF(result != BT_ERROR_NONE, "bt_audio_set_connection_state_changed_cb fail > Error = %d", result);

		if(this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT && this->selected_device_info != NULL)
		{
			result = bt_audio_connect(this->selected_device_info->remote_address, BT_AUDIO_PROFILE_TYPE_A2DP);
			RETM_IF(result != BT_ERROR_NONE, "bt_audio_connect fail > Error = %d", result);

			elm_object_disabled_set(this->action_btn, EINA_TRUE);
		}

		if(this->selected_device_info != NULL)
		{
			result = bt_audio_connect(this->selected_device_info->remote_address, BT_AUDIO_PROFILE_TYPE_A2DP);
			RETM_IF(result != BT_ERROR_NONE, "bt_audio_connect fail > Error = %d", result);

			elm_object_disabled_set(this->action_btn, EINA_TRUE);
		}

		if(this->selected_device_profile_list != NULL)
		{
			g_list_free_full(this->selected_device_profile_list, bluetooth_list_free_func_cb);
			this->selected_device_profile_list = NULL;
		}
	}
}


/**
 * @function		update_bonded_devices
 * @since_tizen		2.3
 * @description		Update Bonded Devices
 * @parameter		bluetooth_view*: Bluetooth View Pointer
 * @return		static int
 */
static int update_bonded_devices(bluetooth_view *this)
{
	DBG(" update_bonded_devices ");
	RETVM_IF(NULL == this, -1, "view is NULL");

	int result = 0;
	if(this->adapter_state == BT_ADAPTER_ENABLED)
	{
		result = bt_adapter_foreach_bonded_device(_adapter_bonded_device_cb, this);
		RETVM_IF(result != BT_ERROR_NONE, result,"bt_adapter_foreach_bonded_device fail > Error = %d", result);

		if(this->bonded_devices_list)
		{
			g_list_free_full(this->bonded_devices_list, bluetooth_list_free_func_cb);
			this->bonded_devices_list = NULL;
		}

	}
	return result;
}

static void _popup_destroy_cb(void *data, Evas_Object *obj, void *event_info)
{
    RETM_IF(NULL == data, "data is null");
	DBG("popup destory cb");


	bluetooth_view *this = data;
	if(this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT)
		bt_audio_disconnect(this->selected_device_info->remote_address, BT_AUDIO_PROFILE_TYPE_ALL);
	else if(this->view->tbt_info->apptype == TBT_APP_BT_HID_CONNECT)
		bt_hid_host_disconnect(this->selected_device_info->remote_address);
	else if(this->view->tbt_info->apptype == TBT_APP_BT_HDP)
		bt_hdp_disconnect(this->selected_device_info->remote_address, this->hdp_channel);


	elm_object_text_set(this->bluetooth_label, "Disconnected");

//    Evas_Object *obj_to_del = data;
//    evas_object_del(obj_to_del);
	evas_object_del(this->popup);
	this->popup_state = 0;

	elm_object_disabled_set(this->action_btn, EINA_FALSE);


    if(event_info)
    {
        Elm_Object_Item *item = event_info;
        elm_genlist_item_selected_set(item, EINA_FALSE);
    }

}



/**
 * @function		_bt_hdp_connected_cb
 * @since_tizen		2.3
 * @description		 Bt Hdp Connected Cb
 * @parameter		int: Int, const char*: Const char Pointer, const char*: Const char Pointer, bt_hdp_channel_type_e: Bt Hdp Channel Type E, void*: Void Pointer
 * @return		static void
 */
static void _bt_hdp_connected_cb(int result, const char *remote_address, const char *app_id,bt_hdp_channel_type_e type, unsigned int channel, void *user_data)
{
	DBG("Inside _bt_hdp_connected_cb...");
	bluetooth_view *this = NULL;
	this = (bluetooth_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");
	RETM_IF(NULL == remote_address, "remote_address is NULL");
	RETM_IF(NULL == app_id, "app_id is NULL");
	DBG("result: %d", result);
	DBG("remote_address: %s", remote_address);
	DBG("app_id: %s", app_id);
	DBG("channel type: %d", type);
	DBG("channel: %d", channel);
	this->hdp_channel = channel;

	if(this->hdp_send_data_flag == false && this->hdp_channel >0)
	{
		ui_utils_label_set_text(this->bluetooth_label, "HDP Connected", "left");
#if 0 /* skip the verification of the Data Sending */
		ui_utils_label_set_text(this->action_btn, "Get Data", "left");
		this->hdp_send_data_flag = true;
#endif

		if (!this->popup_state)
		{
			/* LHtest */
			this->popup = ui_utils_popup_add(this->view->navi, "HDP Device connected!!");
			this->popup_state = 1;
			char msg[512];
			snprintf(msg, sizeof(msg),"<br/>For Disconnecting,<br/>Press OK Button");
			elm_object_text_set(this->popup, msg);
			ui_utils_popup_button_add(this->popup, "OK", _popup_destroy_cb, this);
		}

	}
	else
	{
		ui_utils_label_set_text(this->bluetooth_label, "Try to Connect Again", "left");
	}
}


/**
 * @function		_bt_hdp_disconnected_cb
 * @since_tizen		2.3
 * @description		 Bt Hdp Disconnected Cb
 * @parameter		int: Int, const char*: Const char Pointer, void*: Void Pointer
 * @return		static void
 */
static void _bt_hdp_disconnected_cb(int result, const char *remote_address, unsigned int channel, void *user_data)
{
	DBG("Inside _bt_hdp_disconnected_cb...");

	bluetooth_view *this = NULL;
	this = (bluetooth_view*) user_data;
	RETM_IF(NULL == this, "view is NULL");
	RETM_IF(NULL == remote_address, "remote_address is NULL");

	DBG("result: %d", result);
	DBG("remote_address: %s", remote_address);
	DBG("channel: %d", channel);

	ui_utils_label_set_text(this->bluetooth_label, "HDP Disconnected", "left");
	this->hdp_channel = 0;
	this->hdp_send_data_flag = false;
}


/**
 * @function		_bt_hdp_data_received_cb
 * @since_tizen		2.3
 * @description		 Bt Hdp Data Received Cb
 * @parameter		const char*: Const char Pointer, void*: Void Pointer
 * @return		static void
 */
static void _bt_hdp_data_received_cb(unsigned int channel, const char *data, unsigned int size, void *user_data)
{
	DBG("Inside _bt_hdp_data_received_cb...");

	bluetooth_view *this = NULL;
	this = (bluetooth_view*) user_data;
	RETM_IF(NULL == this, "view is NULL");
	RETM_IF(NULL == data, "data is NULL");

	DBG("channel: %d", channel);
	DBG("data: %s", data);
	DBG("size: %d", size);

//	char* str;
//	str = format_string("Data Recieved: %s", data);
	ui_utils_label_set_text(this->bluetooth_label, "Data Received", "left");
//	SAFE_DELETE(str);
}


/**
 * @function		adapter_device_error
 * @since_tizen		2.3
 * @description		Adapter Device Error
 * @parameter		int: Int
 * @return		static void
 */
static void adapter_device_error(int result)
{
	if(result != BT_ERROR_NONE)
	{
		if(result == BT_ERROR_CANCELLED)
		{
			DBG("BT_ERROR_CANCELLED");
		}
		else if(result == BT_ERROR_INVALID_PARAMETER)
		{
			DBG("BT_ERROR_INVALID_PARAMETER");
		}
		else if(result == BT_ERROR_OUT_OF_MEMORY)
		{
			DBG("BT_ERROR_OUT_OF_MEMORY");
		}
		else if(result == BT_ERROR_RESOURCE_BUSY)
		{
			DBG("BT_ERROR_RESOURCE_BUSY");
		}
		else if(result == BT_ERROR_TIMED_OUT)
		{
			DBG("BT_ERROR_TIMED_OUT");
		}
		else if(result == BT_ERROR_NOW_IN_PROGRESS)
		{
			DBG("BT_ERROR_NOW_IN_PROGRESS");
		}
		else if(result == BT_ERROR_PERMISSION_DENIED)
		{
			DBG("BT_ERROR_PERMISSION_DENIED");
		}
		else if(result == BT_ERROR_QUOTA_EXCEEDED)
		{
			DBG("BT_ERROR_QUOTA_EXCEEDED");
		}
		else if(result == BT_ERROR_NOT_INITIALIZED)
		{
			DBG("BT_ERROR_NOT_INITIALIZED");
		}
		else if(result == BT_ERROR_NOT_ENABLED)
		{
			DBG("BT_ERROR_NOT_ENABLED");
		}
		else if(result == BT_ERROR_ALREADY_DONE)
		{
			DBG("BT_ERROR_ALREADY_DONE");
		}
		else if(result == BT_ERROR_OPERATION_FAILED)
		{
			DBG("BT_ERROR_OPERATION_FAILED");
		}
		else if(result == BT_ERROR_NOT_IN_PROGRESS)
		{
			DBG("BT_ERROR_NOT_IN_PROGRESS");
		}
		else if(result == BT_ERROR_REMOTE_DEVICE_NOT_BONDED)
		{
			DBG("BT_ERROR_REMOTE_DEVICE_NOT_BONDED");
		}
		else if(result == BT_ERROR_AUTH_REJECTED)
		{
			DBG("BT_ERROR_AUTH_REJECTED");
		}
		else if(result == BT_ERROR_AUTH_FAILED)
		{
			DBG("BT_ERROR_AUTH_FAILED");
		}
		else if(result == BT_ERROR_REMOTE_DEVICE_NOT_FOUND)
		{
			DBG("BT_ERROR_REMOTE_DEVICE_NOT_FOUND");
		}
		else if(result == BT_ERROR_SERVICE_SEARCH_FAILED)
		{
			DBG("BT_ERROR_SERVICE_SEARCH_FAILED");
		}
		else if(result == BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED)
		{
			DBG("BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED");
		}
		else if(result == BT_ERROR_AGAIN)
		{
			DBG("BT_ERROR_AGAIN");
		}
		else if(result == BT_ERROR_SERVICE_NOT_FOUND)
		{
			DBG("BT_ERROR_SERVICE_NOT_FOUND");
		}
		else
		{
			DBG("[bt_device_bond_created_cb] Failed. result(%d).", result);
		}

	}
}


/**
 * @function		_control_button_pressed_cb
 * @since_tizen		2.3
 * @description		 Control Button Pressed Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _control_button_pressed_cb(void *data, Evas_Object *obj, void *event_info)
{
	DBG(" _control_button_pressed_cb ");

	RETM_IF(NULL == data, "data is NULL");

	int result;
	bluetooth_view *this = NULL;
	this = (bluetooth_view*)data;
	RETM_IF(NULL == this, "view is NULL");
	this->popup_state = 0;


	if(this->view->tbt_info->apptype == TBT_APP_BT_OPP_CLIENT)
	{
		result = bt_opp_client_initialize();
		RETM_IF(result != BT_ERROR_NONE, "bt_opp_client_initialize fail > Error = %d", result);

		result = bt_opp_client_add_file(get_shared_resource_path(this->view->tbt_info->file_name));
		RETM_IF(result != BT_ERROR_NONE, "bt_opp_client_add_file fail > Error = %d", result);

		if(this->selected_device_info != NULL)
		{
			result = bt_opp_client_push_files(this->selected_device_info->remote_address, _adapter_opp_client_push_responded_cb, _adapter_opp_client_push_progress_cb, _adapter_opp_client_push_finished_cb, this);
			RETM_IF(result != BT_ERROR_NONE, "bt_opp_client_push_files fail > Error = %d", result);

			elm_object_disabled_set(this->action_btn, EINA_TRUE);
		}
	}
	else if(this->view->tbt_info->apptype == TBT_APP_BT_AUTHORIZATION &&
			this->selected_device_info != NULL)
	{
		result = bt_device_set_authorization(this->selected_device_info->remote_address, BT_DEVICE_AUTHORIZED);
		RETM_IF(result != BT_ERROR_NONE, "bt_device_set_authorization fail > Error = %d", result);
	}
	else if(this->view->tbt_info->apptype == TBT_APP_BT_CLIENT_SOCKET &&
			this->selected_device_info != NULL)
	{
		DBG("inside TBT_APP_BT_CLIENT_SOCKET");
		result = send_rfcomm_send_client_data(this);
		RETM_IF(result != BT_ERROR_NONE,"send_rfcomm_send_client_data fail > Error = %d", result);
	}
	else if(this->view->tbt_info->apptype == TBT_APP_BT_OPP_SERVER &&
			this->is_opp_server_accecpt == EINA_TRUE)
	{
		result = bt_opp_server_reject();
		DBG("bt_opp_server_reject :%d",result);
		ui_utils_label_set_text(this->bluetooth_label, "Connection rejected.", "left");
		RETM_IF(result != BT_ERROR_NONE, "bt_opp_server_reject fail > Error = %d", result);
	}
	else if((this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT ||
			this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT_HANDSFREE ||
			this->view->tbt_info->apptype == TBT_APP_BT_AG) &&
			this->selected_device_info != NULL)
	{

		result = bt_audio_initialize();
		DBG("BT_AUDIO INITIALIZED");
		RETM_IF(result != BT_ERROR_NONE, "bt_audio_initialize fail > Error = %d", result);

		result = bt_audio_set_connection_state_changed_cb(_adapter_bt_audio_connection_state_changed_cb, this);
		RETM_IF(result != BT_ERROR_NONE, "bt_audio_set_connection_state_changed_cb fail > Error = %d", result);

		if(this->selected_device_info != NULL)
		{
			if(this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT )
			{
//				result = bt_audio_connect(this->selected_device_info->remote_address, BT_AUDIO_PROFILE_TYPE_A2DP);
				result = bt_audio_connect(this->selected_device_info->remote_address, BT_AUDIO_PROFILE_TYPE_ALL);
				RETM_IF(result != BT_ERROR_NONE, "bt_audio_connect fail > Error = %d", result);
			}

			else
			{
				result = bt_audio_connect(this->selected_device_info->remote_address, BT_AUDIO_PROFILE_TYPE_HSP_HFP);
				RETM_IF(result != BT_ERROR_NONE, "bt_audio_connect fail > Error = %d", result);
			}

				elm_object_disabled_set(this->action_btn, EINA_TRUE);


		}
	}

	else if(this->view->tbt_info->apptype == TBT_APP_BT_HDP)
	{

		if(this->hdp_send_data_flag == false && this->selected_device_info != NULL)
		{
			//DO HDP Stuff
			DBG("HDP connect to source");
			result = bt_hdp_connect_to_source(this->selected_device_info->remote_address, this->hdp_app_id);
			RETM_IF(result != BT_ERROR_NONE, "bt_hdp_connect_to_source error: %s", get_bluetooth_error(result));
		}
#if 0 /* skip the verification of the Data Sending */
		else
		{
			char data[] = {'a', 'a', 'a'}; //arbitrary value
			DBG("channel: %d", this->hdp_channel);
			result = bt_hdp_send_data(this->hdp_channel, data, sizeof(data));
			RETM_IF(result != BT_ERROR_NONE, "bt_hdp_send_data error: %s", get_bluetooth_error(result));
			this->hdp_send_data_flag = false;
			elm_object_disabled_set(this->action_btn, EINA_TRUE);
		}
#endif
	}

	else if(this->view->tbt_info->apptype == TBT_APP_BT_HID_CONNECT)
	{
		//HID Connect code goes here
		result = bt_hid_host_initialize(_bt_hid_host_connection_state_changed_cb, this);
		this->popup_state = 0;
		RETM_IF(result != BT_ERROR_NONE, "bt_hid_host_initialize fail > Error = %s", get_bluetooth_error(result));
		if(this->selected_device_info != NULL)
		{
			result = bt_hid_host_connect(this->selected_device_info->remote_address);
			RETM_IF(result != BT_ERROR_NONE, "bt_hid_host_connect fail > Error = %s", get_bluetooth_error(result));
		}
	}

	else if(this->view->tbt_info->apptype == TBT_APP_BT_SDP)
	{
	 //SDP code goes here

		GList *l;
		elm_list_clear(this->bluetooth_list);
		elm_object_text_set(this->bluetooth_label, "Connected Service Classes:");
		for(l = this->service_list; l != NULL; l = l->next)
		{
			elm_list_item_append(this->bluetooth_list, l->data, NULL, NULL, NULL, NULL);
		}
	}

	else if(this->view->tbt_info->apptype == TBT_APP_BT_AVRCP)
	{
		//AVRCP connect goes here
		DBG("bt_avrcp_target_initialize going to be called..");
		result = bt_avrcp_target_initialize(_bt_avrcp_target_connection_state_changed_cb, this);
		RETM_IF(result != BT_ERROR_NONE, "bt_avrcp_target_initialize fail > Error = %s", get_bluetooth_error(result));
		result = bt_avrcp_set_equalizer_state_changed_cb(_bt_avrcp_equalizer_state_changed_cb, this);
		RETM_IF(result != BT_ERROR_NONE, "bt_avrcp_set_equalizer_state_changed_cb fail > Error = %s", get_bluetooth_error(result));
		result = bt_avrcp_set_repeat_mode_changed_cb(_bt_avrcp_repeat_mode_changed_cb, this);
		RETM_IF(result != BT_ERROR_NONE, "bt_avrcp_set_repeat_mode_changed_cb fail > Error = %s", get_bluetooth_error(result));
		result = bt_avrcp_set_shuffle_mode_changed_cb(_bt_avrcp_shuffle_mode_changed_cb, this);
		RETM_IF(result != BT_ERROR_NONE, "bt_avrcp_set_shuffle_mode_changed_cb fail > Error = %s", get_bluetooth_error(result));
		result = bt_avrcp_set_scan_mode_changed_cb(_bt_avrcp_scan_mode_changed_cb, this);
		RETM_IF(result != BT_ERROR_NONE, "bt_avrcp_set_scan_mode_changed_cb fail > Error = %s", get_bluetooth_error(result));
	}

	else if(this->view->tbt_info->apptype == TBT_APP_BT_AG)
	{
	#ifdef TBT_MODULE_APP_BT_AG
			result = bt_audio_connect(this->selected_device_info->remote_address, BT_AUDIO_PROFILE_TYPE_AG);
			RETM_IF(result != BT_ERROR_NONE, "bt_audio_connect fail > Error = %s", get_bluetooth_error(result));

			result = bt_ag_notify_voice_recognition_state(false);
			RETM_IF(result != BT_ERROR_NONE, "bt_ag_notify_voice_recognition_state fail > Error = %d", result);

			bool isOpened;
			result = bt_ag_is_sco_opened(&isOpened);
			RETM_IF(result != BT_ERROR_NONE, "bt_ag_is_sco_opened fail > Error = %s", get_bluetooth_error(result));

			result = bt_ag_set_sco_state_changed_cb(_bt_ag_sco_state_changed_cb, this);
			RETM_IF(result != BT_ERROR_NONE, "_bt_ag_sco_state_changed_cb fail > Error = %s", get_bluetooth_error(result));
			if(!isOpened)
			{
				result = bt_ag_open_sco();
				RETM_IF(result != BT_ERROR_NONE, "bt_ag_is_sco_opened fail > Error = %s", get_bluetooth_error(result));
			}
	#endif
	}

}


/**
 * @function		_cancel_button_pressed_cb
 * @since_tizen		2.3
 * @description		 Cancel Button Pressed Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _cancel_button_pressed_cb(void *data, Evas_Object *obj, void *event_info)
{
	DBG(" _cancel_button_pressed_cb ");

	RETM_IF(NULL == data, "data is NULL");

	int result;
	bluetooth_view *this = NULL;
	this = (bluetooth_view*)data;
	RETM_IF(NULL == this, "view is NULL");
	result = bt_opp_client_cancel_push();
	RETM_IF(result != BT_ERROR_NONE, "bt_opp_client_cancel_push fail > Error = %d", result);
	elm_object_disabled_set(this->bluetooth_check_button, EINA_TRUE);
	this->is_opp_push_cancelled = true;
}


/**
 * @function		_control_button_bluetooth_pressed_cb
 * @since_tizen		2.3
 * @description		 Control Button Bluetooth Pressed Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _control_button_bluetooth_pressed_cb(void *data, Evas_Object *obj, void *event_info)
{
	DBG(" _control_button_bluetooth_pressed_cb ");

	RETM_IF(NULL == data, "data is NULL");

	int result;
	bluetooth_view *this = NULL;
	this = (bluetooth_view*)data;
	RETM_IF(NULL == this, "view is NULL");

	//Check whether the Bluetooth Service is enabled
	result = bt_adapter_get_state(&this->adapter_state);
	RETM_IF(result != BT_ERROR_NONE, "bt_adapter_get_state fail > Error = %d", result);

	//If the Bluetooth Service is not enabled
	if(this->adapter_state == BT_ADAPTER_DISABLED)
	{
		do_bt_common_feed(this, BT_ON_ATTEMPT, NULL);
	}
	else if(this->adapter_state == BT_ADAPTER_ENABLED)
	{
		result = update_bonded_devices(this);
		RETM_IF(result != BT_ERROR_NONE, "update_bonded_devices fail > Error = %d", result);
		update_view_controls(this);

		if(this->state_object_bt_client_pair)
	 {
		 do_bt_common_feed(this, BT_START_DISCOVERY_REQUEST, NULL);
	 }

	 else if(this->view->tbt_info->apptype == TBT_APP_BT_SERVER_PAIRED ||
				this->view->tbt_info->apptype == TBT_APP_BT_SERVER_SOCKET ||
				this->view->tbt_info->apptype == TBT_APP_BT_OPP_SERVER)
		{
			int duration;
			bt_adapter_visibility_mode_e mode;

			result = bt_adapter_get_visibility(&mode, &duration);
			RETM_IF(result != BT_ERROR_NONE, "[bt_adapter_get_visibility] Failed. > Error = %d", result);

			if(mode == BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE)
			{
//				result = bt_adapter_set_visibility(BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE, 60);
//				adapter_device_error(result);
//				RETM_IF(result != BT_ERROR_NONE, "[bt_adapter_set_visibility] Failed. > Error = %d", result);
				DBG("BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE");

			}
			if(this->view->tbt_info->apptype == TBT_APP_BT_SERVER_SOCKET)
			{
				result = bt_socket_create_rfcomm(BLUETOOTH_SOCKET_UUID, &this->server_socket_fd);
				RETM_IF(result != BT_ERROR_NONE, "[bt_socket_create_rfcomm] Failed. > Error = %d", result);

				result = bt_socket_listen_and_accept_rfcomm (this->server_socket_fd, 3);
				RETM_IF(result != BT_ERROR_NONE, "[bt_socket_listen_and_accept_rfcomm] Failed. > Error = %d", result);
			}
			if(this->view->tbt_info->apptype == TBT_APP_BT_OPP_SERVER)
			{
				if( this->is_opp_server_accecpt == true && this->is_opp_server_push_progress == EINA_FALSE)
				{
					result = bt_opp_server_accept(_adpter_bt_opp_server_transfer_progress_cb_for_opp, _adapter_bt_opp_server_transfer_finished_cb_for_opp, NULL, this, &(this->opp_transfer_id) );
					RETM_IF(result != BT_ERROR_NONE, "[bt_opp_server_accept] Failed. > Error = %d", result);
				}
				else if(this->is_opp_server_push_progress == EINA_TRUE)
				{
					result = bt_opp_server_cancel_transfer(this->opp_transfer_id);
					RETM_IF(result != BT_ERROR_NONE, "[bt_opp_server_cancel_transfer] Failed. > Error = %d", result);
				}
			}

		}
	}
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

	bluetooth_view *this = NULL;
	this = (bluetooth_view*)data;
	RETM_IF(NULL == this, "view is NULL");

	if(result != BT_ERROR_NONE)
	{
		DBG("[adapter_state_changed_cb] Failed! result=%d", result);
		return;
	}

	if(this->state_object_bt_client_pair)
	{
		do_bt_common_feed(this, BT_ADAPTER_ON_CLIENT, (void*)&adapter_state);
	}
	else
	{
		do_bt_common_feed(this, BT_ADAPTER_ON, (void*)&adapter_state);
	}

}


/**
 * @function		_adapter_device_discovery_state_changed_cb
 * @since_tizen		2.3
 * @description		 Adapter Device Discovery State Changed Cb
 * @parameter		int: Int, bt_adapter_device_discovery_info_s*: Bt Adapter Device Discovery Info S Pointer, void*: Void Pointer
 * @return		static void
 */
static void _adapter_device_discovery_state_changed_cb(int result, bt_adapter_device_discovery_state_e discovery_state, bt_adapter_device_discovery_info_s *discovery_info, void* data)
{
	DBG(" _adapter_device_discovery_state_changed_cb ");

	RETM_IF(NULL == data, "data is NULL");

	bluetooth_view *this = NULL;
	this = (bluetooth_view*)data;
	RETM_IF(NULL == this, "view is NULL");

	adapter_device_error(result);

   this->discovery_state = discovery_state;

   if(this->discovery_state == BT_ADAPTER_DEVICE_DISCOVERY_STARTED)
   {
	   g_list_free_full(this->devices_list, bluetooth_list_free_func_cb);
	   this->devices_list = NULL;
   }
   else if(this->discovery_state == BT_ADAPTER_DEVICE_DISCOVERY_FOUND)
   {
	   if(discovery_info)
	   {
			if(is_new_device_found(this, discovery_info) == true)
			{
				if(this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT || this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT_HANDSFREE || this->view->tbt_info->apptype == TBT_APP_BT_PROFILES)
				{
					if(discovery_info->bt_class.major_device_class == BT_MAJOR_DEVICE_CLASS_AUDIO_VIDEO &&
						discovery_info->bt_class.minor_device_class == BT_MINOR_DEVICE_CLASS_AUDIO_VIDEO_WEARABLE_HEADSET)
					{
						bt_adapter_device_discovery_info_s * device_info;
						device_info = malloc(sizeof(bt_adapter_device_discovery_info_s));

						if(device_info != NULL)
						{
							memcpy(device_info, discovery_info, sizeof(bt_adapter_device_discovery_info_s));
							device_info->remote_address = strdup(discovery_info->remote_address);
							device_info->remote_name = strdup(discovery_info->remote_name);

							//Hold the last deviceinfo pointer;
							this->devices_list = g_list_append(this->devices_list, (gpointer)device_info);
						}
					}
				}

				else if(this->view->tbt_info->apptype == TBT_APP_BT_AVRCP)
				{

					if(discovery_info->bt_class.major_device_class == BT_MAJOR_DEVICE_CLASS_AUDIO_VIDEO)
					{
						bt_adapter_device_discovery_info_s * device_info;
						device_info = malloc(sizeof(bt_adapter_device_discovery_info_s));

						if(device_info != NULL)
						{
							memcpy(device_info, discovery_info, sizeof(bt_adapter_device_discovery_info_s));
							device_info->remote_address = strdup(discovery_info->remote_address);
							device_info->remote_name = strdup(discovery_info->remote_name);

							//Hold the last deviceinfo pointer;
							this->devices_list = g_list_append(this->devices_list, (gpointer)device_info);
						}
					}


				}

				else if(this->view->tbt_info->apptype == TBT_APP_BT_AG)
				{


					if(discovery_info->bt_class.major_device_class == BT_MAJOR_DEVICE_CLASS_AUDIO_VIDEO)
					{
						bt_adapter_device_discovery_info_s * device_info;
						device_info = malloc(sizeof(bt_adapter_device_discovery_info_s));

						if(device_info != NULL)
						{
							memcpy(device_info, discovery_info, sizeof(bt_adapter_device_discovery_info_s));
							device_info->remote_address = strdup(discovery_info->remote_address);
							device_info->remote_name = strdup(discovery_info->remote_name);

							//Hold the last deviceinfo pointer;
							this->devices_list = g_list_append(this->devices_list, (gpointer)device_info);
						}
					}


				}

				else if(this->view->tbt_info->apptype == TBT_APP_BT_HID_CONNECT)
				{

					if(discovery_info->bt_class.major_device_class == BT_MAJOR_DEVICE_CLASS_PERIPHERAL &&(
						discovery_info->bt_class.minor_device_class == BT_MINOR_DEVICE_CLASS_PERIPHERAL_KEY_BOARD ||
						discovery_info->bt_class.minor_device_class == BT_MINOR_DEVICE_CLASS_PERIPHERAL_POINTING_DEVICE ||
						discovery_info->bt_class.minor_device_class == BT_MINOR_DEVICE_CLASS_PERIPHERAL_COMBO_KEYBOARD_POINTING_DEVICE ||
						discovery_info->bt_class.minor_device_class == BT_MINOR_DEVICE_CLASS_PERIPHERAL_JOYSTICK ||
						discovery_info->bt_class.minor_device_class == BT_MINOR_DEVICE_CLASS_PERIPHERAL_GAME_PAD ||
						discovery_info->bt_class.minor_device_class == BT_MINOR_DEVICE_CLASS_PERIPHERAL_REMOTE_CONTROL ||
						discovery_info->bt_class.minor_device_class == BT_MINOR_DEVICE_CLASS_PERIPHERAL_SENSING_DEVICE ||
						discovery_info->bt_class.minor_device_class == BT_MINOR_DEVICE_CLASS_PERIPHERAL_DIGITIZER_TABLET ||
						discovery_info->bt_class.minor_device_class == BT_MINOR_DEVICE_CLASS_PERIPHERAL_DIGITAL_PEN  ||
						discovery_info->bt_class.minor_device_class == BT_MINOR_DEVICE_CLASS_PERIPHERAL_HANDHELD_GESTURAL_INPUT_DEVICE))
					{
						bt_adapter_device_discovery_info_s * device_info;
						device_info = malloc(sizeof(bt_adapter_device_discovery_info_s));

						if(device_info != NULL)
						{
							memcpy(device_info, discovery_info, sizeof(bt_adapter_device_discovery_info_s));
							device_info->remote_address = strdup(discovery_info->remote_address);
							device_info->remote_name = strdup(discovery_info->remote_name);

							//Hold the last deviceinfo pointer;
							this->devices_list = g_list_append(this->devices_list, (gpointer)device_info);
						}
					}


				}

				else if(this->view->tbt_info->apptype == TBT_APP_BT_HDP)
				{
					//Filter Health Device only
					if(discovery_info->bt_class.major_device_class == BT_MAJOR_DEVICE_CLASS_HEALTH)
					{
						bt_adapter_device_discovery_info_s * device_info;
						device_info = malloc(sizeof(bt_adapter_device_discovery_info_s));

						if(device_info != NULL)
						{
							memcpy(device_info, discovery_info, sizeof(bt_adapter_device_discovery_info_s));
							device_info->remote_address = strdup(discovery_info->remote_address);
							device_info->remote_name = strdup(discovery_info->remote_name);

							//Hold the last deviceinfo pointer;
							this->devices_list = g_list_append(this->devices_list, (gpointer)device_info);
						}
					}

				}

				else
				{
					bt_adapter_device_discovery_info_s * device_info;
					device_info = malloc(sizeof(bt_adapter_device_discovery_info_s));

					if(device_info != NULL)
					{
						memcpy(device_info, discovery_info, sizeof(bt_adapter_device_discovery_info_s));
						device_info->remote_address = strdup(discovery_info->remote_address);
						device_info->remote_name = strdup(discovery_info->remote_name);

						//Hold the last deviceinfo pointer;
						this->devices_list = g_list_append(this->devices_list, (gpointer)device_info);
					}
				}
			}
	   }
   }

   update_view_controls(this);
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

    RETM_IF(!data, "data is NULL");
    
    RETM_IF(!obj, "obj is NULL");

    bluetooth_view *this;
    this = evas_object_data_get(obj, "bluetooth_view");
    RETM_IF(!this, "view is NULL");

    bt_adapter_device_discovery_info_s *device_info;
    device_info = (bt_adapter_device_discovery_info_s*)data;
	RETM_IF(NULL == device_info, "bt_adapter_device_discovery_info_s is NULL");

	Elm_Object_Item *item;
	item = (Elm_Object_Item*)event_info;
	elm_list_item_selected_set(item, EINA_TRUE);
	this->selected_device_item = item;
	do_bt_common_feed(this, BT_SELECT_DISCOVERED_DEVICE, (void*)device_info);
}


/**
 * @function		_adapter_visibility_mode_changed_cb
 * @since_tizen		2.3
 * @description		 Adapter Visibility Mode Changed Cb
 * @parameter		int: Int, void*: Void Pointer
 * @return		static void
 */
static void _adapter_visibility_mode_changed_cb(int result, bt_adapter_visibility_mode_e visibility_mode, void *data)
{
	DBG(" _adapter_visibility_mode_changed_cb ");

	RETM_IF(NULL == data, "data is NULL");
	bluetooth_view *this = NULL;
	this = (bluetooth_view*)data;
	RETM_IF(NULL == this, "view is NULL");
	RETM_IF(result != BT_ERROR_NONE, "result : %s", get_bluetooth_error(result));

	int ret;

	this->visibility_mode = visibility_mode;

	if(this->view->tbt_info->apptype == TBT_APP_BT_SERVER_PAIRED ||
		this->view->tbt_info->apptype == TBT_APP_BT_SERVER_SOCKET)
	{
		if(is_bluetooth_app_exist == true)
		{
			if(visibility_mode == BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE)
			{
				elm_object_disabled_set(this->bluetooth_btn, EINA_FALSE);
				elm_object_text_set(this->bluetooth_btn, "Set Visible");
				ui_utils_label_set_text(this->bluetooth_label, "Devices is not visible", "left");
			}
			else if(visibility_mode == BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE)
			{
				elm_object_disabled_set(this->bluetooth_btn, EINA_TRUE);
				elm_object_text_set(this->bluetooth_btn, "Set Visible");
				ui_utils_label_set_text(this->bluetooth_label, "Devices is visible", "left");
			}
			else if(visibility_mode == BT_ADAPTER_VISIBILITY_MODE_GENERAL_DISCOVERABLE)
			{
				elm_object_disabled_set(this->bluetooth_btn, EINA_TRUE);
				elm_object_text_set(this->bluetooth_btn, "Set Visible");
				ui_utils_label_set_text(this->bluetooth_label, "Devices is visible", "left");
			}
			ret = create_rfcomm_server(this);
			RETM_IF(ret != BT_ERROR_NONE, "create_rfcomm_server failed");
		}
		else
		{
			if(visibility_mode == BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE)
			{
				elm_object_disabled_set(this->bluetooth_btn, EINA_FALSE);
				elm_object_text_set(this->bluetooth_btn, "Set Visible");
				ui_utils_label_set_text(this->bluetooth_label, "Devices is not visible", "left");
			}
			else if(visibility_mode == BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE)
			{
				elm_object_disabled_set(this->bluetooth_btn, EINA_TRUE);
				elm_object_text_set(this->bluetooth_btn, "Set Visible");
				ui_utils_label_set_text(this->bluetooth_label, "Devices is visible", "left");
			}
			else if(visibility_mode == BT_ADAPTER_VISIBILITY_MODE_GENERAL_DISCOVERABLE)
			{
				elm_object_disabled_set(this->bluetooth_btn, EINA_TRUE);
				elm_object_text_set(this->bluetooth_btn, "Set Visible");
				ui_utils_label_set_text(this->bluetooth_label, "Devices is visible", "left");
			}
			ret = create_rfcomm_server(this);
			RETM_IF(ret != BT_ERROR_NONE, "create_rfcomm_server failed");

		}

	}
}


/**
 * @function		_adapter_authorization_changed_cb
 * @since_tizen		2.3
 * @description		 Adapter Authorization Changed Cb
 * @parameter		bt_device_authorization_e: Bt Device Authorization E, char*: Char Pointer, void*: Void Pointer
 * @return		static void
 */
static void _adapter_authorization_changed_cb(bt_device_authorization_e authorization, char *remote_address, void *data)
{
	DBG(" _adapter_authorization_changed_cb ");

	DBG("_adapter_authorization_changed_cb");
	RETM_IF(NULL == data, "data is NULL");

	bluetooth_view *this = NULL;
	this = (bluetooth_view*)data;
	RETM_IF(NULL == this, "view is NULL");

	if(this->view->tbt_info->apptype == TBT_APP_BT_AUTHORIZATION)
	{

		if(authorization == BT_DEVICE_AUTHORIZED)
		{
			elm_object_item_text_set(this->selected_device_item, format_string("%s(Authorized)", this->selected_device_info->remote_name));
			#ifdef TIZEN_3_0
				ui_utils_label_set_text(this->bluetooth_label, "Authorized", "left");
			#endif
			#ifdef TIZEN_2_4
				ui_utils_label_set_text(this->bluetooth_label, "Authorized", "left");
			#endif
		}
		else if(authorization == BT_DEVICE_UNAUTHORIZED)
		{
			elm_object_item_text_set(this->selected_device_item, format_string("%s(Unauthorized)", this->selected_device_info->remote_name));
			#ifdef TIZEN_3_0
				ui_utils_label_set_text(this->bluetooth_label, "Unauthorized", "left");
			#endif
			#ifdef TIZEN_2_4
				ui_utils_label_set_text(this->bluetooth_label, "Unauthorized", "left");
			#endif
		}
	}

}


/**
 * @function		_adapter_visibility_duration_changed_cb
 * @since_tizen		2.3
 * @description		 Adapter Visibility Duration Changed Cb
 * @parameter		int: Int, void*: Void Pointer
 * @return		static void
 */
static void _adapter_visibility_duration_changed_cb(int duration, void *data)
{
	DBG(" _adapter_visibility_duration_changed_cb ");

	RETM_IF(NULL == data, "data is NULL");

	bluetooth_view *this = NULL;
	this = (bluetooth_view*)data;
	RETM_IF(NULL == this, "view is NULL");

	DBG("[_adapter_visibility_duration_changed_cb] succeeded.");
}


/**
 * @function		_adapter_device_bond_destroyed_cb
 * @since_tizen		2.3
 * @description		 Adapter Device Bond Destroyed Cb
 * @parameter		int: Int, char*: Char Pointer, void*: Void Pointer
 * @return		static void
 */
static void _adapter_device_bond_destroyed_cb(int result, char *remote_address, void *data)
{
	DBG("_adapter_device_bond_destroyed_cb");
	RETM_IF(NULL == data, "data is NULL");

	bluetooth_view *this = NULL;
	this = (bluetooth_view*)data;
	RETM_IF(NULL == this, "view is NULL");
	RETM_IF(NULL == remote_address, "remote_address is NULL");

	adapter_device_error(result);

	if(this->view->tbt_info->apptype == TBT_APP_BT_OPP_CLIENT ||
			this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT ||
			this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT_HANDSFREE ||
			this->view->tbt_info->apptype == TBT_APP_BT_AUTHORIZATION ||
			this->view->tbt_info->apptype == TBT_APP_BT_CLIENT_SOCKET ||
		   this->view->tbt_info->apptype == TBT_APP_BT_CLIENT_PAIRED ||
		   this->view->tbt_info->apptype == TBT_APP_BT_HID_CONNECT ||
		   this->view->tbt_info->apptype == TBT_APP_BT_SDP ||
		   this->view->tbt_info->apptype == TBT_APP_BT_AVRCP ||
		   this->view->tbt_info->apptype == TBT_APP_BT_HDP)
		{
			if(this->selected_device_info != NULL &&
			   result == BT_ERROR_NONE &&
			   !strcmp(remote_address, this->selected_device_info->remote_address))
			{
				util_state_object_feed(this->state_object_bt_client_pair, BT_UNPAIR_SUCCESS, NULL);
			}
		}
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
	DBG("_adapter_device_bond_created_cb");
	RETM_IF(NULL == data, "data is NULL");

	bluetooth_view *this = NULL;
	this = (bluetooth_view*)data;
	RETM_IF(NULL == this, "view is NULL");

	RETM_IF(NULL == device_info, "device_info is NULL");

	adapter_device_error(result);

	DBG("_adapter_device_bond_created_cb");

	if(this->state_object_bt_client_pair )
	{
		if(device_info != NULL &&
		   this->selected_device_info != NULL &&
		   result == BT_ERROR_NONE &&
		   !strcmp(device_info->remote_address, this->selected_device_info->remote_address))
		{
			if(device_info->is_bonded/* && device_info->is_connected*/)
			{

					DBG("###Bond is created!!!");
					do_bt_common_feed(this, BT_BOND_SUCCESS, (void*)device_info);

				if(this->view->tbt_info->apptype == TBT_APP_BT_OPP_CLIENT ||
					this->view->tbt_info->apptype == TBT_APP_BT_AUTHORIZATION ||
				   this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT  ||
				   this->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT_HANDSFREE ||
				   this->view->tbt_info->apptype == TBT_APP_BT_HID_CONNECT ||
				   this->view->tbt_info->apptype == TBT_APP_BT_AVRCP ||
				   this->view->tbt_info->apptype == TBT_APP_BT_HDP)
				{
					elm_object_disabled_set(this->action_btn, EINA_FALSE);
				}
				else if(this->view->tbt_info->apptype == TBT_APP_BT_PROFILES)
				{
					set_connected_profiles(this);
				}

			}

		}
		else if(result == BT_ERROR_CANCELLED)
		{
			do_bt_common_feed(this, BT_BOND_CANCEL, (void*)device_info);
			util_state_object_feed(this->state_object_bt_client_pair, BT_BOND_CANCEL, NULL);
			DBG("_adapter_device_bond_created_cb: BT_ERROR_CANCELLED");
		}
		else if(result == BT_ERROR_AUTH_FAILED)
		{
			util_state_object_feed(this->state_object_bt_client_pair, BT_BOND_AUTH_FAIL, NULL);
			DBG("_adapter_device_bond_created_cb: BT_ERROR_AUTH_FAILED");
		}
		else if(result ==  BT_ERROR_TIMED_OUT)
		{
			util_state_object_feed(this->state_object_bt_client_pair, BT_BOND_TIME_OUT, NULL);
			DBG("_adapter_device_bond_created_cb: BT_ERROR_TIMED_OUT");
		}
		else if(result ==  BT_ERROR_REMOTE_DEVICE_NOT_FOUND)
		{
			util_state_object_feed(this->state_object_bt_client_pair, BT_BOND_DEVICE_LOST, NULL);
			DBG("_adapter_device_bond_created_cb: BT_ERROR_REMOTE_DEVICE_NOT_FOUND");
		}
	}
}


/**
 * @function		_adapter_socket_data_received_cb
 * @since_tizen		2.3
 * @description		 Adapter Socket Data Received Cb
 * @parameter		bt_socket_received_data_s*: Bt Socket Received Data S Pointer, void*: Void Pointer
 * @return		static void
 */
static void  _adapter_socket_data_received_cb(bt_socket_received_data_s *socket_data, void *data)
{
	DBG(" _adapter_socket_data_received_cb ");


	RETM_IF(NULL == data, "data is NULL");
	char* str;

	bluetooth_view *this = NULL;
	this = (bluetooth_view*)data;
	RETM_IF(NULL == this, "view is NULL");

	RETM_IF(NULL == socket_data, "socket_data is NULL");

	int result = 0;

	if(this->view->tbt_info->apptype == TBT_APP_BT_CLIENT_PAIRED ||
		this->view->tbt_info->apptype == TBT_APP_BT_SERVER_SOCKET||
		this->view->tbt_info->apptype == TBT_APP_BT_CLIENT_SOCKET )
	{

		str = format_string("Data: %s", socket_data->data);
		ui_utils_label_set_text(this->bluetooth_label,str , "left");
		SAFE_DELETE(str);

		if(this->view->tbt_info->apptype == TBT_APP_BT_SERVER_SOCKET)
		{
			result = bt_socket_send_data(socket_data->socket_fd, socket_data->data, socket_data->data_size);
			RETM_IF(result < 0 , "bt_socket_send_data fail > Error = %s", get_bluetooth_error(result));
		}

	}

}


/**
 * @function		_adapter_socket_connection_state_changed_cb
 * @since_tizen		2.3
 * @description		 Adapter Socket Connection State Changed Cb
 * @parameter		int: Int, bt_socket_connection_s*: Bt Socket Connection S Pointer, void*: Void Pointer
 * @return		static void
 */
static void _adapter_socket_connection_state_changed_cb(int result, bt_socket_connection_state_e connection_state, bt_socket_connection_s *connection, void *data)
{


	DBG("_adapter_socket_connection_state_changed_cb = %d  ", result );
	RETM_IF(NULL == data, "data is NULL");

	bluetooth_view *this = NULL;
	this = (bluetooth_view*)data;
	RETM_IF(NULL == this, "view is NULL");

   adapter_device_error(result);

	if (connection != NULL)
	{
		DBG("_adapter_socket_connection_state_changed_cb socket_fd: %d", connection->socket_fd);
	}
	else
	{
		DBG("Callback: No connection data");
		return;
	}

   if (connection_state == BT_SOCKET_CONNECTED && this->view->tbt_info->apptype == TBT_APP_BT_CLIENT_SOCKET)
   {
		DBG("connection_state == BT_SOCKET_CONNECTED ");
		ui_utils_label_set_text(this->bluetooth_label, "Connected. Sending data..", "left");
		this->server_socket_fd = connection->socket_fd;
		DBG("server_socket_fd: %d", this->server_socket_fd);
		char* tbt_string = "Hello this is TBT";
		char* str;

		result=BT_ERROR_AGAIN;
		do
		{
		 DBG("bt_socket_send_data :: TRYING... %d ", result);
		 result = bt_socket_send_data (this->server_socket_fd, tbt_string, strlen(tbt_string));
		} while(result==BT_ERROR_AGAIN);

		if(result > 0)
		{
		 str = format_string("Data sent..%d byte", result);
		 ui_utils_label_set_text(this->bluetooth_label, str, "left");
		 SAFE_DELETE(str);
		}
		else
		{
		 str = format_string("Data sent failed! error=%d",result);
		 ui_utils_label_set_text(this->bluetooth_label,str , "left");
		 SAFE_DELETE(str);
		}
		if(result==BT_ERROR_NOT_INITIALIZED) DBG("bt_socket_send_data :: ERROR %d ", result);
		else  if(result==BT_ERROR_PERMISSION_DENIED) DBG("bt_socket_send_data :: ERROR %d ", result);
		else  DBG("bt_socket_send_data :: UNKNOWN_ERROR %d ", result);

		adapter_device_error(result);
		DBG("bt_socket_send_data :: result %d ", result);
		result  = bt_socket_disconnect_rfcomm (this->server_socket_fd);
		DBG("bt_socket_disconnect_rfcomm :: result %d ", result);
   }
   else
   {
		DBG("Callback: Disconnected.");
		DBG("Callback: Socket of disconnection - %d.", connection->socket_fd);
		DBG("Callback: Address of connection - %s.", connection->remote_address);

   }
}


/**
 * @function		_adapter_bonded_device_cb
 * @since_tizen		2.3
 * @description		 Adapter Bonded Device Cb
 * @parameter		bt_device_info_s*: Bt Device Info S Pointer, void*: Void Pointer
 * @return		static bool
 */
static bool _adapter_bonded_device_cb(bt_device_info_s *device_info, void *data)
{
	DBG(" send_rfcomm_send_client_data ");

	RETVM_IF(NULL == data, false, "data is NULL");
	RETVM_IF(NULL == device_info, false, "device_info is NULL");

	bluetooth_view *this = NULL;
	this = (bluetooth_view*)data;
	RETVM_IF(NULL == this, false, "view is NULL");

	bt_device_info_s * device_info_bonded;
	device_info_bonded = malloc(sizeof(bt_device_info_s));

	if(device_info_bonded != NULL)
	{
		memcpy(device_info_bonded, device_info, sizeof(bt_device_info_s));
		device_info_bonded->remote_address = strdup(device_info->remote_address);
		device_info_bonded->remote_name = strdup(device_info->remote_name);

		//Hold the last deviceinfo pointer;
		this->bonded_devices_list = g_list_append(this->bonded_devices_list, (gpointer)device_info_bonded);
	}
	return true;
}


/**
 * @function		_adapter_device_connected_profile_cb
 * @since_tizen		2.3
 * @description		 Adapter Device Connected Profile Cb
 * @parameter		bt_profile_e: Bt Profile E, void*: Void Pointer
 * @return		static bool
 */
static bool _adapter_device_connected_profile_cb(bt_profile_e profile, void *data)
{
	DBG(" _adapter_device_connected_profile_cb");

	char* str;
	RETVM_IF(NULL == data, false,"data is NULL");

	bluetooth_view *this = NULL;
	this = (bluetooth_view*)data;
	RETVM_IF(NULL == this, false,"view is NULL");


	int ret;
	bool connected_status = false;
	ret = bt_device_is_profile_connected (this->selected_device_info->remote_address, profile, &connected_status);
	RETVM_IF(ret != BT_ERROR_NONE, false, "bt_device_is_profile_connected failed");
	RETVM_IF(connected_status != true, false, "profile not connected");


	DBG("_adapter_device_connected_profile_cb called");

	if(this->no_of_profiles == 0)
	{
		str = format_string("%s(Profiles)",this->selected_device_info->remote_name);
		ui_utils_label_set_text(this->bluetooth_label,str , "left");
		elm_list_clear(this->bluetooth_list);
		elm_list_item_append(this->bluetooth_list, this->selected_device_info->remote_name, NULL, NULL, NULL, NULL);
		SAFE_DELETE(str);
	}

	if(this->view->tbt_info->apptype == TBT_APP_BT_PROFILES)
	{
		str = format_string("%s", get_bt_profile_name(profile));
		this->selected_device_profile_list = g_list_append(this->selected_device_profile_list, (gpointer)profile);
		elm_list_item_append(this->bluetooth_list, str, NULL, NULL, NULL, NULL);
		elm_object_disabled_set(this->bluetooth_btn, EINA_TRUE);
		SAFE_DELETE(str);
	}
	this->no_of_profiles++;
	return true;
}


/**
 * @function		_adapter_opp_client_push_responded_cb
 * @since_tizen		2.3
 * @description		 Adapter Opp Client Push Responded Cb
 * @parameter		int: Int, const char*: Const char Pointer, void*: Void Pointer
 * @return		static void
 */
static void _adapter_opp_client_push_responded_cb(int result, const char *remote_address, void *data)
{
	DBG(" _adapter_opp_client_push_responded_cb ");

	RETM_IF(NULL == data, "data is NULL");

	bluetooth_view *this = NULL;
	this = (bluetooth_view*)data;
	RETM_IF(NULL == this,"view is NULL");

	adapter_device_error(result);
	RETM_IF(NULL == data, "data is NULL");

	bluetooth_view *view = NULL;
	view = (bluetooth_view*)data;
	RETM_IF(NULL == view, "view is NULL");

	elm_object_disabled_set(view->bluetooth_check_button, EINA_FALSE);

	if(result == BT_ERROR_NONE)
	{
		ui_utils_label_set_text(this->bluetooth_label, "File Push Accepted","Left");
	}
	else if(result == BT_ERROR_OPERATION_FAILED)
	{
		ui_utils_label_set_text(this->bluetooth_label, "File Push Rejected","Left");
	}
	else
	{
		ERR("_adapter_opp_client_push_responded_cb failed: %s", get_bluetooth_error(result));
	}
}


/**
 * @function		_adapter_opp_client_push_progress_cb
 * @since_tizen		2.3
 * @description		 Adapter Opp Client Push Progress Cb
 * @parameter		const char*: Const char Pointer, void*: Void Pointer
 * @return		static void
 */
static void _adapter_opp_client_push_progress_cb(const char *file, long long size, int percent, void *data)
{
	DBG(" _adapter_opp_client_push_progress_cb ");

	RETM_IF(NULL == data, "data is NULL");
	RETM_IF(NULL == file, "file is NULL");

	bluetooth_view *this = NULL;
	this = (bluetooth_view*)data;
	RETM_IF(NULL == this, "view is NULL");
	elm_object_disabled_set(this->action_btn, EINA_TRUE);
	elm_object_disabled_set(this->bluetooth_btn, EINA_TRUE);
	elm_object_disabled_set(this->bluetooth_btn, EINA_FALSE);

	DBG("size: %ld", (long)size);
	DBG("percent: %d", percent);
	DBG("file: %s", file);
	char* str = format_string("File Send %d %", percent);
	ui_utils_label_set_text(this->bluetooth_label, str, "left");
	SAFE_DELETE(str);
}


/**
 * @function		_adapter_opp_client_push_finished_cb
 * @since_tizen		2.3
 * @description		 Adapter Opp Client Push Finished Cb
 * @parameter		int: Int, const char*: Const char Pointer, void*: Void Pointer
 * @return		static void
 */
static void _adapter_opp_client_push_finished_cb(int result, const char *remote_address, void *data)
{
	DBG(" _adapter_opp_client_push_finished_cb ");

	RETM_IF(NULL == data, "data is NULL");
	RETM_IF(NULL == remote_address, "remote_address is NULL");

	bluetooth_view *this = NULL;
	this = (bluetooth_view*)data;
	RETM_IF(NULL == this, "view is NULL");

	if(result != BT_ERROR_NONE)
	{
		elm_object_disabled_set(this->action_btn, EINA_FALSE);
		ui_utils_label_set_text(this->bluetooth_label, "File Send Failed", "left");

		dlog_print(DLOG_INFO, LOG_TAG, "remote_address: %s", remote_address);
		DBG("[_adapter_opp_client_push_finished_cb]  %d Failed.", result);
	}
	else
	{
		elm_object_disabled_set(this->action_btn, EINA_FALSE);
		if(this->is_opp_push_cancelled == true)
		{
			ui_utils_label_set_text(this->bluetooth_label, "File Send Cancelled", "left");
			this->is_opp_push_cancelled = false;
		}
		else
		{
			ui_utils_label_set_text(this->bluetooth_label, "File Send Successful", "left");
		}
		DBG("[_adapter_opp_client_push_finished_cb] Success.");
	}

	// Delete file info
	result = bt_opp_client_clear_files();
	RETM_IF(result != BT_ERROR_NONE, "bt_opp_client_clear_files fail > Error = %d", result);

	result = bt_opp_client_deinitialize();
	RETM_IF(result != BT_ERROR_NONE, "bt_opp_client_deinitialize fail > Error = %d", result);

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
		bt_adapter_device_discovery_info_s * device_info_a;
		device_info_a = (bt_adapter_device_discovery_info_s*)a;
		RETVM_IF(NULL == device_info_a, -1, "device_info_a is NULL");

		bt_adapter_device_discovery_info_s * device_info_b;
		device_info_b = (bt_adapter_device_discovery_info_s*)b;
		RETVM_IF(NULL == device_info_b, -1, "device_info_b is NULL");
		if(device_info_a->remote_address && device_info_b->remote_address)
		{
			return strcmp(device_info_a->remote_address, device_info_b->remote_address);
		}
	}

	return -1;


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
 * @function		_app_destroy_cb
 * @since_tizen		2.3
 * @description		 App Destroy Cb
 * @parameter		void*: Void Pointer
 * @return		static void
 */
static void _app_destroy_cb(void* this)
{
	RETM_IF(NULL == this, "data is NULL");

	bluetooth_view *view = NULL;
	view = (bluetooth_view*)this;
	RETM_IF(NULL == view, "view is NULL");

	#ifdef DEVICE_TYPE_WEARABLE
		if(get_device_type() == DEVICE_WEARABLE_320_X_320)
		{
			elm_theme_overlay_del(NULL, get_edje_path("custom_button.edj"));
		}
	#endif

	if(view->view->tbt_info->apptype == TBT_APP_BT_SERVER_PAIRED)
	{
		util_state_object_destroy(view->state_object_bt_server_pair);
	}

	if(view->state_object_bt_client_pair)
	{
		util_state_object_destroy(view->state_object_bt_client_pair);
	}

	int result = 0;
	result = bt_adapter_unset_state_changed_cb();
	RETM_IF(result != BT_ERROR_NONE, "bt_adapter_unset_state_changed_cb Failed");

	result = bt_adapter_unset_device_discovery_state_changed_cb();
	RETM_IF(result != BT_ERROR_NONE, "bt_adapter_unset_device_discovery_state_changed_cb Failed");

	result = bt_device_unset_bond_created_cb();
	RETM_IF(result != BT_ERROR_NONE, "bt_device_uset_bond_created_cb Failed");

	result = bt_device_unset_bond_destroyed_cb();
	RETM_IF(result != BT_ERROR_NONE, "bt_device_unset_bond_destroyed_cb Failed");

	result = bt_adapter_unset_visibility_mode_changed_cb();
	RETM_IF(result != BT_ERROR_NONE, "bt_adapter_unset_visibility_mode_changed_cb Failed");

	result = bt_adapter_unset_visibility_duration_changed_cb();
	RETM_IF(result != BT_ERROR_NONE, "bt_adapter_unset_visibility_duration_changed_cb Failed");

	result = bt_device_unset_authorization_changed_cb();
	RETM_IF(result != BT_ERROR_NONE, "bt_device_unset_authorization_changed_cb Failed");

	result = bt_socket_unset_data_received_cb();
	RETM_IF(result != BT_ERROR_NONE, "bt_socket_unset_data_received_cb Failed");

	result = bt_socket_unset_connection_requested_cb();
	RETM_IF(result != BT_ERROR_NONE, "bt_socket_unset_connection_requested_cb Failed");

	result = bt_socket_unset_connection_state_changed_cb();
	RETM_IF(result != BT_ERROR_NONE, "bt_socket_unset_connection_state_changed_cb Failed");



	if(view->view->tbt_info->apptype == TBT_APP_BT_CLIENT_SOCKET)
	{
			result = bt_socket_disconnect_rfcomm (view->server_socket_fd);
			RETM_IF(result != BT_ERROR_NONE, "bt_socket_disconnect_rfcomm fail > Error = %d", result);
	}
	else if(view->view->tbt_info->apptype == TBT_APP_BT_OPP_SERVER)
	{
			result = bt_opp_server_deinitialize();
			RETM_IF(result != BT_ERROR_NONE, "bt_opp_server_deinitialize fail > Error = %d", result);
	}
	else if(view->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT ||
			view->view->tbt_info->apptype == TBT_APP_BT_AUDIO_CONNECT_HANDSFREE ||
			view->view->tbt_info->apptype == TBT_APP_BT_PROFILES ||
			view->view->tbt_info->apptype == TBT_APP_BT_AG)
	{

		if(view->selected_device_info != NULL)
		{
			result = bt_audio_unset_connection_state_changed_cb();
			RETM_IF(result != BT_ERROR_NONE, "bt_audio_unset_connection_state_changed_cb fail > Error = %s", get_bluetooth_error(result));

			result = bt_audio_disconnect(view->selected_device_info->remote_address, BT_AUDIO_PROFILE_TYPE_ALL);
			RETM_IF(result != BT_ERROR_NONE, "bt_audio_disconnect fail > Error = %s", get_bluetooth_error(result));

			result = bt_audio_deinitialize();
			RETM_IF(result != BT_ERROR_NONE, "bt_audio_deinitialize fail > Error = %d", result);
		}
	}
	else if(view->view->tbt_info->apptype == TBT_APP_BT_HID_CONNECT)
	{
		//HID Connect deinitialization code goes here
		result = bt_hid_host_deinitialize();
		RETM_IF(result != BT_ERROR_NONE, "bt_hid_host_deinitialize fail > Error = %s", get_bluetooth_error(result));
		result = bt_hid_host_disconnect(view->selected_device_info->remote_address);
		RETM_IF(result != BT_ERROR_NONE, "bt_hid_host_disconnect fail > Error = %s", get_bluetooth_error(result));

	}
	else if(view->view->tbt_info->apptype == TBT_APP_BT_AVRCP)
	{
		//AVRCP connect deinitialization code goes here
		result = bt_avrcp_unset_equalizer_state_changed_cb();
		RETM_IF(result != BT_ERROR_NONE, "bt_avrcp_unset_equalizer_state_changed_cb fail > Error = %s", get_bluetooth_error(result));
		result = bt_avrcp_unset_repeat_mode_changed_cb();
		RETM_IF(result != BT_ERROR_NONE, "bt_avrcp_unset_repeat_mode_changed_cb fail > Error = %s", get_bluetooth_error(result));
		result = bt_avrcp_unset_shuffle_mode_changed_cb();
		RETM_IF(result != BT_ERROR_NONE, "bt_avrcp_unset_shuffle_mode_changed_cb fail > Error = %s", get_bluetooth_error(result));
		result = bt_avrcp_unset_scan_mode_changed_cb();
		RETM_IF(result != BT_ERROR_NONE, "bt_avrcp_unset_scan_mode_changed_cb fail > Error = %s", get_bluetooth_error(result));
		result = bt_avrcp_target_deinitialize();
		RETM_IF(result != BT_ERROR_NONE, "bt_avrcp_target_deinitialize fail > Error = %s", get_bluetooth_error(result));
	}
	else if(view->view->tbt_info->apptype == TBT_APP_BT_HDP)
	{
		//HDP connect denitialization code goes here
		result = bt_hdp_unset_connection_state_changed_cb();
		RETM_IF(result != BT_ERROR_NONE, "bt_hdp_unset_connection_state_changed_cb error: %s", get_bluetooth_error(result));
		result = bt_hdp_unset_data_received_cb();
		RETM_IF(result != BT_ERROR_NONE, "bt_hdp_unset_data_received_cb error: %s", get_bluetooth_error(result));
		result = bt_hdp_unregister_sink_app(view->hdp_app_id);
		RETM_IF(result != BT_ERROR_NONE, "bt_hdp_unregister_sink_app error: %s", get_bluetooth_error(result));
	}
	else if(view->view->tbt_info->apptype == TBT_APP_BT_AG)
	{
		#ifdef TBT_MODULE_APP_BT_AG
			bool isOpened;
			result = bt_ag_is_sco_opened(&isOpened);
			RETM_IF(result != BT_ERROR_NONE, "bt_ag_is_sco_opened fail > Error = %s", get_bluetooth_error(result));
			if(isOpened)
			{
				result = bt_ag_close_sco();
				RETM_IF(result != BT_ERROR_NONE, "bt_ag_close_sco fail > Error = %s", get_bluetooth_error(result));
				result = bt_ag_unset_sco_state_changed_cb();
			}
		#endif
	}
	else if(view->view->tbt_info->apptype == TBT_APP_BT_SDP)
	{
		result = bt_device_unset_service_searched_cb();
		RETM_IF(result != BT_ERROR_NONE, "bt_device_unset_service_searched_cb Failed");
	}


	if(view->discovery_state == BT_ADAPTER_DEVICE_DISCOVERY_STARTED || view->discovery_state == BT_ADAPTER_DEVICE_DISCOVERY_FOUND)
	{
		result = bt_adapter_stop_device_discovery();
		RETM_IF(result != BT_ERROR_NONE, "bt_adapter_stop_device_discovery error: %s", get_bluetooth_error(result));
	}
	// Release resources
	g_list_free_full(view->devices_list, bluetooth_list_free_func_cb);
	view->devices_list = NULL;

	//Deinitialize Bluetooth
	result = bt_deinitialize();
	if(result != BT_ERROR_NONE)
	{
	   DBG("[bt_deinitialize] Failed.");
	}

	if(view->bt_player)
	{
		player_stop(view->bt_player);
		player_unprepare(view->bt_player);
		player_destroy(view->bt_player);
		view->bt_player = NULL;
	}

	SAFE_DELETE(view->view);
	SAFE_DELETE(view);


}


/**
 * @function		_adapter_bt_audio_connection_state_changed_cb
 * @since_tizen		2.3
 * @description		 Adapter Bt Audio Connection State Changed Cb
 * @parameter		int: Int, const char*: Const char Pointer, void*: Void Pointer
 * @return		static void
 */
static void _adapter_bt_audio_connection_state_changed_cb(int result, bool connected, const char *remote_address, bt_audio_profile_type_e type, void *user_data)
{

	bluetooth_view *view = (bluetooth_view*) user_data;
	int ret;

	DBG(" _adapter_bt_audio_connection_state_changed_cb ");
	RETM_IF(NULL == remote_address, "remote_address NULL");
	RETM_IF(NULL == user_data, "user_data NULL");

	DBG("The value of the result is %d and BT_ERROR_NOT_INITIALIZED is %d", result, BT_ERROR_NOT_INITIALIZED);
	if(connected)
	{
		DBG("Bluetooth audio connected");
		elm_object_text_set(view->bluetooth_label, "Audio Device  connected");

		if (!view->popup_state)
		{
			/* LHtest */
			view->popup = ui_utils_popup_add(view->view->navi, "Audio Device connected!!");
			view->popup_state = 1;
			char msg[512];
			snprintf(msg, sizeof(msg),"<br/>For Disconnecting,<br/>Press OK Button");
			elm_object_text_set(view->popup, msg);
			ui_utils_popup_button_add(view->popup, "OK", _popup_destroy_cb, view);
		}

#if 0

		if(view->view->tbt_info->apptype == TBT_APP_BT_PROFILES)
		{
			DBG("set_connected_profiles if condition: %s", view->selected_device_info->remote_name);
			ret = bt_device_foreach_connected_profiles(view->selected_device_info->remote_address, _adapter_device_connected_profile_cb, view);
			adapter_device_error(result);
			RETM_IF(ret != BT_ERROR_NONE, "bt_device_foreach_connected_profiles fail > Error = %d", ret);
		}
#endif
	}
	else
	{
		DBG("Bluetooth audio disconnected");
	}
}


/**
 * @function		_adapter_bt_device_service_searched_cb
 * @since_tizen		2.3
 * @description		 Adapter Bt Device Service Searched Cb
 * @parameter		int: Int, bt_device_sdp_info_s*: Bt Device Sdp Info S Pointer, void*: Void Pointer
 * @return		static void
 */
static void _adapter_bt_device_service_searched_cb(int result, bt_device_sdp_info_s *sdp_info, void *user_data)
{
	DBG(" _adapter_bt_device_service_searched_cb ");
	RETM_IF(NULL == user_data, "data is NULL");
	RETM_IF(NULL == sdp_info, "sdp_info is NULL");
	bt_service_class_t 	service_mask_list;
	int ret;

	if(sdp_info->service_count)
	{
		int i;
		for(i = 0; i<sdp_info->service_count; i++)
		{
			DBG("Service UUID %d : %s",i, sdp_info->service_uuid[i]);
		}
		ret = bt_device_get_service_mask_from_uuid_list(sdp_info->service_uuid, sdp_info->service_count, &service_mask_list);
		RETM_IF(ret != BT_ERROR_NONE, "bt_device_get_service_mask_from_uuid_list error: %s", get_bluetooth_error(ret));


		DBG("SERVICE MASK: %s", bt_get_service_name(service_mask_list));
		bluetooth_view *this = NULL;
		this = (bluetooth_view*)user_data;
		RETM_IF(NULL == this, "view is NULL");

		this->service_list = bt_get_service_name(service_mask_list);
		if(!this->service_list)
		{
			DBG("this->service_list is NULL");
		}
	}

}



/**
 * @function		_bt_hid_host_connection_state_changed_cb
 * @since_tizen		2.3
 * @description		 Bt Hid Host Connection State Changed Cb
 * @parameter		int: Int, const char*: Const char Pointer, void*: Void Pointer
 * @return		static void
 */
static void _bt_hid_host_connection_state_changed_cb(int result, bool connected, const char *remote_address, void *user_data)
{
	bluetooth_view *this = (bluetooth_view*) user_data;
	RETM_IF(NULL == this, "view is NULL");
	RETM_IF(NULL == remote_address, "remote_address is NULL");
	DBG("_bt_hid_host_connection_state_changed_cb status :%d ", connected);
	if(connected)
	{
		elm_object_text_set(this->bluetooth_label, "HID connected");

		/* LHtest */
		this->popup = ui_utils_popup_add(this->view->navi, "HID connected!!");
		char msg[512];
//		snprintf(msg, sizeof(msg),"<b>'tttttt'</b><br/>requires feature:<br/>'pppppp'<br/>Which is not supported.");
		snprintf(msg, sizeof(msg),"<br/>For Disconnecting,<br/>Press OK Button");
		elm_object_text_set(this->popup, msg);
		ui_utils_popup_button_add(this->popup, "OK", _popup_destroy_cb, this);

	}
}


/**
 * @function		_bt_avrcp_target_connection_state_changed_cb
 * @since_tizen		2.3
 * @description		 Bt Avrcp Target Connection State Changed Cb
 * @parameter		bool: Bool, const char*: Const char Pointer, void*: Void Pointer
 * @return		static void
 */
static void _bt_avrcp_target_connection_state_changed_cb(bool connected, const char *remote_address, void *user_data)
{
	DBG("Inside _bt_avrcp_target_connection_state_changed_cb");
	bluetooth_view *this = (bluetooth_view*) user_data;
	RETM_IF(NULL == this, "view is NULL");
	RETM_IF(NULL == remote_address, "remote_address is NULL");
	if(connected)
	{
		elm_object_text_set(this->bluetooth_label, "AVRCP connected");
	}
	else
	{
		elm_object_text_set(this->bluetooth_label, "AVRCP connection Failed");
	}
}


/**
 * @function		_bt_avrcp_equalizer_state_changed_cb
 * @since_tizen		2.3
 * @description		 Bt Avrcp Equalizer State Changed Cb
 * @parameter		bt_avrcp_equalizer_state_e: Bt Avrcp Equalizer State E, void*: Void Pointer
 * @return		static void
 */
static void _bt_avrcp_equalizer_state_changed_cb(bt_avrcp_equalizer_state_e equalizer, void *user_data)
{
	DBG("Inside _bt_avrcp_equalizer_state_changed_cb");
	bluetooth_view *this = (bluetooth_view*) user_data;
	RETM_IF(NULL == this, "view is NULL");
	if(equalizer == BT_AVRCP_EQUALIZER_STATE_ON)
	{
		elm_object_text_set(this->bluetooth_label, "Equalizer ON");
	}
	else
	{
		elm_object_text_set(this->bluetooth_label, "Equalizer OFF");
	}
}


/**
 * @function		_bt_avrcp_repeat_mode_changed_cb
 * @since_tizen		2.3
 * @description		 Bt Avrcp Repeat Mode Changed Cb
 * @parameter		bt_avrcp_repeat_mode_e: Bt Avrcp Repeat Mode E, void*: Void Pointer
 * @return		static void
 */
static void _bt_avrcp_repeat_mode_changed_cb(bt_avrcp_repeat_mode_e repeat, void *user_data)
{
	DBG("Inside _bt_avrcp_repeat_mode_changed_cb");
	bluetooth_view *this = (bluetooth_view*) user_data;
	RETM_IF(NULL == this, "view is NULL");

switch(repeat)
{
	case BT_AVRCP_REPEAT_MODE_OFF:
		elm_object_text_set(this->bluetooth_label, "Repeat Mode OFF");
		break;
	case BT_AVRCP_REPEAT_MODE_SINGLE_TRACK:
		elm_object_text_set(this->bluetooth_label, "Repeat Mode Single");
		break;
	case BT_AVRCP_REPEAT_MODE_ALL_TRACK:
		elm_object_text_set(this->bluetooth_label, "Repeat Mode All");
		break;
	case BT_AVRCP_REPEAT_MODE_GROUP:
		elm_object_text_set(this->bluetooth_label, "Repeat Mode Group");
		break;
	}
}


/**
 * @function		_bt_avrcp_shuffle_mode_changed_cb
 * @since_tizen		2.3
 * @description		 Bt Avrcp Shuffle Mode Changed Cb
 * @parameter		bt_avrcp_shuffle_mode_e: Bt Avrcp Shuffle Mode E, void*: Void Pointer
 * @return		static void
 */
static void _bt_avrcp_shuffle_mode_changed_cb(bt_avrcp_shuffle_mode_e shuffle, void *user_data)
{
	DBG("Inside _bt_avrcp_shuffle_mode_changed_cb");
	bluetooth_view *this = (bluetooth_view*) user_data;
	RETM_IF(NULL == this, "view is NULL");

	if(shuffle == BT_AVRCP_SHUFFLE_MODE_OFF)
	{
		elm_object_text_set(this->bluetooth_label, "Shuffle Mode OFF");
	}
	else if(BT_AVRCP_SHUFFLE_MODE_ALL_TRACK)
	{
		elm_object_text_set(this->bluetooth_label, "Shuffle Mode ALL");
	}
	else if(BT_AVRCP_SHUFFLE_MODE_GROUP)
	{
		elm_object_text_set(this->bluetooth_label, "Shuffle Mode Group");
	}

}


/**
 * @function		_bt_avrcp_scan_mode_changed_cb
 * @since_tizen		2.3
 * @description		 Bt Avrcp Scan Mode Changed Cb
 * @parameter		bt_avrcp_scan_mode_e: Bt Avrcp Scan Mode E, void*: Void Pointer
 * @return		static void
 */
static void _bt_avrcp_scan_mode_changed_cb(bt_avrcp_scan_mode_e scan, void *user_data)
{
	DBG("Inside _bt_avrcp_scan_mode_changed_cb");
	bluetooth_view *this = (bluetooth_view*) user_data;
	RETM_IF(NULL == this, "view is NULL");

    if(scan == BT_AVRCP_SCAN_MODE_OFF)
    {
    	elm_object_text_set(this->bluetooth_label, "Scan Mode OFF");
    }
    else if(scan == BT_AVRCP_SCAN_MODE_ALL_TRACK)
    {
    	elm_object_text_set(this->bluetooth_label, "Scan Mode All");
    }
    else if(scan == BT_AVRCP_SCAN_MODE_GROUP)
    {
    	elm_object_text_set(this->bluetooth_label, "Scan Mode Group");
    }
}


/**
 * @function		_adpter_bt_opp_server_connection_requested_cb
 * @since_tizen		2.3
 * @description		 Adpter Bt Opp Server Connection Requested Cb
 * @parameter		const char*: Const char Pointer, void*: Void Pointer
 * @return		static void
 */
static void _adpter_bt_opp_server_connection_requested_cb(const char *remote_address, void *user_data)
{
	bluetooth_view *this = (bluetooth_view*) user_data;
	RETM_IF(NULL == this, "view is NULL");
	RETM_IF(NULL == remote_address, "remote_address is NULL");
	DBG(" _adpter_bt_opp_server_connection_requested_cb ");
	elm_object_text_set(this->bluetooth_label,"OPP Requested...");
	elm_object_text_set(this->bluetooth_btn,"Accept");
	elm_object_text_set(this->action_btn,"Reject");
	elm_object_disabled_set(this->bluetooth_btn, EINA_FALSE);
	elm_object_disabled_set(this->action_btn, EINA_FALSE);
	this->is_opp_server_accecpt = EINA_TRUE;
}


/**
 * @function		_adpter_bt_opp_server_transfer_progress_cb_for_opp
 * @since_tizen		2.3
 * @description		 Adpter Bt Opp Server Transfer Progress Cb For Opp
 * @parameter		const char*: Const char Pointer, void*: Void Pointer
 * @return		static void
 */
static void _adpter_bt_opp_server_transfer_progress_cb_for_opp(const char *file, long long size, int percent, void *user_data)
{
	bluetooth_view *this = (bluetooth_view *)user_data;
	RETM_IF(NULL == this, "view is NULL");
	RETM_IF(NULL == file, "file is NULL");
	DBG(" _adpter_bt_opp_server_transfer_progress_cb_for_opp ");
	DBG("OPP Server file: %s", file);
	DBG("OPP Server size: %lld", size);
	DBG("OPP Server percent: %d", percent);
	this->is_opp_server_push_progress = EINA_TRUE;
	elm_object_text_set(this->bluetooth_btn, "Cancel");

	char *msg = format_string("Progress : %d", percent);
	elm_object_text_set(this->bluetooth_label, msg);
}


/**
 * @function		_adapter_bt_opp_server_transfer_finished_cb_for_opp
 * @since_tizen		2.3
 * @description		 Adapter Bt Opp Server Transfer Finished Cb For Opp
 * @parameter		int: Int, const char*: Const char Pointer, void*: Void Pointer
 * @return		static void
 */
static void _adapter_bt_opp_server_transfer_finished_cb_for_opp(int result, const char *file, long long size, void *user_data)
{
	bluetooth_view *this = (bluetooth_view *)user_data;
	RETM_IF(NULL == this, "view is NULL");
	RETM_IF(NULL == file, "file is NULL");
	DBG(" _adpter_bt_opp_server_transfer_progress_cb_for_opp ");
	DBG("OPP Server result: %d", result);
	DBG("OPP Server file: %s", file);
	DBG("OPP Server size: %ld", size);
	elm_object_disabled_set(this->bluetooth_btn, EINA_TRUE);
	elm_object_disabled_set(this->action_btn, EINA_TRUE);
}


/**
 * @function		get_bt_profile_name
 * @since_tizen		2.3
 * @description		Get Bt Profile Name
 * @parameter		bt_profile_e: Bt Profile E
 * @return		static char*
 */
static char* get_bt_profile_name(bt_profile_e profile)
{
	char* profile_name;

switch(profile)
{
case BT_PROFILE_RFCOMM:
	profile_name = "BT_PROFILE_RFCOMM";
	break;
case BT_PROFILE_A2DP:
	profile_name = "BT_PROFILE_A2DP";
	break;
case BT_PROFILE_HSP:
	profile_name = "BT_PROFILE_HSP";
	break;
case BT_PROFILE_HID:
	profile_name = "BT_PROFILE_HID";
	break;
case BT_PROFILE_NAP:
	profile_name = "BT_PROFILE_NAP";
	break;
case BT_PROFILE_AG:
	profile_name = "BT_PROFILE_AG";
	break;
case BT_PROFILE_GATT:
	profile_name = "BT_PROFILE_GATT";
	break;
case BT_PROFILE_NAP_SERVER:
	profile_name = "BT_PROFILE_NAP_SERVER";
	break;
default:
	profile_name = "UNKNOWN";
	break;
}
return profile_name;
}


/**
 * @function		_bt_util_state_object_state_changed_cb_server_pair
 * @since_tizen		2.3
 * @description		 Bt Util State Object State Changed Cb Server Pair
 * @parameter		int: Int, int: Int, int: Int, void*: Void Pointer, void*: Void Pointer
 * @return		static void
 */
static void _bt_util_state_object_state_changed_cb_server_pair(int previous,int now,int feed,void *user_data,void *feed_data)
{
	DBG("Current state is %d while previous state is %d", now, previous);
	bluetooth_view *this = NULL;
	this = (bluetooth_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");
	
	if(now < BT_BOND_CLIENT_SUCCESSFULLY_CREATED)
	{
		do_bt_state_common_check(this, now, feed_data);
	}
}


/**
 * @function		_bt_util_state_object_state_changed_cb_client_pair
 * @since_tizen		2.3
 * @description		 Bt Util State Object State Changed Cb Client Pair
 * @parameter		int: Int, int: Int, int: Int, void*: Void Pointer, void*: Void Pointer
 * @return		static void
 */
static void _bt_util_state_object_state_changed_cb_client_pair(int previous,int now,int feed,void *user_data,void *feed_data)
{
	DBG("Current state is %d while previous state is %d", now, previous);
	bluetooth_view *this = NULL;
	this = (bluetooth_view*)user_data;
	RETM_IF(NULL == this, "view is NULL");
	
	int result = BT_ERROR_NONE;
	if(now < BT_BOND_CLIENT_SUCCESSFULLY_CREATED && now != BT_BOND_CANCELLED)
	{
		do_bt_state_common_check(this, now, feed_data);
	}

	//else
	{

switch(now)
{
			case BT_BOND_CLIENT_SUCCESSFULLY_CREATED:
			{
				bt_adapter_device_discovery_info_s* device_info = (bt_adapter_device_discovery_info_s*)feed_data;
				DBG("_adapter_device_bond_created_cb bond succesful with: %s", device_info->remote_address);
				GList *paired_device = NULL;
				bt_adapter_device_discovery_info_s * device_info_paired = NULL;

				if(this->view->tbt_info->apptype == TBT_APP_BT_SDP)
				{
					bt_device_start_service_search(device_info->remote_address);
				}


				paired_device = g_list_find_custom (this->devices_list, this->selected_device_info, bluetooth_list_find_func_cb);
				RETM_IF(NULL == paired_device, "paired_device is NULL");
				device_info_paired = (bt_adapter_device_discovery_info_s*)paired_device->data;
				RETM_IF(NULL == device_info_paired, "device_info_paired is NULL");
				device_info_paired->is_bonded = true;

				elm_object_disabled_set(this->action_btn, EINA_FALSE);
				evas_object_data_set(this->bluetooth_list, "bluetooth_view", this);
				elm_object_item_text_set(this->selected_device_item, format_string("%s(Paired)", this->selected_device_info->remote_name));
				#ifdef TIZEN_3_0
					ui_utils_label_set_text(this->bluetooth_label, "Paired", "left");
				#endif
				#ifdef TIZEN_2_4
					ui_utils_label_set_text(this->bluetooth_label, "Paired", "left");
				#endif
				if(this->view->tbt_info->apptype == TBT_APP_BT_HDP)
				{
					// Use the appropriate IEEE 11073 data types based on the devices used.
					// Below are some examples.  Refer to relevant Bluetooth HDP specifications for detail.
					//     0x1007 - blood pressure meter
					//     0x1008 - body thermometer
					//     0x100F - body weight scale

					result = bt_hdp_register_sink_app(0x1007, &this->hdp_app_id);
					RETM_IF(result != BT_ERROR_NONE, "bt_hdp_register_sink_app error: %s", get_bluetooth_error(result));
					ui_utils_label_set_text(this->action_btn, "Connect HDP", "left");
				}

				break;
			}

			case BT_BOND_TIMED_OUT:
				elm_object_disabled_set(this->action_btn, EINA_FALSE);
				evas_object_data_set(this->bluetooth_list, "bluetooth_view", this);
				elm_object_item_text_set(this->selected_device_item, format_string("%s(Timedout)", this->selected_device_info->remote_name));
				#ifdef TIZEN_3_0
					ui_utils_label_set_text(this->bluetooth_label, "Timed Out", "left");
				#endif
				#ifdef TIZEN_2_4
					ui_utils_label_set_text(this->bluetooth_label, "Timed Out", "left");
				#endif
				break;
			case BT_BOND_REMOTE_DEVICE_NOT_FOUND:
				elm_object_disabled_set(this->action_btn, EINA_FALSE);
				evas_object_data_set(this->bluetooth_list, "bluetooth_view", this);
				elm_object_item_text_set(this->selected_device_item, format_string("%s(Device Not Found)", this->selected_device_info->remote_name));
				#ifdef TIZEN_3_0
					ui_utils_label_set_text(this->bluetooth_label, "Device Not Found", "left");
				#endif
				#ifdef TIZEN_2_4
					ui_utils_label_set_text(this->bluetooth_label, "Device Not Found", "left");
				#endif
				break;
			case BT_BOND_CANCELLED:
				elm_object_disabled_set(this->action_btn, EINA_FALSE);
				evas_object_data_set(this->bluetooth_list, "bluetooth_view", this);
				elm_object_item_text_set(this->selected_device_item, format_string("%s(Cancelled)", this->selected_device_info->remote_name));
				#ifdef TIZEN_3_0
					ui_utils_label_set_text(this->bluetooth_label, "Cancelled", "left");
				#endif
				#ifdef TIZEN_2_4
					ui_utils_label_set_text(this->bluetooth_label, "Cancelled", "left");
				#endif
				break;
			case BT_BOND_AUTH_FAILED:
				elm_object_disabled_set(this->action_btn, EINA_FALSE);
				evas_object_data_set(this->bluetooth_list, "bluetooth_view", this);
				elm_object_item_text_set(this->selected_device_item, format_string("%s(Auth Failed)", this->selected_device_info->remote_name));
				#ifdef TIZEN_3_0
					ui_utils_label_set_text(this->bluetooth_label, "Auth Failed", "left");
				#endif
				#ifdef TIZEN_2_4
					ui_utils_label_set_text(this->bluetooth_label, "Auth Failed", "left");
				#endif
				break;

			case BT_SUCCESSFULLY_UNPAIRED:
			{
				DBG("Success in unpairing ");
				GList *unpaired_device = NULL;
				bt_adapter_device_discovery_info_s * device_info_unpaired = NULL;

				unpaired_device = g_list_find_custom (this->devices_list, this->selected_device_info, bluetooth_list_find_func_cb);
				device_info_unpaired = (bt_adapter_device_discovery_info_s*)unpaired_device->data;
				device_info_unpaired->is_bonded = false;

				elm_object_disabled_set(this->action_btn, EINA_FALSE);
				evas_object_data_set(this->bluetooth_list, "bluetooth_view", this);
				elm_object_item_text_set(this->selected_device_item, this->selected_device_info->remote_name);
				#ifdef TIZEN_3_0
					ui_utils_label_set_text(this->bluetooth_label, "Unpaired", "left");
				#endif
				#ifdef TIZEN_2_4
					ui_utils_label_set_text(this->bluetooth_label, "Unpaired", "left");
				#endif
				break;
			}
		}
		if(this->view->tbt_info->apptype == TBT_APP_BT_OPP_SERVER)
		{
			elm_object_disabled_set(this->action_btn, EINA_TRUE);
		}
	}
}


/**
 * @function		do_bt_state_common_check
 * @since_tizen		2.3
 * @description		Do Bt State Common Check
 * @parameter		bluetooth_view*: Bluetooth View Pointer, void*: Void Pointer
 * @return		static void
 */
static void do_bt_state_common_check(bluetooth_view *this, int now, void *feed_data)
{
	RETM_IF(NULL == this, "view is NULL");
	

	int result;

switch(now)
{
		case BT_ACTIVATED:
			DBG("BT_ACTIVATED");
			update_bonded_devices(this);

			int* adapter_state = (int*)feed_data;

			this->adapter_state = *adapter_state;
			DBG("Adapter state is %d", *adapter_state);
			update_view_controls(this);
			break;
		case BT_ACTIVATED_CLIENT:
			elm_object_disabled_set(this->bluetooth_btn, EINA_FALSE);
			elm_object_text_set(this->bluetooth_btn, "Discover");
			ui_utils_label_set_text(this->bluetooth_label, "Ready to discover Server", "left");
			break;
		case BT_ACTIVATION_ATTEMPTED:
			result = adapter_on_off_request_send(this);
			RETM_IF(result != BT_ERROR_NONE, "adapter_on_of_request_send fail > Error = %d", result);
			break;
		case BT_DISCOVERY_START_REQUESTED:
		{
			int discover_state;
			discover_state = (int)this->discovery_state;

			if(discover_state  == BT_ADAPTER_DEVICE_DISCOVERY_NONE)
			{
				result = bt_adapter_start_device_discovery();
				RETM_IF(result != BT_ERROR_NONE, "bt_adapter_set_device_discovery_state_changed_cb failed > Error = %d", result);
			}
		}
		break;
		case BT_DISCOVERED_DEVICE_CLICKED:
		{
			DBG("BT_DISCOVERED_DEVICE_CLICKED");
			bt_adapter_device_discovery_info_s *device_info;
			device_info = (bt_adapter_device_discovery_info_s*)feed_data;


			this->selected_device_info = device_info;

			if(this->selected_device_info->is_bonded == false)
			{

				util_state_object_feed(this->state_object_bt_client_pair, BT_ADAPTER_ATTEMP_PAIR, (void*)device_info);
			}

			else if(this->selected_device_info->is_bonded == true)
			{

				util_state_object_feed(this->state_object_bt_client_pair, BT_ADAPTER_ATTEMP_UNPAIR, (void*)device_info);

			}
		}
			break;


		case BT_ACTIVATION_BY_APP_FAILED:
		{
			DBG("Failed to launch Bluetooth On/Off app!\n");
			ui_utils_label_set_text(this->bluetooth_label, "Please Turn on Manually", "left");
			elm_object_disabled_set(this->bluetooth_btn, EINA_TRUE);
		}
			break;

		case BT_PAIR_REQUEST_ATTEMPTED:
		{
			int result;
			DBG("BT_DISCOVERED_DEVICE_CLICKED");
			bt_adapter_device_discovery_info_s *device_info;
			device_info = (bt_adapter_device_discovery_info_s*)feed_data;
		{
			result = bt_adapter_stop_device_discovery();
			DBG("The value of Stop Discovery is %s", get_bluetooth_error(result));
			this->discovery_state = BT_ADAPTER_DEVICE_DISCOVERY_FINISHED;
			update_view_controls(this);

			result = bt_device_create_bond(device_info->remote_address);


			if(result != BT_ERROR_NONE)
			{
				this->selected_device_info = NULL;
				this->selected_device_item = NULL;
				DBG("[bt_device_create_bond] Failed. result(%d).", result);
				return;
			}
			else
			{
				if(this->view->tbt_info->apptype == TBT_APP_BT_CLIENT_PAIRED && elm_check_state_get(this->bluetooth_check)){

					result = bt_device_cancel_bonding();
					DBG("bt_device_cancel_bonding : %d",result);
					RETM_IF(result != BT_ERROR_NONE, "bt_device_cancel_bonding fail > Error = %d", result);
					util_state_object_feed(this->state_object_bt_client_pair, BT_BOND_CANCEL, NULL);
				}
				elm_object_disabled_set(this->action_btn, EINA_TRUE);
				evas_object_data_set(this->bluetooth_list, "bluetooth_view", NULL);

				DBG("bt_device_start_service_search");
				result = bt_device_start_service_search(this->selected_device_info->remote_address);
				RETM_IF(result != BT_ERROR_NONE, "bt_device_start_service_search fail > Error = %d", result);

				elm_object_item_text_set(this->selected_device_item, format_string("%s(Pairing)", this->selected_device_info->remote_name));
				#ifdef TIZEN_3_0
					ui_utils_label_set_text(this->bluetooth_label, "Pairing..", "left");
				#endif
				#ifdef TIZEN_2_4
					ui_utils_label_set_text(this->bluetooth_label, "Pairing..", "left");
				#endif
				DBG("[bt_device_create_bond] succeeded. device_bond_created_cb callback will be called.");
			}
		}
			break;
		}
		case BT_UNPAIR_REQUEST_ATTEMPTED:
		{
			bt_adapter_device_discovery_info_s *device_info;
			device_info = (bt_adapter_device_discovery_info_s*)feed_data;

			bt_device_info_s* device_info_bonded;
			result = bt_adapter_get_bonded_device_info(device_info->remote_address, &device_info_bonded);
			RETM_IF(result != BT_ERROR_NONE, "bt_adapter_get_bonded_device_info fail > Error = %d", result);
			result = bt_device_set_alias(device_info_bonded->remote_address,device_info_bonded->remote_name);
			RETM_IF(result != BT_ERROR_NONE, "bt_device_set_alias fail > Error = %d", result);
			result = bt_adapter_free_device_info (device_info_bonded);
			RETM_IF(result != BT_ERROR_NONE, "bt_adapter_free_device_info fail > Error = %d", result);

			if(this->view->tbt_info->apptype == TBT_APP_BT_PROFILES)
			{
				set_connected_profiles(this);
			}

			else
			{
				int result;
				result = bt_device_destroy_bond(device_info->remote_address);
				if(result != BT_ERROR_NONE)
				{
					this->selected_device_info = NULL;
					DBG("[bt_device_destroy_bond] Failed. result(%d).", result);
					return;
				}
				else
				{
					elm_object_disabled_set(this->action_btn, EINA_TRUE);
					evas_object_data_set(this->bluetooth_list, "bluetooth_view", NULL);
					elm_object_item_text_set(this->selected_device_item, format_string("%s(Unpairing)", this->selected_device_info->remote_name));
					#ifdef TIZEN_3_0
						ui_utils_label_set_text(this->bluetooth_label, "Unpairing..", "left");
					#endif
					#ifdef TIZEN_2_4
						ui_utils_label_set_text(this->bluetooth_label, "Unpairing..", "left");
					#endif
					DBG("[bt_device_destroy_bond] succeeded. device_bond_destroyed_cb callback will be called.");
				}

			}
			break;
		}
		default:
			break;
	}
}


/**
 * @function		do_bt_common_feed
 * @since_tizen		2.3
 * @description		Do Bt Common Feed
 * @parameter		bluetooth_view*: Bluetooth View Pointer, void*: Void Pointer
 * @return		static void
 */
static void do_bt_common_feed(bluetooth_view *this, int feed_value, void* feed_data)
{
	RETM_IF(NULL == this, "view is NULL");
	

	if(this->state_object_bt_client_pair)
	{
		DBG("Feed value is %d", feed_value);
		util_state_object_feed(this->state_object_bt_client_pair, feed_value, feed_data);
	}
	if(this->state_object_bt_server_pair)
	{
		util_state_object_feed(this->state_object_bt_server_pair, feed_value, feed_data);
	}
}


/**
 * @function		bt_get_service_name
 * @since_tizen		2.3
 * @description		Bt Get Service Name
 * @parameter		bt_service_class_t: Bt Service Class T
 * @return		static GList*
 */
static GList* bt_get_service_name(bt_service_class_t service_enum)
{
	char* service_name = NULL;
	GList* service_list = NULL;

	DBG("service enum %d", service_enum);



	if (service_enum & BT_SC_RES_SERVICE_MASK) {
		service_name = "RES Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_SPP_SERVICE_MASK) {
		service_name = "SPP Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_DUN_SERVICE_MASK) {
		service_name = "DUN Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_FAX_SERVICE_MASK) {
		service_name = "FAX Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_LAP_SERVICE_MASK) {
		service_name = "LAP Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_HSP_SERVICE_MASK) {
		service_name = "HSP Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_HFP_SERVICE_MASK) {
		service_name = "HFP Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_OPP_SERVICE_MASK) {
		service_name = "OPP Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_FTP_SERVICE_MASK) {
		service_name = "FTP Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_CTP_SERVICE_MASK) {
		service_name = "CTP Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_ICP_SERVICE_MASK) {
		service_name = "ICP Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_SYNC_SERVICE_MASK) {
		service_name = "DUN Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_BPP_SERVICE_MASK) {
		service_name = "BPP Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_BIP_SERVICE_MASK) {
		service_name = "BIP Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_PANU_SERVICE_MASK) {
		service_name = "PANU Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_NAP_SERVICE_MASK) {
		service_name = "DUN Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_GN_SERVICE_MASK) {
		service_name = "GN Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_SAP_SERVICE_MASK) {
		service_name = "SAP Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_A2DP_SERVICE_MASK) {
		service_name = "A2DP Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_AVRCP_SERVICE_MASK) {
		service_name = "AVRCP Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_PBAP_SERVICE_MASK) {
		service_name = "PBAP Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_HID_SERVICE_MASK) {
		service_name = "HID Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	if (service_enum & BT_SC_A2DP_SOURCE_SERVICE_MASK) {
		service_name = "A2DP Source Service Class";
		DBG("service name %s", service_name);
		service_list = g_list_append(service_list, (gpointer)service_name);
	}
	return service_list;
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
