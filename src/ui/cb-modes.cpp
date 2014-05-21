#include "convivial.h"


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
    const int ich = ModeSym->value()+1;
    std::cerr << "ich=" << ich << std::endl;
    const string  ch = schoenflies.characters[ich];
    std::cerr << "Char=[[" << ch << "]]" << std::endl;
    
    //get the ui base
    Fl_Widget   *pTab    = BasisTabs->value();
    const string basisID = pTab->label();
    std::cerr << "Basis=[[" << basisID << "]]" << std::endl;
    const uiBasis::Pointer *ppBasisUI = db.search(basisID);
    if(!ppBasisUI)
        throw exception("Unexpected unknow basis '%s'", basisID.c_str());
    
    // ready to create the mode and fill data
    
    Mode mode( ch, schoenflies, (*ppBasisUI)->name, App->data.basisDB );
    
    
    return 0;
}

void CreateNewMode()
{
    std::cerr << "Creating New Mode" << std::endl;
    
    
    ReadModeFromUI();
    
    //create an entry for the browser
    
    //success
    Symetry->deactivate();
    ModeParam->activate();
    ModeBrowser->activate();
}
