#ifndef SMT2PARSER_PRO_H
#define SMT2PARSER_PRO_H

#include <map>
#include <sstream>

#include "smt2scanner.h"
#include "predicate.h"
#include "solver.h"

class smt2parser {


public:




        enum sl_decl_kind {
                SLRD_OP_TRUE,
                SLRD_OP_FALSE,
                SLRD_OP_EQ,
                SLRD_OP_DISTINCT,
                SLRD_OP_AND,
                SLRD_OP_OR,
                SLRD_OP_NOT,
                SLRD_OP_IMPLIES,
                SLRD_OP_LE,
                SLRD_OP_GE,
                SLRD_OP_LT,
                SLRD_OP_GT,
                SLRD_OP_ADD,
                SLRD_OP_SUB,
                SLRD_OP_MUL,
                SLRD_OP_EMP,
                SLRD_OP_REF,
                SLRD_OP_SREF,
                SLRD_OP_PTO,
                SLRD_OP_SSEP,
                SLRD_OP_EXISTS,
                SLRD_OP_NIL,
                SLRD_OP_OTHER
        };

        smt2scanner              m_scanner;
        smt2context& m_ctx;
        solver* sol;
        smt2scanner::token       m_curr;

        std::string m_logic_name;

        class local {
        public:
                z3::expr           m_term;
                unsigned         m_level;
        local(z3::expr t, unsigned l):m_term(t), m_level(l) {}
        };

        std::map<z3::symbol, std::vector<z3::expr>> m_sort_fields_table; // srot -> fields
        std::map<z3::symbol, z3::func_decl> m_builtin_func_table;
        std::map<z3::symbol, std::pair<z3::sort, unsigned> > m_builtin_sort_table;

        std::vector<z3::symbol> m_symbol_stack;
        std::vector<z3::expr> m_expr_stack;
        std::vector<unsigned> m_expr_frame_stack;

        std::vector<z3::expr> m_sorted_var_stack; // vars
        std::vector<unsigned> m_var_frame_stack;



        unsigned            m_cache_end;
        std::vector<std::string> m_cached_strings;
        int m_num_open_paren;
        unsigned m_num_bindings;



        z3::symbol               m_exists;

        z3::symbol               m_assert;
        z3::symbol               m_check_sat;
        z3::symbol               m_get_model;
        z3::symbol               m_define_fun;
        z3::symbol               m_declare_fun;
        z3::symbol               m_declare_sort;
        z3::symbol               m_set_logic;
        z3::symbol               m_set_info;


        void scan_core();

        void scan(); // scan a symbol

        void next(); // next symbol

        smt2scanner::token curr() const { return m_curr; }


        bool sync_after_error() ;

        void error(unsigned line, unsigned pos, char const * msg) ;

        void error(char const * msg) ;

        void error_wo_pos(char const * msg) ;


        int line() {m_scanner.get_line();}
        int pos() {m_scanner.get_pos();}
        Log& logger() {return m_ctx.logger();}
        z3::context& z3_ctx() {return m_ctx.z3_context();}

        z3::symbol const & curr_id() const { return m_scanner.get_id(); }
        z3::expr curr_numeral() const { return m_scanner.get_number(); }

        void check_next(smt2scanner::token t, char const * msg);
        bool curr_is_lparen() const { return curr() == smt2scanner::LEFT_PAREN; }
        bool curr_is_rparen() const { return curr() == smt2scanner::RIGHT_PAREN; }
        bool curr_is_identifier() const { return curr() == smt2scanner::SYMBOL_TOKEN; }
        bool curr_is_int() const { return curr() == smt2scanner::INT_TOKEN; }
        bool curr_is_float() const { return curr() == smt2scanner::FLOAT_TOKEN; }

        bool curr_id_is_exists() const { assert(curr_is_identifier()); return curr_id() == m_exists; }

        void check_lparen_next(char const * msg) { check_next(smt2scanner::LEFT_PAREN, msg); }
        void check_rparen_next(char const * msg) { check_next(smt2scanner::RIGHT_PAREN, msg); }
        void check_rparen(char const * msg) { if (!curr_is_rparen()) throw smt2exception(msg, line(), pos()); }
        void check_identifier(char const * msg) { if (!curr_is_identifier()) throw smt2exception(msg, line(), pos()); }
        void check_int(char const * msg) { if (!curr_is_int()) throw smt2exception(msg, line(), pos()); }
        void check_int_or_float(char const * msg) { if (!curr_is_int() && !curr_is_float()) throw smt2exception(msg, line(), pos()); }

//        void parse_sexpr();

        bool check_args_bool(z3::expr_vector& args);
        bool check_args_numeral(z3::expr_vector& args); // int or real
        bool check_args_userdef(z3::expr_vector& args); //
        bool check_args_setref(z3::expr_vector& args);
        bool check_args_space(z3::expr_vector& args);

        z3::expr make_compare(sl_decl_kind kind, z3::expr_vector& args);
        z3::expr make_eq(sl_decl_kind kind, z3::expr_vector& args);
        z3::expr make_arith(sl_decl_kind kind, z3::expr_vector& args);
        z3::expr make_logic(sl_decl_kind kind, z3::expr_vector& args);
        z3::expr make_app(z3::symbol& func_sym, z3::expr_vector& args);


        z3::expr make_ref(z3::expr_vector& args);
        z3::expr make_sref(z3::expr_vector& args);
        z3::expr make_pto(z3::expr_vector& args);
        z3::expr make_ssep(z3::expr_vector& args);
        z3::expr make_exists(z3::expr_vector& args);
        z3::expr make_user_def(z3::func_decl f, z3::expr_vector& args);

        bool check_var_exist(std::vector<z3::expr>& vec, z3::expr& var);
        int check_var_exist(std::vector<z3::expr>& vec, z3::symbol& var_sym);

        void check_base_rule(z3::expr& base_rule);
        pred_rule check_rec_rule(z3::expr& rec_rule, std::string f);
        void check_pred(z3::expr& body, predicate& pred);


        void parse_expr();

        z3::sort parse_sort();

        bool parse_parameters();

        void parse_define_fun();

        void parse_set_logic();

        void parse_declare_sort();

        void parse_declare_fun();

        void parse_assert();

        void parse_get_model();

        void parse_check_sat();

        void parse_set_info();

        void parse_cmd();

        bool operator()();

        void init_theory();

        smt2parser(smt2context & ctx, std::istream & is);
        ~smt2parser();

};


#endif /* SMT2PARSER_PRO_H */
