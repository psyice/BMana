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
			virtual void addNewBook() = 0;
			virtual void deleteBookByID(std::string) = 0;
			virtual void deleteBookByTitle(std::string) = 0;
			virtual void deleteBookByISBN(std::string) = 0;
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
			UserDatabase(jsonValue& jV) : users(jV) {}
			virtual void addNewUser() = 0;
			virtual void deleteUserByID(std::string) = 0;
			virtual void deleteUserByName(std::string) = 0;
			virtual void searchUserByID(std::string) const = 0;
			virtual void searchUserByName(std::string) const = 0;
			void printAllUsers() const;
			void deleteAllUsers();
		protected:
			void printUser(int) const;
			jsonValue& users;
	};

	class Database : public BookDatabase, public UserDatabase{
		public:
			Database() : BookDatabase(emptyBooks), UserDatabase(emptyUsers) {}
			void init();
			void saveChanges();
			//Books
			void booksInterface();
			void addNewBook() override;
			void deleteBookByID(std::string) override;
			void deleteBookByTitle(std::string) override;
			void deleteBookByISBN(std::string) override;
			void searchBookByID(std::string) const override;
			void searchBookByTitle(std::string) const override;
			void searchBookByISBN(std::string) const override;
			//Users
			void usersInterface();
			void addNewUser() override;
			void deleteUserByID(std::string) override;
			void deleteUserByName(std::string) override;
			void searchUserByID(std::string) const override;
			void searchUserByName(std::string) const override;
		private:
			jsonDoc dbDOM;
			jsonDoc emptyDOM;
			jsonValue emptyBooks;
			jsonValue emptyUsers;
			Allocator allocator;
			siMap bookIDToSub;
			siMap bookTitleToSub;
			siMap bookISBNToSub;
			siMap userIDToSub;
			siMap userNameToSub;
	};

	class Debug {
		public:
			static void printID();
			static void printISBN();
	};

}

#endif // _LMS_H_
