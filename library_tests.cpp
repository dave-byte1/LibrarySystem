/*
    library_tests.cpp
    Author: M00865822
    Created: 09/01/2024
    Updated: 11/01/2024
*/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "library.h"

TEST_CASE("Book class functions work as expected", "[Book]")
{
    // Testing Book constructor
    Book book(1, "Test Book", "John", "Doe");

    // Testing Getter functions
    SECTION("Getter functions return correct values")
    {
        REQUIRE(book.getBookID() == 1);
        REQUIRE(book.getBookName() == "Test Book");
        REQUIRE(book.getAuthorFirstName() == "John");
        REQUIRE(book.getAuthorLastName() == "Doe");
        REQUIRE(book.getDueDate() == 0);
    }

    // Making sure due date function is working
    SECTION("SetDueDate function updates due date")
    {
        std::time_t newDueDate = std::time(nullptr) + 3 * 24 * 60 * 60;
        book.setDueDate(newDueDate);
        REQUIRE(book.getDueDate() == newDueDate);
    }

    // Making sure when a Person borrows a book its due date is set back to 0
    SECTION("BorrowBook and ReturnBook functions work as expected")
    {
        Person borrower("Borrower", "Address", "email@example.com");
        std::time_t dueDate = std::time(nullptr) + 3 * 24 * 60 * 60;

        book.borrowBook(&borrower, dueDate);
        REQUIRE(book.getDueDate() == dueDate);
        REQUIRE(book.getBookID() == 1);

        book.returnBook();
        REQUIRE(book.getDueDate() == 0);
        REQUIRE(book.getBookID() == 1);
    }
}

// Testing Member class
TEST_CASE("Member class functions work as expected", "[Member]")
{
    Member member(1, "John", "Address", "john@example.com");

    // Testing getter functions
    SECTION("Getter functions return correct values")
    {
        REQUIRE(member.getMemberID() == 1);
        REQUIRE(member.getName() == "John");
        REQUIRE(member.getAddress() == "Address");
        REQUIRE(member.getEmail() == "john@example.com");
    }

    // Making sure the correct books are set to the correct Member
    SECTION("getBooksBorrowed and setBooksBorrowed functions work as expected")
    {
        Book book1(1, "Book1", "Author1", "Last1");
        Book book2(2, "Book2", "Author2", "Last2");

        member.setBooksBorrowed(book1);
        member.setBooksBorrowed(book2);

        auto &borrowedBooks = member.getBooksBorrowed();
        REQUIRE(borrowedBooks.size() == 2);
        REQUIRE(borrowedBooks[0].getBookName() == "Book1");
        REQUIRE(borrowedBooks[1].getBookName() == "Book2");
    }
}

TEST_CASE("Librarian class functions work as expected", "[Librarian]")
{
    Librarian librarian(1, "Librarian", "Lib Address", "librarian@example.com", 50000);
    Member member(1, "John", "Address", "john@example.com");

    SECTION("Getter functions return correct values")
    {
        REQUIRE(librarian.getStaffID() == 1);
        REQUIRE(librarian.getName() == "Librarian");
        REQUIRE(librarian.getAddress() == "Lib Address");
        REQUIRE(librarian.getEmail() == "librarian@example.com");
        REQUIRE(librarian.getSalary() == 50000);
    }

    SECTION("addMember function works as expected")
    {
        REQUIRE_NOTHROW(librarian.addMember(member));
    }

    SECTION("issueBook, returnBook, displayBorrowedBooks, and calculateFine functions work as expected")
    {

        Member member2(2, "Jackie", "674 ABC Roads", "jack@email.comm");
        // Vector of books for testing
        std::vector<Book> books = readBooksFromCSV("library_books.csv");

        REQUIRE_NOTHROW(librarian.addMember(member2));

        // Issue books to a member
        REQUIRE_NOTHROW(librarian.issueBook(member2, books, 1));
        REQUIRE_NOTHROW(librarian.issueBook(member2, books, 2));

        // Display borrowed books from member
        REQUIRE_NOTHROW(librarian.displayBorrowedBooks(member2));

        // Calculate fines
        REQUIRE_NOTHROW(librarian.calculateFine(member2));

        // Return a book from a member
        REQUIRE_NOTHROW(librarian.returnBook(member2, 1));
        REQUIRE_NOTHROW(librarian.displayBorrowedBooks(member2));
    }
}
