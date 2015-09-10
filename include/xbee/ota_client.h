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
	@addtogroup xbee_ota_client
	@{
	@file xbee/ota_client.h

	Support code for over-the-air (OTA) firmware updates of application code
	on Programmable XBee target.
*/

#ifndef XBEE_OTA_CLIENT_H
#define XBEE_OTA_CLIENT_H

#include "xbee/platform.h"
#include "xbee/xmodem.h"
#include "wpan/aps.h"
#include "xbee/cbuf.h"
#include "xbee/transparent_serial.h"

#define XBEE_OTA_MAX_AUTH_LENGTH		64

/// Structure for tracking state of over-the-air update.
typedef struct xbee_ota_t {
	wpan_dev_t				*dev;		///< local device to send updates through
	addr64					target;	///< network device to update
	uint16_t					flags;	///< combination of XBEE_OTA_FLAG_* values
		/// Send data with APS encryption
		#define XBEE_OTA_FLAG_APS_ENCRYPT		0x0001

	union {
		xbee_cbuf_t				cbuf;	///< track state of circular buffer
		/// buffer for xbee_cbuf_t structure and bytes received from target
		uint8_t					raw[255 + XBEE_CBUF_OVERHEAD];
	} rxbuf;
	xbee_xmodem_state_t	xbxm;		///< track state of Xmodem transfer

	/// Payload used to initiate update
	uint8_t					auth_data[XBEE_OTA_MAX_AUTH_LENGTH];
	uint8_t					auth_length;		///< Number of bytes in \c auth_data
} xbee_ota_t;

/* START FUNCTION DESCRIPTION ********************************************
xbee_ota_init                           <ota_client.h>

SYNTAX:
   int xbee_ota_init( xbee_ota_t *ota,  wpan_dev_t *dev,  const addr64 *target)

DESCRIPTION:
     Initialize an xbee_ota_t structure to send firmware updates to target
     using dev.

     Calls xbee_xmodem_tx_init and xbee_xmodem_set_stream
     (therefore unnecessary to do so in main program).

     Note that when performing OTA updates, you MUST use XBEE_XMODEM_FLAG_64
     (64-byte xmodem blocks) if calling xbee_xmodem_tx_init directly.

     Sends frames to the target to have it start the update cycle.


PARAMETER1:  ota - state-tracking structure for sending update
PARAMETER2:  dev - device to use for sending update
PARAMETER3:  target - 64-bit address of target device


RETURNS:  0        - successfully initialized
          -EINVAL  - invalid parameter passed in

**************************************************************************/
int xbee_ota_init( xbee_ota_t *ota, wpan_dev_t *dev, const addr64 *target);

/* START _FUNCTION DESCRIPTION *******************************************
_xbee_ota_transparent_rx                <ota_client.h>

SYNTAX:
   int _xbee_ota_transparent_rx( const wpan_envelope_t FAR *envelope, 
                                 void FAR *context)

DESCRIPTION:

     Cluster handler for "Digi Transparent Serial" cluster.

     Used in the cluster
     table of #WPAN_ENDPOINT_DIGI_DATA (0xE8) for cluster DIGI_CLUST_SERIAL
     (0x0011).

     This is a preliminary API and WILL change in a future release.  This
     version is dedicated to processing responses in OTA (over-the-air)
     firmware updates and has not been generalized for other uses.


PARAMETER1:  envelope - information about the frame (addresses, endpoint,
              profile, cluster, etc.)
PARAMETER2:  context - pointer to xbee_ota_t structure


RETURNS:  0        - handled data
          !0	some sort of error processing data



**************************************************************************/
int _xbee_ota_transparent_rx( const wpan_envelope_t FAR *envelope,
	void FAR *context);

/* START FUNCTION DESCRIPTION ********************************************
XBEE_OTA_DATA_CLIENT_CLUST_ENTRY        <ota_client.h>

MACRO SYNTAX:
     XBEE_OTA_DATA_CLIENT_CLUST_ENTRY( ota,  flags)

DESCRIPTION:
     Macro for adding the OTA receive cluster (Digi Transparent Serial) to
     the cluster list for WPAN_ENDPOINT_DIGI_DATA.


PARAMETER1:  ota - pointer to an xbee_ota_t structure for tracking update
              state
PARAMETER2:  flags - additional flags for the cluster; typically 0 or
              #WPAN_CLUST_FLAG_ENCRYPT

**************************************************************************/
#define XBEE_OTA_DATA_CLIENT_CLUST_ENTRY(ota, flags)	\
	{ DIGI_CLUST_SERIAL, _xbee_ota_transparent_rx, ota, 		\
		WPAN_CLUST_FLAG_INOUT | WPAN_CLUST_FLAG_NOT_ZCL | flags }

// client cluster used to kick off OTA updates
// flag should be WPAN_CLUST_FLAG_NONE or WPAN_CLUST_FLAG_ENCRYPT
#define XBEE_OTA_CMD_CLIENT_CLUST_ENTRY(handler, context, flag)		\
	{	DIGI_CLUST_PROG_XBEE_OTA_UPD, handler,									\
		context, (flag) | WPAN_CLUST_FLAG_CLIENT | WPAN_CLUST_FLAG_NOT_ZCL }

#endif		// XBEE_OTA_CLIENT_H defined
