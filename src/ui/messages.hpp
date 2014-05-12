#ifndef MESSAGES_INCLUDED
#define MESSAGES_INCLUDED

#include "yocto/exception.hpp"

using namespace yocto;

void PrepareMessage( const char *title );
bool Confirm( const char *text );
void DisplayException( const exception &e );

#endif
