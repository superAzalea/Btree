/*
 * =====================================================================================
 *
 *       Filename:  queue.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/02/2016 10:31:20
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Azalea
 *   Organization:  
 *
 * =====================================================================================
 */
#pragma once
#include "dlist.h"

struct  queue_info {
	struct dlist_info *pdlist;
	int (*push)(struct queue_info *info, const void *pdata, size_t size);
	int (*top)(struct queue_info *info, void *pbuf, size_t size);
	int (*pop)(struct queue_info *info, void *pbuf, size_t size);
	int (*is_empty)(const struct queue_info *info);
};

int queue_init(struct queue_info *info);
int queue_destroy(struct queue_info *info);



