/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __TIZEN_NETWORK_BLUETOOTH_TYPE_INTERNAL_H__
#define __TIZEN_NETWORK_BLUETOOTH_TYPE_INTERNAL_H__

 #ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @file bluetooth_type_internal.h
 */

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Enumerations of the Bluetooth adapter le state.
 * @since_tizen 2.3
 */
typedef enum
{
	BT_ADAPTER_LE_DISABLED = 0x00, /**< Bluetooth le is disabled */
	BT_ADAPTER_LE_ENABLED, /**< Bluetooth le is enabled */
} bt_adapter_le_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Called when the Bluetooth adapter le state changes.
 * @since_tizen 2.3
 *
 * @param[in]   result  The result of the adapter state changing
 * @param[in]   adapter_le_state  The adapter le state to be changed
 * @param[in]   user_data  The user data passed from the callback registration function
 * @pre Either bt_adapter_le_enable() or bt_adapter_le_disable() will invoke this callback if you register this callback using bt_adapter_le_set_state_changed_cb().
 * @see bt_adapter_le_enable()
 * @see bt_adapter_le_disable()
 * @see bt_adapter_le_set_state_changed_cb()
 * @see bt_adapter_le_unset_state_changed_cb()
 */
typedef void (*bt_adapter_le_state_changed_cb)(int result, bt_adapter_le_state_e adapter_le_state, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Enumerations for the call state
 * @since_tizen 2.3
 */
typedef enum {
    BT_AG_CALL_EVENT_IDLE = 0x00,  /**< Idle */
    BT_AG_CALL_EVENT_ANSWERED,  /**< Answered */
    BT_AG_CALL_EVENT_HELD,  /**< Held */
    BT_AG_CALL_EVENT_RETRIEVED,  /**< Retrieved */
    BT_AG_CALL_EVENT_DIALING,  /**< Dialing */
    BT_AG_CALL_EVENT_ALERTING,  /**< Alerting */
    BT_AG_CALL_EVENT_INCOMING,  /**< Incoming */
} bt_ag_call_event_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Enumerations for the call state
 * @since_tizen 2.3
 */
typedef enum {
    BT_AG_CALL_STATE_IDLE = 0x00,  /**< Idle state */
    BT_AG_CALL_STATE_ACTIVE,  /**< Active state */
    BT_AG_CALL_STATE_HELD,  /**< Held state */
    BT_AG_CALL_STATE_DIALING,  /**< Dialing state */
    BT_AG_CALL_STATE_ALERTING,  /**< Alerting state */
    BT_AG_CALL_STATE_INCOMING,  /**< Incoming state */
    BT_AG_CALL_STATE_WAITING,  /**< Waiting for connected indication event after answering an incoming call*/
} bt_ag_call_state_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when the connectable state changes.
 * @since_tizen 2.3
 *
 * @param[in] result The result of the connectable state changing
 * @param[in] connectable The connectable to be changed
 * @param[in] user_data The user data passed from the callback registration function
 *
 * @pre This function will be invoked when the connectable state of local Bluetooth adapter changes
 * if you register this callback using bt_adapter_set_connectable_changed_cb().
 *
 * @see bt_adapter_set_connectable()
 * @see bt_adapter_set_connectable_changed_cb()
 * @see bt_adapter_unset_connectable_changed_cb()
 */
typedef void (*bt_adapter_connectable_changed_cb)
	(int result, bool connectable, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief  Called when the push is requested.
 * @since_tizen 2.3
 *
 * @details You must call bt_opp_server_accept() if you want to accept.
 * Otherwise, you must call bt_opp_server_reject().
 * @param[in] file  The path of file to be pushed
 * @param[in] size The file size (bytes)
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_opp_server_initialize()
 */
typedef void (*bt_opp_server_push_requested_cb)(const char *file, int size, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Enumerations of the Bluetooth adapter le scan type.
 * @since_tizen 2.3
 */
typedef enum
{
	BT_ADAPTER_LE_PASSIVE_SCAN = 0x00,
	BT_ADAPTER_LE_ACTIVE_SCAN
} bt_adapter_le_scan_type_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Enumerations of the Bluetooth le scan mode.
 * @since_tizen 2.3
 */
typedef enum
{
	BT_ADAPTER_LE_SCAN_MODE_BALANCED,
	BT_ADAPTER_LE_SCAN_MODE_LOW_LATENCY,
	BT_ADAPTER_LE_SCAN_MODE_LOW_ENERGY
} bt_adapter_le_scan_mode_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Called when the manufacturer dat changes.
 * @since_tizen 2.3
 *
 * @param[in]   data		The manufacurer data of the Bluetooth device to be changed
 * @param[in]   len			The length of @a data
 * @param[in]   user_data	The user data passed from the callback registration function
 * @pre This function will be invoked when the manufacturer data of Bluetooth adapter changes
 * if callback is registered using bt_adapter_set_manufacturer_data_changed_cb().
 * @see bt_adapter_set_manufacturer_data()
 * @see bt_adapter_set_manufacturer_data_changed_cb()
 * @see bt_adapter_unset_manufacturer_data_changed_cb()
 */
typedef void (*bt_adapter_manufacturer_data_changed_cb) (char *data,
		int len, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Enumerations of the attribute's permission
 * @since_tizen 2.4
 */
typedef enum {
	BT_GATT_PERMISSION_READ = 0x01,
	BT_GATT_PERMISSION_WRITE = 0x02,
	BT_GATT_PERMISSION_ENCRYPTION = 0x04,
	BT_GATT_PERMISSION_AUTHENTICATION = 0x08,
	BT_GATT_PERMISSION_AUTHORIZATION = 0x10,
	BT_GATT_PERMISSION_NONE = 0x20,
} bt_gatt_permission_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief The handle to control Bluetooth LE scan filter
 * @since_tizen 2.4
 */
typedef void* bt_scan_filter_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief The handle of a GATT server
 * @since_tizen 2.4
 */
typedef void* bt_gatt_server_h;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Called when a value of a characteristic or descriptor's GATT handle has been changed
 * @since_tizen 2.4
 *
 * @remarks After this function is returned, a changed vlaue is automatically
 * applied to @a gatt_handle. Before that, @a gatt_handle has an old value.
 *
 * @param[in] remote_address The address of the remote device which requests a change
 * @param[in] server The GATT server handle
 * @param[in] gatt_handle The characteristic or descriptor's GATT handle which has an old value
 * @param[in] offset The requested offset from where the @a gatt_handle value will be updated
 * @param[in] value The new value
 * @param[in] len The length of @a value
 * @param[in] user_data The user data passed from the registration function
 *
 * @see bt_gatt_server_set_value_changed_cb()
 */
typedef void (*bt_gatt_server_value_changed_cb) (char *remote_address,
				bt_gatt_server_h server, bt_gatt_h gatt_handle,
				int offset, char *value, int len,
				void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Called when the remote device requests to read a value on a GATT server
 * @since_tizen 2.4
 *
 * @param[in] remote_address The address of the requesting remote device
 * @param[in] server The GATT server handle
 * @param[in] gatt_handle The characteristic or descriptor's GATT handle to be read
 * @param[in] request_id The identification of this request. It will be used to send a reponse.
 * @param[in] offset The requested offset from where the GATT handle's value is read
 * @param[in] user_data The user data passed from the registration function
 *
 * @see bt_gatt_server_set_read_value_requested_cb()
 * @see bt_gatt_server_send_response()
 */
typedef void (*bt_gatt_server_read_value_requested_cb) (char *remote_address,
		int request_id, bt_gatt_server_h server, bt_gatt_h gatt_handle,
		int offset, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Called when the remote device enables or disables the Notification/Indication for particular characteristics.
 * @since_tizen 2.4
 *
 * @param[in] server The GATT server handle
 * @param[in] gatt_handle The characteristic's GATT handle to be read
 * @param[in] user_data The user data passed from the registration function
 *
 * @see bt_gatt_server_set_read_value_requested_cb()
 */
typedef void (*bt_gatt_server_notification_state_change_cb) (bool notify,
			bt_gatt_server_h server, bt_gatt_h gatt_handle, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Called when the sending notification / indication is done
 * @since_tizen 2.4
 *
 * @remarks In case of an indication, once a confirmation is received from the remote device this callback will be called. \n
 * This callback will be called several times if there are two or more remote devices which enable a Client Characteristic Configuration Descriptor(CCCD). \n
 * For the last remote device, @a completed will be set as true.
 *
 * @param[in] result The result of a sending operation
 * @param[in] remote_address The address of the remote device
 * @param[in] server The GATT server handle
 * @param[in] characteristic The characteristic's GATT handle
 * @param[in] completed If this callback is for the last remote device which enables a CCCD, it will be true. Or it will be false.
 * @param[in] user_data The user data passed from the requesting function
 *
 * @see bt_gatt_server_notify()
 */
typedef void (*bt_gatt_server_notification_sent_cb) (int result,
		char *remote_address, bt_gatt_server_h server,
		bt_gatt_h characteristic, bool completed, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Enumerations for the player control command
 * @since_tizen 2.3
 */
 typedef enum {
	BT_AVRCP_CONTROL_PLAY = 0x01,   /**< Play */
	BT_AVRCP_CONTROL_PAUSE,   /**< Pause */
	BT_AVRCP_CONTROL_STOP,   /**< Stop */
	BT_AVRCP_CONTROL_NEXT,   /**< Next Track*/
	BT_AVRCP_CONTROL_PREVIOUS,   /**< Previous track */
	BT_AVRCP_CONTROL_FAST_FORWARD,   /**< Fast Forward */
	BT_AVRCP_CONTROL_REWIND   /**< Rewind */
} bt_avrcp_player_command_e;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief Structure of Track metadata information.
 * @since_tizen 2.3
 *
 * @see #bt_class_s
 */
typedef struct {
	const char *title;
	const char *artist;
	const char *album;
	const char *genre;
	unsigned int total_tracks;
	unsigned int number;
	unsigned int duration;
} bt_avrcp_metadata_attributes_info_s;

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the connection state is changed.
 * @since_tizen 2.3
 * @param[in] connected  The state to be changed. @a true means connected state, Otherwise, @a false.
 * @param[in] remote_address  The remote address
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_control_initialize()
 * @see bt_avrcp_control_deinitialize()
 */
typedef void (*bt_avrcp_control_connection_state_changed_cb) (bool connected, const char *remote_address, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the Song position mode is changed by the remote target device.
 * @since_tizen 2.3
 *
 * @param[in] position The song position
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_song_position_changed_cb()
 * @see bt_avrcp_unset_song_position_changed_cb()
 */
typedef void (*bt_avrcp_song_position_changed_cb) (unsigned int position, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the Song metadata information is changed by the remote target device.
 * @since_tizen 2.3
 *
 * @param[in] position The song metadata information
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_track_info_changed_cb()
 * @see bt_avrcp_unset_track_info_changed_cb()
 */
typedef void (*bt_avrcp_track_info_changed_cb) (bt_avrcp_metadata_attributes_info_s *track, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Called when the Song Play status mode is changed by the remote target device.
 * @since_tizen 2.3
 *
 * @param[in] play_state The song play status
 * @param[in] user_data The user data passed from the callback registration function
 * @see bt_avrcp_set_play_status_changed_cb()
 * @see bt_avrcp_unset_play_status_changed_cb()
 */
typedef void (*bt_avrcp_play_status_changed_cb) (bt_avrcp_player_state_e play_state, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Device LE connection update structure.
 * @since_tizen 2.3
 */
typedef struct
{
       unsigned int interval_min;   /**< Minimum value for the connection event interval (msec) */
       unsigned int interval_max;   /**< Maximum value for the connection event interval (msec) */
       unsigned int latency;   /**< Slave latency (msec) */
       unsigned int time_out;   /**< Supervision timeout (msec) */
} bt_le_conn_update_s;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __TIZEN_NETWORK_BLUETOOTH_TYPE_INTERNAL_H__
