#include "NOTE.h"
#include <iomanip>
#include <limits>

using namespace std;

NOTE::NOTE() {
    lastName = "";
    firstName = "";
    phone = "";
    birth[0] = birth[1] = birth[2] = 0;
    cout << "NOTE: вызван конструктор по умолчанию\n";
}

NOTE::NOTE(const std::string& ln, const std::string& fn,
    const std::string& ph, int day, int month, int year) {
    lastName = ln;
    firstName = fn;
    phone = ph;
    birth[0] = day; birth[1] = month; birth[2] = year;
    cout << "NOTE: вызван параметрический конструктор (" << lastName << " " << firstName << ")\n";
}

NOTE::NOTE(const NOTE& other) {
    lastName = other.lastName;
    firstName = other.firstName;
    phone = other.phone;
    birth[0] = other.birth[0];
    birth[1] = other.birth[1];
    birth[2] = other.birth[2];
    cout << "NOTE: вызван конструктор копирования (" << lastName << " " << firstName << ")\n";
}

NOTE::~NOTE() {
    cout << "NOTE: вызван деструктор (" << lastName << " " << firstName << ")\n";
}

void NOTE::setLastName(const std::string& s) { lastName = s; }
void NOTE::setFirstName(const std::string& s) { firstName = s; }
void NOTE::setPhone(const std::string& s) { phone = s; }
void NOTE::setBirth(int day, int month, int year) {
    if (day < 1 || day > 31 || month < 1 || month > 12) throw NoteException("Неверная дата");
    birth[0] = day; birth[1] = month; birth[2] = year;
}

const std::string& NOTE::getLastName() const { return lastName; }
const std::string& NOTE::getFirstName() const { return firstName; }
const std::string& NOTE::getPhone() const { return phone; }
void NOTE::getBirth(int& day, int& month, int& year) const {
    day = birth[0]; month = birth[1]; year = birth[2];
}

int NOTE::compareBirth(const NOTE& other) const {
    if (birth[2] < other.birth[2]) return -1;
    if (birth[2] > other.birth[2]) return 1;
    if (birth[1] < other.birth[1]) return -1;
    if (birth[1] > other.birth[1]) return 1;
    if (birth[0] < other.birth[0]) return -1;
    if (birth[0] > other.birth[0]) return 1;
    return 0;
}

std::ostream& operator<<(std::ostream& os, const NOTE& n) {
    os << n.lastName << " " << n.firstName << ", тел: " << n.phone
        << ", дата рождения: " << n.birth[0] << "." << n.birth[1] << "." << n.birth[2];
    return os;
}


std::istream& operator>>(std::istream& is, NOTE& n) {
    std::string tmp;
    if (!std::getline(is, tmp)) return is;
    n.lastName = tmp;

    if (!std::getline(is, tmp)) return is;
    n.firstName = tmp;

    if (!std::getline(is, tmp)) return is;
    n.phone = tmp;

    if (!std::getline(is, tmp)) return is;
    std::istringstream ss(tmp);
    int d = 0, m = 0, y = 0;
    ss >> d >> m >> y;
    if (ss.fail()) {
        ss.clear();
        ss.str(tmp);
        ss >> d >> m >> y;
    }
    n.birth[0] = d; n.birth[1] = m; n.birth[2] = y;
    return is;
}

std::string NOTE::birthToString() const {
    char buf[32];
    printf(buf, "%02d.%02d.%04d", birth[0], birth[1], birth[2]);
    return std::string(buf);
}
