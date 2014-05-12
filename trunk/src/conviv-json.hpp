#include "./conviv.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/exception.hpp"

class Convivjson
{
public:
void save(const string &filename) const;
void load( const string &filename );
};
