/*
 * =====================================================================================
 *
 *       Filename:  dlist.c
 *
 *    Description:  dlist.c
 *
 *        Version:  1.0
   *	Created:  07/02/2016 14:04:08
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Azalea
 *   Organization: 
 * =====================================================================================
 */

/*有头循环 双链表*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dlist.h"
#include <assert.h>

static int dlist_add(const void *pdata, size_t size,
                        struct dlist_info *info)
{
	assert(pdata != NULL);
	assert(info != NULL);

	struct  node_info *pnode = (struct node_info *)
		malloc(sizeof(struct node_info) + size);
	
	if (pnode == NULL) {
		perror("malloc");
		return -1;
	} 
	
	memcpy(pnode->buf, pdata, size);
	
	pnode->next = info->phead->next;
	pnode->prev = info->phead;
	
	info->phead->next = pnode;
	pnode->next->prev = pnode;

	return 0;
}

static int dlist_add_tail(const void *pdata, size_t size,
                        struct dlist_info *info)
{
	assert(pdata != NULL);
	assert(info != NULL);

	struct  node_info *pnode = (struct node_info *)
		malloc(sizeof(struct node_info) + size);
	
	if (pnode == NULL) {
		perror("malloc");
		return -1;
	} 
	
	memcpy(pnode->buf, pdata, size);
	
	pnode->next = info->phead;
	pnode->prev = info->phead->prev;
	
	pnode->prev->next = pnode;
	info->phead->prev = pnode;

	return 0;
}

static void dlist_for_each(struct dlist_info *info,
                  void (*todo)(struct node_info *pnode))
{
	struct  node_info *cur = NULL;
	
	for (cur = info->phead->next; cur != info->phead;
			cur = cur->next) {
		todo(cur);
	}
}

static void dlist_for_each_reverse(struct dlist_info *info,
                  void (*todo)(struct node_info *pnode))
{
	struct  node_info *cur = NULL;
	
	for (cur = info->phead->prev; cur != info->phead;
			cur = cur->prev) {
		todo(cur);
	}
}

static int dlist_del(struct node_info *pnode)
{
	if (pnode == NULL) {
		return -1;
	}

	pnode->next->prev = pnode->prev;
	pnode->prev->next = pnode->next;

	pnode->next = pnode;
	pnode->prev = pnode;

	free(pnode);
	
	return 0;
}

int  dlist_init(struct dlist_info *info)
{
	assert(info != NULL);
	
	info->phead = (struct node_info *)
			malloc(sizeof(struct node_info));
	if (info->phead == NULL) {
		fprintf(stderr, "Error:Out of memory\n");
		return -1;
	}

	info->phead->next = info->phead;
	info->phead->prev = info->phead;

	info->add = dlist_add;
	info->add_tail = dlist_add_tail;
	info->for_each = dlist_for_each;
	info->for_each_reverse = dlist_for_each_reverse;
	info->del = dlist_del;
	
	return 0;
}

int  dlist_destroy(struct dlist_info *info)
{
/* 释放链表中所有有效节点*/
#if 1
	while (info->phead->next != info->phead) {
		dlist_del(info->phead->next);
	}
#endif
/*释放链表的头结点*/
	free(info->phead);

	return 0;
}
