#include "convivial.h"

//==============================================================================
//
// Quit
//
//==============================================================================

void ConfirmQuit() throw()
{
    try
    {
        if( Confirm("Are you really sure you want to quit the program ?"))
            AppWindow->hide();
	    
	//Suppression fichiers conviv.txt, tol.txt et trunc.txt à la fermeture
	std::string command = "rm $CONV_ROOT/src/ui/python/work/conviv.e $CONV_ROOT/src/ui/python/work/tol.txt $CONV_ROOT/src/ui/python/work/trunc.txt";
	system(command.c_str()); 
       
    }
    catch( const exception &e )
    {
        DisplayException(e);
    }
    catch(...)
    {
        const exception e("Unhandled error in ConfirmQuit");
        DisplayException(e);
    }
}

//==============================================================================
//
// Generic
//
//==============================================================================

void SetTheme() throw()
{
    if( ThemeNone->value() )
        Fl::scheme("none");
    
    if( ThemeGtk->value() )
        Fl::scheme("gtk+");
    
    if( ThemePlastic->value() )
        Fl::scheme("plastic");
    
}

void SetDirty() throw()
{
    assert(App);
    App->dirty = true;
    Modified->label("Modified");
}

void SetClean() throw()
{
    assert(App);
    App->dirty = false;
    Modified->label("");
}
