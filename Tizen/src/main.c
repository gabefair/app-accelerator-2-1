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


#include "main-app.h"


/**
 * @function		main
 * @since_tizen		2.3
 * @description		Main
 * @parameter		int: Int
 * @return		int
 */
int main(int argc, char **argv)
{
	int result = 0;
	app_data *app = app_create();

	if(app)
	{
		result = app_run(app, argc, argv);
		app_destroy(app);
	}

	return result;
}
