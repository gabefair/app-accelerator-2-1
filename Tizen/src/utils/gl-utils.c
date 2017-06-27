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
 * @file	gl-utils.c
 * @brief	This file provide necessary Util functions for OpenGL operation.
 *
 * @bug
 * @credit
 */


#include "utils/gl-utils.h"
#include "utils/logger.h"

static const char *VERTEX_SHADER_TEXT =
    "uniform mat4 u_m4P;\n"
    "uniform mat4 u_m4MV;\n"
    "attribute vec4 a_v4VertexCoords;\n"
    "attribute vec4 a_v4VertexColor;\n"
    "varying vec4 v_v4VertexColor;\n"
    "void main()\n"
    "{\n"
    "   v_v4VertexColor = a_v4VertexColor;\n"
    "   gl_Position = u_m4P * u_m4MV * a_v4VertexCoords;\n"
    "}\n";

static const char *FRAGMENT_SHADER_TEXT =
    "precision mediump float;\n"
    "varying vec4 v_v4VertexColor;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = v_v4VertexColor;\n"
    "}\n";


/**
 * @function		color_shader_init
 * @since_tizen		2.3
 * @description		Color Shader Init
 * @parameter		color_shader*: Color Shader Pointer, Evas_GL_API*: Evas GL API Pointer
 * @return		GLboolean
 */
GLboolean color_shader_init(color_shader *obj, Evas_GL_API *api)
{
    RETVM_IF(!obj || !api, GL_FALSE, "NULL arguments");

    obj->vertex_shader = api->glCreateShader(GL_VERTEX_SHADER);
    api->glShaderSource(obj->vertex_shader, 1, &VERTEX_SHADER_TEXT, NULL);
    api->glCompileShader(obj->vertex_shader);

    obj->fragment_Shader = api->glCreateShader(GL_FRAGMENT_SHADER);
    api->glShaderSource(obj->fragment_Shader, 1, &FRAGMENT_SHADER_TEXT, NULL);
    api->glCompileShader(obj->fragment_Shader);

    obj->program = api->glCreateProgram();
    api->glAttachShader(obj->program, obj->vertex_shader);
    api->glAttachShader(obj->program, obj->fragment_Shader);

    api->glBindAttribLocation(obj->program, SHADER_COORDS_ATTR, "a_v4VertexCoords");
    api->glBindAttribLocation(obj->program, SHADER_COLOR_ATTR, "a_v4VertexColor");

    api->glLinkProgram(obj->program);

    obj->u_m4P = api->glGetUniformLocation(obj->program, "u_m4P");
    obj->u_m4MV = api->glGetUniformLocation(obj->program, "u_m4MV");

    return ((obj->u_m4P != -1) && (obj->u_m4MV != -1));
}


/**
 * @function		color_shader_finalize
 * @since_tizen		2.3
 * @description		Color Shader Finalize
 * @parameter		color_shader*: Color Shader Pointer, Evas_GL_API*: Evas GL API Pointer
 * @return		void
 */
void color_shader_finalize(color_shader *obj, Evas_GL_API *api)
{
    RETM_IF(!obj, "NULL arguments");

    if (api)
    {
        if (obj->program != 0)
        {
            api->glDeleteProgram(obj->program);
        }
        if (obj->vertex_shader != 0)
        {
            api->glDeleteShader(obj->vertex_shader);
        }
        if (obj->fragment_Shader != 0)
        {
            api->glDeleteShader(obj->fragment_Shader);
        }
    }
    memset(obj, 0, sizeof(color_shader));
}


/**
 * @function		color_shader_activate
 * @since_tizen		2.3
 * @description		Color Shader Activate
 * @parameter		color_shader*: Color Shader Pointer, Evas_GL_API*: Evas GL API Pointer
 * @return		void
 */
void color_shader_activate(color_shader *obj, Evas_GL_API *api)
{
    RETM_IF(!obj || !api, "NULL arguments");

    api->glUseProgram(obj->program);
    api->glEnableVertexAttribArray(SHADER_COORDS_ATTR);
    api->glEnableVertexAttribArray(SHADER_COLOR_ATTR);
}


/**
 * @function		color_shader_deactivate
 * @since_tizen		2.3
 * @description		Color Shader Deactivate
 * @parameter		color_shader*: Color Shader Pointer, Evas_GL_API*: Evas GL API Pointer
 * @return		void
 */
void color_shader_deactivate(color_shader *obj, Evas_GL_API *api)
{
    RETM_IF(!obj || !api, "NULL arguments");

    api->glDisableVertexAttribArray(SHADER_COORDS_ATTR);
    api->glDisableVertexAttribArray(SHADER_COLOR_ATTR);
    api->glUseProgram(0);
}


/**
 * @function		color_shader_load_p
 * @since_tizen		2.3
 * @description		Color Shader Load P
 * @parameter		color_shader*: Color Shader Pointer, Evas_GL_API*: Evas GL API Pointer, const gl_matrix4*: Const gl Matrix4 Pointer
 * @return		void
 */
void color_shader_load_p(color_shader *obj, Evas_GL_API *api, const gl_matrix4 *matrix)
{
    RETM_IF(!obj || !api, "NULL arguments");

    api->glUniformMatrix4fv(obj->u_m4P, 1, GL_FALSE, &(matrix->m[0][0]));
}


/**
 * @function		color_shader_load_mv
 * @since_tizen		2.3
 * @description		Color Shader Load Mv
 * @parameter		color_shader*: Color Shader Pointer, Evas_GL_API*: Evas GL API Pointer, const gl_matrix4*: Const gl Matrix4 Pointer
 * @return		void
 */
void color_shader_load_mv(color_shader *obj, Evas_GL_API *api, const gl_matrix4 *matrix)
{
    RETM_IF(!obj || !api, "NULL arguments");

    api->glUniformMatrix4fv(obj->u_m4MV, 1, GL_FALSE, &(matrix->m[0][0]));
}


/**
 * @function		color_object3d_draw
 * @since_tizen		2.3
 * @description		Color Object3d Draw
 * @parameter		const color_object3d*: Const color Object3d Pointer, Evas_GL_API*: Evas GL API Pointer
 * @return		void
 */
void color_object3d_draw(const color_object3d *obj, Evas_GL_API *api)
{
    RETM_IF(!obj || !api, "NULL arguments");

    api->glVertexAttribPointer(SHADER_COORDS_ATTR, 3, GL_FLOAT, GL_FALSE,
            sizeof(obj->vertexes[0]), &obj->vertexes->coords);
    api->glVertexAttribPointer(SHADER_COLOR_ATTR, 4, GL_FLOAT, GL_FALSE,
            sizeof(obj->vertexes[0]), &obj->vertexes->color);
    api->glDrawElements(GL_TRIANGLES, obj->indexes_length, GL_UNSIGNED_SHORT, obj->indexes);

    api->glFlush();
}
