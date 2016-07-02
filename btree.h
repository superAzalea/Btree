/*
 * =====================================================================================
 *
 *       Filename:  btree.h
 *
 *    Description:  
 *
 *        Version:  1.0
   *	Created:  07/02/2016 11:13:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Azalea
 *   Organization: 
 * =====================================================================================
 */
#pragma once
#include "queue.h"

struct  bnode_info {
	struct bnode_info *parent;
	struct bnode_info *lchild;
	struct bnode_info *rchild;
};


struct   btree_info {
	struct bnode_info *root;
	int (*cmp)(const struct bnode_info *pa, 
			const struct bnode_info *pb);
	void (*add)(struct bnode_info *pnode, 
			struct btree_info *info);
	void (*del)(struct bnode_info *pnode, 
			struct btree_info *info);
	struct bnode_info *(*find)(const struct bnode_info *key, struct btree_info *info);
	void (*pre_order)(struct btree_info *info, 
		void (*todo)(struct bnode_info *pnode));
	void (*in_order)(struct btree_info *info, 
		void (*todo)(struct bnode_info *pnode));
	void (*post_order)(struct btree_info *info, 
		void (*todo)(struct bnode_info *pnode));
	void (*level_order)(struct btree_info *info, 
		void (*todo)(struct bnode_info *pnode));
	void (*mirror)(struct btree_info *info);
	size_t (*get_depth)(struct btree_info *info);
};

void btree_init(struct btree_info *info, 
			int (*cmp)(const struct bnode_info *pa,
				const struct bnode_info *pb));

void btree_destroy(struct btree_info *info);

static inline void  bnode_init(struct bnode_info *pnode)
{
	pnode->parent = NULL;
	pnode->lchild = NULL;
	pnode->rchild = NULL;
}


#define  offset_of(type, member) \
	((size_t)&(((type *)0)->member))

#define  container_of(ptr, type, member)	\
	((type *)((char *)ptr - offset_of(type, member)))


