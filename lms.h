#ifndef _LMS_H_
#define _LMS_H_

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <string>
#include <map>
#include <fstream>
#include "include/rapidjson/filereadstream.h"
#include "include/rapidjson/filewritestream.h"
#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"

namespace lms {

	extern rapidjson::Document database;
	extern rapidjson::Value emptyValue;
	extern std::map<std::string, int> idToSub;
	extern std::map<std::string, int> titleToSub;
	extern std::map<std::string, int> isbnToSub;

	class Book {
		public:
		private:
			std::string id;
			std::string title;
			std::string author;
			std::string isbn;
			std::string language;
			std::string press;
			int inventory;
			int totalInventory;
	};

	class BookDatabase {
		public:
			BookDatabase() : books(emptyValue) {}
			friend void init(std::string, BookDatabase&);
			friend void saveBooks(std::string, BookDatabase);
			friend void saveChanges(std::string, BookDatabase);
			void addNewBook();
			void deleteByID(std::string);
			void printAllBooks();
			void searchBook(int id);
			void searchByID(std::string);
			void searchByTitle(std::string);
			void searchByISBN(std::string);
			void userInterface(std::string dbPath, BookDatabase& bookDB);
		private:
			void printBook(int);
			rapidjson::Value& books;
	};

	class Debug {
		public:
			static void printID();
	};

	void init(std::string dbPath, BookDatabase& bookDB);
	void saveBooks(std::string, BookDatabase);
	void saveChanges(std::string, BookDatabase);

}

#endif // _LMS_H_
