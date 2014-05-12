#include "./conviv.hpp"
#include "yocto/exception.hpp"



void Conviv:: ValidatePotFmt() const 
{
    if( pot_fmt == "COMPACT" )
        return;
    
    if( pot_fmt == "EXTENDED" )
        return;
    
    throw exception("invalid pot_fmt='%s'", pot_fmt.c_str());
}


void Conviv:: ValidateSymName() const
{
    if( !data.schoenflies.search(sym_name) )
        throw exception("invalid Schoenflies '%s'", sym_name.c_str() );
}

void Conviv:: ValidateFn() const 
{
    if( fn.size() >= fn_max )
        throw exception("fn size too large");
}  

void Conviv:: ValidateFnRot() const
{
    if( fnrot.size() >= fnrot_max )
        throw exception("fnrot size too large");
}

void Conviv:: ValidateTitle() const
{
    if( title.size() >= title_max )
        throw exception("title size too large");
}

void Conviv:: ValidateAll() const
{
    ValidateSymName();
    ValidatePotFmt();
    ValidateFn();
    ValidateFnRot();
    ValidateTitle();
}
