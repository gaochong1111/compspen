#ifndef SMT2SCANNER_H_
#define SMT2SCANNER_H_

#include<iostream>
#include <z3++.h>
#include<vector>
#include<string>

#include "smt2exception.h"
#include "smt2context.h"


#define SCANNER_BUFFER_SIZE 1024


class smt2scanner {
private:
    bool               m_interactive;
    int                m_spos; // position in the current line of the stream
    char               m_curr;  // current char;
    bool               m_at_eof;

    int                m_line;  // line
    int                m_pos;   // start position of the token

    smt2context& m_ctx;

    // data
    z3::symbol             m_id;
    z3::expr             m_number;
    unsigned           m_bv_size;

    // end of data
    signed char        m_normalized[256];
    char               m_buffer[SCANNER_BUFFER_SIZE];
    unsigned           m_bpos;
    unsigned           m_bend;
    std::vector<char>       m_string;
    std::istream&      m_stream;

    bool               m_cache_input;
    std::vector<char>      m_cache;
    std::vector<char>      m_cache_result;



    bool               m_smtlib2_compliant;
    char curr() const { return m_curr; }
    void new_line() { m_line++; m_spos = 0; }
    void next();

public:

    enum token {
        NULL_TOKEN = 0,
        LEFT_PAREN = 1,
        RIGHT_PAREN,
        KEYWORD_TOKEN,
        SYMBOL_TOKEN,
        STRING_TOKEN,
        INT_TOKEN,
        BV_TOKEN,
        FLOAT_TOKEN,
        EOF_TOKEN
    };

    smt2scanner(smt2context& ctx , std::istream& stream, bool interactive = false);

    ~smt2scanner() {}

    int get_line() const { return m_line; }
    int get_pos() const { return m_pos; }
    z3::symbol const & get_id() const { return m_id; }
    z3::expr get_number() const { return m_number; }
    unsigned get_bv_size() const { return m_bv_size; }
    std::string get_string() const { std::string str(m_string.begin(), m_string.end()); return str;}
    token scan();

    token read_symbol_core();
    token read_symbol();
    token read_quoted_symbol();
    void read_comment();
    token read_number();
    token read_signed_number();
    token read_string();
    token read_bv_literal();

    void start_caching() { m_cache_input = true; m_cache.clear(); }
    void stop_caching() { m_cache_input = false; }
    unsigned cache_size() const { return m_cache.size(); }
    void reset_cache() { m_cache.clear(); }
    char const * cached_str(unsigned begin, unsigned end);
};
#endif /* SCANNER_H_ */
