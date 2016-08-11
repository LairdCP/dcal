#ifndef DCAL_BUILDER_H
#define DCAL_BUILDER_H

/* Generated by flatcc 0.3.3 FlatBuffers schema compiler for C by dvide.com */

#ifndef DCAL_READER_H
#include "dcal_reader.h"
#endif
#ifndef FLATBUFFERS_COMMON_BUILDER_H
#include "flatbuffers_common_builder.h"
#endif
#define PDIAGNOSTIC_IGNORE_UNUSED
#include "flatcc/portable/pdiagnostic_push.h"
#ifndef flatbuffers_identifier
#define flatbuffers_identifier 0
#endif
#ifndef flatbuffers_extension
#define flatbuffers_extension ".bin"
#endif

#define __DCAL_session_Magic_formal_args , DCAL_session_Magic_enum_t v0
#define __DCAL_session_Magic_call_args , v0
__flatbuffers_build_scalar(flatbuffers_, DCAL_session_Magic, DCAL_session_Magic_enum_t)
#define __DCAL_session_Commands_formal_args , DCAL_session_Commands_enum_t v0
#define __DCAL_session_Commands_call_args , v0
__flatbuffers_build_scalar(flatbuffers_, DCAL_session_Commands, DCAL_session_Commands_enum_t)

typedef struct DCAL_session_Cmd_pl_union_ref DCAL_session_Cmd_pl_union_ref_t;

static const flatbuffers_voffset_t __DCAL_session_Handshake_required[] = {0 };
__flatbuffers_build_table(flatbuffers_, DCAL_session_Handshake, 5)
static const flatbuffers_voffset_t __DCAL_session_Event_required[] = {0 };
__flatbuffers_build_table(flatbuffers_, DCAL_session_Event, 5)
static const flatbuffers_voffset_t __DCAL_session_U32_required[] = {0 };
__flatbuffers_build_table(flatbuffers_, DCAL_session_U32, 1)
static const flatbuffers_voffset_t __DCAL_session_String_required[] = {0 };
__flatbuffers_build_table(flatbuffers_, DCAL_session_String, 1)
static const flatbuffers_voffset_t __DCAL_session_Status_required[] = {0 };
__flatbuffers_build_table(flatbuffers_, DCAL_session_Status, 17)
static const flatbuffers_voffset_t __DCAL_session_Version_required[] = {0 };
__flatbuffers_build_table(flatbuffers_, DCAL_session_Version, 8)
static const flatbuffers_voffset_t __DCAL_session_Globals_required[] = {0 };
__flatbuffers_build_table(flatbuffers_, DCAL_session_Globals, 24)
static const flatbuffers_voffset_t __DCAL_session_Profile_required[] = {0 };
__flatbuffers_build_table(flatbuffers_, DCAL_session_Profile, 18)
static const flatbuffers_voffset_t __DCAL_session_P_entry_required[] = {0 };
__flatbuffers_build_table(flatbuffers_, DCAL_session_P_entry, 3)
static const flatbuffers_voffset_t __DCAL_session_Profile_list_required[] = {0 };
__flatbuffers_build_table(flatbuffers_, DCAL_session_Profile_list, 1)
static const flatbuffers_voffset_t __DCAL_session_Time_required[] = {0 };
__flatbuffers_build_table(flatbuffers_, DCAL_session_Time, 2)
static const flatbuffers_voffset_t __DCAL_session_Filexfer_required[] = {0 };
__flatbuffers_build_table(flatbuffers_, DCAL_session_Filexfer, 3)
static const flatbuffers_voffset_t __DCAL_session_Command_required[] = {0 };
__flatbuffers_build_table(flatbuffers_, DCAL_session_Command, 3)
#define __DCAL_session_Handshake_formal_args ,\
  flatbuffers_bool_t v0, DCAL_session_Magic_enum_t v1, flatbuffers_string_ref_t v2, uint32_t v3, uint32_t v4
#define __DCAL_session_Handshake_call_args ,\
  v0, v1, v2, v3, v4
static inline DCAL_session_Handshake_ref_t DCAL_session_Handshake_create(flatbuffers_builder_t *B __DCAL_session_Handshake_formal_args);
#define __DCAL_session_Event_formal_args ,\
  uint32_t v0, uint32_t v1, uint32_t v2, flatbuffers_string_ref_t v3, uint16_t v4
#define __DCAL_session_Event_call_args ,\
  v0, v1, v2, v3, v4
static inline DCAL_session_Event_ref_t DCAL_session_Event_create(flatbuffers_builder_t *B __DCAL_session_Event_formal_args);
#define __DCAL_session_U32_formal_args , uint32_t v0
#define __DCAL_session_U32_call_args , v0
static inline DCAL_session_U32_ref_t DCAL_session_U32_create(flatbuffers_builder_t *B __DCAL_session_U32_formal_args);
#define __DCAL_session_String_formal_args , flatbuffers_string_ref_t v0
#define __DCAL_session_String_call_args , v0
static inline DCAL_session_String_ref_t DCAL_session_String_create(flatbuffers_builder_t *B __DCAL_session_String_formal_args);
#define __DCAL_session_Status_formal_args ,\
  uint32_t v0, flatbuffers_string_ref_t v1, flatbuffers_uint8_vec_ref_t v2, uint32_t v3,\
  int32_t v4, flatbuffers_string_ref_t v5, flatbuffers_uint8_vec_ref_t v6, flatbuffers_uint8_vec_ref_t v7,\
  flatbuffers_string_vec_ref_t v8, flatbuffers_uint8_vec_ref_t v9, flatbuffers_uint8_vec_ref_t v10, flatbuffers_string_ref_t v11,\
  uint32_t v12, uint32_t v13, uint32_t v14, uint32_t v15, uint32_t v16
#define __DCAL_session_Status_call_args ,\
  v0, v1, v2, v3,\
  v4, v5, v6, v7,\
  v8, v9, v10, v11,\
  v12, v13, v14, v15, v16
static inline DCAL_session_Status_ref_t DCAL_session_Status_create(flatbuffers_builder_t *B __DCAL_session_Status_formal_args);
#define __DCAL_session_Version_formal_args ,\
  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3,\
  flatbuffers_string_ref_t v4, flatbuffers_string_ref_t v5, flatbuffers_string_ref_t v6, uint32_t v7
#define __DCAL_session_Version_call_args ,\
  v0, v1, v2, v3,\
  v4, v5, v6, v7
static inline DCAL_session_Version_ref_t DCAL_session_Version_create(flatbuffers_builder_t *B __DCAL_session_Version_formal_args);
#define __DCAL_session_Globals_formal_args ,\
  uint32_t v0, uint32_t v1, uint32_t v2, flatbuffers_bool_t v3,\
  uint32_t v4, flatbuffers_bool_t v5, flatbuffers_bool_t v6, flatbuffers_string_ref_t v7,\
  flatbuffers_bool_t v8, uint32_t v9, flatbuffers_bool_t v10, uint32_t v11,\
  uint32_t v12, uint32_t v13, uint32_t v14, uint32_t v15,\
  uint32_t v16, uint32_t v17, uint32_t v18, uint32_t v19,\
  flatbuffers_bool_t v20, flatbuffers_bool_t v21, flatbuffers_bool_t v22, uint32_t v23
#define __DCAL_session_Globals_call_args ,\
  v0, v1, v2, v3,\
  v4, v5, v6, v7,\
  v8, v9, v10, v11,\
  v12, v13, v14, v15,\
  v16, v17, v18, v19,\
  v20, v21, v22, v23
static inline DCAL_session_Globals_ref_t DCAL_session_Globals_create(flatbuffers_builder_t *B __DCAL_session_Globals_formal_args);
#define __DCAL_session_Profile_formal_args ,\
  flatbuffers_string_ref_t v0, flatbuffers_uint8_vec_ref_t v1, flatbuffers_string_ref_t v2, uint32_t v3,\
  uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7,\
  uint32_t v8, uint32_t v9, uint32_t v10, flatbuffers_string_ref_t v11,\
  flatbuffers_string_ref_t v12, flatbuffers_string_ref_t v13, flatbuffers_string_ref_t v14, flatbuffers_string_ref_t v15, uint32_t v16, flatbuffers_bool_t v17
#define __DCAL_session_Profile_call_args ,\
  v0, v1, v2, v3,\
  v4, v5, v6, v7,\
  v8, v9, v10, v11,\
  v12, v13, v14, v15, v16, v17
static inline DCAL_session_Profile_ref_t DCAL_session_Profile_create(flatbuffers_builder_t *B __DCAL_session_Profile_formal_args);
#define __DCAL_session_P_entry_formal_args , flatbuffers_string_ref_t v0, flatbuffers_bool_t v1, flatbuffers_bool_t v2
#define __DCAL_session_P_entry_call_args , v0, v1, v2
static inline DCAL_session_P_entry_ref_t DCAL_session_P_entry_create(flatbuffers_builder_t *B __DCAL_session_P_entry_formal_args);
#define __DCAL_session_Profile_list_formal_args , DCAL_session_P_entry_vec_ref_t v0
#define __DCAL_session_Profile_list_call_args , v0
static inline DCAL_session_Profile_list_ref_t DCAL_session_Profile_list_create(flatbuffers_builder_t *B __DCAL_session_Profile_list_formal_args);
#define __DCAL_session_Time_formal_args , int32_t v0, int32_t v1
#define __DCAL_session_Time_call_args , v0, v1
static inline DCAL_session_Time_ref_t DCAL_session_Time_create(flatbuffers_builder_t *B __DCAL_session_Time_formal_args);
#define __DCAL_session_Filexfer_formal_args , flatbuffers_string_ref_t v0, int32_t v1, int32_t v2
#define __DCAL_session_Filexfer_call_args , v0, v1, v2
static inline DCAL_session_Filexfer_ref_t DCAL_session_Filexfer_create(flatbuffers_builder_t *B __DCAL_session_Filexfer_formal_args);
#define __DCAL_session_Command_formal_args , DCAL_session_Commands_enum_t v0, DCAL_session_Cmd_pl_union_ref_t v2
#define __DCAL_session_Command_call_args , v0, v2
static inline DCAL_session_Command_ref_t DCAL_session_Command_create(flatbuffers_builder_t *B __DCAL_session_Command_formal_args);

struct DCAL_session_Cmd_pl_union_ref {
    DCAL_session_Cmd_pl_union_type_t type;
    union {
        flatbuffers_ref_t _member;
        flatbuffers_ref_t NONE;
        DCAL_session_Globals_ref_t Globals;
        DCAL_session_Profile_ref_t Profile;
        DCAL_session_U32_ref_t U32;
        DCAL_session_String_ref_t String;
        DCAL_session_Time_ref_t Time;
        DCAL_session_Filexfer_ref_t Filexfer;
    };
};

static inline DCAL_session_Cmd_pl_union_ref_t DCAL_session_Cmd_pl_as_NONE()
{ DCAL_session_Cmd_pl_union_ref_t uref; uref.type = DCAL_session_Cmd_pl_NONE; uref._member = 0; return uref; }
static inline DCAL_session_Cmd_pl_union_ref_t DCAL_session_Cmd_pl_as_Globals(DCAL_session_Globals_ref_t ref)
{ DCAL_session_Cmd_pl_union_ref_t uref; uref.type = DCAL_session_Cmd_pl_Globals; uref.Globals = ref; return uref; }
static inline DCAL_session_Cmd_pl_union_ref_t DCAL_session_Cmd_pl_as_Profile(DCAL_session_Profile_ref_t ref)
{ DCAL_session_Cmd_pl_union_ref_t uref; uref.type = DCAL_session_Cmd_pl_Profile; uref.Profile = ref; return uref; }
static inline DCAL_session_Cmd_pl_union_ref_t DCAL_session_Cmd_pl_as_U32(DCAL_session_U32_ref_t ref)
{ DCAL_session_Cmd_pl_union_ref_t uref; uref.type = DCAL_session_Cmd_pl_U32; uref.U32 = ref; return uref; }
static inline DCAL_session_Cmd_pl_union_ref_t DCAL_session_Cmd_pl_as_String(DCAL_session_String_ref_t ref)
{ DCAL_session_Cmd_pl_union_ref_t uref; uref.type = DCAL_session_Cmd_pl_String; uref.String = ref; return uref; }
static inline DCAL_session_Cmd_pl_union_ref_t DCAL_session_Cmd_pl_as_Time(DCAL_session_Time_ref_t ref)
{ DCAL_session_Cmd_pl_union_ref_t uref; uref.type = DCAL_session_Cmd_pl_Time; uref.Time = ref; return uref; }
static inline DCAL_session_Cmd_pl_union_ref_t DCAL_session_Cmd_pl_as_Filexfer(DCAL_session_Filexfer_ref_t ref)
{ DCAL_session_Cmd_pl_union_ref_t uref; uref.type = DCAL_session_Cmd_pl_Filexfer; uref.Filexfer = ref; return uref; }

__flatbuffers_build_scalar_field(0, flatbuffers_, DCAL_session_Handshake_server, flatbuffers_bool, flatbuffers_bool_t, 1, 1, 0)
__flatbuffers_build_scalar_field(1, flatbuffers_, DCAL_session_Handshake_magic, DCAL_session_Magic, DCAL_session_Magic_enum_t, 4, 4, 0)
__flatbuffers_build_string_field(2, flatbuffers_, DCAL_session_Handshake_ip)
__flatbuffers_build_scalar_field(3, flatbuffers_, DCAL_session_Handshake_api_level, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(4, flatbuffers_, DCAL_session_Handshake_error, flatbuffers_uint32, uint32_t, 4, 4, 0)

static inline DCAL_session_Handshake_ref_t DCAL_session_Handshake_create(flatbuffers_builder_t *B __DCAL_session_Handshake_formal_args)
{
    if (DCAL_session_Handshake_start(B)
        || DCAL_session_Handshake_magic_add(B, v1)
        || DCAL_session_Handshake_ip_add(B, v2)
        || DCAL_session_Handshake_api_level_add(B, v3)
        || DCAL_session_Handshake_error_add(B, v4)
        || DCAL_session_Handshake_server_add(B, v0)) {
        return 0;
    }
    return DCAL_session_Handshake_end(B);
}
__flatbuffers_build_table_prolog(flatbuffers_, DCAL_session_Handshake, DCAL_session_Handshake_identifier, DCAL_session_Handshake_type_identifier)

__flatbuffers_build_scalar_field(0, flatbuffers_, DCAL_session_Event_e_type, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(1, flatbuffers_, DCAL_session_Event_status, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(2, flatbuffers_, DCAL_session_Event_reason, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_string_field(3, flatbuffers_, DCAL_session_Event_eth_addr)
__flatbuffers_build_scalar_field(4, flatbuffers_, DCAL_session_Event_flags, flatbuffers_uint16, uint16_t, 2, 2, 0)

static inline DCAL_session_Event_ref_t DCAL_session_Event_create(flatbuffers_builder_t *B __DCAL_session_Event_formal_args)
{
    if (DCAL_session_Event_start(B)
        || DCAL_session_Event_e_type_add(B, v0)
        || DCAL_session_Event_status_add(B, v1)
        || DCAL_session_Event_reason_add(B, v2)
        || DCAL_session_Event_eth_addr_add(B, v3)
        || DCAL_session_Event_flags_add(B, v4)) {
        return 0;
    }
    return DCAL_session_Event_end(B);
}
__flatbuffers_build_table_prolog(flatbuffers_, DCAL_session_Event, DCAL_session_Event_identifier, DCAL_session_Event_type_identifier)

__flatbuffers_build_scalar_field(0, flatbuffers_, DCAL_session_U32_value, flatbuffers_uint32, uint32_t, 4, 4, 0)

static inline DCAL_session_U32_ref_t DCAL_session_U32_create(flatbuffers_builder_t *B __DCAL_session_U32_formal_args)
{
    if (DCAL_session_U32_start(B)
        || DCAL_session_U32_value_add(B, v0)) {
        return 0;
    }
    return DCAL_session_U32_end(B);
}
__flatbuffers_build_table_prolog(flatbuffers_, DCAL_session_U32, DCAL_session_U32_identifier, DCAL_session_U32_type_identifier)

__flatbuffers_build_string_field(0, flatbuffers_, DCAL_session_String_value)

static inline DCAL_session_String_ref_t DCAL_session_String_create(flatbuffers_builder_t *B __DCAL_session_String_formal_args)
{
    if (DCAL_session_String_start(B)
        || DCAL_session_String_value_add(B, v0)) {
        return 0;
    }
    return DCAL_session_String_end(B);
}
__flatbuffers_build_table_prolog(flatbuffers_, DCAL_session_String, DCAL_session_String_identifier, DCAL_session_String_type_identifier)

__flatbuffers_build_scalar_field(0, flatbuffers_, DCAL_session_Status_cardState, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_string_field(1, flatbuffers_, DCAL_session_Status_ProfileName)
__flatbuffers_build_vector_field(2, flatbuffers_, DCAL_session_Status_ssid, flatbuffers_uint8, uint8_t)
__flatbuffers_build_scalar_field(3, flatbuffers_, DCAL_session_Status_channel, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(4, flatbuffers_, DCAL_session_Status_rssi, flatbuffers_int32, int32_t, 4, 4, 0)
__flatbuffers_build_string_field(5, flatbuffers_, DCAL_session_Status_clientName)
__flatbuffers_build_vector_field(6, flatbuffers_, DCAL_session_Status_mac, flatbuffers_uint8, uint8_t)
__flatbuffers_build_vector_field(7, flatbuffers_, DCAL_session_Status_ip, flatbuffers_uint8, uint8_t)
__flatbuffers_build_string_vector_field(8, flatbuffers_, DCAL_session_Status_ipv6)
__flatbuffers_build_vector_field(9, flatbuffers_, DCAL_session_Status_AP_mac, flatbuffers_uint8, uint8_t)
__flatbuffers_build_vector_field(10, flatbuffers_, DCAL_session_Status_AP_ip, flatbuffers_uint8, uint8_t)
__flatbuffers_build_string_field(11, flatbuffers_, DCAL_session_Status_AP_name)
__flatbuffers_build_scalar_field(12, flatbuffers_, DCAL_session_Status_bitRate, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(13, flatbuffers_, DCAL_session_Status_txPower, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(14, flatbuffers_, DCAL_session_Status_dtim, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(15, flatbuffers_, DCAL_session_Status_beaconPeriod, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(16, flatbuffers_, DCAL_session_Status_numipv6addrs, flatbuffers_uint32, uint32_t, 4, 4, 0)

static inline DCAL_session_Status_ref_t DCAL_session_Status_create(flatbuffers_builder_t *B __DCAL_session_Status_formal_args)
{
    if (DCAL_session_Status_start(B)
        || DCAL_session_Status_cardState_add(B, v0)
        || DCAL_session_Status_ProfileName_add(B, v1)
        || DCAL_session_Status_ssid_add(B, v2)
        || DCAL_session_Status_channel_add(B, v3)
        || DCAL_session_Status_rssi_add(B, v4)
        || DCAL_session_Status_clientName_add(B, v5)
        || DCAL_session_Status_mac_add(B, v6)
        || DCAL_session_Status_ip_add(B, v7)
        || DCAL_session_Status_ipv6_add(B, v8)
        || DCAL_session_Status_AP_mac_add(B, v9)
        || DCAL_session_Status_AP_ip_add(B, v10)
        || DCAL_session_Status_AP_name_add(B, v11)
        || DCAL_session_Status_bitRate_add(B, v12)
        || DCAL_session_Status_txPower_add(B, v13)
        || DCAL_session_Status_dtim_add(B, v14)
        || DCAL_session_Status_beaconPeriod_add(B, v15)
        || DCAL_session_Status_numipv6addrs_add(B, v16)) {
        return 0;
    }
    return DCAL_session_Status_end(B);
}
__flatbuffers_build_table_prolog(flatbuffers_, DCAL_session_Status, DCAL_session_Status_identifier, DCAL_session_Status_type_identifier)

__flatbuffers_build_scalar_field(0, flatbuffers_, DCAL_session_Version_sdk, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(1, flatbuffers_, DCAL_session_Version_chipset, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(2, flatbuffers_, DCAL_session_Version_sys, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(3, flatbuffers_, DCAL_session_Version_driver, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_string_field(4, flatbuffers_, DCAL_session_Version_firmware)
__flatbuffers_build_string_field(5, flatbuffers_, DCAL_session_Version_supplicant)
__flatbuffers_build_string_field(6, flatbuffers_, DCAL_session_Version_release)
__flatbuffers_build_scalar_field(7, flatbuffers_, DCAL_session_Version_dcas, flatbuffers_uint32, uint32_t, 4, 4, 0)

static inline DCAL_session_Version_ref_t DCAL_session_Version_create(flatbuffers_builder_t *B __DCAL_session_Version_formal_args)
{
    if (DCAL_session_Version_start(B)
        || DCAL_session_Version_sdk_add(B, v0)
        || DCAL_session_Version_chipset_add(B, v1)
        || DCAL_session_Version_sys_add(B, v2)
        || DCAL_session_Version_driver_add(B, v3)
        || DCAL_session_Version_firmware_add(B, v4)
        || DCAL_session_Version_supplicant_add(B, v5)
        || DCAL_session_Version_release_add(B, v6)
        || DCAL_session_Version_dcas_add(B, v7)) {
        return 0;
    }
    return DCAL_session_Version_end(B);
}
__flatbuffers_build_table_prolog(flatbuffers_, DCAL_session_Version, DCAL_session_Version_identifier, DCAL_session_Version_type_identifier)

__flatbuffers_build_scalar_field(0, flatbuffers_, DCAL_session_Globals_auth, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(1, flatbuffers_, DCAL_session_Globals_channel_set_a, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(2, flatbuffers_, DCAL_session_Globals_channel_set_b, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(3, flatbuffers_, DCAL_session_Globals_auto_profile, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(4, flatbuffers_, DCAL_session_Globals_beacon_miss, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(5, flatbuffers_, DCAL_session_Globals_bt_coex, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(6, flatbuffers_, DCAL_session_Globals_ccx, flatbuffers_bool, flatbuffers_bool_t, 1, 1, 0)
__flatbuffers_build_string_field(7, flatbuffers_, DCAL_session_Globals_cert_path)
__flatbuffers_build_scalar_field(8, flatbuffers_, DCAL_session_Globals_date_check, flatbuffers_bool, flatbuffers_bool_t, 1, 1, 0)
__flatbuffers_build_scalar_field(9, flatbuffers_, DCAL_session_Globals_def_adhoc, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(10, flatbuffers_, DCAL_session_Globals_fips, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(11, flatbuffers_, DCAL_session_Globals_pmk, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(12, flatbuffers_, DCAL_session_Globals_probe_delay, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(13, flatbuffers_, DCAL_session_Globals_regdomain, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(14, flatbuffers_, DCAL_session_Globals_roam_period, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(15, flatbuffers_, DCAL_session_Globals_roam_periodms, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(16, flatbuffers_, DCAL_session_Globals_roam_trigger, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(17, flatbuffers_, DCAL_session_Globals_rts, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(18, flatbuffers_, DCAL_session_Globals_scan_dfs, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(19, flatbuffers_, DCAL_session_Globals_ttls, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(20, flatbuffers_, DCAL_session_Globals_uapsd, flatbuffers_bool, flatbuffers_bool_t, 1, 1, 0)
__flatbuffers_build_scalar_field(21, flatbuffers_, DCAL_session_Globals_wmm, flatbuffers_bool, flatbuffers_bool_t, 1, 1, 0)
__flatbuffers_build_scalar_field(22, flatbuffers_, DCAL_session_Globals_ignore_null_ssid, flatbuffers_bool, flatbuffers_bool_t, 1, 1, 0)
__flatbuffers_build_scalar_field(23, flatbuffers_, DCAL_session_Globals_dfs_channels, flatbuffers_uint32, uint32_t, 4, 4, 0)

static inline DCAL_session_Globals_ref_t DCAL_session_Globals_create(flatbuffers_builder_t *B __DCAL_session_Globals_formal_args)
{
    if (DCAL_session_Globals_start(B)
        || DCAL_session_Globals_auth_add(B, v0)
        || DCAL_session_Globals_channel_set_a_add(B, v1)
        || DCAL_session_Globals_channel_set_b_add(B, v2)
        || DCAL_session_Globals_auto_profile_add(B, v3)
        || DCAL_session_Globals_beacon_miss_add(B, v4)
        || DCAL_session_Globals_bt_coex_add(B, v5)
        || DCAL_session_Globals_cert_path_add(B, v7)
        || DCAL_session_Globals_def_adhoc_add(B, v9)
        || DCAL_session_Globals_fips_add(B, v10)
        || DCAL_session_Globals_pmk_add(B, v11)
        || DCAL_session_Globals_probe_delay_add(B, v12)
        || DCAL_session_Globals_regdomain_add(B, v13)
        || DCAL_session_Globals_roam_period_add(B, v14)
        || DCAL_session_Globals_roam_periodms_add(B, v15)
        || DCAL_session_Globals_roam_trigger_add(B, v16)
        || DCAL_session_Globals_rts_add(B, v17)
        || DCAL_session_Globals_scan_dfs_add(B, v18)
        || DCAL_session_Globals_ttls_add(B, v19)
        || DCAL_session_Globals_dfs_channels_add(B, v23)
        || DCAL_session_Globals_auto_profile_add(B, v3)
        || DCAL_session_Globals_bt_coex_add(B, v5)
        || DCAL_session_Globals_ccx_add(B, v6)
        || DCAL_session_Globals_date_check_add(B, v8)
        || DCAL_session_Globals_fips_add(B, v10)
        || DCAL_session_Globals_uapsd_add(B, v20)
        || DCAL_session_Globals_wmm_add(B, v21)
        || DCAL_session_Globals_ignore_null_ssid_add(B, v22)) {
        return 0;
    }
    return DCAL_session_Globals_end(B);
}
__flatbuffers_build_table_prolog(flatbuffers_, DCAL_session_Globals, DCAL_session_Globals_identifier, DCAL_session_Globals_type_identifier)

__flatbuffers_build_string_field(0, flatbuffers_, DCAL_session_Profile_name)
__flatbuffers_build_vector_field(1, flatbuffers_, DCAL_session_Profile_ssid, flatbuffers_uint8, uint8_t)
__flatbuffers_build_string_field(2, flatbuffers_, DCAL_session_Profile_client_name)
__flatbuffers_build_scalar_field(3, flatbuffers_, DCAL_session_Profile_txPwr, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(4, flatbuffers_, DCAL_session_Profile_pwrsave, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(5, flatbuffers_, DCAL_session_Profile_pspDelay, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(6, flatbuffers_, DCAL_session_Profile_weptype, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(7, flatbuffers_, DCAL_session_Profile_auth, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(8, flatbuffers_, DCAL_session_Profile_eap, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(9, flatbuffers_, DCAL_session_Profile_bitrate, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(10, flatbuffers_, DCAL_session_Profile_radiomode, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_string_field(11, flatbuffers_, DCAL_session_Profile_security1)
__flatbuffers_build_string_field(12, flatbuffers_, DCAL_session_Profile_security2)
__flatbuffers_build_string_field(13, flatbuffers_, DCAL_session_Profile_security3)
__flatbuffers_build_string_field(14, flatbuffers_, DCAL_session_Profile_security4)
__flatbuffers_build_string_field(15, flatbuffers_, DCAL_session_Profile_security5)
__flatbuffers_build_scalar_field(16, flatbuffers_, DCAL_session_Profile_weptxkey, flatbuffers_uint32, uint32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(17, flatbuffers_, DCAL_session_Profile_autoprofile, flatbuffers_bool, flatbuffers_bool_t, 1, 1, 0)

static inline DCAL_session_Profile_ref_t DCAL_session_Profile_create(flatbuffers_builder_t *B __DCAL_session_Profile_formal_args)
{
    if (DCAL_session_Profile_start(B)
        || DCAL_session_Profile_name_add(B, v0)
        || DCAL_session_Profile_ssid_add(B, v1)
        || DCAL_session_Profile_client_name_add(B, v2)
        || DCAL_session_Profile_txPwr_add(B, v3)
        || DCAL_session_Profile_pwrsave_add(B, v4)
        || DCAL_session_Profile_pspDelay_add(B, v5)
        || DCAL_session_Profile_weptype_add(B, v6)
        || DCAL_session_Profile_auth_add(B, v7)
        || DCAL_session_Profile_eap_add(B, v8)
        || DCAL_session_Profile_bitrate_add(B, v9)
        || DCAL_session_Profile_radiomode_add(B, v10)
        || DCAL_session_Profile_security1_add(B, v11)
        || DCAL_session_Profile_security2_add(B, v12)
        || DCAL_session_Profile_security3_add(B, v13)
        || DCAL_session_Profile_security4_add(B, v14)
        || DCAL_session_Profile_security5_add(B, v15)
        || DCAL_session_Profile_weptxkey_add(B, v16)
        || DCAL_session_Profile_autoprofile_add(B, v17)) {
        return 0;
    }
    return DCAL_session_Profile_end(B);
}
__flatbuffers_build_table_prolog(flatbuffers_, DCAL_session_Profile, DCAL_session_Profile_identifier, DCAL_session_Profile_type_identifier)

__flatbuffers_build_string_field(0, flatbuffers_, DCAL_session_P_entry_name)
__flatbuffers_build_scalar_field(1, flatbuffers_, DCAL_session_P_entry_active, flatbuffers_bool, flatbuffers_bool_t, 1, 1, 0)
__flatbuffers_build_scalar_field(2, flatbuffers_, DCAL_session_P_entry_autoprof, flatbuffers_bool, flatbuffers_bool_t, 1, 1, 0)

static inline DCAL_session_P_entry_ref_t DCAL_session_P_entry_create(flatbuffers_builder_t *B __DCAL_session_P_entry_formal_args)
{
    if (DCAL_session_P_entry_start(B)
        || DCAL_session_P_entry_name_add(B, v0)
        || DCAL_session_P_entry_active_add(B, v1)
        || DCAL_session_P_entry_autoprof_add(B, v2)) {
        return 0;
    }
    return DCAL_session_P_entry_end(B);
}
__flatbuffers_build_table_prolog(flatbuffers_, DCAL_session_P_entry, DCAL_session_P_entry_identifier, DCAL_session_P_entry_type_identifier)

__flatbuffers_build_table_vector_field(0, flatbuffers_, DCAL_session_Profile_list_profiles, DCAL_session_P_entry)

static inline DCAL_session_Profile_list_ref_t DCAL_session_Profile_list_create(flatbuffers_builder_t *B __DCAL_session_Profile_list_formal_args)
{
    if (DCAL_session_Profile_list_start(B)
        || DCAL_session_Profile_list_profiles_add(B, v0)) {
        return 0;
    }
    return DCAL_session_Profile_list_end(B);
}
__flatbuffers_build_table_prolog(flatbuffers_, DCAL_session_Profile_list, DCAL_session_Profile_list_identifier, DCAL_session_Profile_list_type_identifier)

__flatbuffers_build_scalar_field(0, flatbuffers_, DCAL_session_Time_tv_sec, flatbuffers_int32, int32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(1, flatbuffers_, DCAL_session_Time_tv_usec, flatbuffers_int32, int32_t, 4, 4, 0)

static inline DCAL_session_Time_ref_t DCAL_session_Time_create(flatbuffers_builder_t *B __DCAL_session_Time_formal_args)
{
    if (DCAL_session_Time_start(B)
        || DCAL_session_Time_tv_sec_add(B, v0)
        || DCAL_session_Time_tv_usec_add(B, v1)) {
        return 0;
    }
    return DCAL_session_Time_end(B);
}
__flatbuffers_build_table_prolog(flatbuffers_, DCAL_session_Time, DCAL_session_Time_identifier, DCAL_session_Time_type_identifier)

__flatbuffers_build_string_field(0, flatbuffers_, DCAL_session_Filexfer_file_path)
__flatbuffers_build_scalar_field(1, flatbuffers_, DCAL_session_Filexfer_size, flatbuffers_int32, int32_t, 4, 4, 0)
__flatbuffers_build_scalar_field(2, flatbuffers_, DCAL_session_Filexfer_mode, flatbuffers_int32, int32_t, 4, 4, 0)

static inline DCAL_session_Filexfer_ref_t DCAL_session_Filexfer_create(flatbuffers_builder_t *B __DCAL_session_Filexfer_formal_args)
{
    if (DCAL_session_Filexfer_start(B)
        || DCAL_session_Filexfer_file_path_add(B, v0)
        || DCAL_session_Filexfer_size_add(B, v1)
        || DCAL_session_Filexfer_mode_add(B, v2)) {
        return 0;
    }
    return DCAL_session_Filexfer_end(B);
}
__flatbuffers_build_table_prolog(flatbuffers_, DCAL_session_Filexfer, DCAL_session_Filexfer_identifier, DCAL_session_Filexfer_type_identifier)

__flatbuffers_build_scalar_field(0, flatbuffers_, DCAL_session_Command_command, DCAL_session_Commands, DCAL_session_Commands_enum_t, 4, 4, 0)
__flatbuffers_build_union_field(2, flatbuffers_, DCAL_session_Command_cmd_pl, DCAL_session_Cmd_pl)
__flatbuffers_build_union_member_field(flatbuffers_, DCAL_session_Command_cmd_pl, DCAL_session_Cmd_pl, Globals, DCAL_session_Globals)
__flatbuffers_build_union_member_field(flatbuffers_, DCAL_session_Command_cmd_pl, DCAL_session_Cmd_pl, Profile, DCAL_session_Profile)
__flatbuffers_build_union_member_field(flatbuffers_, DCAL_session_Command_cmd_pl, DCAL_session_Cmd_pl, U32, DCAL_session_U32)
__flatbuffers_build_union_member_field(flatbuffers_, DCAL_session_Command_cmd_pl, DCAL_session_Cmd_pl, String, DCAL_session_String)
__flatbuffers_build_union_member_field(flatbuffers_, DCAL_session_Command_cmd_pl, DCAL_session_Cmd_pl, Time, DCAL_session_Time)
__flatbuffers_build_union_member_field(flatbuffers_, DCAL_session_Command_cmd_pl, DCAL_session_Cmd_pl, Filexfer, DCAL_session_Filexfer)

static inline DCAL_session_Command_ref_t DCAL_session_Command_create(flatbuffers_builder_t *B __DCAL_session_Command_formal_args)
{
    if (DCAL_session_Command_start(B)
        || DCAL_session_Command_command_add(B, v0)
        || DCAL_session_Command_cmd_pl_add_member(B, v2)
        || DCAL_session_Command_cmd_pl_add_type(B, v2.type)) {
        return 0;
    }
    return DCAL_session_Command_end(B);
}
__flatbuffers_build_table_prolog(flatbuffers_, DCAL_session_Command, DCAL_session_Command_identifier, DCAL_session_Command_type_identifier)

#include "flatcc/portable/pdiagnostic_pop.h"
#endif /* DCAL_BUILDER_H */
