#define _CRT_SECURE_NO_WARNINGS
#include "PhoneBook.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

void PhoneBook::ensureCapacity() {
    if (capacity == 0) {
        capacity = 4;
        items = new NOTE * [capacity];
        for (int i = 0; i < capacity; ++i) items[i] = nullptr;
    }
    else if (count >= capacity) {
        int newcap = capacity * 2;
        NOTE** newarr = new NOTE * [newcap];
        for (int i = 0; i < newcap; ++i) newarr[i] = nullptr;
        for (int i = 0; i < count; ++i) newarr[i] = items[i];
        delete[] items;
        items = newarr;
        capacity = newcap;
    }
}

PhoneBook::PhoneBook() : items(nullptr), count(0), capacity(0) {
    cout << "PhoneBook: создан\n";
}

PhoneBook::PhoneBook(const PhoneBook& other) : items(nullptr), count(0), capacity(0) {
    cout << "PhoneBook: вызван конструктор копирования\n";
    if (other.count > 0) {
        capacity = other.count;
        items = new NOTE * [capacity];
        for (int i = 0; i < other.count; ++i) items[i] = new NOTE(*other.items[i]);
        count = other.count;
    }
}

PhoneBook& PhoneBook::operator=(const PhoneBook& other) {
    if (this == &other) return *this;
    clear();
    cout << "PhoneBook: вызван оператор присваивания\n";
    if (other.count > 0) {
        capacity = other.count;
        items = new NOTE * [capacity];
        for (int i = 0; i < other.count; ++i) items[i] = new NOTE(*other.items[i]);
        count = other.count;
    }
    return *this;
}

PhoneBook::~PhoneBook() {
    clear();
    cout << "PhoneBook: уничтожен\n";
}

void PhoneBook::shiftRightFrom(int pos) {

    for (int i = count; i > pos; --i) items[i] = items[i - 1];
}

void PhoneBook::shiftLeftFrom(int pos) {
    for (int i = pos; i < count - 1; ++i) items[i] = items[i + 1];
    items[count - 1] = nullptr;
}

void PhoneBook::insertAt(int pos, NOTE* note) {
    if (pos < 0 || pos > count) throw NoteException("insertAt: неверная позиция");
    ensureCapacity();

    if (pos == count) {
        items[count++] = note;
    }
    else {

        shiftRightFrom(pos);
        items[pos] = note;
        ++count;
    }
}

void PhoneBook::removeAt(int pos) {
    if (pos < 0 || pos >= count) throw NoteException("removeAt: неверная позиция");
    delete items[pos];
    shiftLeftFrom(pos);
    --count;

}

void PhoneBook::replaceAt(int pos, NOTE* note) {
    if (pos < 0 || pos >= count) throw NoteException("replaceAt: неверная позиция");
    delete items[pos];
    items[pos] = note;
}

NOTE* PhoneBook::getAt(int pos) const {
    if (pos < 0 || pos >= count) return nullptr;
    return items[pos];
}

int PhoneBook::findByPhone(const std::string& phoneNum) const {
    for (int i = 0; i < count; ++i) {
        if (items[i]->getPhone() == phoneNum) return i;
    }
    return -1;
}

void PhoneBook::printAll() const {
    if (count == 0) {
        cout << "PhoneBook пуст.\n";
        return;
    }
    for (int i = 0; i < count; ++i) {
        cout << "[" << i + 1 << "] " << *items[i] << "\n";
    }
}

void PhoneBook::sortByBirth() {

    for (int i = 1; i < count; ++i) {
        NOTE* key = items[i];
        int j = i - 1;
        while (j >= 0 && key->compareBirth(*items[j]) < 0) {
            items[j + 1] = items[j];
            --j;
        }
        items[j + 1] = key;
    }
}

void PhoneBook::saveToFile(const std::string& filename) const {
    ofstream out(filename.c_str());
    if (!out) throw NoteException("saveToFile: не могу открыть файл для записи");
    out << count << "\n";
    for (int i = 0; i < count; ++i) {

        out << items[i]->getLastName() << "\n";
        out << items[i]->getFirstName() << "\n";
        out << items[i]->getPhone() << "\n";
        int d, m, y; items[i]->getBirth(d, m, y);
        out << d << " " << m << " " << y << "\n";
    }
    out.close();
    cout << "Сохранено в " << filename << "\n";
}

void PhoneBook::loadFromFile(const std::string& filename) {
    ifstream in(filename.c_str());
    if (!in) throw NoteException("loadFromFile: не могу открыть файл для чтения");
    // clear current
    for (int i = 0; i < count; ++i) delete items[i];
    delete[] items;
    items = nullptr;
    count = 0;
    capacity = 0;

    int n = 0;
    in >> n;
    in.ignore(numeric_limits<std::streamsize>::max(), '\n');
    if (n < 0) n = 0;
    capacity = n > 4 ? n : 4;
    items = new NOTE * [capacity];
    for (int i = 0; i < capacity; ++i) items[i] = nullptr;

    for (int i = 0; i < n; ++i) {
        NOTE* p = new NOTE();
    
        string line;
        getline(in, line);
        p->setLastName(line);
        getline(in, line); 
        p->setFirstName(line);
        getline(in, line); 
        p->setPhone(line);
        getline(in, line); 
        int d = 0, m = 0, y = 0;
        scanf(line.c_str(), "%d %d %d", &d, &m, &y);
        p->setBirth(d, m, y);
        items[count++] = p;
    }
    cout << "Загружено из " << filename << "\n";
}

void PhoneBook::clear() {
    if (items) {
        for (int i = 0; i < count; ++i) delete items[i];
        delete[] items;
    }
    items = nullptr;
    count = 0;
    capacity = 0;
}
