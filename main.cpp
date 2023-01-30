// Jeff McMillan 1-27-2023 Linked List P2 (Student list integration) project for C++
// This project is a student list (backed by a linked list structure) that 
// provides addition, deletion, printing, averaging, and is automatically sorted based on student IDs.

#include <cstdlib>
#include <iostream>
#include <cstring>

#include "partner/list.h"
#include "student.h"

// #define CONSOLEINDBG

// Read in a line from the console.
void consolein(char* cstr, std::streamsize n) {
	std::cin.getline(cstr, n);
	if (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore();
	}
	#ifdef CONSOLEINDBG
	printf("\"%s\"\n",cstr);
	#endif
}

// Create a student using fields provided by user through the console.
Student* constructStudent() {
	Student* newstu = new Student();
	
	char conversions[32];

	printf("Student ID: ");
	consolein(conversions,32);
	newstu->id = strtol(conversions,nullptr,10);

	printf("First name: ");
	consolein(newstu->firstName, Student::NAMESIZE);
	
	printf("Last name: ");
	consolein(newstu->lastName, Student::NAMESIZE);

	printf("GPA: ");
	consolein(conversions,32);
	newstu->gpa = strtof(conversions,nullptr);

	return newstu;
}

// Print a single student!
void printStudent(const Student &stu) {
	printf("%7i %*s %*s %.2f\n", stu.id, 
	Student::NAMESIZE, stu.firstName, 
	Student::NAMESIZE,  stu.lastName, 
	stu.gpa);
}

// Slides list-adjacent pointers "A,B" along linked list until it finds gap where the new node fits. 
// Only compares against "B" ptr, assumes newnode is greater/eq to "A" ptr.
// Returns new head.
Node<Student>* insertStudent(Node<Student>* a, Node<Student>* b, Node<Student>* newnode) {
	if (!b) {
		if (a) {
			a->setNext(newnode);
			return a;
		}
		return newnode;
	}
	else if (newnode->getData()->id < b->getData()->id) {
		newnode->setNext(b);
		if (a) {
			a->setNext(newnode);
			return a;
		}
		return newnode;
	}
	else {
		insertStudent(b,b->getNext(),newnode);
		return a ? a : b;
	}
}

// Print the head and any remaining nodes.
void printAllStudents(Node<Student>* head) {
	if (!head) return;
	printStudent(*head->getData());
	printAllStudents(head->getNext());
}

// Current student stats
struct StudentSum {
	size_t n; // number of students
	float gpa; // gpa sum
};

// Sum all of student traits.
void sumStudents(StudentSum& sum, Node<Student>* head) {
	if (!head) return;
	Student& stu = *head->getData();
	sum.n++;
	sum.gpa += stu.gpa;
	sumStudents(sum, head->getNext());
}

// Slides list-adjacent pointers "A,B" along linked list until it finds gap where the new node fits. 
// Assume "A" has been checked to not equal the id, "A" and "B" are adjacent pointers.
// Returns new head.
Node<Student>* deleteStudentID(int id, Node<Student>* a, Node<Student>* b) {
	Node<Student>* c = b ? b->getNext() : nullptr;
	if (!b) {
		return a;
	}
	else if (b->getData()->id == id) {
		delete b;
		if (a) {
			a->setNext(c);
			deleteStudentID(id,a,c);
			return a;
		}
		else {
			return deleteStudentID(id,nullptr,c);
		}
	}
	else {
		deleteStudentID(id,b,c);
		return a ? a : b;
	}
}

// Entry point
int main() {

	bool running = true;
	char cmd[16];
	Node<Student>* head = nullptr;

	const char* helpstr = "Command list: ADD PRINT DELETE QUIT AVERAGE HELP";

	printf("%s\n", helpstr);
	// Command loop!
	while (running) {
		printf(":");
		consolein(cmd, 16);
		
		if (strcmp(cmd,"ADD") == 0) {
			Student* newstu = constructStudent();
			
			printf("Created Student:\n");
			printStudent(*newstu);
			
			head = insertStudent(nullptr, head, new Node(newstu));
		}
		else if (strcmp(cmd,"DELETE") == 0) {
			char conversions[16];
			printf("ID TO DELETE: ");
			consolein(conversions,16);
			int id = strtol(conversions,nullptr,10);
			head = deleteStudentID(id,nullptr,head);
		}
		else if (strcmp(cmd,"PRINT") == 0) {
			printAllStudents(head);
		}
		else if (strcmp(cmd,"QUIT") == 0) {
			running = false;
		}
		else if (strcmp(cmd,"AVERAGE") == 0) {
			StudentSum sum;
			sum.n = 0;
			sum.gpa = 0.0f;
			sumStudents(sum, head);
			printf("Total Students: %u\nTotal GPA: %.2f\n", sum.n, sum.gpa);
			float avg = sum.gpa / sum.n;
			if (sum.n > 0) printf("\nAverage GPA: %.2f\n", avg);
		}
		else if (strcmp(cmd,"HELP") == 0) {
			printf("%s\n",helpstr);
		}

		printf("\n");
	}
	
	printf("Goodbye World!\n");
	return 0;
}
