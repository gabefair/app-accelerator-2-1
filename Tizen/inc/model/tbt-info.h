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
 * @file	tbt-info.h
 * @brief	This file provides the basic structure of all modules.
 * @since_tizen 2.3
 * @bug
 * @credit
 */
#ifndef __TBT_INFO_H__
#define __TBT_INFO_H__

#include <sensor.h>
#include <app.h>

typedef struct _tbt_info tbt_info;

typedef enum{
	TBT_INFO_RESULT_NOT_TESTED=0,
	TBT_INFO_RESULT_PASS=1,
	TBT_INFO_RESULT_FAIL=2,
	TBT_INFO_RESULT_NOT_SUPPORTED=3
} tbt_info_result;

/**
 * @brief main Item for tbt list
 * @since_tizen 2.3
 */
typedef enum {
    TBT_APP_CAMERA_CAPTURE = 0,
    TBT_APP_CAMERA_RECORD,
    TBT_APP_CAMERA_FACE_DETECT,
    TBT_APP_CAMERA_HDR_CAPTURE,

    TBT_APP_SOUND_AUDIO,
    TBT_APP_SOUND_VOLUME,

    TBT_APP_GPS_GPS,

    TBT_APP_BT_CLIENT_PAIRED,
    TBT_APP_BT_SERVER_PAIRED,
    TBT_APP_BT_AUDIO_CONNECT,
    TBT_APP_BT_AUDIO_CONNECT_HANDSFREE,
    TBT_APP_BT_AUTHORIZATION,
    TBT_APP_BT_OPP_CLIENT,
    TBT_APP_BT_OPP_SERVER,
    TBT_APP_BT_PROFILES,
    TBT_APP_BT_SDP,
    TBT_APP_BT_SERVER_SOCKET,
    TBT_APP_BT_CLIENT_SOCKET,
    TBT_APP_BT_HID_CONNECT,
    TBT_APP_BT_OOB_PERIPHERAL,
    TBT_APP_BT_OOB_DEVICE,
    TBT_APP_BT_AVRCP,
    TBT_APP_BT_AG,
    TBT_APP_BT_HDP,
    
    TBT_APP_BLE_GATT_CLIENT,
    TBT_APP_BLE_GATT_SERVER,


    TBT_APP_NFC_P2P,
    TBT_APP_NFC_TAG,

    TBT_APP_DISPLAY_DISPLAY,

    TBT_APP_GRAPHICS_GRAPHICS,

    TBT_APP_IDEV_NUM_KEYPAD,
    TBT_APP_IDEV_PH_NUM_KEYPAD,
    TBT_APP_IDEV_IP_KEYPAD,

    TBT_APP_LOCAL_MPEG4,
    TBT_APP_LOCAL_H263,
    TBT_APP_LOCAL_H264,

    TBT_APP_IMAGEVIEW_JPEG,
    TBT_APP_IMAGEVIEW_BMP,
    TBT_APP_IMAGEVIEW_GIF,
    TBT_APP_IMAGEVIEW_PNG,

    TBT_APP_DC_INSERT,
    TBT_APP_DC_DELETE,
    TBT_APP_DC_UPDATE,
    TBT_APP_DC_SELECT,
    TBT_APP_DC_CURSOR,
    TBT_APP_DC_MAP,

    TBT_APP_PICK_VIEW_APP,
    TBT_APP_PICK_PICK_APP,

    TBT_APP_SENSOR_ACCELEROMETER,
    TBT_APP_SENSOR_GYROSCOPE,
    TBT_APP_SENSOR_PROXIMITY,
    TBT_APP_SENSOR_LIGHT,
    TBT_APP_SENSOR_ULTRA_VIOLET,
    TBT_APP_SENSOR_MAGNATOMETER,
    TBT_APP_SENSOR_PRESSURE,


    TBT_APP_ANIMATION_FADE,
    TBT_APP_ANIMATION_POINT,
    TBT_APP_ANIMATION_DIMENSION,
    TBT_APP_ANIMATION_RECT,
    TBT_APP_ANIMATION_ROTATE,

    TBT_APP_UI_RESIZE,
    TBT_APP_UI_SHAPE,
    TBT_APP_UI_ROTATE,
    TBT_APP_UI_FONT,
    TBT_APP_UI_LINE,
    TBT_APP_UI_ALPHA,

    TBT_APP_SL_SCREENLOCK,

    TBT_APP_TOUCH_TOUCH,

    TBT_APP_MOCK,
    TBT_APP_EVENT_VIEW,

    TBT_APP_FEATURE_SUMMARY,
    
	TBT_APP_RUNTIME_INFO,
	
	TBT_APP_OAUTH2,

    TBT_APP_PUSH_SERVICE,

    TBT_APP_MAX,

    TBT_APP_EFL_CALLBACK,

    TBT_APP_SHORTCUT,

    TBT_APP_IME_ALPHABATIC,

    TBT_APP_IME_NUMERIC,

    TBT_APP_IME_EVENT,

    TBT_APP_IME_CONTEXT_GET,

    TBT_APP_WIDGET

} tbt_app_type_e;


/**
 * @brief Measurement units for sensors
 * @since_tizen 2.3
 */
typedef enum {
    SENSOR_UNIT_METRE_PER_SECOND_SQUARED,
    SENSOR_UNIT_MICRO_TESLA,
    SENSOR_UNIT_DEGREE,
    SENSOR_UNIT_LUX,
    SENSOR_UNIT_CENTIMETER,
    SENSOR_UNIT_DEGREE_PER_SECOND,
    SENSOR_UNIT_HECTOPASCAL,
    SENSOR_UNIT_MAX
} sensor_unit_e;



/**
 * @brief Extra value getter function
 * @since_tizen 2.3
 * @param[in]   info    Sensor info
 * @param[in]   values  Values received from the sensor
 * @return Calculated value
 */
typedef float (*sensor_extra_value_get_cb)(const tbt_info *info, const float *values);

/**
 * @brief Extra value associated with the sensor
 * @since_tizen 2.3
 * @details Allows to add value which is not received from the sensor,
 *          but instead calculated based on sensor values
 */
typedef struct _sensor_extra_value
{
    const char *name;   /**< Value name */
    const char *format; /**< Value printf() format for text representation */
    sensor_extra_value_get_cb value_get; /**< Value getter function */
} sensor_extra_value;

/**
 * @brief Module metadata
 * @since_tizen 2.3
 */
struct _tbt_info
{

    const char *name;        			//tbt display name
    const char *parent;      			//tbt display name
    tbt_app_type_e apptype;        		//Values measurement units
    tbt_info_result result;              			//app status: pass or fail
    const char *info;  					//Values measurement units string representation
    const char *icon_name;  			//Values measurement units string representation
    const char *file_name;  			//Values measurement units string representation
    const char *extra_file_name;
    const char *layout_file;  			//Values measurement units string representation
    const char *layout_group;  			//Values measurement units string representation
    sensor_h sensor;               		/**< Sensor handle */
    sensor_type_e sensortype;      		/**< Sensor type, @see sensor_type_e */
    sensor_unit_e units;     			/**< Values measurement units */
    int sensor_response_interval;
    const char **value_names;  			/**< Value names array of value_count size */
    int value_count;        			/**< Values count */
    float value_min;        			/**< Minimal value */
    float value_max;        			/**< Maximal value */
    float value_range;      			/**< Values range */
    const char *units_str;  			/**< Values measurement units string representation */
    const char *value_format;  			/**< Values printf() format for text representation */
    int axes[3];            			/**< Sensor axes directions relative to screen coordinate system */
    sensor_extra_value *extra_values;	/**< Sensor variable to retrieve the sensor extra values */
    int extra_value_count;				/**< The count of the extra values */
    bool is_push_from_outside;			/**< Checks if push data is received or not */
    char* push_operation_id;			/**< Operation id of push data */
    app_control_h push_app_control;		/**< App conrol handler*/
    int required_features_count;		/**< Number of features required */
    int features_required[16];			/**< Index/indexs of features required */

};

/**
 * @brief Feature List
 * @since_tizen 2.3
 */
static char *feature_list[1024] =
{
	"tizen.org/feature/camera",
	"tizen.org/feature/camera.back",
	"tizen.org/feature/camera.back.flash",
	"tizen.org/feature/camera.front",
	"tizen.org/feature/camera.front.flash",
	"tizen.org/feature/database.encryption",
	"tizen.org/feature/fmradio",
	"tizen.org/feature/graphics.acceleration",
	"tizen.org/feature/input.keyboard",
	"tizen.org/feature/input.keyboard.layout",
	"tizen.org/feature/location",
	"tizen.org/feature/location.gps",
	"tizen.org/feature/location.wps",
	"tizen.org/feature/microphone",
	"tizen.org/feature/multi_point_touch.point_count",
	"tizen.org/feature/multi_point_touch.pinch_zoom",
	"tizen.org/feature/network.bluetooth",
	"tizen.org/feature/network.nfc",
	"tizen.org/feature/network.nfc.reserved_push",
	"tizen.org/feature/network.push",
	"tizen.org/feature/network.secure_element",
	"tizen.org/feature/network.telephony",
	"tizen.org/feature/network.telephony.mms",
	"tizen.org/feature/network.telephony.sms.cbs",
	"tizen.org/feature/network.wifi",
	"tizen.org/feature/network.wifi.direct",
	"tizen.org/feature/opengles.texture_format.3dc",
	"tizen.org/feature/opengles.texture_format.atc",
	"tizen.org/feature/opengles.texture_format.etc",
	"tizen.org/feature/opengles.texture_format.ptc",
	"tizen.org/feature/opengles.texture_format.pvrtc",
	"tizen.org/feature/opengles.texture_format.utc",
	"tizen.org/feature/opengles.version.1_1",
	"tizen.org/feature/opengles.version.2_0",
	"tizen.org/feature/platform.core.cpu.arch.armv7",
	"tizen.org/feature/platform.core.fpu.arch.sse2",
	"tizen.org/feature/platform.core.fpu.arch.sse3",
	"tizen.org/feature/platform.core.fpu.arch.ssse3",
	"tizen.org/feature/platform.core.fpu.arch.vfpv3",
	"tizen.org/feature/platform.core.cpu.arch.x86",
	"tizen.org/feature/platform.native.osp_compatible",
	"tizen.org/feature/screen.auto_rotation",
	"tizen.org/feature/sensor.accelerometer",
	"tizen.org/feature/sensor.accelerometer.wakeup",
	"tizen.org/feature/sensor.gyroscope",
	"tizen.org/feature/sensor.gyroscope.wakeup",
	"tizen.org/feature/sensor.magnetometer",
	"tizen.org/feature/sensor.magnetometer.wakeup",
	"tizen.org/feature/sensor.proximity",
	"tizen.org/feature/sensor.proximity.wakeup",
	"tizen.org/feature/sensor.tiltmeter",
	"tizen.org/feature/sensor.tiltmeter.wakeup",
	"tizen.org/feature/shell.appwidget",
	"tizen.org/feature/network.bluetooth",
	"tizen.org/feature/sip.voip",
	"tizen.org/feature/speech.recognition",
	"tizen.org/feature/speech.synthesis",
	"tizen.org/feature/usb.accessory",
	"tizen.org/feature/usb.host",
	"tizen.org/feature/vision.face_recognition",
	"tizen.org/feature/vision.image_recognition",
	"tizen.org/feature/vision.qrcode_generation",
	"tizen.org/feature/vision.qrcode_recognition",
	"tizen.org/feature/bluetooth",
	"tizen.org/feature/bluetooth.le",
	"tizen.org/feature/network.bluetooth.le",
	"tizen.org/feature/bluetooth.audio.media",
	"tizen.org/feature/network.bluetooth.audio.media",
	"tizen.org/feature/bluetooth.audio.call",
	"tizen.org/feature/network.bluetooth.audio.call",
	"tizen.org/feature/bluetooth.hdp",
	"tizen.org/feature/network.bluetooth.hdp"
	"tizen.org/feature/bluetooth.hid",
	"tizen.org/feature/network.bluetooth.hid",
	"tizen.org/feature/bluetooth.opp",
	"tizen.org/feature/network.bluetooth.opp",
	"tizen.org/feature/bluetooth.spp",
	"tizen.org/feature/network.nfc.p2p",
	"tizen.org/feature/network.nfc.tag"
};

#endif /* __TBT_INFO_H__ */
