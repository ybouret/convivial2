#include "conviv.hpp"
#include "yocto/exception.hpp"
#include <iostream>

int main( int argc, char *argv[] )
{
	try 
    {
        ConvivData DATA(argv[0]);
        Conviv CONVIV(DATA);
        
        if( argc > 1 )
        {
            CONVIV.importCONVIV(argv[1]);
            CONVIV.save("output.js");
            CONVIV.load("output.js");
        }
        
        return 0;
    }
    catch( const exception &e )
    {
        std::cerr << e.what() << std::endl;
        std::cerr << e.when() << std::endl;
    }
    catch(...)
    {
        std::cerr << "unhandled exception" << std::endl;
    }
    return -1;
} 
