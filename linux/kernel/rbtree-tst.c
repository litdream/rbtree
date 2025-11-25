/*
 * rbtree-tst.c - Test program for userspace rbtree library
 * Adapted from Linux kernel rbtree implementation
 */

#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct mynode {
	struct rb_node node;
	char *string;
};

struct rb_root mytree = RB_ROOT;

struct mynode *my_search(struct rb_root *root, char *string)
{
	struct rb_node *node = root->rb_node;

	while (node) {
		struct mynode *data = container_of(node, struct mynode, node);
		int result;

		result = strcmp(string, data->string);

		if (result < 0)
			node = node->rb_left;
		else if (result > 0)
			node = node->rb_right;
		else
			return data;
	}
	return NULL;
}

int my_insert(struct rb_root *root, struct mynode *data)
{
	struct rb_node **new = &(root->rb_node), *parent = NULL;

	/* Figure out where to put new node */
	while (*new) {
		struct mynode *this = container_of(*new, struct mynode, node);
		int result = strcmp(data->string, this->string);

		parent = *new;
		if (result < 0)
			new = &((*new)->rb_left);
		else if (result > 0)
			new = &((*new)->rb_right);
		else
			return 0;
	}

	/* Add new node and rebalance tree. */
	rb_link_node(&data->node, parent, new);
	rb_insert_color(&data->node, root);

	return 1;
}

void my_free(struct mynode *node)
{
	if (node != NULL) {
		if (node->string != NULL) {
			free(node->string);
			node->string = NULL;
		}
		free(node);
		node = NULL;
	}
}

#define NUM_NODES 32

int main()
{
	struct mynode *mn[NUM_NODES];
	int i;

	/* Insert nodes */
	printf("Insert nodes from 0 to %d:\n", NUM_NODES - 1);
	for (i = 0; i < NUM_NODES; i++) {
		mn[i] = (struct mynode *)malloc(sizeof(struct mynode));
		mn[i]->string = (char *)malloc(sizeof(char) * 4);
		sprintf(mn[i]->string, "%d", i);
		my_insert(&mytree, mn[i]);
	}

	/* Search and print all nodes */
	struct rb_node *node;
	printf("\nSearch all nodes:\n");
	for (node = rb_first(&mytree); node; node = rb_next(node))
		printf("  key = %s\n", rb_entry(node, struct mynode, node)->string);

	/* Delete node 20 */
	printf("\nDelete node 20:\n");
	struct mynode *data = my_search(&mytree, "20");
	if (data) {
		rb_erase(&data->node, &mytree);
		my_free(data);
		printf("  Node 20 deleted\n");
	}

	/* Delete node 10 */
	printf("\nDelete node 10:\n");
	data = my_search(&mytree, "10");
	if (data) {
		rb_erase(&data->node, &mytree);
		my_free(data);
		printf("  Node 10 deleted\n");
	}

	/* Delete node 15 */
	printf("\nDelete node 15:\n");
	data = my_search(&mytree, "15");
	if (data) {
		rb_erase(&data->node, &mytree);
		my_free(data);
		printf("  Node 15 deleted\n");
	}

	/* Search again */
	printf("\nSearch remaining nodes:\n");
	for (node = rb_first(&mytree); node; node = rb_next(node))
		printf("  key = %s\n", rb_entry(node, struct mynode, node)->string);

	printf("\nTest completed successfully!\n");
	return 0;
}
