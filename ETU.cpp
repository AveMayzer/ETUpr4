#include <iostream>
#include <windows.h>
#include <fstream>
#include <sstream>

using namespace std;

const int N = 650;

int len = 0;

void PrintText(char* text, int* len) {
    unsigned int j = 0;
    for (unsigned int i = 0; i < *len; i++, j++) {
        cout << text[i];
    }
    text[j++] = '\0';
}

void InputFromFile(int N, char* text, char* FileName) {
    ifstream File;

    File.open(FileName);
    if (!File.is_open()) {
        cout << "Файл не удалось открыть." << endl;
    }

    File.getline(text, N, '.');
    File.close();
}

void TextLenght(char* text, int* len) {
    unsigned L = 0;
    
    while (text[L]) {
        ++L;
    }
    *len = L;
}

void FirstEdit(char* text, int* len) {
    unsigned int j = 0;
    int beginLen = *len;
    for (int i = 0; i < beginLen; i++) {
        if (text[i] == ' ' && text[i + 1] == ' ') { // Блок с проверкой на повторяющиеся пробелы.
            continue;
        }

        if (i == 0 || (text[j - 1] == ' ')) { // Блок с проверкой на регистр.
            text[j++] = toupper(text[i]); // Ставит букву заглавной в каждом слове. (под вопросом)
        }

        else {
            text[j++] = tolower(text[i]); 
        }

        if (ispunct(text[i]) && (ispunct(text[i + 1]))) { // Блок с проверкой на повторяющиеся знаки препинания.
            int cnt = 1;
            while (i + cnt < beginLen && ispunct(text[i + cnt])) {
                cnt++;
            }
            i += cnt;
            text[j++] = text[i];
        }

    }
    *len = j;
    text[j] = '\0';
    
}

void PrintWordsWithoutDigits(char* text, int* len) {
    int start = 0;
    bool hasDigit = false;

    for (int i = 0; i <= *len; i++) { 
        if (isspace(text[i]) || text[i] == '\0') {
            if (!hasDigit && start < i) {
                for (int j = start; j < i; j++) {
                    cout << text[j];
                }
                cout << " ";
            }
            hasDigit = false;
            start = i + 1;
        }

        else if (isdigit(text[i])) {
            hasDigit = true;
        }
    }

    cout << endl;
}

void PrintWordsLenght(char* text, int* len) {
    int start = 0;

    for (int i = 0; i <= *len; i++) {
        if (isspace(text[i]) || text[i] == '\0') {
            if (start < i) {
                int j = start;
                for (j; j < i; j++) {
                    cout << text[j];
                }
                cout << "(" << j - start << ") ";
            }
            start = i + 1;
        }
    }
    cout << endl;
}

void InputFromKeyboard(char* text, int N) {
    cin.ignore();
    cin.getline(text, N, '.');
    cin.sync();
}

int LinearSubText(char* text, char* subText, int* textLen) {
    int subLen = 0;
    while (subText[subLen] != '\0') {
        subLen++;
    }
    for (int i = 0; i < *textLen - subLen; i++) {
        int j;
        for (j = 0; j < subLen; j++) {
            if (text[i + j] != subText[j]) {
                break;
            }
        }
        if (j == subLen) { 
            return i;
        }
    }

    return -1;
}


int BoyerMooreSearch(char* text, char* subText, int* textLen) {
    int subLen = 0;
    while (subText[subLen] != '\0') {
        subLen++;
    }

    int badChar[256];
    for (int i = 0; i < 256; i++) {
        badChar[i] = -1;
    }
    for (int i = 0; i < subLen; i++) {
        badChar[(unsigned char)subText[i]] = i;
    }

    int shift = 0;
    while (shift <= (*textLen - subLen)) {
        int j = subLen - 1;

        while (j >= 0 && subText[j] == text[shift + j]) {
            j--;
        }

        if (j < 0) {
            return shift; 
        }
        else {
            shift += max(1, j - badChar[(unsigned char)text[shift + j]]);
        }
    }

    return -1; 
}


int main() {
    setlocale(0, "");
    SetConsoleCP(1251);
    bool started = true; // Для while
    int choice;
    char choiceFileName[50]; // char под название файла.
    char text[N]; // Сам текс.
    char subText[N]; // Подтекст для поиска.
    int* TextLen = &len; // Длина строки text через указатель (в начале len = 0)
    

    while (started) {
        started = true;
        cout << "Нужно выбрать откуда вводить текст: \n1 | С клавиатуры\n2 | Из файла\n3 | Выйти из программы" << endl;
        cin >> choice;

        switch (choice) {
            case 1: cout << "Введите текст: " << endl; InputFromKeyboard(text, N); break;
            case 2: cout << "Введите название файла: " << endl; cin >> choiceFileName;
            InputFromFile(N, text, choiceFileName); break;
            case 3: started = false; break;
        }
        TextLenght(text, TextLen);

        bool started2 = true;
        while (started && started2) {
            cout << "Теперь необходимо выбрать взаимодействие с текстом: \n1 | Редактировать текст (убрать лишние пробелы, знаки препинания, испр. регистра)\n2 | Вывести на экран слова последовательности, в которых не встречаются цифры\n3 | Вывести кол-во символов в каждом слове\n4 | Вывыести начальный индекс подстроки линейным поиском\n5 | Вывод начальный индекс подстроки алгоритмом Бойера-Мура\n6 | Выход из программы." << endl;
            cin >> choice;
            int result;

            switch (choice) {
                case 1: FirstEdit(text, TextLen); cout << "Отредактированный текст: " << text; break;
                case 2: PrintWordsWithoutDigits(text, TextLen); break;
                case 3: PrintWordsLenght(text, TextLen); break;
                case 4: cout << "Введите текс для поиска подстроки в тексте (должно заканчиваться на точку)" << endl; InputFromKeyboard(subText, N);
                    result = LinearSubText(text, subText, TextLen); cout << (result >= 0 ? "Индекс найден: " + to_string(result) : "Индекс не найден"); break;
                case 5: cout << "Введите текс для поиска подстроки в тексте (должно заканчиваться на точку)" << endl; InputFromKeyboard(subText, N);
                    result = BoyerMooreSearch(text, subText, TextLen); cout << (result >= 0 ? "Индекс найден: " + to_string(result) : "Индекс не найден"); break;
                case 6: started2 = false; break;
            }
            cout << endl;
        }
            
    }

    return 0;
}