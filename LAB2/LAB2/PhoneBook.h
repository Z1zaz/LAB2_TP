#pragma once

#include "NOTE.h"

class PhoneBook {
private:
    NOTE** items;
    int count;
    int capacity;

    void ensureCapacity();
    void shiftRightFrom(int pos);
    void shiftLeftFrom(int pos);
public:
    PhoneBook();
    PhoneBook(const PhoneBook& other);
    PhoneBook& operator=(const PhoneBook& other);
    ~PhoneBook();

    int size() const { return count; }


    void insertAt(int pos, NOTE* note);

    
    void removeAt(int pos);

   
    void replaceAt(int pos, NOTE* note);

    
    NOTE* getAt(int pos) const;

    
    int findByPhone(const std::string& phone) const;

   
    void printAll() const;

    void sortByBirth();

    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    void clear();
};
