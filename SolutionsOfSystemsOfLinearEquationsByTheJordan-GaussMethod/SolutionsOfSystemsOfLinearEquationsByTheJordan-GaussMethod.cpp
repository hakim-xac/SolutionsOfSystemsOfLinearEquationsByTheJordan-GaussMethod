#include <iostream>
#include "Solution.h"


int main(int argc, char** argv)
{
    std::setlocale(LC_ALL, "");

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    std::string filename{ "./input.txt" };

    if (argc == 2) {
        filename = argv[1];
    }


    KHAS::Solution solution{ KHAS::NumberOfUnknownVariables {.size = 6 }, filename };
    solution.start();



    system("pause");
}