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

#ifndef __lists_h__
#define __lists_h__

#ifndef STATIC_MEM
#include "platform.h"

typedef struct _list_element{
	struct _list_element *next;
	pvoid handle;
} list_element;

typedef struct _pointer_list{
	list_element *head;
	MUTEX *lock;
} pointer_list;

DCAL_ERR add_to_list( pointer_list **list, pvoid handle);
DCAL_ERR remove_from_list( pointer_list **list, pvoid handle);
bool validate_handle( pointer_list *list, pvoid handle);
DCAL_ERR initlist(pointer_list **list);
DCAL_ERR freelist(pointer_list **list);

#endif // STATIC_MEM
#endif // _lists_h_
