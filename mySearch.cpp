#include "mySearch.h"
#include <iostream>
#include <string>


using namespace std;

SSTable::SSTable(): length(0), origin(nullptr) {
}

// 使用const int* 表示数据来源不会被函数修改
SSTable::SSTable(int length,  int* source): length(length) {
    origin = new int[length];
    for (int i = 0; i < length; ++i) {
        origin[i] = source[i]; // 深拷贝数据
    }
}

SSTable::~SSTable() {
    delete[] origin; // 释放origin指向的数组
}

int SSTable::getLength()  {
    return length;
}

int* SSTable::getOrigin()  {
    return origin;
}

void SSTable::setLength(int length) {
    this->length = length;
}

// 用于设置新的数据源，同样用深拷贝来保证数据的独立性和正确性
void SSTable::setOrigin( int* source) {
    delete[] origin;        // 删除旧数据
    origin = new int[length];
    for (int i = 0; i < length; ++i) {
        origin[i] = source[i];
    }
}

int SSTable::binSearch(int val) {
    int left = 0, right = length - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (origin[mid] == val) {
            return mid; // 找到元素，返回下标
        } else if (origin[mid] < val) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1; // 未找到元素，返回-1
}

BSTreeNode::BSTreeNode() : data(0), lchild(nullptr), rchild(nullptr) {
    // Default constructor
}

BSTreeNode::BSTreeNode(int value) : data(value), lchild(nullptr), rchild(nullptr) {
    // Constructor with data only
}

BSTreeNode::BSTreeNode(int value, BSTreeNode* left, BSTreeNode* right) :
    data(value), lchild(left), rchild(right) {
    // Constructor with data and child nodes
}

BSTreeNode::~BSTreeNode() {
    // Destructor is intentionally empty as the tree should manage node memory
    // If the tree class manages memory, typically you wouldn't delete children in the node destructor
    // to prevent double deletes
}

int BSTreeNode::getData()  {
    return data;
}

BSTreeNode* BSTreeNode::getLChild()  {
    return lchild;
}

BSTreeNode* BSTreeNode::getRChild()  {
    return rchild;
}

void BSTreeNode::setData(int value) {
    data = value;
}

void BSTreeNode::setLChild(BSTreeNode* left) {
    lchild = left;
}

void BSTreeNode::setRChild(BSTreeNode* right) {
    rchild = right;
}


BSTree::BSTree() : num(0), root(nullptr) {}

BSTree::BSTree(int num, int* data) : num(num), root(nullptr) {
    // Initialize the binary search tree with the given data
    for (int i = 0; i < num; i++) {
        addNode(data[i]);
    }
}

BSTree::~BSTree() {
    // Implement the destructor here
}

int BSTree::getNum() {
    return num;
}

BSTreeNode* BSTree::getRoot() {
    return root;
}

void BSTree::setNum(int num) {
    this->num = num;
}

void BSTree::setRoot(BSTreeNode* root) {
    this->root = root;
}

void printTemp(BSTreeNode* root, string& result) {
    if (root == nullptr) {
        return;
    }
    // Pre-order traversal to print the tree
    result += to_string(root->getData()) + " ";
    printTemp(root->getLChild(), result);
    printTemp(root->getRChild(), result);
}

string BSTree::printTree() {
    string result;
    printTemp(root, result);
    return result;
}

bool BSTree::searchNode(int val) {
    BSTreeNode* current = root;
    while (current != nullptr) {
        if (val == current->getData()) {
            return true;
        }
        else if (val < current->getData()) {
            current = current->getLChild();
        }
        else {
            current = current->getRChild();
        }
    }
    return false;
}

bool BSTree::addNode(int val) {
    BSTreeNode* newNode = new BSTreeNode(val);
    if (root == nullptr) {
        root = newNode;
        num++;
        return true;
    }
    BSTreeNode* current = root;
    BSTreeNode* parent = nullptr;
    while (current != nullptr) {
        parent = current;
        if (val < current->getData()) {
            current = current->getLChild();
        }
        else if (val > current->getData()) {
            current = current->getRChild();
        }
        else {
            // Node with the same value already exists
            delete newNode;
            return false;
        }
    }
    if (val < parent->getData()) {
        parent->setLChild(newNode);
    }
    else {
        parent->setRChild(newNode);
    }
    num++;
    return true;
}

bool BSTree::deleteNode(int val) {
    BSTreeNode* current = root;
    BSTreeNode* parent = nullptr;
    bool isLeftChild = true;
    while (current != nullptr && current->getData() != val) {
        parent = current;
        if (val < current->getData()) {
            current = current->getLChild();
            isLeftChild = true;
        }
        else {
            current = current->getRChild();
            isLeftChild = false;
        }
    }
    if (current == nullptr) {
        return false;
    }
    // Case 1: Node to be deleted has no children
    if (current->getLChild() == nullptr && current->getRChild() == nullptr) {
        if (current == root) {
            root = nullptr;
        }
        else if (isLeftChild) {
            parent->setLChild(nullptr);
        }
        else {
            parent->setRChild(nullptr);
        }
    }
    // Case 2: Node to be deleted has one child
    else if (current->getRChild() == nullptr) {
        if (current == root) {
            root = current->getLChild();
        }
        else if (isLeftChild) {
            parent->setLChild(current->getLChild());
        }
        else {
            parent->setRChild(current->getLChild());
        }
    }
    else if (current->getLChild() == nullptr) {
        if (current == root) {
            root = current->getRChild();
        }
        else if (isLeftChild) {
            parent->setLChild(current->getRChild());
        }
        else {
            parent->setRChild(current->getRChild());
        }
    }
    // Case 3: Node to be deleted has two children
    else {
        BSTreeNode* successor = current->getRChild();
        BSTreeNode* successorParent = current;
        while (successor->getLChild() != nullptr) {
            successorParent = successor;
            successor = successor->getLChild();
        }
        if (current == root) {
            root = successor;
        }
        else if (isLeftChild) {
            parent->setLChild(successor);
        }
        else {
            parent->setRChild(successor);
        }
        // Connect successor's right child to successorParent's left child
        successorParent->setLChild(successor->getRChild());
        successor->setLChild(current->getLChild());
        successor->setRChild(current->getRChild());
    }
    num--;
    delete current;
    return true;
}