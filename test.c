/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/02/2016 11:44:46
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Azalea, 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btree.h"
#include <assert.h>

struct  data_info {
	int data;
	struct bnode_info bnode;
};

int bnode_cmp(const struct bnode_info *pa, 
		const struct bnode_info *pb)
{
	struct data_info *_pa = container_of(pa, struct data_info, bnode);
	struct data_info *_pb = container_of(pb, struct data_info, bnode);
	
	return _pa->data - _pb->data;
}

void print_bnode(struct bnode_info *pnode)
{
	struct data_info *pa = container_of(pnode, struct data_info, bnode);
	printf("%d ", pa->data);
}


void find(struct btree_info *info, int key_data)
{
	struct data_info s;
	s.data = key_data;
	
	struct bnode_info *pentry = 
		info->find(&s.bnode, info);

	if (pentry == NULL) {
		fprintf(stderr, "Error:Not Found (%d).\n", key_data);
	} else {
		struct data_info *pa = container_of(pentry,
			struct data_info, bnode);
		printf("found: %d\n", pa->data);
	}

}


int main()
{
	struct data_info s[] = {
		//{6}, {5}, {2}, {0}, {7}, {1}, {8}, {4},
		{7}, {10}, {4}, {2}, {1}, {3}, {5}, {8}, {9}, {6}, {11},
	};
	
	struct btree_info *pbtree = (struct btree_info *)
			malloc(sizeof(*pbtree));

	assert(pbtree != NULL);

	btree_init(pbtree, bnode_cmp);

	int i;
	for (i = 0; i < sizeof s / sizeof s[0]; ++i) {
		pbtree->add(&s[i].bnode, pbtree);
	}
	printf("---pre_order---\n");
	pbtree->pre_order(pbtree, print_bnode);
	printf("\n");
	printf("---in_order---\n");
	pbtree->in_order(pbtree, print_bnode);
	printf("\n");
	printf("---post_order---\n");
	pbtree->post_order(pbtree, print_bnode);
	printf("\n");
	printf("---level_order---\n");
	pbtree->level_order(pbtree, print_bnode);
	printf("\n");
#if 0
	printf("btree_mirror----\n");
	pbtree->mirror(pbtree);
	printf("---pre_order---\n");
	pbtree->pre_order(pbtree, print_bnode);
	printf("\n");
#endif

	printf("depth: %lu\n", pbtree->get_depth(pbtree));
	find(pbtree, 1);
	printf("btree_del\n");
	//pbtree->del(&s[5].bnode, pbtree); //del 1
	//pbtree->del(&s[4].bnode, pbtree); //del 7
	//pbtree->del(&s[1].bnode, pbtree); //del 5
	//pbtree->del(&s[2].bnode, pbtree); //del 2
	pbtree->del(&s[0].bnode, pbtree); //del 0
	printf("---pre_order---\n");
	pbtree->pre_order(pbtree, print_bnode);
	printf("\n");
	btree_destroy(pbtree);
	free(pbtree);

	return 0;
}

