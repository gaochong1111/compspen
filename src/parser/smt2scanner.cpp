#include "smt2scanner.h"
#include <cassert>
#include <climits>
#include <sstream>

void smt2scanner::next() {
    if (m_cache_input)
        m_cache.push_back(m_curr);
    assert(!m_at_eof);
    if (m_interactive) {
        m_curr = m_stream.get();
        if (m_stream.eof())
            m_at_eof = true;
    }
    else if (m_bpos < m_bend) {
        m_curr = m_buffer[m_bpos];
        m_bpos++;
    }
    else {
        m_stream.read(m_buffer, SCANNER_BUFFER_SIZE);
        m_bend = static_cast<unsigned>(m_stream.gcount());
        m_bpos = 0;
        if (m_bpos == m_bend) {
            m_at_eof = true;
        }
        else {
            m_curr = m_buffer[m_bpos];
            m_bpos++;
        }
    }
    m_spos++;
}

void smt2scanner::read_comment() {
    assert(curr() == ';');
    next();
    while (true) {
        char c = curr();
        if (m_at_eof)
            return;
        if (c == '\n') {
            new_line();
            next();
            return;
        }
        next();
    }
}

smt2scanner::token smt2scanner::read_quoted_symbol() {
    assert(curr() == '|');
    bool escape = false;
    m_string.clear();
    next();
    while (true) {
        char c = curr();
        if (m_at_eof) {
            throw smt2exception("unexpected end of quoted symbol", m_line, m_spos);
        }
        else if (c == '\n') {
            new_line();
        }
        else if (c == '|' && !escape) {
            next();
            m_string.push_back(0);
            m_id = m_ctx.z3_context().str_symbol(get_string().c_str());
            return SYMBOL_TOKEN;
        }
        escape = (c == '\\');
        m_string.push_back(c);
        next();
    }
}

smt2scanner::token smt2scanner::read_symbol_core() {
    while (!m_at_eof) {
        char c = curr();
        signed char n = m_normalized[static_cast<unsigned char>(c)];
        if (n == 'a' || n == '0' || n == '-') {
            m_string.push_back(c);
            next();
        }
        else {
            m_string.push_back(0);
            m_id = m_ctx.z3_context().str_symbol(get_string().c_str());
            return SYMBOL_TOKEN;
        }
    }
    return EOF_TOKEN;
}

smt2scanner::token smt2scanner::read_symbol() {
    assert(m_normalized[static_cast<unsigned>(curr())] == 'a' || curr() == ':' || curr() == '-');
    m_string.clear();
    m_string.push_back(curr());
    next();
    return read_symbol_core();
}

smt2scanner::token smt2scanner::read_number() {
    assert('0' <= curr() && curr() <= '9');
    double q = 1;
    int number = curr() - '0';
    next();
    bool is_float = false;

    while (true) {
        char c = curr();
        if ('0' <= c && c <= '9') {
            number = (10)*number + (c - '0');
            if (is_float)
                q *= (10);
            next();
        }
        else if (c == '.') {
            if (is_float)
                break;
            is_float = true;
            next();
        }
        else {
            break;
        }
    }

    m_number = m_ctx.z3_context().int_val(number);
    if (is_float) {
        std::stringstream ss;
        ss << number / q;
        m_number = m_ctx.z3_context().real_val(ss.str().c_str());
    }
    return is_float ? FLOAT_TOKEN : INT_TOKEN;
}

smt2scanner::token smt2scanner::read_signed_number() {
    assert(curr() == '-');
    next();
    if ('0' <= curr() && curr() <= '9') {
        smt2scanner::token r = read_number();
        m_number = -m_number;
        return r;
    }
    else {
        // it is a symbol.
        m_string.clear();
        m_string.push_back('-');
        return read_symbol_core();
    }
}

smt2scanner::token smt2scanner::read_string() {
    assert(curr() == '\"');
    next();
    m_string.clear();
    while (true) {
        char c = curr();
        if (m_at_eof)
            throw smt2exception("unexpected end of string", m_line, m_spos);
        if (c == '\n') {
            new_line();
        }
        else if (c == '\"') {
            next();
            if (curr() != '\"') {
                m_string.push_back(0);
                return STRING_TOKEN;
            }
        }
        m_string.push_back(c);
        next();
    }
}

smt2scanner::token smt2scanner::read_bv_literal() {
    assert(curr() == '#');
    next();
    char c = curr();
    int number = 0;
    if (c == 'x') {
        next();
        c = curr();
        m_bv_size = 0;
        while (true) {
            if ('0' <= c && c <= '9') {
                number *= 16;
                number += c - '0';
            }
            else if ('a' <= c && c <= 'f') {
                number *= (16);
                number += (10 + (c - 'a'));
            }
            else if ('A' <= c && c <= 'F') {
                number *= (16);
                number += (10 + (c - 'A'));
            }
            else {
                if (m_bv_size == 0)
                    throw smt2exception("invalid empty bit-vector literal", m_line, m_spos);
                m_number = m_ctx.z3_context().bv_val(number, 16);
                return BV_TOKEN;
            }
            m_bv_size += 4;
            next();
            c = curr();
        }
    }
    else if (c == 'b') {
        next();
        c = curr();
        number  = (0);
        m_bv_size = 0;
        while (c == '0' || c == '1') {
            number *= (2);
            number += (c - '0');
            m_bv_size++;
            next();
            c = curr();
        }
        if (m_bv_size == 0)
            throw smt2exception("invalid empty bit-vector literal", m_line, m_spos);
        m_number = m_ctx.z3_context().bv_val(number, 2);
        return BV_TOKEN;
    }
    else {
        throw smt2exception("invalid bit-vector literal, expecting 'x' or 'b'", m_line, m_spos);
    }
}

smt2scanner::smt2scanner(smt2context& ctx, std::istream& stream, bool interactive) :
    m_ctx(ctx),
    m_interactive(interactive),
    m_spos(0),
    m_curr(0), // avoid Valgrind warning
    m_at_eof(false),
    m_line(1),
    m_pos(0),
    m_bv_size(UINT_MAX),
    m_bpos(0),
    m_bend(0),
    m_stream(stream),
    m_cache_input(false),
    m_number(ctx.z3_context()),
    m_id(ctx.z3_context().str_symbol("")){

    m_smtlib2_compliant = false;

    for (int i = 0; i < 256; ++i) {
        m_normalized[i] = (signed char) i;
    }
    m_normalized[static_cast<int>('\t')] = ' ';
    m_normalized[static_cast<int>('\r')] = ' ';
    // assert ('a' < 'z');
    for (char ch = 'b'; ch <= 'z'; ++ch) {
        m_normalized[static_cast<int>(ch)] = 'a';
    }
    for (char ch = 'A'; ch <= 'Z'; ++ch) {
        m_normalized[static_cast<int>(ch)] = 'a';
    }
    // assert ('0' < '9', '9' - '0' == 9);
    for (char ch = '1'; ch <= '9'; ++ch) {
        m_normalized[static_cast<int>(ch)] = '0';
    }
    // SMT2 "Symbols": ~ ! @ $ % ^ & * _ - + = < > . ? /
    m_normalized[static_cast<int>('~')] = 'a';
    m_normalized[static_cast<int>('!')] = 'a';
    m_normalized[static_cast<int>('@')] = 'a';
    m_normalized[static_cast<int>('$')] = 'a';
    m_normalized[static_cast<int>('%')] = 'a';
    m_normalized[static_cast<int>('^')] = 'a';
    m_normalized[static_cast<int>('&')] = 'a';
    m_normalized[static_cast<int>('*')] = 'a';
    m_normalized[static_cast<int>('_')] = 'a';
    m_normalized[static_cast<int>('-')] = '-';
    m_normalized[static_cast<int>('+')] = 'a';
    m_normalized[static_cast<int>('=')] = 'a';
    m_normalized[static_cast<int>('<')] = 'a';
    m_normalized[static_cast<int>('>')] = 'a';
    m_normalized[static_cast<int>('.')] = 'a';
    m_normalized[static_cast<int>('?')] = 'a';
    m_normalized[static_cast<int>('/')] = 'a';
    next();
}

smt2scanner::token smt2scanner::scan() {
    while (true) {
        signed char c = curr();
        m_pos = m_spos;

        if (m_at_eof)
            return EOF_TOKEN;

        switch (m_normalized[(unsigned char) c]) {
        case ' ':
            next();
            break;
        case '\n':
            next();
            new_line();
            break;
        case ';':
            read_comment();
            break;
        case ':':
            read_symbol();
            return KEYWORD_TOKEN;
        case '(':
            next();
            return LEFT_PAREN;
        case ')':
            next();
            return RIGHT_PAREN;
        case '|':
            return read_quoted_symbol();
        case 'a':
            return read_symbol();
        case '"':
            return read_string();
        case '0':
            return read_number();
        case '#':
            return read_bv_literal();
        case '-':
            if (m_smtlib2_compliant)
                return read_symbol();
            else
                return read_signed_number();
        default: {
            smt2exception ex("unexpected character", m_line, m_spos);
            next();
            throw ex;
        }}
    }
}

char const * smt2scanner::cached_str(unsigned begin, unsigned end) {
    m_cache_result.clear();
    while (isspace(m_cache[begin]) && begin < end)
        begin++;
    while (begin < end && isspace(m_cache[end-1]))
        end--;
    for (unsigned i = begin; i < end; i++)
        m_cache_result.push_back(m_cache[i]);
    m_cache_result.push_back(0);
    return m_cache_result.data();
}
