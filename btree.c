/*
 * =====================================================================================
 *
 *       Filename:  btree.c
 *
 *    Description:  i
 *
 *        Version:  1.0
   *	Created:  07/02/2016 11:20:08
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Azalea
 *   Organization: 
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "btree.h"

static void btree_add(struct bnode_info *pnode,
                        struct btree_info *info)
{
	assert(pnode != NULL);
	assert(info != NULL);

	bnode_init(pnode);

	//1. 空树
	if (info->root == NULL) {
		info->root = pnode;
	}else {
	//2. 非空树
		struct  bnode_info *cur = info->root;
		
		while (cur != NULL) {
			if (info->cmp(pnode, cur) < 0) {
				if (cur->lchild == NULL) {
					cur->lchild = pnode;
					pnode->parent = cur;
					break;
				} else {
					cur = cur->lchild;
				}
			} else {
				if (cur->rchild == NULL) {
					cur->rchild = pnode;
					pnode->parent = cur;
					break;
				} else {
					cur = cur->rchild;
				}
			}
		}		
	}
}


static   void __pre_order(struct bnode_info *pbnode,
                void (*todo)(struct bnode_info *pnode))
{
	if (pbnode != NULL) {
		todo(pbnode);
		__pre_order(pbnode->lchild, todo);
		__pre_order(pbnode->rchild, todo);
	}
}

static   void btree_pre_order(struct btree_info *info,
                void (*todo)(struct bnode_info *pnode))
{
	__pre_order(info->root, todo);
}


static   void __in_order(struct bnode_info *pbnode,
                void (*todo)(struct bnode_info *pnode))
{
	if (pbnode != NULL) {
		__in_order(pbnode->lchild, todo);
		todo(pbnode);
		__in_order(pbnode->rchild, todo);
	}
}

static   void btree_in_order(struct btree_info *info,
                void (*todo)(struct bnode_info *pnode))
{
	__in_order(info->root, todo);
}

static   void __post_order(struct bnode_info *pbnode,
                void (*todo)(struct bnode_info *pnode))
{
	if (pbnode != NULL) {
		__post_order(pbnode->lchild, todo);
		__post_order(pbnode->rchild, todo);
		todo(pbnode);
	}
}

static   void btree_post_order(struct btree_info *info,
                void (*todo)(struct bnode_info *pnode))
{
	__post_order(info->root, todo);
}

void btree_child_exchange(struct bnode_info *pnode)
{
	struct bnode_info *t = pnode->lchild;
	pnode->lchild = pnode->rchild;
	pnode->rchild = t;
}


void btree_mirror(struct btree_info *info)
{
	btree_pre_order(info, btree_child_exchange);
}

static   void btree_level_order(struct btree_info *info,
                void (*todo)(struct bnode_info *pnode))
{
	struct queue_info *pqueue = (struct queue_info *)
			malloc(sizeof(*pqueue));

	assert(pqueue != NULL);
	queue_init(pqueue);
	
	pqueue->push(pqueue, &info->root, sizeof(info->root));
	
	struct bnode_info *cur  = NULL;
	while (!pqueue->is_empty(pqueue)) {
		pqueue->pop(pqueue, &cur, sizeof(cur));
		todo(cur);

		if (cur->lchild != NULL) {
			pqueue->push(pqueue, &cur->lchild, sizeof(cur));
		}
		
		if (cur->rchild != NULL) {
			pqueue->push(pqueue, &cur->rchild, sizeof(cur));
		}
	}

	queue_destroy(pqueue);
	free(pqueue);
}

size_t __get_depth(struct bnode_info *pnode)
{
	if (pnode == NULL) {
		return 0;
	} else {
		size_t ldepth = __get_depth(pnode->lchild);
		size_t rdepth = __get_depth(pnode->rchild);
		
		if (ldepth < rdepth) {
			return rdepth + 1;
		} else {
			return ldepth + 1;
		}
	}
}

size_t btree_get_depth(struct btree_info *info)
{
	return __get_depth(info->root);
}

struct bnode_info *btree_find(const struct bnode_info *key, 
				struct btree_info *info)
{
	assert(key != NULL);
	assert(info != NULL);

	struct bnode_info *cur = info->root;
	int iret = 0;
	static  int count = 0;
	while (cur != NULL) {
		iret = info->cmp(key, cur);
		++count;
#ifdef _DEBUG
		printf("count = %d\n", count);
#endif
		if (iret == 0) {
			return cur;
		} else if (iret < 0) {
			cur = cur->lchild;
		} else {
			cur = cur->rchild;
		}
	}

	return NULL;
}
static struct bnode_info *__max(struct bnode_info *pnode)
{
	struct  bnode_info *cur = pnode;
	
	while (cur->rchild != NULL) {
		cur = cur->rchild;
	}
	
	return cur;
}

static struct bnode_info *__min(struct bnode_info *pnode)
{
	struct  bnode_info *cur = pnode;
	
	while (cur->lchild != NULL) {
		cur = cur->lchild;
	}
	
	return cur;
}


static void btree_del(struct bnode_info *pnode,
                        struct btree_info *info)
{
	/*用来指向被删除节点的父节点的lchild,rchild的二级指针*/
	struct bnode_info **pparent = NULL;
	
	if (pnode->parent == NULL) {
		pparent = &info->root;
	} else if (pnode->parent->lchild == pnode) {
		pparent = &pnode->parent->lchild;
	} else if (pnode->parent->rchild == pnode) {
		pparent = &pnode->parent->rchild;
	} else {
		fprintf(stderr, "Error:错误的树形关系\n");
		return ;
	}

	if (pnode->lchild == NULL && pnode->rchild == NULL) {
		//1. 叶子节点
		*pparent = NULL;
		pnode->parent = NULL;
	} else if (pnode->lchild != NULL && 
				pnode->rchild == NULL) {
		//2. 有左无右
		*pparent = pnode->lchild;
		pnode->lchild->parent = pnode->parent;
	} else if (pnode->lchild == NULL && 
			pnode->rchild != NULL) {
		//3. 有右无左
		*pparent = pnode->rchild;
		pnode->rchild->parent = pnode->parent;
	} else {
		//4. 左右均有
		/* 在右子树中找出最小节点*/
		struct bnode_info *min = __min(pnode->rchild);
		
		/* 判断是否是相邻节点*/
		if (min == pnode->rchild) {
			//从原有位置删除
			if (min->rchild != NULL) {
				min->rchild->parent = min->parent;
			}
			pnode->rchild = min->rchild;
		} else {
			/*先将其从原有位置删除*/
			if (min->rchild != NULL) {
				min->rchild->parent = min->parent;
			}
			min->parent->lchild = min->rchild;
		}
		/*相同类型的结构体变量可以直接赋值*/	
		*min = *pnode;
		
		min->lchild->parent = min;
		if (min->rchild != NULL) {
			min->rchild->parent = min;
		}
		*pparent = min;
	}
	bnode_init(pnode);
}


void btree_init(struct btree_info *info, 
		int (*cmp)(const struct bnode_info *pa, 
				const struct bnode_info *pb))
{
	assert(info != NULL);
	assert(cmp != NULL);	

	info->root = NULL;
	
	info->cmp = cmp;
	info->add = btree_add;
	info->del = btree_del;
	info->find = btree_find;
	info->pre_order = btree_pre_order;
	info->in_order = btree_in_order;
	info->post_order = btree_post_order;
	info->mirror = btree_mirror;
	info->level_order = btree_level_order;
	info->get_depth = btree_get_depth;
}


void btree_destroy(struct btree_info *info)
{
}
