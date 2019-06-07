#pragma once

#include <cassert>
#include <algorithm>

template <class T>
class Tree
{
	//Public methods
public:
	Tree();
	Tree(const Tree& other);

	~Tree();

	Tree& operator=(const Tree& other);

	//Some move related staff
	Tree(Tree&& other);
	Tree& operator=(Tree&& other);

	//Accessors
	bool empty()const;
	size_t size()const;
	bool find(const T& elem)const;
	void print()const;
	void PrintInorder()const;
	void PrintPreorder()const;
	void PrintPostorder()const;

	//Modifiers
	void insert(const T& elem);
	void remove(const T& elem);
	void clear();

private:
	struct Node
	{
		Node(T data = T(),
			size_t height = 1,
			Node* left = nullptr,
			Node* right = nullptr)
			: m_data{ data }
			, m_height{ height }
			, m_left{ left }
			, m_right{ right }
		{
		}

		T m_data;
		size_t m_height;
		Node* m_left;
		Node* m_right;
	};
	//Static methods
	static Node* copy(const Node* tree);
	static void clear(Node*& tree);
	static void print(const Node* tree, int indent);

	static bool find(const Node* tree, const T& elem);
	std::pair<Tree<T>::Node*, bool> insert(Node*& tree, const T& elem);
	std::pair<Tree<T>::Node*, bool> remove(Node*& tree, const T& elem);

	//Service methods
	Node* rightRotate(Node*& root);
	Node* leftRotate(Node*& root);

	size_t getHeight(Node* node) const;
	int getBalance(Node* node) const;

	void PrintPreorder(Node* ptr)  const;
	void PrintInorder(Node* ptr) const;
	void PrintPostorder(Node* ptr) const;

	//Member variables
	Node* 	m_root;
	size_t	m_size;
};

////////////////////////////////////////////////////////////////////////
//	   		      _ IMPLEMENTATION _                          //
////////////////////////////////////////////////////////////////////////

//_____ STATIC METHODS _
template<class T>
typename Tree<T>::Node* Tree<T>::copy(const Node* tree)
{
	// if tree is empty
	if (nullptr == tree) {
		// we are done
		return nullptr;
	}

	// construct copy of the root with
	return new Node(
		tree->m_info,
		copy(tree->m_left), // left subtree copied
		copy(tree->m_right)  // and right one
	);
}

template<class T>
void Tree<T>::clear(Node*& tree)
{
	// if tree is empty
	if (tree == nullptr)
	{
		// we are done
		return 0;
	}

	// clear left
	clear(tree->m_left);
	// clear left
	clear(tree->m_right);
	// delete root
	delete tree;
	// mark tree empty
	tree = nullptr;
}

template<class T>
void Tree<T>::print(const Node* tree, const int indent)
{
	// if tree is empty
	if (tree == nullptr)
	{
		// we are done
		return 0;
	}
	// some magic margin
	enum { margin = 5 };

	// print left with some indent plus margin
	print(tree->m_right, indent + margin);

	// emulate indentation	
	for (size_t i = 0; i < indent; ++i)
	{
		std::cout << ' ';
	}
	// print root
	std::cout << tree->m_info << std::endl;

	// print left subtree
	print(tree->m_left, indent + margin);
}

template<class T>
bool Tree<T>::find(const Node* tree, const T& elem)
{
	// if tree is empty
	if (tree == nullptr)
	{
		// tree doesn't contain elem for sure 
		return 0;
	}

	// if elem is equal to root element 
	if (tree->m_info == elem)
	{
		// say we are lucky
		return true;
	}

	// if elem compares less than root element
	if (elem < tree->m_info)
	{
		// it must be in left subtree
		return find(tree->m_left, elem);
	}

	// last hope is right subtree
	return find(tree->m_right, elem);
}

template<class T>
std::pair<typename Tree<T>::Node*, bool> Tree<T>::insert(Node*& root, const T& elem)
{
	if (nullptr == root)
	{
		root = new Node(elem);
		++m_size;
		return { root, true };
	}

	if (elem < root->m_data)
	{
		root->m_left = insert(root->m_left, elem).first;
	}
	else if (elem > root->m_data)
	{
		root->m_right = insert(root->m_right, elem).first;
	}
	else
	{
		return { root, false };
	}

	root->m_height = 1 + std::max(getHeight(root->m_left), getHeight(root->m_right));

	const int balance = getBalance(root);

	if ((balance > 1) && (elem < root->m_left->m_data))
	{
		return { rightRotate(root), true};
	}
	if ((balance > 1) && (elem > root->m_left->m_data))
	{
		root->m_left = leftRotate(root->m_left);
		return { rightRotate(root), true };
	}
	if ((balance < -1) && (elem > root->m_right->m_data))
	{
		return { leftRotate(root), true };
	}
	if ((balance < -1) && (elem < root->m_right->m_data))
	{
		root->m_right = rightRotate(root->m_right);
		return { leftRotate(root), true };
	}

	return { root, true };

}

template<class T>
std::pair<typename Tree<T>::Node*, bool>  Tree<T>::remove(Node*& root, const T& elem)
{

}

//_______SERVICE METHODS_______

template<class T>
void Tree<T>::PrintInorder(Node* ptr) const
{
}

template<class T>
typename inline Tree<T>::Node* Tree<T>::rightRotate(Node*& root)
{
	assert(root != nullptr);

	Node* newRoot = root->m_left;
	Node* tmp = newRoot->m_right;

	newRoot->m_right = root;
	root->m_left = tmp;

	root->m_height = 1 + std::max(getHeight(root->m_left), getHeight(root->m_right));
	newRoot->m_height = 1 + std::max(getHeight(newRoot->m_left), getHeight(newRoot->m_right));
	return newRoot;
}

template<class T>
typename inline Tree<T>::Node* Tree<T>::leftRotate(Node*& root)
{
	assert(root != nullptr);

	Node* newRoot = root->m_right;
	Node* tmp = newRoot->m_left;

	newRoot->m_left = root;
	root->m_right = tmp;

	root->m_height = 1 + std::max(getHeight(root->m_left), getHeight(root->m_right));
	newRoot->m_height = 1 + std::max(getHeight(newRoot->m_left), getHeight(newRoot->m_right));
	return newRoot;
}

template<class T>
inline size_t Tree<T>::getHeight(Node* node) const
{
	if (node == nullptr)
	{
		return 0;
	}
	return node->m_height;
}

template<class T>
inline int Tree<T>::getBalance(Node* node) const
{
	if (node == nullptr)
	{
		return 0;
	}

	return getHeight(node->m_left) - getHeight(node->m_right);
}

template<class T>
void Tree<T>::PrintPreorder(Node* ptr)const
{
}

template<class T>
void Tree<T>::PrintPostorder(Node *ptr) const
{
}

//______ PUBLIC METHODS __
template<class T>
Tree<T>::Tree()
{
	m_root = nullptr;
	m_size = 0;
}

template<class T>
Tree<T>::Tree(const Tree& other)
{
}

template<class T>
Tree<T>::~Tree()
{
}

template<class T>
Tree<T>& Tree<T>::operator=(const Tree& other)
{
}

template<class T>
bool Tree<T>::empty()const
{
}

template<class T>
size_t Tree<T>::size()const
{
}

template<class T>
bool Tree<T>::find(const T& elem)const
{
}

template<class T>
void Tree<T>::PrintInorder() const
{
}

template<class T>
void Tree<T>::PrintPreorder() const
{
}

template<class T>
void Tree<T>::PrintPostorder() const
{
}

template<class T>
void Tree<T>::insert(const T& elem)
{
	m_root = insert(m_root, elem).first;
}

template<class T>
void Tree<T>::remove(const T& elem)
{
	m_root = remove(m_root, elem);
}

template<class T>
void Tree<T>::print()const
{

}

template<class T>
void Tree<T>::clear()
{
	clear(m_root);
}
