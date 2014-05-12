#ifndef CONVIV_BASIS_INCLUDED
#define CONVIV_BASIS_INCLUDED 1

#include "./basis-info.hpp"

class Basis
{
public:
    explicit Basis( const string &basis_id, const BasisInfo::DataBase &basis_db);
    virtual ~Basis() throw();
    Basis( const Basis & );
    
    const string    name;
    size_t          size;
    vector<double>  param;
    
    
private:
    YOCTO_DISABLE_ASSIGN(Basis);
    
};

#endif
