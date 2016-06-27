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
	using jsonDoc = rapidjson::Document;
	using jsonValue = rapidjson::Value;
	using string = std::string;
	using siMap = std::map<string, int>;
	using SizeType = rapidjson::SizeType;
	using Allocator = rapidjson::Document::AllocatorType;

	extern	jsonDoc emptyDOM;
	extern jsonValue emptyValue;
	extern Allocator& allocator;
	extern siMap idToSub;
	extern siMap titleToSub;
	extern siMap isbnToSub;

	class BookDatabase {
		public:
			BookDatabase() : books(emptyValue) {}
			void saveChanges();
			void addNewBook();
			void deleteByID(std::string);
			void deleteByTitle(std::string);
			void deleteByISBN(std::string);
			void searchBook(int id);
			void searchBookByID(std::string);
			void searchBookByTitle(std::string);
			void searchBookByISBN(std::string);
			void printAllBooks();
			void deleteAllBooks();
		private:
			void printBook(int); protected:
				jsonValue& books;
	};


	class UserDatabase {
		public:
		private:
	};

	class Database : public BookDatabase, public UserDatabase{
		public:
			Database(string dP) : dbPath(dP) {}
			void init();
			void saveBooks();
			void booksInterface();
		private:
			string dbPath;
			jsonDoc dbDOM;
	};

	class Debug {
		public:
			static void printID();
			static void printISBN();
	};

}

#endif // _LMS_H_
