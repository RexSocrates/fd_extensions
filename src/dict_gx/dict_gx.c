/*
 *   KAMOME Engineering, Inc. Confidential
 *
 *   Source Materials
 *
 *   Copyright (C) 2014 KAMOME Engineering, Inc. All Rights Reserved.
 *   LICENSED MATERIAL - PROGRAM PROPERTY OF KAMOME Engineering, Inc.
 *
 *   The source code for this program is not published or otherwise
 *   divested of its trade secrets.
 */

/* 
 * Dictionary definitions of objects specified in Gx application (3GPP TS 29.212).
 */
#include <freeDiameter/extension.h>

/* The content of this file follows the same structure as dict_base_proto.c */

#define CHECK_dict_new(_type, _data, _parent, _ref)	\
	CHECK_FCT(fd_dict_new(fd_g_config->cnf_dict, (_type), (_data), (_parent), (_ref)));

#define CHECK_dict_search(_type, _criteria, _what, _result)	\
	CHECK_FCT(fd_dict_search(fd_g_config->cnf_dict, (_type), (_criteria), (_what), (_result), ENOENT));

struct local_rules_definition {
	char			*avp_name;
	enum rule_position	 position;
	int			 min;
	int			 max;
};

#define RULE_ORDER(_position) ((((_position) == RULE_FIXED_HEAD) || ((_position) == RULE_FIXED_TAIL)) ? 1 : 0)

#define PARSE_loc_rules(_rulearray, _parent) {								\
	int __ar;											\
	for (__ar = 0; __ar < sizeof(_rulearray) / sizeof((_rulearray)[0]); __ar++) {			\
		struct dict_rule_data __data = {							\
			NULL,										\
			(_rulearray)[__ar].position,							\
			0,										\
			(_rulearray)[__ar].min,								\
			(_rulearray)[__ar].max								\
		};											\
		__data.rule_order = RULE_ORDER(__data.rule_position);					\
		CHECK_FCT(fd_dict_search(								\
			fd_g_config->cnf_dict,								\
			DICT_AVP,									\
			AVP_BY_NAME_ALL_VENDORS,							\
			(_rulearray)[__ar].avp_name,							\
			&__data.rule_avp, 0));								\
		if (!__data.rule_avp) {									\
			TRACE_DEBUG(INFO, "AVP Not found: '%s'", (_rulearray)[__ar].avp_name);		\
			return ENOENT;									\
		}											\
		CHECK_FCT_DO(fd_dict_new(fd_g_config->cnf_dict, DICT_RULE, &__data, _parent, NULL), {	\
			TRACE_DEBUG(INFO, "Error on rule with AVP '%s'", (_rulearray)[__ar].avp_name);	\
			return EINVAL;									\
		});											\
	}												\
}

static int dict_gx_entry(char *conffile)
{
	struct dict_object *gx;
	TRACE_ENTRY("%p", conffile);

	/* Vendors definitions */
	{
		/* 3rd Generation Partnership Project 2 (3GPP2) */
		{
			struct dict_vendor_data vendor_data = { 5535,	"3rd Generation Partnership Project 2 (3GPP2)" };
			CHECK_FCT(fd_dict_new(fd_g_config->cnf_dict, DICT_VENDOR, &vendor_data, NULL, NULL));
		}

		/* 3GPP */
		{
			struct dict_vendor_data vendor_data = { 10415,	"3GPP" };
			CHECK_FCT(fd_dict_new(fd_g_config->cnf_dict, DICT_VENDOR, &vendor_data, NULL, NULL));
		}

		/* ETSI */
		{
			struct dict_vendor_data vendor_data = { 13019,	"ETSI" };
			CHECK_FCT(fd_dict_new(fd_g_config->cnf_dict, DICT_VENDOR, &vendor_data, NULL, NULL));
		}
	}

	/* Applications section */
	{
		/* Gx (3GPP TS 29.212) */
		{
			struct dict_application_data data = { 16777238,	"Gx Application" };
			CHECK_dict_new(DICT_APPLICATION, &data, NULL, &gx);
		}
	}

	/* AVP section */
	{
		struct dict_object *Address_type;
		struct dict_object *DiameterIdentity_type;
		struct dict_object *IPFilterRule_type;
		struct dict_object *Time_type;
		struct dict_object *UTF8String_type;
		CHECK_dict_search(DICT_TYPE, TYPE_BY_NAME, "Address", &Address_type);
		CHECK_dict_search(DICT_TYPE, TYPE_BY_NAME, "DiameterIdentity", &DiameterIdentity_type);
		CHECK_dict_search(DICT_TYPE, TYPE_BY_NAME, "IPFilterRule", &IPFilterRule_type);
		CHECK_dict_search(DICT_TYPE, TYPE_BY_NAME, "Time", &Time_type);
		CHECK_dict_search(DICT_TYPE, TYPE_BY_NAME, "UTF8String", &UTF8String_type);

		/* Access-Network-Charging-Identifier-Gx */
		{
			struct dict_avp_data data = {
				1022,						/* Code */
				10415,						/* Vendor */
				"Access-Network-Charging-Identifier-Gx",	/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Allocation-Retention-Priority */
		{
			struct dict_avp_data data = {
				1034,						/* Code */
				10415,						/* Vendor */
				"Allocation-Retention-Priority",		/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* AN-GW-Address */
		{
			struct dict_avp_data data = {
				1050,						/* Code */
				10415,						/* Vendor */
				"AN-GW-Address",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
		}

		/* AN-GW-Status */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(AN-GW-Status)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "AN_GW_FAILED",	{ .i32 = 0 } };
			struct dict_avp_data data = {
				2811,						/* Code */
				10415,						/* Vendor */
				"AN-GW-Status",					/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* PN-Aggregate-Max-Bitrate-DL */
		{
			struct dict_avp_data data = {
				1040,						/* Code */
				10415,						/* Vendor */
				"PN-Aggregate-Max-Bitrate-DL",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* APN-Aggregate-Max-Bitrate-UL */
		{
			struct dict_avp_data data = {
				1041,						/* Code */
				10415,						/* Vendor */
				"APN-Aggregate-Max-Bitrate-UL",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Application-Detection-Information */
		{
			struct dict_avp_data data = {
				1098,						/* Code */
				10415,						/* Vendor */
				"Application-Detection-Information",		/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Bearer-Control-Mode */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Bearer-Control-Mode)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "UE_ONLY",	{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "RESERVED",	{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "UE_NW",	{ .i32 = 2 } };
			struct dict_avp_data data = {
				1023,						/* Code */
				10415,						/* Vendor */
				"Bearer-Control-Mode",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Bearer-Identifier */
		{
			struct dict_avp_data data = {
				1020,						/* Code */
				10415,						/* Vendor */
				"Bearer-Identifier",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Bearer-Operation */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Bearer-Operation)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "TERMINATION",		{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "ESTABLISHMENT",	{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "MODIFICATION",	{ .i32 = 2 } };
			struct dict_avp_data data = {
				1021,						/* Code */
				10415,						/* Vendor */
				"Bearer-Operation",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Bearer-Usage */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Bearer-Usage)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "GENERAL",		{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "IMS_SIGNALLING",	{ .i32 = 1 } };
			struct dict_avp_data data = {
				1000,						/* Code */
				10415,						/* Vendor */
				"Bearer-Usage",					/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Charging-Correlation-Indicator */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Charging-Correlation-Indicator)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "CHARGING_IDENTIFIER_REQUIRED",	{ .i32 = 0 } };
			struct dict_avp_data data = {
				1073,						/* Code */
				10415,						/* Vendor */
				"Charging-Correlation-Indicator",		/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Charging-Rule-Base-Name */
		{
			struct dict_avp_data data = {
				1004,						/* Code */
				10415,						/* Vendor */
				"Charging-Rule-Base-Name",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
		}

		/* Charging-Rule-Definition */
		{
			struct dict_avp_data data = {
				1003,						/* Code */
				10415,						/* Vendor */
				"Charging-Rule-Definition",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Charging-Rule-Install */
		{
			struct dict_avp_data data = {
				1001,						/* Code */
				10415,						/* Vendor */
				"Charging-Rule-Install",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Charging-Rule-Name */
		{
			struct dict_avp_data data = {
				1005,						/* Code */
				10415,						/* Vendor */
				"Charging-Rule-Name",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Charging-Rule-Remove */
		{
			struct dict_avp_data data = {
				1002,						/* Code */
				10415,						/* Vendor */
				"Charging-Rule-Remove",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Charging-Rule-Report */
		{
			struct dict_avp_data data = {
				1018,						/* Code */
				10415,						/* Vendor */
				"Charging-Rule-Report",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* CoA-Information */
		{
			struct dict_avp_data data = {
				1039,						/* Code */
				10415,						/* Vendor */
				"CoA-Information",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* CoA-IP-Address */
		{
			struct dict_avp_data data = {
				1035,						/* Code */
				10415,						/* Vendor */
				"CoA-IP-Address",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
		}

		/* CSG-Information-Reporting */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(CSG-Information-Reporting)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "CHANGE_CSG_CELL",			{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "CHANGE_CSG_SUBSCRIBED_HYBRID_CELL",	{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "CHANGE_CSG_UNSUBSCRIBED_HYBRID_CELL",	{ .i32 = 2 } };
			struct dict_avp_data data = {
				1071,						/* Code */
				10415,						/* Vendor */
				"CSG-Information-Reporting",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Default-EPS-Bearer-QoS */
		{
			struct dict_avp_data data = {
				1049,						/* Code */
				10415,						/* Vendor */
				"Default-EPS-Bearer-QoS",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Event-Report-Indication */
		{
			struct dict_avp_data data = {
				1033,						/* Code */
				10415,						/* Vendor */
				"Event-Report-Indication",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Event-Trigger */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Event-Trigger)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "SGSN_CHANGE",						{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "QOS_CHANGE",						{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "RAT_CHANGE",						{ .i32 = 2 } };
			struct dict_enumval_data t_3 = { "TFT_CHANGE",						{ .i32 = 3 } };
			struct dict_enumval_data t_4 = { "PLMN_CHANGE",						{ .i32 = 4 } };
			struct dict_enumval_data t_5 = { "LOSS_OF_BEARER",					{ .i32 = 5 } };
			struct dict_enumval_data t_6 = { "RECOVERY_OF_BEARER",					{ .i32 = 6 } };
			struct dict_enumval_data t_7 = { "IP-CAN_CHANGE",					{ .i32 = 7 } };
			struct dict_enumval_data t_8 = { "QOS_CHANGE_EXCEEDING_AUTHORIZATION",			{ .i32 = 11 } };
			struct dict_enumval_data t_9 = { "RAI_CHANGE",						{ .i32 = 12 } };
			struct dict_enumval_data t_10 = { "USER_LOCATION_CHANGE",				{ .i32 = 13 } };
			struct dict_enumval_data t_11 = { "NO_EVENT_TRIGGERS",					{ .i32 = 14 } };
			struct dict_enumval_data t_12 = { "OUT_OF_CREDIT",					{ .i32 = 15 } };
			struct dict_enumval_data t_13 = { "REALLOCATION_OF_CREDIT",				{ .i32 = 16 } };
			struct dict_enumval_data t_14 = { "REVALIDATION_TIMEOUT",				{ .i32 = 17 } };
			struct dict_enumval_data t_15 = { "UE_IP_ADDRESS_ALLOCATE",				{ .i32 = 18 } };
			struct dict_enumval_data t_16 = { "UE_IP_ADDRESS_RELEASE",				{ .i32 = 19 } };
			struct dict_enumval_data t_17 = { "DEFAULT_EPS_BEARER_QOS_CHANGE",			{ .i32 = 20 } };
			struct dict_enumval_data t_18 = { "AN_GW_CHANGE",					{ .i32 = 21 } };
			struct dict_enumval_data t_19 = { "SUCCESSFUL_RESOURCE_ALLOCATION",			{ .i32 = 22 } };
			struct dict_enumval_data t_20 = { "RESOURCE_MODIFICATION_REQUEST",			{ .i32 = 23 } };
			struct dict_enumval_data t_21 = { "PGW_TRACE_CONTROL",					{ .i32 = 24 } };
			struct dict_enumval_data t_22 = { "UE_TIME_ZONE_CHANGE",				{ .i32 = 25 } };
			struct dict_enumval_data t_23 = { "TAI_CHANGE",						{ .i32 = 26 } };
			struct dict_enumval_data t_24 = { "ECGI_CHANGE",					{ .i32 = 27 } };
			struct dict_enumval_data t_25 = { "CHARGING_CORRELATION_EXCHANGE",			{ .i32 = 28 } };
			struct dict_enumval_data t_26 = { "APN-AMBR_MODIFICATION_FAILURE",			{ .i32 = 29 } };
			struct dict_enumval_data t_27 = { "USER_CSG_INFORMATION_CHANGE",			{ .i32 = 30 } };
			struct dict_enumval_data t_28 = { "USAGE_REPORT",					{ .i32 = 33 } };
			struct dict_enumval_data t_29 = { "DEFAULT-EPS-BEARER-QOS_MODIFICATION_FAILURE",	{ .i32 = 34 } };
			struct dict_enumval_data t_30 = { "USER_CSG_HYBRID_SUBSCRIBED_INFORMATION_CHANGE",	{ .i32 = 35 } };
			struct dict_enumval_data t_31 = { "USER_CSG_ HYBRID_UNSUBSCRIBED_INFORMATION_CHANGE",	{ .i32 = 36 } };
			struct dict_enumval_data t_32 = { "ROUTING_RULE_CHANGE",				{ .i32 = 37 } };
			struct dict_enumval_data t_33 = { "APPLICATION_START",					{ .i32 = 39 } };
			struct dict_enumval_data t_34 = { "APPLICATION_STOP",					{ .i32 = 40 } };
			struct dict_enumval_data t_35 = { "CS_TO_PS_HANDOVER",					{ .i32 = 42 } };
			struct dict_enumval_data t_36 = { "UE_LOCAL_IP_ADDRESS_CHANGE",				{ .i32 = 43 } };
			struct dict_enumval_data t_37 = { "H(E)NB_LOCAL_IP_ADDRESS_CHANGE",			{ .i32 = 44 } };
			struct dict_enumval_data t_38 = { "ACCESS_NETWORK_INFO_REPORT",				{ .i32 = 45 } };
			struct dict_avp_data data = {
				1006,						/* Code */
				10415,						/* Vendor */
				"Event-Trigger",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_3, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_4, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_5, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_6, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_7, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_8, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_9, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_10, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_11, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_12, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_13, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_14, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_15, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_16, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_17, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_18, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_19, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_20, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_21, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_22, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_23, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_24, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_25, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_26, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_27, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_28, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_29, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_30, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_31, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_32, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_33, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_34, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_35, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_36, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_37, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_38, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Flow-Direction */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Flow-Direction)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "UNSPECIFIED",		{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "DOWNLINK",		{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "UPLINK",		{ .i32 = 2 } };
			struct dict_enumval_data t_3 = { "BIDIRECTIONAL",	{ .i32 = 3 } };
			struct dict_avp_data data = {
				1080,						/* Code */
				10415,						/* Vendor */
				"Flow-Direction",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_3, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Flow-Information */
		{
			struct dict_avp_data data = {
				1058,						/* Code */
				10415,						/* Vendor */
				"Flow-Information",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Flow-Label */
		{
			struct dict_avp_data data = {
				1057,						/* Code */
				10415,						/* Vendor */
				"Flow-Label",					/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* IP-CAN-Type */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(IP-CAN-Type)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "3GPP-GPRS",		{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "DOCSIS",		{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "xDSL",		{ .i32 = 2 } };
			struct dict_enumval_data t_3 = { "WiMAX",		{ .i32 = 3 } };
			struct dict_enumval_data t_4 = { "3GPP2",		{ .i32 = 4 } };
			struct dict_enumval_data t_5 = { "3GPP-EPS",		{ .i32 = 5 } };
			struct dict_enumval_data t_6 = { "Non-3GPP-EPS",	{ .i32 = 6 } };
			struct dict_avp_data data = {
				1027,						/* Code */
				10415,						/* Vendor */
				"IP-CAN-Type",					/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_3, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_4, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_5, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_6, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Guaranteed-Bitrate-DL */
		{
			struct dict_avp_data data = {
				1025,						/* Code */
				10415,						/* Vendor */
				"Guaranteed-Bitrate-DL",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Guaranteed-Bitrate-UL */
		{
			struct dict_avp_data data = {
				1026,						/* Code */
				10415,						/* Vendor */
				"Guaranteed-Bitrate-UL",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* HeNB-Local-IP-Address */
		{
			struct dict_avp_data data = {
				2804,						/* Code */
				10415,						/* Vendor */
				"HeNB-Local-IP-Address",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
		}

		/* Metering-Method */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Metering-Method)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "DURATION",		{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "VOLUME",		{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "DURATION_VOLUME",	{ .i32 = 2 } };
			struct dict_avp_data data = {
				1007,						/* Code */
				10415,						/* Vendor */
				"Metering-Method",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Monitoring-Key */
		{
			struct dict_avp_data data = {
				1066,						/* Code */
				10415,						/* Vendor */
				"Monitoring-Key",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Mute-Notification */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Mute-Notification)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "MUTE_REQUIRED",	{ .i32 = 0 } };
			struct dict_avp_data data = {
				2809,						/* Code */
				10415,						/* Vendor */
				"Mute-Notification",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Monitoring-Time */
		{
			struct dict_avp_data data = {
				2810,						/* Code */
				10415,						/* Vendor */
				"Monitoring-Time",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
		}

		/* NetLoc-Access-Support */
		{
			struct dict_avp_data data = {
				2824,						/* Code */
				10415,						/* Vendor */
				"NetLoc-Access-Support",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Network-Request-Support */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Network-Request-Support)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "NETWORK_REQUEST NOT SUPPORTED",	{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "NETWORK_REQUEST SUPPORTED",		{ .i32 = 1 } };
			struct dict_avp_data data = {
				1024,						/* Code */
				10415,						/* Vendor */
				"Network-Request-Support",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Offline */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Offline)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "DISABLE_OFFLINE",	{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "ENABLE_OFFLINE",	{ .i32 = 1 } };
			struct dict_avp_data data = {
				1008,						/* Code */
				10415,						/* Vendor */
				"Offline",					/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Online */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Online)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "DISABLE_ONLINE",	{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "ENABLE_ONLINE",	{ .i32 = 1 } };
			struct dict_avp_data data = {
				1009,						/* Code */
				10415,						/* Vendor */
				"Online",					/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Packet-Filter-Content */
		{
			struct dict_avp_data data = {
				1059,						/* Code */
				10415,						/* Vendor */
				"Packet-Filter-Content",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, IPFilterRule_type, NULL);
		}

		/* Packet-Filter-Identifier */
		{
			struct dict_avp_data data = {
				1060,						/* Code */
				10415,						/* Vendor */
				"Packet-Filter-Identifier",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Packet-Filter-Information */
		{
			struct dict_avp_data data = {
				1061,						/* Code */
				10415,						/* Vendor */
				"Packet-Filter-Information",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Packet-Filter-Operation */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Packet-Filter-Operation)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "DELETION",	{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "ADDITION",	{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "MODIFICATION",	{ .i32 = 2 } };
			struct dict_avp_data data = {
				1062,						/* Code */
				10415,						/* Vendor */
				"Packet-Filter-Operation",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Packet-Filter-Usage */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Packet-Filter-Usage)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "SEND_TO_UE",	{ .i32 = 1 } };
			struct dict_avp_data data = {
				1072,						/* Code */
				10415,						/* Vendor */
				"Packet-Filter-Usage",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* PCC-Rule-Status */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(PCC-Rule-Status)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "ACTIVE",			{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "INACTIVE",			{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "TEMPORARILY INACTIVE",	{ .i32 = 2 } };
			struct dict_avp_data data = {
				1019,						/* Code */
				10415,						/* Vendor */
				"PCC-Rule-Status",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* PDN-Connection-ID */
		{
			struct dict_avp_data data = {
				1065,						/* Code */
				10415,						/* Vendor */
				"PDN-Connection-ID",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* RAT-Type */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(RAT-Type)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "WLAN",		{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "VIRTUAL",		{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "UTRAN",		{ .i32 = 1000 } };
			struct dict_enumval_data t_3 = { "GERAN",		{ .i32 = 1001 } };
			struct dict_enumval_data t_4 = { "GAN",			{ .i32 = 1002 } };
			struct dict_enumval_data t_5 = { "HSPA_EVOLUTION",	{ .i32 = 1003 } };
			struct dict_enumval_data t_6 = { "EUTRAN",		{ .i32 = 1004 } };
			struct dict_enumval_data t_7 = { "CDMA2000_1X",		{ .i32 = 2000 } };
			struct dict_enumval_data t_8 = { "HRPD",		{ .i32 = 2001 } };
			struct dict_enumval_data t_9 = { "UMB",			{ .i32 = 2002 } };
			struct dict_enumval_data t_10 = { "EHRPD",		{ .i32 = 2003 } };
			struct dict_avp_data data = {
				1032,						/* Code */
				10415,						/* Vendor */
				"RAT-Type",					/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_3, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_4, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_5, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_6, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_7, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_8, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_9, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_10, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Redirect-Information */
		{
			struct dict_avp_data data = {
				1085,						/* Code */
				10415,						/* Vendor */
				"Redirect-Information",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Redirect-Support */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Redirect-Support)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "REDIRECTION_DISABLED",	{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "REDIRECTION_ENABLED",		{ .i32 = 1 } };
			struct dict_avp_data data = {
				1086,						/* Code */
				10415,						/* Vendor */
				"Redirect-Support",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Reporting-Level */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Reporting-Level)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "SERVICE_IDENTIFIER_LEVEL",		{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "RATING_GROUP_LEVEL",			{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "SPONSORED_CONNECTIVITY_LEVEL",	{ .i32 = 2 } };
			struct dict_avp_data data = {
				1011,						/* Code */
				10415,						/* Vendor */
				"Reporting-Level",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Resource-Allocation-Notification */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Resource-Allocation-Notification)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "ENABLE_NOTIFICATION",	{ .i32 = 0 } };
			struct dict_avp_data data = {
				1063,						/* Code */
				10415,						/* Vendor */
				"Resource-Allocation-Notification",		/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Revalidation-Time */
		{
			struct dict_avp_data data = {
				1042,						/* Code */
				10415,						/* Vendor */
				"Revalidation-Time",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
		}

		/* Routing-Filter */
		{
			struct dict_avp_data data = {
				1078,						/* Code */
				10415,						/* Vendor */
				"Routing-Filter",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Routing-IP-Address */
		{
			struct dict_avp_data data = {
				1079,						/* Code */
				10415,						/* Vendor */
				"Routing-IP-Address",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
		}

		/* Routing-Rule-Definition */
		{
			struct dict_avp_data data = {
				1076,						/* Code */
				10415,						/* Vendor */
				"Routing-Rule-Definition",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Routing-Rule-Identifier */
		{
			struct dict_avp_data data = {
				1077,						/* Code */
				10415,						/* Vendor */
				"Routing-Rule-Identifier",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Routing-Rule-Install */
		{
			struct dict_avp_data data = {
				1081 ,						/* Code */
				10415,						/* Vendor */
				"Routing-Rule-Install",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Routing-Rule-Remove */
		{
			struct dict_avp_data data = {
				1075,						/* Code */
				10415,						/* Vendor */
				"Routing-Rule-Remove",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Precedence */
		{
			struct dict_avp_data data = {
				1010,						/* Code */
				10415,						/* Vendor */
				"Precedence",					/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Pre-emption-Capability */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Pre-emption-Capability)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "PRE-EMPTION_CAPABILITY_ENABLED",	{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "PRE-EMPTION_CAPABILITY_DISABLED",	{ .i32 = 1 } };
			struct dict_avp_data data = {
				1047,						/* Code */
				10415,						/* Vendor */
				"Pre-emption-Capability",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Pre-emption-Vulnerability */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Pre-emption-Vulnerability)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "PRE-EMPTION_VULNERABILITY_ENABLED",	{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "PRE-EMPTION_VULNERABILITY_DISABLED",	{ .i32 = 1 } };
			struct dict_avp_data data = {
				1048,						/* Code */
				10415,						/* Vendor */
				"Pre-emption-Vulnerability",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Priority-Level */
		{
			struct dict_avp_data data = {
				1046,						/* Code */
				10415,						/* Vendor */
				"Priority-Level",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* PS-to-CS-Session-Continuity */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(PS-to-CS-Session-Continuity)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "VIDEO_PS2CS_CONT_CANDIDATE",	{ .i32 = 0 } };
			struct dict_avp_data data = {
				1099,						/* Code */
				10415,						/* Vendor */
				"PS-to-CS-Session-Continuity",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Rule-Activation-Time */
		{
			struct dict_avp_data data = {
				1043,						/* Code */
				10415,						/* Vendor */
				"Rule-Activation-Time",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
		}

		/* Rule-Deactivation-Time */
		{
			struct dict_avp_data data = {
				1044,						/* Code */
				10415,						/* Vendor */
				"Rule-Deactivation-Time",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
		}

		/* Rule-Failure-Code */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Rule-Failure-Code)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "UNKNOWN_RULE_NAME",			{ .i32 = 1 } };
			struct dict_enumval_data t_1 = { "RATING_GROUP_ERROR",			{ .i32 = 2 } };
			struct dict_enumval_data t_2 = { "SERVICE_IDENTIFIER_ERROR",		{ .i32 = 3 } };
			struct dict_enumval_data t_3 = { "GW/PCEF_MALFUNCTION",			{ .i32 = 4 } };
			struct dict_enumval_data t_4 = { "RESOURCES_LIMITATION",		{ .i32 = 5 } };
			struct dict_enumval_data t_5 = { "MAX_NR_BEARERS_REACHED",		{ .i32 = 6 } };
			struct dict_enumval_data t_6 = { "UNKNOWN_BEARER_ID",			{ .i32 = 7 } };
			struct dict_enumval_data t_7 = { "MISSING_BEARER_ID",			{ .i32 = 8 } };
			struct dict_enumval_data t_8 = { "MISSING_FLOW_INFORMATION",		{ .i32 = 9 } };
			struct dict_enumval_data t_9 = { "RESOURCE_ALLOCATION_FAILURE",		{ .i32 = 10 } };
			struct dict_enumval_data t_10 = { "UNSUCCESSFUL_QOS_VALIDATION",	{ .i32 = 11 } };
			struct dict_enumval_data t_11 = { "INCORRECT_FLOW_INFORMATION",		{ .i32 = 12 } };
			struct dict_enumval_data t_12 = { "PS_TO_CS_HANDOVER",			{ .i32 = 13 } };
			struct dict_enumval_data t_13 = { "TDF_APPLICATION_IDENTIFIER_ERROR",	{ .i32 = 14 } };
			struct dict_enumval_data t_14 = { "NO_BEARER_BOUND",			{ .i32 = 15 } };
			struct dict_enumval_data t_15 = { "FILTER_RESTRICTIONS",		{ .i32 = 16 } };
			struct dict_enumval_data t_16 = { "AN_GW_FAILED",			{ .i32 = 17 } };
			struct dict_enumval_data t_17 = { "MISSING_REDIRECT_SERVER_ADDRESS",	{ .i32 = 18 } };
			struct dict_avp_data data = {
				1031,						/* Code */
				10415,						/* Vendor */
				"Rule-Failure-Code",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_3, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_4, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_5, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_6, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_7, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_8, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_9, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_10, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_11, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_12, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_13, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_14, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_15, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_16, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_17, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* QoS-Class-Identifier */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(QoS-Class-Identifier)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "QCI_1",	{ .i32 = 1 } };
			struct dict_enumval_data t_1 = { "QCI_2",	{ .i32 = 2 } };
			struct dict_enumval_data t_2 = { "QCI_3",	{ .i32 = 3 } };
			struct dict_enumval_data t_3 = { "QCI_4",	{ .i32 = 4 } };
			struct dict_enumval_data t_4 = { "QCI_5",	{ .i32 = 5 } };
			struct dict_enumval_data t_5 = { "QCI_6",	{ .i32 = 6 } };
			struct dict_enumval_data t_6 = { "QCI_7",	{ .i32 = 7 } };
			struct dict_enumval_data t_7 = { "QCI_8",	{ .i32 = 8 } };
			struct dict_enumval_data t_8 = { "QCI_9",	{ .i32 = 9 } };
			struct dict_avp_data data = {
				1028,						/* Code */
				10415,						/* Vendor */
				"QoS-Class-Identifier",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_3, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_4, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_5, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_6, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_7, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_8, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* QoS-Information */
		{
			struct dict_avp_data data = {
				1016,						/* Code */
				10415,						/* Vendor */
				"QoS-Information",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* QoS-Negotiation  */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(QoS-Negotiation )",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "NO_QoS_NEGOTIATION",		{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "QoS_NEGOTIATION_SUPPORTED",	{ .i32 = 1 } };
			struct dict_avp_data data = {
				1029,						/* Code */
				10415,						/* Vendor */
				"QoS-Negotiation",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* QoS-Upgrade */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(QoS-Upgrade)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "QoS_UPGRADE_NOT_SUPPORTED",	{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "QoS_UPGRADE_SUPPORTED",	{ .i32 = 1 } };
			struct dict_avp_data data = {
				1030,						/* Code */
				10415,						/* Vendor */
				"QoS-Upgrade",					/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Security-Parameter-Index */
		{
			struct dict_avp_data data = {
				1056,						/* Code */
				10415,						/* Vendor */
				"Security-Parameter-Index",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Session-Release-Cause */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Session-Release-Cause)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "UNSPECIFIED_REASON",			{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "UE_SUBSCRIPTION_REASON",		{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "INSUFFICIENT_SERVER_RESOURCES",	{ .i32 = 2 } };
			struct dict_enumval_data t_3 = { "IP_CAN_SESSION_TERMINATION",		{ .i32 = 3 } };
			struct dict_enumval_data t_4 = { "UE_IP_ADDRESS_RELEASE",		{ .i32 = 4 } };
			struct dict_avp_data data = {
				1045,						/* Code */
				10415,						/* Vendor */
				"Session-Release-Cause",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_3, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_4, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* TDF-Information */
		{
			struct dict_avp_data data = {
				1087,						/* Code */
				10415,						/* Vendor */
				"TDF-Information",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* TDF-Application-Identifier */
		{
			struct dict_avp_data data = {
				1088,						/* Code */
				10415,						/* Vendor */
				"TDF-Application-Identifier",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* TDF-Application-Instance-Identifier */
		{
			struct dict_avp_data data = {
				2802,						/* Code */
				10415,						/* Vendor */
				"TDF-Application-Instance-Identifier",		/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* TDF-Destination-Host */
		{
			struct dict_avp_data data = {
				1089,						/* Code */
				10415,						/* Vendor */
				"TDF-Destination-Host",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, DiameterIdentity_type, NULL);
		}

		/* TDF-Destination-Realm */
		{
			struct dict_avp_data data = {
				1090,						/* Code */
				10415,						/* Vendor */
				"TDF-Destination-Realm",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, DiameterIdentity_type, NULL);
		}

		/* TDF-IP-Address */
		{
			struct dict_avp_data data = {
				1091,						/* Code */
				10415,						/* Vendor */
				"TDF-IP-Address",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
		}

		/* TFT-Filter */
		{
			struct dict_avp_data data = {
				1012,						/* Code */
				10415,						/* Vendor */
				"TFT-Filter",					/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, IPFilterRule_type, NULL);
		}

		/* TFT-Packet-Filter-Information */
		{
			struct dict_avp_data data = {
				1013,						/* Code */
				10415,						/* Vendor */
				"TFT-Packet-Filter-Information",		/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* ToS-Traffic-Class */
		{
			struct dict_avp_data data = {
				1014,						/* Code */
				10415,						/* Vendor */
				"ToS-Traffic-Class",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Tunnel-Header-Filter */
		{
			struct dict_avp_data data = {
				1036,						/* Code */
				10415,						/* Vendor */
				"Tunnel-Header-Filter",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, IPFilterRule_type, NULL);
		}

		/* Tunnel-Header-Length */
		{
			struct dict_avp_data data = {
				1037,						/* Code */
				10415,						/* Vendor */
				"Tunnel-Header-Length",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Tunnel-Information */
		{
			struct dict_avp_data data = {
				1038,						/* Code */
				10415,						/* Vendor */
				"Tunnel-Information",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* UDP-Source-Port */
		{
			struct dict_avp_data data = {
				2806,						/* Code */
				10415,						/* Vendor */
				"UDP-Source-Port",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* UE-Local-IP-Address */
		{
			struct dict_avp_data data = {
				2805,						/* Code */
				10415,						/* Vendor */
				"UE-Local-IP-Address",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
		}

		/* Usage-Monitoring-Information */
		{
			struct dict_avp_data data = {
				1067,						/* Code */
				10415,						/* Vendor */
				"Usage-Monitoring-Information",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Usage-Monitoring-Level */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Usage-Monitoring-Level)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "SESSION_LEVEL",	{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "PCC_RULE_LEVEL",	{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "ADC_RULE_LEVEL",	{ .i32 = 2 } };
			struct dict_avp_data data = {
				1068,						/* Code */
				10415,						/* Vendor */
				"Usage-Monitoring-Level",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Usage-Monitoring-Report */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Usage-Monitoring-Report)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "USAGE_MONITORING_REPORT_REQUIRED",	{ .i32 = 0 } };
			struct dict_avp_data data = {
				1069,						/* Code */
				10415,						/* Vendor */
				"Usage-Monitoring-Report",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Usage-Monitoring-Support */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Usage-Monitoring-Support)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "USAGE_MONITORING_DISABLED",	{ .i32 = 0 } };
			struct dict_avp_data data = {
				1070,						/* Code */
				10415,						/* Vendor */
				"Usage-Monitoring-Support",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* User-Location-Info-Time */
		{
			struct dict_avp_data data = {
				2812,						/* Code */
				10415,						/* Vendor */
				"User-Location-Info-Time",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
		}

		/*
		 * RFC 4006
		 */

		/* CC-Request-Number */
		{
			struct dict_avp_data data = {
				415,						/* Code */
				0,						/* Vendor */
				"CC-Request-Number",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* CC-Request-Type */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(CC-Request-Type)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "INITIAL_REQUEST",	{ .i32 = 1 } };
			struct dict_enumval_data t_1 = { "UPDATE_REQUEST",	{ .i32 = 2 } };
			struct dict_enumval_data t_2 = { "TERMINATION_REQUEST",	{ .i32 = 3 } };
			struct dict_enumval_data t_3 = { "EVENT_REQUEST",	{ .i32 = 4 } };
			struct dict_avp_data data = {
				416,						/* Code */
				0,						/* Vendor */
				"CC-Request-Type",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_3, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Final-Unit-Action */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Final-Unit-Action)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "TERMINATE",		{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "REDIRECT",		{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "RESTRICT_ACCESS",	{ .i32 = 2 } };
			struct dict_avp_data data = {
				449,						/* Code */
				0,						/* Vendor */
				"Final-Unit-Action",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Restriction-Filter-Rule */
		{
			struct dict_avp_data data = {
				438,						/* Code */
				0,						/* Vendor */
				"Restriction-Filter-Rule",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, IPFilterRule_type, NULL);
		}

		/* Redirect-Address-Type */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Redirect-Address-Type)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "IPV4_ADDRESS",	{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "IPV6_ADDRESS",	{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "URL",			{ .i32 = 2 } };
			struct dict_enumval_data t_3 = { "SIP_URI",		{ .i32 = 3 } };
			struct dict_avp_data data = {
				433,						/* Code */
				0,						/* Vendor */
				"Redirect-Address-Type",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_3, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Redirect-Server-Address */
		{
			struct dict_avp_data data = {
				435,						/* Code */
				0,						/* Vendor */
				"Redirect-Server-Address",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
		}

		/* Redirect-Server */
		{
			struct dict_object *avp;
			struct dict_avp_data data = {
				434,						/* Code */
				0,						/* Vendor */
				"Redirect-Server",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			struct local_rules_definition rules[] = {
				{ "Redirect-Address-Type",	RULE_REQUIRED,	-1,	1 },
				{ "Redirect-Server-Address",	RULE_REQUIRED,	-1,	1 }
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, &avp);
			PARSE_loc_rules(rules, avp);
		}

		/* Final-Unit-Indication */
		{
			struct dict_object *avp;
			struct dict_avp_data data = {
				430,						/* Code */
				0,						/* Vendor */
				"Final-Unit-Indication",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			struct local_rules_definition rules[] = {
				{ "Final-Unit-Action",		RULE_REQUIRED,	-1,	 1 },
				{ "Restriction-Filter-Rule",	RULE_OPTIONAL,	-1,	-1 },
				{ "Filter-Id",			RULE_OPTIONAL,	-1,	-1 },
				{ "Redirect-Server",		RULE_OPTIONAL,	-1,	 1 },
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, &avp);
			PARSE_loc_rules(rules, avp);
		}

		/* Tariff-Time-Change */
		{
			struct dict_avp_data data = {
				451,						/* Code */
				0,						/* Vendor */
				"Tariff-Time-Change",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, Time_type, NULL);
		}

		/* CC-Time */
		{
			struct dict_avp_data data = {
				420,						/* Code */
				0,						/* Vendor */
				"CC-Time",					/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Value-Digits */
		{
			struct dict_avp_data data = {
				447,						/* Code */
				0,						/* Vendor */
				"Value-Digits",					/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_INTEGER64				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Exponent */
		{
			struct dict_avp_data data = {
				429,						/* Code */
				0,						/* Vendor */
				"Exponent",					/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Unit-Value */
		{
			struct dict_object *avp;
			struct dict_avp_data data = {
				445,						/* Code */
				0,						/* Vendor */
				"Unit-Value",					/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			struct local_rules_definition rules[] = {
				{ "Value-Digits",	RULE_REQUIRED,	-1,	1 },
				{ "Exponent",		RULE_OPTIONAL,	-1,	1 }
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, &avp);
			PARSE_loc_rules(rules, avp);
		}

		/* Currency-Code */
		{
			struct dict_avp_data data = {
				425,						/* Code */
				0,						/* Vendor */
				"Currency-Code",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* CC-Money */
		{
			struct dict_object *avp;
			struct dict_avp_data data = {
				413,						/* Code */
				0,						/* Vendor */
				"CC-Money",					/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			struct local_rules_definition rules[] = {
				{ "Unit-Value",		RULE_REQUIRED,	-1,	1 },
				{ "Currency-Code",	RULE_OPTIONAL,	-1,	1 }
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, &avp);
			PARSE_loc_rules(rules, avp);
		}

		/* CC-Total-Octets */
		{
			struct dict_avp_data data = {
				421,						/* Code */
				0,						/* Vendor */
				"CC-Total-Octets",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_UNSIGNED64				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* CC-Input-Octets */
		{
			struct dict_avp_data data = {
				412,						/* Code */
				0,						/* Vendor */
				"CC-Input-Octets",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_UNSIGNED64				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* CC-Output-Octets */
		{
			struct dict_avp_data data = {
				414,						/* Code */
				0,						/* Vendor */
				"CC-Output-Octets",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_UNSIGNED64				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* CC-Service-Specific-Units */
		{
			struct dict_avp_data data = {
				417,						/* Code */
				0,						/* Vendor */
				"CC-Service-Specific-Units",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_UNSIGNED64				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Granted-Service-Unit */
		{
			struct dict_object *avp;
			struct dict_avp_data data = {
				431,						/* Code */
				0,						/* Vendor */
				"Granted-Service-Unit",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			struct local_rules_definition rules[] = {
				{ "Tariff-Time-Change",		RULE_OPTIONAL,	-1,	1 },
				{ "CC-Time",			RULE_OPTIONAL,	-1,	1 },
				{ "CC-Money",			RULE_OPTIONAL,	-1,	1 },
				{ "CC-Total-Octets",		RULE_OPTIONAL,	-1,	1 },
				{ "CC-Input-Octets",		RULE_OPTIONAL,	-1,	1 },
				{ "CC-Output-Octets",		RULE_OPTIONAL,	-1,	1 },
				{ "CC-Service-Specific-Units",	RULE_OPTIONAL,	-1,	1 }
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, &avp);
			PARSE_loc_rules(rules, avp);
		}

		/* Rating-Group */
		{
			struct dict_avp_data data = {
				432,						/* Code */
				0,						/* Vendor */
				"Rating-Group",					/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Redirect-Server-Address */
		{
			struct dict_avp_data data = {
				435,						/* Code */
				0,						/* Vendor */
				"Redirect-Server-Address",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
		}

		/* Service-Identifier */
		{
			struct dict_avp_data data = {
				439,						/* Code */
				0,						/* Vendor */
				"Service-Identifier",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Subscription-Id-Type */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Subscription-Id-Type)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "END_USER_E164",	{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "END_USER_IMSI",	{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "END_USER_SIP_URI",	{ .i32 = 2 } };
			struct dict_enumval_data t_3 = { "END_USER_NAI",	{ .i32 = 3 } };
			struct dict_avp_data data = {
				450,						/* Code */
				0,						/* Vendor */
				"Subscription-Id-Type",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_3, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Subscription-Id-Data */
		{
			struct dict_avp_data data = {
				444,						/* Code */
				0,						/* Vendor */
				"Subscription-Id-Data",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new( DICT_AVP, &data , UTF8String_type, NULL);
		}

		/* Subscription-Id */
		{
			struct dict_object *avp;
			struct dict_avp_data data = {
				443,						/* Code */
				0,						/* Vendor */
				"Subscription-Id",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			struct local_rules_definition rules[] = {
				{ "Subscription-Id-Type",	RULE_REQUIRED,	-1,	1 },
				{ "Subscription-Id-Data",	RULE_REQUIRED,	-1,	1 }
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, &avp);
			PARSE_loc_rules(rules, avp);
		}

		/* User-Equipment-Info-Type */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(User-Equipment-Info-Type)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "IMEISV",	{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "MAC",		{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "EUI64",	{ .i32 = 2 } };
			struct dict_avp_data data = {
				459,						/* Code */
				0,						/* Vendor */
				"User-Equipment-Info-Type",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* User-Equipment-Info-Value */
		{
			struct dict_avp_data data = {
				460,						/* Code */
				0,						/* Vendor */
				"User-Equipment-Info-Value",			/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* User-Equipment-Info */
		{
			struct dict_object *avp;
			struct dict_avp_data data = {
				458,						/* Code */
				0,						/* Vendor */
				"User-Equipment-Info",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			struct local_rules_definition rules[] = {
				{ "User-Equipment-Info-Type",	RULE_REQUIRED,	-1,	1 },
				{ "User-Equipment-Info-Value",	RULE_REQUIRED,	-1,	1 }
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, &avp);
			PARSE_loc_rules(rules, avp);
		}

		/* Tariff-Change-Usage */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Tariff-Change-Usage)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "UNIT_BEFORE_TARIFF_CHANGE",	{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "UNIT_AFTER_TARIFF_CHANGE",	{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "UNIT_INDETERMINATE",		{ .i32 = 2 } };
			struct dict_avp_data data = {
				452,						/* Code */
				0,						/* Vendor */
				"Tariff-Change-Usage",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Used-Service-Unit */
		{
			struct dict_object *avp;
			struct dict_avp_data data = {
				446,						/* Code */
				0,						/* Vendor */
				"Used-Service-Unit",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_MANDATORY,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			struct local_rules_definition rules[] = {
				{ "Tariff-Change-Usage",	RULE_OPTIONAL,	-1,	1 },
				{ "CC-Time",			RULE_OPTIONAL,	-1,	1 },
				{ "CC-Money",			RULE_OPTIONAL,	-1,	1 },
				{ "CC-Total-Octets",		RULE_OPTIONAL,	-1,	1 },
				{ "CC-Input-Octets",		RULE_OPTIONAL,	-1,	1 },
				{ "CC-Output-Octets",		RULE_OPTIONAL,	-1,	1 },
				{ "CC-Service-Specific-Units",	RULE_OPTIONAL,	-1,	1 }
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, &avp);
			PARSE_loc_rules(rules, avp);
		}

		/* 3GPP TS 29.229 */

		/* Charging-Information */
		{
			struct dict_avp_data data = {
				618,						/* Code */
				10415,						/* Vendor */
				"Charging-Information",				/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Feature-List-ID */
		{
			struct dict_avp_data data = {
				629,						/* Code */
				10415,						/* Vendor */
				"Feature-List-ID",				/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Feature-List */
		{
			struct dict_avp_data data = {
				630,						/* Code */
				10415,						/* Vendor */
				"Feature-List",					/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		}

		/* Supported-Features */
		{
			struct dict_object *avp;
			struct dict_avp_data data = {
				628,						/* Code */
				10415,						/* Vendor */
				"Supported-Features",				/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			struct local_rules_definition rules[] = {
				{ "Vendor-Id",		RULE_REQUIRED,	-1,	1 },
				{ "Feature-List-ID",	RULE_REQUIRED,	-1,	1 },
				{ "Feature-List",	RULE_REQUIRED,	-1,	1 },
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, &avp);
			PARSE_loc_rules(rules, avp);
		}

		/* 3GPP TS 29.061 */

		/* 3GPP-GGSN-Address */
		{
			struct dict_avp_data data = {
				7,						/* Code */
				10415,						/* Vendor */
				"3GPP-GGSN-Address",				/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* 3GPP-GGSN-IPv6-Address */
		{
			struct dict_avp_data data = {
				16,						/* Code */
				10415,						/* Vendor */
				"3GPP-GGSN-IPv6-Address",			/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* 3GPP-MS-TimeZone */
		{
			struct dict_avp_data data = {
				23,						/* Code */
				10415,						/* Vendor */
				"3GPP-MS-TimeZone",				/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* 3GPP-RAT-Type */
		{
			struct dict_avp_data data = {
				21,						/* Code */
				10415,						/* Vendor */
				"3GPP-RAT-Type",				/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* 3GPP-SGSN-Address */
		{
			struct dict_avp_data data = {
				6,						/* Code */
				10415,						/* Vendor */
				"3GPP-SGSN-Address",				/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* 3GPP-SGSN-IPv6-Address */
		{
			struct dict_avp_data data = {
				15,						/* Code */
				10415,						/* Vendor */
				"3GPP-SGSN-IPv6-Address",			/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* 3GPP-SGSN-MCC-MNC */
		{
			struct dict_avp_data data = {
				18,						/* Code */
				10415,						/* Vendor */
				"3GPP-SGSN-MCC-MNC",				/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
		};

		/* 3GPP-User-Location-Info */
		{
			struct dict_avp_data data = {
				22,						/* Code */
				10415,						/* Vendor */
				"3GPP-User-Location-Info",			/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* RAI */
		{
			struct dict_avp_data data = {
				909,						/* Code */
				10415,						/* Vendor */
				"RAI",						/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
		};

		/* TWAN-Identifier */
		{
			struct dict_avp_data data = {
				29,						/* Code */
				10415,						/* Vendor */
				"TWAN-Identifier",				/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* 3GPP TS 29.214 */

		/* Access-Network-Charging-Address */
		{
			struct dict_avp_data data = {
				501,						/* Code */
				10415,						/* Vendor */
				"Access-Network-Charging-Address",		/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, Address_type, NULL);
		};

		/* Access-Network-Charging-Identifier-Value */
		{
			struct dict_avp_data data = {
				503,						/* Code */
				10415,						/* Vendor */
				"Access-Network-Charging-Identifier-Value",	/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* AF-Charging-Identifier */
		{
			struct dict_avp_data data = {
				505,						/* Code */
				10415,						/* Vendor */
				"AF-Charging-Identifier",			/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* AF-Signalling-Protocol */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32, "Enumerated(AF-Signalling-Protocol)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "NO_INFORMATION",	{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "SIP",			{ .i32 = 1 } };
			struct dict_avp_data data = {
				529,						/* Code */
				10415,						/* Vendor */
				"AF-Signalling-Protocol",			/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		};

		/* Application-Service-Provider-Identity */
		{
			struct dict_avp_data data = {
				532,						/* Code */
				10415,						/* Vendor */
				"Application-Service-Provider-Identity",	/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
		};

		/* Flow-Description */
		{
			struct dict_avp_data data = {
				507,						/* Code */
				10415,						/* Vendor */
				"Flow-Description",				/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_OCTETSTRING				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, IPFilterRule_type, NULL);
		};

		/* Flows */
		{
			struct dict_avp_data data = {
				510,						/* Code */
				10415,						/* Vendor */
				"Flows",					/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_GROUPED				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* Flow-Status */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Flow-Status)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "ENABLED-UPLINK",	{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "ENABLED-DOWNLINK",	{ .i32 = 1 } };
			struct dict_enumval_data t_2 = { "ENABLED",		{ .i32 = 2 } };
			struct dict_enumval_data t_3 = { "DISABLED",		{ .i32 = 3 } };
			struct dict_enumval_data t_4 = { "REMOVED",		{ .i32 = 4 } };
			struct dict_avp_data data = {
				511,						/* Code */
				10415,						/* Vendor */
				"Flow-Status",					/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_2, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_3, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_4, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		};

		/* Max-Requested-Bandwidth-UL */
		{
			struct dict_avp_data data = {
				516,						/* Code */
				10415,						/* Vendor */
				"Max-Requested-Bandwidth-UL",			/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* Max-Requested-Bandwidth-DL */
		{
			struct dict_avp_data data = {
				515,						/* Code */
				10415,						/* Vendor */
				"Max-Requested-Bandwidth-DL",			/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,		/* Fixed flag values */
				AVP_TYPE_UNSIGNED32				/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* Required-Access-Info */
		{
			struct dict_object *type;
			struct dict_type_data tdata = { AVP_TYPE_INTEGER32,	"Enumerated(Required-Access-Info)",	NULL,	NULL,	NULL };
			struct dict_enumval_data t_0 = { "USER_LOCATION",	{ .i32 = 0 } };
			struct dict_enumval_data t_1 = { "MS_TIME_ZONE",	{ .i32 = 1 } };
			struct dict_avp_data data = {
				536,						/* Code */
				10415,						/* Vendor */
				"Tariff-Change-Usage",				/* Name */
				AVP_FLAG_VENDOR | AVP_FLAG_MANDATORY,		/* Fixed flags */
				AVP_FLAG_VENDOR,				/* Fixed flag values */
				AVP_TYPE_INTEGER32				/* base type of data */
			};
			CHECK_dict_new(DICT_TYPE, &tdata, NULL, &type);
			CHECK_dict_new(DICT_ENUMVAL, &t_0, type, NULL);
			CHECK_dict_new(DICT_ENUMVAL, &t_1, type, NULL);
			CHECK_dict_new(DICT_AVP, &data, type, NULL);
		}

		/* Sponsor-Identity */
		{
			struct dict_avp_data data = {
				530,							/* Code */
				10415,							/* Vendor */
				"Sponsor-Identity",					/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,			/* Fixed flags */
				AVP_FLAG_VENDOR,					/* Fixed flag values */
				AVP_TYPE_GROUPED					/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* ETSI TS 283 034 */

		/* Logical-Access-ID */
		{
			struct dict_avp_data data = {
				302,							/* Code */
				13019,							/* Vendor */
				"Logical-Access-ID",					/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,			/* Fixed flags */
				AVP_FLAG_VENDOR,					/* Fixed flag values */
				AVP_TYPE_OCTETSTRING					/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* Physical-Access-ID */
		{
			struct dict_avp_data data = {
				313,							/* Code */
				13019,							/* Vendor */
				"Physical-Access-ID",					/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,			/* Fixed flags */
				AVP_FLAG_VENDOR,					/* Fixed flag values */
				AVP_TYPE_OCTETSTRING					/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, UTF8String_type, NULL);
		};

		/* 3GPP TS 29.272 */

		/* Trace-Data */
		{
			struct dict_avp_data data = {
				1458,							/* Code */
				10415,							/* Vendor */
				"Trace-Data",						/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,			/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,			/* Fixed flag values */
				AVP_TYPE_GROUPED					/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* Trace-Reference */
		{
			struct dict_avp_data data = {
				1459,							/* Code */
				10415,							/* Vendor */
				"Trace-Reference",					/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,			/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,			/* Fixed flag values */
				AVP_TYPE_OCTETSTRING					/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* 3GPP TS 32.299 */

		/* User-CSG-Information */
		{
			struct dict_avp_data data = {
				2319,							/* Code */
				10415,							/* Vendor */
				"User-CSG-Information",					/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,			/* Fixed flags */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,			/* Fixed flag values */
				AVP_TYPE_GROUPED					/* base type of data */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};

		/* 3GPP2 X.S0057-0 */

		/* 3GPP2-BSID */
		{
			struct dict_avp_data data = {
				9010,							/* Code */
				5535,							/* Vendor */
				"3GPP2-BSID",						/* Name */
				AVP_FLAG_VENDOR |AVP_FLAG_MANDATORY,			/* Fixed flags */
				AVP_FLAG_VENDOR,					/* Fixed flag values */
				AVP_TYPE_OCTETSTRING					/* base type of data */ /* XXX: guessed */
			};
			CHECK_dict_new(DICT_AVP, &data, NULL, NULL);
		};
	}

	/* Commands section */
	{
		/* CC-Request (CCR) Command */
		{
			struct dict_object *cmd;
			struct dict_cmd_data data = { 
				272,							/* Code */
				"CC-Request",						/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR,	/* Fixed flags */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE			/* Fixed flag values */
			};
			struct local_rules_definition rules[] = {
				{ "Session-Id",					RULE_FIXED_HEAD,	-1,	 1 },
				{ "Auth-Application-Id",			RULE_REQUIRED,		-1,	 1 },
				{ "Origin-Host",				RULE_REQUIRED,		-1,	 1 },
				{ "Origin-Realm",				RULE_REQUIRED,		-1,	 1 },
				{ "Destination-Realm",				RULE_REQUIRED,		-1,	 1 },
				{ "CC-Request-Type",				RULE_REQUIRED,		-1,	 1 },
				{ "CC-Request-Number",				RULE_REQUIRED,		-1,	 1 },
				{ "Destination-Host",				RULE_OPTIONAL,		-1,	 1 },
				{ "Origin-State-Id",				RULE_OPTIONAL,		-1,	 1 },
				{ "Subscription-Id",				RULE_OPTIONAL,		-1,	-1 },
				{ "Supported-Features",				RULE_OPTIONAL,		-1,	-1 },
				{ "TDF-Information",				RULE_OPTIONAL,		-1,	 1 },
				{ "Network-Request-Support",			RULE_OPTIONAL,		-1,	 1 },
				{ "Packet-Filter-Information",			RULE_OPTIONAL,		-1,	-1 },
				{ "Packet-Filter-Operation",			RULE_OPTIONAL,		-1,	 1 },
				{ "Bearer-Identifier",				RULE_OPTIONAL,		-1,	 1 },
				{ "Bearer-Operation",				RULE_OPTIONAL,		-1,	 1 },
				{ "Framed-IP-Address",				RULE_OPTIONAL,		-1,	 1 },
				{ "Framed-IPv6-Prefix",				RULE_OPTIONAL,		-1,	 1 },
				{ "IP-CAN-Type",				RULE_OPTIONAL,		-1,	 1 },
				{ "3GPP-RAT-Type",				RULE_OPTIONAL,		-1,	 1 },
				{ "RAT-Type",					RULE_OPTIONAL,		-1,	 1 },
				{ "Termination-Cause",				RULE_OPTIONAL,		-1,	 1 },
				{ "User-Equipment-Info",			RULE_OPTIONAL,		-1,	 1 },
				{ "QoS-Information",				RULE_OPTIONAL,		-1,	 1 },
				{ "QoS-Negotiation",				RULE_OPTIONAL,		-1,	 1 },
				{ "QoS-Upgrade",				RULE_OPTIONAL,		-1,	 1 },
				{ "Default-EPS-Bearer-QoS",			RULE_OPTIONAL,		-1,	 1 },
				{ "AN-GW-Address",				RULE_OPTIONAL,		-1,	 2 },
				{ "AN-GW-Status",				RULE_OPTIONAL,		-1,	 1 },
				{ "3GPP-SGSN-MCC-MNC",				RULE_OPTIONAL,		-1,	 1 },
				{ "3GPP-SGSN-Address",				RULE_OPTIONAL,		-1,	 1 },
				{ "3GPP-SGSN-IPv6-Address",			RULE_OPTIONAL,		-1,	 1 },
				{ "3GPP-GGSN-Address",				RULE_OPTIONAL,		-1,	 1 },
				{ "3GPP-GGSN-IPv6-Address",			RULE_OPTIONAL,		-1,	 1 },
				{ "RAI",					RULE_OPTIONAL,		-1,	 1 },
				{ "3GPP-User-Location-Info",			RULE_OPTIONAL,		-1,	 1 },
				{ "User-Location-Info-Time",			RULE_OPTIONAL,		-1,	 1 },
				{ "TWAN-Identifier",				RULE_OPTIONAL,		-1,	 1 },
				{ "3GPP-MS-TimeZone",				RULE_OPTIONAL,		-1,	 1 },
				{ "Called-Station-Id",				RULE_OPTIONAL,		-1,	 1 },
				{ "PDN-Connection-ID",				RULE_OPTIONAL,		-1,	 1 },
				{ "Bearer-Usage",				RULE_OPTIONAL,		-1,	 1 },
				{ "Online",					RULE_OPTIONAL,		-1,	 1 },
				{ "Offline",					RULE_OPTIONAL,		-1,	 1 },
				{ "TFT-Packet-Filter-Information",		RULE_OPTIONAL,		-1,	-1 },
				{ "Charging-Rule-Report",			RULE_OPTIONAL,		-1,	-1 },
				{ "Application-Detection-Information",		RULE_OPTIONAL,		-1,	-1 },
				{ "Event-Trigger",				RULE_OPTIONAL,		-1,	-1 },
				{ "Event-Report-Indication",			RULE_OPTIONAL,		-1,	 1 },
				{ "Access-Network-Charging-Address",		RULE_OPTIONAL,		-1,	 1 },
				{ "Access-Network-Charging-Identifier-Gx",	RULE_OPTIONAL,		-1,	-1 },
				{ "CoA-Information",				RULE_OPTIONAL,		-1,	-1 },
				{ "Usage-Monitoring-Information",		RULE_OPTIONAL,		-1,	-1 },
				{ "Routing-Rule-Install",			RULE_OPTIONAL,		-1,	 1 },
				{ "Routing-Rule-Remove",			RULE_OPTIONAL,		-1,	 1 },
				{ "HeNB-Local-IP-Address",			RULE_OPTIONAL,		-1,	 1 },
				{ "UE-Local-IP-Address",			RULE_OPTIONAL,		-1,	 1 },
				{ "UDP-Source-Port",				RULE_OPTIONAL,		-1,	 1 },
				{ "Logical-Access-ID",				RULE_OPTIONAL,		-1,	 1 },
				{ "Physical-Access-ID",				RULE_OPTIONAL,		-1,	 1 },
				{ "Proxy-Info",					RULE_OPTIONAL,		-1,	-1 },
				{ "Route-Record",				RULE_OPTIONAL,		-1,	-1 },
			};
			CHECK_dict_new(DICT_COMMAND, &data , gx, &cmd);
			PARSE_loc_rules(rules, cmd);
		}

		/* CC-Answer (CCA) Command */
		{
			struct dict_object *cmd;
			struct dict_cmd_data data = {
				272,					/* Code */
				"CC-Answer",				/* Name */
				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE,	/* Fixed flags */
				CMD_FLAG_PROXIABLE			/* Fixed flag values */
			};
			struct local_rules_definition rules[] = {
				{ "Session-Id",				RULE_FIXED_HEAD,	-1,	 1 },
				{ "Auth-Application-Id",		RULE_REQUIRED,		-1,	 1 },
				{ "Origin-Host",			RULE_REQUIRED,		-1,	 1 },
				{ "Origin-Realm",			RULE_REQUIRED,		-1,	 1 },
				{ "Result-Code",			RULE_OPTIONAL,		-1,	 1 },
				{ "Experimental-Result",		RULE_OPTIONAL,		-1,	 1 },
				{ "CC-Request-Type",			RULE_REQUIRED,		-1,	 1 },
				{ "CC-Request-Number",			RULE_REQUIRED,		-1,	 1 },
				{ "Supported-Features",			RULE_OPTIONAL,		-1,	-1 },
				{ "Bearer-Control-Mode",		RULE_OPTIONAL,		-1,	 1 },
				{ "Event-Trigger",			RULE_OPTIONAL,		-1,	-1 },
				{ "Event-Report-Indication",		RULE_OPTIONAL,		-1,	 1 },
				{ "Origin-State-Id",			RULE_OPTIONAL,		-1,	 1 },
				{ "Redirect-Host",			RULE_OPTIONAL,		-1,	-1 },
				{ "Redirect-Host-Usage",		RULE_OPTIONAL,		-1,	 1 },
				{ "Redirect-Max-Cache-Time",		RULE_OPTIONAL,		-1,	 1 },
				{ "Charging-Rule-Remove",		RULE_OPTIONAL,		-1,	-1 },
				{ "Charging-Rule-Install",		RULE_OPTIONAL,		-1,	-1 },
				{ "Charging-Information",		RULE_OPTIONAL,		-1,	 1 },
				{ "Online",				RULE_OPTIONAL,		-1,	 1 },
				{ "Offline",				RULE_OPTIONAL,		-1,	 1 },
				{ "QoS-Information",			RULE_OPTIONAL,		-1,	-1 },
				{ "Revalidation-Time",			RULE_OPTIONAL,		-1,	 1 },
				{ "Default-EPS-Bearer-QoS",		RULE_OPTIONAL,		-1,	 1 },
				{ "Bearer-Usage",			RULE_OPTIONAL,		-1,	 1 },
				{ "Usage-Monitoring-Information",	RULE_OPTIONAL,		-1,	-1 },
				{ "CSG-Information-Reporting",		RULE_OPTIONAL,		-1,	-1 },
				{ "User-CSG-Information",		RULE_OPTIONAL,		-1,	 1 },
				{ "Error-Message",			RULE_OPTIONAL,		-1,	 1 },
				{ "Error-Reporting-Host",		RULE_OPTIONAL,		-1,	 1 },
				{ "Failed-AVP",				RULE_OPTIONAL,		-1,	-1 },
				{ "Proxy-Info",				RULE_OPTIONAL,		-1,	-1 },
				{ "Route-Record",			RULE_OPTIONAL,		-1,	-1 },
			};
			CHECK_dict_new(DICT_COMMAND, &data, gx, &cmd);
			PARSE_loc_rules(rules, cmd);
		}

//		/* Re-Auth-Request */
//		{
//			struct dict_object *cmd;
//			struct dict_cmd_data data = {
//				258,							/* Code */
//				"Re-Auth-Request",					/* Name */
//				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE | CMD_FLAG_ERROR,	/* Fixed flags */
//				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE			/* Fixed flag values */
//			};
//			struct local_rules_definition rules[] = {
//				{ "Session-Id",				RULE_FIXED_HEAD,	-1,	 1 },
//				{ "Auth-Application-Id",		RULE_REQUIRED,		-1,	 1 },
//				{ "Origin-Host",			RULE_REQUIRED,		-1,	 1 },
//				{ "Origin-Realm",			RULE_REQUIRED,		-1,	 1 },
//				{ "Destination-Realm",			RULE_REQUIRED,		-1,	 1 },
//				{ "Destination-Host",			RULE_REQUIRED,		-1,	 1 },
//				{ "Re-Auth-Request-Type",		RULE_REQUIRED,		-1,	 1 },
//				{ "Session-Release-Cause",		RULE_OPTIONAL,		-1,	 1 },
//				{ "Origin-State-Id",			RULE_OPTIONAL,		-1,	 1 },
//				{ "Event-Trigger",			RULE_OPTIONAL,		-1,	-1 },
//				{ "Event-Report-Indication",		RULE_OPTIONAL,		-1,	 1 },
//				{ "Charging-Rule-Remove",		RULE_OPTIONAL,		-1,	-1 },
//				{ "Charging-Rule-Install",		RULE_OPTIONAL,		-1,	-1 },
//				{ "Default-EPS-Bearer-QoS",		RULE_OPTIONAL,		-1,	 1 },
//				{ "QoS-Information",			RULE_OPTIONAL,		-1,	-1 },
//				{ "Revalidation-Time",			RULE_OPTIONAL,		-1,	 1 },
//				{ "Usage-Monitoring-Information",	RULE_OPTIONAL,		-1,	-1 },
//				{ "Proxy-Info",				RULE_OPTIONAL,		-1,	-1 },
//				{ "Route-Record",			RULE_OPTIONAL,		-1,	-1 },
//			};
//			CHECK_dict_new(DICT_COMMAND, &data, gx, &cmd);
//			PARSE_loc_rules(rules, cmd);
//		}
//
//		/* Re-Auth-Answer */
//		{
//			struct dict_object *cmd;
//			struct dict_cmd_data data = {
//				258,					/* Code */
//				"Re-Auth-Answer",			/* Name */
//				CMD_FLAG_REQUEST | CMD_FLAG_PROXIABLE,	/* Fixed flags */
//				CMD_FLAG_PROXIABLE			/* Fixed flag values */
//			};
//			struct local_rules_definition rules[] = {
//				{ "Session-Id",			RULE_FIXED_HEAD,	-1,	 1 },
//				{ "Origin-Host",		RULE_REQUIRED,		-1,	 1 },
//				{ "Origin-Realm",		RULE_REQUIRED,		-1,	 1 },
//				{ "Result-Code",		RULE_OPTIONAL,		-1,	 1 },
//				{ "Experimental-Result",	RULE_OPTIONAL,		-1,	 1 },
//				{ "Origin-State-Id",		RULE_OPTIONAL,		-1,	 1 },
//				{ "IP-CAN-Type",		RULE_OPTIONAL,		-1,	 1 },
//				{ "RAT-Type",			RULE_OPTIONAL,		-1,	 1 },
//				{ "AN-GW-Address",		RULE_OPTIONAL,		-1,	 2 },
//				{ "3GPP-SGSN-MCC-MNC",		RULE_OPTIONAL,		-1,	 1 },
//				{ "3GPP-SGSN-Address",		RULE_OPTIONAL,		-1,	 1 },
//				{ "3GPP-SGSN-IPv6-Address",	RULE_OPTIONAL,		-1,	 1 },
//				{ "RAI",			RULE_OPTIONAL,		-1,	 1 },
//				{ "3GPP-User-Location-Info",	RULE_OPTIONAL,		-1,	 1 },
//				{ "User-Location-Info-Time",	RULE_OPTIONAL,		-1,	 1 },
//				{ "3GPP-MS-TimeZone",		RULE_OPTIONAL,		-1,	 1 },
//				{ "NetLoc-Access-Support",	RULE_OPTIONAL,		-1,	 1 },
//				{ "Charging-Rule-Report",	RULE_OPTIONAL,		-1,	-1 },
//				{ "Error-Message",		RULE_OPTIONAL,		-1,	 1 },
//				{ "Error-Reporting-Host",	RULE_OPTIONAL,		-1,	 1 },
//				{ "Failed-AVP",			RULE_OPTIONAL,		-1,	-1 },
//				{ "Proxy-Info",			RULE_OPTIONAL,		-1,	-1 },
//			};
//			CHECK_dict_new(DICT_COMMAND, &data, gx, &cmd);
//			PARSE_loc_rules(rules, cmd);
//		}
	}

	LOG_D( "Extension 'Dictionary definitions for Gx application' initialized" );
	return 0;
}

EXTENSION_ENTRY("dict_gx", dict_gx_entry);
