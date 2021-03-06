#ifndef __session_h__
#define __session_h__

#include <libssh/libssh.h>
#include <libssh/server.h>
#include <stdbool.h>
#include <time.h>
#include "flatcc/dcal_builder.h"
#include "dcal_api.h"
#include "platform.h"

#define HOST_SZ 256
#define USER_SZ 64

#ifndef CACHE_TIME
#define CACHE_TIME 10 //time pulled status data is valid in seconds - 0 means never expire
#endif

#define MAC_SZ 6
#define IP4_SZ 4
#define IP6_STR_SZ 46 //max string:0000:0000:0000:0000:0000:0000:xxx.xxx.xxx.xxx plus NULL (IPV4 mapped IPV6 address)
#define NAME_SZ 48
#define SSID_SZ 32
#define FILENAME_SZ 256

#define STR_SZ 80

typedef char ipv6_str_type[IP6_STR_SZ];

typedef struct _laird_status_struct {
	unsigned int cardState;
	char ProfileName[NAME_SZ];
	char ssid[SSID_SZ]; //32 characters.  Can contain non-ascii characters.  Not necessarily NULL terminated. Use ssid_len to access data.
	unsigned int ssid_len;
	unsigned int channel;
	int rssi;
	char clientName[NAME_SZ];
	unsigned char mac[MAC_SZ];
	unsigned char ipv4[IP4_SZ];
	ipv6_str_type *ipv6_strs;
	size_t num_ipv6_addr_strs;
	unsigned char ap_mac[MAC_SZ];
	unsigned char ap_ip[IP4_SZ];
	char ap_name[NAME_SZ];
	unsigned int bitRate;
	unsigned int txPower;
	unsigned int dtim;
	unsigned int beaconPeriod;
	time_t timestamp;
} DCAL_STATUS_STRUCT;

typedef struct _versions{
	bool valid;
	unsigned int sdk;
	RADIOCHIPSET chipset;
	LRD_SYSTEM sys;
	unsigned int driver;
	unsigned int dcas;
	unsigned int dcal;
	char firmware[STR_SZ];
	char supplicant[STR_SZ];
	char release[STR_SZ];
} dcal_versions;

typedef struct _scan_item {
	LRD_WF_SSID ssid;
	unsigned char bssid[LRD_WF_MAC_ADDR_LEN];
	int channel;
	int rssi;
	int securityMask;
	LRD_WF_BSSTYPE bssType;
} dcal_scan_item;

#ifndef CONFIG_NAME_SZ
#define CONFIG_NAME_SZ 33
#endif
typedef struct _profile_list_item{
	char profile_name[CONFIG_NAME_SZ];
	bool autoprofile;
	bool active;
} profile_list_item;

typedef struct _internal_session_handle {
	#ifdef STATIC_MEM
	bool valid;
	#endif
	dcal_versions versions;
	uint8_t state;
	ssh_session ssh;
	char host[HOST_SZ];
	char user[USER_SZ];
	char pw[USER_SZ];
	char keyfile[FILENAME_SZ];
	unsigned int port;
	ssh_channel channel;
	int verbosity;
	flatcc_builder_t builder;
	bool builder_init;
	DCAL_STATUS_STRUCT status;
	MUTEX *chan_lock;
	size_t num_profiles;
	profile_list_item *profiles;
	time_t profile_list_timestamp;
	size_t num_scan_items;
	dcal_scan_item * scan_items;
	time_t scan_list_timestamp;
	MUTEX *list_lock;
} internal_session_struct;
typedef internal_session_struct * internal_session_handle;

// session states
#define SESSION_INVALID 0
#define SESSION_ALLOCATED 1
#define SESSION_ERROR 2
#define SESSION_ACTIVE 3

#define DEF_PORT 2222
#define LAIRD_HELLO "HELLO DCAS"

// internal use only
int dcal_send_buffer(laird_session_handle s, void * buffer, size_t nbytes);
// internal use only
int dcal_read_buffer(laird_session_handle s, void * buffer, size_t *nbytes);

int validate_session(laird_session_handle s);
int lock_session_channel(laird_session_handle s);
int unlock_session_channel(laird_session_handle s);
#endif //__session_h__
