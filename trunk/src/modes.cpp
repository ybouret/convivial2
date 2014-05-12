#include "./modes.hpp"

Modes:: Modes() throw() : vector<ModePtr>() {}

Modes:: ~Modes() throw()
{
    vector<ModePtr> &self = *this;
    for( size_t n=size();n>0;--n)
    {
        if( self[n] )
        {
            delete self[n];
            self[n] = 0;
        }
    }
}