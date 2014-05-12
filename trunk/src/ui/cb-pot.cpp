#include "convivial.h"

//==============================================================================
//
// Potential
//
//==============================================================================
void SetPotentialFormat()
{
    assert(App);
    const int idx = PotentialFormat->value();
    
    switch( idx )
    {
        case 0:
            App->pot_fmt = "EXTENDED";
            break;
            
        case 1:
            App->pot_fmt = "COMPACT";
            break;
            
        default:
            break;
    }
    
    SetDirty();
}


void SetPotentialFile()
{
    App->fn = PotentialFile->value();
    SetDirty();
}

#include <FL/Fl_File_Chooser.H>

void BrowsePotentialFile()
{
    assert(App);
    const char *current_fn = App->fn.c_str();
    if(App->fn.size()<=0)
        current_fn = ".";
    
    const char *fn = fl_file_chooser("Choose Potential File", 0, current_fn,1);
    if( fn )
    {
        App->fn = fn;
        PotentialFile->value( App->fn.c_str() );
        SetDirty();
    }
    
}
