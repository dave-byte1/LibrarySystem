/*
    library_interface.cpp
    Author: M00865822
    Created: 11/01/2024
    Updated: 12/01/2024
*/
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "library.h"

std::vector<Member> members;

void addMemberInterface();
void issueBookInterface(std::vector<Book>& books);
void returnBookInterface(std::vector<Book>& books);
void displayBorrowedBooksInterface();
void calculateFinesInterface();

Librarian librarian(1, "Suzie", "123 Noon Street", "librarianSuzie@gmail.com", 41000);

int main() {
    std::vector<Book> books = readBooksFromCSV("library_books.csv");

    if (books.empty()) {
        std::cerr << "No books found in the CSV file." << std::endl;
        return 1;
    }

    int choice;

    std::cout << "----------------------------------------------" << std::endl;
    std::cout << "|            CST LIBRARY SYSTEM              |" << std::endl;
    std::cout << "----------------------------------------------" << std::endl;

    do {
        std::cout << "----------------------------------------------" << std::endl;
        std::cout << "|           Welcome Librarian " << librarian.getName() << "          |" << std::endl;
        std::cout << "|                                            |" << std::endl;
        std::cout << "|  Enter your choice:                        |" << std::endl;
        std::cout << "|--------------------------------------------|" << std::endl;
        std::cout << "|  1. Add a member.                          |" << std::endl;
        std::cout << "|  2. Issue a book to a member.              |" << std::endl;
        std::cout << "|  3. Return a book.                         |" << std::endl;
        std::cout << "|  4. Display books borrowed by a member.    |" << std::endl;
        std::cout << "|  5. Calculate fines for a member.          |" << std::endl;
        std::cout << "|                                            |" << std::endl;
        std::cout << "|  0. Exit the program.                      |" << std::endl;
        std::cout << "|--------------------------------------------|" << std::endl;

        std::cin >> choice;
        std::cin.clear();

        switch (choice) {
            case 1:
                addMemberInterface();
                break;
            case 2:
                issueBookInterface(books);
                break;
            case 3:
                returnBookInterface(books);
                break;
            case 4:
                displayBorrowedBooksInterface();
                break;
            case 5:
                calculateFinesInterface();
                break;
            case 0:
                break;
            default:
                std::cout << "Please enter a valid choice." << std::endl;
        }
    } while (choice != 0);

    return 0;
}

/*
    @brief Interface for the librarian to add a member to the library.

    This function will prompt the librarian to enter member details and then adds the new member
    to the library system. If the member ID is already taken it will exit the function.
*/
void addMemberInterface() {
    int ID;
    std::string name, address, email;

    std::cout << "Enter the member's ID: ";
    std::cin >> ID;
    std::cin.ignore();

    // Check if the member ID already exists
    for (const auto& member : members) {
        if (member.getMemberID() == ID) {
            std::cout << "Member ID " << ID << " is already taken. Please try a different ID." << std::endl;
            return;  // Exit the function if ID is taken
        }
    }

    std::cout << "Enter the member's name: ";
    std::getline(std::cin, name);

    std::cout << "Enter the member's home address: ";
    std::getline(std::cin, address);

    std::cout << "Enter the member's email: ";
    std::getline(std::cin, email);

    Member newMember(ID, name, address, email);
    members.push_back(newMember);

    std::cout << "Member added successfully!\n";
}

/*
    @brief Interface for the librarian to issue a book to a member.

    This function prompts the librarian for a member ID and a book ID, then issues the book
    to the member (if book is not borrowed) from the vector of books in the CSV file.

    @param books A reference to the vector of books available in the library.
*/
void issueBookInterface(std::vector<Book>& books) {
    int memberID, bookID;
    std::cout << "Enter member ID: ";
    std::cin >> memberID;

    std::cout << "Enter book ID: ";
    std::cin >> bookID;

    // Find the member and the book by ID
    for (auto& member : members) {
        // If member ID matches vector of members
        if (member.getMemberID() == memberID) {
            for (size_t i = 0; i < books.size(); ++i) {
                // Find book by book ID and issue the book when book ID matches
                if (books[i].getBookID() == bookID) {
                    librarian.issueBook(member, books, i + 1);  // Pass index to issueBook
                    return;
                }
            }
        }
    }

    std::cout << "Invalid member ID or book ID.\n";
}

/*
    @brief Interface to return a book to the library.

    This function prompts the librarian for a member ID and a book ID, then returns the book
    with the specified book ID. If the book ID or member ID is not valid no book will be returned.

    @param books Reference to the vector of books available.
*/
void returnBookInterface(std::vector<Book>& books) {
    int memberID, bookID;
    std::cout << "Enter member ID: ";
    std::cin >> memberID;

    std::cout << "Enter book ID: ";
    std::cin >> bookID;

    for (auto& member : members) {
        if (member.getMemberID() == memberID) {
            librarian.returnBook(member, bookID);
            return;
        }
    }

    std::cout << "Invalid member ID or book ID.\n";
}

/*
    @brief Interface to display borrowed books by a member.

    This function allows the librarian to see all borrowed books by a member
    identified by their member ID.
*/
void displayBorrowedBooksInterface() {
    int memberID;
    std::cout << "Enter member ID: ";
    std::cin >> memberID;

    for (const auto& member : members) {
        if (member.getMemberID() == memberID) {
            librarian.displayBorrowedBooks(const_cast<Member&>(member));
            return;
        }
    }

    std::cout << "Invalid member ID.\n";
}

/*
    @brief Interface to calculate fines for a member.

    This function allows the librarian to see a members fines (if any) by calling the
    'librarian.calculateFine' function from the Librarian class.
*/
void calculateFinesInterface() {
    int memberID;
    std::cout << "Enter member ID: ";
    std::cin >> memberID;

    for (const auto& member : members) {
        if (member.getMemberID() == memberID) {
            librarian.calculateFine(const_cast<Member&>(member));
            return;
        }
    }

    std::cout << "Invalid member ID.\n";
}