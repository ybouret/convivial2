#ifndef __CONVIV_DATA_INCLUDED
#define __CONVIV_DATA_INCLUDED 1

#include "./schoenflies.hpp"
#include "./basis-info.hpp"
#include "yocto/json/value.hpp"

class ConvivData 
{
public:
    explicit ConvivData( const char *application_path );
    virtual ~ConvivData() throw();
    
    Schoenflies::DataBase schoenflies;
    vector<char>          htmlManual;
    BasisInfo::DataBase   basisDB;
    //mutable JSON::Parser  js;

    void displaySchoenflies( std::ostream &os ) const;

private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(ConvivData);
};

#endif
