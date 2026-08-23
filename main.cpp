#include "Engine.h"
#include <iostream>


int main(int argc, char** argv)
{
    try
    {
        Engine engine;
        engine.Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal: " << e.what() << '\n';
        
        return 1;
    }

    return 0;
}