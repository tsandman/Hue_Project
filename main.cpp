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
        
        //TODO make func
        size_t pos = 0;
        while ((pos = rawName.find('"', pos)) != std::string::npos)
            rawName = rawName.erase(pos, 1);

        state[lightIdx]["name"] = rawName;

        state[lightIdx]["id"] = atoi( id.c_str() );
        
        URL_base = "http://" + ip_hue + "/api/newdeveloper/lights/" + id;
        curl.SetOpt_URL( URL_base );
        std::stringstream sstr(curl.RequestGet());

        Json::Value idInfo;
        sstr >> idInfo;
        std::cout << "\nLight " + id + ":\n";
        std::cout << idInfo.toStyledString() << std::endl;

        state[lightIdx]["on"] = idInfo["state"]["on"];
        state[lightIdx]["brightness"] = idInfo["state"]["bri"];

        lightIdx++;
    }
    
    std::cout << "\nState:\n";
    std::cout << state.toStyledString() << std::endl;

    //curl.SetOpt_READDATA( cmd );
    //cout << curl.RequestPut() << endl;


   
    return 0;
}