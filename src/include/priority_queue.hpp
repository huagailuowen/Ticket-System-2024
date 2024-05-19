#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {

/**
 * a container like std::priority_queue which is a heap internal.
 */
template<typename T, class Compare = std::less<T>>
class priority_queue {
// public:
	struct Node{
		T val;
		Node *left, *right;
		Node(){left=right=nullptr;}
		Node(const T &val):val(val){left=right=nullptr;}
	};

	Node *root;
	int siz=0;
public:
	/**
	 * TODO constructors
	 */
	priority_queue() {root=nullptr;siz=0;}
	Node * queue_copy(Node *const node)
	{
		if(node == nullptr)
			return nullptr;
		Node *new_node = new Node(node->val);
		new_node->left = queue_copy(node->left);
		new_node->right = queue_copy(node->right);
		return new_node;
	}
	priority_queue(const priority_queue &other) {
		root = queue_copy(other.root);
		siz = other.siz;
	}
	/**
	 * TODO deconstructor
	 */
	void queue_delete(Node *node)
	{
		if(node == nullptr)
			return;
		queue_delete(node->left);
		queue_delete(node->right);
		delete node;
	}
	~priority_queue() {
		queue_delete(root);
	}
	/**
	 * TODO Assignment operator
	 */
	priority_queue &operator=(const priority_queue &other) {
		if(this == &other)
			return *this;
		queue_delete(root);
		root = queue_copy(other.root);
		siz = other.siz;
		return *this;
	}
	/**
	 * get the top of the queue.
	 * @return a reference of the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	const T & top() const {
		if(root == nullptr)
			throw container_is_empty();
		return root->val;
	}
	/**
	 * TODO
	 * push new element to the priority queue.
	 */
	Node * queue_merge(Node *first, Node *second)
	{
		if(first == nullptr)
			return second;
		if(second == nullptr)
			return first;
		if(Compare()(first->val,second->val))
			std::swap(first,second);
		// try{
		first->right = queue_merge(first->right,second);
		// }catch(...){
		// 	throw "!!";
		// }
		std::swap(first->left,first->right);
		return first;
	}
	void push(const T &e) {
		Node *new_node = new Node(e);
		if(root == nullptr)
		{
			root = new_node;
			siz++;
			return;
		}
		try{
			root =queue_merge(root,new_node);
		}catch(...){
			delete new_node;
			return;
		}
		siz++;
	}
	/**
	 * TODO
	 * delete the top element.
	 * throw container_is_empty if empty() returns true;
	 */
	void pop() {
		if(root == nullptr)
			throw container_is_empty();
		Node *left = root->left;
		Node *right = root->right;
		Node *target=root;
		try{
			root = queue_merge(left,right);
		}catch(...){
			return;
		}
		delete target;
		siz--;
	}
	/**
	 * return the number of the elements.
	 */
	size_t size() const {
		return siz;
	}
	/**
	 * check if the container has at least an element.
	 * @return true if it is empty, false if it has at least an element.
	 */
	bool empty() const {
		return root == nullptr;
	}
	/**
	 * merge two priority_queues with at most O(logn) complexity.
	 * clear the other priority_queue.
	 */
	void merge(priority_queue &other) {
		try{
			root = queue_merge(root,other.root);
		}catch(...){
			return;
		}
		siz += other.siz;
		other.root = nullptr;
		other.siz = 0;
	}
};

}

#endif
