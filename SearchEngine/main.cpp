#include "tests.cpp"
#include "entryinterface.h"
#include <fstream>
#include <iostream>
#include <chrono>

using namespace std::chrono;

using namespace std;

int main(int argc, char *argv[])
{
    string test = "-t";
    if (argc == 2 && argv[1]==test){

        int result = Catch::Session().run();
        return (result < 0xff ? result : 0xff);
    }  else {
        QueryProcessor query;
        query.menu();
        return 0;
    }
}
