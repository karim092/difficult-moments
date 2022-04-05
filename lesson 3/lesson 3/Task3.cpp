#include <iostream>

#include <iomanip>
#include "Task3.h"
#include "Task2.h"

using namespace std;

void task_3()
{
    Matrix m(4);
    m.fillRandom();

    for (const double& i : m)
    {
        cout << setw(5) << i;
    }

    cout << endl;
}