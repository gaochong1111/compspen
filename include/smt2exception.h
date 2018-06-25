#ifndef SMT2_EXCEPTION_H_
#define SMT2_EXCEPTION_H_

#define ERR_OK 0
#include <sstream>
#include <z3++.h>

class smt2exception {
        int         m_line;
        int         m_pos;
        std::string m_msg;

        std::string compose(char const* msg, z3::symbol const& s) {
                std::stringstream stm;
                stm << msg << s;
                return stm.str();
        }
public:

        smt2exception(char const * msg):m_msg(msg), m_line(-1), m_pos(-1) {}
        smt2exception(std::string const & msg):m_msg(msg), m_line(-1), m_pos(-1) {}
        smt2exception(std::string const & msg, int line, int pos):m_msg(msg), m_line(line), m_pos(pos) {}
smt2exception(char const * msg, z3::symbol const & s):m_msg(compose(msg,s)),m_line(-1),m_pos(-1) {}
        smt2exception(char const * msg, z3::symbol const & s, int line, int pos):m_msg(compose(msg,s)),m_line(line),m_pos(pos) {}

        bool has_pos() const { return m_line >= 0; }
        int line() const { return m_line; }
        int pos() const { return m_pos; }
        std::string get_msg() const{
                if (has_pos()) {
                        std::stringstream stm;
                        stm << m_msg << " before (line: " << m_line << ", col: " << m_pos <<")";
                        return stm.str();
                } else {
                        return m_msg;
                }
        }
};
#endif /* .h */
