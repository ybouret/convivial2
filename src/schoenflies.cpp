#include "./schoenflies.hpp"
#include <cctype>

static inline string __make_uc( const string &s )
{
    string ans(s.size(),as_capacity);
    for( size_t i=0; i < s.size(); ++i)
        ans.append( toupper( s[i] ) );
    return ans;
}

Schoenflies:: Schoenflies( const string &id, const string &descr, bool isAbelian ):
symbol(id),
ucsym( __make_uc(symbol) ),
info(descr),
characters(),
abelian(isAbelian)
{
    
}

Schoenflies:: ~Schoenflies() throw()
{
    
}

const string & Schoenflies:: key() const throw() { return ucsym; }

Schoenflies:: Schoenflies( const Schoenflies &other ) :
symbol( other.symbol ),
ucsym(  other.ucsym ),
info( other.info ),
characters( other.characters ),
abelian( other.abelian )
{
}

void Schoenflies:: display( std::ostream &os ) const
{
    os << "<" << symbol << "> " << ( abelian ? "Abelian" : "NOT Abelian" ) << "'" << info << "'" << std::endl;
    os << "|_" << characters << std::endl;
}