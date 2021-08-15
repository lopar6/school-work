//============================================================================
// Name        : coursePlanner.cpp
// Author      : Logan Parker
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Simple console based application for a mock course planner
//============================================================================

#include <iostream>
#include <algorithm>
#include <string> 
#include <fstream>
#include <vector>
#include <climits>

struct Course {
    std::string courseId;
    std::string title;
    std::vector<std::string> prerequisites;
    Course(std::string _id, std::string _title, std::vector<std::string> _prerequisites){
        courseId = _id;
        title = _title;
        prerequisites = _prerequisites;
    }
    Course() {}
};

class HashTable {

    private:
        const unsigned int DEFAULT_SIZE = 20;

        struct Node {
            Course course;
            unsigned int key;
            Node *next;

            // default constructor
            Node() {
                key = UINT_MAX;
                next = nullptr;
            }

            // initialize with a course
            Node(Course _course) {
                key = UINT_MAX;
                next = nullptr;
                course = _course;
            }

            // initialize with a course and a key
            Node(Course _course, unsigned int _key) {
                course = _course;
                key = _key;
            }
        };

        std::vector<Node> nodes;
        std::vector<Course*> alphaOrderedCourses;
        unsigned int tableSize = DEFAULT_SIZE;
        unsigned int hash(std::string key);
        void BubbleSort(std::vector<Course*> courses, int n);

    public:
        HashTable();
        HashTable(unsigned int size);
        virtual ~HashTable();
        void Insert(Course course);
        void InsertToAlpha(Course* course);
        void SortAlphaOrdered();
        void PrintAllCourses();
        void PrintCourse(std::string courseId);
};

HashTable::HashTable() {
    // Initalize node structure by resizing to tableSize
    nodes.resize(tableSize);
}

HashTable::HashTable(unsigned int size) {
    tableSize = size;
    nodes.resize(tableSize);
}


HashTable::~HashTable() {
    delete[] &nodes;
}

// have value given courseId string
unsigned int HashTable::hash(std::string key) {
    // take last 3 digits, cast to int, add ascii value of first letter, mod
    unsigned int hashValue;
    int k = (int)key[0];
    key = key.substr(4, 7);
    hashValue = (k + std::stoi(key) / 7) % tableSize;
    return hashValue;
}

void HashTable::Insert(Course course) {
    unsigned int key = hash(course.courseId);
    Node* node = new Node(course, key);
    // if no entry found for the key
    if(nodes[key].key == UINT_MAX) {
        // assign this node to the key position
        nodes[key] = *node;
    }
    // else find the next open node
    else {
        Node* current = &nodes[key];
        while(current->next != nullptr){
            current = current->next;
        }
        current->next = node;
    }
}

// insert course to alpha list (does not sort)
void HashTable::InsertToAlpha(Course* course){
    alphaOrderedCourses.push_back(course);
}


// bubble sort 
void HashTable::SortAlphaOrdered() {
    int i, j;
    Course* temp;
    for (i = 0; i < alphaOrderedCourses.size(); i++) {
        for (j = 0; j < alphaOrderedCourses.size()-i-1; j++) {
            if (alphaOrderedCourses[j]->title.compare(alphaOrderedCourses[j+1]->title) > 0) {
                std::cout << alphaOrderedCourses[j]->title << std::endl;
                temp = alphaOrderedCourses[j];
                alphaOrderedCourses[j] = alphaOrderedCourses[j+1];
                alphaOrderedCourses[j+1] = temp;
            }
        }
    }
}

// prints all courses in alphaOrdered course list
void HashTable::PrintAllCourses() {
    for(int i = 0; i < alphaOrderedCourses.size(); i++) {
        std::cout << alphaOrderedCourses[i]->title << std::endl;
    }
}

// Prints course information for the course with given ID
void HashTable::PrintCourse(std::string _id) {
    try {
        int key = hash(_id);
        std::cout << std::endl << nodes[key].course.courseId << std::endl << nodes[key].course.title << std::endl;
        if (nodes[key].course.prerequisites.size() > 0) {
            std::cout << " prerequisites: " << std::endl;
            for(int i = 0; i < nodes[key].course.prerequisites.size(); i++) {
                std::cout << "    " << nodes[key].course.prerequisites[i] << std::endl;
            }
        }
        std::cout << std::endl;
    }
    catch(...){
        std::cout << std::endl << _id << " not found" << std::endl << std::endl;
    }
}

// Creates course instance from CSV line and adds to Hashtable
void addCourse(std::string courseInformation, HashTable* hashTable) {
    std::string courseId;
    std::string title;
    std::vector<std::string> prerequisites;
    std::string value;
    int position;
    int i = 0;


    while(courseInformation.size() > 0) {
        position = courseInformation.find(",");
        if (position == std::string::npos || position == -1) {
            position = courseInformation.size() - 1;
        }
        value = courseInformation.substr(0, position);
        if(i == 0){
            courseId = value;
        }
        else if (i == 1) {
            title = value;
        }
        else {
            prerequisites.push_back(value);
        }
        courseInformation.erase(0, position + 1);
        i++;
    }

    Course* course = new Course(courseId, title, prerequisites);
    hashTable->Insert(*course);
    hashTable->InsertToAlpha(course);
    return;
}

// Loads course data from user specified file
void loadDataFile(HashTable* hashTable) {
    std::string fileName;
    std::string line;
    std::fstream file;
    std::cout << "File name: ";
    std::getline(std::cin, fileName);
    try {
        file.open(fileName);
    }
    catch (...) {
        std::cout << "File " << fileName << " was not found." << std::endl;
        return;
    }
    if(file.is_open()) {
        while(std::getline(file, line)) {
            // std::cout << line << std::endl;
            addCourse(line, hashTable);
        }
        file.close();
    }
    else{
        std::cout << "Unable to open file \"" << fileName << "\"" << std::endl;
    }
}

std::string getCourseId() {
    std::string line;
    std::cout << "Enter course ID: ";
    std::getline(std::cin, line);
    return line;
}

// Display the main menu
void displayMenu() {
    std::cout << std::endl 
        << " 1. Load Data Structure." << std::endl
        << " 2. Print Course List." << std::endl
        << " 3. Print Course." << std::endl
        << " 4. Exit" << std::endl;
}

// Get the user selected input for the menu
int getSelectionInput() { 
    std::string input;
    int intInput;
    std::getline(std::cin, input);
    try {
        intInput = std::stoi(input);
    }
    catch (...) {
        std::cout << input << "  is not a valid option." << std::endl;
        return -1;
    }
    return intInput;
}

// Execute the user selected action
void executeSelection(int selection, HashTable* hashTable) {
    if (selection == -1 || selection == 9 || selection == 4) {
    }
    else if  (selection == 1) {
        loadDataFile(hashTable);
        hashTable->SortAlphaOrdered();
    }
    else if  (selection == 2) {
        hashTable->PrintAllCourses();
    }
    else if  (selection == 3) {
        std::string courseId = getCourseId();
        hashTable->PrintCourse(courseId);
    }
    else {
        std::cout << selection << " is not a valid option." << std::endl;
    }
}

int main () {
    int input = 0;
    HashTable* hashTable = new HashTable();
    while(input != 9 && input != 4) {
        displayMenu();
        input = getSelectionInput();
        executeSelection(input, hashTable);
    }
    std::cout << "Thank you for using the course planner!" << std::endl;
    return 0;
}
