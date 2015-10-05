/* 
 * File:   HTTPResponse.cpp
 * Author: s
 * 
 * Created on June 19, 2015, 4:26 PM
 */

#include "HTTPResponse.h"
#include "utils.h"
#include <algorithm> //transform

LobKo::HTTPResponse::HTTPResponse(shared_ptr<HTTPRequest>& sp) :
spHTTPRequest_(sp) {

    p_proto_start_pos_ = p_response_code_digit_ = p_reason_phrase_ = NULL;
    p_current_header_name_start_ = p_current_header_value_start_ = NULL;

    parse_state = state_response_line;
    parse_response_line_state = state_parse_response_line_begin;
    parse_headers_state = state_parse_headers_begin;

}

LobKo::HTTPResponse::~HTTPResponse() {
    ;
}

const shared_ptr<LobKo::JumboBuff> LobKo::HTTPResponse::getJumboBuff() const {
    return spJumboBuff_;
}

void LobKo::HTTPResponse::setJumboBuff(shared_ptr<LobKo::JumboBuff> newBuff) {
    spJumboBuff_ = newBuff;
}

LobKo::HTTPResponse::parseStatus LobKo::HTTPResponse::parseResponseLine() {
    const char* &cur_pos = spJumboBuff_->currentPos_;
    const char* &water_mark = spJumboBuff_->watermark_;
    const char* const end = spJumboBuff_->start_ + spJumboBuff_->buffSize_;

    if ( parse_state != state_response_line ) {
        return PARSE_NOT_CORRESPOND_STATE;
    }
    parse_state = state_response_line;

    for ( char ch; cur_pos < water_mark; ) {
        ch = *cur_pos;

        // IF CODE == 100 it is necessary to set state_parse_response_line_begin
        //and p_proto_start_pos_ = p_response_code_digit_ = p_reason_phrase_ = NULL;

        switch (parse_response_line_state) {
            case state_parse_response_line_begin:
                if ( ch == 'H' || ch == 'h' ) {
                    p_proto_start_pos_ = cur_pos;
                    parse_response_line_state = state_parse_response_line_H;
                } else {
                    return parseStatus::PARSE_PROTO_ERROR;
                }
                break;
            case state_parse_response_line_H:
                if ( ch == 'T' || ch == 't' ) {
                    parse_response_line_state = state_parse_response_line_HT;
                } else {
                    return parseStatus::PARSE_PROTO_ERROR;
                };
                break;
            case state_parse_response_line_HT:
                if ( ch == 'T' || ch == 't' ) {
                    parse_response_line_state = state_parse_response_line_HTT;
                } else {
                    return parseStatus::PARSE_PROTO_ERROR;
                };
                break;
            case state_parse_response_line_HTT:
                if ( ch == 'P' || ch == 'p' ) {
                    parse_response_line_state = state_parse_response_line_HTTP;
                } else {
                    return parseStatus::PARSE_PROTO_ERROR;
                };
                break;
            case state_parse_response_line_HTTP:
                if ( ch == '/' ) {
                    parse_response_line_state = state_parse_response_line_HTTP_slash;
                } else {
                    return parseStatus::PARSE_PROTO_ERROR;
                };
                break;
            case state_parse_response_line_HTTP_slash:
                if ( ch == '0' ) {
                    parse_response_line_state = state_parse_response_line_HTTP_slash_0;
                } else if ( ch == '1' ) {
                    parse_response_line_state = state_parse_response_line_HTTP_slash_1;
                } else {
                    return parseStatus::PARSE_PROTO_ERROR;
                };
                break;
            case state_parse_response_line_HTTP_slash_0:
                if ( ch == '.' ) {
                    parse_response_line_state = state_parse_response_line_HTTP_slash_0_dot;
                } else {
                    return parseStatus::PARSE_PROTO_ERROR;
                };
                break;
            case state_parse_response_line_HTTP_slash_1:
                if ( ch == '.' ) {
                    parse_response_line_state = state_parse_response_line_HTTP_slash_1_dot;
                } else {
                    return parseStatus::PARSE_PROTO_ERROR;
                };
                break;
            case state_parse_response_line_HTTP_slash_0_dot:
                if ( ch == '9' ) {
                    parse_response_line_state = state_parse_response_line_HTTP_slash_0_dot_9;

                } else {
                    return parseStatus::PARSE_PROTO_ERROR;
                };
                break;
            case state_parse_response_line_HTTP_slash_1_dot:
                if ( ch == '0' ) {
                    parse_response_line_state = state_parse_response_line_HTTP_slash_1_dot_0;
                } else if ( ch == '1' ) {
                    parse_response_line_state = state_parse_response_line_HTTP_slash_1_dot_1;

                } else {
                    return parseStatus::PARSE_PROTO_ERROR;
                };
                break;
            case state_parse_response_line_HTTP_slash_0_dot_9:
            case state_parse_response_line_HTTP_slash_1_dot_0:
            case state_parse_response_line_HTTP_slash_1_dot_1:
                if ( p_proto_start_pos_ < cur_pos ) {
                    proto_.append(std::string(p_proto_start_pos_, cur_pos - p_proto_start_pos_));
                } else {
                    proto_.append(std::string(spJumboBuff_->start_, cur_pos - spJumboBuff_->start_));
                };
                if ( ch == ' ' || ch == '\t' ) {
                    parse_response_line_state = state_parse_response_line_proto_whitespace;
                } else {
                    return parseStatus::PARSE_PROTO_ERROR;
                }
                break;
            case state_parse_response_line_proto_whitespace:
                if ( isDigit12345(ch) ) {
                    p_response_code_digit_ = cur_pos;
                    parse_response_line_state = state_parse_response_line_proto_whitespace_digit;
                } else if ( ch == ' ' || ch == '\t' ) {
                    ;
                } else {
                    return parseStatus::PARSE_PROTO_ERROR;
                };
                break;

            case state_parse_response_line_proto_whitespace_digit:
                if ( isDigit(ch) ) {
                    parse_response_line_state = state_parse_response_line_proto_whitespace_digit_digit;
                } else {
                    return parseStatus::PARSE_CODE_ERROR;
                };
                break;
            case state_parse_response_line_proto_whitespace_digit_digit:
                if ( isDigit(ch) ) {
                    parse_response_line_state = state_parse_response_line_proto_whitespace_digit_digit_digit;
                } else {
                    return parseStatus::PARSE_CODE_ERROR;
                };
                break;
            case state_parse_response_line_proto_whitespace_digit_digit_digit:
                if ( p_response_code_digit_ < cur_pos ) {
                    status_code_.append(std::string(p_response_code_digit_, cur_pos - p_response_code_digit_));
                } else {
                    status_code_.append(std::string(spJumboBuff_->start_, cur_pos - spJumboBuff_->start_));
                };
                if ( ch == ' ' || ch == '\t' ) {
                    parse_response_line_state = state_parse_response_line_proto_code_whitespace;
                } else {
                    return parseStatus::PARSE_CODE_ERROR;
                }
                break;
            case state_parse_response_line_proto_code_whitespace:
                if ( isAllowedReasonPhraseSymb(ch) ) {
                    p_reason_phrase_ = cur_pos;
                    parse_response_line_state = state_parse_response_line_proto_code_reason;
                } else if ( ch == ' ' || ch == '\t' ) {
                    ;
                } else {
                    return parseStatus::PARSE_REASON_STR_ERROR;
                }
                break;
            case state_parse_response_line_proto_code_reason:
                if ( isAllowedReasonPhraseSymb(ch) ) {
                    ;
                } else if ( ch = SymbolCode::CR ) {
                    parse_response_line_state = state_parse_response_line_proto_code_reason_CR;

                    if ( p_reason_phrase_ < cur_pos ) {
                        reason_phrase_.append(std::string(p_reason_phrase_, cur_pos - p_reason_phrase_));
                    } else {
                        reason_phrase_.append(std::string(spJumboBuff_->start_, cur_pos - spJumboBuff_->start_));
                    };
                } else if ( ch = SymbolCode::LF ) {
                    parse_response_line_state = state_parse_response_line_proto_code_reason_LF;

                    if ( p_reason_phrase_ < cur_pos ) {
                        reason_phrase_.append(std::string(p_reason_phrase_, cur_pos - p_reason_phrase_));
                    } else {
                        reason_phrase_.append(std::string(spJumboBuff_->start_, cur_pos - spJumboBuff_->start_));
                    };
                } else {
                    return parseStatus::PARSE_REASON_STR_ERROR;
                }
                break;
            case state_parse_response_line_proto_code_reason_CR:
                if ( ch = SymbolCode::LF ) {
                    parse_response_line_state = state_parse_response_line_proto_code_reason_CR_LF;

                } else {
                    return parseStatus::PARSE_REASON_STR_ERROR;
                }
                break;
            case state_parse_response_line_proto_code_reason_LF:
            case state_parse_response_line_proto_code_reason_CR_LF:
                //respone line was parsed
                parse_state = state_response_line_finished;

                assert(p_proto_start_pos_ && p_response_code_digit_ && p_reason_phrase_);

                return PARSE_SUCCESS;
                break;
            default:
                assert("switch (parse_response_line_state) . Unknown state");
                //break;
        }; //switch (parse_response_line_state) 
        //!!!!!!!!!!!!!!!!!
        ++cur_pos;
        //!!!!!!!!!!!!!!!!!
        if ( cur_pos == end ) { //carry
            if ( parse_state == state_response_line ) {
                switch (parse_response_line_state) {
                    case state_parse_response_line_H:
                    case state_parse_response_line_HT:
                    case state_parse_response_line_HTT:
                    case state_parse_response_line_HTTP:
                    case state_parse_response_line_HTTP_slash:
                    case state_parse_response_line_HTTP_slash_0:
                    case state_parse_response_line_HTTP_slash_0_dot:
                    case state_parse_response_line_HTTP_slash_0_dot_9:
                    case state_parse_response_line_HTTP_slash_1:
                    case state_parse_response_line_HTTP_slash_1_dot:
                    case state_parse_response_line_HTTP_slash_1_dot_0:
                    case state_parse_response_line_HTTP_slash_1_dot_1:
                        proto_.append(std::string(p_proto_start_pos_, cur_pos - p_proto_start_pos_));
                        break;
                    case state_parse_response_line_proto_whitespace_digit:
                    case state_parse_response_line_proto_whitespace_digit_digit:
                    case state_parse_response_line_proto_whitespace_digit_digit_digit:
                        status_code_.append(std::string(p_response_code_digit_, cur_pos - p_response_code_digit_));
                        break;
                    case state_parse_response_line_proto_code_reason:
                        reason_phrase_.append(std::string(p_reason_phrase_, cur_pos - p_reason_phrase_));
                        break;
                    default:
                       ;
                }
            } // end if ( parse_state == state_parse_response_line ) {

            return PARSE_BUFFER_END_REACHED;
        } // end if ( cur_pos == end ) { //carry

        //!!!!!!!!!!!!!!!!!

    } // end for ( char ch; cur_pos < water_mark; ++cur_pos )

    return PARSE_WATERMARK_REACHED;
}

LobKo::HTTPResponse::parseStatus LobKo::HTTPResponse::parseHeaderLine() {
    const char* &cur_pos = spJumboBuff_->currentPos_;
    const char* &water_mark = spJumboBuff_->watermark_;
    const char* const end = spJumboBuff_->start_ + spJumboBuff_->buffSize_;

    parse_state = state_headers;

    for ( char ch; cur_pos < water_mark; ) {
        ch = *cur_pos;

        switch (parse_headers_state) {
            case state_parse_headers_first_header_colon_whitespace_symb_finished:
                curr_header_name = "";
                curr_header_value = "";
                p_current_header_name_start_ = NULL;
                p_current_header_value_start_ = NULL;
                //clean up, NO break!
            case state_parse_headers_begin:
                if ( ch == LF ) {
                    parse_headers_state = state_parse_headers_begin_LF;

                    ++cur_pos;
                    parse_state = state_headers_finished;
                    return PARSE_ALL_HEADERS_SUCCESS;
                } else if ( ch == CR ) {
                    parse_headers_state = state_parse_headers_begin_CR;
                } else if ( ch == ' ' || ch == '\t' ) {
                    return PARSE_HEADER_ERROR;
                } else if ( !isAllowedHeaderNameSymb(ch) ) {

                    return PARSE_HEADER_ERROR;
                } else {
                    p_current_header_name_start_ = cur_pos;

                    parse_headers_state = state_parse_headers_first_header;
                }
                break;
            case state_parse_headers_begin_CR:
                if ( ch == LF ) {
                    parse_headers_state = state_parse_headers_begin_CR_LF;

                    ++cur_pos;
                    parse_state = state_headers_finished;
                    return PARSE_ALL_HEADERS_SUCCESS;
                } else {
                    return PARSE_HEADER_ERROR;
                }
                break;
            case state_parse_headers_first_header:
                if ( ch == ':' ) {
                    if ( p_current_header_name_start_ < cur_pos ) {
                        curr_header_name.append(p_current_header_name_start_, cur_pos - p_current_header_name_start_);
                    } else {
                        curr_header_name.append(spJumboBuff_->start_, cur_pos - spJumboBuff_->start_);
                    };

                    parse_headers_state = state_parse_headers_first_header_colon;
                } else if ( !isAllowedHeaderNameSymb(ch) ) {
                    return PARSE_HEADER_ERROR;
                }
                break;
            case state_parse_headers_first_header_colon:
                if ( ch == ' ' || ch == '\t' ) {
                    parse_headers_state = state_parse_headers_first_header_colon_whitespace;
                } else if ( ch == LF ) {
                    parse_headers_state = state_parse_headers_first_header_colon_LF;
                    curr_header_value = "";

                    ++cur_pos;
                    return PARSE_SUCCESS;
                } else if ( ch == CR ) {
                    parse_headers_state = state_parse_headers_first_header_colon_CR;
                    curr_header_value = "";
                } else {
                    return PARSE_HEADER_ERROR;
                };
                break;
            case state_parse_headers_first_header_colon_CR:
                if ( ch == LF ) {
                    parse_headers_state = state_parse_headers_first_header_colon_CR_LF;

                    ++cur_pos;
                    return PARSE_SUCCESS;
                } else {
                    return PARSE_HEADER_ERROR;
                }
                break;
            case state_parse_headers_first_header_colon_whitespace:
                if ( ch == LF ) {
                    parse_headers_state = state_parse_headers_first_header_colon_whitespace_LF;
                    curr_header_value = "";

                    ++cur_pos;
                    return PARSE_SUCCESS;
                } else if ( ch == CR ) {
                    parse_headers_state = state_parse_headers_first_header_colon_whitespace_CR;
                    curr_header_value = "";
                }
                if ( ch != ' ' && ch != '\t' ) {
                    parse_headers_state = state_parse_headers_first_header_colon_whitespace_symb;
                    p_current_header_value_start_ = cur_pos;
                }
                break;
            case state_parse_headers_first_header_colon_whitespace_CR:
                if ( ch == LF ) {
                    parse_headers_state = state_parse_headers_first_header_colon_whitespace_CR_LF;

                    ++cur_pos;
                    return PARSE_SUCCESS;
                } else {
                    return PARSE_HEADER_ERROR;
                }
                break;
            case state_parse_headers_first_header_colon_whitespace_symb:
                if ( ch == LF ) {
                    parse_headers_state = state_parse_headers_first_header_colon_whitespace_symb_LF;

                    if ( p_current_header_value_start_ < cur_pos ) {
                        curr_header_value.append(p_current_header_value_start_, cur_pos - p_current_header_value_start_);
                    } else {
                        curr_header_value.append(spJumboBuff_->start_, cur_pos - spJumboBuff_->start_);
                    };
                } else if ( ch == CR ) {
                    parse_headers_state = state_parse_headers_first_header_colon_whitespace_symb_CR;

                    if ( p_current_header_value_start_ < cur_pos ) {
                        curr_header_value.append(p_current_header_value_start_, cur_pos - p_current_header_value_start_);
                    } else {
                        curr_header_value.append(spJumboBuff_->start_, cur_pos - spJumboBuff_->start_);
                    };
                };
                break;
            case state_parse_headers_first_header_colon_whitespace_symb_CR:
                if ( ch == LF ) {
                    parse_headers_state = state_parse_headers_first_header_colon_whitespace_symb_CR_LF;
                } else {
                    return PARSE_HEADER_ERROR;
                }
                break;
            case state_parse_headers_first_header_colon_whitespace_symb_LF:
            case state_parse_headers_first_header_colon_whitespace_symb_CR_LF:
                //header line was parsed
                parse_headers_state = state_parse_headers_first_header_colon_whitespace_symb_finished;

                assert(p_current_header_name_start_ && p_current_header_value_start_);

                return PARSE_SUCCESS;
                break;
            default:
                assert("switch (parse_response_line_state) . Unknown state");
                break;
        };
        //!!!!!!!!!!!!!!!!!
        ++cur_pos;
        //!!!!!!!!!!!!!!!!!
        if ( cur_pos == end ) { //carry
            if ( parse_state == state_headers ) {
                switch (parse_headers_state) {
                    case state_parse_headers_first_header:
                        curr_header_name.append(p_current_header_name_start_, cur_pos - p_current_header_name_start_);
                    case state_parse_headers_first_header_colon_whitespace_symb:
                        curr_header_value.append(p_current_header_value_start_, cur_pos - p_current_header_value_start_);
                        break;
                    default:
                        ;
                };
            }
            return PARSE_BUFFER_END_REACHED;
        };

    } //end for (char ch; cur_pos < water_mark;) 

    return PARSE_WATERMARK_REACHED;
}

void LobKo::HTTPResponse::assignKnownHeader(const string& name, const string& value) {
    string low_case_name = name;
    low_case_name.resize(name.size());

    std::transform(name.begin(), name.end(), low_case_name.begin(), ::tolower);
    if ( low_case_name == "content-length" ) {
        shared_ptr<Content_Length> spHeader(new Content_Length(name, value));

        spContent_Length = spHeader;
    } else {
        unsupported_headers_[name] = value;
    }
}