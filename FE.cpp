//библиотеки
#define _CRT_SECURE_NO_DEPRECATE 
#include <iostream>
#include <string>
#include <exception>
#include <fstream>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <ctime>
//пространства имен
using namespace std;
namespace fs = filesystem;

//глобальные переменные
string COMMAND;							//переменная для введения комманд
string ANS;								//дополнительная переменная для работы с командами

string FILE_NAME;						//временное хранилище имени файла с которым будем работать
string CHECKDIR;						//переменная для проверки след. директории в которую хотим перейти
string CURRENT_DIRECTORY = "D:\\";		//глобальная главная директория
string ADD_TO_CURRDIR;					//переменная для проверки след. директории в которую хотим перейти
string CURRENT_DIRECTORY_PLUSFILE;		//для работы с файлами, чтобы не было проблем с основной директорией
string OLD;								//для переименовывания/копирования/перемещения - старая директория/имя
string NEW;								//для переименовывания/копирования/перемещения - новая директория/имя
string LOG;								//переменная для записи логов

//прототипы функций
void Show();					//Функция для вывода содержимого диска			
void ShowDir();					//Функция для подробного вывода содержимого диска			
void rename();					//rename
void go_back();					//	cd /..
void go_to_dir();				//	cd
void size_dir();				//	dir /s /d
void size_file();				//	dir /s /f
void delete_file();				//	del /f
void delete_dir_rmdir();		//  rmdir
void create_file();				//  create /f
void create_dir();				//	create /d
void copy_file();				//	copy /f
void copy_dir();				//	copy /d
void move_file();				//	move /f
void move_dir();				//	move /d
void dir_extencion();			//	dir /e
void dir_name();				//	dir /f 

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

void LogHistory(string command);
void LogOpen();
int main() {
	LogHistory(LOG = "/*---------------------------------------------------------*/\nSession started");
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
			else if (COMMAND == "log") { LogOpen(); }
			else if (COMMAND == "exit") { 
				LogHistory(LOG = "/*---------------------------------------------------------*/\nSession closed");
				system("exit"); 
			}
			else { UnknownComErr(); }
		} while (COMMAND != "exit");
	}
	catch (fs::filesystem_error const& ex) {				//catch был в комплекте с библиотекой <filesystem>
		LogHistory(LOG = "Filesystem error");
		cout << "what():  " << ex.what() << '\n'
			<< "path1(): " << ex.path1() << '\n'
			<< "path2(): " << ex.path2() << '\n'
			<< "code().message():  " << ex.code().message() << '\n'
			<< "code().category(): " << ex.code().category().name() << '\n';
	}
	LogHistory(LOG = "/*---------------------------------------------------------*/\nSession closed");
	return 0;
}


/*поиск файлов по расширению.
выводит на экран количество и названия найденных файлов
*/
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
		//LOG = "Function [dir_extencion] found " + count + ADD_TO_CURRDIR + " exctensions in directory " + CURRENT_DIRECTORY;
		LogHistory(LOG = "function [dir_extencion] found " + to_string(count) + ADD_TO_CURRDIR + " exctensions in directory " + CURRENT_DIRECTORY);
		cout << "found " << count << " files with " << ADD_TO_CURRDIR << " exctension " << endl;
	}
}

/// 
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
		LogHistory(LOG = "function [dir_name] found " + to_string(count) + " files with " + ADD_TO_CURRDIR + " name ");
		cout << "found " << count << " files with " << ADD_TO_CURRDIR << " name " << endl;
	}
}

/*функция для проверки директорий
если директория существует, возвращается true
если директория не существует, возвращается false
*/
bool CheckCurDir(string dir) {
	if (fs::exists(dir) == true) {
		LogHistory(LOG = "function [CheckCurDir] returned true(ok) ");
		return true;
	}
	else {
		LogHistory(LOG = "function [CheckCurDir] returned false (err)");
		cout << "\nDir err\n";
		system("pause");
		system("cls");
		return false;
	}
}

/*функция для итерирования по проводнику
в эта функция возвращвет нас на одну директорию назад
*/
void go_back() {
	string for_log;
	CHECKDIR = CURRENT_DIRECTORY;
	for_log = CHECKDIR;
	reverse(CHECKDIR.begin(), CHECKDIR.end());
	if (CHECKDIR.find("\\") == 0) {
		CHECKDIR.erase(CHECKDIR.find("\\"), 1);
	}
	reverse(CHECKDIR.begin(), CHECKDIR.end());
	int count = CHECKDIR.find("\\") + 1;
	int index = (CHECKDIR.size() - CHECKDIR.find("\\"));
	CHECKDIR.erase(count, index);
	if (CheckCurDir(CHECKDIR)) {
		CURRENT_DIRECTORY = CHECKDIR;
		LogHistory(LOG = "function [go_back] changed directory from " + for_log + " to " + CURRENT_DIRECTORY);
	}
}
/*функция для итерирования по проводнику
в эта функция перемещает нас на одну директорию вперед
*/
void go_to_dir() {
	string for_log;
	CHECKDIR = CURRENT_DIRECTORY;
	for_log = CHECKDIR;
	cin >> ADD_TO_CURRDIR;
	ADD_TO_CURRDIR += "\\";
	CHECKDIR += ADD_TO_CURRDIR;
	if (CheckCurDir(CHECKDIR)) {
		CURRENT_DIRECTORY = CHECKDIR;
		LogHistory(LOG = "function [go_back] changed directory from " + for_log + " to " + CURRENT_DIRECTORY);
	}
}

/*функция для узнавания размера папки*/
void size_dir() {
	int64_t fsize = 0;
	int64_t count = 0;
	if (CheckCurDir(CURRENT_DIRECTORY)) {
		for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{ CURRENT_DIRECTORY }) {
			++count;
			fs::path dir = fs::current_path() / dir_entry;
			fsize += fs::file_size(dir_entry);
		}
		cout << endl << "Size of " << count << " files: " << fsize << " bytes\n";
		LogHistory(LOG = "function [sizedir] size of " + to_string(count) + " file(s) in directory " + CURRENT_DIRECTORY + ": " + to_string(fsize) + " bytes");
		system("pause");
	}
}
/*функция для узнавания размера одного файла*/
void size_file() {
	FilePlusDir();
	int64_t fsize = 0;
	int64_t count = 0;
	if (CheckCurDir(CURRENT_DIRECTORY_PLUSFILE)) {
		fsize = fs::file_size(CURRENT_DIRECTORY_PLUSFILE);
		++count;
		cout << "size of " << count << " file: " << fsize << " bytes\n";
		LogHistory(LOG = "function [size_file] size of " + to_string(count) + " file(s) in directory " + CURRENT_DIRECTORY_PLUSFILE + ": " + to_string(fsize) + " bytes");
		system("pause");
	}
}
/*функция для удаления одного файла*/
void delete_file() {
	FilePlusDir();
	if (CheckCurDir(CURRENT_DIRECTORY_PLUSFILE)) {
		fs::remove_all(CURRENT_DIRECTORY_PLUSFILE);
		cout << endl << FILE_NAME << " deleted\n";
		LogHistory(LOG = "function [delete_file] deleted " + CURRENT_DIRECTORY_PLUSFILE);
		system("pause");
	}
}
/*функция для удаления одной папки*/
void delete_dir_rmdir() {
	cin >> CHECKDIR;   //путь удаляемой папки
	if (CheckCurDir(CHECKDIR)) {
		fs::remove_all(CHECKDIR);
		cout << endl << CHECKDIR << " deleted\n";
		LogHistory(LOG = "function [delete_dir_rmdir] deleted " + CHECKDIR);
		system("pause");
	}
}

/*функция для создания любого типа файла*/
void create_file() {
	FilePlusDir();
	cout << endl << "Created" << endl;
	LogHistory(LOG = "function [create_file] created " + FILE_NAME + "in directory " + CURRENT_DIRECTORY);
	system("pause");
}

/*функция для создания одной папки*/
void create_dir() {
	CHECKDIR = CURRENT_DIRECTORY;
	cin >> ADD_TO_CURRDIR;
	CHECKDIR += ADD_TO_CURRDIR;
	if (CheckCurDir(CHECKDIR)) {
		CURRENT_DIRECTORY = CHECKDIR;
		fs::create_directories(CURRENT_DIRECTORY);
		cout << endl << "Created" << endl;
		LogHistory(LOG = "function [create_dir] created directory " + CURRENT_DIRECTORY);
		system("pause");
	}
}
/*функция для копирования одного файла 
нужно указывать полные директории откуда и куда копировать */
void copy_file() {
	cin >> OLD >> NEW;
	if (CheckCurDir(OLD) && CheckCurDir(NEW)) {
		fs::copy(OLD, NEW);
		cout << endl << "file " << OLD << " copied successfully to " << NEW << endl;
		LogHistory(LOG = "function [copy_file] copied file from " + OLD + " to " + NEW);
		system("pause");
	}
}
/*функция для копирования одной папки
нужно указывать полные директории откуда и куда копировать */
void copy_dir() {
	cin >> OLD >> NEW;
	if (CheckCurDir(OLD) && CheckCurDir(NEW)) {
		const auto copyOptions = fs::copy_options::update_existing | fs::copy_options::recursive;
		fs::copy(OLD, NEW, copyOptions);
		cout << endl << "directory " << OLD << " copied successfully to " << NEW << endl;
		LogHistory(LOG = "function [copy_dir] copied directory from " + OLD + " to " + NEW);
		system("pause");
	}
}
/*функция для перемещения одного файла
нужно указывать полные директории откуда и куда копировать */
void move_file() {
	cin >> OLD >> NEW;
	if (CheckCurDir(OLD) && CheckCurDir(NEW)) {
		fs::copy(OLD, NEW);
		fs::remove_all(OLD);
		cout << "file " << OLD << " moved successfully to " << NEW << endl;
		LogHistory(LOG = "function [move_file] moved file from " + OLD + " to " + NEW);
		system("pause");
	}
}
/*функция для перемещения одной папки
нужно указывать полные директории откуда и куда копировать */
void move_dir() {
	cin >> OLD >> NEW;
	if (CheckCurDir(OLD) && CheckCurDir(NEW)) {
		const auto copyOptions = fs::copy_options::update_existing | fs::copy_options::recursive;
		fs::copy(OLD, NEW, copyOptions);
		fs::remove_all(OLD);
		cout << "directory " << OLD << " moved successfully to " << NEW << endl;
		LogHistory(LOG = "function [move_dir] moved directory from " + OLD + " to " + NEW);
		system("pause");
	}
}
/*функция для переименовывания одного файла/папки
нужно указывать полные директории откуда и куда копировать */
void rename() {
	if (cin >> OLD >> NEW && CheckCurDir(OLD) && CheckCurDir(NEW)) {
		fs::rename(OLD, NEW);
		cout << endl << OLD << " renamed to " << NEW << endl;
		LogHistory(LOG = "function [rename] renamed file/dir from " + OLD + " to " + NEW);
		system("pause");
	}
}

/*функция готорая выводит /общее/свободное/доступное пространство на диске C:\ и D:\ */
void Space() {
	fs::space_info devi = fs::space("d:\\");
	fs::space_info tmpi = fs::space("c:\\");
	cout << "\tCapacity\t" << "Free\t" << "\tAvailable\n";
	cout << "d:\t" << devi.capacity / 1048576 << "\t\t" << devi.free / 1048576 << "\t\t" << devi.available / 1048576 << "\t   MEGABYTES" << endl;
	cout << "c:\t" << tmpi.capacity / 1048576 << "\t\t" << tmpi.free / 1048576 << "\t\t" << tmpi.available / 1048576 << "\t   MEGABYTES" << endl;
	LogHistory(LOG = "function [Space] ");
	system("pause");
}

/*функция которая подробно выводит содержимое папки
она зайдет в каждую папку и выведет ее содержимое
*/
void ShowDir() {
	if (CheckCurDir(CURRENT_DIRECTORY)) {
		for (auto const& dir_entry : filesystem::recursive_directory_iterator{ CURRENT_DIRECTORY }) {
			cout << dir_entry.path().string() << endl;
			LogHistory(LOG = "function [ShowDir] printed [" + dir_entry.path().string() + "]");
		}
	}
}
/*функция выводит содержимое папки, в которой находимся*/
void Show() {
	if (CheckCurDir(CURRENT_DIRECTORY)) {
		for (auto& p : fs::directory_iterator(CURRENT_DIRECTORY)) {
			LogHistory(LOG = "function [Show] printed [" + p.path().string() + "]");
			cout << p.path().string() << endl;
		}
	}
}

void Dir() {
	cin >> ANS;
	if (ANS == "/e") { dir_extencion(); }
	if (ANS == "/i") { ShowDir(); }
	if (ANS == "/f") { dir_name(); }///
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
/*просто для сокращения кода*/
void FilePlusDir() {
	cin >> FILE_NAME;
	CURRENT_DIRECTORY_PLUSFILE = CURRENT_DIRECTORY;
	CURRENT_DIRECTORY_PLUSFILE += FILE_NAME;
	LogHistory(LOG = "function [FilePlusDir] ");
}

void open_manual() {
	LogHistory(LOG = "function [open_manual] ");
	system("D:\\FileExplorer\\manual.txt");
}

void UnknownComErr() {
	LogHistory(LOG = "UnknownComErr ");
	cout << "\nUnknown command\n";
	system("pause");
	system("cls");
}

void LogHistory(string command) {
	string path("D:\\FileExplorer\\log.txt");
	ofstream put(path, ios::app);
	time_t seconds = time(NULL);
	tm* timeinfo = localtime(&seconds);
	for (int i = 0; i < 1; i++) {
		put << command << "\t || time: " << asctime(timeinfo);
	}
}
void LogOpen() { system("D:\\FileExplorer\\log.txt"); }
