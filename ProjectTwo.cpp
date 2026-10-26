/*
 * ProjectTwo.cpp
 *
 *  Created on: Oct 19, 2026
 *      Author: Rina Bickel
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Store all the information for each of the courses
struct Course {
	string courseNumber;
	string courseName;
	vector<string> prerequisites;
};

// A node will hold one course in the binary search tree
struct Node {
	Course course;
	Node* left;
	Node* right;

	Node(Course newCourse) {
		course = newCourse;
		left = nullptr;
		right = nullptr;
	}
};

// The binary search tree stores the courses
class BinarySearchTree {
private:
	Node* root;

//Remove the nodes when they are not needed
void clearNodes(Node* node) {
	if (node == nullptr) {
		return;
	}

	clearNodes(node->left);
	clearNodes(node->right);

	delete node;
}

public:
	BinarySearchTree() {
		root = nullptr;
	}

// This cleans up the tree when the program is closed
~BinarySearchTree() {
	clearNodes(root);
}

// This looks to see if the courses have been loaded
	bool IsEmpty() const {
		return root == nullptr;
	}

// This will add a course to the binary search tree
void Insert(Course newCourse) {
	if (root == nullptr) {
		root = new Node(newCourse);
		return;
	}

	Node* current = root;

	while (current != nullptr) {
		if (newCourse.courseNumber == current->course.courseNumber) {
			current->course = newCourse;
			return;
		}

		if (newCourse.courseNumber < current->course.courseNumber) {
			if (current->left == nullptr) {
				current->left = new Node(newCourse);
				return;
			}
			current = current->left;
		}
		else {
			if (current->right == nullptr) {
				current->right = new Node(newCourse);
				return;
			}
			current = current->right;
		}
	}
}

// Search for a course number in the tree
Course Search(string courseNumber) {
	Node* current = root;

	while (current != nullptr) {
		if (courseNumber == current->course.courseNumber) {
			return current->course;

		}

		if (courseNumber < current->course.courseNumber) {
			current = current->left;
		}
		else {
			current = current->right;

		}
	}

	return Course();
	}

// This prints the course in alphanumeric order
void PrintInOrder(Node* node) {
	if (node == nullptr) {
		return;
	}

	PrintInOrder(node->left);

	cout << node->course.courseNumber << ", "
		 << node->course.courseName << endl;

	PrintInOrder(node->right);

}

// This shows the list of courses
void PrintCourseList() {
	if (root == nullptr) {
		cout << "No courses loaded." << endl;
		return;
	}

		cout << "Here is a sample schedule:" << endl;
		PrintInOrder(root);
}


//Checks that all the prerequisite courses exit
void CheckPrerequisites(Node* node) {
	if (node == nullptr) {
		return;
	}

	for (string prerequisite : node->course.prerequisites) {
		Course found = Search(prerequisite);

		if (found.courseNumber.empty()) {
			cout << "Error: Prerequisite " << prerequisite
				 << " does not exist for "
				 << node->course.courseNumber << endl;
		}
	}

	CheckPrerequisites(node->left);
	CheckPrerequisites(node->right);
}

void ValidatePrerequisites() {
	CheckPrerequisites(root);
}

};

// Loads the courses from the CSV file
bool loadCourses(string filename, BinarySearchTree& courses) {
	ifstream file(filename);

	if (!file.is_open()) {
		cout << "Error opening file." << endl;
		return false;

	}

	string line;
	int count = 0;

	while (getline(file, line)) {
		if (line.empty()) {
			continue;

		}

		stringstream ss(line);
		Course course;

		getline(ss, course.courseNumber, ',');
		getline(ss, course.courseName, ',');

		if (course.courseNumber.empty() || course.courseName.empty()) {
			cout << "Error: Invalid course data." << endl;
			continue;
		}

		string prerequisite;

		while (getline(ss, prerequisite, ',')) {
			if (!prerequisite.empty()) {
				course.prerequisites.push_back(prerequisite);

			}
		}

		courses.Insert(course);
		count++;

	}

	file.close();
	courses.ValidatePrerequisites();

	cout << count << " courses loaded." << endl;
	return count > 0;

}

// This shows the course and its prerequisites
void printCourseInformation(BinarySearchTree& courses, string courseNumber) {
	Course course = courses.Search(courseNumber);

	if (course.courseNumber.empty()) {
		cout << "Course not found." << endl;
		return;
	}

	cout << course.courseNumber << ", " << course.courseName << endl;

	if (course.prerequisites.empty()) {
		cout << "Prerequisites: None" << endl;
		return;
	}

	cout << "Prerequisites:" << endl;

	for (string prerequisiteNumber : course.prerequisites) {
		Course prerequisite = courses.Search(prerequisiteNumber);

		if (!prerequisite.courseNumber.empty()) {
			cout << prerequisite.courseNumber << ", "
				 << prerequisite.courseName << endl;
		}
		else {
			cout << "Prerequisite not found: "
				 << prerequisiteNumber << endl;
		}
	}
}

int main() {
	BinarySearchTree courses;
	string choice;
	string filename;

	cout << "Welcome to the course planner!" << endl;

	while (choice != "9") {
		cout << endl;
		cout << "1. Load Course Data" << endl;
		cout << "2. Print Course List" << endl;
		cout << "3. Print Course Information" << endl;
		cout << "9. Exit" << endl;

		cout << "What do you want to do? ";
		getline(cin, choice);

		if (choice == "1") {
			cout << "Enter the file name: ";
			getline(cin, filename);
			loadCourses(filename, courses);
		}
		else if (choice == "2") {
			courses.PrintCourseList();
		}
		else if (choice == "3") {
			if (courses.IsEmpty()) {
				cout << "No courses loaded." << endl;

			}
			else {
				string courseNumber;

				cout << "What course would you like information on? ";
				getline(cin, courseNumber);

				printCourseInformation(courses, courseNumber);
			}
		}
			else if (choice == "9") {
			cout << "Goodbye!" << endl;
		}
		else {
			cout << "Invalid menu option. Try again." << endl;
		}
	}

	return 0;
}
