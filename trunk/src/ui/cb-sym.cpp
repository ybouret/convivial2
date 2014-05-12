#include "convivial.h"

//==============================================================================
//
// Symmetry
//
//==============================================================================

void SetUsedSym() throw()
{
    assert(App);
    App->sym = ( 0 == UsedSym->value() ) ? false : true;
    if( App->sym )
        UsedSym->label("in use");
    else
        UsedSym->label("no use");
    SetDirty();
    
}

void SetSymetry() throw()
{
    
    std::cerr << "Symetry Value=" << Symetry->value()<< std::endl;
    const size_t       idx = Symetry->value() + 1;
    const Schoenflies &s   = App->data.schoenflies(idx);
    SetDirty();
    
    App->sym_name = s.ucsym;
    SymInfo->label( s.info.c_str() );
    ModeSymInfo->label( s.symbol.c_str() );
    
    ModeSym->clear();
    for( size_t i=1; i <= s.characters.size(); ++i )
    {
        ModeSym->add( s.characters[i].c_str() );
    }
    ModeSym->value(0);
}
