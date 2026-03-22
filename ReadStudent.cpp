#include <iostream>
#include <cstdio>

using namespace std;

struct Student {
	int id;
	char name[50];
	int age;
};

int main() {
	// open file for reading in binary mode
	FILE *fp = fopen("students.db", "rb");
	if (!fp) {
		cout << "Error opening file. Run createStudent first." << endl;
		return 1;
	}

	// jump directly to record #3 (index 2, since we start counting from 0)
	// each record is sizeof(Student) bytes, record 3 starts at offset 2 * sizeof(Student)
	fseek(fp, 2 * sizeof(Student), SEEK_SET);

	// read only that one record
	Student s;
	fread(&s, sizeof(Student), 1, fp);

	cout << "Record #3:" << endl;
	cout << "ID:   " << s.id << endl;
	cout << "Name: " << s.name << endl;
	cout << "Age:  " << s.age << endl;

	fclose(fp);
	return 0;
}
