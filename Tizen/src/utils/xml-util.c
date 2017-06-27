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
 * @file	xml-util.c
 * @brief	This file provide Util Funtction for writing XML Report.
 *
 * @bug
 * @credit
 */


#include "utils/xml-util.h"
#include <app_preference.h>
#include <system_settings.h>
#include <system_info.h>


static char chItoa_temp[15];


/**
 * @function		xmlUtil_itoa
 * @since_tizen		2.3
 * @description		XmlUtil Itoa
 * @parameter		int: Int
 * @return		char*
 */
char* xmlUtil_itoa(int value)
{
    snprintf(chItoa_temp, sizeof(chItoa_temp), "%d", value);
    return chItoa_temp;
}


/**
 * @function		xmlUtil_get_tbtinfo_by_apptype
 * @since_tizen		2.3
 * @description		XmlUtil Get Tbtinfo By Apptype
 * @parameter		const tbt_info*: Const tbt Info Pointer
 * @return		tbt_info*
 */
tbt_info* xmlUtil_get_tbtinfo_by_apptype(const tbt_info *infos,const int len,const int apptype)
{
	int i;
	for(i=0;i<len;++i) if(infos[i].apptype==apptype) return (tbt_info*) &infos[i];

	return NULL;
}


/**
 * @function		xmlUtil_writeResults
 * @since_tizen		2.3
 * @description		XmlUtil WriteResults
 * @parameter		const char*: Const char Pointer, tbt_info*: Tbt Info Pointer
 * @return		void
 */
void xmlUtil_writeResults(const char *inputFilePath,tbt_info *moduleList,const int testDataCount)
{

	DBG("1");

	int i;
	tbt_info *test;

	// lets get device info
	char *device_name=NULL;
	char *device_country=NULL;
	char *device_language=NULL;
	char *device_platfor_name=NULL;
	char *device_id=NULL;

	system_settings_get_value_string (SYSTEM_SETTINGS_KEY_DEVICE_NAME , &device_name);
	system_settings_get_value_string (SYSTEM_SETTINGS_KEY_LOCALE_COUNTRY  , &device_country);
	system_settings_get_value_string (SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE , &device_language);
	system_info_get_platform_string("http://tizen.org/system/model_name",&device_id);

	system_info_get_platform_string ("tizen.org/system/platform.name", &device_platfor_name);



	int passfail[4]={0,0,0,0};
	char *passfailtext[4];
	passfailtext[TBT_INFO_RESULT_NOT_TESTED]="Not-Tested";
	passfailtext[TBT_INFO_RESULT_PASS]="Pass";
	passfailtext[TBT_INFO_RESULT_FAIL]="Fail";
	passfailtext[TBT_INFO_RESULT_NOT_SUPPORTED]="Feature-Not-Supported";


	// get summary info
	for (i = 0; i < testDataCount; i++)
	{

		DBG("3 %d %s",i,moduleList[i].name);
			test=&moduleList[i];
			// just for safety
			if(moduleList[i].result>3) moduleList[i].result=TBT_INFO_RESULT_NOT_TESTED; // set result not tested
			passfail[moduleList[i].result]++;
	}



	xmlTextWriterPtr writer;
	writer = xmlNewTextWriterFilename(inputFilePath, 0);
	xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);

	char prefName[32];

	// Xml Start
	xmlTextWriterStartElement(writer, (xmlChar*)"tbt-core");
	{
		// Device
		xmlTextWriterStartElement(writer, (xmlChar*)"device-info");
		{
			xmlTextWriterStartElement(writer, (xmlChar*)"device-model");
			{
				if(device_id==NULL)
					xmlTextWriterWriteAttribute(writer,(xmlChar*)"Name",(xmlChar*)"Unknown");
				else
				{
					xmlTextWriterWriteAttribute(writer,(xmlChar*)"Name",(xmlChar*)device_id);
					free(device_id);
				}
			}
			xmlTextWriterEndElement(writer);

			xmlTextWriterStartElement(writer, (xmlChar*)"device-name");
			{
				if(device_name==NULL)
					xmlTextWriterWriteAttribute(writer,(xmlChar*)"Name",(xmlChar*)"Unknown");
				else
				{
					xmlTextWriterWriteAttribute(writer,(xmlChar*)"Name",(xmlChar*)device_name);
					free(device_name);
				}
			}
			xmlTextWriterEndElement(writer);

			xmlTextWriterStartElement(writer, (xmlChar*)"device-platform");
			{
				if(device_platfor_name==NULL)
					xmlTextWriterWriteAttribute(writer,(xmlChar*)"Name",(xmlChar*)"Unknown");
				else
				{
					xmlTextWriterWriteAttribute(writer,(xmlChar*)"Name",(xmlChar*)device_platfor_name);
					free(device_platfor_name);
				}
			}
			xmlTextWriterEndElement(writer);


			xmlTextWriterStartElement(writer, (xmlChar*)"system-country");
			{
				if(device_country==NULL)
					xmlTextWriterWriteAttribute(writer,(xmlChar*)"Name",(xmlChar*)"Unknown");
				else
				{
					xmlTextWriterWriteAttribute(writer,(xmlChar*)"Name",(xmlChar*)device_country);
					free(device_country);
				}
			}
			xmlTextWriterEndElement(writer);

			xmlTextWriterStartElement(writer, (xmlChar*)"system-language");
			{
				if(device_language==NULL)
					xmlTextWriterWriteAttribute(writer,(xmlChar*)"Name",(xmlChar*)"Unknown");
				else
				{
					xmlTextWriterWriteAttribute(writer,(xmlChar*)"Name",(xmlChar*)device_language);
					free(device_language);
				}
			}
			xmlTextWriterEndElement(writer);
		}
		xmlTextWriterEndElement(writer);


		// Summary
		xmlTextWriterStartElement(writer, (xmlChar*)"summary");
		{
			// total
			xmlTextWriterStartElement(writer, (xmlChar*)"Total");
			{
				xmlTextWriterWriteAttribute(writer,(xmlChar*)"count",(xmlChar*)xmlUtil_itoa(testDataCount));
			}
			xmlTextWriterEndElement(writer);

			// pass
			xmlTextWriterStartElement(writer, (xmlChar*)passfailtext[TBT_INFO_RESULT_NOT_TESTED]);
			{
				xmlTextWriterWriteAttribute(writer,(xmlChar*)"count",(xmlChar*)xmlUtil_itoa(passfail[TBT_INFO_RESULT_NOT_TESTED]));
			}
			xmlTextWriterEndElement(writer);

			// fail
			xmlTextWriterStartElement(writer, (xmlChar*)passfailtext[TBT_INFO_RESULT_PASS]);
			{
				xmlTextWriterWriteAttribute(writer,(xmlChar*)"count",(xmlChar*)xmlUtil_itoa(passfail[TBT_INFO_RESULT_PASS]));
			}
			xmlTextWriterEndElement(writer);

			// not tested
			xmlTextWriterStartElement(writer, (xmlChar*)passfailtext[TBT_INFO_RESULT_FAIL]);
			{
				xmlTextWriterWriteAttribute(writer,(xmlChar*)"count",(xmlChar*)xmlUtil_itoa(passfail[TBT_INFO_RESULT_FAIL]));
			}
			xmlTextWriterEndElement(writer);

			// feature unsupported
			xmlTextWriterStartElement(writer, (xmlChar*)passfailtext[TBT_INFO_RESULT_NOT_SUPPORTED]);
			{
				xmlTextWriterWriteAttribute(writer,(xmlChar*)"count",(xmlChar*)xmlUtil_itoa(passfail[TBT_INFO_RESULT_NOT_SUPPORTED]));
			}
			xmlTextWriterEndElement(writer);

		}
		xmlTextWriterEndElement(writer);
		// end of summary

		// module list start
		xmlTextWriterStartElement(writer, (xmlChar*)"module-list");
		{
			for (i = 0; i < testDataCount; i++)
			{
				test=&moduleList[i];

				// module start
				xmlTextWriterStartElement(writer, (xmlChar*)"module");
				{
					xmlTextWriterWriteAttribute(writer,(xmlChar*)"name",(xmlChar*)test->name);
					xmlTextWriterWriteAttribute(writer,(xmlChar*)"value",(xmlChar*)xmlUtil_itoa(test->result));
					xmlTextWriterWriteAttribute(writer,(xmlChar*)"apptype",(xmlChar*)xmlUtil_itoa(test->apptype));
					xmlTextWriterWriteAttribute(writer,(xmlChar*)"result",(xmlChar*)passfailtext[test->result]);
				}
				xmlTextWriterEndElement(writer);
				// module end

				// lets save in preference also :)
				snprintf(prefName, sizeof(prefName), "mod_%d",test->apptype);
				preference_set_int (prefName, test->result);


			}
		  }
		  xmlTextWriterEndElement(writer);
		  // module list end
	}
	xmlTextWriterEndElement(writer);
	// XML end



	xmlTextWriterEndDocument(writer);
	xmlFreeTextWriter(writer);
}


/**
 * @function		xmlUtil_readResults
 * @since_tizen		2.3
 * @description		XmlUtil ReadResults
 * @parameter		tbt_info*: Tbt Info Pointer
 * @return		bool
 */
bool xmlUtil_readResults(tbt_info *moduleList,const int len)
{
	int i,ret,val;
	char prefName[32];
	for (i = 0; i < len; i++)
	{
		snprintf(prefName, sizeof(prefName), "mod_%d",moduleList[i].apptype);
		ret = preference_get_int (prefName, &val);
		if(ret==PREFERENCE_ERROR_NONE)
		{
			// save result
			moduleList[i].result=val;
		}
		else
		{
			DBG("preference_get_int failed.");
		}
	}

	return true;

}
