#include "basis.hpp"
#include "yocto/exception.hpp"

Basis:: ~Basis() throw()
{
    
}

Basis:: Basis( const string &basis_id, const BasisInfo::DataBase &basis_db ) :
name(basis_id),
size(0),
param(4,as_capacity)
{
    if( !basis_db.search(name) )
        throw exception("Invalid Basis.name='%s'", name.c_str());
}
