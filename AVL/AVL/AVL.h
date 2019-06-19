#pragma once

#include <cassert>
#include <algorithm>

template <class T>
class Tree
{
private:
	struct Node
	{
		Node(T data = T(),
			size_t height = 1,
			Node* left = nullptr,
			Node* right = nullptr,
			Node* parent = nullptr)
			: m_data{ data }
			, m_height{ height }
			, m_left{ left }
			, m_right{ right }
			, m_parent{ parent }
		{
		}

		T m_data;
		size_t m_height;
		Node* m_left;
		Node* m_right;
		Node* m_parent;
	};
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
	bool empty() const;
	size_t size() const;
	bool find(const T& elem) const;
	void PrintInorder() const;
	void PrintPreorder() const;
	void PrintPostorder() const;

	// Iterators
	class Iterator
	{
	public:
		Iterator(Node* current, Node* prev = nullptr);
		Iterator(const Iterator& other);
		Iterator(Iterator&& other);
		~Iterator() = default;

		Iterator& operator=(const Iterator& other);
		Iterator& operator=(Iterator&& other);

		Iterator& operator++();
		Iterator operator++(int);

		T& operator*() const;
		Node* node() const;


		bool operator==(const Iterator& other) const;
		bool operator!=(const Iterator& other) const;

		static Iterator begin(Node* rootNode);
		static Iterator end();

	private:
		Node* m_current;
		Node* m_prev;
	};

	Iterator begin();
	Iterator end();

	//Modifiers
	void insert(const T& elem);
	bool remove(Iterator& it);
	void clear();

private:
	//Static methods
	static Node* copy(const Node* tree);
	static void clear(Node*& tree);
	static void print(const Node* tree, int indent);

	static bool find(const Node* tree, const T& elem);

	std::pair<Tree<T>::Node*, bool> insert(Node*& tree, const T& elem);
	std::pair<Tree<T>::Node*, bool>  remove(Node*& tree, const T& elem);

	//Service methods
	Node* rightRotate(Node*& root);
	Node* leftRotate(Node*& root);
	Node* minValueNode(Node*& root);

	size_t getHeight(Node* node) const;
	int getBalance(Node* node) const;

	void PrintInorder(Node* node) const;
	void PrintPreorder(Node* node)  const;
	void PrintPostorder(Node* node) const;

	//Member variables
	Node* 	m_root;
	size_t	m_size;
};

///////////////////////////////////////////////////////////////////////
//	   		      _ IMPLEMENTATION _								  //
///////////////////////////////////////////////////////////////////////

//_____ STATIC METHODS _
template<class T>
typename Tree<T>::Node* Tree<T>::copy(const Node* tree)
{
	// if tree is empty
	if (nullptr == tree)
	{
		// we are done
		return nullptr;
	}

	// construct copy of the root with
	return new Node(
		tree->m_info,
		tree->m_height,
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
		return { rightRotate(root), true };
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
inline std::pair<typename Tree<T>::Node*, bool>  Tree<T>::remove(Node*& root, const T& elem)
{
	if (elem < root->m_data)
	{
		if (root->m_left == nullptr)
		{
			return {nullptr, false };
		}

		root->m_left = remove(root->m_left, elem).first;
	}
	else if (elem > root->m_data)
	{
		if (root->m_right == nullptr)
		{
			return { nullptr, false };
		}

		root->m_right = remove(root->m_right, elem).first;
	}
	else
	{
		if ((root->m_left == nullptr) ||
			(root->m_right == nullptr))
		{
			Node* temp = root->m_left ?
				root->m_left :
				root->m_right;

			if (temp == nullptr)
			{
				temp = root;
				root = nullptr;
			}
			else
			{
				*root = *temp;
			}  

			delete temp;
		}
		else
		{
			Node* temp = minValueNode(root->m_right);

			root->m_data = temp->m_data;

			root->m_right = remove(root->m_right, temp->m_data).first;
		}
	}

	if (root == nullptr)
	{
		return { root, true };
	}

	root->m_height = 1 + std::max(
		getHeight(root->m_left), 
		getHeight(root->m_right));

	const int balance = getBalance(root);

	if (balance > 1 && getBalance(root->m_left) >= 0)
	{
		return { rightRotate(root), true };
	}
	if (balance > 1 && getBalance(root->m_left) < 0)
	{
		root->m_left = leftRotate(root->m_left);
		return { rightRotate(root), true };
	}
	if (balance < -1 && getBalance(root->m_right) <= 0)
	{
		return { leftRotate(root), true };
	}
	if (balance < -1 && getBalance(root->m_right) > 0)
	{
		root->m_right = rightRotate(root->m_right);
		return { leftRotate(root), true };
	}

	return { root, true };
}

//_______SERVICE METHODS_______

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
inline typename Tree<T>::Node* Tree<T>::minValueNode(Node*& root)
{
	while (root->m_left != nullptr)
	{
		root = root->m_left;
	}

	return root;
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
void Tree<T>::PrintInorder(Node* node) const
{
	if (node->m_left != nullptr)
	{
		postOrder(node->m_left);
	}

	std::cout << node->data << " ";

	if (node->m_right != nullptr)
	{
		postOrder(node->m_right);
	}
}

template<class T>
void Tree<T>::PrintPreorder(Node* node) const
{
	std::cout << node->data << " ";

	if (node->m_left != nullptr)
	{
		postOrder(node->m_left);
	}

	if (node->m_right != nullptr)
	{
		postOrder(node->m_right);
	}
}

template<class T>
void Tree<T>::PrintPostorder(Node* node) const
{
	if (node->m_left != nullptr)
	{
		postOrder(node->m_left);
	}

	if (node->m_right != nullptr)
	{
		postOrder(node->m_right);
	}

	std::cout << node->data << " ";
}

//______ PUBLIC METHODS __

template<class T>
Tree<T>::Tree()
	: m_root(nullptr)
	, m_size(0)
{
}

template<class T>
Tree<T>::Tree(const Tree& other)
	: m_root(other.m_root)
	, m_size(other.m_size)
{
}

template<class T>
inline Tree<T>::~Tree()
{
	auto it = begin();

	while (it != end())
	{
		delete (it++).node();
	}
}

template<class T>
Tree<T>& Tree<T>::operator=(const Tree& other)
{
	if (this == &other)
	{
		return *this;
	}

	m_root = other.m_root;
	m_size = other.m_size;

	return *this;
}

template<class T>
inline Tree<T>::Tree(Tree&& other)
	: m_root(std::move(other.m_root))
	, m_size(std::move(other.m_size))
{
	other.m_root = nullptr;
}

template<class T>
inline Tree<T>& Tree<T>::operator=(Tree&& other)
{
	if (this == &other)
	{
		return *this;
	}

	m_root = other.m_root;
	m_size = other.m_size;

	other.m_root = nullptr;

	return *this;
}

template<class T>
bool Tree<T>::empty() const
{
	return (m_size == 0);
}

template<class T>
size_t Tree<T>::size() const
{
	return m_size;
}

template<class T>
bool Tree<T>::find(const T& elem) const
{
	if (nullptr == m_root)
	{
		return false;
	}

	return find(m_root, elem);
}

template<class T>
void Tree<T>::PrintInorder() const
{
	if (nullptr == m_root)
	{
		return;
	}

	return PrintInorder(m_root);
}

template<class T>
void Tree<T>::PrintPreorder() const
{
	if (nullptr == m_root)
	{
		return;
	}

	return PrintPreorder(m_root);
}

template<class T>
void Tree<T>::PrintPostorder() const
{
	if (nullptr == m_root)
	{
		return;
	}

	return PrintPostorder(m_root);
}

template<class T>
void Tree<T>::insert(const T& elem)
{
	m_root = insert(m_root, elem).first;
}

template<class T>
bool Tree<T>::remove(Iterator& it)
{
	if (m_root == nullptr)
	{
		return false;
	}

	Tree<T>::Node* currentNode = it.node();

	return remove(m_root, currentNode->m_data).second;
}

template<class T>
void Tree<T>::clear()
{
	clear(m_root);
}


////////////////////////////////////////////////////////////////////////
//	   						_ ITERATOR _							  //
////////////////////////////////////////////////////////////////////////

template<class T>
inline Tree<T>::Iterator::Iterator(Node* current, Node* prev)
	: m_current(current)
	, m_prev(prev)
{
}

template<class T>
inline Tree<T>::Iterator::Iterator(const Iterator& other)
	: m_current(other.m_current)
	, m_prev(other.m_prev)
{
}

template<class T>
inline Tree<T>::Iterator::Iterator(Iterator&& other)
	: m_current(std::move(other.m_current))
	, m_prev(std::move(other.m_prev))
{
	other.m_current = nullptr;
	other.m_prev = nullptr;
}

template<class T>
inline typename Tree<T>::Iterator& Tree<T>::Iterator::operator=(const Iterator& other)
{
	if (this == &other)
	{
		return *this;
	}

	m_current = other.m_current;
	m_prev = other.m_prev;

	other.m_current = nullptr;
	other.m_prev = nullptr;

	return *this;
}

template<class T>
inline typename Tree<T>::Iterator& Tree<T>::Iterator::operator=(Iterator&& other)
{
	if (this == &other)
	{
		return *this;
	}

	m_current = other.m_current;
	m_prev = other.m_prev;

	other.m_current = nullptr;
	other.m_prev = nullptr;

	return *this;
}

template<class T>
inline typename Tree<T>::Iterator& Tree<T>::Iterator::operator++()
{
	if (m_current == nullptr)
	{
		return *this;
	}

	Tree<T>::Node* parent = m_current->m_parent;

	if (parent == nullptr)
	{
		m_current = nullptr;
		return *this;
	}

	if ((m_current == parent->m_left) && (parent->m_right != nullptr))
	{
		m_current = parent->m_right;
	}
	else
	{
		m_current = parent;
		return *this;
	}

	while (true)
	{
		if (m_current->m_left != nullptr)
		{
			m_current = m_current->m_left;
		}
		else if (m_current->m_right != nullptr)
		{
			m_current = m_current->m_right;
		}
		else
		{
			return *this;
		}
	}

	assert(false);
}

template<class T>
inline typename Tree<T>::Iterator Tree<T>::Iterator::operator++(int)
{
	Tree<T>::Iterator old = *this;

	++(*this);

	return old;
}

template<class T>
inline T& Tree<T>::Iterator::operator*() const
{
	return m_current.m_data;
}

template<class T>
inline typename Tree<T>::Node* Tree<T>::Iterator::node() const
{
	return m_current;
}

template<class T>
inline bool Tree<T>::Iterator::operator==(const Iterator& other) const
{
	return (m_current == other.m_current && m_prev == other.m_prev);
}

template<class T>
inline bool Tree<T>::Iterator::operator!=(const Iterator& other) const
{
	return (m_current != other.m_current || m_prev != other.m_prev);
}

template<class T>
inline typename Tree<T>::Iterator Tree<T>::Iterator::begin(Node * rootNode)
{
	if (rootNode == nullptr)
	{
		return Iterator(rootNode);
	}

	while (true)
	{
		if (rootNode->m_left != nullptr)
		{
			rootNode = rootNode->m_left;
		}
		else if (rootNode->m_right != nullptr)
		{
			rootNode = rootNode->m_right;
		}
		else
		{
			return Iterator(rootNode);
		}
	}

	assert(false);
}

template<class T>
inline typename Tree<T>::Iterator Tree<T>::Iterator::end()
{
	return nullptr;
}

template<class T>
inline typename Tree<T>::Iterator Tree<T>::begin()
{
	return Tree<T>::Iterator::begin(m_root);
}

template<class T>
inline typename Tree<T>::Iterator Tree<T>::end()
{
	return Tree<T>::Iterator::end();
}
