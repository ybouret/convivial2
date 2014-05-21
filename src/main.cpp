#include "convivial.h"
#include "yocto/exception.hpp"
#include <iostream>
#include "yocto/ptr/auto.hpp"

Conviv             *App       = 0;
uiBasis::DataBase  *uiBasisDB = 0;


int main( int argc, char *argv[] )
{
	try
    {
        ////////////////////////////////////////////////////////////////////////
        //
        // initialize FLTK
        //
        ////////////////////////////////////////////////////////////////////////
        uiMakeWindows();
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // initialize Conviv
        //
        ////////////////////////////////////////////////////////////////////////
        ConvivData        DATA(argv[0]);
        Conviv            CONVIV(DATA);
        uiBasis::DataBase UI_BasisDB;
        
        App       =  &CONVIV;
        uiBasisDB =  &UI_BasisDB;
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Fill The Interface with default values
        //
        ////////////////////////////////////////////////////////////////////////
        
        //======================================================================
        // set the help
        //======================================================================
        ConvivHelp->value( DATA.htmlManual() );
        
        //======================================================================
        // fill the symetry Choice
        //======================================================================
        for( size_t i=1; i <= DATA.schoenflies.size(); ++i )
        {
            const Schoenflies &s = DATA.schoenflies(i);
            Symetry->add( s.symbol.c_str() );
        }
        
        //======================================================================
        // fill the basis tabs
        //======================================================================
        for( size_t i=1; i <= DATA.basisDB.size(); ++i )
        {
            const BasisInfo &bi = DATA.basisDB(i);
            const uiBasis::Pointer p( new uiBasis( *BasisTabs, bi) );
            if( ! uiBasisDB->insert(p) )
                throw exception("Unexpected multiple uiBasis");
        }
        
        
        SetTheme();
        Symetry->value(0);
        SetSymetry();
        UsedSym->value(1);
        SetUsedSym();
        SetPotentialFormat();
        ObsFile->value( "none" );
        SetObsFile();
        SetClean();
        
        
        if( argc > 1 )
        {
            CONVIV.importCONVIV(argv[1]);
            CONVIV.save("output.js");
            CONVIV.load("output.js");
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        //  Main Loop
        //
        ////////////////////////////////////////////////////////////////////////
        AppWindow->show();
        return Fl::run();
    }
    catch( const exception &e )
    {
        DisplayException(e);
    }
    catch(...)
    {
        const exception e("Unhandled Error");
        DisplayException(e);
    }
    return -1;
}

