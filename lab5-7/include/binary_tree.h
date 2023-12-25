#pragma once

#include <compare>
#include <stdexcept>
#include <vector>

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
            _root = _copy_tree(rhs._root);
            return *this;
        }

        BinaryTree& operator=(BinaryTree&& rhs) noexcept {
            _root = rhs._root;
            return *this;
        }

        void insert(T key) {
            _root = _insert(_root, key);
        }

        bool search(T key) const {
            _Node* found = _search(_root, key);

            if (found == nullptr) return false;
            
            return true;
        }

        // // delete the node with all its children
        // void delete_node(T key) {
        //     _Node* node = _search(_root, key);
        //     if (node == nullptr) throw std::logic_error("Key not found");
        //     _delete_tree(node);
        // }

        std::vector<T> get_tops() {
            std::vector<T> tops;
            _get_tops(_root, tops);
            return tops;
        }

        std::vector<T> get_children(T key) {
            _Node* found = _search(_root, key);
            if (found == nullptr) throw std::logic_error("Key not found");
            std::vector<T> tops;
            _get_tops(found, tops);
            tops.pop_back();
            return tops;
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
        
        _Node* _insert(_Node* node, T key) {
            if (node == nullptr)
                return _new_node(key);

            if (key < node->key)
                node->left = _insert(node->left, key);
            else if (key > node->key)
                node->right = _insert(node->right, key);
        
            return node;
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

        _Node* _search(_Node* root, int key) const {
            if (root == nullptr || root->key == key)
                return root;

            if (root->key < key)
                return _search(root->right, key);

            return _search(root->left, key);
        }

        void _get_tops(_Node* root, std::vector<T>& tops) {
            if (root == nullptr) return;
            _get_tops(root->left, tops);
            _get_tops(root->right, tops);
            tops.push_back(root->key);
        }
    };
}
