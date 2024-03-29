#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "dcal_api.h"
#include "dcal_internal_api.h"
#include "profile.h"
#include "session.h"
#include "buffer.h"
#include "platform.h"
#include "common.h"

#ifdef STATIC_MEM

static internal_profile_struct static_profile = { 0 };

#else

#include "lists.h"
static pointer_list * profiles = NULL;

#endif

void __attribute__ ((constructor)) initprofiles(void)
{
	int rc;
	rc = initlist(&profiles);
	if (rc)
		DBGERROR("initlist() failed for profiles list with:%d\n", rc);
}

void __attribute__ ((destructor)) profiles_fini(void)
{
	int rc;
	rc = freelist(&profiles);
	profiles = NULL;
	if(rc)
		DBGERROR("freelist() failed for profiles list with: %d\n", rc);
}

static void clear_security_by_index(internal_profile_handle p, int i)
{
	assert((i>0)&&(i<6));
	switch(i){
		case 1: memset(p->security1,0,CRYPT_BUFFER_SIZE);
		case 2: memset(p->security2,0,CRYPT_BUFFER_SIZE);
		case 3: memset(p->security3,0,CRYPT_BUFFER_SIZE);
		case 4: memset(p->security4,0,CRYPT_BUFFER_SIZE);
		default:
		case 5: memset(p->security5,0,CRYPT_BUFFER_SIZE);
	}
}

#define clear_user(p) clear_security_by_index(p,1)
#define clear_password(p) clear_security_by_index(p,2)
#define clear_psk(p) clear_security_by_index(p,1)
#define clear_cacert(p) clear_security_by_index(p,3)
#define clear_pacfilename(p) clear_security_by_index(p,3)
#define clear_pacpassword(p) clear_security_by_index(p,4)
#define clear_usercert(p) clear_security_by_index(p,4)
#define clear_usercertpassword(p) clear_security_by_index(p,5)
#define clear_all(p) do {\
                         clear_security_by_index(p,1);\
                         clear_security_by_index(p,2);\
                         clear_security_by_index(p,3);\
                         clear_security_by_index(p,4);\
                         clear_security_by_index(p,5); } while (0)

#define set_user(p, str) clear_and_strncpy(p->security1, str, CRYPT_BUFFER_SIZE)
#define set_password(p, str) clear_and_strncpy(p->security2, str, CRYPT_BUFFER_SIZE)
#define set_psk(p, str) clear_and_strncpy(p->security1, str, CRYPT_BUFFER_SIZE)
#define set_cacert(p, str) clear_and_strncpy(p->security3, str, CRYPT_BUFFER_SIZE)
#define set_pacfilename(p, str) clear_and_strncpy(p->security3, str, CRYPT_BUFFER_SIZE)
#define set_pacpassword(p, str) clear_and_strncpy(p->security4, str, CRYPT_BUFFER_SIZE)
#define set_usercert(p, str) clear_and_strncpy(p->security4, str, CRYPT_BUFFER_SIZE)
#define set_usercertpassword(p, str) clear_and_strncpy(p->security5, str, CRYPT_BUFFER_SIZE)

#define copy_user(p, dest)  memcpy(dest, p->security1, CRYPT_BUFFER_SIZE)
#define copy_password(p, dest) memcpy(dest, p->security2, CRYPT_BUFFER_SIZE)
#define copy_psk(p, dest) memcpy(dest, p->security1, CRYPT_BUFFER_SIZE)
#define copy_cacert(p, dest) memcpy(dest, p->security3, CRYPT_BUFFER_SIZE)
#define copy_pacfilename(p, dest) memcpy(dest, p->security3, CRYPT_BUFFER_SIZE)
#define copy_pacpassword(p, dest) memcpy(dest, p->security4, CRYPT_BUFFER_SIZE)
#define copy_usercert(p, dest) memcpy(dest, p->security4, CRYPT_BUFFER_SIZE)
#define copy_usercertpassword(p, dest) memcpy(dest, p->security5, CRYPT_BUFFER_SIZE)

int dcal_wifi_profile_create( laird_profile_handle * profile)
{
	internal_profile_handle handle=NULL;
	int ret = DCAL_SUCCESS;

	REPORT_ENTRY_DEBUG;

	if (profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else {
	#if STATIC_MEM
		if(static_profile.valid)
			ret = DCAL_HANDLE_IN_USE;
		else
			handle = &static_profile;
			memset(handle, 0, sizeof(internal_profile_struct));
	#else // not STATIC_MEM
	#ifdef DEBUG
		if(validate_handle(profiles, profile))
			ret = DCAL_HANDLE_IN_USE;
		else
	#endif
		{
			handle = (internal_profile_handle) malloc(sizeof(internal_profile_struct));
			if (handle==NULL)
				ret = DCAL_NO_MEMORY;
			else {
				memset(handle, 0, sizeof(internal_profile_struct));
				ret = add_to_list(&profiles, handle);
				// profile defaults
				handle->radiomode = RADIOMODE_ABGN;
				handle->powersave = POWERSAVE_FAST;
				handle->pspdelay = DEFAULT_PSP_DELAY;
			}
		}
	#endif // STATIC_MEM
	}
	if (ret==DCAL_SUCCESS)
		*profile = handle;
	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_pull( laird_session_handle session,
                                 laird_profile_handle * profile,
                                 char * profilename)
{
	int ret = DCAL_SUCCESS;
	REPORT_ENTRY_DEBUG;

// must have a session, a profile pointer to receive the value,
// a string for the name, and the string cannot be a null string
	if ((profilename==NULL) || (profilename[0]==0))
		ret = DCAL_INVALID_PARAMETER;
	#ifdef DEBUG
	else if (validate_handle(profiles, profile))
		ret = DCAL_HANDLE_IN_USE;
	else if (!validate_session(session))
		return DCAL_INVALID_HANDLE;
	#endif
	else {
		internal_session_handle s = (internal_session_handle)session;
		// Attempt to retrieve profile from device
		flatcc_builder_t *B;
		char buffer[BUF_SZ] = {0};
		size_t size = BUF_SZ;
		flatbuffers_thash_t buftype;

		B = &s->builder;
		flatcc_builder_reset(B);
		flatbuffers_buffer_start(B, ns(Command_type_identifier));

		ns(Command_start(B));
		ns(Command_command_add(B, ns(Commands_GETPROFILE)));

		ns(Command_cmd_pl_Profile_start(B));
		ns(String_value_create_str(B, profilename));
		ns(Command_cmd_pl_Profile_end(B));

		ns(Command_end_as_root(B));

		size=flatcc_builder_get_buffer_size(B);
		assert(size<=BUF_SZ);
		flatcc_builder_copy_buffer(B, buffer, size);
		ret = lock_session_channel(session);
		if(ret)
			return REPORT_RETURN_DBG(ret);
		ret = dcal_send_buffer(session, buffer, size);

		if (ret != DCAL_SUCCESS) {
			unlock_session_channel(session);
			return REPORT_RETURN_DBG(ret);
		}

		//get response
		size=BUF_SZ;
		ret = dcal_read_buffer(session, buffer, &size);
		unlock_session_channel(session);

		if (ret != DCAL_SUCCESS)
			return REPORT_RETURN_DBG(ret);

		//is return buffer an ack buffer?
		buftype = verify_buffer(buffer, size);

		if(buftype != ns(Profile_type_hash)) {
			if(buftype != ns(Handshake_type_hash)){
				DBGERROR("could not verify handshake buffer.  Validated as: %s\n", buftype_to_string(buftype));
				return REPORT_RETURN_DBG(DCAL_FLATBUFF_ERROR);
			}

			ret =handshake_error_code(ns(Handshake_as_root(buffer)));

			DBGERROR("Failed to retrieve profile: %s.  Error received: %d\n",
			          profilename ,ret);
			return REPORT_RETURN_DBG(ret);
		}

		//if valid, get handle (ifdef for STATIC or not)
		dcal_wifi_profile_create( profile);


		assert(*profile);
		//copy data from buffer to handle
		internal_profile_handle p = (internal_profile_handle)*profile;

		ns(Profile_table_t) pt = ns(Profile_as_root(buffer));

		strncpy(p->profilename, ns(Profile_name(pt)), CONFIG_NAME_SZ);
		p->ssid.len = flatbuffers_uint8_vec_len(ns(Profile_ssid(pt)));
		assert(p->ssid.len <= SSID_SZ);
		memcpy(p->ssid.val, ns(Profile_ssid(pt)), p->ssid.len);
		strncpy(p->clientname, ns(Profile_client_name(pt)), CLIENT_NAME_SZ);
		p->txpower = ns(Profile_txPwr(pt));
		p->authtype = ns(Profile_auth(pt));
		p->eap = ns(Profile_eap(pt));
		p->pspdelay = ns(Profile_pspDelay(pt));
		p->powersave = ns(Profile_pwrsave(pt));
		p->weptype = ns(Profile_weptype(pt));
		switch(p->weptype){
			case WPA_PSK:
			case WPA2_PSK:
			case WPA_PSK_AES:
			case WPA2_PSK_TKIP:
			case WAPI_PSK:
				p->psk = true;
				break;
			default:
				p->psk = false;
				break;
		}
		switch(p->weptype){
			case WPA2_AES:
			case CCKM_AES:
			case WPA_PSK_AES:
			case WPA_AES:
			case WPA2_PSK:
				p->aes = true;
				break;
			default:
				p->aes = false;
				break;
		}

		p->bitrate = ns(Profile_bitrate(pt));
		p->radiomode = ns(Profile_radiomode(pt));
		// we don't actually send the encrypted data - only null or "1"
		if(flatbuffers_string_len(ns(Profile_security1(pt))))
			strncpy(p->security1, ns(Profile_security1(pt)), 1);
		if(flatbuffers_string_len(ns(Profile_security2(pt))))
			strncpy(p->security2, ns(Profile_security2(pt)), 1);
		if(flatbuffers_string_len(ns(Profile_security3(pt))))
			strncpy(p->security3, ns(Profile_security3(pt)), 1);
		if(flatbuffers_string_len(ns(Profile_security4(pt))))
			strncpy(p->security4, ns(Profile_security4(pt)), 1);
		if(flatbuffers_string_len(ns(Profile_security5(pt))))
			strncpy(p->security5, ns(Profile_security5(pt)), 1);

		p->txkey = ns(Profile_weptxkey(pt));
		p->autoprofile = ns(Profile_autoprofile(pt));
		#ifdef STATIC_MEM
		p->valid = true;
		#endif

	}
	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_close_handle( laird_profile_handle p)
{
	internal_profile_handle profile = (internal_profile_handle)p;
	int ret = DCAL_SUCCESS;

	REPORT_ENTRY_DEBUG;

	if(profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, p))
		ret = DCAL_INVALID_HANDLE;
	else {
		#ifdef STATIC_MEM
			((internal_profile_handle)profile)->valid = false;
		#else
			ret = remove_from_list(&profiles, profile);
			if (ret==DCAL_SUCCESS)
				profile = NULL;
		#endif
	}

	return REPORT_RETURN_DBG(ret);

}

//  push and profile_activate both send the local profile to the remote
//  radio device.  Activate_by_name only activates the named profile on
//  the remote radio, without sending a profile
int dcal_wifi_profile_push( laird_session_handle session,
                                 laird_profile_handle profile)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	internal_session_handle s = (internal_session_handle)session;
	REPORT_ENTRY_DEBUG;

	if (!validate_session(session))
		return DCAL_INVALID_HANDLE;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		flatcc_builder_t *B;
		char buffer[BUF_SZ] = {0};
		size_t size = BUF_SZ;
		flatbuffers_thash_t buftype;

		B = &s->builder;
		flatcc_builder_reset(B);
		flatbuffers_buffer_start(B, ns(Command_type_identifier));

		ns(Command_start(B));
		ns(Command_command_add(B, ns(Commands_SETPROFILE)));

		ns(Command_cmd_pl_Profile_start(B));
		ns(Profile_name_create_str(B, p->profilename));
		ns(Profile_ssid_create(B, (unsigned char *)p->ssid.val, p->ssid.len));
		ns(Profile_client_name_create_str(B, p->clientname));
		ns(Profile_txPwr_add(B, p->txpower));
		ns(Profile_pwrsave_add(B, p->powersave));
		ns(Profile_pspDelay_add(B, p->pspdelay));
		ns(Profile_weptype_add(B, p->weptype));
		ns(Profile_auth_add(B, p->authtype));
		ns(Profile_eap_add(B, p->eap));
		ns(Profile_bitrate_add(B, p->bitrate));
		ns(Profile_radiomode_add(B, p->radiomode));
		ns(Profile_weptxkey_add(B, p->txkey));
		ns(Profile_security1_create_str(B, p->security1));
		ns(Profile_security2_create_str(B, p->security2));
		ns(Profile_security3_create_str(B, p->security3));
		ns(Profile_security4_create_str(B, p->security4));
		ns(Profile_security5_create_str(B, p->security5));
		ns(Profile_autoprofile_add(B, p->autoprofile));
		ns(Command_cmd_pl_Profile_end(B));

		ns(Command_end_as_root(B));

		size=flatcc_builder_get_buffer_size(B);
		assert(size<=BUF_SZ);
		flatcc_builder_copy_buffer(B, buffer, size);
		ret = lock_session_channel(session);
		if(ret)
			return REPORT_RETURN_DBG(ret);
		ret = dcal_send_buffer(session, buffer, size);

		if (ret != DCAL_SUCCESS) {
			unlock_session_channel(session);
			return REPORT_RETURN_DBG(ret);
		}

		//get response
		size=BUF_SZ;
		ret = dcal_read_buffer(session, buffer, &size);
		unlock_session_channel(session);

		if (ret != DCAL_SUCCESS)
			return REPORT_RETURN_DBG(ret);

		//is return buffer an ack buffer?
			buftype = verify_buffer(buffer, size);

		if(buftype != ns(Handshake_type_hash)){
			DBGERROR("could not verify handshake buffer.  Validated as: %s\n", buftype_to_string(buftype));
			return REPORT_RETURN_DBG(DCAL_FLATBUFF_ERROR);
		}

		ret = handshake_error_code(ns(Handshake_as_root(buffer)));

	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_activate( laird_session_handle session,
                                     laird_profile_handle profile)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if (!validate_session(session))
		return DCAL_INVALID_HANDLE;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		ret = dcal_wifi_profile_push(session, profile);

		if (!ret)
			ret = dcal_wifi_profile_activate_by_name(session, p->profilename);
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_activate_by_name( laird_session_handle session,
                                          char * profile_name)
{
	int ret = DCAL_SUCCESS;
	internal_session_handle s = (internal_session_handle)session;
	REPORT_ENTRY_DEBUG;

	if ((profile_name==NULL) || (profile_name[0]==0))
		ret = DCAL_INVALID_PARAMETER;
	else if (!validate_session(session))
		return DCAL_INVALID_HANDLE;
	else {
		flatcc_builder_t *B;
		char buffer[BUF_SZ] = {0};
		size_t size = BUF_SZ;
		flatbuffers_thash_t buftype;

		B = &s->builder;
		flatcc_builder_reset(B);
		flatbuffers_buffer_start(B, ns(Command_type_identifier));

		ns(Command_start(B));
		ns(Command_command_add(B, ns(Commands_ACTIVATEPROFILE)));

		ns(Command_cmd_pl_Profile_start(B));
		ns(String_value_create_str(B, profile_name));
		ns(Command_cmd_pl_Profile_end(B));

		ns(Command_end_as_root(B));

		size=flatcc_builder_get_buffer_size(B);
		assert(size<=BUF_SZ);
		flatcc_builder_copy_buffer(B, buffer, size);
		ret = lock_session_channel(session);
		if(ret)
			return REPORT_RETURN_DBG(ret);
		ret = dcal_send_buffer(session, buffer, size);

		if (ret != DCAL_SUCCESS) {
			unlock_session_channel(session);
			return REPORT_RETURN_DBG(ret);
		}

		//get response
		size=BUF_SZ;
		ret = dcal_read_buffer(session, buffer, &size);
		unlock_session_channel(session);

		if (ret != DCAL_SUCCESS)
			return REPORT_RETURN_DBG(ret);

		//is return buffer an ack buffer?
			buftype = verify_buffer(buffer, size);

		if(buftype != ns(Handshake_type_hash)){
			DBGERROR("could not verify handshake buffer.  Validated as: %s\n", buftype_to_string(buftype));
			return REPORT_RETURN_DBG(DCAL_FLATBUFF_ERROR);
		}

		ret =handshake_error_code(ns(Handshake_as_root(buffer)));

	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_delete_from_device( laird_session_handle session,
                                          char * profile_name)
{
	int ret = DCAL_SUCCESS;
	internal_session_handle s = (internal_session_handle)session;
	REPORT_ENTRY_DEBUG;

	if ((profile_name==NULL) || (profile_name[0]==0))
		ret = DCAL_INVALID_PARAMETER;
	else if (!validate_session(session))
		return DCAL_INVALID_HANDLE;
	else {
		flatcc_builder_t *B;
		char buffer[BUF_SZ] = {0};
		size_t size = BUF_SZ;
		flatbuffers_thash_t buftype;

		B = &s->builder;
		flatcc_builder_reset(B);

		ns(Command_start(B));
		ns(Command_command_add(B, ns(Commands_DELPROFILE)));

		ns(Command_cmd_pl_Profile_start(B));
		ns(String_value_create_str(B, profile_name));
		ns(Command_cmd_pl_Profile_end(B));

		ns(Command_end_as_root(B));

		size=flatcc_builder_get_buffer_size(B);
		assert(size<=BUF_SZ);
		flatcc_builder_copy_buffer(B, buffer, size);
		ret = lock_session_channel(session);
		if(ret)
			return REPORT_RETURN_DBG(ret);
		ret = dcal_send_buffer(session, buffer, size);

		if (ret != DCAL_SUCCESS) {
			unlock_session_channel(session);
			return REPORT_RETURN_DBG(ret);
		}

		//get response
		size=BUF_SZ;
		ret = dcal_read_buffer(session, buffer, &size);
		unlock_session_channel(session);

		if (ret != DCAL_SUCCESS)
			return REPORT_RETURN_DBG(ret);

		//is return buffer an ack buffer?
			buftype = verify_buffer(buffer, size);

		if(buftype != ns(Handshake_type_hash)){
			DBGERROR("could not verify handshake buffer.  Validated as: %s\n", buftype_to_string(buftype));
			return REPORT_RETURN_DBG(DCAL_FLATBUFF_ERROR);
		}

		ret =handshake_error_code(ns(Handshake_as_root(buffer)));

	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_set_profilename(laird_profile_handle profile,
                                           char * profilename )
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (profilename==NULL) || (profilename[0]==0))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		clear_and_strncpy(p->profilename, profilename, CONFIG_NAME_SZ);
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_get_profilename(laird_profile_handle profile,
                                      char * profilename, size_t buflen )
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (profilename==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else if(strlen(p->profilename)+1 > buflen)
		ret = DCAL_BUFFER_TOO_SMALL;
	else
		clear_and_strncpy(profilename, p->profilename, buflen);

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_set_SSID( laird_profile_handle profile,
                                       LRD_WF_SSID *ssid)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (ssid==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		clear_and_strncpy((char*)&p->ssid, (char*)ssid, sizeof(LRD_WF_SSID));
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_get_SSID( laird_profile_handle profile,
                                       LRD_WF_SSID *ssid)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (ssid==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		clear_and_strncpy((char*)ssid, (char*)&p->ssid, sizeof(LRD_WF_SSID));
	}

	return REPORT_RETURN_DBG(ret);
}

/* weptype options available in the linux cli
'none' or 'WEP_OFF'
'wep' or 'WEP_ON'
'wep-eap' or 'WEP_AUTO'
'psk' or 'WPA_PSK'
'tkip' or 'WPA_TKIP'
'wpa2-psk' or 'WPA2_PSK'
'wpa2-aes' or 'WPA2_AES'
'cckm-tkip' or 'CCKM_TKIP'
'cckm-aes' or 'CCKM_AES'
'wpa-psk-aes' or 'WPA_PSK_AES'
'wpa-aes' or 'WPA_AES'
*/
int dcal_wifi_profile_set_encrypt_std( laird_profile_handle profile,
                                            ENCRYPT_STD estd)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (estd < ES_NONE) || (estd > ES_CCKM))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		switch(estd){
			case ES_WEP:
				if (p->eap==EAP_NONE)
					p->weptype = WEP_ON;
				else
					p->weptype = WEP_AUTO;
				break;
			case ES_WPA:
				if (p->psk)
					if (p->aes)
						p->weptype = WPA_PSK_AES;
					else
						p->weptype = WPA_PSK;
				else if(p->aes)
					p->weptype = WPA_AES;
				else
					p->weptype = WPA_TKIP;
				break;
			case ES_WPA2:
				if (p->psk)
					p->weptype = WPA2_PSK;
				else
					p->weptype = WPA2_AES;
				break;
			case ES_CCKM:
				if (!p->aes)
					p->weptype = CCKM_AES;
				else
					p->weptype = CCKM_TKIP;
				break;
			case ES_NONE:
			default:
				p->weptype = WEP_OFF; break;
		}
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_get_encrypt_std( laird_profile_handle profile,
                                            ENCRYPT_STD *estd)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (estd==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		switch(p->weptype){
			case WEP_ON:
			case WEP_AUTO:
				*estd = ES_WEP;
				break;
			case WPA_PSK:
			case WPA_TKIP:
			case WPA_PSK_AES:
			case WPA_AES:
				*estd = ES_WPA;
				break;
			case WPA2_PSK:
			case WPA2_AES:
				*estd = ES_WPA2;
				break;
			case CCKM_TKIP:
			case CCKM_AES:
				*estd = ES_CCKM;
				break;
			case WEP_OFF:
			default:
				*estd = ES_NONE;
		}
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_set_encryption( laird_profile_handle profile,
                                           ENCRYPTION enc)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (enc < ENC_NONE) || (enc > ENC_TKIP))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		switch(enc){
			case ENC_AES:
				switch(p->weptype){
					case WPA_PSK:
						p->weptype = WPA_PSK_AES;
						break;
					case WPA_TKIP:
						p->weptype = WPA_AES;
						break;
					case WPA2_PSK:
						p->weptype=WPA2_AES;
						p->psk = false;
						clear_psk(p);
						break;
					case WPA2_AES:
					case WPA_PSK_AES:
					case CCKM_AES:
					case WPA_AES:
						//no-op - already AES
						break;
					case CCKM_TKIP:
						p->weptype=WPA2_AES;
						break;
					default:
						ret = DCAL_INVALID_CONFIGURATION;
				}
				if (ret==DCAL_SUCCESS)
					p->aes = true;
			break;
			case ENC_TKIP:
				switch(p->weptype){
					case WPA_PSK:
					case WPA_PSK_AES:
						p->weptype = WPA_TKIP;
						p->psk = false;
						clear_psk(p);
					break;
					case CCKM_AES:
						p->weptype = CCKM_TKIP;
						break;
					case CCKM_TKIP:
					case WPA_TKIP:
						//no-op already TKIP
					break;
					default:
						ret = DCAL_INVALID_CONFIGURATION;
				}
				if (ret==DCAL_SUCCESS)
					p->aes=false;
			case ENC_NONE:
				p->aes=false;
				switch (p->weptype){
					case WPA_PSK:
					case WPA_PSK_AES:
						p->psk = false;
						clear_psk(p);
					case WPA_AES:  //dont need to clear PSK but same end target
						p->weptype = WPA_TKIP;
						break;
					case WPA2_PSK:
						p->weptype = WPA2_AES;
						p->psk = false;
						clear_psk(p);
						break;
					case CCKM_AES:
						p->weptype=CCKM_TKIP;
						break;
					default:
						break;  //no errors for other cases
				}
			break;
		}
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_get_encryption( laird_profile_handle profile,
                                           ENCRYPTION *enc)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (enc==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		switch(p->weptype){
			case WPA_TKIP:
			case CCKM_TKIP:
				*enc = ENC_TKIP;
				break;
			case WPA2_AES:
			case CCKM_AES:
			case WPA_AES:
			case WPA_PSK_AES:
			case WPA2_PSK:
				*enc = ENC_AES;
				break;
			default:
				*enc = ENC_NONE;
		}
	}

	return REPORT_RETURN_DBG(ret);
}


int dcal_wifi_profile_set_auth( laird_profile_handle profile,
                                     AUTH auth)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (auth < AUTH_OPEN) || (auth > AUTH_NETWORK_EAP))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		p->authtype = auth;
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_get_auth( laird_profile_handle profile,
                                     AUTH *auth)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (auth==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		*auth = p->authtype;
	}

	return REPORT_RETURN_DBG(ret);
}


int dcal_wifi_profile_set_eap( laird_profile_handle profile,
                                    EAPTYPE eap)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (eap < EAP_NONE) || (eap > EAP_WAPI_CERT))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		p->psk = false;
		if (p->weptype == WEP_ON)
			p->weptype = WEP_AUTO;
		if (p->eap != eap)
			clear_all(p);  // clear all security fields if changing eaptype
		p->eap = eap;
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_get_eap( laird_profile_handle profile,
                                    EAPTYPE *eap)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (eap==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		*eap = p->eap;
	}

	return REPORT_RETURN_DBG(ret);
}

// setting psk==NULL will clear PSK
int dcal_wifi_profile_set_psk( laird_profile_handle profile,
                                    char * psk)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if (profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		p->psk = true;  //even if the psk field is cleared, we want to mark the profile as psk
		switch(p->weptype) {
			case WPA2_PSK:
			case WPA2_AES:
				p->weptype = WPA2_PSK;
				break;
			default:
				p->weptype = p->aes?WPA_PSK_AES:WPA_PSK;
		}
		if (psk)
			set_psk(p, psk);
		else
			clear_psk(p);
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_psk_is_set( laird_profile_handle profile,
                                    bool * psk)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (psk==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		//assumes no null characters allowed in field
		*psk = p->security1[0]!=0;
	}

	return REPORT_RETURN_DBG(ret);
}

// setting user==NULL will cause it to be cleared
int dcal_wifi_profile_set_user( laird_profile_handle profile,
                                     char * user)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if (profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		if(user)
			set_user(p, user);
		else
			clear_user(p);
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_user_is_set( laird_profile_handle profile,
                                     bool * user)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (user==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		//assumes no null characters allowed in field
		*user=p->security1[0]!=0;
	}

	return REPORT_RETURN_DBG(ret);
}

// setting password==NULL will cause it to be cleared
int dcal_wifi_profile_set_password( laird_profile_handle profile,
                                         char * password)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if (profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		if(password)
			set_password(p, password);
		else
			clear_password(p);
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_password_is_set( laird_profile_handle profile,
                                         bool * password)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (password==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		//assumes no null characters allowed in field
		*password = p->security2[0]!=0;
	}

	return REPORT_RETURN_DBG(ret);
}

// setting cacert==NULL will cause it to be cleared
int dcal_wifi_profile_set_cacert( laird_profile_handle profile,
                                       char * cacert)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if (profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		if(cacert)
			set_cacert(p, cacert);
		else
			clear_cacert(p);
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_cacert_is_set( laird_profile_handle profile,
                                       bool * cacert)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (cacert==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		//assumes no null characters allowed in field
		*cacert = p->security3[0]!=0;
	}

	return REPORT_RETURN_DBG(ret);
}

// setting pacfilename==NULL will cause it to be cleared
int dcal_wifi_profile_set_pacfile( laird_profile_handle profile,
                                 char * pacfilename)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if (profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		if(pacfilename)
			set_pacfilename(p, pacfilename);
		else
			clear_pacfilename(p);
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_pacfile_is_set( laird_profile_handle profile,
                                 bool * pacfilename)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (pacfilename==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		//assumes no null characters allowed in field
		*pacfilename = p->security3[0]!=0;
	}

	return REPORT_RETURN_DBG(ret);
}

// setting pacpassword==NULL will cause it to be cleared
int dcal_wifi_profile_set_pacpassword( laird_profile_handle profile,
                                 char * pacpassword)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if (profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		if(pacpassword)
			set_pacpassword(p, pacpassword);
		else
			clear_pacpassword(p);
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_pacpassword_is_set( laird_profile_handle profile,
                                 bool * pacpassword)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (pacpassword==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		//assumes no null characters allowed in field
		*pacpassword = p->security4[0]!=0;
	}

	return REPORT_RETURN_DBG(ret);
}

//setting usercert==null will cause it to be cleared
int dcal_wifi_profile_set_usercert( laird_profile_handle profile,
                                 char * usercert)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if (profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		if(usercert)
			set_usercert(p, usercert);
		else
			clear_usercert(p);
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_usercert_is_set( laird_profile_handle profile,
                                 bool * usercert)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (usercert==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		//assumes no null characters allowed in field
		*usercert= p->security4[0]!=0;
	}

	return REPORT_RETURN_DBG(ret);
}

// setting usercert_password==NULL will cause it to be cleared.
int dcal_wifi_profile_set_usercert_password( laird_profile_handle profile,
                                 char * usercert_password)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if (profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		if(usercert_password)
			set_usercertpassword(p, usercert_password);
		else
			clear_usercertpassword(p);
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_usercert_password_is_set( laird_profile_handle profile,
                                 bool * usercert_password)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (usercert_password==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		//assumes no null characters allowed in field
		*usercert_password= p->security4[0]!=0;
	}

	return REPORT_RETURN_DBG(ret);
}

// setting wepkey==NULL will cause it to be cleared
int dcal_wifi_profile_set_wep_key( laird_profile_handle profile,
                                 char * wepkey, int index)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	char * dest;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (wepkey==NULL) || (index<1) || (index>4))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		if (p->weptype == WEP_AUTO){
			p->weptype = WEP_ON;
			p->eap = EAP_NONE;
		}
		if(wepkey){
			switch(index) {
				case 1: dest = (char*)&p->security1; break;
				case 2: dest = (char*)&p->security2; break;
				case 3: dest = (char*)&p->security3; break;
				default:
				case 4: dest = (char*)&p->security4; break;
			}
			clear_and_strncpy( dest, wepkey, CRYPT_BUFFER_SIZE);
		}
		else
			clear_security_by_index(p, index);
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_wep_key_is_set( laird_profile_handle profile,
                                 bool * wepkey, int index)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	char * src;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (wepkey==NULL) || (index <1) || (index>4))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		switch(index) {
			case 1: src = (char*)&p->security1; break;
			case 2: src = (char*)&p->security2; break;
			case 3: src = (char*)&p->security3; break;
			default:
			case 4: src = (char*)&p->security4; break;
		}
		*wepkey = (src[0]!=0);
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_set_wep_txkey( laird_profile_handle profile,
                                 unsigned int txkey)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if (profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		p->txkey = txkey;
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_get_wep_txkey( laird_profile_handle profile,
                                 unsigned int *txkey)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (txkey==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		*txkey = p->txkey;
	}

	return REPORT_RETURN_DBG(ret);
}

// sending clientname==NULL will clear the client name
int dcal_wifi_profile_set_clientname( laird_profile_handle profile,
                                char * clientname)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if (profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		if(clientname)
			clear_and_strncpy((char*)&p->clientname, clientname, CLIENT_NAME_SZ);
		else
			memset(p->clientname, 0, CLIENT_NAME_SZ);
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_get_clientname( laird_profile_handle profile,
                                char * clientname_buffer, size_t buflen)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (clientname_buffer==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else if(strlen(p->clientname)+1 > buflen)
		ret = DCAL_BUFFER_TOO_SMALL;
	else
		memcpy(clientname_buffer, p->clientname, buflen);

	return REPORT_RETURN_DBG(ret);
}


int dcal_wifi_profile_set_radiomode( laird_profile_handle profile,
                                RADIOMODE mode)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if (profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		p->radiomode = mode;
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_get_radiomode( laird_profile_handle profile,
                                RADIOMODE * mode)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (mode==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		*mode = p->radiomode;
	}

	return REPORT_RETURN_DBG(ret);
}


int dcal_wifi_profile_set_powersave( laird_profile_handle profile,
                               POWERSAVE powersave)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if (profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		p->powersave = powersave;
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_get_powersave( laird_profile_handle profile,
                               POWERSAVE * powersave)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (powersave==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		*powersave = p->powersave;
	}

	return REPORT_RETURN_DBG(ret);
}


int dcal_wifi_profile_set_psp_delay( laird_profile_handle profile,
                               unsigned int pspdelay)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if (profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		p->pspdelay = pspdelay;
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_get_psp_delay( laird_profile_handle profile,
                               unsigned int * pspdelay)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (pspdelay==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		*pspdelay = p->pspdelay;
	}

	return REPORT_RETURN_DBG(ret);
}


int dcal_wifi_profile_set_txpower( laird_profile_handle profile,
                                int txpower)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if (profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		p->txpower = txpower;
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_get_txpower( laird_profile_handle profile,
                                int *txpower)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (txpower==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		*txpower = p->txpower;
	}

	return REPORT_RETURN_DBG(ret);
}


int dcal_wifi_profile_set_bitrate( laird_profile_handle profile,
                               BITRATE bitrate)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if (profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		p->bitrate = bitrate;
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_get_bitrate( laird_profile_handle profile,
                               BITRATE *bitrate)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (bitrate==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		*bitrate = p->bitrate;
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_set_autoprofile( laird_profile_handle profile,
                               bool autoprofile)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if (profile==NULL)
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		p->autoprofile = autoprofile;
	}

	return REPORT_RETURN_DBG(ret);
}

int dcal_wifi_profile_get_autoprofile( laird_profile_handle profile,
                               bool *autoprofile)
{
	int ret = DCAL_SUCCESS;
	internal_profile_handle p = (internal_profile_handle)profile;
	REPORT_ENTRY_DEBUG;

	if ((profile==NULL) || (autoprofile==NULL))
		ret = DCAL_INVALID_PARAMETER;
	else if(!validate_handle(profiles, profile))
		ret = DCAL_INVALID_HANDLE;
	else {
		*autoprofile = p->autoprofile;
	}

	return REPORT_RETURN_DBG(ret);
}

// for use when debugging - may remove before publishing
void dcal_wifi_profile_printf( laird_profile_handle profile)
{
	internal_profile_handle p = (internal_profile_handle)profile;
	char * str=NULL;

	printf("Profile:\n");

	if (p==NULL) {
		printf("is null\n");
		return;
	}
	else if(!validate_handle(profiles, profile)) {
		printf("invalid profile handle\n");
		return;
	}
	#ifdef STATIC_MEM
		printf("\tvalid: %svalid\n",p->valid?"":"not ");
	#endif
	printf("\tprofilename: %s\n", p->profilename);
	printf("\tssid: %s\n", p->ssid.val);
	printf("\tclientname: %s\n", p->clientname);
	printf("\ttxpower: %d\n", p->txpower);
	printf("\tauthtype: %s\n", (p->authtype==AUTH_OPEN)?"open":(p->authtype==AUTH_SHARED)?"shared":"eap");

	switch(p->eap) {
		case EAP_NONE: str="none";break;
		case EAP_LEAP: str="leap";break;
		case EAP_EAPFAST: str= "eapfast"; break;
		case EAP_PEAPMSCHAP: str="peapmschap";break;
		case EAP_PEAPGTC: str="peapgtc"; break;
		case EAP_EAPTLS: str="eaptls"; break;
		case EAP_EAPTTLS: str="eapttls"; break;
		case EAP_PEAPTLS: str="peaptls"; break;
		case EAP_WAPI_CERT: str="wapi"; break;
		default: str="error";
	}
	printf("\teap: %s\n", str);

	printf("\taes: %s\n", p->aes?"true":"false");
	printf("\tpsk: %s\n", p->psk?"true":"false");
	printf("\ttxkey: %d\n", p->txkey);

	switch(p->powersave){
		case POWERSAVE_OFF: str="off";break;
		case POWERSAVE_MAX: str="max";break;
		case POWERSAVE_FAST: str="fast";break;
		default:
			str="error";
	}
	printf("\tpowersave: %s\n",str);

	printf("\tpspdelay: %d\n", p->pspdelay);

	switch(p->weptype) {
		case WEP_OFF: str="wep_off"; break;
		case WEP_ON: str="WEP_on"; break;
		case WEP_AUTO: str="WEP_auto"; break;
		case WPA_PSK: str="WPA_psk"; break;
		case WPA_TKIP: str="WPA_tkip"; break;
		case WPA2_PSK: str="wpa2_psk"; break;
		case WPA2_AES: str="wpa2_aes"; break;
		case CCKM_TKIP: str="cckm_tkip"; break;
		case WEP_CKIP: str="wep_ckip"; break;
		case WEP_AUTO_CKIP: str="wep_auto_ckip"; break;
		case CCKM_AES: str="cckm_aes"; break;
		case WPA_PSK_AES: str="wpa_psk_aes"; break;
		case WPA_AES: str="wpa_aes"; break;
		case WPA2_PSK_TKIP: str="wpa2_psk_tkip"; break;
		case WPA2_TKIP: str="wpa2_tkip"; break;
		case WAPI_PSK: str="wapi_psk"; break;
		case WAPI_CERT: str="wapi_cert"; break;
	default: str = "error";
	}
	printf("\tweptype: %s\n", str);

	printf("\tbitrate enum: %d\n", p->bitrate);

	switch(p->radiomode){
		case RADIOMODE_B_ONLY: str="B only"; break;
		case RADIOMODE_BG: str="BG"; break;
		case RADIOMODE_G_ONLY: str="G only"; break;
		case RADIOMODE_BG_LRS: str="BG_LRS"; break;
		case RADIOMODE_A_ONLY: str="A only"; break;
		case RADIOMODE_ABG: str="ABG"; break;
		case RADIOMODE_BGA: str="BGA"; break;
		case RADIOMODE_ADHOC: str="ADHOC"; break;
		case RADIOMODE_GN: str="GN"; break;
		case RADIOMODE_AN: str="AN"; break;
		case RADIOMODE_ABGN: str="ABGN"; break;
		case RADIOMODE_BGAN: str="BAGN"; break;
		case RADIOMODE_BGN: str="BGN"; break;
		default: str="error";
	}
	printf("\tradio mode: %s\n", str);

	printf("\tautoprofile: %s\n", p->autoprofile?"true":"false");

	printf("\tsecurity1: %s\n", p->security1);
	printf("\tsecurity2: %s\n", p->security2);
	printf("\tsecurity3: %s\n", p->security3);
	printf("\tsecurity4: %s\n", p->security4);
	printf("\tsecurity5: %s\n", p->security5);
}

int dcal_wifi_pull_scan_list(laird_session_handle session, size_t *count)
{
	int i, ret = DCAL_SUCCESS;
	internal_session_handle s = (internal_session_handle)session;
	REPORT_ENTRY_DEBUG;

	if (count==NULL)
		return DCAL_INVALID_PARAMETER;
	else if (!validate_session(session))
		return DCAL_INVALID_HANDLE;
	else {
		flatcc_builder_t *B;
		char buffer[BUF_SZ] = {0};
		size_t size = BUF_SZ;
		flatbuffers_thash_t buftype;

		B = &s->builder;
		flatcc_builder_reset(B);

		flatbuffers_buffer_start(B, ns(Command_type_identifier));
		ns(Command_start(B));
		ns(Command_command_add(B, ns(Commands_GETSCANLIST)));
		ns(Command_end_as_root(B));

		size=flatcc_builder_get_buffer_size(B);
		assert(size<=BUF_SZ);
		flatcc_builder_copy_buffer(B, buffer, size);
		ret = lock_session_channel(session);
		if(ret)
			return REPORT_RETURN_DBG(ret);
		ret = dcal_send_buffer(session, buffer, size);

		if (ret != DCAL_SUCCESS) {
			unlock_session_channel(session);
			return REPORT_RETURN_DBG(ret);
		}

		//get response
		size=BUF_SZ;
		ret = dcal_read_buffer(session, buffer, &size);
		unlock_session_channel(session);

		if (ret != DCAL_SUCCESS)
			return REPORT_RETURN_DBG(ret);

		//is return buffer an ack buffer?
		buftype = verify_buffer(buffer, size);

		if(buftype != ns(Scan_list_type_hash)){
			if(buftype != ns(Handshake_type_hash)){
				DBGERROR("could not verify handshake buffer.  Validated as: %s\n", buftype_to_string(buftype));
				return REPORT_RETURN_DBG(DCAL_FLATBUFF_ERROR);
			}

			ret =handshake_error_code(ns(Handshake_as_root(buffer)));
		}else{

			ns(Scan_list_table_t) sl = ns(Scan_list_as_root(buffer));

			size = ns(Scan_item_vec_len(ns(Scan_list_items(sl))));

			ns(Scan_item_table_t) si;

			void * list = malloc(size*sizeof(dcal_scan_item));
			if (list==NULL) {
				return DCAL_NO_MEMORY;
			}
			memset(list, 0, size*sizeof(dcal_scan_item));

			LOCK(s->list_lock);
			if (s->scan_items)
				free(s->scan_items);

			s->scan_items = list;
			s->num_scan_items = size;
			*count=size;
			for (i=0;i<size;i++){
				si = ns(Scan_item_vec_at(ns(Scan_list_items(sl)),i));
				s->scan_items[i].ssid.len = flatbuffers_uint8_vec_len(ns(Scan_item_ssid(si)));
				assert(s->scan_items[i].ssid.len <= SSID_SZ);
				memcpy(s->scan_items[i].ssid.val, ns(Scan_item_ssid(si)), s->scan_items[i].ssid.len);
				memcpy(s->scan_items[i].bssid, ns(Scan_item_mac(si)), LRD_WF_MAC_ADDR_LEN);
				s->scan_items[i].channel = ns(Scan_item_channel(si));
				s->scan_items[i].rssi = ns(Scan_item_rssi(si))/100;
				s->scan_items[i].securityMask = ns(Scan_item_securityMask(si));
				s->scan_items[i].bssType = ns(Scan_item_bss(si))?ADHOC:INFRASTRUCTURE;

			}

			time(&s->scan_list_timestamp);

			UNLOCK(s->list_lock);
		}
	}
	return ret;
}

#define SCAN_LIST_TIMEOUT 10
static int get_scan_list_entry_element(laird_session_handle session,
                                       int index,
                                       LRD_WF_SSID *ssid,
                                       unsigned char *bssid, int bssidbuflen,
                                       int *channel,
                                       int *rssi,
                                       int *securityMask,
                                       LRD_WF_BSSTYPE *bssType)
{
	int ret = DCAL_SUCCESS;
	internal_session_handle s = (internal_session_handle) session;
	time_t now;

	if (!validate_session(session))
		return DCAL_INVALID_HANDLE;

	time(&now);

	if (now- s->scan_list_timestamp > SCAN_LIST_TIMEOUT)
		return DCAL_DATA_STALE;

	LOCK(s->list_lock);

	if (index > s-> num_scan_items) {
		ret = DCAL_INDEX_OUT_OF_BOUNDS;
		goto cleanup;
	}

	if (ssid)
		memcpy(ssid, &s->scan_items[index].ssid, sizeof(LRD_WF_SSID));

	else if (bssid) {
		if (bssidbuflen < LRD_WF_MAC_ADDR_LEN){
			ret = DCAL_BUFFER_TOO_SMALL;
			goto cleanup;
		}
		memcpy(bssid, s->scan_items[index].bssid, LRD_WF_MAC_ADDR_LEN);
	}

	else if (channel) *channel = s->scan_items[index].channel;
	else if (rssi) *rssi = s->scan_items[index].rssi;
	else if (securityMask) *securityMask = s->scan_items[index].securityMask;
	else if (bssType) *bssType = s->scan_items[index].bssType;
	else
		ret = DCAL_INVALID_PARAMETER;

cleanup:
	UNLOCK(s->list_lock);

	return ret;
}

int dcal_wifi_get_scan_list_entry_ssid(laird_session_handle session,
                                  int index, LRD_WF_SSID *ssid)
{
	return get_scan_list_entry_element(session, index, ssid, NULL, 0, NULL, NULL, NULL, NULL);
}

int dcal_wifi_get_scan_list_entry_bssid(laird_session_handle session,
                                  int index, unsigned char * bssid, int bssidbuflen)
{
	return get_scan_list_entry_element(session, index, NULL, bssid, bssidbuflen, NULL, NULL, NULL, NULL);
}

int dcal_wifi_get_scan_list_entry_channel(laird_session_handle session,
                                  int index, int * channel)
{
	return get_scan_list_entry_element(session, index, NULL, NULL, 0, channel, NULL, NULL, NULL);
}

int dcal_wifi_get_scan_list_entry_rssi(laird_session_handle session,
                                  int index, int * rssi)
{
	return get_scan_list_entry_element(session, index, NULL, NULL, 0, NULL, rssi, NULL, NULL);
}

int dcal_wifi_get_scan_list_entry_securityMask(laird_session_handle session,
                                  int index, int * securityMask)
{
	return get_scan_list_entry_element(session, index, NULL, NULL, 0, NULL, NULL, securityMask, NULL);
}

int dcal_wifi_get_scan_list_entry_type(laird_session_handle session,
                                  int index, LRD_WF_BSSTYPE * bssType)
{
	return get_scan_list_entry_element(session, index, NULL, NULL, 0, NULL, NULL, NULL, bssType);
}

// WiFi Profile Management_
// both the create and pull functions will allocate a laird_profile_handle
// that require the close_handle function to be called when done with then
// handle
int dcal_wifi_pull_profile_list(laird_session_handle session, size_t *count)
{
	int i, ret = DCAL_SUCCESS;
	internal_session_handle s = (internal_session_handle)session;
	REPORT_ENTRY_DEBUG;

	if  (count==NULL)
		return DCAL_INVALID_PARAMETER;
	else if (!validate_session(session))
		return DCAL_INVALID_HANDLE;
	else {
		flatcc_builder_t *B;
		char buffer[BUF_SZ] = {0};
		size_t size = BUF_SZ;
		flatbuffers_thash_t buftype;

		B = &s->builder;
		flatcc_builder_reset(B);

		flatbuffers_buffer_start(B, ns(Command_type_identifier));
		ns(Command_start(B));
		ns(Command_command_add(B, ns(Commands_GETPROFILELIST)));
		ns(Command_end_as_root(B));

		size=flatcc_builder_get_buffer_size(B);
		assert(size<=BUF_SZ);
		flatcc_builder_copy_buffer(B, buffer, size);
		ret = lock_session_channel(session);
		if(ret)
			return REPORT_RETURN_DBG(ret);
		ret = dcal_send_buffer(session, buffer, size);

		if (ret != DCAL_SUCCESS) {
			unlock_session_channel(session);
			return REPORT_RETURN_DBG(ret);
		}

		//get response
		size=BUF_SZ;
		ret = dcal_read_buffer(session, buffer, &size);
		unlock_session_channel(session);

		if (ret != DCAL_SUCCESS)
			return REPORT_RETURN_DBG(ret);

		//is return buffer an ack buffer?
		buftype = verify_buffer(buffer, size);

		if(buftype != ns(Profile_list_type_hash)){
			if(buftype != ns(Handshake_type_hash)){
				DBGERROR("could not verify handshake buffer.  Validated as: %s\n", buftype_to_string(buftype));
				return REPORT_RETURN_DBG(DCAL_FLATBUFF_ERROR);
			}

			ret =handshake_error_code(ns(Handshake_as_root(buffer)));
		}else{

			ns(Profile_list_table_t) pl = ns(Profile_list_as_root(buffer));

			size = ns(P_entry_vec_len(ns(Profile_list_profiles(pl))));

			ns(P_entry_table_t) pe;

			void * list = malloc(size*sizeof(profile_list_item));
			if (list==NULL) {
				return DCAL_NO_MEMORY;
			}
			memset(list, 0, size*sizeof(profile_list_item));

			LOCK(s->list_lock);
			if (s->profiles)
				free(s->profiles);

			s->profiles = list;
			s->num_profiles = size;
			*count=size;
			for (i=0;i<size;i++){
				pe = ns(P_entry_vec_at(ns(Profile_list_profiles(pl)),i));
				strncpy(s->profiles[i].profile_name, ns(P_entry_name(pe)), CONFIG_NAME_SZ);
				s->profiles[i].autoprofile = (ns(P_entry_autoprof(pe)));
				s->profiles[i].active = (ns(P_entry_active(pe)));
			}

			time(&s->profile_list_timestamp);

			UNLOCK(s->list_lock);
		}
	}
	return ret;
}

#define PROFILE_LIST_TIMEOUT 60

static int dcal_wifi_get_profile_list_entry(laird_session_handle session, int index, char * profilename, size_t buflen, bool *autoprofile, bool * active)
{
	int ret = DCAL_SUCCESS;
	internal_session_handle s = (internal_session_handle) session;
	time_t now;

	if (!validate_session(session))
		return DCAL_INVALID_HANDLE;

	time(&now);

	if (now- s->profile_list_timestamp > PROFILE_LIST_TIMEOUT)
		return DCAL_DATA_STALE;

	LOCK(s->list_lock);

	if (index > s-> num_profiles) {
		ret = DCAL_INDEX_OUT_OF_BOUNDS;
		goto cleanup;
	}

	if ((profilename) && (buflen <= strlen(s->profiles[index].profile_name))){
		ret = DCAL_INVALID_PARAMETER;
		goto cleanup;
	} else
		strncpy(profilename, s->profiles[index].profile_name, buflen);
	if(autoprofile) *autoprofile = s->profiles[index].autoprofile;
	if(active) *active = s->profiles[index].active;

cleanup:
	UNLOCK(s->list_lock);

	return ret;
}

int dcal_wifi_get_profile_list_entry_profilename(laird_session_handle session, int index, char * profilename, size_t buflen)
{
	return dcal_wifi_get_profile_list_entry(session, index, profilename, buflen, NULL, NULL);
}

int dcal_wifi_get_profile_list_entry_autoprofile(laird_session_handle session, int index, bool *autoprofile)
{
	return dcal_wifi_get_profile_list_entry(session, index, NULL, 0, autoprofile, NULL);
}

int dcal_wifi_get_profile_list_entry_active(laird_session_handle session, int index, bool * active)
{
	return dcal_wifi_get_profile_list_entry(session, index, NULL, 0, NULL, active);
}
