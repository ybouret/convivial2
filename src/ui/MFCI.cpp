#include "convivial.h"
#include <string>
#include <cstdlib>
#include <Python.h>
#include <iostream>
#include <fstream>

using namespace std;

//==============================================================================
//
// MFCI
//
//==============================================================================


#include <FL/Fl_File_Chooser.H>


void BrowseExeConviv()
{
    assert(App);
    const char *current_c = App->c.c_str();
    if(App->c.size()<=0)
        current_c = ".";
    
    
    const char *c = fl_file_chooser("Choose Conviv Executable", 0, current_c, 1);
    if( c )
    {
        std::cerr << "Choosen [" << c << "]" << std::endl;
        App->c = c;
        ExecutableConviv->value( App->c.c_str() );
        SetDirty();
    }
    else
        return;
    
	//Création lien symbolique de conviv.e dans work/
	std::string conviv =  App->c.c_str();
    
	//Adresse à partir de laquelle l'interface est exécutée
	char temp[PATH_MAX];
	getcwd(temp, PATH_MAX);
	std::string pwd = temp;
    std::cerr << "PWD=" << pwd << std::endl;
    
	std::string command = "ln -s " + pwd + "/" + conviv + " " + GetConvroot();
    std::cerr << "Command='" << command << "'" << std::endl;
	system(command.c_str());
    
	SetDirty();
}



void BrowseConvivInput()
{
    assert(App);
    const char *current_m = App->m.c_str();
    if(App->m.size()<=0)
        current_m = ".";
    
    const char *m = fl_file_chooser("Choose Conviv Input", 0, current_m, 1);
    if( m )
    {
        App->m = m;
        ConvivInput->value( App->m.c_str() );
        SetDirty();
    }
}



void BrowseConvivOutput()
{
    assert(App);
    const char *current_o = App->o.c_str();
    if(App->o.size()<=0)
        current_o = ".";
    
    const char *o = fl_dir_chooser("Choose Conviv Output directory", current_o, 0);
    if( o )
    {
        App->o = o;
        ConvivOutput->value( App->o.c_str() );
        SetDirty();
    }
}



//Chemin absolu du dossier work
std::string GetConvroot()
{
	char* l;
	l = getenv("CONV_ROOT");
	std::string li   = l;
	std::string link = li + "/src/ui/python/work/";
    
	return link;
}



//Enregistrer la valeur de tolérance dans le fichier tol.txt dans le dossier work
void SetTolerance()
{
	std::string link = GetConvroot() + "tol.txt";
	ofstream fichier(link.c_str());
	
    if(fichier)
    {
		fichier << SCFtol->value() << endl;
        fichier.close();
    }
    else
        cerr << "Erreur à l'ouverture" << endl;
    
	SetDirty();
}



//Enregistrer la valeur de basis trunc dans le fichier trunc.txt dans le dossier work
void SetBasisTrunc()
{
	std::string link = GetConvroot() + "trunc.txt";
	ofstream fichier(link.c_str());
	
    if(fichier)
    {
		fichier << BasisTrunc->value() << endl;
        fichier.close();
    }
    else
        cerr << "Erreur à l'ouverture" << endl;
    
	SetDirty();
}



//Obtenir le nom du fichier des potentiels
std::string GetPotPath()
{
	std::string link = GetConvroot() + "modele.inp";
    
	ifstream fichier(link.c_str());
    
	std::string contenu;
    for (int i=1 ; i<=2 ; i++) {
		getline(fichier, contenu);
	}
    
	std::string pot = contenu.c_str();
    
	while (pot[0]==' ') {
		pot.erase(0,1);
	}
    
	while (pot[0]!=' ') {
		pot.erase(0,1);
	}
    
	while (pot[0]==' ') {
		pot.erase(0,1);
	}
    
	return pot;
}



//Obtenir le nom du fichier des observables
std::string GetObsPath()
{
	std::string link = GetConvroot() + "modele.inp";
    
	ifstream fichier(link.c_str());
    
	std::string contenu;
    for (int i=1 ; i<=3 ; i++) {
		getline(fichier, contenu);
	}
    
	std::string obs = contenu.c_str();
    
	while (obs[0]==' ') {
		obs.erase(0,1);
	}
    
	return obs;
}



//Déplacer input, fichier des potentiels, observables et si nécessaire de restart dans le dossier work
void MoveFiles()
{
    
    //Création lien symbolique de conviv.e dans work/
	std::string conviv =  App->c.c_str();
    
	//Adresse à partir de laquelle l'interface est exécutée
	char temp[PATH_MAX];
	getcwd(temp, PATH_MAX);
	std::string pwd = temp;
    std::cerr << "PWD=" << pwd << std::endl;
    
	std::string command = "ln -s " + pwd + "/" + conviv + " " + GetConvroot();
    std::cerr << "Command='" << command << "'" << std::endl;
	system(command.c_str());

    
	//Place input sélectionné par l'utilisateur dans $CONV_ROOT/src/ui/Python/work renommé en modele.inp
	std::string modele  = App->m.c_str();
	command = "cp " + modele + " " + GetConvroot() + "modele.inp";
    std::cerr << "Copy command='" << command << "'" << std::endl;
	if( system(command.c_str()) != 0 )
    {
        std::cerr << "COPY FAILURE" << std::endl;
    }
    
    //exit(0);
    
	//Place fichier des potentiels dans $CONV_ROOT/src/ui/Python/work
	std::string pot = GetPotPath();
    
	//On vérifie si l'adresse du potential est absolue, si oui on le copie directement à partir de cette adresse, sinon on ajoute cette adresse à celle de l'inp
	if (pot[0]=='/') {
		std::string command = "cp " + pot + " $CONV_ROOT/src/ui/python/work/";
		system(command.c_str());
	}
    
	else {
		int i = modele.length();
        
		while (modele[i]!='/') {
			i--;
		}
        
		modele.erase(i+1,modele.length());
		command = "cp " + modele + pot + " $CONV_ROOT/src/ui/python/work/";
		system(command.c_str());
	}
    
	//Place fichier des observables dans $CONV_ROOT/src/ui/Python/work
	std::string obs = GetObsPath();
    
	//On vérifie si c'est norot ou pas
	if (obs!="norot") {
        
		//On vérifie si l'adresse du potential est absolue, si oui on le copie directement à partir de cette adresse, sinon on ajoute cette adresse à celle de 			l'inp
		if (obs[0]=='/') {
			command = "cp " + obs + " $CONV_ROOT/src/ui/python/work/";
			system(command.c_str());
		}
        
		else {
			int i = modele.length();
            
			while (modele[i]!='/') {
				i--;
			}
            
			modele.erase(i+1,modele.length());
            
			command = "cp " + modele + obs + " $CONV_ROOT/src/ui/python/work/";
			system(command.c_str());
		}
	}
	
	//Déplace fichiers de restart si existants dans $CONV_ROOT/src/ui/Python/work
	if (GetRestartNameIn()!="erreur") {
		command = "cp " + modele + GetRestartNameIn() + "* $CONV_ROOT/src/ui/python/work/";
		system(command.c_str());
	}
}



//Vérifier si OUT dans input
bool IsOut()
{
	std::string link = GetConvroot() + "modele.inp";
	ifstream fichier(link.c_str());
    
	std::string contenu;
	for (int i = 1 ; i<=7 ; i++) {
		getline(fichier, contenu);
	}
    
	while (contenu=="NO") {
		getline(fichier, contenu);
	}
    
	if (contenu=="IN") {
		getline(fichier, contenu);
		getline(fichier, contenu);
	}
    
	if (contenu=="OUT") {
		return true;
	}
    
	else {
		return false;
	}
    
	fichier.close();
}



//Vérifier si IN dans input
bool IsIn()
{
	std::string link = GetConvroot() + "modele.inp";
	ifstream fichier(link.c_str());
    
	std::string contenu;
	for (int i = 1 ; i<=7 ; i++) {
		getline(fichier, contenu);
	}
    
	while (contenu=="NO") {
		getline(fichier, contenu);
	}
    
	if (contenu=="IN") {
		return true;
	}
    
	else {
		return false;
	}
    
	fichier.close();
}



//Obtenir nom du fichier de restart donné
std::string GetRestartNameOut()
{
	if (IsOut()) {
        
		std::string link = GetConvroot() + "modele.inp";
		ifstream fichier(link.c_str());
        
		std::string contenu;
		for (int i = 1 ; i<=7 ; i++) {
			getline(fichier, contenu);
		}
        
		while (contenu=="NO") {
			getline(fichier, contenu);
		}
        
		if (contenu=="IN") {
			getline(fichier, contenu);
			getline(fichier, contenu);
		}
        
		getline(fichier, contenu);
        
		fichier.close();
        
		return contenu;
	}
    
	else {
		return "erreur";
	}
}



//Obtenir nom du fichier de restart donné
std::string GetRestartNameIn()
{
	if (IsIn()) {
        
		std::string link = GetConvroot() + "modele.inp";
		ifstream fichier(link.c_str());
        
		std::string contenu;
		for (int i = 1 ; i<=7 ; i++) {
			getline(fichier, contenu);
		}
        
		while (contenu=="NO") {
			getline(fichier, contenu);
		}
        
		getline(fichier, contenu);
        
		fichier.close();
        
		return contenu;
	}
    
	else {
		return "erreur";
	}
}



//Déplacer ou supprimer fichiers du dossier work dans dossier output de l'utilisateur
void RemoveFiles(int i)
{
    
	std::string out = App->o.c_str();
	std::string command;
    
	if (i==1 or i==2 or i==3 or i==4) {
		if (IsIn()) {
			command = "mv $CONV_ROOT/src/ui/python/work/" + GetRestartNameIn() + "* " + out;
			system(command.c_str());
		}
        
		if (GetObsPath()!="norot") {
			command = "rm $CONV_ROOT/src/ui/python/work/" + GetObsPath();
			system(command.c_str());
		}
        
		command = "rm $CONV_ROOT/src/ui/python/work/" + GetPotPath();
		system(command.c_str());
        
		command = "rm $CONV_ROOT/src/ui/python/work/out";
		system(command.c_str());
        
		command = "mv $CONV_ROOT/src/ui/python/work/out[0-9]* " + out;
		system(command.c_str());
        
		command = "mv $CONV_ROOT/src/ui/python/work/zpe_trie.out[0-9]* " + out;
		system(command.c_str());
	}
    
	if (i==1) {
		if (IsOut()) {
			command = "mv $CONV_ROOT/src/ui/python/work/" + GetRestartNameOut() + "* " + out;
			system(command.c_str());
		}
        
		command = "rm $CONV_ROOT/src/ui/python/work/modele.inp";
		system(command.c_str());
        
		command = "rm $CONV_ROOT/src/ui/python/work/zpe_trie.out";
		system(command.c_str());
	}
    
	if (i==2) {
		command = "mv $CONV_ROOT/src/ui/python/work/step[0-9]* " + out;
		system(command.c_str());
        
		command = "mv $CONV_ROOT/src/ui/python/work/modele.inp " + out;
		system(command.c_str());
        
		command = "rm $CONV_ROOT/src/ui/python/work/zpe_trie.out";
		system(command.c_str());
	}
    
	if (i==3) {
		if (IsOut()) {
			command = "mv $CONV_ROOT/src/ui/python/work/" + GetRestartNameOut() + "* " + out;
			system(command.c_str());
		}
        
		command = "rm $CONV_ROOT/src/ui/python/work/modele.inp";
		system(command.c_str());
        
		command = "mv $CONV_ROOT/src/ui/python/work/inp " + out;
		system(command.c_str());
	}
    
	if (i==4) {
		command = "mv $CONV_ROOT/src/ui/python/work/step[0-9]* " + out;
		system(command.c_str());
        
		command = "mv $CONV_ROOT/src/ui/python/work/modele.inp " + out;
		system(command.c_str());
	}
}



void ExeSimpleRun()
{
	MoveFiles();
    
	//Execution python
	system("cd $CONV_ROOT/src/ui/python/ ; python ExeSimpleRun.py");
    
	RemoveFiles(1);
}



void ExeSCFRun()
{
	std::string fichier = GetConvroot() + "tol.txt";
    
	//Test si valeur tolérance entrée
	ifstream tol(fichier.c_str());
    
	if (tol) {
		MoveFiles();
        
		//Execution python
		system("cd $CONV_ROOT/src/ui/python/ ; python ExeSCFRun.py");
        
		RemoveFiles(2);
        
		tol.close();
	}
    
	else {
		cout << "Pas de valeur de tolérance" << endl;
	}
}



void Pairing()
{
	std::string fichier = GetConvroot() + "trunc.txt";
    
	//Test si valeur trunc entrée
	ifstream trunc(fichier.c_str());
    
	if (trunc) {
		MoveFiles();
        
		system("cd $CONV_ROOT/src/ui/python/ ; python Pairing.py");
        
		RemoveFiles(3);
        
		trunc.close();
	}
    
	else {
		cout << "Pas de valeur de trunc" << endl;
	}
}



void Hierarchy()
{
	std::string fichier = GetConvroot() + "tol.txt";
    
	ifstream tol(fichier.c_str());
    
	fichier = GetConvroot() + "trunc.txt";
    
	ifstream trunc(fichier.c_str());
    
	//test si valeurs tol et trunc entrées
	if (tol and trunc) {
		MoveFiles();
        
		system("cd $CONV_ROOT/src/ui/python/ ; python Hierarchy.py");
        
		RemoveFiles(4);
        
		tol.close();
		trunc.close();
	}
    
	else {
		cout << "Pas de valeur tol et/ou trunc" << endl;
	}
}



//Supprime tout le contenu du dossier work (en cas d'arret prématuré d'un script) puis recréé un lien symbolique conviv.e et les fichiers texte tol et trunc
void Clean()
{
	std::string command = "rm -R " + GetConvroot();
	system(command.c_str());
    
	command = "mkdir " + GetConvroot();
	system(command.c_str());
    
	std::string conviv =  App->c.c_str();
    
	command = "ln -s " + GetConvroot() + "../../../../" + conviv + " " + GetConvroot();
	system(command.c_str());
    
	SetTolerance();
	SetBasisTrunc();
}