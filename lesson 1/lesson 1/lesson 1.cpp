// lesson 1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

#include <string>
#include <optional>
#include <tuple>
#include <vector>
#include <fstream>
#include <sstream>
#include <any>

using namespace std;

struct Person
{
    string firstname;
    string lastname;
    optional<string> patronymic = nullopt;
};

ostream& operator<<(ostream& out, const Person& p)
{
    out << p.firstname << ' ' << p.lastname << ' ' << p.patronymic.value_or("");

    return out;
}

bool operator<(const Person& p1, const Person& p2)
{
    string patr1 = p1.patronymic.value_or("");
    string patr2 = p2.patronymic.value_or("");

    return tie(p1.firstname, p1.lastname, patr1)
        < tie(p2.firstname, p2.lastname, patr2);
}

bool operator==(const Person& p1, const Person& p2)
{
    string patr1 = p1.patronymic.value_or("");
    string patr2 = p2.patronymic.value_or("");

    return tie(p1.firstname, p1.lastname, patr1)
        == tie(p2.firstname, p2.lastname, patr2);
}

struct PhoneNumber
{
    int country_code;
    int city_code;
    string number;
    optional<int> additional_number = nullopt;
};

ostream& operator<<(ostream& out, const PhoneNumber& p)
{
    out << '+' << p.country_code << '(' << p.city_code << ')' << p.number;

    if (p.additional_number.has_value())
    {
        out << ' ' << p.additional_number.value();
    }

    return out;
}

using perPhone = vector<pair<Person, PhoneNumber>>;

class PhoneBook
{
private:

    perPhone m_data;

public:

    PhoneBook(ifstream& file)
    {
      
        file.open("C:\\Users\\user\\source\\repos\\lesson 1\\PhoneBook.txt", ios::in);
        if (!file)
        {
            cout << "Не удаётся открыть файл!" << endl;
            exit(-1);
        }

        while (file)
        {
            string s;

            getline(file, s, '\n');

            stringstream str;
            str << s;

            pair<Person, PhoneNumber> entry;

            for (int i = 0; str; ++i)
            {
                switch (i)
                {
                case 0:
                    str >> entry.first.firstname;
                    break;
                case 1:
                    str >> entry.first.lastname;
                    break;
                case 2:
                    str >> s;
                    entry.first.patronymic = s;
                    break;
                case 3:
                    str >> entry.second.country_code;
                    break;
                case 4:
                    str >> entry.second.city_code;
                    break;
                case 5:
                    str >> entry.second.number;
                    break;
                case 6:
                    int num;
                    str >> num;
                    entry.second.additional_number = num;
                    break;
                }
            }

            m_data.push_back(entry);
        }
    }

    friend ostream& operator<<(ostream& out, const PhoneBook& pb);
};

ostream& operator<<(ostream& out, const PhoneBook& pb)
{
    for (const auto& [first, second] : pb.m_data)
    {
        out << first << ' ' << second << endl;
    }

    return out;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    ifstream file("PhoneBook.txt");
    PhoneBook book(file);
    cout << book;

    return 0;
}



