#pragma once

#include <iostream>
#include <string>
#include <exception>
#include <sstream>

class NoteException : public std::exception {
    std::string msg;
public:
    NoteException(const std::string& m) : msg(m) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

class NOTE {
private:
    std::string lastName;
    std::string firstName;
    std::string phone;
    int birth[3]; 

public:
    
    NOTE();
    NOTE(const std::string& ln, const std::string& fn,
        const std::string& ph, int day, int month, int year);
    NOTE(const NOTE& other);
    ~NOTE();

    
    void setLastName(const std::string& s);
    void setFirstName(const std::string& s);
    void setPhone(const std::string& s);
    void setBirth(int day, int month, int year);

    const std::string& getLastName() const;
    const std::string& getFirstName() const;
    const std::string& getPhone() const;
    void getBirth(int& day, int& month, int& year) const;

    
    int compareBirth(const NOTE& other) const;

    // stream operators
    friend std::ostream& operator<<(std::ostream& os, const NOTE& n);
    friend std::istream& operator>>(std::istream& is, NOTE& n);

    // helper to format birthdate
    std::string birthToString() const;
};


