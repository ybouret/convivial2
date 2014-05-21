#include "mode.hpp"
#include "yocto/exception.hpp"

Mode:: ~Mode() throw()
{
}


static inline
size_t __get_dof( const string &ch )
{
    if(ch.size()<=0)
        throw exception("Invalid Mode Character");
    
    switch( ch[0] )
    {
        case 'A':
        case 'B':
            return 1;
            
        case 'E':
            return 2;
            
        case 'F':
            return 3;
            
        default:
            throw exception("Invalid Character '%s'", ch.c_str());
            
    }
    
    return 0;
}

static inline
const string & __get_ch( const string             &ch,
                        const Schoenflies         &sym)
{
    bool found = false;
    for( size_t i=1; i <= sym.characters.size(); ++i )
    {
        if( sym.characters[i] == ch )
        {
            found = true;
            break;
        }
    }
    
    if(!found)
        throw exception("Invalid Character '%s' for symmetry '%s'", ch.c_str(), sym.symbol.c_str() );
    
    return ch;
}

Mode:: Mode(const string              &ch,
            const Schoenflies         &sym,
            const string              &basis_id,
            const BasisInfo::DataBase &basis_db ) :
character( __get_ch(ch,sym)  ),
DOF( __get_dof(character) ),
basis( basis_id, basis_db ),
next(0),
prev(0)
{
    
}


void Mode:: print() const
{
    std::cerr << "Mode '" << character << "'" << std::endl;
    std::cerr << "|_DOF    = " << DOF << std::endl;
    std::cerr << "|_Basis  = " << basis.name << std::endl;
    std::cerr << "  |_size = " << basis.size << std::endl;
    std::cerr << "  |_#prm = " << basis.param.size() << std::endl;
    for(size_t i=1;i<=basis.param.size();++i)
    {
        std::cerr << "    |_<" << basis.param[i] << ">" << std::endl;
    }
    
}

void Mode::make_info()
{
    info = character + '\t' + basis.name;
    info += '\t' + vformat("%u",unsigned(basis.size));
    for(size_t i=1;i<=basis.param.size();++i)
    {
        info += '\t' + vformat("%.6g", basis.param[i]);
    }
}


