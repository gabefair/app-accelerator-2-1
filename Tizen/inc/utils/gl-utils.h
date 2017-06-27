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
 * @file	gl-utils.h
 * @brief	This file provide necessary Util functions for OpenGL operation.
 * @since_tizen 2.3
 * @bug
 * @credit
 */
#ifndef __GL_UTILS_H__
#define __GL_UTILS_H__

#include "gl-types.h"

/**
 * @brief Common shader vertex attribute constants
 * @since_tizen 2.3
 */
enum
{
    SHADER_COORDS_ATTR = 0,
    SHADER_COLOR_ATTR = 1
};

/**
 * @brief Color shader program structure
 * @since_tizen 2.3
 * Object MUST be filled with 0 before using in any of the API functions.
 */
typedef struct
{
    GLuint program;
    GLuint vertex_shader;
    GLuint fragment_Shader;
    GLint u_m4P;
    GLint u_m4MV;

} color_shader;

/**
 * @brief 3D color vertex data
 * @since_tizen 2.3
 */
typedef struct
{
    gl_vector3 coords;
    gl_vector4 color;

} color_vertex3d;

/**
 * @brief 3D color object vertex data
 * @since_tizen 2.3
 */
typedef struct
{
    const color_vertex3d *vertexes;
    const GLshort *indexes;
    GLuint indexes_length;

} color_object3d;

/**
 * @brief Initialises color shader object.
 * @since_tizen 2.3
 * On failed initialisation object may be partially initialised so MUST be finalised.
 * @param[in] obj   Pointer to color shader object.
 * @param[in] api   Pointer to Evas_GL_API structure.
 * @return GL_TRUE on success, GL_FALSE otherwise.
 */
GLboolean color_shader_init(color_shader *obj, Evas_GL_API *api);

/**
 * @brief Frees resources associated with shader object.
 * @since_tizen 2.3
 * May be called without initialisation.
 * May be reinitialised again after the call.
 * @param[in] obj   Pointer to color shader object.
 * @param[in] api   Pointer to Evas_GL_API structure.
 */
void color_shader_finalize(color_shader *obj, Evas_GL_API *api);

/**
 * @brief Activates shader so it will be used in rendering
 * @since_tizen 2.3
 * @param[in] obj   Pointer to color shader object.
 * @param[in] api   Pointer to Evas_GL_API structure.
 */
void color_shader_activate(color_shader *obj, Evas_GL_API *api);

/**
 * @brief Deactivated shader so it will NOT be used in rendering
 * @since_tizen 2.3
 * @param[in] obj   Pointer to color shader object.
 * @param[in] api   Pointer to Evas_GL_API structure.
 */
void color_shader_deactivate(color_shader *obj, Evas_GL_API *api);

/**
 * @brief Loads Projection matrix to use in rendering. Shader MUST be active.
 * @since_tizen 2.3
 * @param[in] obj       Pointer to color shader object.
 * @param[in] api       Pointer to Evas_GL_API structure.
 * @param[in] matrix    Pointer to Projection matrix.
 */
void color_shader_load_p(color_shader *obj, Evas_GL_API *api, const gl_matrix4 *matrix);

/**
 * @brief Loads Model-View matrix to use in rendering. Shader MUST be active.
 * @since_tizen 2.3
 * @param[in] obj       Pointer to color shader object.
 * @param[in] api       Pointer to Evas_GL_API structure.
 * @param[in] matrix    Pointer to Model-View matrix.
 */
void color_shader_load_mv(color_shader *obj, Evas_GL_API *api, const gl_matrix4 *matrix);

/**
 * @brief Draws color 3D object. Appropriate shader MUST be active.
 * @since_tizen 2.3
 * @param[in] obj   Pointer to color 3D object structure.
 * @param[in] api   Pointer to Evas_GL_API structure.
 */
void color_object3d_draw(const color_object3d *obj, Evas_GL_API *api);

#endif /* __GL_UTILS_H__ */
