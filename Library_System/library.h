/*
    library.h
    Author: M00865822
    Created: 011/01/2024
    Updated: 012/01/2024
*/

#ifndef LIBRARY_H
#define LIBRARY_H

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// CLASS DECLERATIONS
class Book;
class Person;
class Member;
class Librarian;

// Person class declaration
class Person {
   private:
    std::string name;
    std::string address;
    std::string email;

   public:
    Person();
    Person(const std::string& name, const std::string& address, const std::string& email);

    std::string getName() const;
    void setName(const std::string& newName);

    std::string getAddress() const;
    void setAddress(const std::string& newAddress);

    std::string getEmail() const;
    void setEmail(const std::string& newEmail);
};

// Book class declaration
class Book {
   private:
    int bookID;
    std::string bookName;
    std::string authorFirstName;
    std::string authorLastName;
    std::string bookType;
    std::time_t dueDate;
    Person* borrower;

   public:
    // Constructor for the Book class
    Book(int bookID, const std::string& bookName, const std::string& authorFirstName, const std::string& authorLastName);

    int getBookID() const;
    std::string getBookName() const;
    std::string getAuthorFirstName() const;
    std::string getAuthorLastName() const;
    std::time_t getDueDate() const;
    std::string getDueDateAsString() const;
    void setDueDate(std::time_t newDueDate);
    void returnBook();

    void borrowBook(Person* person, std::time_t newDueDate);
};

// Member class declaration
class Member : public Person {
   private:
    int memberID;
    std::vector<Book> booksLoaned;  // Collection of books loaned by the member

   public:
    Member(int memberID, const std::string& name, const std::string& address, const std::string& email);

    // Getter function to retrieve member ID
    int getMemberID() const;
    // Getter function to retrieve the books borrowed by the member
    std::vector<Book>& getBooksBorrowed();

    // Function to add a borrowed book to the member's collection of books
    void setBooksBorrowed(const Book& book);
};

// Librarian class declaration
class Librarian : public Person {
   private:
    int staffID;
    int salary;

   public:
    Librarian(int staffID, const std::string& name, const std::string& address, const std::string& email, int salary);

    int getStaffID() const;
    void setStaffID(int newStaffID);

    int getSalary() const;
    void setSalary(int newSalary);

    void addMember(Member& member);

    void issueBook(Member& member, std::vector<Book>& books, int bookIndex);

    void returnBook(Member& member, int bookId);

    void displayBorrowedBooks(Member& member);

    void calculateFine(Member& member);
};

// Function declaration
std::vector<Book> readBooksFromCSV(const std::string& filename);

#endif 
