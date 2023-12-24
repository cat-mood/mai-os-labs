#pragma once

#include <compare>
#include <stdexcept>

namespace mysys {
    template <class T>
    requires std::three_way_comparable<T>
    class BinaryTree {
    public:
        BinaryTree() : _root{nullptr} {}

        BinaryTree(const BinaryTree& other) {
            _root = _copy_tree(other->_root);
        }

        BinaryTree(BinaryTree&& other) noexcept {
            _root = other._root;
        }

        ~BinaryTree() noexcept {
            _delete_tree(_root);
        }

        BinaryTree& operator=(const BinaryTree& rhs) {
            _root = _copy_tree(rhs->_root);
            return *this;
        }

        BinaryTree& operator=(BinaryTree&& rhs) noexcept {
            _root = rhs->_root;
            return *this;
        }

        // returns parent id
        int insert(T key) {
            _Node* parent = _insert(_root, nullptr, key);
            if (parent == nullptr) return 0;
            return parent->key;
        }

        bool search(T key) const {
            _Node* found = _search(_root, key);

            if (found == nullptr) return false;
            
            return true;
        }

        // delete the node with all its children
        void delete_node(T key) {
            _Node* node = _search(_root, key);
            if (node == nullptr) throw std::logic_error("Key not found");
            _delete_tree(node);
        }

    private:
        struct _Node {
            T key;
            _Node* left;
            _Node* right;
        };

        _Node* _root;

        _Node* _new_node(T item) {
            _Node* temp = new _Node;
            temp->key = item;
            temp->left = temp->right = nullptr;
            return temp;
        }
        
        // returns pointer to parent of new node
        _Node* _insert(_Node* node, _Node* parent, T key) {
            if (node == nullptr)
                return _new_node(key);

            if (key < node->key)
                node->left = _insert(node->left, node, key);
            else if (key > node->key)
                node->right = _insert(node->right, node, key);
        
            return parent;
        }

        void _delete_tree(_Node* node) noexcept {
            if (node == nullptr) return;
            _Node* left = node->left;
            _Node* right = node->right;
            delete node;
            _delete_tree(left);
            _delete_tree(right);
        }

        _Node* _create_tree(_Node* left, _Node* right, T key) {
            _Node* root = _new_node(key);
            root->left = left;
            root->right = right;
            return root;
        }

        _Node* _copy_tree(_Node* other) {
            if (other == nullptr) return nullptr;
            _Node* root = _new_node(other->key);
            root->left = _copy_tree(other->left);
            root->right = _copy_tree(other->right);
            return root;
        }

        _Node* _search(_Node* root, int key) {
            if (root == nullptr || root->key == key)
                return root;

            if (root->key < key)
                return search(root->right, key);

            return search(root->left, key);
        }
    };
}
