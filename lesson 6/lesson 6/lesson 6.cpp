// lesson 6.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <mutex>
#include <algorithm>
#include <random>
#include "Timer.cpp"

// Task 1

static std::mutex mtx_cout;

class pcout
{
private:
    std::lock_guard<std::mutex> lg;
public:
    pcout() : lg(std::lock_guard<std::mutex>(mtx_cout))
    {
    }

    template<typename T>
    pcout& operator<<(const T& data)
    {
        std::cout << data;
        return *this;
    }

    pcout& operator<<(std::ostream& (*fp)(std::ostream&))                // т.к. std::endl является функцией,
    {                                                                    // то для неё делаем перегрузку operator<<
        std::cout << fp;                                                 // которая принимает указатель на функцию
        return *this;                                                    // типа std::ostream& и возвращает наш
    }                                                                    // защищённый поток вывода
};

void doSomething(int number)
{
    pcout() << "start thread " << number << std::endl;
    pcout() << "stop thread " << number << std::endl;
}

void task_1()
{
    std::thread th1(doSomething, 1);
    std::thread th2(doSomething, 2);
    std::thread th3(doSomething, 3);
    th1.join();
    th2.join();
    th3.join();
}

// Task2 

bool isPrime(size_t num)
{
    size_t limit = num / 2;

    if (num > 2)
    {
        for (size_t i = 2; i <= limit; ++i)
        {
            if (num % i == 0)
            {
                return false;
            }
        }
    }

    return true;
}

void task_2()
{

    size_t maxValue = 500000;
    size_t counThreads = 30;
    std::vector<size_t> answers;
    std::mutex m;

    Timer timer("With threads");

    for (size_t i = 0; i < maxValue;)
    {
        std::vector<std::thread> v;

        for (size_t j = i + counThreads; i <= j; ++i)
        {
            v.push_back(std::thread([=, &m, &answers]()
                {
                    if (isPrime(i))
                    {
                        std::lock_guard lg(m);
                        answers.push_back(i);
                    }
                }));
        }

        for (auto& t : v)
        {
            t.join();
        }
    }

    //    for (const auto &a : answers)
    //    {
    //        std::cout << "Число " << a << " простое" << std::endl;
    //    }
    timer.print();

    answers.clear();
    answers.shrink_to_fit();

    timer.start("Without threads");

    for (size_t i = 0; i < maxValue; ++i)
    {
        if (isPrime(i))
        {
            answers.push_back(i);
        }
    }

    //    for (const auto &a : answers)
    //    {
    //        std::cout << "Число " << a << " простое" << std::endl;
    //    }
    timer.print();
}

// Task 3

std::mutex m;

void generate_things(std::vector<int>& v)
{
    std::random_device rd;
    std::mt19937 mersenne(rd());
    std::uniform_int_distribution<int> urd(0, 1000);

    std::lock_guard lg(m);

    std::generate(v.begin(), v.end(), [&]()
        {
            return urd(mersenne);
        });
}

void pop_thing(std::vector<int>& v)
{
    std::lock_guard lg(m);

    std::cout << *std::max_element(v.begin(), v.end()) << std::endl;
}

void task_3()
{
    std::vector<int> v(100);
    size_t count = 100;

    while (count--)
    {
        std::thread owner([&]()
            {
                generate_things(v);
            });
        std::thread thief([&]()
            {
                pop_thing(v);
            });

        owner.join();
        thief.join();
    }
}

int main()
{
    task_1();
    task_2();
    task_3();

    return 0;
}