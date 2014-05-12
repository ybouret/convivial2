#include "./conviv.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/exception.hpp"
#include "yocto/string/tokenizer.hpp"

static inline bool __getline( ios::istream &fp, string &line )
{
    line.clear();
    return fp.read_line(line) > 0;
}

#define __GETLINE() __getline(fp,line)

//==============================================================================
// statement 1 helper
//==============================================================================
static inline void parseSym( const string &line, string &sym_name, bool &sym )
{
    vector<string> fields;
    tokenizer::split(fields,line,character<char>::is_space );
    if( fields.size() < 2 )
        throw exception("invalid statement character(len=3)::sym_name logical::sym");
    
    for( size_t i=1; i <= fields.size(); ++i) fields[i].to_upper();
    
    sym_name = fields[1];
    const string &flag = fields[2];
    std::cerr << "#### sym_name='" << sym_name << "' sym=" << flag << std::endl;
    
    if( flag == ".F." )
    {
        sym = false;
        return;
    }
    
    if( flag == ".T." )
    {
        sym = true;
        return;
    }
    
    throw exception("invalid logical::sym '%s'", flag.c_str());
    
}


//==============================================================================
// statement 2 helper
//==============================================================================

static inline void parsePot( const string &line, string &pot_fmt, string &fn )
{
    vector<string> fields;
    tokenizer::split(fields,line,character<char>::is_space );
    if( fields.size() < 2 )
        throw exception("invalid statement character(len=8)::pot_fmt character(len=256)::fn");
    
    pot_fmt = fields[1];
    fn      = fields[2];
        
}


//==============================================================================
//
// Inport Data from a CONVIV input
//
//==============================================================================
void Conviv:: importCONVIV(const string &filename)
{
    std::cerr << "#### Loading Input '" << filename << "'" << std::endl;
    ios::icstream fp( filename );
    string        line;
    
    //--------------------------------------------------------------------------
    // statement 1
    //--------------------------------------------------------------------------
    if( ! __GETLINE() ) throw exception("Missing line for statement #1");
    parseSym(line,sym_name,sym);
    ValidateSymName();
    
    //--------------------------------------------------------------------------
    // statement 2
    //--------------------------------------------------------------------------
    if( ! __GETLINE() ) throw exception("Missing line for statement #2");
    parsePot(line, pot_fmt, fn);
    ValidatePotFmt();
    ValidateFn();
    
    //--------------------------------------------------------------------------
    // statement 3
    //--------------------------------------------------------------------------
    if( ! __GETLINE() ) throw exception("Missing line for statement #3");
    fnrot = line;
    ValidateFnRot();
        
    //--------------------------------------------------------------------------
    // statement 4
    //--------------------------------------------------------------------------
    if( ! __GETLINE() ) throw exception("Missing line for statement #4");
    title = line;
    ValidateTitle();
                                    
}