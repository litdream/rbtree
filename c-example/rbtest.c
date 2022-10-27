#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "rbtree.h"

/*
Generating random characters:

MACOS: (25 characters)
$ head -c 25 /dev/urandom| base64| python3 -c "import sys; line=sys.stdin.readline(); print(line.replace('=',''))"

Linux:
$

 */

#define IDLEN  99

struct person {
	struct rb_node node;
	
	char id[IDLEN + 1];
	char fname[50];
	char lname[50];
};

int make_person(struct person *p, char *id, char *fname, char *lname)
{
	memset(p, 0, sizeof(struct person));
	strncpy(p->id, id, 35);
	strncpy(p->fname, fname, strlen(fname));
	strncpy(p->lname, lname, strlen(lname));

	return 0;
}

struct person * my_search(struct rb_root *root, char *string)
{
  	struct rb_node *node = root->rb_node;

  	while (node) {
  		struct person *data = container_of(node, struct person, id);
		int result;

		result = strcmp(string, data->id);

		if (result < 0)
  			node = node->rb_left;
		else if (result > 0)
  			node = node->rb_right;
		else
  			return data;
	}
	return NULL;
}

int my_insert(struct rb_root *root, struct person *data)
{
  	struct rb_node **new = &(root->rb_node), *parent = NULL;

  	/* Figure out where to put new node */
  	while (*new) {
  		struct person *this = container_of(*new, struct person, node);
  		int result = strcmp(data->id, this->id);

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

int main(int argc, char **argv)
{
	struct rb_root tree = RB_ROOT;

	struct person a;
	make_person(&a, "N1x9mNI+FhR8oxDKWzNvYQABQ/FyGcsxGQ", "John", "Herron");

	struct person b;
	make_person(&b, "BWrXM0IU4Mj+sUwX2gHu1FLs3LJBILLt1A", "Angela", "Smith");

	struct person c;
	make_person(&c, "LHt7WnZhGnUzGYp5z0RED1QtW8ZZPyrcrQ", "Erin", "Caldwell");

	struct person d;
	make_person(&d, "IVHS9tFnku8BCwBFi7AZgVYxk1+1XSea7g", "Ronald", "Collin");

	struct person e;
	make_person(&e, "H+eD8XMhD6azE8DJHlVxhAGxp5rTcM2PtQ", "Samuel", "Randolph");

	struct person f;
	make_person(&f, "KNEquuTe+/fjjHAyNtOj8fzgDZj+OLuMvA", "Emil", "Gentry");

	struct person g;
	make_person(&g, "A6ZJkbDhtHBFankqsOCkvocxjQAvEx1NuQ", "George", "Hanscom");

	my_insert(&tree, &a);
	my_insert(&tree, &b);
	my_insert(&tree, &c);
	my_insert(&tree, &d);
	my_insert(&tree, &e);
	my_insert(&tree, &f);
	my_insert(&tree, &g);

	struct rb_node *node;
	printf("search all nodes: \n");
	for (node = rb_first(&tree); node; node = rb_next(node)) {
		struct person *found = rb_entry(node, struct person, id);
		printf("data = %s, %s, %s\n", found->id, found->fname, found->lname);
	}
	
	return 0;
}
