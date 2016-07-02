/*
 * =====================================================================================
 *
 *       Filename:  dlist.h
 *
 *    Description:  dlist.h
 *
 *        Version:  1.0
  *	Created:  07/02/2016 13:28:22
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Azalea
 *   Organization:  
 *
 * =====================================================================================
 */
#pragma once

/*
char a[0]: 零长数组，元素个数为0的数组
1. 通常用在结构体的尾成员
2. 其不占有成员大小(大小为0)
3. 主要是用来以后做空间的灵活扩展使用
4. 注意：零长数组是GCC特有的技术，如果考虑未来的可移植性可以考虑使用1个元素的数组，其会占用1字节的空间
*/
struct  node_info {
	struct  node_info *next;
	struct  node_info *prev;
	char buf[1]; // char buf[0]; char *pbuf;
};

struct  dlist_info {
	struct node_info *phead;
	int (*add)(const void *pdata, size_t size, 
			struct dlist_info *info);
	int (*add_tail)(const void *pdata, size_t size, 
			struct dlist_info *info);
	//正向遍历,todo 指向一个函数，该函数来处理每一个节点
	void (*for_each)(struct dlist_info *info, 
		void (*todo)(struct node_info *pnode));
	//反向遍历
	void (*for_each_reverse)(struct dlist_info *info, 
		void (*todo)(struct node_info *pnode));
	int (*del)(struct node_info *pnode);
};

/*构造和析构*/
int  dlist_init(struct dlist_info *info);
int  dlist_destroy(struct dlist_info *info);





