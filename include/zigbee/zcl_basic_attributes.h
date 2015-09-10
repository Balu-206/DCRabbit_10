/*
	Copyright (c)2010 Digi International Inc., All Rights Reserved

	This software contains proprietary and confidential information of Digi
	International Inc.  By accepting transfer of this copy, Recipient agrees
	to retain this software in confidence, to prevent disclosure to others,
	and to make no use of this software other than that for which it was
	delivered.  This is a published copyrighted work of Digi International
	Inc.  Except as permitted by federal law, 17 USC 117, copying is strictly
	prohibited.

	Restricted Rights Legend

	Use, duplication, or disclosure by the Government is subject to
	restrictions set forth in sub-paragraph (c)(1)(ii) of The Rights in
	Technical Data and Computer Software clause at DFARS 252.227-7031 or
	subparagraphs (c)(1) and (2) of the Commercial Computer Software -
	Restricted Rights at 48 CFR 52.227-19, as applicable.

	Digi International Inc. 11001 Bren Road East, Minnetonka, MN 55343
*/
/**
	@addtogroup zcl_basic
	@{
	@file zigbee/zcl_basic_attributes.h

	@section zcl_basic ZCL Basic Cluster Overview
	This header should be included from the main .C file (after defining the
	macros described below) to add the globals #zcl_basic_dev_info and
	#zcl_basic_attributes[] to the program.

	It makes use of the following optional macros:
	-	ZCL_APP_VERSION (8-bit)
	-	ZCL_STACK_VERSION (8-bit)
	-	ZCL_HW_VERSION (8-bit)
	-	ZCL_MANUFACTURER_NAME (32-char null-terminated string)
	-	ZCL_MODEL_IDENTIFIER (32-char null-terminated string)
	-	ZCL_DATE_CODE (16-char null-terminated string, starting with YYYYMMDD)
	-	ZCL_FACTORY_RESET_FN (function pointer)

	And one required macro:
	-	ZCL_POWER_SOURCE (8-bit) set to one of the following, optionally ORed
			with #ZCL_BASIC_PS_BATTERY_BACKUP.
			- #ZCL_BASIC_PS_UNKNOWN
			- #ZCL_BASIC_PS_SINGLE_PHASE
			- #ZCL_BASIC_PS_THREE_PHASE
			- #ZCL_BASIC_PS_BATTERY
			- #ZCL_BASIC_PS_DC
			- #ZCL_BASIC_PS_EMERGENCY_CONST
			- #ZCL_BASIC_PS_EMERGENCY_SWITCH

	For example:
@code
	#include "zigbee/zcl_basic.h"
	#define ZCL_APP_VERSION			0x12
	#define ZCL_MANUFACTURER_NAME	"Digi International"
	#define ZCL_MODEL_IDENTIFIER	"Win32"
	#define ZCL_DATE_CODE			"20100601 dev"
	#define ZCL_POWER_SOURCE		ZCL_BASIC_PS_SINGLE_PHASE
	#include "zigbee/zcl_basic_attributes.h"

	// Can now use ZCL_CLUST_ENTRY_BASIC_SERVER in an endpoint's cluster table.
@endcode

	@def ZCL_FACTORY_RESET_FN
	Define #ZCL_FACTORY_RESET_FN at the project level to a function call
	to reset the device to factory defaults.  The function doesn't take
	parameters and should return void:

	@code
	void factory_reset( void);
	#define ZCL_FACTORY_RESET_FN		factory_reset
	@endcode
*/

#include "zigbee/zcl_basic.h"

#ifndef ZCL_POWER_SOURCE
	#error You must at least define ZCL_POWER_SOURCE before including this header
#endif

/// Global used to hold values referenced by #zcl_basic_attributes[].
const struct zcl_basic_dev_info {
	uint8_t	ZCLVersion;
#ifdef ZCL_APP_VERSION
	uint8_t	ApplicationVersion;
#endif
#ifdef ZCL_STACK_VERSION
	uint8_t	StackVersion;
#endif
#ifdef ZCL_HW_VERSION
	uint8_t	HWVersion;
#endif
	uint8_t	PowerSource;
} zcl_basic_dev_info = {
	ZCL_VERSION,
#ifdef ZCL_APP_VERSION
	ZCL_APP_VERSION,
#endif
#ifdef ZCL_STACK_VERSION
	ZCL_STACK_VERSION,
#endif
#ifdef ZCL_HW_VERSION
	ZCL_HW_VERSION,
#endif
	ZCL_POWER_SOURCE
};

/// Table of attributes for the Basic Cluster Server.
// note that this list must be kept sorted by attribute ID
const zcl_attribute_base_t FAR zcl_basic_attributes[] =
{
//	  ID, Flags, Type, Address to data, min, max, read, write
	{ ZCL_BASIC_ATTR_ZCL_VERSION,
		ZCL_ATTRIB_FLAG_READONLY,				// flags
		ZCL_TYPE_UNSIGNED_8BIT,					// type
		&zcl_basic_dev_info.ZCLVersion,		// address to data
	},
#ifdef ZCL_APP_VERSION
	{ ZCL_BASIC_ATTR_APP_VERSION,
		ZCL_ATTRIB_FLAG_READONLY,
		ZCL_TYPE_UNSIGNED_8BIT,
		&zcl_basic_dev_info.ApplicationVersion,
	},
#endif
#ifdef ZCL_STACK_VERSION_ADDR
	{ ZCL_BASIC_ATTR_STACK_VERSION,
		ZCL_ATTRIB_FLAG_READONLY,
		ZCL_TYPE_UNSIGNED_8BIT,
		ZCL_STACK_VERSION_ADDR,
	},
#elif defined ZCL_STACK_VERSION
	{ ZCL_BASIC_ATTR_STACK_VERSION,
		ZCL_ATTRIB_FLAG_READONLY,
		ZCL_TYPE_UNSIGNED_8BIT,
		&zcl_basic_dev_info.StackVersion,
	},
#endif
#ifdef ZCL_HW_VERSION
	{ ZCL_BASIC_ATTR_HW_VERSION,
		ZCL_ATTRIB_FLAG_READONLY,
		ZCL_TYPE_UNSIGNED_8BIT,
		&zcl_basic_dev_info.HWVersion,
	},
#endif
#ifdef ZCL_MANUFACTURER_NAME
	{ ZCL_BASIC_ATTR_MANUFACTURER_NAME,
		ZCL_ATTRIB_FLAG_READONLY,
		ZCL_TYPE_STRING_CHAR,
		ZCL_MANUFACTURER_NAME,
	},
#endif
#ifdef ZCL_MODEL_IDENTIFIER
	{ ZCL_BASIC_ATTR_MODEL_IDENTIFIER,
		ZCL_ATTRIB_FLAG_READONLY,
		ZCL_TYPE_STRING_CHAR,
		ZCL_MODEL_IDENTIFIER,
	},
#endif
#ifdef ZCL_DATE_CODE
	{ ZCL_BASIC_ATTR_DATE_CODE,
		ZCL_ATTRIB_FLAG_READONLY,
		ZCL_TYPE_STRING_CHAR,
		ZCL_DATE_CODE,
	},
#endif
	{ ZCL_BASIC_ATTR_POWER_SOURCE,
		ZCL_ATTRIB_FLAG_READONLY,
		ZCL_TYPE_ENUM_8BIT,
		&zcl_basic_dev_info.PowerSource,
	},

	{ ZCL_ATTRIBUTE_END_OF_LIST }
};

const zcl_attribute_tree_t FAR zcl_basic_attribute_tree[] =
									{ { ZCL_MFG_NONE, zcl_basic_attributes, NULL } };

