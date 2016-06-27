#include "lms.h"

rapidjson::Document lms::database;
rapidjson::Value lms::emptyValue;
std::map<std::string, int> lms::idToSub;
std::map<std::string, int> lms::titleToSub;
std::map<std::string, int> lms::isbnToSub;

void
lms::
init(std::string dbPath, lms::BookDatabase& bookDB) {
	FILE* fp = fopen(dbPath.c_str(), "r");
	char readBuffer[65536];
	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	lms::database.ParseStream(is);
	fclose(fp);
	bookDB.books = database["books"];
	for (rapidjson::SizeType i = 0; i < bookDB.books.Size(); ++i) {
			lms::idToSub[bookDB.books[i]["id"].GetString()] = i;
			lms::titleToSub[bookDB.books[i]["title"].GetString()] = i;
			lms::isbnToSub[bookDB.books[i]["isbn"].GetString()] = i;
	}
}

void
lms::BookDatabase::
addNewBook() {
	std::cin.ignore();
	rapidjson::Document::AllocatorType& allocator = lms::database.GetAllocator();
	rapidjson::Value book(rapidjson::kObjectType);
	std::string input;
	rapidjson::Value str(rapidjson::kStringType);
	std::cout << "请输入ID:";
	getline(std::cin, input);
	str.SetString(input.c_str(), allocator);
	book.AddMember("id", str, allocator);
	std::cout << "请输入书名:";
	getline(std::cin, input);
	str.SetString(input.c_str(), allocator);
	book.AddMember("title", str, allocator);
	std::cout << "请输入作者:";
	getline(std::cin, input);
	str.SetString(input.c_str(), allocator);
	book.AddMember("author", str, allocator);
	std::cout << "请输入ISBN号:";
	getline(std::cin, input);
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
lms::BookDatabase::
deleteByID(std::string id) {
	// FIXME 首次删除会导致Array下标变化, 进而导致第二次删除会失败
	if (lms::idToSub.count(id) != 0) {
		int sub = lms::idToSub[id];
		lms::idToSub.erase(idToSub.find(id));
		lms::titleToSub.erase(titleToSub.find((std::string)books[sub]["title"].GetString()));
		lms::isbnToSub.erase(isbnToSub.find((std::string)books[sub]["isbn"].GetString()));
		books[sub] = NULL;
		std::cout << "删除成功" << std::endl;
	} else {
		std::cerr << "书籍不存在, 删除失败" << std::endl;
	}
}

void
lms::BookDatabase::
printBook(int sub) {
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
searchByID(std::string id) {
	if (lms::idToSub[id] == 0) {
		std::cerr << "该图书不存在" << std::endl;
	} else {
		printBook(lms::idToSub[id]);
	}
}

void
lms::BookDatabase::
searchByTitle(std::string title) {
	if (lms::titleToSub[title] == 0) {
		std::cerr << "该图书不存在" << std::endl;
	} else {
		printBook(lms::titleToSub[title]);
	}
}

void
lms::BookDatabase::
searchByISBN(std::string isbn) {
	if (lms::isbnToSub[isbn] == 0) {
		std::cerr << "该图书不存在" << std::endl;
	} else {
		printBook(lms::isbnToSub[isbn]);
	}
}

void 
lms::BookDatabase::
printAllBooks() {
	std::cout << std::setfill(' ') << std::setw(10) << std::setiosflags(std::ios::right) << "id"
		<< std::setfill(' ') << std::setw(20) << std::setiosflags(std::ios::right) << "title"
		<< std::setfill(' ') << std::setw(20) << std::setiosflags(std::ios::right) << "author"
		<< std::setfill(' ') << std::setw(20) << std::setiosflags(std::ios::right) << "isbn"
		<< std::setfill(' ') << std::setw(15) << std::setiosflags(std::ios::right) << "language"
		<< std::setfill(' ') << std::setw(20) << std::setiosflags(std::ios::right) << "press"
		<< std::setfill(' ') << std::setw(10) << std::setiosflags(std::ios::right) << "inv"
		<< std::setfill(' ') << std::setw(10) << std::setiosflags(std::ios::right) << "totalInv" << std::endl;
	std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
	for (rapidjson::SizeType i = 0; i < books.Size(); ++i) {
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
lms::Debug::
printID() {
	for (auto i : idToSub) {
		std::cout << i.first << " " << i.second << std::endl;
	}
}

void
lms::
saveBooks(std::string dbPath, BookDatabase bookDB) {
	//for (rapidjson::Value::ValueIterator itr = bookDB.books.Begin(); itr != bookDB.books.End(); ) {
	//if (*itr == NULL) {
	//itr =	bookDB.books.Erase(itr);
	//} else {
	//++itr;
	//}
	//}
	FILE* fp = fopen(dbPath.c_str(), "w");
	char writeBuffer[65536];
	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
	lms::database["books"] = bookDB.books;
	lms::database.Accept(writer);
	fclose(fp);
	bookDB.books = lms::database["books"];
}

void
lms::
saveChanges(std::string dbPath, BookDatabase bookDB) {
	for (rapidjson::Value::ValueIterator itr = bookDB.books.Begin(); itr != bookDB.books.End(); ) {
		if (*itr == NULL) {
			itr =	bookDB.books.Erase(itr);
		} else {
			++itr;
		}
	}
	FILE* fp = fopen(dbPath.c_str(), "w");
	char writeBuffer[65536];
	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
	lms::database["books"] = bookDB.books;
	lms::database.Accept(writer);
	fclose(fp);
	bookDB.books = lms::database["books"];
	exit(0);
}

void
lms::BookDatabase::
userInterface(std::string dbPath, BookDatabase& bookDB) {
UIBegin:
	std::cout << "图书维护 1：新增 2：更改 3：删除 4：查找 5：显示 6：全删 0：退出=>"; 
	int selection;
	std::cin >> selection;
	switch(selection) {
		case(1) : addNewBook(); saveBooks(dbPath, bookDB); break;
		case(2) : break;
		case(3) : {
								std::cout << "删除 1: 按id删除 2: 按书名删除 3: 按isbn删除=>";
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
								}
								saveBooks(dbPath, bookDB);
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
														 searchByID(id);
														 break;
													 }
								}
								break;
							}
		case(5) : printAllBooks(); break;
		case(6) : break;
		case(0) : lms::saveChanges(dbPath, bookDB);
		default : goto UIBegin;
	}
	goto UIBegin;
}
