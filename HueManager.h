#include "lib/jsoncpp/include/json/json.h"
#include "lib/my_curl/my_curl.hpp"

class HueManager
{
private:
    std::string ipAddress;
    std::string hueUsername;

    Json::Value state;
    Json::Value prevState;

    std::vector<std::string> stateDictFormat;

    int numLightsDetected;

    Json::StreamWriterBuilder builder;
    Json::Value emptyJsonObject;

    my_curl_t curl;

    //Constants
    const std::string lightsURL;

    std::string sortedStr( Json::Value& value, std::vector<std::string> sortKeys, int vecLen );
    std::string formatStateString( Json::Value& value );

public:
    HueManager() {};
    HueManager( std::string ip_hue, std::string hueUsernameIn );

    void UpdateState( bool flagPrint );
    void PrintStateChange();

    inline bool GetStateChange() { return state != prevState; };
};