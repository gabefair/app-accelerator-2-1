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
 * @file	color-utils.h
 * @brief	This file provide different color information.
 * @since_tizen 2.3
 * @bug
 * @credit
 */
#ifndef __COLOR_UTILS_H__
#define __COLOR_UTILS_H__

/**
 * @brief Retrive color component from color mask
 * @since_tizen 2.3
 * @param[in]   color   Integer color representation (i.e. 0xRRGGBB)
 * @return Color component
 */
#define RED(color)      ((color >> 16) & 0xff)
#define GREEN(color)    ((color >> 8) & 0xff)
#define BLUE(color)     ((color) & 0xff)

/**
 * @brief Expand color integer representation as comma separated color
 *        components values
 * @since_tizen 2.3
 */
#define RGB(color)    RED(color), GREEN(color), BLUE(color)

/**
 * @brief Expand color integer representation as comma separated color
 *        components float values (from 0.0 to 1.0)
 * @since_tizen 2.3
 */
#define FRGB(color)   RED(color) / 255.0, GREEN(color) / 255.0, BLUE(color) / 255.0

#endif /* __COLOR_UTILS_H__ */
