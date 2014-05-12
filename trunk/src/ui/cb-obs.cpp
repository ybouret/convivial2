#include "convivial.h"

//==============================================================================
//
// Observables
//
//==============================================================================
void SetObsFile()
{
    App->fnrot = ObsFile->value();
    SetDirty();
}

#include <FL/Fl_File_Chooser.H>

void BrowseObsFile()
{
    assert(App);
    const char *current_fnrot = App->fnrot.c_str();
    if(App->fnrot.size()<=0)
        current_fnrot = ".";
    
    const char *fnrot = fl_file_chooser("Choose Observables File", 0, current_fnrot,1);
    if( fnrot )
    {
        App->fnrot = fnrot;
        ObsFile->value( App->fnrot.c_str() );
        SetDirty();
    }
}
