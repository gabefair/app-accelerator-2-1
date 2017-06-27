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
 * @file	gl-types.c
 * @brief	This file provide necessary functions for OpenGl ES 2.0 transformation.
 *
 * @bug
 * @credit
 */

#include <math.h>

#include "utils/gl-types.h"
#include "utils/logger.h"

#define GL_PI 3.1415926535897932384626433832795f
#define GL_DEG_TO_RAD (GL_PI / 180.0f)
#define GL_ZERO_EPS 1.0e-9f
#define GL_MAX_FOV_DEG (90.0f - GL_ZERO_EPS)


/**
 * @function		gl_matrix4_load_identity
 * @since_tizen		2.3
 * @description		Gl Matrix4 Load Identity
 * @parameter		gl_matrix4*: Gl Matrix4 Pointer
 * @return		void
 */
void gl_matrix4_load_identity(gl_matrix4 *result)
{
    RETM_IF(!result, "NULL arguments");

    memset(&result->m, 0, sizeof(result->m));
    int i;
    for (i = 0; i < 4; ++i)
    {
        result->m[i][i] = 1.0f;
    }
}


/**
 * @function		gl_matrix4_load_frustum
 * @since_tizen		2.3
 * @description		Gl Matrix4 Load Frustum
 * @parameter		gl_matrix4*: Gl Matrix4 Pointer
 * @return		void
 */
void gl_matrix4_load_frustum(gl_matrix4 *result, GLfloat left, GLfloat right,         GLfloat bottom, GLfloat top, GLfloat near, GLfloat far)
{
    RETM_IF(!result, "NULL arguments");

    GLfloat diffX = right - left;
    GLfloat diffY = top - bottom;
    GLfloat diffZ = far - near;

    RETM_IF((fabsf(diffX) <= GL_ZERO_EPS) || (fabsf(diffY) <= GL_ZERO_EPS) ||
            (diffZ <= GL_ZERO_EPS) || (near <= GL_ZERO_EPS),
            "Invalid parameters");

    result->m[0][0] = 2.0f * near / diffX;
    result->m[1][1] = 2.0f * near / diffY;
    result->m[2][0] = (right + left) / diffX;
    result->m[2][1] = (top + bottom) / diffY;
    result->m[2][2] = -(far + near) / diffZ;
    result->m[2][3] = -1.0f;
    result->m[3][2] = -2.0f * far * near / diffZ;

    result->m[0][1] = result->m[0][2] = result->m[0][3] = 0.0f;
    result->m[1][0] = result->m[1][2] = result->m[1][3] = 0.0f;
    result->m[3][0] = result->m[3][1] = result->m[3][3] = 0.0f;
}


/**
 * @function		gl_matrix4_load_perspective
 * @since_tizen		2.3
 * @description		Gl Matrix4 Load Perspective
 * @parameter		gl_matrix4*: Gl Matrix4 Pointer
 * @return		void
 */
void gl_matrix4_load_perspective(gl_matrix4 *result, GLfloat fov_deg,         GLfloat aspect, GLfloat near, GLfloat far)
{
    RETM_IF(!result, "NULL arguments");
    RETM_IF(fabsf(fov_deg) >= GL_MAX_FOV_DEG, "Invalid parameters");

    GLfloat dist = tanf(fov_deg * GL_DEG_TO_RAD * 0.5f) * near;
    GLfloat top = (aspect < 1.0f) ? (dist / aspect) : dist;
    GLfloat right = (aspect < 1.0f) ? dist : (dist * aspect);

    gl_matrix4_load_frustum(result, -right, right, -top, top, near, far);
}


/**
 * @function		gl_matrix4_translate
 * @since_tizen		2.3
 * @description		Gl Matrix4 Translate
 * @parameter		gl_matrix4*: Gl Matrix4 Pointer
 * @return		void
 */
void gl_matrix4_translate(gl_matrix4 *result, gl_vector3 v)
{
    RETM_IF(!result, "NULL arguments");

    result->m[3][0] += (result->m[0][0] * v.x + result->m[1][0] * v.y + result->m[2][0] * v.z);
    result->m[3][1] += (result->m[0][1] * v.x + result->m[1][1] * v.y + result->m[2][1] * v.z);
    result->m[3][2] += (result->m[0][2] * v.x + result->m[1][2] * v.y + result->m[2][2] * v.z);
    result->m[3][3] += (result->m[0][3] * v.x + result->m[1][3] * v.y + result->m[2][3] * v.z);
}


/**
 * @function		gl_matrix4_rotate
 * @since_tizen		2.3
 * @description		Gl Matrix4 Rotate
 * @parameter		gl_matrix4*: Gl Matrix4 Pointer
 * @return		void
 */
void gl_matrix4_rotate(gl_matrix4 *result, GLfloat angle_deg, gl_vector3 v)
{
    RETM_IF(!result, "NULL arguments");

    GLfloat m = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    RETM_IF(m <= GL_ZERO_EPS, "Invalid parameters");

    gl_vector3 nv;
    nv.x = v.x / m;
    nv.y = v.y / m;
    nv.z = v.z / m;

    GLfloat angle_rad = angle_deg * GL_DEG_TO_RAD;
    GLfloat cosa = cosf(angle_rad);
    GLfloat sina = sinf(angle_rad);
    GLfloat cosa1 = 1.0f - cosa;

    gl_matrix4 rotate;

    rotate.m[0][0] = (nv.x * nv.x) * cosa1 + cosa;
    rotate.m[0][1] = (nv.x * nv.y) * cosa1 + nv.z * sina;
    rotate.m[0][2] = (nv.z * nv.x) * cosa1 - nv.y * sina;
    rotate.m[0][3] = 0.0f;

    rotate.m[1][0] = (nv.x * nv.y) * cosa1 - nv.z * sina;
    rotate.m[1][1] = (nv.y * nv.y) * cosa1 + cosa;
    rotate.m[1][2] = (nv.y * nv.z) * cosa1 + nv.x * sina;
    rotate.m[1][3] = 0.0f;

    rotate.m[2][0] = (nv.z * nv.x) * cosa1 + nv.y * sina;
    rotate.m[2][1] = (nv.y * nv.z) * cosa1 - nv.x * sina;
    rotate.m[2][2] = (nv.z * nv.z) * cosa1 + cosa;

    rotate.m[2][3] = rotate.m[3][0] = rotate.m[3][1] = rotate.m[3][2] = 0.0f;
    rotate.m[3][3] = 1.0f;

    gl_matrix4_mul(result, &rotate);
}


/**
 * @function		gl_matrix4_mul
 * @since_tizen		2.3
 * @description		Gl Matrix4 Mul
 * @parameter		gl_matrix4*: Gl Matrix4 Pointer, const gl_matrix4*: Const gl Matrix4 Pointer
 * @return		void
 */
void gl_matrix4_mul(gl_matrix4 *result, const gl_matrix4 *rv)
{
    RETM_IF(!result || !rv, "NULL arguments");

    gl_matrix4 tmp;

    tmp.m[0][0] = result->m[0][0] * rv->m[0][0] + result->m[1][0] * rv->m[0][1] +
                  result->m[2][0] * rv->m[0][2] + result->m[3][0] * rv->m[0][3];
    tmp.m[1][0] = result->m[0][0] * rv->m[1][0] + result->m[1][0] * rv->m[1][1] +
                  result->m[2][0] * rv->m[1][2] + result->m[3][0] * rv->m[1][3];
    tmp.m[2][0] = result->m[0][0] * rv->m[2][0] + result->m[1][0] * rv->m[2][1] +
                  result->m[2][0] * rv->m[2][2] + result->m[3][0] * rv->m[2][3];
    tmp.m[3][0] = result->m[0][0] * rv->m[3][0] + result->m[1][0] * rv->m[3][1] +
                  result->m[2][0] * rv->m[3][2] + result->m[3][0] * rv->m[3][3];
    tmp.m[0][1] = result->m[0][1] * rv->m[0][0] + result->m[1][1] * rv->m[0][1] +
                  result->m[2][1] * rv->m[0][2] + result->m[3][1] * rv->m[0][3];
    tmp.m[1][1] = result->m[0][1] * rv->m[1][0] + result->m[1][1] * rv->m[1][1] +
                  result->m[2][1] * rv->m[1][2] + result->m[3][1] * rv->m[1][3];
    tmp.m[2][1] = result->m[0][1] * rv->m[2][0] + result->m[1][1] * rv->m[2][1] +
                  result->m[2][1] * rv->m[2][2] + result->m[3][1] * rv->m[2][3];
    tmp.m[3][1] = result->m[0][1] * rv->m[3][0] + result->m[1][1] * rv->m[3][1] +
                  result->m[2][1] * rv->m[3][2] + result->m[3][1] * rv->m[3][3];
    tmp.m[0][2] = result->m[0][2] * rv->m[0][0] + result->m[1][2] * rv->m[0][1] +
                  result->m[2][2] * rv->m[0][2] + result->m[3][2] * rv->m[0][3];
    tmp.m[1][2] = result->m[0][2] * rv->m[1][0] + result->m[1][2] * rv->m[1][1] +
                  result->m[2][2] * rv->m[1][2] + result->m[3][2] * rv->m[1][3];
    tmp.m[2][2] = result->m[0][2] * rv->m[2][0] + result->m[1][2] * rv->m[2][1] +
                  result->m[2][2] * rv->m[2][2] + result->m[3][2] * rv->m[2][3];
    tmp.m[3][2] = result->m[0][2] * rv->m[3][0] + result->m[1][2] * rv->m[3][1] +
                  result->m[2][2] * rv->m[3][2] + result->m[3][2] * rv->m[3][3];
    tmp.m[0][3] = result->m[0][3] * rv->m[0][0] + result->m[1][3] * rv->m[0][1] +
                  result->m[2][3] * rv->m[0][2] + result->m[3][3] * rv->m[0][3];
    tmp.m[1][3] = result->m[0][3] * rv->m[1][0] + result->m[1][3] * rv->m[1][1] +
                  result->m[2][3] * rv->m[1][2] + result->m[3][3] * rv->m[1][3];
    tmp.m[2][3] = result->m[0][3] * rv->m[2][0] + result->m[1][3] * rv->m[2][1] +
                  result->m[2][3] * rv->m[2][2] + result->m[3][3] * rv->m[2][3];
    tmp.m[3][3] = result->m[0][3] * rv->m[3][0] + result->m[1][3] * rv->m[3][1] +
                  result->m[2][3] * rv->m[3][2] + result->m[3][3] * rv->m[3][3];

    *result = tmp;
}
