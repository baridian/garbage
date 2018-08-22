#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "stack.h"

tree newTree(int dataSize, void *rootData)
{
	tree toReturn;
	toReturn.root = (treeNode *) malloc(sizeof(treeNode));
	toReturn.dataSize = (unsigned) dataSize;
	toReturn.root->data = malloc(toReturn.dataSize);
	memcpy(toReturn.root->data, rootData, toReturn.dataSize);
	toReturn.root->parent = NULL;
	toReturn.root->left = NULL;
	toReturn.root->right = NULL;
	toReturn.current = toReturn.root;
	return toReturn;
}

void addNode(tree t, treeDir dir, void *data)
{
	switch (dir)
	{
		case left:
			if (t.current->left != NULL)
			{
				fprintf(stderr, "ERROR: attempted to add child node to non-leaf\n");
				exit(1);
			}
			else
			{
				t.current->left = (treeNode *) malloc(sizeof(treeNode));
				t.current->left->parent = t.current;
				t.current->left->left = NULL;
				t.current->left->right = NULL;
				t.current->left->data = malloc(t.dataSize);
				memcpy(t.current->left->data, data, t.dataSize);
			}
			break;
		case right:
			if (t.current->right != NULL)
			{
				fprintf(stderr, "ERROR: attempted to add child node to non-leaf\n");
				exit(1);
			}
			else
			{
				t.current->right = (treeNode *) malloc(sizeof(treeNode));
				t.current->right->parent = t.current;
				t.current->right->left = NULL;
				t.current->right->right = NULL;
				t.current->right->data = malloc(t.dataSize);
				memcpy(t.current->right->data, data, t.dataSize);
			}
			break;
		default:
			fprintf(stderr,"ERROR: Invalid direction\n");
			exit(1);
	}
}

void *readNode(tree t)
{
	return t.current->data;
}

void step(tree *t, treeDir dir)
{
	switch (dir)
	{
		case up:
			if (t->current->parent != NULL)
				t->current = t->current->parent;
			break;
		case left:
			if (t->current->left != NULL)
				t->current = t->current->left;
			break;
		case right:
			if (t->current->right != NULL)
				t->current = t->current->right;
			break;
		default:
			fprintf(stderr,"ERROR: Invalid direction\n");
			exit(1);
	}
}

void splice(tree t, treeDir dir, tree splicingOn)
{
	if (dir == left && t.current->left == NULL)
	{
		t.current->left = splicingOn.root;
		splicingOn.root->parent = t.current;
		return;
	}
	if (dir == right && t.current->right == NULL)
	{
		t.current->right = splicingOn.root;
		splicingOn.root->parent = t.current;
	}
	else
	{
		fprintf(stderr,"ERROR: Attempted to splice tree to non-leaf\n");
		exit(1);
	}
}

void resetToRoot(tree *t)
{
	t->current = t->root;
}

void trim(tree *t, treeDir dir)
{
	tree toFree;
	toFree.dataSize = t->dataSize;
	switch (dir)
	{
		case left:
			toFree.root = t->current->left;
			t->current->left = NULL;
			break;
		case right:
			toFree.root = t->current->right;
			t->current->right = NULL;
			break;
		case up:
			toFree.root = t->root;
			if (t->current->parent->left == t->current)
				t->current->parent->left = NULL;
			else
				t->current->parent->right = NULL;
			t->current->parent = NULL;
			t->root = t->current;
			break;
		default:
			fprintf(stderr,"ERROR: invalid direction\n");
			exit(1);
	}
	toFree.root->parent = NULL;
	toFree.current = toFree.root;
	freeTree(toFree);
}

treeNodeType currentNodeType(tree t)
{
	if (t.current->left == NULL && t.current->right == NULL)
		return leaf;
	else if (t.current->left == NULL)
		return rightInternal;
	else if (t.current->right == NULL)
		return leftInternal;
	else
		return dualInternal;
}

int isRoot(tree t)
{
	if (t.current->parent == NULL)
		return 1;
	return 0;
}

static void stepLeftWhilePossible(tree *t, int *level, int *deepest, treeNode **deepestNode, stack *path)
{
	treeDir pathTaken;
	while (currentNodeType(*t) != leaf && currentNodeType(*t) != rightInternal)
	{
		if (currentNodeType(*t) == dualInternal)
		{
			pathTaken = left;
			spush(&pathTaken, path);
		}
		(*level)++;
		step(t, left);
		if (*level > *deepest)
		{
			*deepest = *level;
			*deepestNode = t->current;
		}
	}
}

static void stepUpToFirstDual(tree *t, stack path, int *level)
{
	while (stackSize(path) > 0 && currentNodeType(*t) != dualInternal)
	{
		(*level)--;
		step(t, up);
	}
}

static void stepRightIfDual(tree *t, int *level, stack *path)
{
	treeDir pathTaken;
	if (currentNodeType(*t) == dualInternal)
	{
		(*level)++;
		step(t, right);
		spop(path);
		pathTaken = right;
		spush(&pathTaken, path);
	}
}

static void stepRightUntilCanStepLeft(tree *t, int *level, int *deepest, treeNode **deepestNode)
{
	while (currentNodeType(*t) == rightInternal)
	{
		(*level)++;
		step(t, right);
		if (*level > *deepest)
		{
			*deepest = *level;
			*deepestNode = t->current;
		}
	}
}

static void stepUpAndIntoFirstUnvisitedRightNodeIfLeaf(tree *t, stack *path, int *level)
{
	treeDir toPush = right;
	int firstTime = 1;
	if (currentNodeType(*t) == leaf && !isRoot(*t))
	{
		while (stackSize(*path) > 0 && *(treeNodeType *) speek(*path) == right)
		{
			(*level)--;
			step(t, up);
			if (currentNodeType(*t) == dualInternal)
			{
				if (firstTime)
					firstTime = 0;
				else
					spop(path);
			}
		}
		if (stackSize(*path) > 0)
		{
			(*level)++;
			step(t, right);
			spop(path);
			spush(&toPush,path);
		}
	}
}

void stepToLowestInternal(tree *t)
{
	int current = 0;
	int deepest = 0;
	treeNode *deepestNode = t->root;
	stack path = newStack(sizeof(treeDir));
	t->current = t->root;
	do
	{
		stepLeftWhilePossible(t, &current, &deepest, &deepestNode, &path);

		stepUpToFirstDual(t, path, &current);

		stepRightIfDual(t, &current, &path);

		stepRightUntilCanStepLeft(t, &current, &deepest, &deepestNode);

		stepUpAndIntoFirstUnvisitedRightNodeIfLeaf(t, &path, &current);
	} while (stackSize(path) > 0);
	frees(path);
	t->current = deepestNode;
}

treeDir stepUpAndGetStepToPrevious(tree *t)
{
	treeNode *old = t->current;
	step(t,up);
	if(t->current->left == old)
		return left;
	return right;
}

void freeTree(tree t)
{
	do
	{
		stepToLowestInternal(&t);
		free(t.current->data);
		if(t.current->parent)
		{
			if (t.current->parent->left == t.current)
			{
				t.current->parent->left = NULL;
			}
			else
			{
				t.current->parent->right = NULL;
			}
		}
		free(t.current);
	} while (t.root != t.current);
}
