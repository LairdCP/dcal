#include "buffer.h"
#include "debug.h"
#include "buffer.h"
#include "handshake.h"
#include "dcal_api.h"

#define BUF_SZ 2048

int do_command( laird_session_handle s, unsigned long cmd)
{
	int ret = DCAL_SUCCESS;
	char buffer[BUF_SZ];
	size_t size = 0;
	flatcc_builder_t *B;
	internal_session_handle session=NULL;
	flatbuffers_thash_t buftype;

	REPORT_ENTRY_DEBUG;

	if (!validate_session(s))
		return REPORT_RETURN_DBG( DCAL_INVALID_HANDLE);

	session = s;
	if (!session->builder_init)
		return REPORT_RETURN_DBG(DCAL_FLATCC_NOT_INITIALIZED);

	B = &session->builder;

	size = BUF_SZ;
	memset(buffer, 0, BUF_SZ);

	flatcc_builder_reset(B);
	flatbuffers_buffer_start(B, ns(Command_type_identifier));

	ns(Command_start(B));
	ns(Command_command_add(B, cmd));
	ns(Command_end_as_root(B));

	size = flatcc_builder_get_buffer_size(B);
	assert(size <= BUF_SZ);
	flatcc_builder_copy_buffer(B, buffer, size);

	ret = lock_session_channel(session);
	if(ret)
		return REPORT_RETURN_DBG(ret);
	ret = dcal_send_buffer( session, buffer, size);

	if (ret != DCAL_SUCCESS) {
		unlock_session_channel(session);
		return REPORT_RETURN_DBG(ret);
	}
// get response
	size = BUF_SZ;
	ret = dcal_read_buffer( session, buffer, &size);
	unlock_session_channel(session);

	if (ret != DCAL_SUCCESS)
		return REPORT_RETURN_DBG(ret);

//is return buffer an ack buffer?
	buftype = verify_buffer(buffer, size);

	if(buftype != ns(Handshake_type_hash)){
		DBGERROR("could not verify ack buffer.  Validated as: %s\n", buftype_to_string(buftype));
		return REPORT_RETURN_DBG(DCAL_FLATBUFF_ERROR);
	}

	if (is_handshake_ack_valid(ns(Handshake_as_root(buffer))))
		ret = DCAL_FLATBUFF_ERROR;
	else
		ret = DCAL_SUCCESS;

	return REPORT_RETURN_DBG (ret);
}

int dcal_wifi_restart( laird_session_handle session)
{
	return do_command(session, ns(Commands_WIFIRESTART));
}

int dcal_system_restart( laird_session_handle session)
{
	int ret = do_command(session, ns(Commands_SYSTEMREBOOT));

	if (ret==DCAL_SUCCESS)
		ret = dcal_session_close(session);

	return ret;
}

