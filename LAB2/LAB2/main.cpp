#include "PhoneBook.h"
#include "NOTE.h"
#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <algorithm>
#define NOMINMAX
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;


void readLine(const string& prompt, string& out) {
    cout << prompt;
    std::getline(cin, out);

}



NOTE* createNoteInteractive() {
    string ln, fn, ph;
    int d, m, y;
    cout << "Введите фамилию: "; getline(cin, ln);
    cout << "Введите имя: "; getline(cin, fn);
    cout << "Введите номер телефона: "; getline(cin, ph);
    cout << "Введите дату рождения (дд мм гггг): ";
    while (!(cin >> d >> m >> y)) {
        cout << "Неверный ввод. Введите три числа (дд мм гггг): ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    NOTE* p = new NOTE(ln, fn, ph, d, m, y);
    return p;
}


static inline std::string trimLeft(const std::string& s) {
    size_t i = 0;
    while (i < s.size() && isspace((unsigned char)s[i])) ++i;
    return s.substr(i);
}

void printSentencesStartingWithDash(const std::string& filename) {
    ifstream in(filename.c_str());
    if (!in) {
        cout << "Не могу открыть файл: " << filename << "\n";
        return;
    }
   
    std::string text, line;
    while (std::getline(in, line)) {
        text += line;
        text += '\n';
    }
    in.close();

    
    std::string accum;
    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i];
        accum.push_back(c);
        if (c == '.' || c == '!' || c == '?') {
            string s = accum;
            string left = trimLeft(s);
            if (!left.empty() && (left[0] == '-' || left[0] == (char)0xE2 /* possible utf8 em-dash start */)) {
               
                cout << trimLeft(accum) << "\n";
            }
            accum.clear();
        }
    }
    
    if (!accum.empty()) {
        string left = trimLeft(accum);
        if (!left.empty() && (left[0] == '-' || left[0] == (char)0xE2)) {
            cout << left << "\n";
        }
    }
}

int main() {
    
    system("chcp 1251 > nul");
    setlocale(LC_ALL, "Russian");

    PhoneBook pb;
    int choice = -1;

    while (true) {
        cout << "\n=== Меню ===\n"
            << "1. Добавить запись (в конец)\n"
            << "2. Вставить запись на позицию\n"
            << "3. Удалить запись по номеру в списке\n"
            << "4. Редактировать запись по номеру в списке\n"
            << "5. Показать все записи\n"
            << "6. Найти по телефону\n"
            << "7. Отсортировать по дате рождения\n"
            << "8. Сохранить в файл\n"
            << "9. Загрузить из файла\n"
            << "10. Задача 2: вывести предложения, начинающиеся с тире из файла\n"
            << "0. Выход\n"
            << "Выбор: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        try {
            if (choice == 1) {
                NOTE* p = createNoteInteractive();
                pb.insertAt(pb.size(), p);
            }
            else if (choice == 2) {
                int pos;
                cout << "Введите позицию для вставки (1.." << pb.size() + 1 << "): ";
                while (!(cin >> pos)) {
                    cout << "Неверный ввод. Введите число: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (pos < 1 || pos > pb.size() + 1) {
                    cout << "Неверная позиция\n";
                }
                else {
                    NOTE* p = createNoteInteractive();
                    pb.insertAt(pos - 1, p);
                }
            }
            else if (choice == 3) {
                if (pb.size() == 0) { cout << "Список пуст\n"; continue; }
                int pos;
                cout << "Введите номер записи для удаления (1.." << pb.size() << "): ";
                while (!(cin >> pos)) {
                    cout << "Неверный ввод. Введите число: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                pb.removeAt(pos - 1);
            }
            else if (choice == 4) {
                if (pb.size() == 0) { cout << "Список пуст\n"; continue; }
                int pos;
                cout << "Введите номер записи для редактирования (1.." << pb.size() << "): ";
                while (!(cin >> pos)) {
                    cout << "Неверный ввод. Введите число: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (pos < 1 || pos > pb.size()) { cout << "Неверная позиция\n"; continue; }
                cout << "Ввод новых данных для записи " << pos << ":\n";
                NOTE* p = createNoteInteractive();
                pb.replaceAt(pos - 1, p);
            }
            else if (choice == 5) {
                pb.printAll();
            }
            else if (choice == 6) {
                string tel;
                cout << "Введите номер телефона для поиска: ";
                getline(cin, tel);
                int idx = pb.findByPhone(tel);
                if (idx == -1) cout << "Человек с таким номером не найден\n";
                else cout << "Найдена запись [" << idx + 1 << "]: " << *pb.getAt(idx) << "\n";
            }
            else if (choice == 7) {
                pb.sortByBirth();
                cout << "Отсортировано\n";
            }
            else if (choice == 8) {
                string fname;
                cout << "Имя файла для сохранения: ";
                getline(cin, fname);
                pb.saveToFile(fname);
            }
            else if (choice == 9) {
                string fname;
                cout << "Имя файла для загрузки: ";
                getline(cin, fname);
                pb.loadFromFile(fname);
            }
            else if (choice == 10) {
                string fname;
                cout << "Имя файла для анализа: ";
                getline(cin, fname);
                printSentencesStartingWithDash(fname);
            }
            else if (choice == 0) {
                cout << "Выход...\n";
                break;
            }
            else {
                cout << "Неверный пункт меню\n";
            }
        }
        catch (const NoteException& ex) {
            cout << "Исключение: " << ex.what() << "\n";
        }
        catch (const std::exception& ex) {
            cout << "std::exception: " << ex.what() << "\n";
        }
    }

    return 0;
}
