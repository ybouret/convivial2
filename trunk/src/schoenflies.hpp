#ifndef __SCHOENFLIES_INCLUDED
#define __SCHOENFLIES_INCLUDED 1

#include "yocto/string.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/hashing/sha1.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;

typedef hashing::sha1                      HashingFunction;
typedef key_hasher<string,HashingFunction> StringHasher;

////////////////////////////////////////////////////////////////////////////////
//
//! Schoenflies symbol
//
////////////////////////////////////////////////////////////////////////////////
class Schoenflies
{
public:
    Schoenflies( const string &id, const string &descr, bool isAbelian );
    ~Schoenflies() throw();
    const string         symbol;     //!< unique identifier
    const string         ucsym;      //!< upper case symbol
    const string         info;       //!< short description
    const vector<string> characters; //!< possible characters
    const bool           abelian;    //!< true if is Abelian
    
    Schoenflies( const Schoenflies &other );
    const string &key() const throw(); //!< upper case symbol

    typedef set<string,Schoenflies,StringHasher> DataBase;
    
    void display( std::ostream &os ) const;
    
private:
    YOCTO_DISABLE_ASSIGN(Schoenflies);
};

#endif
