// my_curl.hpp written by JaeHyukKwak
#include <string>
#include <iostream> // cout, cerr
#include <fstream> // ifstream
#include <stdexcept> // runtime_error
#include <memory>   // unique_ptr
#include <string.h> // memcpy
using namespace std;

#include <curl/curl.h>

class my_curl_t
{
    unique_ptr< CURL, decltype( curl_easy_cleanup )* > curl_;
    string html_;   // temporary buffer to store HTML content
    string put_;   // temporary buffer to store PUT content

public:
    my_curl_t() : curl_( curl_easy_init(), curl_easy_cleanup )
    {
        if ( nullptr == curl_ )
            throw runtime_error( "curl_easy_init() failed." );
        SetOpt_( CURLOPT_WRITEDATA, &html_ );
        SetOpt_( CURLOPT_WRITEFUNCTION, WriteMemoryCallback );
        SetOpt_( CURLOPT_READDATA, &put_ );
        SetOpt_( CURLOPT_READFUNCTION, ReadDataCallback );
    }

    string RequestGet() { return Request_( 0 ); }
    string RequestPut() { return Request_( 1 ); }

    void SetOpt_READDATA( string data ) { put_ = data; }
    void SetOpt_URL( string url ) { SetOpt_( CURLOPT_URL, url.c_str() ); }

private:
    string Request_( int post )
    {
        SetOpt_( CURLOPT_UPLOAD, post );
        const CURLcode res = curl_easy_perform( curl_.get() );
        if ( CURLE_OK != res )
            throw runtime_error( string( "curl_easy_perform() failed: " ) + curl_easy_strerror( res ) );
        const string htmlCopied = html_;
        html_.clear();
        return htmlCopied;
    }

    template< typename ParamType >
    void SetOpt_( CURLoption option, const ParamType &parameter )
    {
        CURLcode res = curl_easy_setopt( curl_.get(), option, parameter );
        if ( CURLE_OK != res )
            throw runtime_error( string( "curl_easy_setopt failed: " ) + curl_easy_strerror( res ) );
    }

    static size_t WriteMemoryCallback( void *contents, size_t size, size_t numberOfMemoryBlock, void *userChunk )
    {
        const size_t realSize = size * numberOfMemoryBlock;
        string &html = *static_cast< string * >( userChunk );
        html += string( static_cast< char* >( contents ), realSize );
        return realSize;
    }

    static size_t ReadDataCallback( char *buf, size_t size, size_t nitems, void *instream )
    {
        string &put = *static_cast< string * >( instream );
        const size_t putSize = put.size();
        const size_t minSize = min( putSize, size * nitems );
        memcpy( buf, put.c_str(), minSize );
        if ( minSize == putSize ) put.clear();
        else put = put.substr( minSize );
        return minSize;
    }
};