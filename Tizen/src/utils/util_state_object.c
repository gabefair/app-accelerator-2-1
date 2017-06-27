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
 *  @file util_state_object.c
 *
 *	@brief
 *	State machine
 *  Implementation of util_state_object
 */
#include "utils/util_state_object.h"


// define custom logging for state
#define __LOG(prio, fmt, arg...) dlog_print(prio, "UTIL_STATE", "%s (%d) > " fmt, __func__, __LINE__, ##arg)
#define __LOG_OUT(prio, fmt, arg...) dlog_print(prio, "UTIL_STATE", fmt, ##arg)
#define logd(fmt, arg...) __LOG(DLOG_DEBUG, fmt, ##arg)
#define logw(fmt, arg...) __LOG(DLOG_WARN, fmt, ##arg)
#define loge(fmt, arg...) __LOG(DLOG_ERROR, fmt, ##arg)
#define logi(fmt, arg...) __LOG(DLOG_INFO, fmt, ##arg)
#define log_output(fmt, arg...) __LOG_OUT(DLOG_INFO, fmt, ##arg)


// define structures
struct _util_state_transition
{
	int input;
	util_state_info *destination;
};


struct _util_state_info
{
	int state;
	GList *transitions;
	char *name;
};


struct _util_state_object
{
	bool initiated;
	char *name;
	GList *states;
	util_state_info *current;

	util_state_object_state_changed_cb state_changed_cb;
	void *user_data;
};



// Define internals
void util_state_object_destroy_transition(util_state_transition *object);
void util_state_object_destroy_state(util_state_info *object);




//


/**
 * @function		util_state_object_create
 * @since_tizen		2.3
 * @description		Util State Object Create
 * @parameter		const char*: Const char Pointer
 * @return		util_state_object*
 */
util_state_object* util_state_object_create(const char *name)
{
	util_state_object *object=malloc(sizeof(util_state_object));
	RETVM_IF(!object, NULL, "malloc failed");
	object->initiated=false;

	if(name!=NULL) object->name=strdup(name);
	else object->name=strdup("UN-NAMED");

	object->current=NULL;
	object->state_changed_cb=NULL;
	object->states=NULL;
	object->user_data=NULL;

	return object;
}


/**
 * @function		util_state_object_destroy_transition
 * @since_tizen		2.3
 * @description		Util State Object Destroy Transition
 * @parameter		util_state_transition*: Util State Transition Pointer
 * @return		void
 */
void util_state_object_destroy_transition(util_state_transition *object)
{
	free(object);
	return;
}


/**
 * @function		util_state_object_destroy_state
 * @since_tizen		2.3
 * @description		Util State Object Destroy State
 * @parameter		util_state_info*: Util State Info Pointer
 * @return		void
 */
void util_state_object_destroy_state(util_state_info *object)
{
	if(object==NULL) return;
	GList *it;
	for(it=g_list_first(object->transitions);it!=NULL;it=it->next)
	{
		logd("   state %s destroying transition %d", object->name, ((util_state_transition*) it->data)->input);
		util_state_object_destroy_transition((util_state_transition*) it->data);

	}
	g_list_free(object->transitions);
	if(object->name!=NULL) free(object->name);
	free(object);
	return;
}


/**
 * @function		util_state_object_destroy
 * @since_tizen		2.3
 * @description		Util State Object Destroy
 * @parameter		util_state_object*: Util State Object Pointer
 * @return		void
 */
void util_state_object_destroy(util_state_object *object)
{
	GList *it;
	for(it=g_list_first(object->states);it!=NULL;it=it->next)
	{
		util_state_object_destroy_state((util_state_info*) it->data);
	}

	if(object->name!=NULL)
		free(object->name);

	free(object);
	return;
}


/**
 * @function		util_state_object_get_state
 * @since_tizen		2.3
 * @description		Util State Object Get State
 * @parameter		util_state_object*: Util State Object Pointer, int: Int
 * @return		util_state_info*
 */
util_state_info* util_state_object_get_state(util_state_object *object,int state_value)
{
	if(object!=NULL)
	{
		GList *it;
		for(it=g_list_first(object->states);it!=NULL;it=it->next)
		{
			if(((util_state_info*) it->data)->state==state_value) return (util_state_info*) it->data;
		}

		logw("util_state_object_get_state :: state=%d not found!",state_value);
	}
	else loge("util_state_object_get_state :: util_state_object is NULL");
	return NULL;
}


/**
 * @function		util_state_object_add_state
 * @since_tizen		2.3
 * @description		Util State Object Add State
 * @parameter		util_state_object*: Util State Object Pointer, const char*: Const char Pointer, int: Int
 * @return		util_state_info*
 */
util_state_info* util_state_object_add_state(util_state_object *object,const char *name,int state_value)
{
	util_state_info* state=util_state_object_get_state(object,state_value);
	if(state==NULL)
	{
		logi("util_state_object_add_state :: state=%d,'%s' Adding State",state_value, name);
		state = malloc(sizeof(util_state_info));
		RETVM_IF(!state, NULL, "malloc failed");
		object->states = g_list_append(object->states,state);
	}
	else
	{
		logw("util_state_object_add_state :: state=%d,'%s' already defined. Over writing.",state->state, state->name);
		if(state->name!=NULL) free(state->name);
		g_list_free(state->transitions);
	}

	state->name=strdup(name);
	state->state=state_value;
	state->transitions=NULL;

	return NULL;
}


/**
 * @function		util_state_object_get_transition
 * @since_tizen		2.3
 * @description		Util State Object Get Transition
 * @parameter		util_state_info*: Util State Info Pointer, int: Int
 * @return		util_state_transition*
 */
util_state_transition* util_state_object_get_transition(util_state_info* state,int feed)
{
	if(state!=NULL)
	{
		GList *it;
		for(it=g_list_first(state->transitions);it!=NULL;it=it->next)
		{
			if(((util_state_transition*) it->data)->input==feed) return (util_state_transition*) it->data;
		}
	}
	return NULL;
}


/**
 * @function		util_state_object_add_transition
 * @since_tizen		2.3
 * @description		Util State Object Add Transition
 * @parameter		util_state_object*: Util State Object Pointer, int: Int, int: Int, int: Int
 * @return		util_state_transition*
 */
util_state_transition* util_state_object_add_transition(util_state_object *object,int from_state,int feed_value,int to_state)
{
	logi("adding transition...");
	if(object!=NULL)
	{
		util_state_info *from=util_state_object_get_state(object,from_state);
		util_state_info *to=util_state_object_get_state(object,to_state);

		if(from==NULL)
		{
			loge("util_state_object_add_transition :: state=%d (from) is not defined",from_state);
			return NULL;
		}

		if(to==NULL)
		{
			loge("util_state_object_add_transition :: state=%d (to) is not defined",to_state);
			return NULL;
		}

		util_state_transition* transition = util_state_object_get_transition(from,feed_value);
		if(transition==NULL)
		{
			transition=malloc(sizeof(util_state_transition));
			transition->destination=to;
			transition->input=feed_value;
			from->transitions = g_list_append(from->transitions,transition);

			//LOGI("util_state_object_add_transition :: Transition (%d , %d) => %d Added",from->state,feed_value,transition->destination->state);
		}
		else
		{
			logw("util_state_object_add_transition :: Transition Exists. (%d , %d) => %d Over writing transition",from->state,feed_value,transition->destination->state);
			transition->destination=to;
			transition->input=feed_value;
		}

		int count = g_list_length(from->transitions);
		logi("[%s] [%s] has %d transition.",object->name,from->name,count);
		return transition;

	}

	loge("util_state_object_add_transition :: state_object is null");

	return NULL;
}


/**
 * @function		util_state_object_init
 * @since_tizen		2.3
 * @description		Util State Object Init
 * @parameter		util_state_object*: Util State Object Pointer, int: Int, void*: Void Pointer
 * @return		bool
 */
bool util_state_object_init(util_state_object *state_object,int initial_state, util_state_object_state_changed_cb state_change_cb,void *user_data)
{
	if(state_object!=NULL && state_object->initiated==false)
	{
		state_object->current=util_state_object_get_state(state_object,initial_state);
		state_object->user_data=user_data;

		if(state_object->current!=NULL)
		{
			state_object->initiated=true;
			state_object->state_changed_cb=state_change_cb;

			logi("(%s,%s) state_object INIT",state_object->name, state_object->current->name);
			return true;
		}
		else loge("util_state_object_init :: invalid state=%d",initial_state);

	}

	return false;
}


/**
 * @function		util_state_object_feed
 * @since_tizen		2.3
 * @description		Util State Object Feed
 * @parameter		util_state_object*: Util State Object Pointer, void*: Void Pointer
 * @return		void
 */
void util_state_object_feed(util_state_object *state_object, int feed_value,void *feed_data)
{
	if(state_object==NULL || state_object->initiated==false || state_object->current==NULL) return;
	logi("(%s,%s) feeding %d",state_object->name, state_object->current->name, feed_value);

	//util_state_transition *it=NULL;
	GList *it=NULL;
	util_state_info *prev=state_object->current;
	for(it=g_list_first(state_object->current->transitions);it!=NULL;it=it->next)
	{
		util_state_transition *transition=it->data;
		if( transition->input == feed_value)
		{
			state_object->current=transition->destination;
			break;
		}
	}

	if(state_object->state_changed_cb!=NULL)
		(*state_object->state_changed_cb)(prev->state,state_object->current->state,feed_value,state_object->user_data,feed_data);

	logi("(%s,%s) Latest",state_object->name, state_object->current->name);

}


/**
 * @function		util_state_object_info
 * @since_tizen		2.3
 * @description		Util State Object Info
 * @parameter		util_state_object*: Util State Object Pointer
 * @return		void
 */
void util_state_object_info(util_state_object *state_object)
{
	if(state_object==NULL) return;

	log_output("===================");
	log_output("--STATE-MACHINE(%s)",state_object->name);
	GList *it=NULL,*it2=NULL;
	int count;
	for(it=g_list_first(state_object->states);it!=NULL;it=it->next)
	{
		util_state_info *state=(util_state_info*)it->data;
		count=g_list_length(state->transitions);
		log_output("--|--%s[%d] (%d)",state->name,state->state,count);

		for(it2=g_list_first(state->transitions);it2!=NULL;it2=it2->next)
		{
			util_state_transition *transition=(util_state_transition*)it2->data;
			log_output("--|--|--%d => %s[%d]",transition->input,transition->destination->name,transition->destination->state);
		}
		log_output("--|--|");

	}
	log_output("--|");

	log_output("===================");

}


/**
 * @function		on_state_changed
 * @since_tizen		2.3
 * @description		On State Changed
 * @parameter		int: Int, int: Int, int: Int, void*: Void Pointer, void*: Void Pointer
 * @return		static void
 */
static void on_state_changed(int previous,int now,int feed,void *user_data, void *feed_data)
{
	logi("__TEST__ on_state_changed: now=%d, from=%d, feed=%d",now,previous,feed);
}


/**
 * @function		util_state_test1
 * @since_tizen		2.3
 * @description		Util State Test1
 * @parameter		NA
 * @return		void
 */
void util_state_test1()
{
	util_state_object *state_object=util_state_object_create("test1");

	util_state_object_add_state(state_object,"state1",0);
	util_state_object_add_state(state_object,"state2",1);
	util_state_object_add_state(state_object,"state3",2);

	util_state_object_add_transition(state_object,0,0,0);
	util_state_object_add_transition(state_object,0,1,1);
	util_state_object_add_transition(state_object,0,2,2);

	util_state_object_add_transition(state_object,1,0,0);
	util_state_object_add_transition(state_object,1,1,1);
	util_state_object_add_transition(state_object,1,2,2);

	util_state_object_add_transition(state_object,2,0,0);
	util_state_object_add_transition(state_object,2,1,1);
	util_state_object_add_transition(state_object,2,2,2);
	util_state_object_add_transition(state_object,2,3,1);

	util_state_object_info(state_object);

	util_state_object_init(state_object, 0,on_state_changed,NULL);
	util_state_object_feed(state_object, 1, NULL);
	util_state_object_feed(state_object, 2, NULL);
	util_state_object_feed(state_object, 3, NULL);
	util_state_object_feed(state_object, 0, NULL);
	util_state_object_feed(state_object, 0, NULL);
	util_state_object_feed(state_object, 0, NULL);
	util_state_object_feed(state_object, 0, NULL);

	util_state_object_destroy(state_object);
}


/**
 * @function		util_state_object_current_state
 * @since_tizen		2.3
 * @description		Util State Object Current State
 * @parameter		util_state_object*: Util State Object Pointer
 * @return		int
 */
int util_state_object_current_state(util_state_object *state_object)
{
	if(state_object==NULL || state_object->current==NULL || state_object->initiated==false)
		return -1;

	return state_object->current->state;
}


/**
 * @function		util_state_object_current_state_name
 * @since_tizen		2.3
 * @description		Util State Object Current State Name
 * @parameter		util_state_object*: Util State Object Pointer
 * @return		const char*
 */
const char* util_state_object_current_state_name(util_state_object *state_object)
{
	if(state_object==NULL || state_object->current==NULL || state_object->initiated==false)
		return NULL;

	return state_object->current->name;
}
