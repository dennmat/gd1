/*#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <stdio.h>

template <class Type>
class QuadTreeNode {
public:
	const static int NODE_CAPACITY = 4;

	QuadTreeNode * northEast;
	QuadTreeNode * northWest;
	QuadTreeNode * southEast;
	QuadTreeNode * southWest;
	int x, y; //center

	Type data[QuadTreeNode::NODE_CAPACITY];

};

template <class Type>
class QuadTree {
protected:
	QuadTreeNode root;
public:
	QuadTree();

	void insert(int x, int y, Type data);
	Type get(int x, int y);

	~QuadTree();
};

#endif*/