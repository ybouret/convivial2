#ifndef __CONVIV_INCLUDED
#define __CONVIV_INCLUDED 1

#include "./conviv-data.hpp"
#include "./conviv-json.hpp"
#include "./modes.hpp"

using namespace yocto;


////////////////////////////////////////////////////////////////////////////////
//
//! Conviv Simulation
//
////////////////////////////////////////////////////////////////////////////////
class Conviv
{
public:
    explicit Conviv( const ConvivData &the_data );
    virtual ~Conviv() throw();
    void reset() throw();
        
    bool dirty;
    
    //--------------------------------------------------------------------------
    // statement 1
    //--------------------------------------------------------------------------
    string sym_name; //!< character(len=3)
    bool   sym;      //!< logical
    
    //--------------------------------------------------------------------------
    // statement 2
    //--------------------------------------------------------------------------
    string pot_fmt; //!< character(len=8)
    string fn;      //!< charater(len=256)
    string m;       //!< charater(len=256)
    string o;       //!< charater(len=256)
    string c;       //!< charater(len=256)
    static const size_t fn_max = 256;
    
    
    //--------------------------------------------------------------------------
    // statement 3
    //--------------------------------------------------------------------------
    string fnrot; //!< character(len=64)
    static const size_t fnrot_max = 64;
    
    //--------------------------------------------------------------------------
    // statement 4
    //--------------------------------------------------------------------------
    string title; //!<  character(len=64)
    static const size_t title_max = 64;
    
    //--------------------------------------------------------------------------
    // modes
    //--------------------------------------------------------------------------
    Modes modes;
    
    //==========================================================================
    // I/O
    //==========================================================================
    
    //! try to parse a CONVIV input
    void importCONVIV( const string &filename );
    
    //! export a CONVIV input
    void exportCONVIV( const string &filename ) const;
    
    //! save the encoded JSON
    void save( const string &filename ) const;
    
    //! load a JSON description
    void load( const string &filename);
    
    //==========================================================================
    // Auxiliary Data
    //==========================================================================
    const ConvivData &data;
    
    
    void ValidateAll() const;
    
private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(Conviv);
    
    void ValidateSymName() const;
    void ValidatePotFmt() const;
    void ValidateFn() const;
    void ValidateFnRot() const;
    void ValidateTitle() const;
    
};


#endif
