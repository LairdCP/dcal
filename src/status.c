#include "buffer.h"
#include "debug.h"
#include "buffer.h"
#include "dcal_api.h"

#define BUF_SZ 2048

int build_query_status( flatcc_builder_t *B)
{

	flatcc_builder_reset(B);
	flatbuffers_buffer_start(B, ns(Command_type_identifier));

	ns(Command_start(B));
	ns(Command_command_add(B, ns(Commands_GETSTATUS)));
	ns(Command_end_as_root(B));

	return 0;
}

int dcal_device_status( laird_session_handle s, DCAL_STATUS_STRUCT * s_struct)
{
	int ret = DCAL_SUCCESS;
	char buffer[BUF_SZ];
	size_t i, size = 0;
	flatcc_builder_t *B;
	ns(Status_table_t) status = NULL;
	internal_session_handle session=NULL;
	flatbuffers_thash_t buftype;

	REPORT_ENTRY_DEBUG;

	if ((s==NULL) || (s_struct==NULL)){
		return REPORT_RETURN_DBG(DCAL_INVALID_PARAMETER);
	}

	session = s;
	if (!session->builder_init)
		return REPORT_RETURN_DBG(DCAL_FLATCC_NOT_INITIALIZED);

	B = &session->builder;

	size = BUF_SZ;
	memset(buffer, 0, BUF_SZ);
	build_query_status(B);

	size = flatcc_builder_get_buffer_size(B);
	assert(size <= BUF_SZ);
	flatcc_builder_copy_buffer(B, buffer, size);

	ret = dcal_send_buffer( session, buffer, size);

// get response
	size = BUF_SZ;
	ret = dcal_read_buffer( session, buffer, &size);

	if (ret != DCAL_SUCCESS)
		return REPORT_RETURN_DBG(ret);

//is return buffer a status buffer?
	buftype = verify_buffer(buffer, size);

	if(buftype == ns(Handshake_type_hash))
		return handshake_error_code(ns(Handshake_as_root(buffer)));

	if(buftype != ns(Status_type_hash)){
		DBGERROR("could not verify status buffer.  Validated as: %s\n", buftype_to_string(buftype));
		return (DCAL_FLATBUFF_ERROR);
	}

	status = ns(Status_as_root(buffer));

	memset(s_struct, 0, sizeof(DCAL_STATUS_STRUCT));
	s_struct->cardState = ns(Status_cardState(status));
	strncpy(s_struct->ProfileName, ns(Status_ProfileName(status)),NAME_SZ);
	s_struct->ssid_len =flatbuffers_uint8_vec_len(ns(Status_ssid(status)));
	if (s_struct->ssid_len > SSID_SZ)
		return DCAL_FLATBUFF_ERROR;
	memcpy(s_struct->ssid, ns(Status_ssid(status)), s_struct->ssid_len);
	s_struct->channel = ns(Status_channel(status));
	s_struct->rssi = ns(Status_rssi(status));
	strncpy(s_struct->clientName, ns(Status_clientName(status)), NAME_SZ);
	memcpy(s_struct->mac, ns(Status_mac(status)), MAC_SZ);
	memcpy(s_struct->ipv4, ns(Status_ip(status)), IP4_SZ);
	memcpy(s_struct->ap_mac, ns(Status_AP_mac(status)), MAC_SZ);
	memcpy(s_struct->ap_ip, ns(Status_AP_ip(status)), IP4_SZ);
	s_struct->bitRate = ns(Status_bitRate(status))/2;
	s_struct->txPower = ns(Status_txPower(status));
	s_struct->beaconPeriod = ns(Status_beaconPeriod(status));
	s_struct->dtim = ns(Status_dtim(status));

//TODO determine how to present a variable number of addresses
	flatbuffers_string_vec_t ipaddresses = ns(Status_ipv6(status));
	size_t num_ips = flatbuffers_string_vec_len(ipaddresses);
	for (i=0; ((i < num_ips) && i < 1); i++)
		strncpy(s_struct->ipv6, flatbuffers_string_vec_at(ipaddresses,i),IP6_STR_SZ);

	return REPORT_RETURN_DBG (ret);
}

