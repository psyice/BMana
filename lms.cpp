/**********************************************************************
* 图书管理系统
* *********************************************************************
* 删除图书:
*           删除图书时, 将作为索引的三个字典对应的书籍删除, 并将json
*           数组中对应的书籍信息删除并设为NULL
*           在退出时, 统一保存更改, 写入磁盘文件
* 新增图书:
*
* 修改图书:
*						Todo
* 搜索图书:
*						从字典中读取书籍在json 数组中的下标, 并访问相应书籍
**********************************************************************/

#include "lms.h"


void
lms::Database::
init() {
	FILE* fp = fopen("db.json", "r");
	if (fp == nullptr) {
		fp = fopen("db.json", "w");
		char fuck[] = "{\"books\":[{\"id\":\"01\",\"title\":\"HelloWorld!\",\"author\":\"Psyice\",\"isbn\":\"123456789\",\"language\":\"English\",\"press\":\"ShanghaiJixiang\",\"inventory\":1,\"totalInventory\":2, \"borrowerID\" : [\"01\"]},{\"id\":\"02\",\"title\":\"WorldHello!\",\"author\":\"Sice\",\"isbn\":\"987654321\",\"language\":\"Chinese\",\"press\":\"ShanghaiJixiang\",\"inventory\":2,\"totalInventory\":2, \"borrowerID\" : []}],\"readers\":[{\"id\":\"01\",\"name\":\"Sice\", \"borrowedID\" : [\"01\"]}]}" ;
		fwrite(fuck, sizeof(char), sizeof(fuck), fp);
		std::cout << "初始化数据库成功\n";
		fclose(fp);
		fp = fopen("db.json", "r");
	}
	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	dbDOM.ParseStream(is);
	fclose(fp);
	books = dbDOM["books"];
	users = dbDOM["readers"];
	for (SizeType i = 0; i < books.Size(); ++i) {
		bookIDToSub[books[i]["id"].GetString()] = i;
		bookTitleToSub[books[i]["title"].GetString()] = i;
		bookISBNToSub[books[i]["isbn"].GetString()] = i;
	}
	for (SizeType i = 0; i < users.Size(); ++i) {
		userIDToSub[users[i]["id"].GetString()] = i;
		userNameToSub[users[i]["name"].GetString()] = i;
	}
}

void
lms::Database::
saveChanges() {
	for (rapidjson::Value::ValueIterator itr = books.Begin(); itr != books.End(); ) {
		if (*itr == NULL) {
			itr =	books.Erase(itr);
		} else {
			++itr;
		}
	}
	for (rapidjson::Value::ValueIterator itr = users.Begin(); itr != users.End(); ) {
		if (*itr == NULL) {
			itr = users.Erase(itr);
		} else {
			++itr;
		}
	}
	FILE* fp = fopen("db.json", "w");
	char writeBuffer[65536];
	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
	dbDOM["books"] = books;
	dbDOM["readers"] = users;
	dbDOM.Accept(writer);
	books = dbDOM["books"];
	users = dbDOM["readers"];
	fclose(fp);
}

void
lms::Database::
borrowBook() {
	std::cout << "输入借书人ID=>";
	string userID;
	std::cin >> userID;
	std::cout << "输入图书ID=>";
	string bookID;
	std::cin >> bookID;
	std::cout << "将 " << books[bookIDToSub[bookID]]["title"].GetString() << " 借给 " << users[userIDToSub[userID]]["name"].GetString() << " ?(y/n)";
	char confirm;
	std::cin >> confirm;
	if (confirm == 'y') {
		if (books[bookIDToSub[bookID]]["inventory"].GetInt() == 0) {
			std::cout << "书籍库存为0, 无法借出\n";
			return;
		} else {
			books[bookIDToSub[bookID]]["inventory"].SetInt(books[bookIDToSub[bookID]]["inventory"].GetInt() - 1);
			rapidjson::Value str(rapidjson::kStringType);
			str.SetString(userID.c_str(), allocator);
			books[bookIDToSub[bookID]]["borrowerID"].PushBack(str, allocator);
			str.SetString(bookID.c_str(), allocator);
			users[bookIDToSub[bookID]]["borrowedID"].PushBack(str, allocator);
			std::cout << "借出成功!\n";
		}
	} else {
		return;
	}
}

void
lms::Database::
returnBook() {
	std::cout << "输入还书人ID=>";
	string userID;
	std::cin >> userID;
	std::cout << "输入图书ID=>";
	string bookID;
	std::cin >> bookID;
	for (rapidjson::Value::ValueIterator itr = users[userIDToSub[userID]]["borrowedID"].Begin(); itr != users[userIDToSub[userID]]["borrowedID"].End(); ++itr) {
		if (itr -> GetString() == bookID) {
			users[userIDToSub[userID]]["borrowedID"].Erase(itr);
			break;
		}
	}
	for (rapidjson::Value::ValueIterator itr = books[userIDToSub[bookID]]["borrowerID"].Begin(); itr != books[userIDToSub[bookID]]["borrowerID"].End(); ++itr) {
		if (itr -> GetString() == userID) {
			books[bookIDToSub[bookID]]["borrowerID"].Erase(itr);
			break;
		}
	}
	books[bookIDToSub[bookID]]["inventory"].SetInt(books[bookIDToSub[bookID]]["inventory"].GetInt() + 1);
	std::cout << "还书成功!\n";
}

void
lms::Database::
mainInterface() {
Begin:
	std::cout << "图书管理系统 1：借书 2：还书 3：图书维护 4：用户维护 0：退出=>"; 
	int selection;
	std::cin >> selection;
	switch(selection) {
		case(1) : borrowBook(); break;
		case(2) : returnBook(); break;
		case(3) : booksInterface(); break;
		case(4) : usersInterface(); break;
		case(0) : saveChanges(); return;
	}
	goto Begin;
}

/**********************************************************************
 *                             Books Part                             *
 **********************************************************************/

void
lms::Database::
booksInterface() {
Begin:
	std::cout << "图书维护 1：新增 2：更改 3：删除 4：查找 5：显示 6：全删 0：退出=>"; 
	int selection;
	std::cin >> selection;
	switch(selection) {
		case(1) : addNewBook(); break;
		case(2) : break;
		case(3) : {
								std::cout << "删除 1: 按id删除 2: 按书名删除 3: 按ISBN删除=>";
								int selection2;
								std::cin >> selection2;
								switch(selection2) {
									case(1) :{
														 std::cout << "请输入ID:" << std::endl;
														 std::string id;
														 std::cin >> id;
														 deleteBookByID(id);
														 break;
													 }
									case(2) :{
														 std::cout << "请输入书名:" << std::endl;
														 std::string title;
														 std::cin.ignore();
														 std::getline(std::cin, title);
														 deleteBookByTitle(title);
														 break;
													 }
									case(3) :{
														 std::cout << "请输入ISBN:" << std::endl;
														 std::string isbn;
														 std::cin >> isbn;
														 deleteBookByISBN(isbn);
														 break;
													 }
								}
								break;
							}
		case(4) : {
								std::cout << "查找 1: 按id查找 2: 按书名查找 3: 按isbn查找=>";
								int selection2;
								std::cin >> selection2;
								switch(selection2) {
									case(1) :{
														 std::cout << "请输入ID:" << std::endl;
														 std::string id;
														 std::cin >> id;
														 searchBookByID(id);
														 break;
													 }
									case(2) :{
														 std::cout << "请输入书名:" << std::endl;
														 std::string title;
														 std::cin.ignore();
														 std::getline(std::cin, title);
														 searchBookByTitle(title);
														 break;
													 }
									case(3) :{
														 std::cout << "请输入ISBN:" << std::endl;
														 std::string isbn;
														 std::cin >> isbn;
														 searchBookByISBN(isbn);
														 break;
													 } } break; } case(5) : printAllBooks(); break;
		case(6) : deleteAllBooks(); break;
		case(0) : return;
	}
	goto Begin;
}

void
lms::Database::
addNewBook() {
	std::cin.ignore();
	rapidjson::Value book(rapidjson::kObjectType);
	std::string input;
	rapidjson::Value str(rapidjson::kStringType);
	std::cout << "请输入ID:";
	getline(std::cin, input);
	bookIDToSub[input] = books.Size();
	str.SetString(input.c_str(), allocator);
	book.AddMember("id", str, allocator);
	std::cout << "请输入书名:";
	getline(std::cin, input);
	bookTitleToSub[input] = books.Size();
	str.SetString(input.c_str(), allocator);
	book.AddMember("title", str, allocator);
	std::cout << "请输入作者:";
	getline(std::cin, input);
	str.SetString(input.c_str(), allocator);
	book.AddMember("author", str, allocator);
	std::cout << "请输入ISBN号:";
	getline(std::cin, input);
	bookISBNToSub[input] = books.Size();
	str.SetString(input.c_str(), allocator);
	book.AddMember("isbn", str, allocator);
	std::cout << "请输入语言:";
	getline(std::cin, input);
	str.SetString(input.c_str(), allocator);
	book.AddMember("language", str, allocator);
	std::cout << "请输入出版社:";
	getline(std::cin, input);
	str.SetString(input.c_str(), allocator);
	book.AddMember("press", str, allocator);
	book.AddMember("inventory", 1, allocator);
	book.AddMember("totalInventory", 1, allocator);
	books.PushBack(book, allocator);
}

void
lms::Database::
deleteBookByID(std::string id) {
	if (bookIDToSub.count(id) != 0) {
		int sub = bookIDToSub[id];
		bookIDToSub.erase(bookIDToSub.find(id));
		bookTitleToSub.erase(bookTitleToSub.find((std::string)books[sub]["title"].GetString()));
		bookISBNToSub.erase(bookISBNToSub.find((std::string)books[sub]["isbn"].GetString()));
		books[sub] = NULL;
		std::cout << "删除成功" << std::endl;
	} else {
		std::cerr << "书籍不存在, 删除失败" << std::endl;
	}
}

void
lms::Database::
deleteBookByTitle(std::string title) {
	if (bookTitleToSub.count(title) != 0) {
		int sub = bookTitleToSub[title];
		bookIDToSub.erase(bookIDToSub.find((std::string)books[sub]["id"].GetString()));
		bookTitleToSub.erase(bookTitleToSub.find(title));
		bookISBNToSub.erase(bookISBNToSub.find((std::string)books[sub]["isbn"].GetString()));
		books[sub] = NULL;
		std::cout << "删除成功" << std::endl;
	} else {
		std::cerr << "书籍不存在, 删除失败" << std::endl;
	}
}

void
lms::Database::
deleteBookByISBN(std::string isbn) {
	if (bookISBNToSub.count(isbn) != 0) {
		int sub = bookISBNToSub[isbn];
		bookIDToSub.erase(bookIDToSub.find((std::string)books[sub]["id"].GetString()));
		bookTitleToSub.erase(bookTitleToSub.find((std::string)books[sub]["title"].GetString()));
		bookISBNToSub.erase(bookISBNToSub.find(isbn));
		books[sub] = NULL;
		std::cout << "删除成功" << std::endl;
	} else {
		std::cerr << "书籍不存在, 删除失败" << std::endl;
	}
}

void
lms::Database::
searchBookByID(std::string id) const {
	if (bookIDToSub.count(id) == 0) {
		std::cerr << "该图书不存在" << std::endl;
	} else {
		printBook(bookIDToSub.at(id));
	}
}

void
lms::Database::
searchBookByTitle(std::string title) const {
	if (bookTitleToSub.count(title) == 0) {
		std::cerr << "该图书不存在" << std::endl;
	} else {
		printBook(bookTitleToSub.at(title));
	}
}

void
lms::Database::
searchBookByISBN(std::string isbn) const {
	if (bookISBNToSub.count(isbn) == 0) {
		std::cerr << "该图书不存在" << std::endl;
	} else {
		printBook(bookISBNToSub.at(isbn));
	}
}

void
lms::BookDatabase::
printBook(int sub) const {
	std::cout << "┏━━━━━━━━┳━━━━━━━┓ " << std::endl;
	std::cout << "┃　　　编号　　  ┃ " << books[sub]["id"].GetString() << std::endl;
	std::cout << "┣━━━━━━━━╋━━━━━━━┫ " << std::endl;
	std::cout << "┃　　　书名　　　┃ " << books[sub]["title"].GetString() << std::endl;
	std::cout << "┣━━━━━━━━╋━━━━━━━┫ " << std::endl;
	std::cout << "┃　　　作者　　　┃ " << books[sub]["author"].GetString() << std::endl;
	std::cout << "┣━━━━━━━━╋━━━━━━━┫ " << std::endl;
	std::cout << "┃　　　ISBN　　　┃ " << books[sub]["isbn"].GetString() << std::endl;
	std::cout << "┣━━━━━━━━╋━━━━━━━┫ " << std::endl;
	std::cout << "┃　　　语言　　　┃ " << books[sub]["language"].GetString() << std::endl;
	std::cout << "┣━━━━━━━━╋━━━━━━━┫ " << std::endl;
	std::cout << "┃　　 出版社 　　┃ " << books[sub]["press"].GetString() << std::endl;
	std::cout << "┣━━━━━━━━╋━━━━━━━┫ " << std::endl;
	std::cout << "┃　　 现库存 　　┃ " << books[sub]["inventory"].GetInt() << std::endl;
	std::cout << "┣━━━━━━━━╋━━━━━━━┫ " << std::endl;
	std::cout << "┃　　 总库存 　　┃ " << books[sub]["totalInventory"].GetInt() << std::endl;
	std::cout << "┣━━━━━━━━╋━━━━━━━┫ " << std::endl;
	std::cout << "┃　　借书人ID　　┃ ";
	for (rapidjson::Value::ValueIterator itr = books[sub]["borrowerID"].Begin(); itr != books[sub]["borrowerID"].End(); ++itr) {
		std::cout << itr -> GetString() <<", ";
	}
	std::cout << "\n┗━━━━━━━━┻━━━━━━━┛ " << std::endl;
}

void 
lms::BookDatabase::
printAllBooks() const {
	std::cout << std::setfill(' ') << std::setw(10) << std::setiosflags(std::ios::right) << "id"
		<< std::setfill(' ') << std::setw(20) << std::setiosflags(std::ios::right) << "title"
		<< std::setfill(' ') << std::setw(10) << std::setiosflags(std::ios::right) << "author"
		<< std::setfill(' ') << std::setw(15) << std::setiosflags(std::ios::right) << "isbn"
		<< std::setfill(' ') << std::setw(15) << std::setiosflags(std::ios::right) << "language"
		<< std::setfill(' ') << std::setw(15) << std::setiosflags(std::ios::right) << "press"
		<< std::setfill(' ') << std::setw(5) << std::setiosflags(std::ios::right) << "inv"
		<< std::setfill(' ') << std::setw(5) << std::setiosflags(std::ios::right) << "tIv"
		<< "    borrowerID" << std::endl;
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
	for (SizeType i = 0; i < books.Size(); ++i) {
		if (books[i] != NULL) {
			std::cout << std::setfill(' ') << std::setw(10) << std::setiosflags(std::ios::right) << books[i]["id"].GetString()
				<< std::setfill(' ') << std::setw(20) << std::setiosflags(std::ios::right) << books[i]["title"].GetString()
				<< std::setfill(' ') << std::setw(10) << std::setiosflags(std::ios::right) << books[i]["author"].GetString()
				<< std::setfill(' ') << std::setw(15) << std::setiosflags(std::ios::right) << books[i]["isbn"].GetString()
				<< std::setfill(' ') << std::setw(15) << std::setiosflags(std::ios::right) << books[i]["language"].GetString()
				<< std::setfill(' ') << std::setw(15) << std::setiosflags(std::ios::right) << books[i]["press"].GetString()
				<< std::setfill(' ') << std::setw(5) << std::setiosflags(std::ios::right) << books[i]["inventory"].GetInt()
				<< std::setfill(' ') << std::setw(5) << std::setiosflags(std::ios::right) << books[i]["totalInventory"].GetInt()
				<< "    ";
			for (rapidjson::Value::ValueIterator itr = books[i]["borrowerID"].Begin(); itr != books[i]["borrowerID"].End(); ++itr) {
				std::cout << itr -> GetString() <<", ";
			}
			std::cout << std::endl;
		}
	}
}

void
lms::BookDatabase::
deleteAllBooks() {
	std::cout << "危险: 确认删除所有图书信息?(y/n)";
	char yn;
	std::cin >> yn;
	if (yn == 'y') {
		for (rapidjson::Value::ValueIterator it = books.Begin(); it != books.End(); ) {
			it = books.Erase(it);
		}
		std::cout << "删除成功!\n";
	}
}

/**********************************************************************
 *                             User Parts                             *
 **********************************************************************/

void
lms::Database::
usersInterface() {
Begin:
	std::cout << "用户维护 1：新增 2：更改 3：删除 4：查找 5：显示 6：全删 0：退出=>"; 
	int selection;
	std::cin >> selection;
	switch(selection) {
		case(1) : addNewUser(); break;
		case(2) : break;
		case(3) : {
								std::cout << "删除 1: 按id删除 2: 按姓名删除=>";
								int selection2;
								std::cin >> selection2;
								switch(selection2) {
									case(1) :{
														 std::cout << "请输入ID:" << std::endl;
														 std::string id;
														 std::cin >> id;
														 deleteUserByID(id);
														 break;
													 }
									case(2) :{
														 std::cout << "请输入姓名:" << std::endl;
														 std::string name;
														 std::cin.ignore();
														 std::getline(std::cin, name);
														 deleteUserByName(name);
														 break;
													 }
								}
								break;
							}
		case(4) : {
								std::cout << "查找 1: 按id查找 2: 按姓名查找=>";
								int selection2;
								std::cin >> selection2;
								switch(selection2) {
									case(1) :{
														 std::cout << "请输入ID:" << std::endl;
														 std::string id;
														 std::cin >> id;
														 searchUserByID(id);
														 break;
													 }
									case(2) :{
														 std::cout << "请输入姓名:" << std::endl;
														 std::string name;
														 std::cin.ignore();
														 std::getline(std::cin, name);
														 searchUserByName(name);
														 break;
													 }
								}
								break;
							}
		case(5) : printAllUsers(); break;
		case(6) : deleteAllUsers(); break;
		case(0) : return;
	}
	goto Begin;
}

void
lms::Database::
addNewUser() {
	std::cin.ignore();
	rapidjson::Value user(rapidjson::kObjectType);
	std::string input;
	rapidjson::Value str(rapidjson::kStringType);
	std::cout << "请输入ID:";
	getline(std::cin, input);
	userIDToSub[input] = users.Size();
	str.SetString(input.c_str(), allocator);
	user.AddMember("id", str, allocator);
	std::cout << "请输入姓名:";
	getline(std::cin, input);
	userNameToSub[input] = users.Size();
	str.SetString(input.c_str(), allocator);
	user.AddMember("name", str, allocator);
	users.PushBack(user, allocator);
}

void
lms::Database::
deleteUserByID(std::string id) {
	if (userIDToSub.count(id) != 0) {
		int sub = userIDToSub[id];
		userIDToSub.erase(userIDToSub.find(id));
		userNameToSub.erase(userNameToSub.find((std::string)users[sub]["name"].GetString()));
		users[sub] = NULL;
		std::cout << "删除成功" << std::endl;
	} else {
		std::cerr << "用户不存在, 删除失败" << std::endl;
	}
}

void
lms::Database::
deleteUserByName(std::string name) {
	if (userNameToSub.count(name) != 0) {
		int sub = userIDToSub[name];
		userNameToSub.erase(userNameToSub.find(name));
		userIDToSub.erase(userIDToSub.find((std::string)users[sub]["id"].GetString()));
		users[sub] = NULL;
		std::cout << "删除成功" << std::endl;
	} else {
		std::cerr << "用户不存在, 删除失败" << std::endl;
	}
}

void
lms::Database::
searchUserByID(std::string id) const {
	if (userIDToSub.count(id) == 0) {
		std::cerr << "该用户不存在" << std::endl;
	} else {
		printUser(userIDToSub.at(id));
	}
}

void
lms::Database::
searchUserByName(std::string name) const {
	if (userNameToSub.count(name) == 0) {
		std::cerr << "该用户不存在" << std::endl;
	} else {
		printUser(userNameToSub.at(name));
	}

}

void
lms::UserDatabase::
printUser(int sub) const {
	std::cout << "┏━━━━━━━━┳━━━━━━━┓ " << std::endl;
	std::cout << "┃　　　编号　　  ┃ " << users[sub]["id"].GetString() << std::endl;
	std::cout << "┣━━━━━━━━╋━━━━━━━┫ " << std::endl;
	std::cout << "┃　　　姓名　　　┃ " << users[sub]["name"].GetString() << std::endl;
	std::cout << "┣━━━━━━━━╋━━━━━━━┫ " << std::endl;
	std::cout << "┃　　所借书ID　　┃ ";
	for (rapidjson::Value::ValueIterator itr = users[sub]["borrowedID"].Begin(); itr != users[sub]["borrowedID"].End(); ++itr) {
		std::cout << itr -> GetString() <<", ";
	}
	std::cout << "\n┗━━━━━━━━┻━━━━━━━┛ " << std::endl;
}

void
lms::UserDatabase::
printAllUsers() const {
	std::cout << std::setfill(' ') << std::setw(10) << std::setiosflags(std::ios::right) << "id"
		<< std::setfill(' ') << std::setw(20) << std::setiosflags(std::ios::right) << "name"
		<< std::setfill(' ') << std::setw(20) << std::setiosflags(std::ios::right) << "borrowedBooks" << std::endl;
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
	for (SizeType i = 0; i < users.Size(); ++i) {
		if (users[i] != NULL) {
			std::cout << std::setfill(' ') << std::setw(10) << std::setiosflags(std::ios::right) << users[i]["id"].GetString()
				<< std::setfill(' ') << std::setw(20) << std::setiosflags(std::ios::right) << users[i]["name"].GetString();
			std::cout << "       ";
			for (rapidjson::Value::ValueIterator itr = users[i]["borrowedID"].Begin(); itr != users[i]["borrowedID"].End(); ++itr) {
				std::cout << itr -> GetString() <<", ";
			}
			std::cout << std::endl;
		}
	}
}

void
lms::UserDatabase::
deleteAllUsers() {
	std::cout << "危险: 确认删除所有用户信息?(y/n)";
	char yn;
	std::cin >> yn;
	if (yn == 'y') {
		for (rapidjson::Value::ValueIterator it = users.Begin(); it != users.End(); ) {
			it = users.Erase(it);
		}
		std::cout << "删除成功!\n";
	}
}
