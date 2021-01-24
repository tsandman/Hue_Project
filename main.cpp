#include "my_curl.hpp"
#include <jsoncpp/json/json.h>
#include <sstream> 

static string ip_hue = "192.168.1.77";

int main()
{
    string URL_base = "http://";
    URL_base += ip_hue + "/api/newdeveloper/lights";

    my_curl_t curl;
    curl.SetOpt_URL( URL_base );
    //cout << curl.RequestGet() << endl;

    std::stringstream sstr(curl.RequestGet());
    Json::Value json;
    sstr >> json;

    std::cout << json.toStyledString() << std::endl;

    Json::StreamWriterBuilder builder;
    builder["indentation"] = ""; // If you want whitespace-less output

    Json::Value state;
    std::vector<int> lightIds;
    std::vector<std::string> lightNames;
    int lightIdx = 0;
    for ( auto const& id : json.getMemberNames() )  
    {
        std::string rawName = Json::writeString( builder, json[id]["name"] );
        state[lightIdx]["name"] = .erase( remove( s.begin(), s.end(), '\"' ),
    s.end()
    );
        state[lightIdx]["id"] = atoi( id.c_str() );
        
        lightIdx++;
    }
    
    std::cout << "\nState:\n";
    std::cout << state.toStyledString() << std::endl;
    //"name":"Red Lamp",
    // "id":"1",
    // "on":true,
    // "brightness":45

    //const char *key = argv[2];
    //const char *value = argv[3];
    //string cmd = string( "{\"" ) + key + "\":" + value + "}";
    //curl.SetOpt_READDATA( cmd );
    //cout << curl.RequestPut() << endl;


   
    return 0;
}