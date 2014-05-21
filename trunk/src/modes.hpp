#ifndef MODES_INCLUDED
#define MODES_INCLUDED 1

#include "./mode.hpp"

class Modes :public core::list_of_cpp<Mode>
{
public:
    explicit Modes() throw();
    virtual ~Modes() throw();
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(Modes);
};


#endif
