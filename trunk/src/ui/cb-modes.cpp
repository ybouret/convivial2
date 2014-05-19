#include "convivial.h"

void CreateNewMode()
{
    std::cerr << "Creating New Mode" << std::endl;
    
    const uiBasis::DataBase &db = *uiBasisDB;
    
    //create and editable mode from current settings
    
    // get the current symetry group
    const int isym = Symetry->value()+1;
    const Schoenflies &schoenflies = App->data.schoenflies(isym);
    std::cerr << "Using Group [[" << schoenflies.symbol << "]]" << std::endl;
    
    // get the character
    const int ich = ModeSym->value()+1;
    std::cerr << "ich=" << ich << std::endl;
    const string  ch = schoenflies.characters[ich];
    std::cerr << "Char=[[" << ch << "]]" << std::endl;
    
    //success
    Symetry->deactivate();
    ModeParam->activate();
    ModeBrowser->activate();
}
