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
 * @file	xml-util.h
 * @brief	This file provide utility funtction's for writing XML Report.
 * @since_tizen 2.3
 * @bug
 * @credit
 */

#ifndef XML_UTIL_H_
#define XML_UTIL_H_


#include "model/tbt-info.h"
#include "utils/logger.h"

// For xml read/write
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlreader.h>


/**
 * Helper function. implementing traditional itoa
 * @since_tizen 2.3
 * @param value
 * @return
 */
char* xmlUtil_itoa(int value);


/**
 * Search tbt_info with specified apptype
 * @since_tizen 2.3
 * @param infos
 * @param len
 * @param apptype
 * @return
 */
tbt_info *xmlUtil_get_tbtinfo_by_apptype(const tbt_info *infos,const int len,const int apptype);


/**
 * Write Xml report and save test status in preference
 * @since_tizen 2.3
 * @param inputFilePath
 * @param moduleList
 * @param testDataCount
 */
void xmlUtil_writeResults(const char *inputFilePath,tbt_info *moduleList,const int testDataCount);


/**
 * Read test status from preference.
 * @since_tizen 2.3
 * @param moduleList	tbt_info list
 * @param len	Length of tbt_info list.
 * @return	Whether its is succeded or not.
 */
bool xmlUtil_readResults(tbt_info *moduleList,const int len);



#endif /* XML_UTIL_H_ */
