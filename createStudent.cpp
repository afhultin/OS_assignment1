#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

struct Student {
	int id;
	char name[50];
	int age;
};

int main() {
	// open file for writing in binary mode
	FILE *fp = fopen("students.db", "wb");
	if (!fp) {
		cout << "Error opening file for writing" << endl;
		return 1;
	}

	// create 5 student records
	Student students[5];

	students[0].id = 1;
	strcpy(students[0].name, "Alice");
	students[0].age = 20;

	students[1].id = 2;
	strcpy(students[1].name, "Bob");
	students[1].age = 22;

	students[2].id = 3;
	strcpy(students[2].name, "Charlie");
	students[2].age = 21;

	students[3].id = 4;
	strcpy(students[3].name, "Diana");
	students[3].age = 23;

	students[4].id = 5;
	strcpy(students[4].name, "Eve");
	students[4].age = 19;

	// write all 5 records to the file
	fwrite(students, sizeof(Student), 5, fp);

	fclose(fp);
	cout << "Wrote 5 student records to students.db" << endl;

	return 0;
}
