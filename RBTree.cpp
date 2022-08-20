#include "RBTree.h"

Node::Node()
{
    data = 0;
    parent = nullptr;
    left_chld = nullptr;
    right_chld = nullptr;
    colour = 0;
    x = 0;
}
// node default cons

Node::~Node()
{
    data = 0;
    parent = nullptr;
    left_chld = nullptr;
    right_chld = nullptr;
    colour = 0;
    x = 0;
}
// node destructor

int RBTree::max(int a, int b) const
{
    return a > b ? a : b;
}
// max element

RBTree::RBTree()
{
    scale = 1;
    LEAF = new Node;
    LEAF->colour = 0;
    LEAF->left_chld = nullptr;
    LEAF->right_chld = nullptr;
    root = LEAF;
}
// RBTree default cons

RBTree::~RBTree()
{
    // post order traverse to delete all nodes
    rec_node_del(root);
    this->root = nullptr;
}
// RBTree destructor

int RBTree::recursive_height(const Node *node) const
{
    if (node == LEAF || (node->left_chld == LEAF && node->right_chld == LEAF))
    {
        return 0;
    }

    return 1 + max(recursive_height(node->left_chld),
                   recursive_height(node->right_chld));
}
// recursive_height


bool RBTree::is_empty() const
{
    return root == LEAF;
}
// is_empty

void RBTree::left_rotate(Node *x)
{
    Node *y = x->right_chld;//lock x's right child pointer
    x->right_chld = y->left_chld;//assign y's left child to x's right child
    if (y->left_chld != LEAF)//if y's left cild is not leaf, set x as it's parent
    {
        y->left_chld->parent = x;
    }
    y->parent = x->parent;//set x's parent as y's parent
    if (x->parent == nullptr)//if x is root, set y as root
    {
        this->root = y;
    }
    else if (x == x->parent->left_chld)//else, if x is left child of it's parent, set y as left child of x's parent
    {
        x->parent->left_chld = y;
    }
    else//else, if x is right child of it's parent, set y as right child of x's parent
    {
        x->parent->right_chld = y;
    }
    y->left_chld = x;//set x as y's left child
    x->parent = y;//set y as x's parent
}
// left_rotate
void RBTree::right_rotate(Node *x)
{
    Node *y = x->left_chld;//lock x's left child pointer
    x->left_chld = y->right_chld;//assign y's right child to x's right child
    if (y->right_chld != LEAF)//if y's right cild is not leaf, set x as it's parent
    {
        y->right_chld->parent = x;
    }
    y->parent = x->parent;//set x's parent as y's parent
    if (x->parent == nullptr)//if x is root, set y as root
    {
        this->root = y;
    }
    else if (x == x->parent->right_chld)//else, if x is right child of it's parent, set y as right child of x's parent
    {
        x->parent->right_chld = y;
    }
    else//else, if x is left child of it's parent, set y as left child of x's parent
    {
        x->parent->left_chld = y;
    }
    y->right_chld = x;//set x as y's rightt child
    x->parent = y;//set y as x's parent
}
// right_rotate
void RBTree::balance_insert(Node *in_node)
{
    Node *u;
    while (in_node->parent->colour == 1)
    {
        if (in_node->parent == in_node->parent->parent->right_chld)//in_node is on the right side
        {
            u = in_node->parent->parent->left_chld; // uncle node pointer
            if (u->colour == 1)//case 1.1
            {
                u->colour = 0;
                in_node->parent->colour = 0;
                in_node->parent->parent->colour = 1;
                in_node = in_node->parent->parent;
            }
            else//case 1.2
            {
                if (in_node == in_node->parent->left_chld)//case 1.2.1
                {
                    in_node = in_node->parent;
                    right_rotate(in_node);
                }
                in_node->parent->colour = 0;//case 1.2.2
                in_node->parent->parent->colour = 1;
                left_rotate(in_node->parent->parent);
            }
        }
        else//in_node is on the left side
        {
            u = in_node->parent->parent->right_chld; // uncle
            if (u->colour == 1)//case 1.1
            {
                u->colour = 0;
                in_node->parent->colour = 0;
                in_node->parent->parent->colour = 1;
                in_node = in_node->parent->parent;
            }
            else//case 1.2
            {
                if (in_node == in_node->parent->right_chld)//case 1.2.3
                {
                    in_node = in_node->parent;
                    left_rotate(in_node);
                }
                in_node->parent->colour = 0;//case 1.2.4
                in_node->parent->parent->colour = 1;
                right_rotate(in_node->parent->parent);
            }
        }
        if (in_node == root)//if root is reached, break
        {
            break;
        }
    }
    root->colour = 0;//colour the root black
}
// balance_insert

bool RBTree::insert(const int &val)
{
    Node *in_node = new Node; //alocate new node and initialize it
    in_node->parent = nullptr;
    in_node->data = val;
    in_node->left_chld = LEAF;
    in_node->right_chld = LEAF;
    in_node->colour = 1;

    Node *y = nullptr;//tracking of parent
    Node *x = this->root; // tracking current pos, we start from root

    while (x != LEAF) //while we dont get to leaf node, look where to put new node
    {
        y = x; //follow the parrent
        if (in_node->data < x->data) //if its smaller
        {
            x = x->left_chld; //go left
        }
        else //if its bigger
        {
            x = x->right_chld; //go right
        }
    }

    in_node->parent = y; //we set the parrent of the new node

    if (y == nullptr)
    {
        root = in_node; // we set the new node to be root
    }
    else if (in_node->data < y->data) // left or right of the parent
    {
        y->left_chld = in_node;
    }
    else
    {
        y->right_chld = in_node;
    }

    if (in_node->parent == nullptr) //if its the first node
    {
        in_node->colour = 0; //we set it to be black
        return 0;
    }

    // balance the tree
    balance_insert(in_node);

    return true; // Successful
}

void RBTree::rbt_reset()
{
    // delete each node
    rec_node_del(root);
    root = LEAF;
    return;
}
int RBTree::get_tree_height() const
{
    if (this->is_empty())
    {
        return 0;
    }
    return recursive_height(root);
}
void RBTree::stitch_tree(Node *deleted, Node *stiched)
{
    if (deleted->parent == nullptr) //We check if the parent of the node is root
    {
        root = stiched; //child becomes the root
    }
    else if (deleted == deleted->parent->left_chld) // stiched node is it’s parent’s left child
    {
        deleted->parent->left_chld = stiched;
    }
    else //stiched node is it’s parent’s right child
    {
        deleted->parent->right_chld = stiched;
    }

    stiched->parent = deleted->parent;
}
// stitch_tree

void RBTree::balance_delete(Node *x)
{
    Node *s;
    while (x != root && x->colour == 0) // Case 3
    {
        if (x == x->parent->left_chld) // cases if x's sibling is right child
        {
            s = x->parent->right_chld;
            if (s->colour == 1) // Case 3.1
            {
                s->colour = 0;
                x->parent->colour = 1;
                left_rotate(x->parent);
                s = x->parent->right_chld;
            }
            if (s->left_chld->colour == 0 && s->right_chld->colour == 0) // case 3.2
            {
                s->colour = 1;
                x = x->parent;
            }
            else
            {
                if (s->right_chld->colour == 0) // case 3.3
                {
                    s->left_chld->colour = 0;
                    s->colour = 1;
                    right_rotate(s);
                    s = x->parent->right_chld;
                }
                s->colour = x->parent->colour; // case 3.4
                x->parent->colour = 0;
                s->right_chld->colour = 0;
                left_rotate(x->parent);
                x = root;
            }
        }
        else // cases if x's sibling is left child
        {
            s = x->parent->left_chld;
            if (s->colour == 1) // case 3.1, mirrored
            {
                s->colour = 0;
                x->parent->colour = 1;
                right_rotate(x->parent);
                s = x->parent->left_chld;
            }
            if (s->right_chld->colour == 0 && s->right_chld->colour == 0)// case 3.2, mirrored
            {
                s->colour = 1;
                x = x->parent;
            }
            else
            {
                if (s->left_chld->colour == 0)// case 3.3, mirrored
                {
                    s->right_chld->colour = 0;
                    s->colour = 1;
                    left_rotate(s);
                    s = x->parent->left_chld;
                }
                s->colour = x->parent->colour;// case 3.4, mirrored
                x->parent->colour = 0;
                s->left_chld->colour = 0;
                right_rotate(x->parent);
                x = root;
            }
        }
    }
    x->colour = 0;//finally, colour x black
}
// balance_delete
Node *RBTree::min_node(Node *in_node)
{
    while (in_node->left_chld != LEAF)
    {
        in_node = in_node->left_chld;
    }
    return in_node;
}
// min

Node *RBTree::max_node(Node *in_node)
{
    while (in_node->right_chld != LEAF)
    {
        in_node = in_node->right_chld;
    }
    return in_node;
}
// max

void RBTree::delete_node(Node *in_node, double in_val)
{
    Node *z = LEAF;//create tracking pointers
    Node *x;
    Node *y;
    while (in_node != LEAF)//move the current position, until the value to delete is reached
    {
        if (in_node->data == in_val)
        {
            z = in_node;
        }
        if (in_node->data <= in_val)
        {
            in_node = in_node->right_chld;
        }
        else
        {
            in_node = in_node->left_chld;
        }
    }
    if (z == LEAF)//if leaf node is reached, break(the value specified is not in the tree)
    {
        return;
    }
    y = z;//save previous position
    int y_original_colour = y->colour;//save y's original colour
    if (z->left_chld == LEAF)//delete node according to BST deletion algorithm + Case 1&2 of the Red_Black deletion algorithm
    {
        x = z->right_chld;
        stitch_tree(z, z->right_chld);
    }
    else if (z->right_chld == LEAF)
    {
        x = z->left_chld;
        stitch_tree(z, z->left_chld);
    }
    else
    {
        y = min_node(z->right_chld);
        y_original_colour = y->colour;
        x = y->right_chld;
        if (y->parent == z)
        {
            x->parent = y;
        }
        else
        {
            stitch_tree(y, y->right_chld);
            y->right_chld = z->right_chld;
            y->right_chld->parent = y;
        }
        stitch_tree(z, y);
        y->left_chld = z->left_chld;
        y->left_chld->parent = y;
        y->colour = z->colour;
    }
    delete z;//free the memorya allocated to deleted node
    if (y_original_colour == 0)//if we deleted a black node, call balance_delete to fix the tree
    {
        balance_delete(x);
    }
}
// delete_node
bool RBTree::delete_value(int val)
{
    if (this->is_empty())//if the tree is empty, return false
    {
        return false;
    }
    delete_node(this->root, val);//otherwise, proceed with deletion, and return true once finished
    return true;
}

QString RBTree::get_preorder_trav() const
{
    QStack<Node *> stack;
    QString traversal;
    Node *root = this->root;
    while (true)
    {
        // Go to the left extreme insert all the elements to stack, add to string as encountered
        while (root != LEAF) //while we dont get to leaf node
        {
            traversal.append(QString::number(root->data) + " "); //save data to the string
            stack.push(root);
            root = root->left_chld; //go left
        }
        // check if Stack is empty
        if (stack.isEmpty())
        {
            return traversal;
        }
        // pop the element from the stack, add the nodes at
        // the right to the Stack
        root = stack.pop(); //go up
        root = root->right_chld; //look for right child
    }
}

void RBTree::rec_node_del(const Node *node)
{
    if (node == LEAF)
    {
        return;
    }
    else
    {
        rec_node_del(node->left_chld);
        rec_node_del(node->right_chld);
        delete node;
        node = nullptr;
        return;
    }
}

// Set all nodes' x value to 0 in preperation for redrawing with a scale multiplier applied.
void RBTree::reset_pos(Node *node)
{
    if (node == LEAF)
    {
        return;
    }
    reset_pos(node->left_chld);
    node->x = 0;
    reset_pos(node->right_chld);

    return;
}

void RBTree::draw(QPainter *painter, double &scale)
{
    if (this->root == LEAF)
    {
        return;
    }
    // Set properties of the painter for drawing the tree
    this->painter = painter;
    this->painter->setFont(QFont("Times", 12 * scale, QFont::Normal));

    // Set variables for drawing the tree
    this->scale = scale;
    this->radius = 20 * scale;
    this->xspace = radius;
    this->yspace = radius * 5;

    // Before drawing, must make sure that all nodes have x = 0 since in rec_draw() we check value of x on some nodes.
    reset_pos(root);

    // first node drawn (leftmost node) needs to have a static, predefined
    // location for the rest of the tree to be based off.
    Node *leftmost = get_left_node(root);
    leftmost->x = radius * 2;

    // Draw the tree
    this->rec_draw(root);
    return;
}

// Recursively get the leftmost node

Node *RBTree::get_left_node(Node *node) const
{
    if (node->left_chld == LEAF)
    {
        return node;
    }
    return get_left_node(node->left_chld);
}

// Get the level of the node by tracing back its parents
int RBTree::get_level(Node *node)
{
    int level = 1;
    Node *current = node;

    while (current->parent != nullptr)
    {
        current = current->parent; // keep going up
        ++level;                   // inc level
    }
    return level;
}

// Calculate where the rightmost node is drawn of a left subtree
int RBTree::get_lft_tree(const Node *node)
{
    if (node->right_chld == LEAF)
    {
        return node->x;
    }
    return get_lft_tree(node->right_chld);
}

// Calculate where the ancestor of a node is so that the leftmost node of the right
// subtree can be drawn just to the right of the ancestor
int RBTree::get_anc_loc(const Node *node)
{
    // All ancestor's node->x will be 0 unless it has already been drawn -
    // find the ancestor who's x != 0
    Node *currentNode = node->parent;

    while (currentNode->x == 0)
    {
        currentNode = currentNode->parent;
    }

    return currentNode->x;
}

int RBTree::get_y() const
{
    int level = get_tree_height() + 1;
    return (level * radius * 2 + yspace * (level - 1)) + radius * 2;
}

int RBTree::get_x() const
{
    if (this->root == LEAF)
    {
        return radius * 3;
    }

    Node *current = root;
    while (current->right_chld != LEAF)
    {
        current = current->right_chld;
    }
    return current->x + radius * 3;
}

void RBTree::rec_draw(Node *node)
{
    // Base case
    if (node == LEAF)
    {
        return;
    }
    // Draw left subtree
    QBrush brush;
    QPen pen;
    brush.setStyle(Qt::SolidPattern);
    pen.setColor(Qt::yellow);
    pen.setCapStyle(Qt::MPenCapStyle);
    painter->setPen(QPen(Qt::cyan, 5));

    this->rec_draw(node->left_chld);
    if (node->colour == 0) //if the colour of the node is 0
    {
        brush.setColor(Qt::black); //set the brush to be black
    }
    else
    {
        brush.setColor(Qt::red); //set the brush to be red
    }
    painter->setBrush(brush);//set the brush for the painter
    // Set the y position of the node based off of the level of the node and the radius
    int level = get_level(node); //get the level of the node
    int y = level * radius * 2 + yspace * (level - 1); //calculate y

    // if there is a left child, we need to draw this parent relative to it
    if (node->left_chld != LEAF)
    {
        node->x = get_lft_tree(node->left_chld) + radius + xspace; //finds nodes x cordinate


        // Draw line to left child
        painter->drawLine(QPoint(node->x, y + radius), QPoint(node->left_chld->x + 2, ((level + 1) * radius * 2 + yspace * level) - radius));
    }

    // in case of a node without a left child that is not the leftmost in the tree
    // - rules out root of tree (would be leftmost)
    // - must be the right child of some ancestor (parent, grandparent, etc..)
    // - must draw relative to first ancestor where x != 0

    else if (node->x == 0)
    {
        node->x = get_anc_loc(node) + radius + xspace;
    }

    // Draw the node
    painter->drawEllipse(QPoint(node->x + 3, y - 3), 30, 30);

    // Adjust the text horizontally depending on how many digits are in it
    int adjText;
    if (std::abs(node->data) < 10)
    {
        adjText = 4;
    }
    else if (std::abs(node->data) < 100)
    {
        adjText = 11;
    }
    else if (std::abs(node->data) < 1000)
    {
        adjText = 17;
    }
    else
    {
        adjText = 16;
    }
    painter->setFont(QFont("Times", 20, QFont::Bold));

    painter->drawText(QPoint(node->x - (adjText * scale), y + (5 * scale)), QString::number(node->data));

    // Draw the right subtree
    this->rec_draw(node->right_chld);

    // Draw the line to the right child (if applicable).
    // Must be done after recursively drawing right child, otherwise x values will still be 0.
    if (node->right_chld != LEAF)
    {
        painter->drawLine(QPoint(node->x, y + radius), QPoint(node->right_chld->x - 2, ((level + 1) * radius * 2 + yspace * level) - radius));
    }
    return;
}
