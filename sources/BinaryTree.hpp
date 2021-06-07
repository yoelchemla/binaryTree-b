#pragma once
#include <memory>
#include <iostream>
#include <stack>
namespace ariel
{
    template <typename T>
    class BinaryTree
    { 
        class Node
        {
            friend class BinaryTree;
            T data;
            Node *parent;
            Node *right;
            Node *left;
        public:
            T get_data(){//getter
                return data;
            }
            Node(T val) : data(val), right(nullptr), left(nullptr), parent(nullptr){};//initialization list
            bool operator==(Node copy){
                return this->data == copy->data && this->left == copy->left &&this->right == copy.right && this->parent == copy->parent;
            }
        };
        Node *root;
        enum Order
        {
            preorder,inorder,postorder
        };
        class Iterator
        {
            Node *curr_ptr;
            Order order1;
            std::stack<Node*> stk;

        public:  
            Iterator() : curr_ptr(nullptr){};
            Iterator(const Order order1, Node *node=nullptr) : curr_ptr(node) ,order1(order1){
               init_Stack(node);
                if (!stk.empty()){
                    curr_ptr = stk.top();
                    stk.pop();
                }
            }
            Node *get_curr(){
                return curr_ptr;
            }
            void init_Stack(Node *node){
                if (node == nullptr){return;} //break
                if (order1 == preorder){
                    init_Stack(node->right);
                    init_Stack(node->left);
                    stk.push(node);
                }
                else if (order1 == postorder){
                     stk.push(node);
                    init_Stack(node->right);
                    init_Stack(node->left);
                }
                else {// inorder  
                   init_Stack(node->right); 
                    stk.push(node); 
                    init_Stack(node->left); 
                }
            }
            //prefix
            Iterator operator++(int){
                Iterator temp = *this;
                if (!stk.empty()){
                 curr_ptr = stk.top();
                 stk.pop();
                }
                else{ //is empty
                curr_ptr = nullptr;
                }
                return temp;
            }
            //postfix
            Iterator &operator++(){
                if (!stk.empty()){ //the stack isn't empty
                curr_ptr = stk.top();
                stk.pop();
                }
                else {  // the stack is empty
                curr_ptr = nullptr;
                }
                return *this;
            }
            T &operator*() const{
                return curr_ptr->data;
            }
            T *operator->() const{
                return &(curr_ptr->data);
            }
            bool operator==(const Iterator &ans) const{
                return curr_ptr == ans.curr_ptr;
            }
            bool operator!=(const Iterator &ans) const{
                return !(*this == ans);
            }
        };
       //struct of the tree
    public:
        BinaryTree<T>() : root(nullptr){};
        void copy_constructor(Node *node, const Node *other_node){
            
            if (other_node->right != nullptr){
                node->right = new Node(other_node->right->data);
                copy_constructor(node->right, other_node->right);
            }
            if (other_node->left != nullptr){
                node->left = new Node(other_node->left->data);
                copy_constructor(node->left, other_node->left);
            }
        }
        BinaryTree<T> &operator=(const BinaryTree<T> &btree){
            if (root != nullptr){
                delete root;
            }
            if (btree.root != nullptr){
                root = new Node{btree.root->data};
                copy_constructor(root, btree.root);
            }
            if (this == &btree){
                return *this;
            }
            return *this;
        }
        BinaryTree<T>(const BinaryTree<T> &btree){ 
            if (btree.root != nullptr){
                this->root = new Node(btree.root->data);
                copy_constructor(root, btree.root);
            }
        }
        BinaryTree<T> &operator=(BinaryTree<T> &&btree){
            if (root){
            delete root;
            }
            root = btree.root;
            btree.root = nullptr;
            return *this;
        }
        BinaryTree(BinaryTree<T> &&btree){
            this->root = btree.root;
            btree.root = nullptr;
        }
        ~BinaryTree<T>(){ //destructor
            if (root){
                for (auto it = (*this).begin_postorder(); it != (*this).end_postorder(); ++it){
                    delete it.get_curr();
                }
            }
        }
        friend std::ostream &operator<<(std::ostream &os, const BinaryTree<T> &t){ //output
            os << t.root->data;
            return os;
        }
        //method add root
        BinaryTree<T> &add_root(T data){
            if (root == nullptr){ // not exist -> new root            
                this->root = new Node(data);
            }   
            else{ // exist -> update
              root->data = data;
            }
            return *this;
        }
        Node *find_node(Node *node, T val){
            if (node == nullptr){ // the pointer is null 
                return nullptr;
            }
            for (auto iter = begin_inorder(); iter != end_inorder(); ++iter){
                if (*iter == val){
                    return iter.get_curr();
                }
            }
            return nullptr;
        }
        //add right method
        BinaryTree<T> &add_right(T parent_value, T child_value){
            Node *f = find_node(this->root, parent_value);
            if (f == nullptr){ // the pointer is null
                throw std::invalid_argument("didn't founded");
            }
            if (f->right == nullptr){ //didn't have a right son
                f->right = new Node(child_value);
                f->right->parent = f; // Update the parent
            }
            else {// If the son exist
                f->right->data = child_value;
            }
            return *this;
        }
        //add left method
        BinaryTree<T> &add_left(T parent_value, T child_value){
            Node *f = find_node(this->root, parent_value);
            if (f == nullptr) {
             throw std::invalid_argument("didn't founded"); //exception
            }
            if (f->left == nullptr){ //not have a left son
                f->left = new Node(child_value);
                f->left->parent = f; // Update the parent
            }
            else {// If the son exist
                f->left->data = child_value;
            }
            return *this;
        }
        Iterator begin(){
            return Iterator{inorder,root};
        };
        Iterator end(){
            return Iterator{};
        };
        Iterator begin_preorder(){
            return Iterator{preorder,root};
        } 
        Iterator end_preorder(){
            return Iterator{};
        }
        Iterator begin_inorder(){
            return Iterator{inorder, root};
        }
        Iterator end_inorder(){
            return Iterator{};
        }
        Iterator begin_postorder(){
            return Iterator{ postorder, root};
        }
        Iterator end_postorder(){
            return Iterator{};
        }
    };
}
