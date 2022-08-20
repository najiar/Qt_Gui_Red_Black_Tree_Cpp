#ifndef RBTree_H_
#define RBTree_H_

#include <iostream>
#include <QPainter>
#include <QStack>
#include <QQueue>
#include <string>

class Node
{
    friend class RBTree;

    public:
            Node(); // Node constructor
            ~Node(); // node destructor
    private:
            int data; // hold the value of the node
            int x; // used for graphical position of the node
            Node *left_chld; // pointer to left child
            Node *right_chld; // pointer to right child
            Node *parent; // pointer to parent
            bool colour; // Nodes colour
};
// Class Node

// Class RBTree

class RBTree
{
    public:
            RBTree(); // constructor
            ~RBTree(); // destructor
            QString get_preorder_trav() const; // preorder travarsel of the tree used to save tree to a file
            bool is_empty() const; // check if its empty
            bool insert(const int & val); // inserts a node acording to bst alogothim and calls balance_insert afterwards
            bool delete_value(int val); // calls delete_node to delete the value
            void rbt_reset(); // resets the tree
            void draw(QPainter *painter, double &scale); //draws the tree
            int get_y() const; // gets the y coordinate of the tree
            int get_x() const; // gets the x coordinate of the tree

    private:
            QPainter *painter;
            Node *root; // pointer of type node to hold the root of the tree
            Node *LEAF; // pointer of type node to represent a leaf node
            int yspace; // y coordinate
            int xspace; // x coordinate
            int radius; // radius
            double scale; // scale

            int max(int a, int b) const; // returns the max element
            int recursive_height(const Node* node) const; // recursivly calculate the heigt of the tree
            int get_tree_height() const; // returns the height of the tree
            Node* get_left_node(Node* node) const; // get the leftmost node
            Node* min_node(Node* in_node); // get the min node
            Node* max_node(Node* in_node); // get the max node
            int get_level(Node* node); // get the level of a node
            int get_lft_tree(const Node* node); // get the rightmost node in the left subtree
            int get_anc_loc(const Node* node);  // returns the position of the next node to be drawn
            void reset_pos(Node* node); // reset the possition of the nodes
            void rec_node_del(const Node* node); // recursicly delete nodes
            void rec_draw(Node* node); // recursivly draw node
            void balance_insert(Node* in_node); // balances the tree after the insert function
            void left_rotate(Node* x); // left rotates at a given node
            void right_rotate(Node* x); // right rotate at a give node
            void stitch_tree(Node* deleted, Node* stiched); // connects the grandparent with child(after deleting a node)
            void balance_delete(Node * x); // balances the tree after the delete function
            void delete_node(Node* in_node, double in_val); // deletes the node acording to bst alogorithm and calls balance delete if needed
};
#endif /* RBTree_H_ */
