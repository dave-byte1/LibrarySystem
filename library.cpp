/*
    library.cpp
    Author: M00865822
    Created: 01/01/2024
    Updated: N/A
*/

#include <iostream>

class Person {
   private:
    std::string name;
    std::string address;
    std::string email;

   public:
    Person() = default;
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

int main() {
    // Testing the Person class

    Person person("Dan Man", "376 Fake St", "random@gmail.com");
    std::cout << "Person details: " << person.getName() << ", " << person.getAddress() << ", " << person.getEmail() << std::endl;

    return 0;
}