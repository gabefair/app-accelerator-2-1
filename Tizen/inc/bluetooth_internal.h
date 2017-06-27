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


#ifndef __TIZEN_NETWORK_BLUETOOTH_INTERNAL_H__
#define __TIZEN_NETWORK_BLUETOOTH_INTERNAL_H__

#include <glib.h>
#include "bluetooth_type.h"
#include "bluetooth_type_internal.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/**
 * @file        bluetooth_internal.h
 */

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Enables the local Bluetooth adapter, asynchronously.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @details This function enables Bluetooth protocol stack and hardware.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_ALREADY_DONE  Already enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation now in progress
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre Bluetooth service must be initialized with bt_initialize().
 * @pre The state of local Bluetooth must be #BT_ADAPTER_DISABLED
 * @post This function invokes bt_adapter_state_changed_cb().
 *
 * @see bt_initialize()
 * @see bt_adapter_get_state()
 * @see bt_adapter_set_state_changed_cb()
 * @see bt_adapter_unset_state_changed_cb()
 * @see bt_adapter_state_changed_cb()
 *
 */
int bt_adapter_enable(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Disables the local Bluetooth adapter, asynchronously.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @details This function disables Bluetooth protocol stack and hardware.
 *
 * @remarks You should disable Bluetooth adapter, which is helpful for saving power.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation now in progress
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED
 * @post This function invokes bt_adapter_state_changed_cb().
 *
 * @see bt_adapter_get_state()
 * @see bt_adapter_state_changed_cb()
 * @see bt_adapter_set_state_changed_cb()
 * @see bt_adapter_unset_state_changed_cb ()
 *
 */
int bt_adapter_disable(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Recover the local Bluetooth adapter, asynchronously.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @details This function does recovery logic, disables Bluetooth protocol stack and hardware, then enables after a few seconds.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation now in progress
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED
 * @post This function invokes bt_adapter_state_changed_cb().
 *
 * @see bt_adapter_get_state()
 * @see bt_adapter_state_changed_cb()
 * @see bt_adapter_set_state_changed_cb()
 * @see bt_adapter_unset_state_changed_cb ()
 *
 */
int bt_adapter_recover(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Reset the local Bluetooth adapter, synchronously.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @details This function resets Bluetooth protocol and values.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre Bluetooth service must be initialized with bt_initialize().
 * @post bt_adapter_state_changed_cb() will be invoked if The state of local Bluetooth was #BT_ADAPTER_ENABLED.
 *
 * @see bt_initialize()
 * @see bt_adapter_get_state()
 * @see bt_adapter_set_state_changed_cb()
 * @see bt_adapter_unset_state_changed_cb()
 * @see bt_adapter_state_changed_cb()
 *
 */
int bt_adapter_reset(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Gets the version of local Bluetooth adapter.
 * @since_tizen 2.3
 * @remarks The @a local_version must be released with free() by you.
 *
 * @param[out] local_version The version of local Bluetooth adapter
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 */
int bt_adapter_get_version(char **local_version);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Gets the information regarding local Bluetooth adapter.
 * @since_tizen 2.3
 * @remarks The @a all parameters must be released with free() by you.
 *
 * @param[out] chipset Chipset name of local Bluetooth adapter
 * @param[out] firmware Firmware info. of local Bluetooth adapter
 * @param[out] stack_version Bluetooth stack version
 * @param[out] profiles The profile list of local Bluetooth adapter
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 */
int bt_adapter_get_local_info(char **chipset, char **firmware, char **stack_version, char **profiles);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Sets the visibility mode.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks #BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE will change to #BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE
 * after the given @a duration goes.
 *
 * @param[in] discoverable_mode The Bluetooth visibility mode to set
 * @param[in] duration The duration until the visibility mode is changed to #BT_ADAPTER_VISIBILITY_MODE_NON_DISCOVERABLE (in seconds).
 * @a duration is used only for #BT_ADAPTER_VISIBILITY_MODE_LIMITED_DISCOVERABLE mode.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @post bt_adapter_visibility_mode_changed_cb() will be invoked if this function returns #BT_ERROR_NONE.
 *
 * @see bt_adapter_get_visibility()
 * @see bt_adapter_visibility_mode_changed_cb()
 * @see bt_adapter_set_visibility_mode_changed_cb()
 * @see bt_adapter_unset_visibility_mode_changed_cb()
 */
int bt_adapter_set_visibility(bt_adapter_visibility_mode_e discoverable_mode, int duration);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Registers a callback function to be invoked when the connectable state changes.
 * @since_tizen 2.3
 *
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 *
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_adapter_connectable_changed_cb() will be invoked.
 *
 * @see bt_initialize()
 * @see bt_adapter_connectable_changed_cb()
 * @see bt_adapter_unset_connectable_changed_cb()
 */
int bt_adapter_set_connectable_changed_cb(bt_adapter_connectable_changed_cb callback, void *user_data);

/**
 * @internal
 * @ingroup  CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Unregisters the callback function.
 * @since_tizen 2.3
 *
 * @return  0 on success, otherwise a negative error value.
 * @retval  #BT_ERROR_NONE  Successful
 * @retval  #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval  #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre  The Bluetooth service must be initialized with bt_initialize().
 *
 * @see  bt_initialize()
 * @see  bt_adapter_set_connectable_changed_cb()
 */
int bt_adapter_unset_connectable_changed_cb(void);

/**
 * @internal
 * @ingroup  CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Gets the connectable state of local Bluetooth adapter.
 * @since_tizen 2.3
 *
 * @remarks When connectable state is false, no device can connect to this device and visibility mode cannot be changed.
 *
 * @param[out] connectable The connectable state of local Bluetooth adapter
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 *
 * @see bt_adapter_set_connectable()
 */
int bt_adapter_get_connectable(bool *connectable);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Sets the connectable state of local Bluetooth adapter.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks When connectable state is false, no device can connect to this device and visibility mode cannot be changed.
 *
 * @param[in] connectable The connectable state to set
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @post bt_adapter_connectable_changed_cb() will be invoked if this function returns #BT_ERROR_NONE.
 *
 * @see bt_adapter_get_connectable()
 * @see bt_adapter_connectable_changed_cb()
 * @see bt_adapter_set_connectable_changed_cb()
 * @see bt_adapter_unset_connectable_changed_cb()
 */
int bt_adapter_set_connectable(bool connectable);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief Sets the manufacturer data of local Bluetooth adapter.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in]   data	The manufacturer specific data of the Bluetooth device.
 * @param[in]   len	The length of @a data.Maximaum length is 240 bytes.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @post bt_adapter_manufacturer_data_changed_cb() will be invoked
 * if this function returns #BT_ERROR_NONE.
 *
 * @see bt_adapter_manufacturer_data_changed_cb
 * @see bt_adapter_set_manufacturer_data_changed_cb()
 * @see bt_adapter_unset_manufacturer_data_changed_cb()
 */
int bt_adapter_set_manufacturer_data(char *data, int len);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief  Registers a callback function to be invoked
 * when the manufacturer data of Bluetooth adapter changes.
 * @since_tizen 2.3
 *
 * @param[in]   callback	The callback function to invoke
 * @param[in]   user_data	The user data to be passed to the callback function
 *
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post  bt_adapter_manufacturer_data_changed_cb() will be invoked.
 *
 * @see bt_initialize()
 * @see bt_adapter_unset_manufacturer_data_changed_cb()
 */
int bt_adapter_set_manufacturer_data_changed_cb(
		bt_adapter_manufacturer_data_changed_cb callback,
		void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_MODULE
 * @brief	Unregisters the callback function.
 * @since_tizen 2.3
 *
 * @return	0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 * @see bt_initialize()
 * @see bt_adapter_set_manufacturer_data_changed_cb()
 */
int bt_adapter_unset_manufacturer_data_changed_cb(void);


/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Enables the local Bluetooth le adapter, asynchronously.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @details This function enables Bluetooth protocol stack and hardware.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_ALREADY_DONE  Already enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation now in progress
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre Bluetooth service must be initialized with bt_initialize().
 * @post This function invokes bt_adapter_le_state_changed_cb().
 *
 * @see bt_initialize()
 * @see bt_adapter_le_get_state()
 * @see bt_adapter_le_set_state_changed_cb()
 * @see bt_adapter_le_unset_state_changed_cb()
 * @see bt_adapter_le_state_changed_cb()
 *
 */
int bt_adapter_le_enable(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Disables the local Bluetooth le adapter, asynchronously.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @details This function disables Bluetooth le protocol stack and hardware.
 *
 * @remarks
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation now in progress
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_LE_ENABLED
 * @post This function invokes bt_adapter_le_state_changed_cb().
 *
 * @see bt_adapter_le_get_state()
 * @see bt_adapter_le_state_changed_cb()
 * @see bt_adapter_le_set_state_changed_cb()
 * @see bt_adapter_le_unset_state_changed_cb ()
 *
 */
int bt_adapter_le_disable(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Gets the current state of local Bluetooth adapter.
 * @since_tizen 2.3
 *
 * @param[out] adapter_le_state The current adapter le state
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre Bluetooth service must be initialized with bt_initialize().
 *
 * @see bt_initialize()
 */
int bt_adapter_le_get_state(bt_adapter_le_state_e *adapter_le_state);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Registers a callback function to be invoked when the Bluetooth adapter le state changes.
 * @since_tizen 2.3
 *
 * @param[in] callback	The callback function to invoke
 * @param[in] user_data	The user data to be passed to the callback function
 *
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_adapter_le_state_changed_cb() will be invoked.
 *
 * @see bt_initialize()
 * @see bt_adapter_le_state_changed_cb()
 * @see bt_adapter_le_unset_state_changed_cb()
 */
int bt_adapter_le_set_state_changed_cb(bt_adapter_le_state_changed_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief	Unregisters the callback function.
 * @since_tizen 2.3
 *
 * @return	0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 * @see bt_initialize()
 * @see bt_adapter_le_set_state_changed_cb()
 */
int bt_adapter_le_unset_state_changed_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief add address to whitelist for accepting scanning request.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks If the adress is in the whitelist then other LE devices are able to
 * search this device. Before calling this API, make sure that the adapter is
 * enabled. There is no callback event for this API.

 * @param[in] address The other device's address
 * @param[in] address_type The other device's address type
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Adapter is not enabled
 * @retval #BT_ERROR_RESOURCE_BUSY  Device or resource busy
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 *
 * @see bt_adapter_le_start_advertising_new()
 * @see bt_adapter_le_stop_advertising()
 */
int bt_adapter_le_add_white_list(const char *address, bt_device_address_type_e address_type);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief remove address from the whitelist for not accepting scanning request.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks If the adress is in the whitelist then other LE devices are able to
 * search this device. Before calling this API, make sure that the adapter is
 * enabled. There is no callback event for this API.
 *
 * @param[in] address The other device's address
 * @param[in] address_type The other device's address type
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Adapter is not enabled
 * @retval #BT_ERROR_RESOURCE_BUSY  Device or resource busy
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 *
 * @see bt_adapter_le_start_advertising_new()
 * @see bt_adapter_le_stop_advertising()
 */
int bt_adapter_le_remove_white_list(const char *address, bt_device_address_type_e address_type);

/**
 * @internal
 * @deprecated Deprecated since 2.3.1
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief clear address from the whitelist for not accepting scanning request.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks If the adress is in the whitelist then other LE devices are able to
 * search this device. Before calling this API, make sure that the adapter is
 * enabled. There is no callback event for this API.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Adapter is not enabled
 * @retval #BT_ERROR_RESOURCE_BUSY  Device or resource busy
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 *
 * @see bt_adapter_le_start_advertising_new()
 * @see bt_adapter_le_stop_advertising()
 */
int bt_adapter_le_clear_white_list(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief  Sets the Privacy feature state of local Bluetooth adapter.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @param[in] enable_privacy The privacy feature to set/unset.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @pre The state of local Bluetooth must be #BT_ADAPTER_LE_ENABLED.
 *
 */
int bt_adapter_le_enable_privacy(bool enable_privacy);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Set Bluetooth LE scan mode
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in]  scan_mode  The scan mode
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_NOT_ENABLED Adapter is not enabled
 * @retval #BT_ERROR_INVALID_PARAM Parameter is invalid
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 */
int bt_adapter_le_set_scan_mode(bt_adapter_le_scan_mode_e scan_mode);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Creates scan filter to find only LE advertisement which has specific data.
 * @since_tizen 2.4
 *
 * @param[out] scan_filter The handle of scan filter
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 * @see bt_adapter_le_destroy_scan_filter()
 */
int bt_adapter_le_create_scan_filter(bt_scan_filter_h *scan_filter);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Destroys scan filter.
 * @since_tizen 2.4
 *
 * @param[in] scan_filter The handle of scan filter
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 * @see bt_adapter_le_create_scan_filter()
 */
int bt_adapter_le_destroy_scan_filter(bt_scan_filter_h scan_filter);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Adds the device address to filter advertisements
 * @since_tizen 2.4
 *
 * @param[in] scan_filter The handle of scan filter
 * @param[in] address The device address to filter advertisements
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 */
int bt_adapter_le_scan_filter_set_device_address(bt_scan_filter_h scan_filter, const char *address);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Adds the device name to filter advertisements
 * @since_tizen 2.4
 *
 * @param[in] scan_filter The handle of scan filter
 * @param[in] name The device name to filter advertisements
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 */
int bt_adapter_le_scan_filter_set_device_name(bt_scan_filter_h scan_filter, const char *name);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Adds the service UUID to filter advertisements
 * @since_tizen 2.4
 *
 * @param[in] scan_filter The handle of scan filter
 * @param[in] uuid The service UUID to filter advertisements
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 */
int bt_adapter_le_scan_filter_set_service_uuid(bt_scan_filter_h scan_filter, const char *uuid);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Adds the service uuid and the mask to filter advertisements by partial data
 * @since_tizen 2.4
 *
 * @remarks the length of mask msut be the same with the length of service uuid.
 *
 * @param[in] scan_filter The handle of scan filter
 * @param[in] uuid The service UUID to filter advertisements
 * @param[in] mask The mask to filter advertisements
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 */
int bt_adapter_le_scan_filter_set_service_uuid_with_mask(bt_scan_filter_h scan_filter,
			const char *uuid, const char *mask);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Adds the service solicitation UUID to filter advertisements
 * @since_tizen 2.4
 *
 * @param[in] scan_filter The handle of scan filter
 * @param[in] uuid The service solicitation UUID to filter advertisements
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 */
int bt_adapter_le_scan_filter_set_service_solicitation_uuid(bt_scan_filter_h scan_filter, const char *uuid);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Adds the service solicitation uuid and the mask to filter advertisements by partial data
 * @since_tizen 2.4
 *
 * @remarks the length of mask msut be the same with the length of service solicitation uuid.
 *
 * @param[in] scan_filter The handle of scan filter
 * @param[in] uuid The service solicitation UUID to filter advertisements
 * @param[in] mask The mask to filter advertisements
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 */
int bt_adapter_le_scan_filter_set_service_solicitation_uuid_with_mask(bt_scan_filter_h scan_filter,
			const char *uuid, const char *mask);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Adds the service data to filter advertisements
 * @since_tizen 2.4
 *
 * @param[in] scan_filter The handle of scan filter
 * @param[in] uuid The service UUID to filter advertisements
 * @param[in] data The service data to filter advertisements
 * @param[in] data_len The length of the service data
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 */
int bt_adapter_le_scan_filter_set_service_data(bt_scan_filter_h scan_filter,
			const char *uuid, const char *data, unsigned int data_len);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Adds the service data and the mask to filter advertisements by partial data
 * @since_tizen 2.4
 *
 * @remarks the length of mask msut be the same with the length of service data.
 *
 * @param[in] scan_filter The handle of scan filter
 * @param[in] uuid The service UUID to filter advertisements
 * @param[in] data The service data to filter advertisements
 * @param[in] data_len The length of the service data
 * @param[in] mask The mask to filter advertisements
 * @param[in] mask_len The length of the mask to be set.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 */
int bt_adapter_le_scan_filter_set_service_data_with_mask(bt_scan_filter_h scan_filter,
			const char *uuid, const char *data, unsigned int data_len, const char *mask, unsigned int mask_len);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Adds the manufacturer data to filter advertisements
 * @since_tizen 2.4
 *
 * @param[in] scan_filter The handle of scan filter
 * @param[in] manufacturer_id The manufacturer ID (0x0000 ~ 0xFFFF)
 * @param[in] data The manufacturer data (byte array)
 * @param[in] data_len The length of manufacturer data
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 */
int bt_adapter_le_scan_filter_set_manufacturer_data(bt_scan_filter_h scan_filter,
			int manufacturer_id, const char *data, unsigned int data_len);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Adds the manufacturer data and the mask to filter advertisements by partial data
 * @since_tizen 2.4
 *
 * @remarks the length of mask msut be the same with the length of service uuid.
 *
 * @param[in] scan_filter The handle of scan filter
 * @param[in] manufacturer_id The manufacturer ID (0x0000 ~ 0xFFFF)
 * @param[in] data The manufacturer data (byte array)
 * @param[in] data_len The length of manufacturer data
 * @param[in] mask The mask to filter advertisements
 * @param[in] mask_len The length of the mask to be set.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 */
int bt_adapter_le_scan_filter_set_manufacturer_data_with_mask(bt_scan_filter_h scan_filter,
			int manufacturer_id, const char *data, unsigned int data_len, const char *mask, unsigned int mask_len);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Registers the scan filter to use for scanning
 * @since_tizen 2.4
 *
 * @remarks Several scan filters can be registered. And the specific advertisements, satisfy the one of scan filters, will be found.
 *
 * @param[in] scan_filter The handle of scan filter
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Scan is in progress
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 */
int bt_adapter_le_register_scan_filter(bt_scan_filter_h scan_filter);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Unregisters the scan filter to be registered
 * @since_tizen 2.4
 *
 * @param[in] scan_filter The handle of scan filter
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Scan is in progress
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 */
int bt_adapter_le_unregister_scan_filter(bt_scan_filter_h scan_filter);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Unregisters all scan filters to be registered
 * @since_tizen 2.4
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Scan is in progress
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 */
int bt_adapter_le_unregister_all_scan_filters(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_ADAPTER_LE_MODULE
 * @brief Set advertising filter policy to use white list
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 *
 * @param[in] advertiser The handle of advertiser
 * @param[in] filter_policy The filter policy of advertising
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Operation is now in progress
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 *
 * @see bt_adapter_le_start_advertising_new()
 */
int bt_adapter_le_set_advertising_filter_policy(bt_advertiser_h advertiser, bt_adapter_le_advertising_filter_policy_e filter_policy);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Gets a connection state
 * @since_tizen 2.4
 *
 * @param[in] remote_address The address of the remote Bluetooth device
 * @param[in] link_type The link type to get a connection state
 * @param[out] connected The connection state
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED   Operation failed
 */
int bt_device_get_connection_state(const char *remote_address, bt_device_connection_link_type_e link_type, bool *connected);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Creates a bond with a remote Bluetooth device, asynchronously.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks A bond can be destroyed by bt_device_destroy_bond().\n
 * The bonding request can be cancelled by bt_device_cancel_bonding().
 *
 * @param[in] remote_address The address of the remote Bluetooth device with which the bond should be created
 * @param[in] conn_type The connection type(LE or BREDR) to create bond with remote device
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_RESOURCE_BUSY     Device or resource busy
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @pre The remote device must be discoverable with bt_adapter_start_device_discovery().
 * @post This function invokes bt_device_bond_created_cb().
 *
 * @see bt_adapter_start_device_discovery()
 * @see bt_device_create_bond()
 * @see bt_device_bond_created_cb()
 * @see bt_device_cancel_bonding()
 * @see bt_device_destroy_bond()
 * @see bt_device_set_bond_created_cb()
 * @see bt_device_unset_bond_created_cb()
 */
int bt_device_create_bond_by_type(const char *remote_address,
				  bt_device_connection_link_type_e conn_type);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief Cancels service search process.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_BONDED  Remote device not bonded
 * @retval #BT_ERROR_NOT_IN_PROGRESS  Operation not in progress
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The service search must be in progress by bt_device_start_service_search().
 *
 * @see bt_device_start_service_search()
 * @see bt_device_service_searched_cb()
 * @see bt_device_set_service_searched_cb()
 * @see bt_device_unset_service_searched_cb()
 */
int bt_device_cancel_service_search(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Registers a rfcomm server socket with a specific UUID. Activation by dbus is possible when the profile is connected.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 *
 * @remarks A socket can be destroyed by bt_socket_destroy_rfcomm_ex().
 * Application should call this API to receive a connection event when launched again by dbus.
 *
 * @param[in] uuid The UUID of service to provide
 * @param[in] bus_name bus_name of the application which is provided in service file.
 * @param[in] object_path dbus of the application
 * @return 0 on success, otherwise a negative error value.
 *
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_NOW_IN_PROGRESS  Already registered
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 *
 * @see bt_socket_listen_and_accept_rfcomm_ex()
 * @see bt_socket_destroy_rfcomm_ex()
 */
int bt_socket_create_rfcomm_ex(const char *uuid, const char *bus_name, const char *object_path);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Removes the rfcomm server socket which was created using bt_socket_create_rfcomm_ex().
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @remarks If callback function bt_socket_connection_state_changed_cb() is set and the remote Bluetooth device is connected,
 * then bt_socket_connection_state_changed_cb() will be called when this function is finished successfully.
 *
 * @param[in] uuid The UUID (which was created using bt_socket_create_rfcomm()) to destroy
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The socket must be created with bt_socket_create_rfcomm_ex().
 * @post If callback function bt_socket_connection_state_changed_cb() is set and the remote Bluetooth device is connected,
 * then bt_socket_connection_state_changed_cb() will be called.
 * @see bt_socket_create_rfcomm_ex()
 * @see bt_socket_connection_state_changed_cb()
 * @see bt_socket_set_connection_state_changed_cb()
 * @see bt_socket_unset_connection_state_changed_cb()
 */
int bt_socket_destroy_rfcomm_ex(const char *uuid);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Starts listening on passed rfcomm socket and accepts connection requests. Activation by dbus is possible when the profile is connected.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @details Pop-up is shown automatically when a RFCOMM connection is requested.
 * bt_socket_connection_state_changed_cb() will be called with
 * #BT_SOCKET_CONNECTED if you click "yes" and connection is finished successfully.
 * @param[in] uuid The UUID of service to provide
 * @param[in] max_pending_connections The maximum number of pending connections
 * @param[in] bus_name bus_name of the application which is provided in service file.
 * @param[in] object_path dbus of the application
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The socket must be created with bt_socket_create_rfcomm_ex().
 * @post If callback function bt_socket_connection_state_changed_cb() is set,
 * then bt_socket_connection_state_changed_cb() will be called when the remote Bluetooth device is connected.
 * @see bt_socket_create_rfcomm_ex()
 * @see bt_socket_connection_state_changed_cb()
 * @see bt_socket_set_connection_state_changed_cb()
 * @see bt_socket_unset_connection_state_changed_cb()
 */
int bt_socket_listen_and_accept_rfcomm_ex(const char *uuid, int max_pending_connections, const char* bus_name, const char *object_path);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief Starts listening on passed rfcomm socket.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @details bt_socket_connection_requested_cb() will be called when a RFCOMM connection is requested.
 *
 * @param[in] socket_fd  The file descriptor socket on which start to listen
 * @param[in] max_pending_connections  The number of pending connections
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The socket must be created with bt_socket_create_rfcomm().
 * @post This function invokes bt_socket_connection_state_changed_cb().
 *
 * @see bt_socket_create_rfcomm()
 * @see bt_socket_set_connection_requested_cb()
 * @see bt_socket_unset_connection_requested_cb()
 * @see bt_socket_connection_requested_cb()
 */
int bt_socket_listen(int socket_fd, int max_pending_connections);

/**
 * @internal
 * @ingroup  CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Accepts a connection request.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] requested_socket_fd  The file descriptor of socket on which a connection is requested
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The connection is requested by bt_socket_connection_requested_cb().
 * @see bt_socket_create_rfcomm()
 * @see bt_socket_connection_requested_cb()
 * @see bt_socket_listen()
 * @see bt_socket_reject()
*/
int bt_socket_accept(int requested_socket_fd);

/**
 * @internal
 * @ingroup  CAPI_NETWORK_BLUETOOTH_SOCKET_MODULE
 * @brief  Rejects a connection request.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] socket_fd  The file descriptor of socket on which a connection is requested
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The connection is requested by bt_socket_connection_requested_cb().
 * @see bt_socket_create_rfcomm()
 * @see bt_socket_connection_requested_cb()
 * @see bt_socket_listen()
 * @see bt_socket_accept()
 */
int bt_socket_reject(int socket_fd);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_OPP_SERVER_MODULE
 * @brief Initializes the Bluetooth OPP server requested by bt_opp_server_push_requested_cb().
 * @since_tizen 2.3
 * @details The popup appears when an OPP connection is requested from a remote device.
 * If you accept the request, then connection will be established and bt_opp_server_push_requested_cb() will be called.
 * At that time, you can call either bt_opp_server_accept() or bt_opp_server_reject().
 * @remarks This function must be called to start Bluetooth OPP server. You must free all resources of the Bluetooth service
 * by calling bt_opp_server_deinitialize() if Bluetooth OPP service is no longer needed.
 * @param[in] destination  The destination path
 * @param[in] push_requested_cb  The callback called when a push is requested
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_RESOURCE_BUSY  Device or resource busy
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED   Not supported
 *
 * @see  bt_opp_server_push_requested_cb()
 * @see  bt_opp_server_deinitialize()
 * @see  bt_opp_server_accept()
 * @see  bt_opp_server_reject()
 */
int bt_opp_server_initialize(const char *destination, bt_opp_server_push_requested_cb push_requested_cb, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Notifies the call event to the remote bluetooth device.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @remarks Before notifying #BT_AG_CALL_EVENT_ANSWERED or #BT_AG_CALL_EVENT_DIALING, you should open SCO(Synchronous Connection Oriented link)
 * if Bluetooth Hands-Free need SCO connection.
 * @param[in] event  The call event
 * @param[in] call_id  The call ID
 * @param[in] phone_number  The phone number. You must set this value in case of #BT_AG_CALL_EVENT_DIALING and #BT_AG_CALL_EVENT_INCOMING.
 * In other cases, this value can be NULL.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @see bt_audio_connect()
 */
int bt_ag_notify_call_event(bt_ag_call_event_e event, unsigned int call_id, const char *phone_number);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Notifies the call list to the remote bluetooth device.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] list  The call list
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @see bt_audio_connect()
 */
int bt_ag_notify_call_list(bt_call_list_h list);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Notifies the state of voice recognition.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] state  The state of voice recognition: (@c true = enabled, @c  false = disabled)
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio device must be connected with bt_audio_connect().
 * @see bt_audio_connect()
 */
int bt_ag_notify_voice_recognition_state(bool state);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Registers a callback function that will be invoked when a call handling event happened from Hands-Free.
 * @since_tizen 2.3
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_call_handling_event_cb()
 * @see bt_ag_unset_call_handling_event_cb()
 */
int bt_ag_set_call_handling_event_cb(bt_ag_call_handling_event_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Unregisters a callback function that will be invoked when a call handling event happened from Hands-Free.
 * @since_tizen 2.3
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_call_handling_event_cb()
 * @see bt_ag_set_call_handling_event_cb()
 */
int bt_ag_unset_call_handling_event_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Registers a callback function that will be invoked when a multi call handling event happened from Hands-Free.
 * @since_tizen 2.3
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_multi_call_handling_event_cb()
 * @see bt_ag_unset_multi_call_handling_event_cb()
 */
int bt_ag_set_multi_call_handling_event_cb(bt_ag_multi_call_handling_event_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Unregisters a callback function that will be invoked when a multi call handling event happened from Hands-Free.
 * @since_tizen 2.3
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_multi_call_handling_event_cb()
 * @see bt_ag_set_multi_call_handling_event_cb()
 */
int bt_ag_unset_multi_call_handling_event_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Registers a callback function that will be invoked when a DTMF(Dual Tone Multi Frequency) is transmitted from Hands-Free.
 * @since_tizen 2.3
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_dtmf_transmitted_cb()
 * @see bt_ag_unset_dtmf_transmitted_cb()
 */
int bt_ag_set_dtmf_transmitted_cb(bt_ag_dtmf_transmitted_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Unregisters a callback function that will be invoked when a DTMF(Dual Tone Multi Frequency) is transmitted from Hands-Free.
 * @since_tizen 2.3
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_dtmf_transmitted_cb()
 * @see bt_ag_set_dtmf_transmitted_cb()
 */
int bt_ag_unset_dtmf_transmitted_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Notifies the speaker gain to the remote device.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @details This function sends a signal to the remote device. This signal has the gain value.
 * @a gain is represented on a scale from 0 to 15. This value is absolute value relating to a particular volume level.
 * When the speaker gain of remote device is changed to the requested gain, bt_audio_speaker_gain_changed_cb() will be called.
 * @param[in] gain The gain of speaker (0 ~ 15)
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device is connected by bt_audio_connect() with #BT_AUDIO_PROFILE_TYPE_HSP_HFP service.
 * @see bt_ag_get_speaker_gain()
 * @see bt_ag_set_speaker_gain_changed_cb()
 * @see bt_ag_unset_speaker_gain_changed_cb()
 */
int bt_ag_notify_speaker_gain(int gain);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Gets the current speaker gain of the remote device.
 * @since_tizen 2.3
 * @details This function gets the value of speaker gain of the remote device.
 * @a gain is represented on a scale from 0 to 15. This value is absolute value relating to a particular volume level.
 * @param[out] gain The gain of speaker (0 ~ 15)
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device is connected by bt_audio_connect() with #BT_AUDIO_PROFILE_TYPE_HSP_HFP service.
 * @see bt_ag_notify_speaker_gain()
 * @see bt_ag_set_speaker_gain_changed_cb()
 * @see bt_ag_unset_speaker_gain_changed_cb()
 */
int bt_ag_get_speaker_gain(int *gain);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Checks whether the remoted device enables NREC(Noise Reduction and Echo Canceling) or not.
 * @since_tizen 2.3
 * @param[out] enabled The NREC status: (@c true = enabled, @c  false = not enabled)
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device is connected by bt_audio_connect() with #BT_AUDIO_PROFILE_TYPE_HSP_HFP service.
 * @see bt_audio_connect()
 */
int bt_ag_is_nrec_enabled(bool *enabled);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Registers a callback function that will be invoked when the speaker gain of the remote device is changed.
 * @since_tizen 2.3
 * @details This function let you know the change of the speaker gain of the remote device.
 * @a gain is represented on a scale from 0 to 15. This value is absolute value relating to a particular volume level.
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_unset_speaker_gain_changed_cb()
 */
int bt_ag_set_speaker_gain_changed_cb(bt_ag_speaker_gain_changed_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Unregisters a callback function that will be invoked when the speaker gain of the remote device is changed.
 * @since_tizen 2.3
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_set_speaker_gain_changed_cb()
 */
int bt_ag_unset_speaker_gain_changed_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Registers a callback function that will be invoked when the microphone gain of the remote device is changed.
 * @since_tizen 2.3
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_unset_microphone_gain_changed_cb()
 */
int bt_ag_set_microphone_gain_changed_cb(bt_ag_microphone_gain_changed_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief  Unregisters a callback function that will be invoked when the microphone gain of the remote device is changed.
 * @since_tizen 2.3
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_ag_set_microphone_gain_changed_cb()
 */
int bt_ag_unset_microphone_gain_changed_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_CALL_MODULE
 * @brief Creates a handle of call list.
 * @since_tizen 2.3
 * @param[out] list  The handle of call list
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_call_list_destroy()
 */
int bt_call_list_create(bt_call_list_h *list);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_CALL_MODULE
 * @brief Destroys the handle of call list.
 * @since_tizen 2.3
 * @param[in] list  The handle of call list
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_call_list_create()
 */
int bt_call_list_destroy(bt_call_list_h list);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_CALL_MODULE
 * @brief Resets the handle of call list.
 * @since_tizen 2.3
 * @param[in] list  The handle of call list
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_call_list_create()
 */
int bt_call_list_reset(bt_call_list_h list);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_CALL_MODULE
 * @brief Adds a call to the handle of call list.
 * @since_tizen 2.3
 * @param[in] list  The handle of call list
 * @param[in] call_id  The call ID
 * @param[in] state  The state of audio gate call
 * @param[in] phone_number The phone number. You must set this value in case of #BT_AG_CALL_EVENT_DIALING and      #BT_AG_CALL_EVENT_INCOMING.
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_call_list_create()
 */
int bt_call_list_add(bt_call_list_h list, unsigned int call_id, bt_ag_call_state_e state, const char *phone_number);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Checks whether the remoted device is wbs (Wide Band Speech) mode or not.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[out] wbs_mode The wbs status: (@c true = wide band speech, @c  false = narrow band speech)
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The remote device is connected by bt_audio_connect() with #BT_AUDIO_PROFILE_TYPE_HSP_HFP service.
 * @see bt_audio_connect()
 */
int bt_ag_is_wbs_mode(bool *wbs_mode);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_AG_MODULE
 * @brief Gets the HF(Hands-Free) profile connected status for AG role.
 * @since_tizen 2.4
 * @param[out] connected the connected status: (@c true = connected , @c  false = not connected )
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 */
int bt_ag_is_connected(bool *connected);

/**
 * @ingroup  CAPI_NETWORK_BLUETOOTH_AUDIO_A2DP_MODULE
 * @brief  Sets copy protection. streaming application that needs to have the copy protection for the streaming data, shall invoke this API.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] status - TRUE/FALSE
 * @return  0 on success, otherwise negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_OPERATION_FAILED  on failure
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 */
int bt_a2dp_set_content_protection(bool status);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief Gets the specification name from the UUID
 * @since_tizen 2.4
 *
 * @remarks @a name must be released with free() by you.
 *
 * @param[in] uuid The UUID
 * @param[out] name The specification name which defined from www.bluetooth.org
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_get_uuid()
 */
int bt_gatt_get_uuid_specification_name(const char *uuid, char **name);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief Destroys the GATT handle
 * @since_tizen 2.4
 *
 * @param[in] gatt_handle The handle of service, characteristic or descriptor
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_service_create()
 * @see bt_gatt_characteristic_create()
 * @see bt_gatt_descriptor_create()
 */
int bt_gatt_destroy(bt_gatt_h gatt_handle);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Gets the permissions which a characteristic or descriptor's GATT handle has
 * @since_tizen 2.4
 *
 * @param[in] gatt_handle The handle of a characteristic or descriptor
 * @param[out] permissions The permissions which a characteristic or descriptor's GATT handle has.
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_characteristic_create()
 * @see bt_gatt_descriptor_create()
 * @see bt_gatt_permission_e
 */
int bt_gatt_get_permissions(bt_gatt_h gatt_handle, int *permissions);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Updates the permissions which a characteristic or descriptor's GATT handle has
 * @since_tizen 2.4
 *
 * @param[in] gatt_handle The handle of a characteristic or descriptor
 * @param[in] permissions The permissions to be updated
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_characteristic_create()
 * @see bt_gatt_descriptor_create()
 * @see bt_gatt_permission_e
 */
int bt_gatt_set_permissions(bt_gatt_h gatt_handle, int permissions);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Creates the GATT service
 * @since_tizen 2.4
 *
 * @param[in] uuid The UUID of the service
 * @param[in] type The type of the service
 * @param[out] service The GATT handle of the created service
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_destroy()
 */
int bt_gatt_service_create(const char *uuid, bt_gatt_service_type_e type,
			   bt_gatt_h *service);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Adds a characteristic to a specified service
 * @since_tizen 2.4
 *
 * @param[in] service The service's GATT handle
 * @param[in] characteristic The characteristic's GATT handle to be added
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_service_create()
 * @see bt_gatt_characteristic_create()
 */
int bt_gatt_service_add_characteristic(bt_gatt_h service,
					bt_gatt_h characteristic);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Adds a service to a specified service as included service
 * @since_tizen 2.4
 *
 * @param[in] service The service's GATT handle
 * @param[in] included_service The service's GATT handle to be added as included service
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_service_create()
 */
int bt_gatt_service_add_included_service(bt_gatt_h service,
					bt_gatt_h included_service);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Gets the GATT server handle to which the specified service belongs
 * @since_tizen 2.4
 *
 * @param[in] service The service's GATT handle
 * @param[out] server The GATT server handle to which @a service belongs
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_server_create()
 * @see bt_gatt_service_create()
 * @see bt_gatt_server_register_service()
 */
int bt_gatt_service_get_server(bt_gatt_h service, bt_gatt_server_h *server);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Creates the GATT characteristic
 * @since_tizen 2.4
 *
 * @param[in] uuid The UUID of the characteristic
 * @param[in] permissions the permissions of the characteristic
 * @param[in] properties The properties of the characteristic
 * @param[in] Value The value(byte stream) of the characteristic
 * @param[in] value_length The length of @a value
 * @param[out] characteristic The GATT handle of the created characteristic
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_destroy()
 */
int bt_gatt_characteristic_create(const char *uuid, int permissions,
				int properties, const char *value, int value_length,
				bt_gatt_h *characteristic);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Adds a descriptor to a specified characteristic
 * @since_tizen 2.4
 *
 * @param[in] characteristic The GATT handle of the characteristic
 * @param[in] descriptor The descriptor's GATT handle to be added
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_characteristic_create()
 * @see bt_gatt_descriptor_create()
 */
int bt_gatt_characteristic_add_descriptor(bt_gatt_h characteristic,
					bt_gatt_h descriptor);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Updates the properties which a characteristic's GATT handle has
 * @since_tizen 2.4
 *
 * @param[in] characteristic The characteristic's GATT handle
 * @param[in] properties The properties to be updated
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_property_e
 */
int bt_gatt_characteristic_set_properties(bt_gatt_h characteristic, int properties);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Creates the GATT characteristic descriptor
 * @since_tizen 2.4
 *
 * @param[in] uuid The UUID of the descriptor
 * @param[in] permissions The permissions of the descriptor
 * @param[in] value The value(byte stream) associated with the descriptor
 * @param[in] value_length The length of @a value
 * @param[out] descriptor The GATT handle of the created characteristic descriptor
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_destroy()
 */
int bt_gatt_descriptor_create(const char *uuid, int permissions,
				const char *value, int value_length,
				bt_gatt_h *descriptor);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief Initializes the GATT Server
 * @since_tizen 2.4
 *
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre	The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @see	bt_gatt_server_deinitialize()
 */
int bt_gatt_server_initialize(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief DeInitializes the GATT server
 * @since_tizen 2.4
 *
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE	Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre	The state of local Bluetooth must be #BT_ADAPTER_ENABLED.
 * @see	bt_gatt_server_initialize()
 */
int bt_gatt_server_deinitialize(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Creates the GATT server's handle
 * @since_tizen 2.4
 *
 * @param[out] server The GATT server's handle
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_server_destroy()
 */
int bt_gatt_server_create(bt_gatt_server_h *server);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Destroys the GATT server's handle
 * @since_tizen 2.4
 *
 * @remarks All registered services to GATT server are unregistered
 *
 * @param[in] server The GATT server's handle
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_server_create()
 */
int bt_gatt_server_destroy(bt_gatt_server_h server);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Registers a callback function to be invoked when a read request for a specified characteristic or descriptor is issued from a remote device(GATT client)
 * @since_tizen 2.4
 *
 * @param[in] gatt_handle The GATT handle of a characteristic or descriptor
 * @param[in] callback The callback to be invoked
 * @param[in] user_data The user data to be passed to @a callback function
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_server_read_value_requested_cb()
 * @see bt_gatt_server_send_response()
 */
int bt_gatt_server_set_read_value_requested_cb(bt_gatt_h gatt_handle,
				bt_gatt_server_read_value_requested_cb callback,
				void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Registers a callback function to be invoked when the remote device enables or disables the Notification/Indication for particular characteristics.
 * @since_tizen 2.4
 *
 * @param[in] gatt_handle The GATT handle of a characteristic or descriptor
 * @param[in] callback The callback to be invoked
 * @param[in] user_data The user data to be passed to @a callback function
 * @return	0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER	Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED	Not supported
 *
 * @see bt_gatt_server_notification_state_change_cb()
 */
int bt_gatt_server_set_notification_state_change_cb(bt_gatt_h gatt_handle,
				bt_gatt_server_notification_state_change_cb callback,
				void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Registers a callback function to be invoked when a value of a characteristic or descriptor has been changed by a remote device(GATT client)'s request
 * @since_tizen 2.4
 *
 * @param[in] gatt_handle The GATT handle of a characteristic or descriptor
 * @param[in] callback The callback to be invoked
 * @param[in] user_data The user data to be passed to @a callback function
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_server_value_changed_cb()
 */
int bt_gatt_server_set_value_changed_cb(bt_gatt_h gatt_handle,
				bt_gatt_server_value_changed_cb callback,
				void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Registers a specified service to the specified GATT server that the local device is hosting
 * @since_tizen 2.4
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 *
 * @param[in] server The GATT server that local device is hosting
 * @param[in] service The service, which needs to be registered in @a server
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_ALREADY_DONE Operation is already done
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_server_create()
 * @see bt_gatt_service_create()
 * @see bt_gatt_server_unregister_service()
 */
int bt_gatt_server_register_service(bt_gatt_server_h server, bt_gatt_h service);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Unregisters a specified service from the specified GATT server that the local device is hosting
 * @since_tizen 2.4
 *
 * @param[in] server The GATT server that local device is hosting
 * @param[in] service The service, which needs to be unregistered from @a server
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_SERVICE_NOT_FOUND  Service not found
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_server_register_service()
 */
int bt_gatt_server_unregister_service(bt_gatt_server_h server, bt_gatt_h service);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Unregisters all services from the specified GATT server that the local device is hosting
 * @since_tizen 2.4
 *
 * @param[in] server The GATT server that local device is hosting
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_server_register_service()
 */
int bt_gatt_server_unregister_all_services(bt_gatt_server_h server);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Sends a response to the remote device as a result of a read request
 * @since_tizen 2.4
 *
 * @remarks Until this function is called, a read request is not finished.
 *
 * @param[in] request_id The identification of a read request
 * @param[in] offset The offset from where a value is read
 * @param[in] value The value to be sent. It will be sent from @a offset. If it is NULL, a requested GATT handle's value will be sent from @a offset.
 * @param[in] value_length Value Length
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_server_read_value_requested_cb()
 */
int bt_gatt_server_send_response(int request_id,
			int offset, char *value, int value_length);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Notifys a value change of the characteristic to the remote devices which enable a Client Characteristic Configuration Descriptor
 * @since_tizen 2.4
 *
 * @param[in] characteristic The characteristic which has a changed value
 * @param[in] need_confirm If it is true, an indication is sent. If it is false, a notification is sent.
 * @param[in] callback The function to be invoked on each remote device when a sending operation is done
 * @param[in] user_data The user data to be passed to @a callback function
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_server_notification_sent_cb()
 */
int bt_gatt_server_notify(bt_gatt_h characteristic, bool need_confirm,
		bt_gatt_server_notification_sent_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Gets a service's GATT handle which has specific UUID
 * @since_tizen 2.4
 *
 * @remark The returned GATT handle must not be freed by application.
 * It will be freed when an associated server is destroyed by bt_gatt_server_destroy().
 * If there are multiple services which have same UUID, only the first matched one will be returned.
 *
 * @param[in] server The GATT server's handle
 * @param[in] uuid The serivce's GATT handle which has this UUID will be returned if it exists
 * @param[out] service The service's GATT handle which has @a uuid if it exists
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NO_DATA  No data available
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 */
int bt_gatt_server_get_service(bt_gatt_server_h server, const char *uuid,
				bt_gatt_h *service);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_GATT_MODULE
 * @brief  Invokes @a callback function on each service that belongs to the specified GATT server
 * @since_tizen 2.4
 *
 * @param[in] server The GATT server's handle
 * @param[in] callback The function to be invoked on each service
 * @param[in] user_data The user data to be passed to @a callback function
 * @return  0 on success, otherwise a negative error value
 * @retval #BT_ERROR_NONE Successful
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @see bt_gatt_foreach_cb()
 */
int bt_gatt_server_foreach_services(bt_gatt_server_h server,
		bt_gatt_foreach_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Registers a callback function that will be invoked when the song position is changed by the remote target device.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @see bt_initialize()
 * @see bt_avrcp_unset_song_position_changed_cb()
 */
int bt_avrcp_set_song_position_changed_cb(bt_avrcp_song_position_changed_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Unregisters a callback function that will be invoked when the song position is changed by the remote target device.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @see bt_initialize()
 * @see bt_avrcp_set_song_position_changed_cb()
 */
int bt_avrcp_unset_song_position_changed_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Registers a callback function that will be invoked when the Play status is changed by the remote target device.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @see bt_initialize()
 * @see bt_avrcp_unset_play_status_changed_cb()
 */
int bt_avrcp_set_play_status_changed_cb(bt_avrcp_play_status_changed_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Unregisters a callback function that will be invoked when the Play status is changed by the remote target device.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @see bt_initialize()
 * @see bt_avrcp_unset_play_status_changed_cb()
 */
int bt_avrcp_unset_play_status_changed_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Registers a callback function that will be invoked when the track metadata is changed by the remote target device.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @see bt_initialize()
 * @see bt_avrcp_unset_track_info_changed_cb()
 */
int bt_avrcp_set_track_info_changed_cb(bt_avrcp_track_info_changed_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  Unregisters a callback function that will be invoked when the track metadata is changed by the remote target device.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @see bt_initialize()
 * @see bt_avrcp_set_track_info_changed_cb()
 */
int bt_avrcp_unset_track_info_changed_cb(void);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief  Registers a callback function that will be invoked when the A2DP Source connection state is changed.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_audio_connection_state_changed_cb()
 */
int bt_a2dp_source_audio_set_connection_state_changed_cb(bt_audio_connection_state_changed_cb callback, void *user_data);

/**
 * @ingroup CAPI_NETWORK_BLUETOOTH_AUDIO_MODULE
 * @brief  Unregisters a callback function that will be invoked when the A2DP Source connection state is changed.
 * @since_tizen @if MOBILE 2.3 @elseif WEARABLE 2.3.1 @endif
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth audio service must be initialized with bt_audio_initialize().
 * @see bt_audio_initialize()
 * @see bt_audio_connection_state_changed_cb()
 * @see bt_audio_set_connection_state_changed_cb()
 */
int bt_a2dp_source_audio_unset_connection_state_changed_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief Initializes the Bluetooth AVRCP(Audio/Video Remote Control Profile) service.
 * @since_tizen 2.4
 * @remarks This function must be called before Bluetooth AVRCP service. \n
 * You must free all resources of the this service by calling bt_avrcp_control_deinitialize()
 * if Bluetooth AVRCP service is no longer needed.
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] callback The callback function called when the connection state is changed
 * @param[in] user_data The user data to be passed to the callback function
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @see bt_initialize()
 * @see bt_avrcp_control_deinitialize()
 */
int bt_avrcp_control_initialize(bt_avrcp_control_connection_state_changed_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief Deinitializes the Bluetooth AVRCP(Audio/Video Remote Control Profile) service.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_avrcp_control_initialize().
 * @see bt_avrcp_control_initialize()
 */
int bt_avrcp_control_deinitialize(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief Connects the Bluetooth AVRCP(Audio/Video Remote Control Profile) service.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_avrcp_control_initialize().
 * @see bt_avrcp_control_initialize()
 */
int bt_avrcp_control_connect(const char *remote_address);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief Disconnects the Bluetooth AVRCP(Audio/Video Remote Control Profile) service.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_avrcp_control_initialize().
 * @see bt_avrcp_control_initialize()
 * @see bt_avrcp_control_connect()
 */
int bt_avrcp_control_disconnect(const char *remote_address);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief Handle the Bluetooth AVRCP(Audio/Video Remote Control Profile) commands.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] AVRCP Commands Play, Pause, Next, Rewind.
 * @return 0 on success, otherwise a negative error value.
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 * @pre The Bluetooth audio service must be initialized with bt_avrcp_control_initialize().
 * @see bt_avrcp_control_initialize()
 * @see bt_avrcp_control_connect()
 */
int bt_avrcp_control_send_player_command(bt_avrcp_player_command_e cmd);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  send the equalizer change to the remote device.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] state To set the equalizer state ON/OFF
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_control_set_equalizer_state(bt_avrcp_equalizer_state_e state);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  get the the equalize state of the remote device.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] equalizer To get the equalizer value ON/OFF
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_control_get_equalizer_state(bt_avrcp_equalizer_state_e *state);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  send the repeat change to the remote device.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] Mode To set the repeat mode OFF, SINGLE, ALL
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_control_set_repeat_mode(bt_avrcp_repeat_mode_e mode);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  get the repeat state of the remote device.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] Mode To get the repeat mode OFF, SINGLE, ALL
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_control_get_repeat_mode(bt_avrcp_repeat_mode_e *mode);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  send the shuffle change to the remote device.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] Mode To set the shuffle mode OFF, GROUP, ALL
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_control_set_shuffle_mode(bt_avrcp_shuffle_mode_e mode);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  get the shuffle state of the remote device.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] Mode To get the shuffle mode OFF, GROUP, ALL
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_control_get_shuffle_mode(bt_avrcp_shuffle_mode_e *mode);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  send the scan mode change to the remote device.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] Mode To set the scan mode OFF, GROUP, ALL
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_control_set_scan_mode(bt_avrcp_scan_mode_e mode);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  get the scan mode of the remote device.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] Mode To get the scan mode OFF, GROUP, ALL
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_control_get_scan_mode(bt_avrcp_scan_mode_e *mode);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  get the song position vale of the remote device.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] Mode To get the position value in milliseconds
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_control_get_position(unsigned int *position);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  get the play status of the remote device.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] Mode To get the play status PLAYING, STOPPED...
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_control_get_play_status(bt_avrcp_player_state_e *status);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  get the metadata of the remote device.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] track To get the metadata of track played in remote device.
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device must be connected.
 * @see bt_avrcp_target_connection_state_changed_cb()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_control_get_track_info(bt_avrcp_metadata_attributes_info_s **track);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_AVRCP_MODULE
 * @brief  to free the metadata  staructure.
 * @since_tizen 2.4
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @param[in] track To free the memory of metadata structure.
 * @return  0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @pre track should point to valid metadata address.
 * @see bt_avrcp_control_get_track_info()
 * @see bt_avrcp_target_initialize()
 */
int bt_avrcp_control_free_track_info(bt_avrcp_metadata_attributes_info_s *track);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief Activates the NAP(Network Access Point).
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_ALREADY_DONE  Operation is already done
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The state of local Bluetooth must be #BT_ADAPTER_ENABLED
 * @see bt_nap_deactivate()
 */
int bt_nap_activate(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief Deactivates the NAP(Network Access Point).
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_ALREADY_DONE  Operation is already done
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth NAP service must be activated with bt_nap_activate().
 * @see bt_nap_activate()
 */
int bt_nap_deactivate(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief Disconnects the all PANUs(Personal Area Networking User) which are connected to you.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth NAP service must be activated with bt_nap_activate().
 * @see bt_nap_activate()
 */
int bt_nap_disconnect_all(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief Disconnects the specified PANU(Personal Area Networking User) which is connected to you.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] remote_address  The remote address
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth NAP service must be activated with bt_nap_activate().
 * @see bt_nap_activate()
 */
int bt_nap_disconnect(const char *remote_address);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief  Registers a callback function that will be invoked when the connection state changes.
 * @since_tizen 2.3
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_nap_connection_state_changed_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_nap_connection_state_changed_cb()
 * @see bt_nap_unset_connection_state_changed_cb()
 */
int bt_nap_set_connection_state_changed_cb(bt_nap_connection_state_changed_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_NAP_MODULE
 * @brief  Unregisters a callback function that will be invoked when the connection state changes.
 * @since_tizen 2.3
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_nap_connection_state_changed_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_nap_connection_state_changed_cb()
 * @see bt_nap_set_connection_state_changed_cb()
 */
int bt_nap_unset_connection_state_changed_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE
 * @brief  Registers a callback function that will be invoked when the connection state changes.
 * @since_tizen 2.3
 * @param[in] callback The callback function to register
 * @param[in] user_data The user data to be passed to the callback function
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_nap_connection_state_changed_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_panu_connection_state_changed_cb()
 * @see bt_panu_unset_connection_state_changed_cb()
 */
int bt_panu_set_connection_state_changed_cb(bt_panu_connection_state_changed_cb callback, void *user_data);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE
 * @brief  Unregisters a callback function that will be invoked when the connection state changes.
 * @since_tizen 2.3
 * @return   0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized with bt_initialize().
 * @post bt_nap_connection_state_changed_cb() will be invoked.
 * @see bt_initialize()
 * @see bt_panu_connection_state_changed_cb()
 * @see bt_panu_set_connection_state_changed_cb()
 */
int bt_panu_unset_connection_state_changed_cb(void);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE
 * @brief Connects the remote device with the PAN(Personal Area Networking) service, asynchronously.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] remote_address  The remote address
 * @param[in] type  The type of PAN service
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The local device must be bonded with the remote device by bt_device_create_bond().
 * @post bt_panu_connection_state_changed_cb() will be invoked.
 * @see bt_panu_disconnect()
 * @see bt_panu_connection_state_changed_cb()
 */
int bt_panu_connect(const char *remote_address, bt_panu_service_type_e type);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_PAN_PANU_MODULE
 * @brief Disconnects the remote device with the PAN(Personal Area Networking) service, asynchronously.
 * @since_tizen 2.3
 * @privlevel public
 * @privilege %http://tizen.org/privilege/bluetooth
 * @param[in] remote_address  The remote address
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #BT_ERROR_NOT_ENABLED  Not enabled
 * @retval #BT_ERROR_REMOTE_DEVICE_NOT_CONNECTED  Remote device is not connected
 * @retval #BT_ERROR_OUT_OF_MEMORY  Out of memory
 * @retval #BT_ERROR_OPERATION_FAILED  Operation failed
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The remote device must be connected by bt_panu_connect().
 * @post bt_panu_connection_state_changed_cb() will be invoked.
 * @see bt_panu_connect()
 * @see bt_panu_connection_state_changed_cb()
 */
int bt_panu_disconnect(const char *remote_address);

/**
 * @internal
 * @ingroup CAPI_NETWORK_BLUETOOTH_DEVICE_MODULE
 * @brief   update LE connection.
 * @since_tizen 2.3
 * @privlevel platform
 * @privilege %http://tizen.org/privilege/bluetooth.admin
 * @return 0 on success, otherwise a negative error value.
 * @retval #BT_ERROR_NONE  Successful
 * @retval #BT_ERROR_NOT_INITIALIZED  Not initialized
 * @retval #BT_ERROR_PERMISSION_DENIED  Permission denied
 * @retval #BT_ERROR_NOT_SUPPORTED  Not supported
 *
 * @pre The Bluetooth service must be initialized by bt_initialize().
 * @pre The remote device must be connected with bt_gatt_connect().
 */
int bt_device_le_conn_update(const char *device_address,
			     const bt_le_conn_update_s *parameters);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __TIZEN_NETWORK_BLUETOOTH_INTERNAL_H__
