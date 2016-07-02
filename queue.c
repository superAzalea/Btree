/*
 * =====================================================================================
 *
 *       Filename:  queue.c
 *
 *    Description:  
 *
 *        Version:  1.0
  *	Created:  07/02/2016 10:35:48
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Azalea
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"

static int queue_push(struct queue_info *info, 
		const void *pdata, size_t size)
{
	assert(info != NULL);
	assert(pdata != NULL);

	return info->pdlist->add_tail(pdata, size, info->pdlist);
}

static int queue_is_empty(const struct queue_info *info)
{
	return info->pdlist->phead->next == info->pdlist->phead;
}

static int queue_top(struct queue_info *info,
		void *pbuf, size_t size)
{
	assert(info != NULL);
	assert(pbuf != NULL);
	
	if (queue_is_empty(info)) {
		fprintf(stderr, "queue top failed (queue is empty).\n");
		return -1;
	}

	memcpy(pbuf, info->pdlist->phead->next->buf, size);
	return 0;	
}


static int queue_pop(struct queue_info *info,
		void *pbuf, size_t size)
{
	if (queue_top(info, pbuf, size) < 0) {
		fprintf(stderr, "queue pop failed.\n");
		return -1;
	}
	
	return	info->pdlist->del(info->pdlist->phead->next);

}

int queue_init(struct queue_info *info)
{
	assert(info != NULL);
	
	info->pdlist = (struct dlist_info *)
		malloc(sizeof(struct dlist_info));
	
	if (info->pdlist == NULL) {
		perror("malloc");
		goto err;
	}

	dlist_init(info->pdlist);

	info->push = queue_push;
	info->top = queue_top;
	info->pop = queue_pop;
	info->is_empty = queue_is_empty;

	return 0;

err:
	return -1;
}

int queue_destroy(struct queue_info *info)
{
	dlist_destroy(info->pdlist);
	free(info->pdlist);

	return 0;
}
