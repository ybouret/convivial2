#ifndef MODES_INCLUDED
#define MODES_INCLUDED 1

#include "./mode.hpp"

typedef shared_ptr<Mode> ModePtr;
class Modes : public vector<ModePtr>
{
public:
    explicit Modes() throw();
    virtual ~Modes() throw();
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(Modes);
};


#endif
