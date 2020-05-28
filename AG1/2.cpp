#include <iostream>
#include <cassert>
#include <functional>
//#include "AVLTree.h"
//#include "BinomialHeap.h"
//#include "BinaryHeap.h"
using namespace std;

template <class T>
class BinaryHeapNode
{
public:
	BinaryHeapNode(T* data, int index) : data(data), index(index) {}
	T* data;
	int index;
};
template <class T, class E>
class BinaryHeap
{
public:
	BinaryHeap(std::function<int(T*, T*)> comparer) :head(nullptr), m_comparer(comparer), data(nullptr), m_size(-1), m_allocated(16) {
		nodes = (BinaryHeapNode<T> * *)malloc(m_allocated * sizeof(BinaryHeapNode<T>*));
	}
	BinaryHeapNode<T>* head;
	void BubbleUp(BinaryHeapNode<T>* n);
	void BubbleDown(BinaryHeapNode<T>* n);

	T* GetLChild();
	T* GetRChild();

	T* ExtractMin();
	T* GetMin();
	BinaryHeapNode<T>* Insert(T* data);
	BinaryHeapNode<T>** nodes;
	std::function<int(T*, T*)> m_comparer;
	E* data;
	~BinaryHeap()
	{
		delete data;

		for (int i = 0; i <= m_size; i++)
			delete nodes[i];

		free(nodes);
	}

private:
	void CheckMem();
	int m_size;
	int m_allocated;
};
template <class T, class E>

void  BinaryHeap<T, E>::BubbleUp(BinaryHeapNode<T>* n)
{
	if (n->index <= 0 || n->index > m_size)
		return;

	int parent = ((n->index + 1) / 2) - 1;
	if (m_comparer(nodes[parent]->data, n->data) == 1)
	{
		int i1 = n->index;
		int i2 = parent;
		BinaryHeapNode<T>* tmp = nodes[i1];
		nodes[i1] = nodes[i2];
		nodes[i2] = tmp;

		nodes[i1]->index = i1;
		nodes[i2]->index = i2;
		BubbleUp(nodes[i2]);
	}
}
template <class T, class E>
void BinaryHeap<T, E>::BubbleDown(BinaryHeapNode<T>* n)
{
	if (n->index < 0 || n->index > m_size)
		return;

	int firstChild = ((n->index + 1) * 2) - 1;
	int smallerChildIndex = firstChild;

	if (firstChild > m_size)
		return; //No child

	int secondChild = firstChild + 1;
	if (secondChild <= m_size)
		smallerChildIndex = m_comparer(nodes[firstChild]->data, nodes[secondChild]->data) == 1 ? secondChild : firstChild;

	if (m_comparer(nodes[smallerChildIndex]->data, n->data) == -1)//Smaller child is bigger
	{
		int i1 = n->index;
		int i2 = smallerChildIndex;
		BinaryHeapNode<T>* tmp = nodes[i1];
		nodes[i1] = nodes[i2];
		nodes[i2] = tmp;

		nodes[i1]->index = i1;
		nodes[i2]->index = i2;
		BubbleDown(nodes[i2]);
	}

}
template <class T, class E>
T* BinaryHeap<T, E>::GetMin()
{
	if (m_size < 0)
		return nullptr;

	return nodes[0]->data;
}

template <class T, class E>
T* BinaryHeap<T, E>::GetLChild()
{
	if (m_size < 1) return nullptr;

	return nodes[1]->data;
}

template <class T, class E>
T* BinaryHeap<T, E>::GetRChild()
{
	if (m_size < 2) return nullptr;

	return nodes[2]->data;
}

template <class T, class E>
T* BinaryHeap<T, E>::ExtractMin()
{
	if (m_size < 0)
		return nullptr;
	//Dont delete, just bubble down
	BinaryHeapNode<T>* tmp = nodes[0];
	nodes[0] = nodes[m_size];
	nodes[m_size] = tmp;
	T* ret = tmp->data;
	nodes[0]->index = 0;
	delete nodes[m_size];
	BubbleDown(nodes[0]);
	m_size--;
	return ret;
}
template <class T, class E>
BinaryHeapNode<T>* BinaryHeap<T, E>::Insert(T* data)
{
	CheckMem();
	BinaryHeapNode<T>* toAdd = new BinaryHeapNode<T>(data, m_size);
	nodes[m_size] = toAdd;
	BubbleUp(toAdd);
	return toAdd;
}
template <class T, class E>
void BinaryHeap<T, E>::CheckMem()
{
	m_size++;
	// array is too small
	if (m_size >= m_allocated)
	{
		m_allocated *= 2;
		nodes = (BinaryHeapNode<T> * *)realloc(nodes, m_allocated * sizeof(BinaryHeapNode<T>*));
	}
}


template<class T>
class AVLNode
{
public:
	AVLNode(T* c) : data(c)
	{
		lChild = NULL;
		rChild = NULL;
		height = 1; //Leaf
		weight = 1;
	}
	AVLNode<T>* lChild;
	AVLNode<T>* rChild;
	~AVLNode() {
		delete data;
	}
	T* data;
	int height;
	int weight;
};
template<typename T>
class AVLTree
{
public:
	AVLTree(std::function<int(T*, T*)> predicate) : top(nullptr), m_comparer(predicate) {}
	inline AVLNode<T>* AddNode(T* node);
	inline AVLNode<T>* FindNode(T* dummyKey);
	AVLNode<T>* GetMin(AVLNode<T>* root);
	AVLNode<T>* DeleteNode(AVLNode<T>* root, T* dummyKey);
	AVLNode<T>* top;
	void FreeNode(AVLNode<T>* t)
	{
		if (!t)
			return;
		FreeNode(t->lChild);
		FreeNode(t->rChild);
		delete t;
	}
	~AVLTree() {
		FreeNode(top);
	}
private:
	inline AVLNode<T>* InsertNodeRecusrive(AVLNode<T>* node, T* dummy);
	inline AVLNode<T>* Balance(AVLNode<T>* Node, T* dummyNode);
	inline AVLNode<T>* RightRotate(AVLNode<T>* subtree);
	inline AVLNode<T>* LeftRotate(AVLNode<T>* subtree);
	AVLNode<T>* BalanceAfterDelete(AVLNode<T>* root);
	std::function<int(T*, T*)> m_comparer;
};

template<typename T>
inline AVLNode<T>* AVLTree<T>::GetMin(AVLNode<T>* root)
{
	if (!root)
		return NULL;

	AVLNode<T>* curr = root;
	while (curr->lChild)
		curr = curr->lChild;
	return curr;
}

template<typename T>
AVLNode<T>* AVLTree<T>::BalanceAfterDelete(AVLNode<T>* root)
{
	if (!top)
		return NULL;

	int balance = GetBalance(root);

	if (balance > 1 &&
		GetBalance(root->lChild) >= 0)
		return RightRotate(root);

	// Left Right Case
	if (balance > 1 &&
		GetBalance(root->lChild) < 0)
	{
		root->lChild = LeftRotate(root->lChild);
		return RightRotate(root);
	}

	// Right Right Case
	if (balance < -1 &&
		GetBalance(root->rChild) <= 0)
		return LeftRotate(root);

	// Right Left Case
	if (balance < -1 &&
		GetBalance(root->rChild) > 0)
	{
		root->rChild = RightRotate(root->rChild);
		return LeftRotate(root);
	}

	return root;
}

template<typename T>
AVLNode<T>* AVLTree<T>::DeleteNode(AVLNode<T>* root, T* dummyKey)
{
	//BST
	//Nothing here... Wasted time
	if (!root)
		return root;

	if (!root->lChild && !root->rChild && root == top)
	{
		if (top->weight > 1)
		{
			top->weight--;
			return top;
		}


		delete(top);
		top = nullptr;
		return nullptr;
	}

	if (m_comparer(dummyKey, root->data) == -1)
		root->lChild = DeleteNode(root->lChild, dummyKey);

	else if (m_comparer(dummyKey, root->data) == 1)
		root->rChild = DeleteNode(root->rChild, dummyKey);
	//Got u!
	else
	{
		if (root->weight > 1)
		{
			root->weight--;
			return root;
		}
		//One or no child
		if (!root->lChild || !root->rChild)
		{
			AVLNode<T>* tmp = root->lChild ?
				root->lChild :
				root->rChild;

			// No child case, just delete the root
			if (!tmp)
			{
				tmp = root;
				root = NULL;
			}
			//One child case, just copy data...
			else
			{
				root->data = tmp->data;
				root->height = tmp->height;
				root->lChild = tmp->lChild;
				root->rChild = tmp->rChild;
				root->weight = tmp->weight;
			}

			delete(tmp);
			tmp = nullptr;
		}
		//Two children
		else
		{
			//Get min successor (potomek) of its bigger child
			AVLNode<T>* tmp = GetMin(root->rChild);

			root->data = tmp->data;
			//Swap right child with deleted node
			root->rChild = DeleteNode(root->rChild,
				tmp->data);
		}
	}

	if (!root)
		return root;

	root->height = GetHeight(root->lChild) < GetHeight(root->rChild)
		? GetHeight(root->rChild) + 1 : GetHeight(root->lChild) + 1;

	return BalanceAfterDelete(root);
}

template<typename T>
inline unsigned int GetHeight(AVLNode<T>* node)
{
	if (node)
		return node->height;
	return 0;
}
template<typename T>
inline int GetBalance(AVLNode<T>* node)
{
	if (node)
		return GetHeight(node->lChild) - GetHeight(node->rChild);

	return 0;
}
template<typename T>
inline AVLNode<T>* AVLTree<T>::FindNode(T* dummyKey)
{
	if (!top)
		return nullptr;

	AVLNode<T>* tmp = top;

	while (tmp)
	{
		if (m_comparer(tmp->data, dummyKey) == 0)
			return tmp;
		else if (m_comparer(tmp->data, dummyKey) == 1)
			tmp = tmp->lChild;
		else if (m_comparer(tmp->data, dummyKey) == -1)
			tmp = tmp->rChild;
	}

	return tmp;
}
template<typename T>
inline AVLNode<T>* AVLTree<T>::RightRotate(AVLNode<T>* subtree)
{
	bool topFlag = m_comparer(subtree->data, top->data) == 0;
	//Let subtree root be the R, it's left child L , then "nameOfNode(p)", where p = r/l represents node's right or left child respecitvely
	//As we rotate, we know that L exists for sure, oterwise, we wouldnt get right rotation request.
	AVLNode<T>* L = subtree->lChild;
	AVLNode<T>* Lr = L->rChild;

	//L is new root, new L(r) is R
	L->rChild = subtree;
	subtree->lChild = Lr;
	//Update heights!
	subtree->height = GetHeight(subtree->lChild) < GetHeight(subtree->rChild)
		? GetHeight(subtree->rChild) + 1 : GetHeight(subtree->lChild) + 1;

	L->height = GetHeight(L->lChild) < GetHeight(L->rChild)
		? GetHeight(L->rChild) + 1 : GetHeight(L->lChild) + 1;

	//Thats all, return new root of that subtree
	if (topFlag)
		top = L;
	return L;
}
template<typename T>
inline AVLNode<T>* AVLTree<T>::LeftRotate(AVLNode<T>* subtree)
{
	//!Right rotation
	bool topFlag = m_comparer(subtree->data, top->data) == 0;

	AVLNode<T>* L = subtree->rChild;
	AVLNode<T>* Lr = L->lChild;

	L->lChild = subtree;
	subtree->rChild = Lr;

	subtree->height = GetHeight(subtree->lChild) < GetHeight(subtree->rChild)
		? GetHeight(subtree->rChild) + 1 : GetHeight(subtree->lChild) + 1;

	L->height = GetHeight(L->lChild) < GetHeight(L->rChild)
		? GetHeight(L->rChild) + 1 : GetHeight(L->lChild) + 1;
	if (topFlag)
		top = L;
	return L;
}
template<typename T>
inline AVLNode<T>* AVLTree<T>::AddNode(T* node)
{
	if (!top)
	{
		top = new AVLNode<T>(node);
		return top;
	}
	return InsertNodeRecusrive(top, node);
}
template<typename T>
inline AVLNode<T>* AVLTree<T>::InsertNodeRecusrive(AVLNode<T>* toInsert, T* dummyNode)
{
	//BST insert
	if (toInsert == nullptr)
		return new AVLNode<T>(dummyNode);

	if (m_comparer(toInsert->data, dummyNode) == 1)
		toInsert->lChild = InsertNodeRecusrive(toInsert->lChild, dummyNode);
	else if (m_comparer(toInsert->data, dummyNode) == -1)
		toInsert->rChild = InsertNodeRecusrive(toInsert->rChild, dummyNode);
	else {
		//Handle same key instert
		toInsert->weight++;
		return toInsert;
	}

	//As recursion ends, "bubble up the path" and update each ancestros height
	toInsert->height = GetHeight(toInsert->lChild) < GetHeight(toInsert->rChild)
		? GetHeight(toInsert->rChild) + 1 : GetHeight(toInsert->lChild) + 1;

	return Balance(toInsert, dummyNode);
}
template<typename T>
inline AVLNode<T>* AVLTree<T>::Balance(AVLNode<T>* balanceRoot, T* dummyNode)
{
	int balanceFactor = GetBalance(balanceRoot);
	//5 cases
	//Left left
	if (balanceRoot->lChild && balanceFactor > 1 && m_comparer(balanceRoot->lChild->data, dummyNode) == 1)
		return RightRotate(balanceRoot);
	//Left right
	if (balanceRoot->lChild && balanceFactor > 1 && m_comparer(balanceRoot->lChild->data, dummyNode) == -1)
	{
		//Left rotate by root of left child
		balanceRoot->lChild = LeftRotate(balanceRoot->lChild);
		//Transormed to Left left case
		return RightRotate(balanceRoot);
	}

	//Right Right
	if (balanceRoot->rChild && balanceFactor < -1 && m_comparer(balanceRoot->rChild->data, dummyNode) == -1)
		return LeftRotate(balanceRoot);

	//Right left
	if (balanceRoot->rChild && balanceFactor < -1 && m_comparer(balanceRoot->rChild->data, dummyNode) == 1)
	{
		//Left rotate by root of right child
		balanceRoot->rChild = RightRotate(balanceRoot->rChild);
		//Transormed to Right right case
		return LeftRotate(balanceRoot);
	}
	//No need to rotate
	return balanceRoot;
}

template<class T>
class BinomialTree
{
public:
	BinomialTree(T* val) : rightMostChild(nullptr), GreaterOrderTree(nullptr), LessOrderTree(nullptr), order(1), val(val) {}
	BinomialTree<T>* rightMostChild;
	BinomialTree<T>* GreaterOrderTree;
	BinomialTree<T>* LessOrderTree;
	int order;
	T* val;
};
/*Custom built binomial heap*/
template<class T, class E>
class BinomialHeap
{
public:

	BinomialHeap(std::function<int(T*, T*)> comparer) : head(nullptr), tail(nullptr), min(nullptr), lessOrderMin(nullptr), m_comparer(comparer), data(nullptr),
		m_minUpdated(false) { }

	BinomialHeap(E* data, std::function<int(T*, T*)> comparer) : head(nullptr), tail(nullptr), min(nullptr), lessOrderMin(nullptr),
		m_comparer(comparer), data(data), m_minUpdated(false) { }
	void Print();

	BinomialHeap<T, E>* PrepareOneProductAddHeap(T* data);
	void Merge(BinomialHeap<T, E>* src);
	void PrepareTrees(BinomialTree<T>* src, BinomialTree<T>* dst);
	BinomialTree<T>* MergeTrees(BinomialTree<T>* src, BinomialTree<T>* dst);
	void LinkTree(BinomialTree<T>* t);
	void Add(T* data);
	T* ExtractMin();
	T* UpdateMin();
	BinomialTree<T>* head;
	BinomialTree<T>* tail;
	BinomialTree<T>* min;
	BinomialTree<T>* lessOrderMin;
	std::function<int(T*, T*)> m_comparer;
	E* data;
	~BinomialHeap()
	{
		FreeTree(head);
	}
	void FreeTree(BinomialTree<T>* head)
	{
		if (!head)
			return;

		FreeTree(head->GreaterOrderTree);
		FreeTree(head->rightMostChild);
		delete head;
	}
private:
	bool m_minUpdated;
};

template<class T, class E>
T* BinomialHeap<T, E>::UpdateMin()
{
	if (m_minUpdated)
	{
		if (min)
			return min->val;
		return nullptr;
	}


	BinomialTree<T>* tmp = head;
	BinomialTree<T>* bfrMin = nullptr;
	if (!min)
		min = tmp;

	while (tmp)
	{
		if (m_comparer(min->val, tmp->val) == 1)
		{
			min = tmp;
			lessOrderMin = bfrMin;
		}
		bfrMin = tmp;
		tmp = tmp->GreaterOrderTree;
	}
	m_minUpdated = true;
	if (!min)
		return nullptr;

	return min->val;
}

template<class T, class E>
void BinomialHeap<T, E>::Print()
{
	BinomialTree<T>* curr = head;
	if (!curr)
		std::cout << "empty";

	while (curr)
	{
		std::cout << "---" << curr->order;
		curr = curr->GreaterOrderTree;
	}

}
template<class T, class E>
T* BinomialHeap<T, E>::ExtractMin()
{
	UpdateMin();
	BinomialTree<T>* t = head;
	if (!t)
		return NULL;
	if (min->LessOrderTree)
	{
		BinomialTree<T>* tmp = min->LessOrderTree->GreaterOrderTree;
		min->LessOrderTree->GreaterOrderTree = min->GreaterOrderTree;
		tmp->LessOrderTree = min->LessOrderTree;
	}
	else
	{
		head = min->GreaterOrderTree;
		if (head)
			head->LessOrderTree = nullptr;
	}

	BinomialHeap<T, E>* toMerge = new BinomialHeap<T, E>(nullptr, nullptr);

	toMerge->head = min->rightMostChild;
	BinomialTree<T>* tmp = min->rightMostChild;
	if (tmp)
		while (tmp->GreaterOrderTree)
			tmp = tmp->GreaterOrderTree;

	T* val = min->val;

	toMerge->tail = tmp;
	Merge(toMerge);

	toMerge->head = nullptr;
	delete toMerge;
	delete min;

	min = nullptr;
	m_minUpdated = false;

	UpdateMin();

	return val;
}
template<class T>
//Make the tree disconnected
void UnlinkTree(BinomialTree<T>* t)
{
	t->GreaterOrderTree = nullptr;
	t->LessOrderTree = nullptr;
}
template<class T, class E>
//Returns prepared trees, where dst < src
void BinomialHeap<T, E>::PrepareTrees(BinomialTree<T>* src, BinomialTree<T>* dst)
{

	//DST is bigger than SRC, swap.
	if (m_comparer(dst->val, src->val) == 1)
	{
		T* tmp = src->val;
		BinomialTree<T>* srcRCH = src->rightMostChild;
		int orderTmp = src->order;
		src->val = dst->val;
		src->order = dst->order;
		src->rightMostChild = dst->rightMostChild;


		dst->rightMostChild = srcRCH;
		dst->order = orderTmp;
		dst->val = tmp;
	}
	//Now, DST is smaller than SRC, so append SRC to DST, to do so, delete all connections from SRC and transer them to DST, make them INDEPENDENT
	UnlinkTree(src);
	UnlinkTree(dst);
}
template<class T, class E>
//Merge two trees
BinomialTree<T>* BinomialHeap<T, E>::MergeTrees(BinomialTree<T>* src, BinomialTree<T>* dst)
{
	PrepareTrees(src, dst);
	//Merging rom src to dst
	BinomialTree<T>* tmp = dst->rightMostChild;
	//If there is none, we are merging two one order trees
	if (!tmp)
	{
		dst->rightMostChild = src;
		dst->order *= 2;
		return dst;
	}

	//Get to the end
	while (tmp->GreaterOrderTree)
		tmp = tmp->GreaterOrderTree;

	//Append to subtree
	tmp->GreaterOrderTree = src;
	if (src)
		src->LessOrderTree = tmp;
	//tail = src;

	//tmp->GreaterOrderTree = src;
	dst->order *= 2;
	//delete src;
	return dst;
}
template<class T, class E>
//Append tree to heap
void BinomialHeap<T, E>::LinkTree(BinomialTree<T>* t)
{
	BinomialTree<T>* end = tail;
	if (!end)
	{
		head = t;
		//BinomialTree<T>* tmp = t;
		while (t->GreaterOrderTree)
			t = t->GreaterOrderTree;

		tail = t;
		return;
	}
	//while (end->GreaterOrderTree)
	//	end = end->GreaterOrderTree;

	tail->GreaterOrderTree = t;
	if (t)
	{
		t->LessOrderTree = tail;

		while (t->GreaterOrderTree)
			t = t->GreaterOrderTree;

		tail = t;
	}
}
template<class T, class E>
BinomialHeap<T, E>* BinomialHeap<T, E>::PrepareOneProductAddHeap(T* data)
{
	BinomialHeap<T, E>* h = new BinomialHeap<T, E>(nullptr, m_comparer);
	h->head = new BinomialTree<T>(data);
	h->min = h->head;
	h->tail = h->head;
	h->data = nullptr;
	return h;
}
template<class T, class E>
void BinomialHeap<T, E>::Add(T* data)
{
	BinomialHeap<T, E>* h = PrepareOneProductAddHeap(data);
	if (!this->head)
	{
		head = h->head;
		min = h->head;
		tail = h->head;
		h->head->rightMostChild = nullptr;
		h->head->GreaterOrderTree = nullptr;
		h->head = nullptr;
		delete(h);
		return;
	}
	Merge(h);
	h->head = nullptr;
	delete h;
	m_minUpdated = false;
}
template<class T, class E>
void BinomialHeap<T, E>::Merge(BinomialHeap<T, E>* src)
{
	if (src == NULL || src->head == NULL)
		return;

	//Current trees to merge
	BinomialTree<T>* tSrc = src->head;
	BinomialTree<T>* tDst = head;
	//Disconnect current heap's head
	head = nullptr;
	tail = nullptr;
	//Pointers to next tree in each heap
	BinomialTree<T>* nextSrc = tSrc;
	BinomialTree<T>* nextDst = tDst;

	BinomialTree<T>* carryMerge = nullptr;
	//Something in both heaps
	while (tSrc && tDst)
	{
		//Keep the pointers to nextOrderTree in both heaps, when we are Merging trees, we destroy the connections
		nextSrc = tSrc->GreaterOrderTree;
		nextDst = tDst->GreaterOrderTree;
		//Carry flag
		if (carryMerge)
		{
			//Link and carry further
			if (tSrc->order == carryMerge->order && tDst->order == carryMerge->order)
			{
				LinkTree(carryMerge);
				carryMerge = MergeTrees(tSrc, tDst);
				tDst = nextDst;
				tSrc = nextSrc;
			}
			//Order of dst == to carry merge order, merge those and carry
			else if (tDst->order == carryMerge->order)
			{
				carryMerge = MergeTrees(tDst, carryMerge);
				//tDst to next
				tDst = nextDst;
			}
			//Order of src == to carry merge order, merge those and carry
			else if (tSrc->order == carryMerge->order)
			{
				carryMerge = MergeTrees(tSrc, carryMerge);
				//tSrc to next
				tSrc = nextSrc;
			}
			else
			{
				//Just link the carry
				LinkTree(carryMerge);
				carryMerge = nullptr;
			}
		}
		//No carry from last add
		else
		{
			//src has bigger degree than dst, just link dst to return heap
			if (tSrc->order > tDst->order)
			{
				//Destroy its connection
				UnlinkTree(tDst);
				//Link to return
				LinkTree(tDst);
				//Next...
				tDst = nextDst;
			}
			//Same as above
			else if (tSrc->order < tDst->order)
			{
				UnlinkTree(tSrc);
				LinkTree(tSrc);
				tSrc = nextSrc;
			}
			//Same orders => create carry merge
			else if (tSrc->order == tDst->order)
			{
				carryMerge = MergeTrees(tSrc, tDst);
				//next link
				tDst = nextDst;
				tSrc = nextSrc;
			}
		}
		//If there is a carry and one of the heaps ran out of trees, fake that carryMerge as the nextTree
		if (carryMerge)
		{
			if (tDst && !tSrc)
			{
				tSrc = carryMerge;
				carryMerge = nullptr;
			}
			else if (!tDst && tSrc)
			{
				tDst = carryMerge;
				carryMerge = nullptr;
			}
		}
	}

	BinomialTree<T>* continuing = NULL;
	if (tSrc)
		continuing = tSrc;
	else if (tDst)
		continuing = tDst;

	//One heap has bigger order trees, link her to end
	if (continuing)
		LinkTree(continuing);
	//If there is nothing to continue with, we could have carried the carryMerge, so just link it to the end
	else if (carryMerge)
		LinkTree(carryMerge);

	//End the heap
	LinkTree(nullptr);
}


class CChain;

class CProduct
{
public:
	CProduct(unsigned int id, unsigned int revenue, int chainId) : id(id), revenue(revenue), chainId(chainId)
	{

	}
	unsigned int id;
	unsigned int revenue;
	int chainId;
};
struct CData
{
	CData(int chain) :chainId(chain), chainIndex(-1) {}
	int chainId;
	int chainIndex;
};

int ProductsComparer(CProduct* p1, CProduct* p2)
{
	if (p1->revenue == p2->revenue)
	{
		if (p1->id > p2->id)
			return 1;
		else if (p1->id < p2->id)
			return -1;
		return 0;
	}

	if (p1->revenue > p2->revenue)
		return 1;
	else if (p1->revenue < p2->revenue)
		return -1;
	return 0;
}

class CChain
{
public:
	CChain(int id) : id(id), productsRoot(nullptr), min(nullptr), d(nullptr), selfNode(nullptr) {}
	int id;
	BinomialHeap<CProduct, CData>* productsRoot;
	void UpdateMerge(BinaryHeap<CChain, CData>* minsHeap)
	{
		minsHeap->BubbleUp(selfNode);
		minsHeap->BubbleDown(selfNode);
	}
	void AddProduct(CProduct* toAdd, BinaryHeap<CChain, CData>* minsHeap)
	{
		if (!productsRoot)
		{
			d = new CData(toAdd->chainId);
			productsRoot = new BinomialHeap<CProduct, CData>(d, ProductsComparer);
		}

		productsRoot->Add(toAdd);

		if (!selfNode)
			selfNode = minsHeap->Insert(this);


		if (!min)
		{
			min = toAdd;
			minsHeap->BubbleUp(selfNode);
		}
		else if (ProductsComparer(min, toAdd) == 1)
		{
			min = productsRoot->UpdateMin();
			minsHeap->BubbleUp(selfNode);
		}

	}
	void RemoveMinProduct(BinaryHeap<CChain, CData>* minsHeap)
	{
		min = productsRoot->UpdateMin();
		minsHeap->BubbleDown(selfNode);
	}
	~CChain()
	{
		delete productsRoot;
		delete d;
	}
	int chainIndex;
	CProduct* min;
	CData* d;
	BinaryHeapNode<CChain>* selfNode;
};

int ProductsComparerMins(CProduct* p1, CProduct* p2, CData* d1, CData* d2)
{
	if (!p1 && !p2)
		return 0;
	else if (!p1)
		return 1;
	else if (!p2)
		return -1;

	if (p1->revenue != p2->revenue)
	{
		if (p1->revenue > p2->revenue)
			return 1;
		else if (p1->revenue < p2->revenue)
			return -1;
		return 0;
	}
	if (d1->chainId != d2->chainId)
	{
		if (d1->chainId > d2->chainId)
			return 1;
		else if (d1->chainId < d2->chainId)
			return -1;
		return 0;
	}

	if (p1->id > p2->id)
		return 1;
	else if (p1->id < p2->id)
		return -1;
	return 0;
}

int ChainComparerMins(CChain* p1, CChain* p2)
{
	return ProductsComparerMins(p1->min, p2->min, p1->d, p2->d);
}
class CHolding
{
public:
	void Add(int chain, unsigned int id, unsigned int revenue);
	bool Remove(int chain, unsigned int& id);
	bool Remove(unsigned int& id);
	void Merge(int dstChain, int srcChain);
	void Merge(int dstChain, int srcChain, bool& res);
	bool Merge(void);
	AVLTree<CChain>* container = nullptr;
	BinaryHeap<CChain, CData>* minsProducts = nullptr;
	~CHolding()
	{
		delete minsProducts;
		delete container;
	}

};
int ChainComparer(CChain* c1, CChain* c2)
{
	if (c1->id > c2->id)
		return 1;
	else if (c1->id < c2->id)
		return -1;

	return 0;
}


void CHolding::Add(int chain, unsigned int id, unsigned int revenue)
{
	if (!container)
		container = new AVLTree<CChain>(ChainComparer);

	if (!minsProducts)
		minsProducts = new BinaryHeap<CChain, CData>(ChainComparerMins);

	CChain* toFind = new CChain(chain);

	AVLNode<CChain>* n = container->FindNode(toFind);
	CProduct* toAdd = new CProduct(id, revenue, chain);

	if (!n)
	{
		toFind->AddProduct(toAdd, minsProducts);
		n = container->AddNode(toFind);
	}
	else
	{
		n->data->AddProduct(toAdd, minsProducts);
		delete toFind;
	}
}


bool CHolding::Remove(int chain, unsigned int& id)
{
	if (!container)
		return false;

	if (!minsProducts)
		return false;

	CChain* dummyFind = new CChain(chain);
	AVLNode<CChain>* n = container->FindNode(dummyFind);

	if (!n || !n->data || !n->data->productsRoot)
	{
		delete(dummyFind);
		dummyFind = nullptr;
		return false;
	}

	//Get min from chains binomial heap
	CProduct* min = n->data->productsRoot->ExtractMin();

	//Empty heap
	if (!min)
	{
		delete dummyFind;
		return false;
	}


	id = min->id;
	n->data->RemoveMinProduct(minsProducts);
	delete min;
	delete dummyFind;
	dummyFind = nullptr;
	return true;
}

bool CHolding::Remove(unsigned int& id)
{
	if (!minsProducts)
		return false;

	CChain* res = minsProducts->GetMin();
	if (!res)
		return false;

	return Remove(res->id, id);
}


void CHolding::Merge(int dstChain, int srcChain)
{
	if (dstChain == srcChain)
		return;
	if (!container)
		return;
	CChain* dummyKey = new CChain(dstChain);
	AVLNode<CChain>* dst = container->FindNode(dummyKey);
	dummyKey->id = srcChain;
	AVLNode<CChain>* src = container->FindNode(dummyKey);

	delete dummyKey;

	if (!dst && !src)
	{
		return;
	}
	else if (!src || !src->data->productsRoot)
	{
		return;
	}
	else if (!dst || !dst->data->productsRoot)
	{
		if (!dst)
		{
			CChain* dummy = new CChain(dstChain);
			dummy->selfNode = minsProducts->Insert(dummy);
			dummy->min = nullptr;
			container->AddNode(dummy);
			dst = container->FindNode(dummy);
		}
		if (!dst->data->productsRoot)
		{
			CData* d = new CData(dstChain);
			dst->data->productsRoot = new BinomialHeap<CProduct, CData>(d, ProductsComparer);
		}
	}

	dst->data->productsRoot->Merge(src->data->productsRoot);
	dst->data->productsRoot->UpdateMin();
	dst->data->UpdateMerge(minsProducts);
	src->data->productsRoot->head = nullptr;
	delete src->data->productsRoot;
	src->data->productsRoot = nullptr;
	src->data->min = nullptr;
	src->data->UpdateMerge(minsProducts);


}


bool CHolding::Merge(void)
{
	if (!minsProducts)
		return false;

	CChain* c = minsProducts->GetMin();
	CChain* ch1 = minsProducts->GetLChild();
	CChain* ch2 = minsProducts->GetRChild();
	CChain* res = nullptr;

	if (ch1 && ch2)
	{
		if (ChainComparerMins(ch1, ch2) == 1)
			res = ch2;
		else
			res = ch1;
	}
	else if (ch1)
		res = ch1;
	else if (ch2)
		res = ch2;

	if (!c && !res)
		return false;
	else if (!c)
		return false;
	else if (!res)
		return false;
	else if (!c->productsRoot)
		return false;
	else if (!res->productsRoot)
		return false;

	if (c->id < res->id)
		Merge(c->id, res->id);
	else
		Merge(res->id, c->id);

	return true;
}