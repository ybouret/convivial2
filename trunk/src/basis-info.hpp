#ifndef BASIS_INFO_INCLUDED
#define BASIS_INFO_INCLUDED


#include "yocto/string.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/sequence/vector.hpp"

using namespace yocto;

//! info about a basis
class BasisInfo
{
public:
    const string   name;  //!< the name "HO",...
    vector<string> param; //!< name of the parameters
    
    explicit BasisInfo( const string &id );
    virtual ~BasisInfo() throw();
    
    BasisInfo( const BasisInfo &);
    const string & key() const throw();
    
    typedef set<string,BasisInfo> DataBase;
    
private:
    YOCTO_DISABLE_ASSIGN(BasisInfo);
};



#endif
