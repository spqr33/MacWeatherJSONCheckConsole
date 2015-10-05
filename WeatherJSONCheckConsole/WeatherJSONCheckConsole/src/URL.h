/* 
 * File:   URL.h
 * Author: s
 *
 * Created on June 12, 2015, 4:09 PM
 */

#ifndef URL_H
#define	URL_H

#include <string>
#include <cassert>
#include <map>
#include <cstdint>
#include <stdexcept>
#include <sstream>
#include <arpa/inet.h>   //htons

using std::string;
using std::uint16_t;
using std::map;

namespace LobKo {

    class URL {
        friend std::ostream& LobKo::operator<<(std::ostream& out, const URL& url);
    public:

        enum Default {
            PORT = 80
        };
        URL(const string& original_string); //throw std::invalid_argument
        ~URL();

        const string& getHost() const;
        const string& getPath() const;
        const uint16_t& getPort() const;
        inline const string& originalRequestString() const;
    private:
        URL(const URL& orig);
        
        string schema_;
        string username_;
        string password_;
        string host_;
        uint16_t port_; //network order!
        string path_;
        string original_request_string_;

        class QuerySring {
        public:
            //QuerySring(const string& query_string);

            QuerySring() {
            };
            ~QuerySring();
            const string& getQuery() const;
        private:
            string full_query_string_;
            map<string, string> data_; //name value
        } *query_string_; //todo
        string fragment_id; //todo
    
        inline bool isAllowedSymb(char c) const;
        inline bool isDigit(char c) const;
        /**

         * @param first
         * @param last
         * @return 0 - if error; else port in net byteorder
         */
        inline uint16_t netOrderPortFromString(const char* first, const char* last) const;

        enum states_hostname {
            begin,
            allowedsym,
            allowedsym_at,
            allowedsym_at_allowedsym,
            allowedsym_at_allowedsym_colon,
            allowedsym_at_allowedsym_colon_digit,
            allowedsym_colon,
            allowedsym_colon_digit,
            allowedsym_colon_allowedsym,
            allowedsym_colon_allowedsym_at,
            allowedsym_colon_allowedsym_at_allowedsym,
            allowedsym_colon_allowedsym_at_allowedsym_colon,
            allowedsym_colon_allowedsym_at_allowedsym_colon_digit
        };

    };
    std::ostream& operator<<(std::ostream& out, const URL& url);

    bool LobKo::URL::isAllowedSymb(char c) const {
        char ch = c | (char) 0x20;

        if (ch >= 'a' && ch <= 'z') {
            return true;
        }
        if (c >= '0' && c <= '9') {
            return true;
        }
        if (c == '.' || c == '-' || c == '_' || c == '+') {
            return true;
        }
        return false;
    }

    bool LobKo::URL::isDigit(char c) const {
        if (c >= '0' && c <= '9') {
            return true;
        }

        return false;
    }

    uint16_t LobKo::URL::netOrderPortFromString(const char* first, const char* last) const {
        assert(first && "LobKo::URL::netOrderPortFromString(), first == NULL");
        assert(last && "LobKo::URL::netOrderPortFromString(), last == NULL");
        //todo implement cache - pair<string, port>
        std::istringstream inpStr(std::string(first, last + 1));
        unsigned long long port;

        inpStr >> port;
        if (port > (uint16_t) (~0) || port == 0) {
            throw std::invalid_argument("received incorrect port");
            return 0;
        }
        return htons((uint16_t) port);
    }

    const string& LobKo::URL::originalRequestString() const {
        return original_request_string_;
    }
};
#endif	/* URL_H */

