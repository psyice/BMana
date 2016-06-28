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


	class BookDatabase {
		public:
			BookDatabase(jsonValue& jV) : books(jV) {}
			void saveChanges();
			virtual void addNewBook() = 0;
			virtual void deleteByID(std::string) = 0;
			virtual void deleteByTitle(std::string) = 0;
			virtual void deleteByISBN(std::string) = 0;
			//virtual void searchBook(int id) const = 0;
			virtual void searchBookByID(std::string) const = 0;
			virtual void searchBookByTitle(std::string) const = 0;
			virtual void searchBookByISBN(std::string) const = 0;
			void printAllBooks() const;
			void deleteAllBooks();
		protected:
			void printBook(int) const;
			jsonValue& books;
	};


	class UserDatabase {
		public:
		private:
	};

	class Database : public BookDatabase, public UserDatabase{
		public:
			Database(string dP) : BookDatabase(emptyValue), dbPath(dP)  {}
			void init();
			void saveBooks();
			void booksInterface();
			void addNewBook() override;
			void deleteByID(std::string) override;
			void deleteByTitle(std::string) override;
			void deleteByISBN(std::string) override;
			//void searchBook(int id) const override;
			void searchBookByID(std::string) const override;
			void searchBookByTitle(std::string) const override;
			void searchBookByISBN(std::string) const override;
		private:
			string dbPath;
			jsonDoc dbDOM;
			jsonDoc emptyDOM;
			jsonValue emptyValue;
			Allocator allocator;
			siMap idToSub;
			siMap titleToSub;
			siMap isbnToSub;
	};

	class Debug {
		public:
			static void printID();
			static void printISBN();
	};

}

#endif // _LMS_H_
