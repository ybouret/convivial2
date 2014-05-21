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
        const string V     = I->value();
        if(V.size()>0)
        {
            const double value = strconv::to<double>( I->value(), "basis parameter" );
            basis.param.push_back(value);
        }
        else
            basis.param.push_back(0);
    }
    
    mode->print();
    
    SetDirty();
    return mode.yield();
}


void ModesToUI()
{
    int num_modes = 0;
    int num_dof   = 0;
    ModeBrowser->clear();
    for( Mode *mode = App->modes.head; mode; mode=mode->next )
    {
        ++num_modes;
        num_dof += mode->DOF;
        mode->make_info();
        ModeBrowser->add(mode->info.c_str());
    }
    
    NumModes->value(num_modes);
    NumDOF->value(num_dof);
    
    
    if(num_modes<=0)
    {
        Symetry->activate();
        ModeParam->deactivate();
        ModeBrowser->deactivate();
    }
    
}

void OnSelectedMode()
{
    const int browser_line = ModeBrowser->value();
    std::cerr << "Selected Mode #" << browser_line << std::endl;
    const Mode *mode = App->modes.fetch(browser_line-1);
    
    //Set the ModeSym
    const string &ch  = mode->character;
    const int     isym = Symetry->value()+1;
    const Schoenflies &schoenflies = App->data.schoenflies(isym);
    for(size_t i=1;i<=schoenflies.characters.size();++i)
    {
        if(ch == schoenflies.characters[i])
        {
            ModeSym->value(i-1);
        }
    }
    
    //Set the Tabs
    const string     &base_name = mode->basis.name;
    uiBasis::Pointer *ppB       = uiBasisDB->search(base_name);
    if(!ppB)
        throw exception("Can't find UI Tab for '%s'", base_name.c_str());
    
    uiBasis   &ui_basis = **ppB;
    Fl_Widget *w        = ui_basis.group;
    BasisTabs->value(w);
    
    //propagate values
    BasisSize->value( mode->basis.size );
    
    for(size_t i=1;i<=mode->basis.param.size();++i)
    {
        const string s = vformat("%.6g",mode->basis.param[i]);
        ui_basis.input[i]->value( s.c_str() );
    }
    
}


void UpdateMode()
{
    const int n    = ModeBrowser->value();
    Mode     *old_mode = App->modes.fetch(n-1);
    Mode     *new_mode = ReadModeFromUI();
    App->modes.replace(old_mode, new_mode);
    delete old_mode;
    ModesToUI();
    ModeBrowser->select(n);
    OnSelectedMode();
    
}

#include "yocto/code/utils.hpp"

void DeleteMode()
{
    if( !Confirm("Delete This Mode?"))
        return;
    
    const int browser_line = ModeBrowser->value();
    Mode *mode = App->modes.fetch(browser_line-1);
    delete App->modes.unlink(mode);
    ModesToUI();
    if(App->modes.size)
    {
        ModeBrowser->value(max_of<int>(browser_line-1,1));
        OnSelectedMode();
    }
}


//==============================================================================
// create and append a new mode
//==============================================================================
void CreateNewMode()
{
    std::cerr << "Creating New Mode" << std::endl;
    
    
    auto_ptr<Mode> mode( ReadModeFromUI() );
    
    //create an entry for the browser
    App->modes.push_back( ReadModeFromUI() );
    ModesToUI();
    // select the last one
    ModeBrowser->select(App->modes.size);
    
    //success
    Symetry->deactivate();
    ModeParam->activate();
    ModeBrowser->activate();
}
