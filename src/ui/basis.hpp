#ifndef UI_BASIS_INCLUDED
#define UI_BASIS_INCLUDED 1

#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>

#include "../basis-info.hpp"
#include "yocto/ptr/intr.hpp"
#include "yocto/associative/set.hpp"
#include "yocto/counted.hpp"

using namespace yocto;

class uiBasis : public counted
{
public:
    const string       name;
    Fl_Group          *group;
    vector<Fl_Input *> input;

    explicit uiBasis(Fl_Group        &tabs,
                     const BasisInfo &bi);
    
    const string & key() const throw();
    
    virtual ~uiBasis() throw();
    
    typedef intr_ptr<string,uiBasis>      Pointer;
    typedef set<string,uiBasis::Pointer>  DataBase;
    
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(uiBasis);
};

extern uiBasis::DataBase *uiBasisDB;
void   SetCurrentMode(); //!< the callback

#endif
