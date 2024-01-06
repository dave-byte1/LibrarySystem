/*
    library.cpp
    Author: M00865822
    Created: 01/01/2024
    Updated: 06/01/2024
*/

#include <algorithm>
#include <ctime>
#include <iostream>
#include <vector>

class Book;

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

class Member : public Person {
   private:
    int memberID;
    std::vector<Book> booksLoaned;  // Collection of books loaned by the member

   public:
    Member(int memberID, const std::string& name, const std::string& address, const std::string& email)
        : Person(name, address, email), memberID(memberID) {}

    // Getter function to retrieve member ID
    int getMemberID() const {
        return memberID;
    }
    // Getter function to retrieve the books borrowed by the member
    std::vector<Book>& getBooksBorrowed() {
        return booksLoaned;
    }

    // Function to add a borrowed book to the member's collection of books
    void setBooksBorrowed(const Book& book) {
        booksLoaned.push_back(book);
    }
};

class Librarian : public Person {
   private:
    int staffID;
    int salary;

   public:
    Librarian(int staffID, const std::string& name, const std::string& address, const std::string& email, int salary)
        : Person(name, address, email), staffID(staffID), salary(salary) {}

    int getStaffID() const {
        return staffID;
    }
    void setStaffID(int newStaffID) {
        staffID = newStaffID;
    }

    int getSalary() const {
        return salary;
    }
    void setSalary(int newSalary) {
        salary = newSalary;
    }

    // Function which lets the librarian add a member to the library
    void addMember(Member& member) {
        std::cout << "Librarian " << getName() << " added member " << member.getName() << std::endl;
    }

    /*
        @brief Issue a book to a member.

        This function allows the librarian to issue a book to a member. First it checks that
        the librarian has entered a valid book index, then it sets the due date of the book,
        then it updates the member's collection of books, and finally it prints a message indicating
        that the function succeeded.

        @param member The member who the book is issued to.
        @param books The vector of all the available books.
        @param bookIndex What index of book should be issued.
    */
    void issueBook(Member& member, std::vector<Book>& books, int bookIndex) {
        bookIndex = bookIndex - 1;
        if (bookIndex < 0 || bookIndex >= books.size()) {
            std::cerr << "Invalid book index." << std::endl;
            return;
        }

        Book& book = books[bookIndex];

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
    void returnBook(Member& member, int bookId) {
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
    void displayBorrowedBooks(Member& member) {
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
    void calculateFine(Member& member) {
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
};

int main() {
    // Testing the Person class
    // Person person("Dan Man", "376 Fake St", "random@gmail.com");
    // std::cout << "Person details: " << person.getName() << ", " << person.getAddress() << ", " << person.getEmail() << "\n"
    //<< std::endl;

    // Testing the Book class
    // Book book(1, "Harry Potter", "J.K.", "Rowling");
    // std::cout << "Book details: " << book.getBookName() << ", " << book.getAuthorFirstName() << " " << book.getAuthorLastName() << std::endl;

    // book.borrowBook(nullptr);
    // std::cout << "Due Date: " << book.getDueDateAsString() << std::endl;

    // Testing the Member class
    // Member member(1, "Ben", "123 Fake St", "bens@gmail.com");
    // std::cout << "Member details: " << member.getName() << ", " << member.getAddress() << ", " << member.getMemberID() << std::endl;

    // Testing the Librarian class

    Librarian librarian(1, "Chloe", "123 Fake St", "chloeLib@gmail.com", 50000);

    Member member(1, "Jack", "674 ABC Road", "jack@email.com");

    librarian.addMember(member);

    // vector of books for testing
    std::vector<Book> books = {
        Book(1, "The Catcher in the Rye", "J.D.", "Salinger"),
        Book(2, "To Kill a Mockingbird", "Harper", "Lee"),
        Book(3, "1984", "George", "Orwell"),
    };

    librarian.issueBook(member, books, 1);
    librarian.issueBook(member, books, 2);

    librarian.displayBorrowedBooks(member);

    librarian.calculateFine(member);

    librarian.returnBook(member, 2);

    librarian.displayBorrowedBooks(member);

    return 0;
}