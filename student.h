#pragma once

#include <cstddef>

class Student {
public:
	static const size_t NAMESIZE = 16;
	
	int id;
	char firstName[NAMESIZE], lastName[NAMESIZE];
	float gpa;
};
