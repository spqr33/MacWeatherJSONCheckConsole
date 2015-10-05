/* 
 * File:   HTTPResponse.h
 * Author: s
 *
 * Created on June 19, 2015, 4:26 PM
 */

#ifndef HTTPRESPONSE_H
#define	HTTPRESPONSE_H

#include <memory>
#include "HTTPRequest.h"
#include "JumboBuff.h"
#include <string>
#include "Content_Length.h"
#include <map>


namespace LobKo {
    class HTTPRequest;

    class HTTPResponse {
    public:

        enum parseStatus {
            PARSE_SUCCESS, PARSE_BUFFER_END_REACHED, PARSE_WATERMARK_REACHED,
            PARSE_PROTO_ERROR, PARSE_CODE_ERROR, PARSE_REASON_STR_ERROR,
            PARSE_NOT_CORRESPOND_STATE, PARSE_HEADER_ERROR, PARSE_ALL_HEADERS_SUCCESS
        };

        enum parse_states {
            //state_parse_begin,
            state_response_line,
            state_response_line_finished,
            state_headers,
            state_headers_finished,
            state_body_finished

        };

        enum parse_response_line_states {
            state_parse_response_line_begin,
            state_parse_response_line_H,
            state_parse_response_line_HT,
            state_parse_response_line_HTT,
            state_parse_response_line_HTTP,
            state_parse_response_line_HTTP_slash,
            state_parse_response_line_HTTP_slash_0,
            state_parse_response_line_HTTP_slash_0_dot,
            state_parse_response_line_HTTP_slash_0_dot_9,
            state_parse_response_line_HTTP_slash_1,
            state_parse_response_line_HTTP_slash_1_dot,
            state_parse_response_line_HTTP_slash_1_dot_0,
            state_parse_response_line_HTTP_slash_1_dot_1,
            state_parse_response_line_proto_whitespace,
            state_parse_response_line_proto_whitespace_digit,
            state_parse_response_line_proto_whitespace_digit_digit,
            state_parse_response_line_proto_whitespace_digit_digit_digit,
            state_parse_response_line_proto_code_whitespace,
            state_parse_response_line_proto_code_reason,
            state_parse_response_line_proto_code_reason_CR,
            state_parse_response_line_proto_code_reason_LF,
            state_parse_response_line_proto_code_reason_CR_LF
        };

        enum parse_headers_states {
            state_parse_headers_begin,
            state_parse_headers_begin_LF,
            state_parse_headers_begin_CR,
            state_parse_headers_begin_CR_LF,
            state_parse_headers_first_header,
            state_parse_headers_first_header_colon,
            state_parse_headers_first_header_colon_LF,
            state_parse_headers_first_header_colon_CR,
            state_parse_headers_first_header_colon_CR_LF,
            state_parse_headers_first_header_colon_whitespace,
            state_parse_headers_first_header_colon_whitespace_LF,
            state_parse_headers_first_header_colon_whitespace_CR,
            state_parse_headers_first_header_colon_whitespace_CR_LF,
            state_parse_headers_first_header_colon_whitespace_symb,
            state_parse_headers_first_header_colon_whitespace_symb_LF,
            state_parse_headers_first_header_colon_whitespace_symb_CR,
            state_parse_headers_first_header_colon_whitespace_symb_CR_LF,
            state_parse_headers_first_header_colon_whitespace_symb_finished,
        };

        HTTPResponse(shared_ptr<HTTPRequest>& sp);
        virtual ~HTTPResponse();

        const shared_ptr<JumboBuff> getJumboBuff() const;
        void setJumboBuff(shared_ptr<JumboBuff>);

        inline parse_states get_parse_state() const;
        inline const string& get_proto() const;
        inline const string& get_status_code() const;
        inline const string& get_reason_phrase() const;
        inline const string& get_curr_name() const;
        inline const string& get_curr_value() const;

        parseStatus parseResponseLine();
        parseStatus parseHeaderLine();

        void assignKnownHeader(const string& name, const string& value);

        inline const shared_ptr<HTTPRequest> getLinkedHTTPRequest() const;
        shared_ptr<Content_Length> spContent_Length;
    private:
        HTTPResponse(const HTTPResponse& orig);
        HTTPResponse& operator=(HTTPResponse& orig);

        shared_ptr<HTTPRequest> spHTTPRequest_;
        shared_ptr<JumboBuff> spJumboBuff_;
        std::map<string, string> unsupported_headers_;

        std::string proto_;
        std::string status_code_;
        std::string reason_phrase_;

        std::string curr_header_name;
        std::string curr_header_value;

        const char* p_proto_start_pos_;
        const char* p_response_code_digit_;
        const char* p_reason_phrase_;
        const char* p_current_header_name_start_;
        const char* p_current_header_value_start_;

        parse_states parse_state;
        parse_response_line_states parse_response_line_state;
        parse_headers_states parse_headers_state;
    private:
        inline bool isAllowedReasonPhraseSymb(char c) const;
        inline bool isAllowedHeaderNameSymb(char c) const;
        inline bool isDigit12345(const char& c) const;
        inline bool isDigit(const char& c) const;
    };
};

bool LobKo::HTTPResponse::isDigit12345(const char& c) const {
    if (c >= '1' && c <= '5') {
        return true;
    }

    return false;
}

bool LobKo::HTTPResponse::isDigit(const char& c) const {
    if (c >= '0' && c <= '9') {
        return true;
    }

    return false;
}

bool LobKo::HTTPResponse::isAllowedReasonPhraseSymb(char c) const {
    char ch = c | (char) 0x20;

    if (ch >= 'a' && ch <= 'z') {
        return true;
    }
    if (c >= '0' && c <= '9') {
        return true;
    }
    if (c == ' ' || c == '.' || c == '-' || c == '_' || c == '+') {
        return true;
    }
    return false;
}

bool LobKo::HTTPResponse::isAllowedHeaderNameSymb(char c) const {
    static unsigned char lowcase[] =
            "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
            "\0\0\0\0\0\0\0\0\0\0\0\0\0-\0\0" "0123456789\0\0\0\0\0\0"
            "\0abcdefghijklmnopqrstuvwxyz\0\0\0\0\0"
            "\0abcdefghijklmnopqrstuvwxyz\0\0\0\0\0"
            "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
            "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
            "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
            "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

    return lowcase[c];
};

LobKo::HTTPResponse::parse_states LobKo::HTTPResponse::get_parse_state() const {
    return parse_state;
};

const string& LobKo::HTTPResponse::get_proto() const {
    return proto_;
}

const string& LobKo::HTTPResponse::get_status_code() const {
    return status_code_;
}

const string& LobKo::HTTPResponse::get_reason_phrase() const {
    return reason_phrase_;
}

const string& LobKo::HTTPResponse::get_curr_name() const {
    return curr_header_name;
}

const string& LobKo::HTTPResponse::get_curr_value() const {
    return curr_header_value;
}

const shared_ptr<LobKo::HTTPRequest> LobKo::HTTPResponse::getLinkedHTTPRequest() const {
    return spHTTPRequest_;
}
#endif	/* HTTPRESPONSE_H */

