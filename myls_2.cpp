#include <iostream>
#include <string>
#include <iomanip>
//#include <stdlib.h> // For free()
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <cstring>
#include <ctime>

using namespace std;

// function to sort an array of string of some size.
void sort(string n[], int size) {
	for (int i = 0; i < size - 1; i++) {
		for (int j = 0; j < size - i - 1; j++) {
			if (n[j] > n[j+1]) {
				string temp = n[j];
				n[j] = n[j+1];
				n[j+1] = temp;
			}
		}
	}
}

// functino to get permission from mode
string formatPermissions(mode_t mode) {
	string perms(10, '-');

	// owner permissions
	if (mode & S_IRUSR) perms[1] = 'r';
	if (mode & S_IWUSR) perms[2] = 'w';
	if (mode & S_IXUSR) perms[3] = 'x';
	// group permissions
	if (mode & S_IRGRP) perms[4] = 'r';
	if (mode & S_IWGRP) perms[5] = 'w';
	if (mode & S_IXGRP) perms[6] = 'x';
	// other permissions
	if (mode & S_IROTH) perms[7] = 'r';
	if (mode & S_IWOTH) perms[8] = 'w';
	if (mode & S_IXOTH) perms[9] = 'x';

	// file type character
	if (S_ISDIR(mode))  perms[0] = 'd';
	if (S_ISLNK(mode))  perms[0] = 'l';

	return perms;
}

// finction to return "[DIR]     ", ["SYMLNK]", "[FILE]    ", "[OTHER]  "
// based on the mode
string fileType(mode_t mode) {
	if (S_ISDIR(mode))  return "[DIR]   ";
	if (S_ISLNK(mode))  return "[SYMLNK]";
	if (S_ISREG(mode))  return "[FILE]  ";
	return "[OTHER] ";
}

// function to format time as [Month name] [day] [hour]:[min]
string fomratTime(time_t t) {
	char buf[80];
	struct tm *tm_info = localtime(&t);
	strftime(buf, sizeof(buf), "%b %d %H:%M", tm_info);
	return string(buf);
}

int main() {
	char cwd[256];
	string names[256];
	string user_cmd;
	while (1) {
		getcwd(cwd, sizeof(cwd));
            	cout << endl << cwd << "> ";

	        getline(cin, user_cmd);
            	if (user_cmd.substr(0, 2)  == "ls") {
                	DIR * dir = opendir(cwd);
                	if (!dir) {
                        	cout << "Error openning directory " << cwd << endl;
                        	return 1;
                	}
                	struct dirent * dir_entry;
                	int p = 0, i=0;
                	while ((dir_entry = readdir(dir)) != 0)
                        	names[i++] = dir_entry->d_name;

			sort(names, i);

			if (user_cmd == "ls -l" || user_cmd == "ls -al") {
				// long listing format
				for (int j=0; j<i; j++) {
					if (user_cmd == "ls -l" && names[j][0] == '.') continue;
					struct stat st;
					string path = string(cwd) + "/" + names[j];
					lstat(path.c_str(), &st);

					struct passwd *pw = getpwuid(st.st_uid);

					cout << fileType(st.st_mode) << "\t"
					     << formatPermissions(st.st_mode) << "\t"
					     << st.st_nlink << "\t"
					     << st.st_size << "\t"
					     << pw->pw_name << "\t"
					     << fomratTime(st.st_mtime) << "\t"
					     << st.st_ino << "\t"
					     << names[j] << endl;
				}
			} else {
				// basic ls format
				for (int j=0; j<i; j++) {
					if (names[j][0] == '.') continue;
					p += names[j].length();
					if (p > 80) { cout << endl; p =0; }
					cout << names[j] << "  ";
				}
			}
	     	}
		if (user_cmd == "exit") break;
    	}
	return 0;
}
