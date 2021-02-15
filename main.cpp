#include "HueManager.h"
#include <unistd.h>

int main( int argc, char* argv[] )
{
    //Handle exception
    if ( argc != 3 )
    {
        std::cout << "\nERROR: Not enough input arguments";
        std::cout << "\n       Inputs: IP of Hue Bridge, Hue Dev Username\n";
        exit(0);
    }
    
    //Get command line arguments
    std::string ip_hue = argv[1];
    std::string username_hue = argv[2];

    //Instantiate Hue Object
    HueManager hueObj( ip_hue, username_hue );

    //Initialize hue dictionary & print state
    hueObj.UpdateState( true );

    //Monitor Hue port for changes in light state
    bool runHueMonitor = true;
    while ( runHueMonitor )
    {
        hueObj.UpdateState( false );
        if ( hueObj.GetStateChange() )
        {
            hueObj.PrintStateChange();
        }
    }
   
    return 0;
}