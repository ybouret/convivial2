#ifndef MODE_INCLUDED
#define MODE_INCLUDED 1


#include "./basis.hpp"
#include "./schoenflies.hpp"
#include "yocto/ptr/shared.hpp"

class Mode
{
public:
    explicit Mode(const string              &ch,
                  const Schoenflies         &sym,
                  const string              &basis_id ,
                  const BasisInfo::DataBase &basis_db );
    virtual ~Mode() throw();
    
    const string  character; //!< associated to symmetry
    const size_t  DOF;       //!< degrees of freedom : A|B=1, E=2, F=3
    Basis         basis;     //!< basis for this mode
    string        info;      //!< for browser
    
    Mode *next;
    Mode *prev;
    
    void print() const;
    void make_info();
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(Mode);
};

#endif
