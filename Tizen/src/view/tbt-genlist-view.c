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
 * @file	tbt-genlist-view.h
 * @brief	Brief description of file.
 *			Detailed description of file.
 *
 * @bug		No Known bugs
 * @credit	Tizen SDK Version 2.3.0 Sample Example
 *
 */
#include "utils/app_module_config.h"

#include <assert.h>
#include <stdbool.h>
#include <Elementary.h>

#ifdef TIZEN_3_0
	#include <efl_extension.h>
#endif
#ifdef TIZEN_2_4
	#include <efl_extension.h>
#endif
#ifdef TIZEN_2_3_1
	#include "efl_extension_events.h"
#endif

#include "utils/config.h"
#include "utils/logger.h"
#include "utils/ui-utils.h"
#include "model/tbt-list.h"

#include "view/tbt-ui-view.h"
#include "view/tbt-bluetooth-view.h"

#include "view/tbt-genlist-view.h"
#include "view/tbt-bluetooth-view.h"
#include "view/tbt-efl-callbacks.h"
#include "view/tbt-shortcut-view.h"
#include "view/tbt-bluetoothoob-view.h"
#include "view/tbt-bluetoothle-view.h"

typedef struct
{
	Evas_Object *navi;
	Evas_Object *genlist;
    Elm_Object_Item *navi_item;
}genlist_view;
static char* group_index_tag;
typedef struct
{
	int index;
	Elm_Object_Item *item;
}item_data;
static Elm_Object_Item *group_item_array[100];
static int group_item_count = 0;

static void tbt_genlist_view_fill(genlist_view *view);

static void _gl_del_cb(void *data, Evas_Object *obj EINA_UNUSED);
static void _gl_expanded_cb(void *data, Evas_Object *obj, void *event_info);
static char *_gl_text_get_cb(void *data, Evas_Object *obj, const char *part);
static Evas_Object *_gl_content_get_cb(void *data, Evas_Object *obj, const char *part);
static void _gl_item_selected_cb(void *data, Evas_Object *obj EINA_UNUSED, void *event_info);
static void _gl_loaded_cb(void *data EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED);
static void _gl_realized_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info);
static void _gl_contracted_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info);
static void _gl_longpressed_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info);
static void _gl_group_selected_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info);


/**
 * @function		tbt_genlist_view_create
 * @since_tizen		2.3
 * @description		Tbt Genlist View Create
 * @parameter		Evas_Object*: Evas Object Pointer
 * @return		Evas_Object*
 */
Evas_Object *tbt_genlist_view_create(Evas_Object *navi)
{
	RETVM_IF(NULL == navi, NULL, "navi is null");

    genlist_view *view = NULL;
    view = calloc(1, sizeof(genlist_view));
    RETVM_IF(!view, NULL, "calloc failed");

    #ifdef DEVICE_TYPE_MOBILE
		group_index_tag = "group_index";
    #else
		group_index_tag = "groupindex";
	#endif

    view->navi = navi;
    view->genlist = elm_genlist_add(view->navi);
    if(!view->genlist)
    {
        SAFE_DELETE(view);
        return NULL;
    }

    tbt_list_init();
    tbt_genlist_view_fill(view);

	view->navi_item = elm_naviframe_item_push(view->navi, "TBT", NULL, NULL, view->genlist, NULL);

	return view->genlist;
}


/**
 * @function		tbt_genlist_view_fill
 * @since_tizen		2.3
 * @description		Tbt Genlist View Fill
 * @parameter		genlist_view*: Genlist View Pointer
 * @return		static void
 */
static void tbt_genlist_view_fill(genlist_view *view)
{
	int index;
	group_item_count = 0;
	unsigned count = 0;
	const tbt_info *end;
	tbt_info *item;
	Elm_Object_Item *git;

	item = (tbt_info*)tbt_list_get(&count);
	RETM_IF(!item, "item is NULL");
	end = item + count;

	//Create item class
	Elm_Genlist_Item_Class *itc = elm_genlist_item_class_new();
    Elm_Genlist_Item_Class *gitc = elm_genlist_item_class_new();

	gitc->item_style = group_index_tag;
	gitc->func.text_get = _gl_text_get_cb;
	gitc->func.content_get = _gl_content_get_cb;
	gitc->func.del = _gl_del_cb;

	itc->item_style = "default";
	itc->func.text_get = _gl_text_get_cb;
	itc->func.content_get = _gl_content_get_cb;
	itc->func.del = _gl_del_cb;

	elm_scroller_single_direction_set(view->genlist, ELM_SCROLLER_SINGLE_DIRECTION_HARD);

	//HOMOGENEOUS MODE
	//If item height is same when each style name is same, Use homogeneous mode.
	elm_genlist_homogeneous_set(view->genlist, EINA_TRUE);

	//COMPRESS MODE
	//For the Mobile view, because it has full window, compresse mode should be used.
	elm_genlist_mode_set(view->genlist, ELM_LIST_COMPRESS);

	//Add Smart Callback
	evas_object_smart_callback_add(view->genlist, "realized", _gl_realized_cb, NULL);
	evas_object_smart_callback_add(view->genlist, "loaded", _gl_loaded_cb, NULL);
	evas_object_smart_callback_add(view->genlist, "longpressed", _gl_longpressed_cb, NULL);
	evas_object_smart_callback_add(view->genlist, "expanded", _gl_expanded_cb, view);
	evas_object_smart_callback_add(view->genlist, "contracted", _gl_contracted_cb, NULL);


	char parent[MAX_LENGTH] = {'\0'};
	char groupIcon[128];

	index = count -1;
	item = (tbt_info*)(end -1);//last valide item of the list

	while(index >= 0)
	{
		///
		/// lets available features and set tbt_info
		///
		// FEATURE
		bool features_exists=true;
		int feature_count=item->required_features_count;
		while(features_exists && feature_count-->0)
		{
			features_exists = does_feature_exist(feature_list[item->features_required[feature_count]]);
		}

		if(!features_exists) item->result=TBT_INFO_RESULT_NOT_SUPPORTED;

		if(&parent[0] != NULL && strcmp(parent, item->parent) != 0)
		{
			item_data *id = NULL;
			id = calloc(sizeof(item_data), 1);
			RETM_IF(!id, "calloc is NULL");
			id->index = index;

			strncpy(parent, item->parent, sizeof(parent));
			snprintf(groupIcon, sizeof(groupIcon), "%simages/%s",app_get_resource_path(),"g_camera.png");
			git = elm_genlist_item_prepend(view->genlist,			    //genlist object
										  gitc,							//item class
										  id,							//item class user data
										  NULL,
										  ELM_GENLIST_ITEM_TREE,		//item type
										  _gl_group_selected_cb,	    //select smart callback
										  id);						    //smart callback user data
			id->item = git;
			group_item_array[group_item_count++] = git;
			elm_genlist_item_expanded_set(git, EINA_TRUE);

		}
		index --;
		item --;
	}
	elm_genlist_item_class_free(gitc);
	elm_genlist_item_class_free(itc);


	//FIXME: this code was working fine in 2.3, it is need for viewing the genlist from top. In 2.4 it causes launch problem
	//elm_genlist_item_expanded_set(group_item_array[0], EINA_FALSE);
	evas_object_show(view->genlist);
	elm_genlist_item_show( elm_genlist_nth_item_get(view->genlist, 0) , ELM_GENLIST_ITEM_SCROLLTO_TOP);
}


/**
 * @function		_gl_text_get_cb
 * @since_tizen		2.3
 * @description		 Gl Text Get Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, const char*: Const char Pointer
 * @return		static char*
 */
static char *_gl_text_get_cb(void *data, Evas_Object *obj, const char *part)
{
	item_data *id = data;
	RETVM_IF(!id, NULL, "item is NULL");
	RETVM_IF(!id->item, NULL, "item is NULL");

	char buf[1024];
	const Elm_Genlist_Item_Class *itc = elm_genlist_item_item_class_get(id->item);

	const tbt_info *item = NULL;
	item = tbt_list_item_get(id->index);
	RETVM_IF(!item, NULL, "item is NULL");

	DBG("ITEM STYLE: %s", itc->item_style);

	if(itc->item_style && !strcmp(itc->item_style, group_index_tag))
	{
		/*if(part && !strcmp(part, "elm.text.main"))*/
		if(part && !strcmp(part, "elm.text"))
		{
			snprintf(buf, 1023, "%s", item->parent);
			return strdup(buf);
		}
	}
	else if(itc->item_style && !strcmp(itc->item_style, "default"))
	{

		if(part && !strcmp(part, "elm.text"))
		{
			return strdup(item->name);
		}


	}

	return NULL;
}


/**
 * @function		_gl_content_get_cb
 * @since_tizen		2.3
 * @description		 Gl Content Get Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, const char*: Const char Pointer
 * @return		static Evas_Object*
 */
static Evas_Object *_gl_content_get_cb(void *data, Evas_Object *obj, const char *part)
{
	item_data *id = data;
	RETVM_IF(!id, NULL, "item is NULL");
	RETVM_IF(!id->item, NULL, "item is NULL");

	const tbt_info *item = NULL;
	item = tbt_list_item_get(id->index);
	RETVM_IF(!item, NULL, "item is NULL");


	Evas_Object *content = NULL;
	const Elm_Genlist_Item_Class *itc = elm_genlist_item_item_class_get(id->item);

	DBG("hoho PART NAME: %s", part);
	if(itc->item_style &&
	   !strcmp(itc->item_style, "default"))
	{
		DBG("hoho almost I m here!!!");
		if(part &&
		   !strcmp(part, "elm.swallow.icon"))
		{
			DBG("hoho I m here!!!");
			switch(item->result)
			{
				case TBT_INFO_RESULT_NOT_TESTED:
					content = ui_utils_icon_create_new(obj, "icon/app_not_tested.png");
					break;
				case TBT_INFO_RESULT_PASS:
					content = ui_utils_icon_create_new(obj, "icon/app_passed.png");
					break;
				case TBT_INFO_RESULT_FAIL:
					content = ui_utils_icon_create_new(obj, "icon/app_failed.png");
					break;
				case TBT_INFO_RESULT_NOT_SUPPORTED:
					content = ui_utils_icon_create_new(obj, "icon/app_not_supported.png");
					break;
				default:
					content = ui_utils_icon_create_new(obj, "icon/app_not_supported.png");
					break;
			}
			//elm_layout_content_set(content, "elm.swallow.icon", icon);
			//This code is needed to make the icon visiable in the wearable targerts
			evas_object_size_hint_min_set(content, 50, 50);
			evas_object_size_hint_max_set(content, 50, 50);

		}
	}

	return content;
}


/**
 * @function		_gl_del_cb
 * @since_tizen		2.3
 * @description		 Gl Del Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer
 * @return		static void
 */
static void _gl_del_cb(void *data, Evas_Object *obj EINA_UNUSED)
{
	//Unrealized callback can be called after this.
	//Accessing item_data_s can be dangerous on unrealized callback.
	item_data *id = data;
	RETM_IF(!id, "item is NULL");
	SAFE_DELETE(id);
}


/**
 * @function		_gl_loaded_cb
 * @since_tizen		2.3
 * @description		 Gl Loaded Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _gl_loaded_cb(void *data EINA_UNUSED, Evas_Object *obj, void *event_info EINA_UNUSED)
{

}


/**
 * @function		_gl_realized_cb
 * @since_tizen		2.3
 * @description		 Gl Realized Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _gl_realized_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info)
{
	Elm_Object_Item *it = event_info;
	item_data *id = elm_object_item_data_get(it);
	RETM_IF(!id, "item is NULL");
}


/**
 * @function		_gl_longpressed_cb
 * @since_tizen		2.3
 * @description		 Gl Longpressed Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _gl_longpressed_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info)
{
	Elm_Object_Item *it = event_info;
	item_data *id = elm_object_item_data_get(it);
	RETM_IF(!id, "item is NULL");
}


/**
 * @function		_gl_expanded_cb
 * @since_tizen		2.3
 * @description		 Gl Expanded Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _gl_expanded_cb(void *data, Evas_Object *obj, void *event_info)
{
	RETM_IF(!data, "data is NULL");
	

	genlist_view *view;
	view = (genlist_view*)data;
	RETM_IF(!view, "view is NULL");

	Elm_Object_Item *it;
	Elm_Object_Item *git = event_info;
	item_data *item = elm_object_item_data_get(git);

	int index = 0;
	int item_count = 0;

	item_count = tbt_list_item_count(item->index);

	if(item_count < 0)
	{
		return;
	}
	DBG("Item Count : %d index : %d",item_count,item->index);
	Elm_Genlist_Item_Class *itc = elm_genlist_item_class_new();

	itc->item_style = "default";
	itc->func.text_get = _gl_text_get_cb;
	itc->func.content_get = _gl_content_get_cb;
	itc->func.del = _gl_del_cb;

	for(index = item_count; index <= item->index; index++)
	{
		item_data *id = calloc(sizeof(item_data), 1);
		RETM_IF(!id, "calloc failed");
		id->index = index;

			    it = elm_genlist_item_append(obj,					//genlist object
												 itc,					//item class
												 id,						//item class user data
												 git,					//parent item
												 ELM_GENLIST_ITEM_NONE,	//item type
												 _gl_item_selected_cb,	//select smart callback
												 view);

		id->item = it;
	}

	elm_genlist_item_class_free(itc);
}


/**
 * @function		_gl_contracted_cb
 * @since_tizen		2.3
 * @description		 Gl Contracted Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _gl_contracted_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info)
{
   Elm_Object_Item *it = event_info;
   elm_genlist_item_subitems_clear(it);
}


/**
 * @function		_gl_item_selected_cb
 * @since_tizen		2.3
 * @description		 Gl Item Selected Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _gl_item_selected_cb(void *data, Evas_Object *obj EINA_UNUSED, void *event_info)
{
	RETM_IF(!data, "data is NULL");
	

	genlist_view *view = NULL;
	view = (genlist_view*)data;
	RETM_IF(!view, "view is NULL");

	Elm_Object_Item *it = event_info;
	item_data *id = elm_object_item_data_get(it);
	RETM_IF(!id, "item is NULL");

	//Un-highlight item
	elm_genlist_item_selected_set(it, EINA_FALSE);

	tbt_info *info = NULL;
	info = tbt_list_item_get(id->index);
	RETM_IF(!info, "item is NULL");



	///
	/// lets check all features exists or not
	///

	bool features_exists = true;
	char *required_feature = NULL;

	int feature_count = info->required_features_count;

	while(features_exists && feature_count-->0)
	{
		features_exists = does_feature_exist(feature_list[info->features_required[feature_count]]);
		required_feature = feature_list[info->features_required[feature_count]];
	}


	if(!features_exists)
	{
		// As feature doesn't exists show a friendly popup, and exit
		Evas_Object *popup = ui_utils_popup_add(view->navi, "Feature Not Supported");
		char msg[512];
		snprintf(msg, sizeof(msg),"<b>'%s'</b><br/>requires feature:<br/>'%s'<br/>Which is not supported.",info->name,required_feature);

		elm_object_text_set(popup, msg);

		DBG("__BB Calling popup done. r	eturning");
		RETM_IF(true, "'%s' can't start because feature '%s' doesn't supported in this device.",info->name,required_feature);
	}




    switch(info->apptype)
    {

		#ifdef TBT_MODULE_BT
			case TBT_APP_BT_CLIENT_PAIRED:
				bluetooth_view_add(view->navi, info, it);
				break;
			case TBT_APP_BT_SERVER_PAIRED:
				bluetooth_view_add(view->navi, info, it);
				break;
			case TBT_APP_BT_AUDIO_CONNECT:
				bluetooth_view_add(view->navi, info, it);
				break;
			case TBT_APP_BT_AUDIO_CONNECT_HANDSFREE:
				bluetooth_view_add(view->navi, info, it);
				break;
			case TBT_APP_BT_AUTHORIZATION:
				bluetooth_view_add(view->navi, info, it);
				break;
			case TBT_APP_BT_OPP_CLIENT:
				bluetooth_view_add(view->navi, info, it);
				break;
			case TBT_APP_BT_OPP_SERVER:
				bluetooth_view_add(view->navi, info, it);
				break;
			case TBT_APP_BT_PROFILES:
				bluetooth_view_add(view->navi, info, it);
				break;
			case TBT_APP_BT_SDP:
				bluetooth_view_add(view->navi, info, it);
				break;
			case TBT_APP_BT_SERVER_SOCKET:
				bluetooth_view_add(view->navi, info, it);
				break;
			case TBT_APP_BT_CLIENT_SOCKET:
				bluetooth_view_add(view->navi, info, it);
				break;
			case TBT_APP_BT_HID_CONNECT:
				bluetooth_view_add(view->navi, info, it);
				break;
			case TBT_APP_BT_AVRCP:
				bluetooth_view_add(view->navi, info, it);
				break;
			case TBT_APP_BT_AG:
				bluetooth_view_add(view->navi, info, it);
				break;
			case TBT_APP_BT_HDP:
				bluetooth_view_add(view->navi, info, it);
				break;
			#endif

		#ifdef TBT_MODULE_BT_OOB
			case TBT_APP_BT_OOB_PERIPHERAL:
				bluetoothoob_view_add(view->navi, info, it);
				break;
			case TBT_APP_BT_OOB_DEVICE:
				bluetoothoob_view_add(view->navi, info, it);
				break;
		#endif

		#ifdef TBT_MODULE_BLE
			case TBT_APP_BLE_GATT_CLIENT:
				bluetoothle_view_add(view->navi, info, it);
				break;
			case TBT_APP_BLE_GATT_SERVER:
				bluetoothle_view_add(view->navi, info, it);
				break;
		#endif

		#ifdef TBT_MODULE_APP
			case TBT_APP_PICK_VIEW_APP:
				pick_view_add(view->navi, info, it);
				break;
			case TBT_APP_PICK_PICK_APP:
				pick_view_add(view->navi, info, it);
				break;
		#endif

		#ifdef TBT_MODULE_UI
			case TBT_APP_UI_RESIZE:
				ui_view_add(view->navi, info, it);
				break;
			case TBT_APP_UI_SHAPE:
				ui_view_add(view->navi, info, it);
				break;
			case TBT_APP_UI_ROTATE:
				ui_view_add(view->navi, info, it);
				break;
			case TBT_APP_UI_FONT:
				ui_view_add(view->navi, info, it);
				break;
			case TBT_APP_UI_LINE:
				ui_view_add(view->navi, info, it);
				break;
			case TBT_APP_UI_ALPHA:
				ui_view_add(view->navi, info, it);
				break;
		#endif

		#ifdef TBT_MODULE_EFL
			case TBT_APP_EFL_CALLBACK:
				efl_callbacks_view_add(view->navi, info, it);
				break;
		#endif
		#ifdef TBT_MODULE_SHORTCUT
			case TBT_APP_SHORTCUT:
				shortcut_view_add(view->navi, info, it);
				break;
		#endif

		default:
			assert(false);
			break;
	}
}


/**
 * @function		_gl_group_selected_cb
 * @since_tizen		2.3
 * @description		 Gl Group Selected Cb
 * @parameter		void*: Void Pointer, Evas_Object*: Evas Object Pointer, void*: Void Pointer
 * @return		static void
 */
static void _gl_group_selected_cb(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, void *event_info)
{
	Elm_Object_Item *it = event_info;
	item_data *id = elm_object_item_data_get(it);
	DBG("item_data getting ");
	RETM_IF(!id, "item is NULL");
	DBG("item_data found ");
	//Unhighlight item
	elm_genlist_item_selected_set(it, EINA_FALSE);

	Eina_Bool expanded = elm_genlist_item_expanded_get(it);
	DBG("Expanded getting%d: ",expanded);
	elm_genlist_item_expanded_set(it, !expanded);

	DBG("Expanded found%d: ",expanded);
}
