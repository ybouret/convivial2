#include "./conviv.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/exception.hpp"

void Conviv:: save(const string &filename) const
{
    
    ValidateAll();
    
    JSON::Value   jv( JSON::IsObject );
    JSON::Object &jo = jv.as<JSON::Object>();
    
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
    
    
    //----------------------------------------------------------------------
    // modes
    //----------------------------------------------------------------------
    {
        JSON::Value mv_val(JSON::IsArray);
        JSON::Array &mv = mv_val.as<JSON::Array>();
        for( const Mode *mode = modes.head;mode;mode=mode->next)
        {
            JSON::Array &m = mv.append<JSON::Array>();
            
            // character
            m.append<JSON::String>() = mode->character;
            
            // basis
            const Basis &b = mode->basis;
            
            m.append<JSON::String>() = b.name;
            m.append<JSON::Number>() = b.size;
            
            JSON::Array &prm = m.append<JSON::Array>();
            for(size_t i=1;i<=b.param.size();++i)
            {
                prm.append<JSON::Number>() = b.param[i];
            }
            
            
        }
        jo["modes"].swap_with(mv_val);
    }
    
    //==========================================================================
    // output object
    //==========================================================================
    ios::ocstream fp(  filename, false );
    //jv.output(fp);
}



void Conviv:: load( const string &filename )
{
    reset();
    try
    {
        //ios::icstream input(filename);
        Lang::Module::Handle hModule( new Lang::Module(filename) );
        Lang::Source         source(hModule);
        const JSON::Value  &jv = JSON::Value::LoadFrom(source);
        const JSON::Object &jo = jv.as<JSON::Object>();
        
        {
            ios::ocstream out( ios::cstderr );
            //jv.output(out);
        }
        
        //----------------------------------------------------------------------
        // statement 1
        //----------------------------------------------------------------------
        sym_name = jo["sym_name"].as<JSON::String>();
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
        pot_fmt = jo["pot_fmt"].as<JSON::String>(); ValidatePotFmt();
        fn      = jo["fn"].as<JSON::String>();      ValidateFn();
        
        
        //----------------------------------------------------------------------
        // statement 3
        //----------------------------------------------------------------------
        fnrot = jo["fnrot"].as<JSON::String>(); ValidateFnRot();
        
        //----------------------------------------------------------------------
        // statement 4
        //----------------------------------------------------------------------
        title = jo["title"].as<JSON::String>(); ValidateTitle();
        
        //----------------------------------------------------------------------
        // modes
        //----------------------------------------------------------------------
        
    }
    catch(...)
    {
        reset();
        throw;
    }
}
