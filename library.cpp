/*
    library.cpp
    Author: M00865822
    Created: 01/01/2024
    Updated: 04/01/2024
*/

#include <ctime>
#include <iostream>

class Person {
   private:
    std::string name;
    std::string address;
    std::string email;

   public:
    Person() = default;
    // Constructor for the Person class
    Person(const std::string& name, const std::string& address, const std::string& email)
        : name(name), address(address), email(email) {}

    std::string getName() const {
        return name;
    }
    void setName(const std::string& newName) {
        name = newName;
    }

    std::string getAddress() const {
        return address;
    }
    void setAddress(const std::string& newAddress) {
        address = newAddress;
    }

    std::string getEmail() const {
        return email;
    }
    void setEmail(const std::string& newEmail) {
        email = newEmail;
    }
};

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
    Book(int bookID, const std::string& bookName, const std::string& authorFirstName, const std::string& authorLastName)
        : bookID(bookID), bookName(bookName), authorFirstName(authorFirstName), authorLastName(authorLastName), borrower(nullptr) {}

    int getBookID() const {
        return bookID;
    }
    std::string getBookName() const {
        return bookName;
    }
    std::string getAuthorFirstName() const {
        return authorFirstName;
    }
    std::string getAuthorLastName() const {
        return authorLastName;
    }
    std::time_t getDueDate() const {
        return dueDate;
    }
    std::string getDueDateAsString() const {
        return std::ctime(&dueDate);
    }
    void setDueDate(std::time_t newDueDate) {
        dueDate = newDueDate;
    }
    void returnBook() {
        borrower = nullptr;
    }

    void borrowBook(Person* person, int daysToAdd = 3) {
        borrower = person;

        // Get the current time
        std::time_t now = std::time(nullptr);

        // Set due date 3 days from now (24 hours * 60 minutes * 60 seconds)
        dueDate = now + daysToAdd * 24 * 60 * 60;
    }
};

int main() {
    // Testing the Person class
    Person person("Dan Man", "376 Fake St", "random@gmail.com");
    std::cout << "Person details: " << person.getName() << ", " << person.getAddress() << ", " << person.getEmail() << "\n"
              << std::endl;

    // Testing the Book class
    Book book(1, "Harry Potter", "J.K.", "Rowling");
    std::cout << "Book details: " << book.getBookName() << ", " << book.getAuthorFirstName() << " " << book.getAuthorLastName() << std::endl;

    book.borrowBook(nullptr);
    std::cout << "Due Date: " << book.getDueDateAsString() << std::endl;
    return 0;
}