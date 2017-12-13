#include "./conviv-data.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/exception.hpp"
#include "yocto/ios/rc.hpp"

void ConvivData:: displaySchoenflies( std::ostream &os ) const
{
    for( size_t i=1; i <= schoenflies.size(); ++i )
    {
        schoenflies(i).display( os );
    }
}

ConvivData:: ~ConvivData() throw()
{
}

static inline
JSON::Value & load_from( ios::resources &rc, const char *filename )
{
    const string           fn(filename);
    Lang::Input            fp( rc.load_stream(fn) );
    Lang::Module::Handle   hM( new Lang::Module(fn,fp) );
    Lang::Source           JS(hM);
    return JSON::Value::LoadFrom(JS);
}

ConvivData:: ConvivData( const char *application_path ) :
schoenflies(32,as_capacity),
htmlManual()
{
    ios::resources rc( application_path );
    
    //==========================================================================
    //
    // get schoenflies notation
    //
    //==========================================================================
    {
        //----------------------------------------------------------------------
        // load and parse the resource
        //----------------------------------------------------------------------
        JSON::Value           &jv = load_from(rc,"schoenflies.js");
        if( jv.type != JSON::IsObject )
            throw exception("invalid schoenflies.js");
        
        //----------------------------------------------------------------------
        // convert into a Schoenflies object
        //----------------------------------------------------------------------
        const JSON::Object &s  = jv.as<JSON::Object>();
        const size_t        ns = s.length();
        schoenflies.reserve(ns);
        for( JSON::Object::const_iterator p = s.begin();p!=s.end();++p)
        {
            const JSON::Pair   &P       = *p;
            const string       &symbol  = P.name;
            const JSON::Object &data    = P.value.as<JSON::Object>();
            const JSON::String &info    = data["info"].as<JSON::String>();
            const JSON::Value  &abelian = data["abelian"];
            
            bool isAbelian = false;
            switch ( abelian.type )
            {
                case JSON::IsTrue:  isAbelian= true;  break;
                case JSON::IsFalse: isAbelian= false; break;
                default:
                    throw exception("Schoenflies '%s': bad abelian field", symbol.c_str() );
            }
            
            Schoenflies        sym(symbol,info,isAbelian);
            vector<string>    &ch  = (vector<string>&) sym.characters;
            const JSON::Array &arr = data["char"].as<JSON::Array>();
            const size_t       nc  = arr.length();
            ch.reserve(nc);
            for( size_t i=1; i <= nc; ++i ) ch.push_back( arr[i].as<JSON::String>() );
            
            if( !schoenflies.insert(sym) )
                throw exception("unexpected multiple Schoenflies '%s'", symbol.c_str() );
        }
        
        //----------------------------------------------------------------------
        // free resources
        //----------------------------------------------------------------------
        jv.clear();
    }
    
    
    //==========================================================================
    //
    // get document
    //
    //==========================================================================
    {
        rc.load_chars(htmlManual, "doc.html", 1);
    }

    //==========================================================================
    //
    // get basis info
    //
    //==========================================================================
    {
        //----------------------------------------------------------------------
        // load and parse the resource
        //----------------------------------------------------------------------
        JSON::Value           &jv = load_from(rc,"basis.js");

        if( jv.type != JSON::IsObject )
            throw exception("invalid basis.js");
        
        const JSON::Object &db = jv.as<JSON::Object>();
        basisDB.reserve( db.length() );
        for( JSON::Object::const_iterator p = db.begin();p!=db.end();++p)
        {
            const JSON::Pair   &P       = *p;
            BasisInfo           bi(P.name);
            if( P.value.type != JSON::IsArray )
                throw exception("invalid value for '%s'", P.name.c_str());
            const JSON::Array &arr = P.value.as<JSON::Array>();
            for( size_t i=1; i <= arr.length(); ++i )
            {
                const JSON::Value &jstr = arr[i];
                if( jstr.type != JSON::IsString )
                    throw exception("'%s'[%d]: not a string", P.name.c_str(), int(i));
                bi.param.push_back( jstr.as<JSON::String>() );
            }
            if( !basisDB.insert(bi) )
                throw exception("Multiple BasisInfo '%s'", bi.name.c_str());
        }
        
    }
    
    
}


