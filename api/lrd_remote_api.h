/*
Copyright (c) 2016, Laird
Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

// This header file is for use in end users applications which utilize
// the Laird remote API to setup and get status from Laird workplace bridges

#ifndef _LRD_REMOTE_API_
#define _LRD_REMOTE_API_

#ifdef __cplusplus
extern "C" {
#endif

#include "sdc_sdk.h"

#define LRD_REMOTE_VERSION 0x01010101

typedef enum _LRD_API_ERR{
	LRD_API_SUCCESS = 0,
	LRD_API_INVALID_HANDLE,
	LRD_API_NO_NETWORK_ACCESS
} LRD_API_ERR;

#define TBD 20*4 // arbitrary size.  Will be specific once the internal structure elements are fully defined.
typedef char * FQDN;

typedef struct _larid_session_handle {
	uint32_t version;
	uint8_t data[TBD];
} laird_session_handle;

typedef struct _larid_profile_handle {
	uint32_t version;
	uint8_t data[TBD];
} laird_profile_handle;

typedef struct _laird_status_struct {
	uint32_t interesting_items[TBD];
} LRD_API_STATUS_STRUCT;

// API session management

LRD_API_ERR LRD_API_create_session( laird_session_handle * session);
LRD_API_ERR LRD_API_setip( laird_session_handle session, FQDN address );
LRD_API_ERR LRD_API_setkey( laird_session_handle session, char * keydata, char size);
LRD_API_ERR LRD_API_session_open ( laird_session_handle session );
LRD_API_ERR LRD_API_session_close( laird_session_handle session);

// Device Status

LRD_API_ERR LRD_API_DeviceStatus( laird_session_handle session, LRD_API_STATUS_STRUCT * status_struct);

// WiFi Management

// WiFi Profile Management
LRD_API_ERR LRD_API_PROFILE_Create(laird_session_handle * handle);
LRD_API_ERR LRD_API_PROFILE_Pull(laird_session_handle session, laird_profile_handle *handle, char *profileName);
LRD_API_ERR LRD_API_PROFILE_Push(laird_session_handle session, laird_profile_handle *handle);
LRD_API_ERR LRD_API_PROFILE_Set_profileName(laird_profile_handle profile, char * name);
LRD_API_ERR LRD_API_PROFILE_Get_profileName(laird_profile_handle profile, char * name);
LRD_API_ERR LRD_API_PROFILE_Set_SSID(laird_profile_handle profile, LRD_WF_SSID *ssid);
LRD_API_ERR LRD_API_PROFILE_Get_SSID(laird_profile_handle profile, LRD_WF_SSID *ssid);
LRD_API_ERR LRD_API_PROFILE_Set_txPower(laird_profile_handle profile, int txPower);
LRD_API_ERR LRD_API_PROFILE_Get_txPower(laird_profile_handle profile, int txPower);
LRD_API_ERR LRD_API_PROFILE_Set_authType(laird_profile_handle profile, AUTH auth);
LRD_API_ERR LRD_API_PROFILE_Get_authType(laird_profile_handle profile, AUTH auth);
LRD_API_ERR LRD_API_PROFILE_Set_eapType(laird_profile_handle profile, EAPTYPE eap);
LRD_API_ERR LRD_API_PROFILE_Get_eapType(laird_profile_handle profile, EAPTYPE eap);
LRD_API_ERR LRD_API_PROFILE_Set_powerSave(laird_profile_handle profile, POWERSAVE powersave);
LRD_API_ERR LRD_API_PROFILE_Get_powerSave(laird_profile_handle profile, POWERSAVE powersave);
LRD_API_ERR LRD_API_PROFILE_Set_pspDelay(laird_profile_handle profile, int pspdelay);
LRD_API_ERR LRD_API_PROFILE_Get_pspDelay(laird_profile_handle profile, int pspdelay);
LRD_API_ERR LRD_API_PROFILE_Set_wepType(laird_profile_handle profile, WEPTYPE wepType);
LRD_API_ERR LRD_API_PROFILE_Get_wepType(laird_profile_handle profile, WEPTYPE wepType);
LRD_API_ERR LRD_API_PROFILE_Set_bitRate(laird_profile_handle profile, BITRATE bitrate);
LRD_API_ERR LRD_API_PROFILE_Get_bitRate(laird_profile_handle profile, BITRATE bitrate);
LRD_API_ERR LRD_API_PROFILE_Set_radioMode(laird_profile_handle profile, RADIOMODE radiomode);
LRD_API_ERR LRD_API_PROFILE_Get_radioMode(laird_profile_handle profile, RADIOMODE radiomode);
LRD_API_ERR LRD_API_PROFILE_Set_username(laird_profile_handle profile, char * username, char * len);
LRD_API_ERR LRD_API_PROFILE_Get_username(laird_profile_handle profile, char * username, char * len);
LRD_API_ERR LRD_API_PROFILE_Set_userPwd(laird_profile_handle profile, char * userpwd, char * len);
LRD_API_ERR LRD_API_PROFILE_Get_userPwd(laird_profile_handle profile, char * userpwd, char * len);
LRD_API_ERR LRD_API_PROFILE_Set_PSK(laird_profile_handle profile, char * userpwd, char * len);
LRD_API_ERR LRD_API_PROFILE_Get_PSK(laird_profile_handle profile, char * userpwd, char * len);

// interesting stuff

#ifdef __cplusplus
}
#endif
#endif //_LRD_REMOTE_API
