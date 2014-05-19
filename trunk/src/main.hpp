#ifndef MAIN_INCLUDED
#define MAIN_INCLUDED  1

#include "conviv.hpp"
#include "ui/messages.hpp"
#include "ui/basis.hpp"
#include <string>


//cb-gen.cpp
void ConfirmQuit() throw();
void SetTheme() throw();
void SetDirty() throw();
void SetClean() throw();

//cb-sym.cpp
void SetSymetry() throw();
void SetUsedSym() throw();

//cb-pot.cpp
void SetPotentialFormat();
void SetPotentialFile();
void BrowsePotentialFile();

//cb-obs.cpp
void SetObsFile();
void BrowseObsFile();

//cb-mode.cpp
void CreateNewMode();

//MFCI.cpp
void BrowseExeConviv();
void BrowseConvivInput();
void BrowseConvivOutput();

std::string GetConvroot();

void SetTolerance();
void SetBasisTrunc();
void MoveFiles();
void RemoveFiles(int i);

bool IsOut();
bool IsIn();
std::string GetRestartNameOut();
std::string GetRestartNameIn();
std::string GetPotPath();
std::string GetObsPath();

void runPython(char *fn);

void ExeSimpleRun();
void ExeSCFRun();
void Pairing();
void Hierarchy();

void Clean();

extern Conviv *App;


#endif
