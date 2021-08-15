//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Logan Parker
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <time.h>
#include <string>
#include <algorithm>
#include <stdlib.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        bidId = "0";
        title = "No Title";
        fund = "No Fund";
        amount = 0.0;
    }
};

// Internal structure for tree node
struct Node {
    Bid bid;
    Node* left = nullptr;
    Node* right = nullptr;
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void AddNode(Node* node);
    void InOrder(Node* node);
    Node* RemoveNode(Node* node, string bidId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void PostOrder();
    void PostOrder(Node*);
    void PreOrder();
    void PreOrder(Node*);
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // FixMe (1): initialize housekeeping variables
    //root is equal to nullptr
    root = nullptr;
}

/**
 * helper to deconstructor
 */
void deleteNodes(Node* node){
    if (node->right != nullptr) {
        deleteNodes(node->right);
    }
    if (node->left != nullptr) {
        deleteNodes(node->left);
    }
    delete node;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    deleteNodes(root);
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // FixMe (2): In order root
    // call inOrder fuction and pass root
    InOrder(root);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
    // FixMe (3): Post order root
    // postOrder root
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
    // FixMe (4): Pre order root
    // preOrder root
}

/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
    // FIXME (5) Implement inserting a bid into the tree
    Node* node = new Node;
    node->bid = bid;

    // if root equal to null ptr
    if (root == nullptr) {
      // root is equal to new node bid
        root = node;
    }
    else {
        // add Node root and bid
        AddNode(node);
    }
}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {
    // FIXME (6) Implement removing a bid from the tree
    // remove node root bidID
    Node* current = root;
    Node* prev = nullptr;
    while (current != nullptr) {
        if (current->bid.bidId == bidId) {
            if (current->left == nullptr && current->right == nullptr){
                if (prev == nullptr){
                    root = nullptr;
                }
                else if (prev->left == current) {
                    prev->left = nullptr;
                }
                else{
                    prev->right = nullptr;
                }
            }
            else if (current->right == nullptr) {
                if (prev == nullptr) {
                    root = current->left;
                }
                else if (prev->left == current) {
                    prev->left  = current->left;
                }
                else {
                    prev->right = current->left;
                }
            }
            else if (current->left == nullptr) {
                if (prev == nullptr) {
                    root = current->right;
                }
                else if (prev->left == current) {
                    prev->left = current->right;
                }
                else {
                    prev->right = current->right;
                }
            }
            else {
                Node* preSuccessor = nullptr;
                Node* successor = current->right;
                while (successor->left != nullptr) {
                    preSuccessor = successor;
                    successor = successor->left;
                }
                if (preSuccessor != nullptr) {
                    preSuccessor->left = nullptr;
                    preSuccessor->right = nullptr;
                }
                successor->left = current->left;
                successor->right = current->right;
                if (prev->left == current) {
                    prev->left = successor;
                }
                else {
                    prev->right = successor;
                }
            }
            return;
        }
        else if (current->bid.bidId < bidId) {
            prev = current;
            current = current->right;
        }
        else {
            prev = current;
            current = current->left;
        }
    }
    
}

/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {
    // FIXME (7) Implement searching the tree for a bid
    // set current node equal to root
    Node* current = root;
    // keep looping downwards until bottom reached or matching bidId found
    while (current != nullptr) {
        // if match found, return current bid
        if (current->bid.bidId == bidId){
            return current->bid;
        }
        // if bid is smaller than current node then traverse left
        else if (bidId < current->bid.bidId) {
            current == current->left;
        }
        // else larger so traverse right
        else {
            current == current->right;
        }
    }
    Bid bid;
    return bid;
}

/**
 * Add a bid to some node (iterative)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::AddNode(Node* node) {
    // FIXME (8) Implement inserting a bid into the tree
    Node* current = root;

    while (true) {
        // if node is larger then add to right
        if (node->bid.bidId > current->bid.bidId) {
            // if no right node
            if (current->right != nullptr) {
                // this node becomes right
                current = current->right;
            }
            else {
                current->right = node;
                break;
            }
        } 
        // else down the left node
        else {
            // if there is a left node
            if (current->left != nullptr) {
                // this node becomes left
                current = current->left;
            }
            else {
                current->left = node;
                break;
            }
        }
    }
    node->left = nullptr;
    node->right = nullptr;
}

void BinarySearchTree::InOrder(Node* node) {
      // FixMe (9): Pre order root
      //if node is not equal to null ptr
      if (node != nullptr) {
      //InOrder not left
      InOrder(node->left);
      //output bidID, title, amount, fund
      cout << node->bid.bidId << " " << node->bid.title << " " << node->bid.amount << " " << node->bid.fund << endl;
      //InOder right
      InOrder(node->right);
      }
}
void BinarySearchTree::PostOrder(Node* node) {
      // FixMe (10): Pre order root
      //if node is not equal to null ptr
      //postOrder left
      //postOrder right
      //output bidID, title, amount, fund

}

void BinarySearchTree::PreOrder(Node* node) {
      // FixMe (11): Pre order root
      //if node is not equal to null ptr
      //output bidID, title, amount, fund
      //postOrder left
      //postOrder right      
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bst->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98109";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            bst = new BinarySearchTree();

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bst);

            //cout << bst->Size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bst->InOrder();
            break;

        case 3:
            ticks = clock();

            bid = bst->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
            	cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 4:
            bst->Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

	return 0;
}
