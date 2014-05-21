#include "convivial.h"
#include "yocto/ptr/auto.hpp"
#include "yocto/string/conv.hpp"

// create a new node from UI
Mode *ReadModeFromUI()
{
    
    std::cerr << "ReadModeFromUI" << std::endl;
    const uiBasis::DataBase &db = *uiBasisDB;
    
    //__________________________________________________________________________
    //
    // get the current symetry group
    //__________________________________________________________________________
    const int isym = Symetry->value()+1;
    const Schoenflies &schoenflies = App->data.schoenflies(isym);
    std::cerr << "Using Group [[" << schoenflies.symbol << "]]" << std::endl;
    
    //get the character
    const int     ich = ModeSym->value()+1;
    const string  ch  = schoenflies.characters[ich];
    
    //get the ui base
    Fl_Widget   *pTab    = BasisTabs->value();
    const string basisID = pTab->label();
    std::cerr << "Basis=[[" << basisID << "]]" << std::endl;
    const uiBasis::Pointer *ppBasisUI = db.search(basisID);
    if(!ppBasisUI)
        throw exception("Unexpected unknow basis '%s'", basisID.c_str());
    
    const uiBasis &ui_basis = **ppBasisUI;
    // ready to create the mode and fill data
    auto_ptr<Mode> mode(new Mode( ch, schoenflies, ui_basis.name, App->data.basisDB ));
    
    Basis &basis = mode->basis;
    basis.size   = size_t(BasisSize->value());
    basis.param.free();
    const array<Fl_Input*> &input = ui_basis.input;
    for(size_t i=1;i<=input.size();++i)
    {
        Fl_Input    *I     = input[i];
        const double value = strconv::to<double>( I->value(), "basis parameter" );
        basis.param.push_back(value);
    }
    
    mode->print();
    
    
    return mode.yield();
}


void ModesToUI()
{
    
}

//==============================================================================
// create and append a new mode
//==============================================================================
void CreateNewMode()
{
    std::cerr << "Creating New Mode" << std::endl;
    
    
    auto_ptr<Mode> mode( ReadModeFromUI() );
    
    //create an entry for the browser
    
    //success
    Symetry->deactivate();
    ModeParam->activate();
    ModeBrowser->activate();
}
