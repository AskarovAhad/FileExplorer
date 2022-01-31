#define _CRT_SECURE_NO_DEPRECATE 
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <exception>
#include <fstream>
#include <stdio.h>
#include <io.h>
#include <direct.h>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <iterator>
using namespace std;
namespace fs = filesystem;

string FILE_NAME;
string COMMAND;
string ANS;

string CHECKDIR;
string CURRENT_DIRECTORY = "D:\\";
string ADD_TO_CURRDIR;
string CURRENT_DIRECTORY_PLUSFILE;
string OLD;
string NEW;
string NEWNAME;


void Show();
void ShowDir();
void rename();
void go_back();					//	cd /..
void go_to_dir();				//	cd
void size_dir();				//	dir/s/d
void size_file();				//	dir/s/f
void delete_file();				//	del/f
void delete_dir_rmdir();		//  rmdir
void create_file();				//  create/f
void create_dir();				//	create/d
void copy_file();				//	copy /f
void copy_dir();				//	copy /d
void move_file();				//	move /f
void move_dir();				//	move /d
void dir_extencion();			//	dir/e
void dir_name();				//	dir/f 

void Dir();
void Del();
void Create();
void Copy();
void Move();
void Space();
bool CheckCurDir(string dir);
void open_manual();
void UnknownComErr();

void FilePlusDir();

int main() {
	setlocale(0, "");
	try {
		do {
			Show();
			cout << CURRENT_DIRECTORY << "->";
			cin >> COMMAND;
			system("cls");
			if (COMMAND == "cd") { go_to_dir(); }
			//else if (COMMAND == "disk") { ChangeDisk(); }
			else if (COMMAND == "cd..") { go_back(); }
			else if (COMMAND == "dir") { Dir(); }
			else if (COMMAND == "del") { Del(); }
			else if (COMMAND == "create") { Create(); }
			else if (COMMAND == "copy") { Copy(); }
			else if (COMMAND == "move") { Move(); }
			else if (COMMAND == "rename") { rename(); }
			else if (COMMAND == "manual") { open_manual(); }
			else if (COMMAND == "space") { Space(); }
			else if (COMMAND == "exit") { system("exit"); }
			else { UnknownComErr(); }
		} while (COMMAND != "exit");
	}
	catch (fs::filesystem_error const& ex) {
		cout << "what():  " << ex.what() << '\n'
			<< "path1(): " << ex.path1() << '\n'
			<< "path2(): " << ex.path2() << '\n'
			<< "code().message():  " << ex.code().message() << '\n'
			<< "code().category(): " << ex.code().category().name() << '\n';
	}
	return 0;
}

void dir_extencion() {
	int count = 0;
	cin >> ADD_TO_CURRDIR;
	if (CheckCurDir(CURRENT_DIRECTORY)) {
		for (auto& p : fs::directory_iterator(CURRENT_DIRECTORY)) {
			if (p.path().extension() == ADD_TO_CURRDIR) {
				cout << p.path()/*.stem()*/.string() << endl;
				++count;
			}
		}
		cout << "found " << count << " files with " << ADD_TO_CURRDIR << " exctension " << endl;
	}
}
void dir_name() {
	int count = 0;
	cin >> ADD_TO_CURRDIR;
	if (CheckCurDir(CURRENT_DIRECTORY)) {
		for (auto& p : fs::directory_iterator(CURRENT_DIRECTORY)) {
			if (p.path() == ADD_TO_CURRDIR) {
				cout << p.path().stem().string() << endl;
				++count;
			}
		}
		cout << "found " << count << " files with " << ADD_TO_CURRDIR << " exctension " << endl;
	}
}

bool CheckCurDir(string dir) {
	if (fs::exists(dir) == true) {
		return true;
	}
	else {
		cout << "\nDir err\n";
		system("pause");
		system("cls");
		return false;
	}
}

void go_back() {
	CHECKDIR = CURRENT_DIRECTORY;
	reverse(CHECKDIR.begin(), CHECKDIR.end());
	if (CHECKDIR.find("\\") == 0) {
		CHECKDIR.erase(CHECKDIR.find("\\"), 1);
	}
	reverse(CHECKDIR.begin(), CHECKDIR.end());
	CHECKDIR.erase(CHECKDIR.size() - CHECKDIR.find("\\"),
		CHECKDIR.find("\\"));
	if (CheckCurDir(CHECKDIR)) {
		CURRENT_DIRECTORY = CHECKDIR;
	}
}
void go_to_dir() {
	CHECKDIR = CURRENT_DIRECTORY;
	cin >> ADD_TO_CURRDIR;
	ADD_TO_CURRDIR += "\\";
	CHECKDIR += ADD_TO_CURRDIR;
	if (CheckCurDir(CHECKDIR)) {
		CURRENT_DIRECTORY = CHECKDIR;
	}
}


void size_dir() {
	int64_t fsize = 0;
	int64_t count = 0;
	if (CheckCurDir(CURRENT_DIRECTORY)) {
		for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ CURRENT_DIRECTORY }) {
			++count;
			fs::path dir = fs::current_path() / dir_entry;
			fsize += fs::file_size(dir_entry);
		}
		//system("cls");
		cout << endl << "Size of " << count << " files: " << fsize << " bytes\n";
	}
}
void size_file() {
	FilePlusDir();
	int64_t fsize = 0;
	int64_t count = 0;
	if (CheckCurDir(CURRENT_DIRECTORY_PLUSFILE)) {
		fsize = fs::file_size(CURRENT_DIRECTORY_PLUSFILE);
		++count;
		cout << "size of " << count << " file: " << fsize << " bytes\n";
	}
}

void delete_file() {
	FilePlusDir();
	if (CheckCurDir(CURRENT_DIRECTORY_PLUSFILE)) {
		fs::remove_all(CURRENT_DIRECTORY_PLUSFILE);
		cout << endl << FILE_NAME << " deleted\n";
	}
}
void delete_dir_rmdir() {
	cin >> CHECKDIR;   //путь удаляемой папки
	if (CheckCurDir(CHECKDIR)) {
		fs::remove_all(CHECKDIR);
		cout << endl << CHECKDIR << " deleted\n";
	}
}

void create_file() {
	FilePlusDir();
	cout << endl << "Created" << endl;
}
void create_dir() {
	CHECKDIR = CURRENT_DIRECTORY;
	cin >> ADD_TO_CURRDIR;
	CHECKDIR += ADD_TO_CURRDIR;
	if (CheckCurDir(CHECKDIR)) {
		CURRENT_DIRECTORY = CHECKDIR;
		fs::create_directories(CURRENT_DIRECTORY);
		cout << endl << "Created" << endl;
	}
}

void copy_file() {
	cin >> OLD >> NEW;
	if (CheckCurDir(OLD) && CheckCurDir(NEW)) {
		fs::copy(OLD, NEW);
		cout << endl << "file " << OLD << " copied successfully to " << NEW << endl;
	}
}
void copy_dir() {
	cin >> OLD >> NEW;
	if (CheckCurDir(OLD) && CheckCurDir(NEW)) {
		const auto copyOptions = fs::copy_options::update_existing | fs::copy_options::recursive;
		fs::copy(OLD, NEW, copyOptions);
		cout << endl << "directory " << OLD << " copied successfully to " << NEW << endl;
	}
}

void move_file() {
	cin >> OLD >> NEW;
	if (CheckCurDir(OLD) && CheckCurDir(NEW)) {
		fs::copy(OLD, NEW);
		fs::remove_all(OLD);
		cout << "file " << OLD << " moved successfully to " << NEW << endl;
	}
}
void move_dir() {
	cin >> OLD >> NEW;
	if (CheckCurDir(OLD) && CheckCurDir(NEW)) {
		const auto copyOptions = fs::copy_options::update_existing | fs::copy_options::recursive;
		fs::copy(OLD, NEW, copyOptions);
		fs::remove_all(OLD);
		cout << "directory " << OLD << " moved successfully to " << NEW << endl;
	}
}

void rename() {
	if (cin >> OLD >> NEW && CheckCurDir(OLD) && CheckCurDir(NEW)) {
		fs::rename(OLD, NEWNAME);
		cout << endl << OLD << " renamed to " << NEW << endl;
	}
}



void Space() {
	fs::space_info devi = fs::space("d:\\");
	fs::space_info tmpi = fs::space("c:\\");
	cout << "\tCapacity\t" << "Free\t" << "\tAvailable\n";
	cout << "d:\t" << devi.capacity / 1048576 << "\t\t" << devi.free / 1048576 << "\t\t" << devi.available / 1048576 << "\t   MEGABYTES" << endl;
	cout << "c:\t" << tmpi.capacity / 1048576 << "\t\t" << tmpi.free / 1048576 << "\t\t" << tmpi.available / 1048576 << "\t   MEGABYTES" << endl;
}

void ShowDir() {
	if (CheckCurDir(CURRENT_DIRECTORY)) {
		for (auto const& dir_entry : filesystem::recursive_directory_iterator{ CURRENT_DIRECTORY }) {
			cout << dir_entry.path().string() << endl;
		}
	}
}
void Show() {
	if (CheckCurDir(CURRENT_DIRECTORY)) {
		for (auto& p : fs::directory_iterator(CURRENT_DIRECTORY)) {
			cout << p.path().string() << endl;
		}
	}
}

void Dir() {
	cin >> ANS;
	if (ANS == "/e") { dir_extencion(); }
	if (ANS == "/i") { ShowDir(); }
	if (ANS == "/f") { dir_name(); }
	if (ANS == "/s") {
		cin >> ANS;
		if (ANS == "/d") { size_dir(); }
		else if (ANS == "/f") { size_file(); }
		else { UnknownComErr(); }
	}
	if (CheckCurDir(ANS)) {
		CURRENT_DIRECTORY = ANS;
	}
}
void Del() {
	cin >> ANS;
	if (ANS == "/f") { delete_file(); }
	else if (ANS == "/d") { delete_dir_rmdir(); }
	else { UnknownComErr(); }
}
void Create() {
	cin >> ANS;
	if (ANS == "/f") { create_file(); }
	else if (ANS == "/d") { create_dir(); }
	else { UnknownComErr(); }
}
void Copy() {
	cin >> ANS;
	if (ANS == "/f") { copy_file(); }
	else if (ANS == "/d") { copy_dir(); }
	else { UnknownComErr(); }
}
void Move() {
	cin >> ANS;
	if (ANS == "/f") { move_file(); }
	else if (ANS == "/d") { move_dir(); }
	else { UnknownComErr(); }
}

void FilePlusDir() {
	cin >> FILE_NAME;
	CURRENT_DIRECTORY_PLUSFILE = CURRENT_DIRECTORY;
	CURRENT_DIRECTORY_PLUSFILE += FILE_NAME;
}

void open_manual() {
	system("D:\\FileExplorer\\manual.txt");
}

void UnknownComErr() {
	cout << "\nUnknown command\n";
	system("pause");
	system("cls");
}
