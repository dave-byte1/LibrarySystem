/*
    library.cpp
    Author: M00865822
    Created: 01/01/2024
    Updated: 015/01/2024
*/
#include "library.h"

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

Person::Person() = default;
// Constructor for the Person class
Person::Person(const std::string& name, const std::string& address, const std::string& email)
    : name(name), address(address), email(email) {}

std::string Person::getName() const {
    return name;
}
void Person::setName(const std::string& newName) {
    name = newName;
}

std::string Person::getAddress() const {
    return address;
}
void Person::setAddress(const std::string& newAddress) {
    address = newAddress;
}

std::string Person::getEmail() const {
    return email;
}
void Person::setEmail(const std::string& newEmail) {
    email = newEmail;
}

// Constructor for the Book class
Book::Book(int bookID, const std::string& bookName, const std::string& authorFirstName, const std::string& authorLastName)
    : bookID(bookID), bookName(bookName), authorFirstName(authorFirstName), authorLastName(authorLastName), borrower(nullptr) {}

int Book::getBookID() const {
    return bookID;
}
std::string Book::getBookName() const {
    return bookName;
}
std::string Book::getAuthorFirstName() const {
    return authorFirstName;
}
std::string Book::getAuthorLastName() const {
    return authorLastName;
}
std::time_t Book::getDueDate() const {
    return dueDate;
}
std::string Book::getDueDateAsString() const {
    return std::ctime(&dueDate);
}
void Book::setDueDate(std::time_t newDueDate) {
    dueDate = newDueDate;
}
void Book::returnBook() {
    borrower = nullptr;
    dueDate = 0;  // Reset the due date when book returned
}

void Book::borrowBook(Person* person, std::time_t newDueDate) {
    if (borrower != nullptr) {
        std::cerr << "Book '" << bookName << "' is not available for borrowing." << std::endl;
        return;
    }

    borrower = person;
    dueDate = newDueDate;
}

Member::Member(int memberID, const std::string& name, const std::string& address, const std::string& email)
    : Person(name, address, email), memberID(memberID) {}

// Getter function to retrieve member ID
int Member::getMemberID() const {
    return memberID;
}
// Getter function to retrieve the books borrowed by the member
std::vector<Book>& Member::getBooksBorrowed() {
    return booksLoaned;
}

// Function to add a borrowed book to the member's collection of books
void Member::setBooksBorrowed(const Book& book) {
    booksLoaned.push_back(book);
}

Librarian::Librarian(int staffID, const std::string& name, const std::string& address, const std::string& email, int salary)
    : Person(name, address, email), staffID(staffID), salary(salary) {}

int Librarian::getStaffID() const {
    return staffID;
}
void Librarian::setStaffID(int newStaffID) {
    staffID = newStaffID;
}

int Librarian::getSalary() const {
    return salary;
}
void Librarian::setSalary(int newSalary) {
    salary = newSalary;
}

// Function which lets the librarian add a member to the library
void Librarian::addMember(Member& member) {
    std::cout << "Librarian " << getName() << " added member " << member.getName() << std::endl;
}

/*
    @brief Issue a book to a member.

    This function allows the librarian to issue a book to a member if it's not being borrowed. First it checks that
    the librarian has entered a valid book index, then it checks if the book is available for borrowing,
    then sets the due date of the book, after that it updates the member's collection of books, and finally it
    prints a message indicating that the function succeeded.

    @param member The member who the book is issued to.
    @param books The vector of all the available books.
    @param bookIndex What index of book should be issued.
*/
void Librarian::issueBook(Member& member, std::vector<Book>& books, int bookIndex) {
    bookIndex = bookIndex - 1;
    if (bookIndex < 0 || bookIndex >= static_cast<int>(books.size())) {
        std::cerr << "Invalid book index." << std::endl;
        return;
    }

    Book& book = books[bookIndex];

    // Check if the book is already being borrowed
    if (book.getDueDate() != 0) {
        std::cerr << "Book '" << book.getBookName() << "' is not available for borrowing." << std::endl;
        return;
    }

    book.borrowBook(&member, std::time(nullptr) + 3 * 24 * 60 * 60);  // Due date set 3 days from now

    // Add borrowed book to the member's collection
    member.setBooksBorrowed(book);
    std::cout << "Librarian " << getName() << " issued book '" << book.getBookName() << "' to member " << member.getName() << std::endl;
}

/*
    @brief Returns a borrowed book to the library.

    This function allows the librarian to receive a borrowed book from a member.
    It searches for the book with the specified ID in the member's collection of books,
    returns the book, and removes it from the member's list of borrowed books.

    @param member The member returning a book.
    @param bookId The ID of the book to be returned.
*/
void Librarian::returnBook(Member& member, int bookId) {
    auto& booksLoaned = member.getBooksBorrowed();

    // Find the book with the specified ID in the member's collection of books
    auto it = std::find_if(booksLoaned.begin(), booksLoaned.end(), [bookId](const Book& book) {
        return book.getBookID() == bookId;
    });

    if (it != booksLoaned.end()) {
        // Return the book and remove it from the member's list
        it->returnBook();
        booksLoaned.erase(it);
        std::cout << "Librarian " << getName() << " reveived the book successfully." << std::endl;
    } else {
        std::cerr << "Book with ID " << bookId << " not found in the list of borrowed books." << std::endl;
    }
}

/*
    @brief Display the list of books borrowed by a member.

    This function prints the names and IDs of the books borrowed by a member.

    @param member The member whose borrowed books are to be displayed.
*/
void Librarian::displayBorrowedBooks(Member& member) {
    const auto& borrowedBooks = member.getBooksBorrowed();
    std::cout << "Books borrowed by member " << member.getName() << ":" << std::endl;

    // Iterate through the borrowed books and display their names and IDs
    for (const auto& book : borrowedBooks) {
        std::cout << " Book Name - " << book.getBookName() << std::endl;
        std::cout << " Book ID - " << book.getBookID() << '\n'
                  << std::endl;
    }
}

/*
    @brief Calculate and display the fines for overdue books borrowed by the member.

    This function iterates through the books borrowed by a member, checks if each book is overdue,
    calculates the fine based on the amount of days overdue (£1 per day overdue), and prints the fine amount.

    @param member The member for whom the fines are calculated.
*/
void Librarian::calculateFine(Member& member) {
    // Retrive the vector of borrowed books from the member
    const auto& borrowedBooks = member.getBooksBorrowed();

    // Iterate through the vector of borrowed books
    for (const auto& book : borrowedBooks) {
        std::time_t now = std::time(nullptr);
        // Check if the book is overdue
        if (book.getDueDate() < now) {
            int daysOverdue = static_cast<int>((now - book.getDueDate()) / (24 * 60 * 60));
            int fine = daysOverdue * 1;  // £1 per day overdue

            // Display the fine for the overdue book
            std::cout << "Fine for book '" << book.getBookName() << "': £" << fine << std::endl;
        } else {
            std::cout << "Book '" << book.getBookName() << "' is not overdue." << std::endl;
        }
    }
}

/*
    @brief Read books from a CSV file and return vector of Book objects

    This function reads books from the CSV file and creates Book objects based on the file's content.
    The function reads the columns for Book ID, Book name, Author First Name, and Author Last Name.

    @param filename The name of the CSV file to be read.
    @return A vector of book objects representing the books read from the CSV file.
*/
std::vector<Book> readBooksFromCSV(const std::string& filename) {
    std::vector<Book> books;       // Vector to store Book objects
    std::ifstream file(filename);  // Open CSV file

    // Check if file is open
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return books;
    }

    std::string line;
    std::getline(file, line);  // Read and ignore header line

    // While loop to iterate through each line in the file
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;

        // Tokenizing each line with ','
        while (ss.good()) {
            std::string token;
            std::getline(ss, token, ',');

            // Handle case when the book name has commas which are enclosed in ""
            if (!token.empty() && token.front() == '"') {
                std::string tempToken;
                std::getline(ss, tempToken, '"');
                token += "," + tempToken;
                ss.ignore();
            }

            tokens.push_back(token);
        }

        // If there are enough tokens to create a Book object
        if (tokens.size() >= 4) {
            int bookID = std::stoi(tokens[0]);        // 1st token is bookID
            std::string bookName = tokens[1];         // 2nd token is bookName
            std::string authorFirstName = tokens[3];  // 4th token is authorFirstName
            std::string authorLastName = tokens[4];   // 5th token is authorLastName

            // Create Book object and add it to the vector
            books.emplace_back(bookID, bookName, authorFirstName, authorLastName);
        }
    }

    file.close();
    return books;  // Return the vector of books
}