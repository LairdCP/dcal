#ifndef __buffer_h__
#define __buffer_h__

#include "flatcc/dcal_builder.h"
#include "flatcc/dcal_verifier.h"
#undef ns
#define ns(x) FLATBUFFERS_WRAP_NAMESPACE(DCAL_session, x)
#include "session.h"

flatbuffers_thash_t verify_buffer(const void * buf, const size_t size);
char * buftype_to_string(flatbuffers_thash_t buftype);

int handshake_error_code(ns(Handshake_table_t) handshake);

#endif // __buffer_h__
