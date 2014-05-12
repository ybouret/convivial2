#include "./conviv.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/exception.hpp"

void Conviv:: save(const string &filename) const
{
    
    ValidateAll();
    
    JSON::Value   jv( JSON::IsObject );
    JSON::Object &jo = jv.asObject();
    
    //==========================================================================
    // fill object
    //==========================================================================
    
    //----------------------------------------------------------------------
    // statement 1
    //----------------------------------------------------------------------
    jo["sym_name"] = sym_name;
    jo["sym"].make( sym ? JSON::IsTrue : JSON::IsFalse );
    
    //----------------------------------------------------------------------
    // statement 2
    //----------------------------------------------------------------------
    jo["pot_fmt"]  = pot_fmt;
    jo["fn"]       = fn;
    
    //----------------------------------------------------------------------
    // statement 3
    //----------------------------------------------------------------------
    jo["fnrot"]    = fnrot;
    
    //----------------------------------------------------------------------
    // statement 4
    //----------------------------------------------------------------------
    jo["title"] = title;
    
    
    //==========================================================================
    // output object
    //==========================================================================
    ios::ocstream fp(  filename, false );
    jv.output(fp);
}



void Conviv:: load( const string &filename )
{
    reset();
    try
    {
        lingua::input input( new ios::icstream(filename) );
        const JSON::Value  &jv = data.js(input);
        const JSON::Object &jo = jv.asObject(); 
        
        {
            ios::ocstream out( ios::cstderr );
            jv.output(out);
        }
        
        //----------------------------------------------------------------------
        // statement 1
        //----------------------------------------------------------------------
        sym_name = jo["sym_name"].asString();
        ValidateSymName();
        
        switch ( jo["sym"].type ) 
        {
            case JSON::IsTrue:
                sym = true;
                break;
                
            case JSON::IsFalse:
                sym = false;
                break;
                
            default:
                throw exception("invalid value for sym");
        }
        
        //----------------------------------------------------------------------
        // statement 2
        //----------------------------------------------------------------------
        pot_fmt = jo["pot_fmt"].asString(); ValidatePotFmt();
        fn      = jo["fn"].asString();      ValidateFn();
        
        
        //----------------------------------------------------------------------
        // statement 3
        //----------------------------------------------------------------------
        fnrot = jo["fnrot"].asString(); ValidateFnRot();
        
        //----------------------------------------------------------------------
        // statement 4
        //----------------------------------------------------------------------
        title = jo["title"].asString(); ValidateTitle();
    }
    catch(...)
    {
        reset();
        throw;
    }
}
