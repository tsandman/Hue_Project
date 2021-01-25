#include "HueManager.h"
#include <unistd.h>

static string ip_hue = "192.168.1.77";
static string username_hue = "newdeveloper";

int main()
{
    //Instantiate Hue Object
    HueManager hueObj( ip_hue, username_hue );

    //Initialize hue dictionary & print state
    hueObj.UpdateState( true );

    bool runHueMonitor = true;
    while ( runHueMonitor )
    {
        hueObj.UpdateState( false );
        if ( hueObj.GetStateChange() )
        {
            hueObj.PrintStateChange();
        }
    }
    //curl.SetOpt_READDATA( cmd );
    //cout << curl.RequestPut() << endl;


   
    return 0;
}