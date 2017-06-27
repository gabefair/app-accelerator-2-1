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
 * @file util_state_object.h
 * @since_tizen 2.3
 * @brief
 * State machine
 *
 * @debugtag UTIL_STATE
 *
 * State machine
 * @example

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

 *
 *	@todo
 *	multiple input state machine. Decision based on multiple feed_value
 */

#ifndef _UTIL_STATE_OBJECT_H_
#define _UTIL_STATE_OBJECT_H_


#include <stdbool.h>
#include <tizen.h>
#include "logger.h"
#include <stdlib.h>

#include <glib.h>


typedef struct _util_state_transition util_state_transition;
typedef struct _util_state_info util_state_info;
typedef struct _util_state_object util_state_object;


/**
 * Callback for state object
 * @since_tizen 2.3
 */
typedef void (*util_state_object_state_changed_cb) (int previous,int now,int feed,void *user_data,void *feed_data);


/**
 *	Create state object
 *  @since_tizen 2.3
 */
util_state_object* util_state_object_create(const char *name);


/**
 * Destroy state object. Cleans all references and memory
 * @since_tizen 2.3
 */
void util_state_object_destroy(util_state_object *object);


/**
 * Get state info object from state object.
 * @since_tizen 2.3
 */
util_state_info* util_state_object_get_state(util_state_object *object,int state_value);


/**
 * Add new state to state object.
 * @since_tizen 2.3
 */
util_state_info* util_state_object_add_state(util_state_object *object,const char *name,int state_value);


/**
 * Get transition object from state if has mapping for feed
 */
util_state_transition* util_state_object_get_transition(util_state_info* state,int feed);


/**
 * Add new transition to state object.
 */
util_state_transition* util_state_object_add_transition(util_state_object *object,int from_state,int feed_value,int to_state);


/**
 * Initiates the state object with initial state
 */
bool util_state_object_init(util_state_object *state_object,int initial_state, util_state_object_state_changed_cb state_change_cb,void *user_data);

/**
 * Feed new value to state object
 */
void util_state_object_feed(util_state_object *state_object, int feed_value,void *feed_data);


/**
 * 	Dump information of current stage object mapping. (tag:UTIL_STATE)
 */
void util_state_object_info(util_state_object *state_object);


/**
 * Get current state of state object
 */
int util_state_object_current_state(util_state_object *state_object);


/**
 * Get the name of current state.
 */
const char* util_state_object_current_state_name(util_state_object *state_object);


/**
 * Test for this module
 */
void util_state_test1();


#endif // _COMMONUTIL_H_

