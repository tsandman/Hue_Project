#include "Utility.h"

void Utility::formatStateString( std::string& stringIn )
{
    size_t pos = 0;
    while ( ( pos = stringIn.find( '"', pos ) ) != std::string::npos )
    {
        stringIn = stringIn.erase(pos, 1);
    }
}