#include "lms.h"

void
lms::Database::
addNewBook() {
	std::cin.ignore();
	rapidjson::Value book(rapidjson::kObjectType);
	std::string input;
	rapidjson::Value str(rapidjson::kStringType);
	std::cout << "请输入ID:";
	getline(std::cin, input);
	idToSub[input] = books.Size();
	str.SetString(input.c_str(), allocator);
	book.AddMember("id", str, allocator);
	std::cout << "请输入书名:";
	getline(std::cin, input);
	titleToSub[input] = books.Size();
	str.SetString(input.c_str(), allocator);
	book.AddMember("title", str, allocator);
	std::cout << "请输入作者:";
	getline(std::cin, input);
	str.SetString(input.c_str(), allocator);
	book.AddMember("author", str, allocator);
	std::cout << "请输入ISBN号:";
	getline(std::cin, input);
	isbnToSub[input] = books.Size();
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
deleteByID(std::string id) {
	if (idToSub.count(id) != 0) {
		int sub = idToSub[id];
		idToSub.erase(idToSub.find(id));
		titleToSub.erase(titleToSub.find((std::string)books[sub]["title"].GetString()));
		isbnToSub.erase(isbnToSub.find((std::string)books[sub]["isbn"].GetString()));
		books[sub] = NULL;
		std::cout << "删除成功" << std::endl;
	} else {
		std::cerr << "书籍不存在, 删除失败" << std::endl;
	}
}

void
lms::Database::
deleteByTitle(std::string title) {
	if (titleToSub.count(title) != 0) {
		int sub = titleToSub[title];
		idToSub.erase(idToSub.find((std::string)books[sub]["id"].GetString()));
		titleToSub.erase(titleToSub.find(title));
		isbnToSub.erase(isbnToSub.find((std::string)books[sub]["isbn"].GetString()));
		books[sub] = NULL;
		std::cout << "删除成功" << std::endl;
	} else {
		std::cerr << "书籍不存在, 删除失败" << std::endl;
	}
}

void
lms::Database::
deleteByISBN(std::string isbn) {
	if (isbnToSub.count(isbn) != 0) {
		int sub = isbnToSub[isbn];
		idToSub.erase(idToSub.find((std::string)books[sub]["id"].GetString()));
		titleToSub.erase(titleToSub.find((std::string)books[sub]["title"].GetString()));
		isbnToSub.erase(isbnToSub.find(isbn));
		books[sub] = NULL;
		std::cout << "删除成功" << std::endl;
	} else {
		std::cerr << "书籍不存在, 删除失败" << std::endl;
	}
}

void
lms::Database::
searchBookByID(std::string id) const {
	if (idToSub.count(id) == 0) {
		std::cerr << "该图书不存在" << std::endl;
	} else {
		printBook(idToSub.at(id));
	}
}

void
lms::Database::
searchBookByTitle(std::string title) const {
	if (titleToSub.count(title) == 0) {
		std::cerr << "该图书不存在" << std::endl;
	} else {
		printBook(titleToSub.at(title));
	}
}

void
lms::Database::
searchBookByISBN(std::string isbn) const {
	if (isbnToSub.count(isbn) == 0) {
		std::cerr << "该图书不存在" << std::endl;
	} else {
		printBook(isbnToSub.at(isbn));
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
	std::cout << "┗━━━━━━━━┻━━━━━━━┛ " << std::endl;
}

void 
lms::BookDatabase::
printAllBooks() const {
	std::cout << std::setfill(' ') << std::setw(10) << std::setiosflags(std::ios::right) << "id"
		<< std::setfill(' ') << std::setw(20) << std::setiosflags(std::ios::right) << "title"
		<< std::setfill(' ') << std::setw(20) << std::setiosflags(std::ios::right) << "author"
		<< std::setfill(' ') << std::setw(20) << std::setiosflags(std::ios::right) << "isbn"
		<< std::setfill(' ') << std::setw(15) << std::setiosflags(std::ios::right) << "language"
		<< std::setfill(' ') << std::setw(20) << std::setiosflags(std::ios::right) << "press"
		<< std::setfill(' ') << std::setw(10) << std::setiosflags(std::ios::right) << "inv"
		<< std::setfill(' ') << std::setw(10) << std::setiosflags(std::ios::right) << "totalInv" << std::endl;
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
	for (SizeType i = 0; i < books.Size(); ++i) {
		if (books[i] != NULL) {
			std::cout << std::setfill(' ') << std::setw(10) << std::setiosflags(std::ios::right) << books[i]["id"].GetString()
				<< std::setfill(' ') << std::setw(20) << std::setiosflags(std::ios::right) << books[i]["title"].GetString()
				<< std::setfill(' ') << std::setw(20) << std::setiosflags(std::ios::right) << books[i]["author"].GetString()
				<< std::setfill(' ') << std::setw(20) << std::setiosflags(std::ios::right) << books[i]["isbn"].GetString()
				<< std::setfill(' ') << std::setw(15) << std::setiosflags(std::ios::right) << books[i]["language"].GetString()
				<< std::setfill(' ') << std::setw(20) << std::setiosflags(std::ios::right) << books[i]["press"].GetString()
				<< std::setfill(' ') << std::setw(10) << std::setiosflags(std::ios::right) << books[i]["inventory"].GetInt()
				<< std::setfill(' ') << std::setw(10) << std::setiosflags(std::ios::right) << books[i]["totalInventory"].GetInt() << std::endl;
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

void
lms::Database::
init() {
	FILE* fp = fopen(dbPath.c_str(), "r");
	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	dbDOM.ParseStream(is);
	fclose(fp);
	books = dbDOM["books"];
	for (SizeType i = 0; i < books.Size(); ++i) {
		idToSub[books[i]["id"].GetString()] = i;
		titleToSub[books[i]["title"].GetString()] = i;
		isbnToSub[books[i]["isbn"].GetString()] = i;
	}
}

void
lms::Database::
saveBooks() {
	for (rapidjson::Value::ValueIterator itr = books.Begin(); itr != books.End(); ) {
		if (*itr == NULL) {
			itr =	books.Erase(itr);
		} else {
			++itr;
		}
	}
	FILE* fp = fopen(dbPath.c_str(), "w");
	char writeBuffer[65536];
	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
	dbDOM["books"] = books;
	dbDOM.Accept(writer);
	fclose(fp);
	books = dbDOM["books"];
	exit(0);
}

void
lms::Database::
booksInterface() {
UIBegin:
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
														 deleteByID(id);
														 break;
													 }
									case(2) :{
														 std::cout << "请输入书名:" << std::endl;
														 std::string title;
														 std::cin.ignore();
														 std::getline(std::cin, title);
														 deleteByTitle(title);
														 break;
													 }
									case(3) :{
														 std::cout << "请输入ISBN:" << std::endl;
														 std::string isbn;
														 std::cin >> isbn;
														 deleteByISBN(isbn);
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
													 }
								}
								break;
							}
		case(5) : printAllBooks(); break;
		case(6) : deleteAllBooks(); break;
		case(0) : saveBooks(); return;
		default : goto UIBegin;
	}
	goto UIBegin;
}

