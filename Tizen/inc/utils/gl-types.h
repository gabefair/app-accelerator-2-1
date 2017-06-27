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
 * @file	gl-types.h
 * @brief	This file provide necessary functions for OpenGl ES 2.0 transformation.
 * @since_tizen 2.3
 * @bug
 * @credit
 */
#ifndef __GL_TYPES_H__
#define __GL_TYPES_H__

#include <Elementary.h>


typedef struct
{
	GLfloat x, y;

} gl_vector2;

/**
 * @brief 3D vector to represent position and color information in Open GL.
 * @since_tizen 2.3
 */
typedef union
{
    struct
    {
        GLfloat x, y, z;
    };
    struct
    {
        GLfloat r, g, b;
    };
} gl_vector3;

/**
 * @brief 4D vector to represent position and color information in Open GL.
 * @since_tizen 2.3
 */
typedef union
{
    struct
    {
        GLfloat x, y, z, w;
    };
    struct
    {
        GLfloat r, g, b, a;
    };
} gl_vector4;

/**
 * @brief 4x4 matrix to perform transformations in 3D space.
 * @since_tizen 2.3
 */
typedef struct
{
    GLfloat m[4][4];

} gl_matrix4;

/**
 * @brief Loads identity matrix values to the passed matrix.
 * @since_tizen 2.3
 * @param[in] result    Pointer to the result matrix.
 */
void gl_matrix4_load_identity(gl_matrix4 *result);

/**
 * @brief Loads frustum projection matrix values to the passed matrix.
 * @since_tizen 2.3
 * @param[in] result    Pointer to the result matrix.
 * @param[in] left      The coordinate for the left-vertical clipping plane.
 * @param[in] right     The coordinate for the right-vertical clipping plane.
 * @param[in] bottom    The coordinate for the bottom-horizontal clipping plane.
 * @param[in] top       The coordinate for the top-horizontal clipping plane.
 * @param[in] near      The distances to the near-depth clipping plane. Must be positive.
 * @param[in] far       The distances to the far-depth clipping planes. Must be positive.
 */
void gl_matrix4_load_frustum(gl_matrix4 *result, GLfloat left, GLfloat right,
        GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);

/**
 * @brief Loads perspective projection matrix values to the passed matrix.
 * @since_tizen 2.3
 * @param[in] result    Pointer to the result matrix.
 * @param[in] fov_deg   Field of View angle in degree for minimal dimension (vertical or horizontal).
 * @param[in] aspect    View-port width to height ratio.
 * @param[in] near      The distances to the near-depth clipping plane. Must be positive.
 * @param[in] far       The distances to the far-depth clipping planes. Must be positive.
 */
void gl_matrix4_load_perspective(gl_matrix4 *result, GLfloat fov_deg,
        GLfloat aspect, GLfloat near, GLfloat far);

/**
 * @brief Multiplies the passed matrix by a translation matrix.
 * @since_tizen 2.3
 * @param[in] result    Pointer to the result matrix.
 * @param[in] v         Translation vector.
 */
void gl_matrix4_translate(gl_matrix4 *result, gl_vector3 v);

/**
 * @brief Multiplies the passed matrix by a rotation matrix.
 * @since_tizen 2.3
 * @param[in] result    Pointer to the result matrix.
 * @param[in] angle_deg The angle of rotation, in degrees.
 * @param[in] v         Rotation vector.
 */
void gl_matrix4_rotate(gl_matrix4 *result, GLfloat angle_deg, gl_vector3 v);

/**
 * @brief Multiplies the passed matrix by the custom matrix.
 * @since_tizen 2.3
 * @param[in] result    Pointer to the result matrix.
 * @param[in] rv        Custom matrix of the multiplication.
 */
void gl_matrix4_mul(gl_matrix4 *result, const gl_matrix4 *rv);

#endif /* __GL_TYPES_H__ */
