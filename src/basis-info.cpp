#include "basis-info.hpp"

BasisInfo:: ~BasisInfo() throw()
{
}

BasisInfo:: BasisInfo( const string &id ) :
name(id),
param(4,as_capacity)
{
}

BasisInfo:: BasisInfo( const BasisInfo &other ) :
name( other.name ),
param( other.param )
{
}

const string & BasisInfo:: key() const throw() { return name; }