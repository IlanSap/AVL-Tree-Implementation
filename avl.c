#include "avl.h"
#include <stdio.h>
#include <stdlib.h>


/* Optional utility functions*/
int util_max(int x, int y)
{
	return x > y ? x : y;
}
int util_abs(int x)
{
	return (x > 0) ? x : -x;
}

// AVL Operations
void fix_parameters(AVLNodePtr root);
int check_height(AVLNodePtr root);
AVLNodePtr avl_find_min(AVLNodePtr root);
AVLNodePtr balance_avl(AVLNodePtr root);
AVLNodePtr left_rotate(AVLNodePtr root);
AVLNodePtr right_rotate(AVLNodePtr root);

// AVL Sum Operations
int avl_sum_smaller(AVLNodePtr root, int x);
int avl_sum_bigger(AVLNodePtr root, int y);

// Merge Operations
void avl_to_array(AVLNodePtr root, int arr[], int* next);
int* new_array(int x);
int* merge_array(int arr1[], int arr2[], int len1, int len2, int* to_cut);
int* cut_duplicates(int arr[], int size, int duplicates);
AVLNodePtr array_to_avl(int array[], int head, int tail);

/////////////////////////////////////////////////// A V L ///////////////////////////////////////////////////

/* finds x in an AVL tree (if exist) */
AVLNodePtr avl_search(AVLNodePtr root, int x) /* O(log(n)) */
{
	if (root == NULL) /* x is not in tree */
		return NULL;
	else if (root->key == x)
		return root;
	else if (root->key < x)
		avl_search(root->child[RIGHT], x); /*go right*/
	else
		avl_search(root->child[LEFT], x); /* go left*/
}


/* inserts a Node to tree while keeping the AVL attributes */
AVLNodePtr avl_insert(AVLNodePtr root, int x) /* O(log(n)) */
{
	if (root == NULL) /* correct position found */
		return new_avl_node(x);
	else if (root->key == x)
		return root;
	else if (root->key > x) /* go left*/
	{
		root->child[LEFT] = avl_insert(root->child[LEFT], x);
		fix_parameters(root);
	}
	else if (root->key < x) /* go right*/
	{
		root->child[RIGHT] = avl_insert(root->child[RIGHT], x);
		fix_parameters(root);
	}
	return balance_avl(root);
}


/* constructs a new Node */
AVLNodePtr new_avl_node(int x)  /* O(log(n)) */
{
	AVLNodePtr newNode = (AVLNodePtr)malloc(sizeof(AVLNode)); /* memory allocation */
	if (newNode == NULL) {
		printf("Allocation failed! \n");
		return NULL;
	}
	newNode->key = x; /* set fields */
	newNode->height = 0;
	newNode->tree_size = 1;
	newNode->tree_sum = x;
	newNode->child[RIGHT] = NULL;
	newNode->child[LEFT] = NULL;
	return newNode;
}


/* deletes a certaiin Node from a tree while keeping the AVL attributes */
AVLNodePtr avl_delete(AVLNodePtr root, int x) /* O(log(n)) */
{
	AVLNodePtr temp;
	if (root == NULL) /* tree is empty */
		return NULL;
	if (root->child[LEFT] == NULL && root->child[RIGHT] == NULL && root->key == x) /* if leaf*/
	{
		free(root);
		return NULL;
	}
	else if (root->key > x) { /* go left*/
		root->child[LEFT] = avl_delete(root->child[LEFT], x);
	}
	else if (root->key < x) { /* go right*/
		root->child[RIGHT] = avl_delete(root->child[RIGHT], x);
	}
	else {
		if (root->child[LEFT] == NULL) /*right child*/
		{
			temp = root;
			root = root->child[RIGHT];
			free(temp);
			return balance_avl(root); /*get balance*/
		}
		else if (root->child[RIGHT] == NULL) /*left child*/
		{
			temp = root;
			root = root->child[LEFT];
			free(temp);
			return balance_avl(root); /*get balance*/
		}
		else /* there are 2 children*/
		{
			temp = avl_find_min(root->child[RIGHT]); /*finding successor*/
			root->key = temp->key;
			root->child[RIGHT] = avl_delete(root->child[RIGHT], root->key);
		}
	}
	fix_parameters(root);
	return balance_avl(root);
}


/* deletes a whole tree */
void avl_delete_all(AVLNodePtr root) /* O(n) */
{
	if (root == NULL) /* tree is empty */
		return;
	avl_delete_all(root->child[RIGHT]); /* go right */
	avl_delete_all(root->child[LEFT]); /* go left */
	free(root);
}


/////////////////////////////////////////////////// U T I L I T Y  ///////////////////////////////////////////////////

/* validates that the tree is AVL */
AVLNodePtr balance_avl(AVLNodePtr root) /* O(1) */
{
	if ((root->child[LEFT] == NULL && root->child[RIGHT] == NULL) ||
		(root->child[LEFT] == NULL && root->child[RIGHT]->height < 1) ||
		(root->child[RIGHT] == NULL && root->child[LEFT]->height < 1) ||
		(root->child[RIGHT] && root->child[LEFT] && (util_abs((root->child[LEFT]->height - root->child[RIGHT]->height)) <= 1)))  /* tree is AVL */
		return root;
	else  /* tree needs to be balanced */
	{
		if (check_height(root) == 1) /* right is higher*/
		{
			if (check_height(root->child[RIGHT]) == -1) /*RL rotation*/
				root->child[RIGHT] = right_rotate(root->child[RIGHT]);
			return left_rotate(root);
		}
		else /* left is higher */
		{
			if (check_height(root->child[LEFT]) == 1) /* operates LR rotation */
				root->child[LEFT] = left_rotate(root->child[LEFT]);
			return right_rotate(root);
		}
	}
}


/* Check which sub-tree is higher: if left sub-tree higher return -1, else if right sub-tree higher return 1, else return 0 for equal */
int check_height(AVLNodePtr root)
{
	if (root->child[RIGHT] == NULL && root->child[LEFT] == NULL) /* same height */
		return 0;
	else if (root->child[RIGHT] == NULL) /* left higher */
		return -1;
	else if (root->child[LEFT] == NULL) /* right higher */
		return 1;
	else if (root->child[LEFT]->height > root->child[RIGHT]->height) /* left higher */
		return -1;
	else
		return (root->child[LEFT]->height < root->child[RIGHT]->height); /* right higher */
}


/* LR rotation operation*/
AVLNodePtr left_rotate(AVLNodePtr root) /* O(1) */
{
	AVLNodePtr x = root->child[RIGHT]->child[LEFT];
	AVLNodePtr y = root->child[RIGHT];
	root->child[RIGHT]->child[LEFT] = root;
	root->child[RIGHT] = x;
	fix_parameters(root);
	fix_parameters(y);
	return y;
}


/* RL rotation operation */
AVLNodePtr right_rotate(AVLNodePtr root) /* O(1) */
{
	AVLNodePtr x = root->child[LEFT]->child[RIGHT];
	AVLNodePtr y = root->child[LEFT];
	root->child[LEFT]->child[RIGHT] = root;
	root->child[LEFT] = x;
	fix_parameters(root);
	fix_parameters(y);
	return y;
}

/* finds the minimum Node (most left leaf) */
AVLNodePtr avl_find_min(AVLNodePtr root) /* O(1) */
{
	if (root == NULL || root->child[LEFT] == NULL) /* tree is empty or minimum found */
		return root;
	else
		return avl_find_min(root->child[LEFT]);
}

/* validates that the parameters of a Node are correct */
void fix_parameters(AVLNodePtr root) /* O(1) */
{
	if (root->child[LEFT] == NULL && root->child[RIGHT] == NULL)  /* Node is leaf */
	{
		root->height = 0;
		root->tree_size = 1;
		root->tree_sum = root->key;
	}
	else if (root->child[LEFT] == NULL) /* Node has right child */
	{
		root->height = root->child[RIGHT]->height + 1;
		root->tree_size = root->child[RIGHT]->tree_size + 1;
		root->tree_sum = root->child[RIGHT]->tree_sum + root->key;
	}
	else if (root->child[RIGHT] == NULL) /* Node has left child */
	{
		root->height = root->child[LEFT]->height + 1;
		root->tree_size = root->child[LEFT]->tree_size + 1;
		root->tree_sum = root->child[LEFT]->tree_sum + root->key;
	}
	else /* Node has two children */
	{
		root->height = util_max(root->child[RIGHT]->height, root->child[LEFT]->height) + 1;
		root->tree_size = root->child[RIGHT]->tree_size + root->child[LEFT]->tree_size + 1;
		root->tree_sum = root->child[RIGHT]->tree_sum + root->child[LEFT]->tree_sum + root->key;
	}
}

/////////////////////////////////////////////////// L C A ///////////////////////////////////////////////////

/* returns the Node on the "trip" to x or y, where the node's key is bigger than x and amaller than y */
AVLNodePtr avl_LCA(AVLNodePtr root, int x, int y) /* O(log(n)) */
{
	if (root == NULL) /* tree is smpty or x and y are not in tree's range */
		return NULL;

	/* If both x and y are bigger than root, then LCA is in the right.*/
	if (root->key < x && root->key < y)
		return avl_LCA(root->child[RIGHT], x, y);

	/* If both x and y are smaller than root, then LCA is in the left*/
	if (root->key > x && root->key > y)
		return avl_LCA(root->child[LEFT], x, y);

	/* returns the root that upholds the condition: x < key < y */
	return root;
}

/////////////////////////////////////////////////// S U M ///////////////////////////////////////////////////

/* returns the sum of all the Nodes in [x,y] */
int avl_sum(AVLNodePtr root, int x, int y) /* O(log(n)) */
{
	AVLNodePtr lca = avl_LCA(root, x, y);
	if (lca == NULL || y < x) /* LCA doesn't exists or wrong range input */
		return 0;
	/* returns the sum of the LCA tree, minus the sum of elements smaller than x, minus the sum of elements bigger than y */
	return lca->tree_sum - avl_sum_smaller(lca, x) - avl_sum_bigger(lca, y);
}


/* returns the sum of the Nodes smaller than x */
int avl_sum_smaller(AVLNodePtr root, int x) /* O(log(n)) */
{
	if (root == NULL) /* x doesn't exist in tree */
		return 0;

	if (root->key == x) /* return sum of x's left sub-tree */
	{
		if (root->child[LEFT] == NULL)
			return 0;
		else
			return root->child[LEFT]->tree_sum;
	}

	if (root->key < x) /* sums the left "skipped" sub-trees on the trip to x */
	{
		if (root->child[LEFT] == NULL)
			return root->key + avl_sum_smaller(root->child[RIGHT], x);
		else
			return root->key + root->child[LEFT]->tree_sum + avl_sum_smaller(root->child[RIGHT], x);
	}

	else /* x is on the left */
		return avl_sum_smaller(root->child[LEFT], x);
}


/* returns the sum of the Nodes bigger than y */
int avl_sum_bigger(AVLNodePtr root, int y) /* O(log(n)) */
{
	if (root == NULL) /* x doesn't exist in tree */
		return 0;

	if (root->key == y) /* return sum of y's right sub-tree */
	{
		if (root->child[RIGHT] == NULL)
			return 0;
		else
			return root->child[RIGHT]->tree_sum;
	}

	if (root->key > y) /* sums the right "skipped" sub-trees on the trip to y */
	{
		if (root->child[RIGHT] == NULL)
			return root->key + avl_sum_bigger(root->child[LEFT], y);
		else
			return root->key + root->child[RIGHT]->tree_sum + avl_sum_bigger(root->child[LEFT], y);
	}

	else /* y is on the right */
		return avl_sum_bigger(root->child[RIGHT], y);
}


/////////////////////////////////////////////////// M E R G E ///////////////////////////////////////////////////

/* operates merge to 2 AVL trees */
AVLNodePtr avl_merge(AVLNodePtr root1, AVLNodePtr root2) /* O(n) */
{
	if (root1 == NULL) /* root1 is empty or both trees empty */
		return root2;
	else if (root2 == NULL) /* root2 is empty */
		return root1;

	int duplicates = 0, root1len = 0, root2len = 0;
	root1len = root1->tree_size; /* builds a sorted InOrder array from root1 */
	int* arr1 = new_array(root1len);
	int a = 0;
	avl_to_array(root1, arr1, &a);

	root2len = root2->tree_size; /* builds a sorted InOrder array from root2 */
	int* arr2 = new_array(root2len);
	int b = 0;
	avl_to_array(root2, arr2, &b);

	int* mergedArr = merge_array(arr1, arr2, root1len, root2len, &duplicates); /* merge two arrays */
	free(arr1);
	free(arr2);

	int* final_array = cut_duplicates(mergedArr, (root1len + root2len), duplicates); /* deletes duplicates (validates BST) */
	free(mergedArr);

	AVLNodePtr merged_AVL = array_to_avl(final_array, 0, (root1len + root2len - duplicates - 1)); /* build an AVL tree from the merged array */
	free(final_array);

	balance_avl(merged_AVL);
	return merged_AVL;
}


/* allocates new array */
int* new_array(int x) /* O(1) */
{
	int* newArray = (int*)calloc(x, sizeof(int)); /* allocates an all-zero array */
	if (newArray == NULL) {
		printf("Allocation failed! \n");
		return NULL;
	}
	return newArray;
}


/* builds a sorted InOrder array from AVL */
void avl_to_array(AVLNodePtr root, int arr[], int* next) /* O(n) */
{
	if (root == NULL) /* tree is empty */
		return;

	avl_to_array(root->child[LEFT], arr, next); /* go left */

	arr[*next] = root->key; /* update array */
	(*next)++;

	avl_to_array(root->child[RIGHT], arr, next); /* go right */
}


/* merges two sorted arrays */
int* merge_array(int arr1[], int arr2[], int len1, int len2, int* to_cut) /* O(n+m)    n = len1, m = len2 */
{
	int* mergedArr = new_array(len1 + len2);
	int i = 0, j = 0, m = 0, duplicates = 0;

	while (i < len1 && j < len2) /* goes through all of both arrays */
	{
		if (arr1[i] < arr2[j]) /* add the smaller element first */
		{
			mergedArr[m] = arr1[i];
			i++;
			m++;
		}
		else
		{
			if (arr1[i] > arr2[j])
			{
				mergedArr[m] = arr2[j];
				m++;
			}
			else
				duplicates++;
			j++;
		}
	}

	while (i < len1) /* array1 tail */
	{
		mergedArr[m] = arr1[i];
		i++; m++;
	}

	while (j < len2) /* array2 tail */
	{
		mergedArr[m] = arr2[j];
		j++; m++;
	}
	*to_cut = duplicates; /* counts duplicates */
	return mergedArr;
}


/* deletes the duplicates from an array */
int* cut_duplicates(int arr[], int size, int duplicates) /* O(d)   d = duplicates */
{
	int i = 0;
	int* new_arr = new_array(size - duplicates); /* builds the new shorter (id needed) array */
	for (i = 0; i < size - duplicates; i++)
	{
		new_arr[i] = arr[i];
	}
	return new_arr;
}


/* builds an AVL from a sorted InOrder array */
AVLNodePtr array_to_avl(int array[], int head, int tail) /* O(n) */
{
	if (head > tail) /* array is empty */
		return NULL;

	int mid = (head + tail) / 2;
	AVLNodePtr root = new_avl_node(array[mid]); /* constructs root */
	root->child[LEFT] = array_to_avl(array, head, mid - 1); /* constructs left sub-tree */
	root->child[RIGHT] = array_to_avl(array, mid + 1, tail); /* constructs right sub-tree */

	fix_parameters(root);
	return root;
}
