#include "HueManager.h"
#include "Utility.h"
#include <sstream> 
#include <map> 
#include <algorithm> 

HueManager::HueManager( std::string ip_hue, std::string hueUsernameIn ):
ipAddress( ip_hue ),
hueUsername( hueUsernameIn ),
lightsURL( "http://" + ip_hue + "/api/" + hueUsernameIn + "/lights" )
{
    //JSON format settings
    builder["indentation"] = ""; //whitespace-less output
    
    Json::Reader reader;
    reader.parse( "{}", emptyJsonObject );

    stateDictFormat = { "name", "id", "on", "brightness" };
}

//Requests Hue Information and updates state member
void HueManager::UpdateState( bool flagPrint )
{
    //Request Hue Information from Hue API
    curl.SetOpt_URL( lightsURL );

    //Populate JSON object
    std::stringstream ss_hueRequest( curl.RequestGet() );
    Json::Value hueRequest;
    ss_hueRequest >> hueRequest;
    
    //Set previous state var
    prevState = state;

    //Iterate JSON object to populate state dictionary
    int lightIdx = 0;
    for ( auto const& id : hueRequest.getMemberNames() )  
    {
        //Get light name
        std::string lightName = Json::writeString( builder, hueRequest[id]["name"] );
        Utility::formatStateString( lightName );

        //Populate name & id
        state[lightIdx]["name"] = lightName;
        state[lightIdx]["id"] = atoi( id.c_str() );
        
        //Request light information
        curl.SetOpt_URL( lightsURL + "/" + id );
        std::stringstream ss_idInfo( curl.RequestGet() );

        Json::Value idInfo;
        ss_idInfo >> idInfo;

        //Fill on & brightness
        state[lightIdx]["on"] = idInfo["state"]["on"];
        state[lightIdx]["brightness"] = idInfo["state"]["bri"];

        lightIdx++;
    }

    numLightsDetected = lightIdx;
    
    //Print state dictionary if selected
    if ( flagPrint )
    {
        std::cout << sortedStr( state, stateDictFormat, numLightsDetected ) << std::endl;
        //std::cout << state.toStyledString() << std::endl;
    }
}

//Compares light to previous state and prints the delta
void HueManager::PrintStateChange()
{
    for ( int lightIdx = 0; lightIdx < numLightsDetected; lightIdx++ )
    {
        Json::Value stateChange;
        bool changeFlag = false;
        for ( auto const& stateAttribute : state[lightIdx].getMemberNames() )  
        {
            Json::Value stateChange;
            if ( state[lightIdx][stateAttribute] != prevState[lightIdx][stateAttribute] && stateAttribute != "id" )
            {
                stateChange[lightIdx][stateAttribute] = state[lightIdx][stateAttribute];
                changeFlag = true;
                
                stateChange[lightIdx]["id"] = state[lightIdx]["id"];
                std::cout << formatStateString( stateChange[lightIdx] ) << std::endl;
            }
        }
        if ( changeFlag )
        {
            stateChange[lightIdx]["id"] = state[lightIdx]["id"];
        }
    }
}

//Changes order of Json keys for printout
std::string HueManager::sortedStr( Json::Value & value, std::vector<std::string> sortKeys, int vecLen ) 
{
    Json::Value sortedValue;  // The JSON object to store the new (sorted) hash
    std::string result;
    char newKey[60];

    if ( vecLen > 0 )
    {
        result = "[\n";
        for ( int vecIdx = 0; vecIdx < vecLen; vecIdx++ )
        {
            // Build new sortedValue
            int i = 0;
            for ( auto & key : sortKeys ) 
            {
                sprintf(newKey, "SORTEDKEY:%03d-%s", i++, key.c_str());
                sortedValue[newKey] = value[vecIdx][key];
            }

            // Write to string
            Json::StyledWriter w;
            result += w.write(sortedValue);

            // Remove tags from primary keys 
            std::size_t pos = 0;       
            while ( ( pos = result.find("SORTEDKEY:", pos) ) != std::string::npos ) 
            {
                result.erase(pos, 14);
            }
        }
        result += "]\n";
    }
    else
    {
        // Build new sortedValue
        int i = 0;
        for ( auto & key : sortKeys ) 
        {
            sprintf(newKey, "SORTEDKEY:%03d-%s", i++, key.c_str());
            sortedValue[newKey] = value[key];
        }
    
        // Write to string
        Json::StyledWriter w;
        result += w.write(sortedValue);
    
        // Remove tags from primary keys 
        std::size_t pos = 0;       
        while ( ( pos = result.find("SORTEDKEY:", pos) ) != std::string::npos ) 
        {
            result.erase(pos, 14);
        }
    }
    

    return result;
}

//Formats state string in proper order
std::string HueManager::formatStateString( Json::Value& value )
{
    std::vector<string> attrVec;
    std::vector<string> attrVecOut;


    for ( auto const& stateAttribute : value.getMemberNames() )  
    {
        attrVec.push_back( stateAttribute );
        auto itr = std::find( attrVec.begin(), attrVec.end(), stateAttribute );
        
        /* can be deleted
        if (itr == attrVec.end()) 
        {
            auto itr = std::find( attrVec.begin(), attrVec.end(), stateAttribute );
            attrVec.erase(itr);
        }
        */
    }

    //for ( std::vector<std::string>::iterator it = stateDictFormat.begin(); it != stateDictFormat.end(); it++ )
    for ( int i=0; i < stateDictFormat.size(); i++ )
    {
        auto itr = std::find( attrVec.begin(), attrVec.end(), stateDictFormat[i] );
        if ( itr != attrVec.end() )
        {
            attrVecOut.push_back( stateDictFormat[i] );
        }
    }

    return sortedStr( value, attrVecOut, 0 );
}