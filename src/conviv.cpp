#include "./conviv.hpp"

Conviv:: ~Conviv() throw()
{
}

Conviv:: Conviv( const ConvivData &d ) :
dirty(false),
sym_name(),
sym(false),
pot_fmt(),
fn(),
fnrot(),
title(),
data(d)
{
    reset();
}

void Conviv:: reset() throw()
{
    dirty = false;
    sym_name.clear();
    sym = false;
    pot_fmt = "EXTENDED";
    fn.clear();
    fnrot   = "norot";
    title.clear();
    modes.clear();
}


