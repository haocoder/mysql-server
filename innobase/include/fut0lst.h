/**********************************************************************
File-based list utilities

(c) 1995 Innobase Oy

Created 11/28/1995 Heikki Tuuri
***********************************************************************/

#ifndef fut0lst_h
#define fut0lst_h

#include "univ.i"

#include "fil0fil.h"
#include "mtr0mtr.h"

/*
 * 磁盘双链表： 用于组织磁盘文件数据
 * 由于磁盘数据块设备无法像内存一样通过指针进行随机地址访问，
 * 要访问某个磁盘数据，需要先将其对应的磁盘块读取到内存，再通过
 * 块内的偏移位置最终定位所需的数据
 * 使用场景：
 *  1. 表空间管理模块
 *  2. 事务处理模块
 *  3. B树模块
 *
 *
 */

/* The C 'types' of base node and list node: these should be used to
write self-documenting code. Of course, the sizeof macro cannot be
applied to these types! */

typedef	byte	flst_base_node_t;  // 基节点
typedef	byte	flst_node_t;    // 节点链表

/* The physical size of a list base node in bytes */
/* 一个base node包含：
 *  1. 一个表示链表节点个数的字段： 4个字节
 *  2. 两个链接链表起始和末尾节点的指针： 2 * 6B
 */
#define	FLST_BASE_NODE_SIZE	(4 + 2 * FIL_ADDR_SIZE)  // 16B

/* The physical size of a list node in bytes */
/*
 * 链表节点包含两个字段：
 * 1. 指向前序节点的指针
 * 2. 指向后继节点的指针
 */
#define	FLST_NODE_SIZE		(2 * FIL_ADDR_SIZE)     // 12B


/************************************************************************
Initializes a list base node. */
UNIV_INLINE
void
flst_init(
/*======*/
	flst_base_node_t*	base,	/* in: pointer to base node */
	mtr_t*			mtr);	/* in: mini-transaction handle */
/************************************************************************
Adds a node as the last node in a list. */

void
flst_add_last(
/*==========*/
	flst_base_node_t*	base,	/* in: pointer to base node of list */
	flst_node_t*		node,	/* in: node to add */
	mtr_t*			mtr);	/* in: mini-transaction handle */
/************************************************************************
Adds a node as the first node in a list. */

void
flst_add_first(
/*===========*/
	flst_base_node_t*	base,	/* in: pointer to base node of list */
	flst_node_t*		node,	/* in: node to add */
	mtr_t*			mtr);	/* in: mini-transaction handle */
/************************************************************************
Inserts a node after another in a list. */

void
flst_insert_after(
/*==============*/
	flst_base_node_t*	base,	/* in: pointer to base node of list */
	flst_node_t*		node1,	/* in: node to insert after */
	flst_node_t*		node2,	/* in: node to add */
	mtr_t*			mtr);	/* in: mini-transaction handle */
/************************************************************************
Inserts a node before another in a list. */

void
flst_insert_before(
/*===============*/
	flst_base_node_t*	base,	/* in: pointer to base node of list */
	flst_node_t*		node2,	/* in: node to insert */
	flst_node_t*		node3,	/* in: node to insert before */
	mtr_t*			mtr);	/* in: mini-transaction handle */
/************************************************************************
Removes a node. */

void
flst_remove(
/*========*/
	flst_base_node_t*	base,	/* in: pointer to base node of list */
	flst_node_t*		node2,	/* in: node to remove */
	mtr_t*			mtr);	/* in: mini-transaction handle */
/************************************************************************
Cuts off the tail of the list, including the node given. The number of
nodes which will be removed must be provided by the caller, as this function
does not measure the length of the tail. */

void
flst_cut_end(
/*=========*/
	flst_base_node_t*	base,	/* in: pointer to base node of list */
	flst_node_t*		node2,	/* in: first node to remove */
	ulint			n_nodes,/* in: number of nodes to remove,
					must be >= 1 */
	mtr_t*			mtr);	/* in: mini-transaction handle */
/************************************************************************
Cuts off the tail of the list, not including the given node. The number of
nodes which will be removed must be provided by the caller, as this function
does not measure the length of the tail. */

void
flst_truncate_end(
/*==============*/
	flst_base_node_t*	base,	/* in: pointer to base node of list */
	flst_node_t*		node2,	/* in: first node not to remove */
	ulint			n_nodes,/* in: number of nodes to remove */
	mtr_t*			mtr);	/* in: mini-transaction handle */
/************************************************************************
Gets list length. */
UNIV_INLINE
ulint
flst_get_len(
/*=========*/
					/* out: length */
	flst_base_node_t*	base,	/* in: pointer to base node */
	mtr_t*			mtr);	/* in: mini-transaction handle */
/************************************************************************
Gets list first node address. */
UNIV_INLINE
fil_addr_t
flst_get_first(
/*===========*/
					/* out: file address */
	flst_base_node_t*	base,	/* in: pointer to base node */
	mtr_t*			mtr);	/* in: mini-transaction handle */
/************************************************************************
Gets list last node address. */
UNIV_INLINE
fil_addr_t
flst_get_last(
/*==========*/
					/* out: file address */
	flst_base_node_t*	base,	/* in: pointer to base node */
	mtr_t*			mtr);	/* in: mini-transaction handle */
/************************************************************************
Gets list next node address. */
UNIV_INLINE
fil_addr_t
flst_get_next_addr(
/*===============*/
				/* out: file address */
	flst_node_t*	node,	/* in: pointer to node */
	mtr_t*		mtr);	/* in: mini-transaction handle */
/************************************************************************
Gets list prev node address. */
UNIV_INLINE
fil_addr_t
flst_get_prev_addr(
/*===============*/
				/* out: file address */
	flst_node_t*	node,	/* in: pointer to node */
	mtr_t*		mtr);	/* in: mini-transaction handle */
/************************************************************************
Writes a file address. */
UNIV_INLINE
void
flst_write_addr(
/*============*/
	fil_faddr_t*	faddr,	/* in: pointer to file faddress */
	fil_addr_t	addr,	/* in: file address */
	mtr_t*		mtr);	/* in: mini-transaction handle */
/************************************************************************
Reads a file address. */
UNIV_INLINE
fil_addr_t
flst_read_addr(
/*===========*/
				/* out: file address */
	fil_faddr_t*	faddr,	/* in: pointer to file faddress */
	mtr_t*		mtr);	/* in: mini-transaction handle */
/************************************************************************
Validates a file-based list. */

ibool
flst_validate(
/*==========*/
					/* out: TRUE if ok */
	flst_base_node_t*	base,	/* in: pointer to base node of list */
	mtr_t*			mtr1);	/* in: mtr */
/************************************************************************
Prints info of a file-based list. */

void
flst_print(
/*=======*/
	flst_base_node_t*	base,	/* in: pointer to base node of list */
	mtr_t*			mtr);	/* in: mtr */


#ifndef UNIV_NONINL
#include "fut0lst.ic"
#endif

#endif
