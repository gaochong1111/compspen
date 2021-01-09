#include <cassert>

#include "smt2parser.h"

#include "solverfactory.h"

void smt2parser::scan_core() {
        m_cache_end = m_scanner.cache_size();
        m_curr      = m_scanner.scan();
}

void smt2parser::scan() {
        switch (m_curr) {
        case smt2scanner::LEFT_PAREN: m_num_open_paren++; break;
        case smt2scanner::RIGHT_PAREN: m_num_open_paren--; break;
        default: break;
        }
        scan_core();
}

void smt2parser::next() {
        if (m_curr != smt2scanner::EOF_TOKEN)
                scan();
}


void smt2parser::check_next(smt2scanner::token t, const char *msg) {
        if (curr() == t) {
                next();
                return;
        }
        throw smt2exception(msg, line(),  pos());
}

// consume garbage
// return true if managed to recover from the error...
bool smt2parser::sync_after_error() {
        while (true) {
                try {
                        while (curr_is_rparen())
                                next();
                        if (m_num_open_paren < 0)
                                m_num_open_paren = 0;
                        if (curr() == smt2scanner::EOF_TOKEN && m_num_open_paren == 0)
                                return true;
                        assert(m_num_open_paren >= 0);
                        while (m_num_open_paren > 0 || !curr_is_lparen()) {
                                if (curr() == smt2scanner::EOF_TOKEN) {
                                        return false;
                                }
                                assert(m_num_open_paren >= 0);
                                next();
                                assert(m_num_open_paren >= -1);
                                if (m_num_open_paren < 0)
                                        m_num_open_paren = 0;
                                assert(m_num_open_paren >= 0);
                        }
                        return true;
                }
                catch (smt2exception & ex) {
                        assert(ex.has_pos());
                        error(ex.line(), ex.pos(), ex.get_msg().c_str());
                }
        }
}

void smt2parser::error(unsigned line, unsigned pos, char const * msg) {
        logger().log_format("(error (line: %d, column: %d), %s) \n", line, pos, msg);

        if (m_ctx.exit_on_error()) {
                exit(1);
        }
}


void smt2parser::error(char const * msg) {
        error(line(),  pos(), msg);
}

void smt2parser::error_wo_pos(char const * msg) {
        logger().log_format("(error: %s)", msg);
}


/**
 * check the args are all bool type
 * @param args : the parameters
 * @return true: if all of the parameters are bool type
 */
bool smt2parser::check_args_bool(z3::expr_vector& args) {
        for (unsigned i=0; i<args.size(); i++) {
                if (!args[i].is_bool()) {
                        return false;
                }
        }
        return true;
}
/**
 * check the args are all numeral type
 * @param args : the parameters
 * @return true: if all of the parameters are  arith type
 */
bool smt2parser::check_args_numeral(z3::expr_vector& args) {
        for (unsigned i=0; i<args.size(); i++) {
                if (!args[i].is_arith()) {
                        return false;
                }
        }
        return true;
}

/**
 * check the args are all user_def type
 * @param args : the parameters
 * @return true: if all of the parameters are $UNINTERPRETED_SORT
 */
bool smt2parser::check_args_userdef(z3::expr_vector& args) {
        for (unsigned i=0; i<args.size(); i++) {
                if (args[i].get_sort().sort_kind() != Z3_UNINTERPRETED_SORT) {
                        return false;
                }
        }
        return true;
}

/**
 * check the args are all (SetRef A)
 * @param args : parameters
 * @return true: if all of the parameters are (Array A Void)
 */
bool smt2parser::check_args_setref(z3::expr_vector &args) {
        assert(args.size() > 0);
        z3::sort sort_0 = args[0].get_sort();
        if (!sort_0.is_array() || sort_0.array_range().to_string() != "Void") {
                return false;
        }
        z3::sort A_sort = sort_0.array_domain();
        for (unsigned i=1; i<args.size(); i++) {
                z3::sort sort_i = args[i].get_sort();
                if (!sort_i.is_array() || sort_i.array_range().to_string() != "Void" || sort_i.array_domain().to_string() != A_sort.to_string()) {
                        return false;
                }
        }
        return true;
}

/**
 * check the args are all (Space)
 * @param args : parameters
 * @return true : if all of the parameters are (Space)
 */
bool smt2parser::check_args_space(z3::expr_vector &args) {
        for (unsigned i=0; i<args.size(); i++) {
                if (args[i].get_sort().to_string() != "Space") {
                        return false;
                }
        }
        return true;
}

/**
 * check whether the $var is in $vec
 * @param vec : var list
 * @param var : var
 * @return true: if $var in $vec, otherwise false
 */
bool smt2parser::check_var_exist(std::vector<z3::expr> &vec, z3::expr& var) {
        for (int i=0; i<vec.size(); i++) {
                if (vec[i].hash() == var.hash()) {
                        return true;
                }
        }
        return false;
}

/**
 * check whether the $var_sym is in $vec
 * @param vec : var list
 * @param var_sym : maybe a variable
 * @return index: if $var in $vec, otherwise -1
 */
int smt2parser::check_var_exist(std::vector<z3::expr> &vec, z3::symbol& var_sym) {
        for (int i=0; i<vec.size(); i++) {
                if (vec[i].to_string() == var_sym.str()) {
                        return i;
                }
        }
        return -1;
}

/**
 * check base rule (alpha = beta)
 * @param base_rule : expr
 *
 */
void smt2parser::check_base_rule(z3::expr &base_rule) {

        if (!base_rule.is_app()) {
                throw smt2exception("app is excepted in the base rule", line(), pos());
        }
        if (base_rule.decl().name().str() != "and") {
            z3::expr_vector and_items(z3_ctx());
            and_items.push_back(base_rule);
            base_rule = z3::mk_and(and_items);
        }
        for (int i=0; i<base_rule.num_args()-1; i++) {
                if (!base_rule.arg(i).is_app() ||
                    (base_rule.arg(i).decl().name().str() != "tobool" && base_rule.arg(i).decl().name().str() != "=" && base_rule.arg(i).decl().name().str() != "distinct")) {
                        throw smt2exception("'=' or 'distinct' is excepted in the base rule", line(), pos());
                }
        }
}

/**
 * check recursive rule (exists (parameters) (data (ssep )))
 * @param rec_rule : expr
 * @return rule : if NO EXCEPTION
 */
pred_rule smt2parser::check_rec_rule(z3::expr &rec_rule, std::string f_name) {
        // pars
        if (!rec_rule.is_quantifier()) {
                throw smt2exception("'exists' is excepted in the recursive rule", line(), pos());
        }
        z3::expr body = rec_rule.body();
        if (!body.is_app()) {
                throw smt2exception("app is excepted in the recursive rule", line(), pos());
        }
        if (body.decl().name().str() != "and") {
            z3::expr_vector and_items(z3_ctx());
            and_items.push_back(body);
            body = z3::mk_and(and_items);
        }
        // 1. data
        z3::expr_vector data_items(z3_ctx());

        int i=0;
        int ct = 0;
        for (; i<body.num_args(); i++) {
                assert(body.arg(i).is_app());
                if (body.arg(i).decl().name().str() != "tobool") {
                        if (body.arg(i).decl().name().str() == "distinct") {
                            ct++;
                        } else {
                            data_items.push_back(body.arg(i));
                        }
                } else {
                        break;
                }
        }
        if (ct == 1) {
            m_ctx.division = "ACYCLIC-SL";
        } else if (ct == 2) {
            m_ctx.division = "ACYCLIC-DLL";
        }
        if (i==body.num_args()) {
                throw smt2exception("'tobool' is excepted in the recursive rule", line(), pos());
        }

        z3::expr data = mk_and(data_items);

        z3::expr ssep = body.arg(i).arg(0);
        if (ssep.arg(0).decl().name().str() != "pto") {
                throw smt2exception("'pto' is excepted first in the 'ssep'");
        }
        // 2. pto
        z3::expr pto = ssep.arg(0);
        // 3. rec app
        z3::expr_vector rec_apps(z3_ctx());
        for (int i=1; i<ssep.num_args(); i++) {
                if (ssep.arg(i).decl().name().str() == f_name) {
                        // rec app
                        rec_apps.push_back(ssep.arg(i));
                } else {
                        // non-rec app TODO.
                }
        }

        pred_rule rule(data, pto, rec_apps);
        return rule;
}




/**
 * check the $body sat the predicate constraint
 * (tospace (or (and ) (exists )))
 * @param body : the define-fun body
 * @return true : if sat
 */
void smt2parser::check_pred(z3::expr& body, predicate& pred) {
        std::string f_name = pred.get_pred_name();
        // TODO ..
        // 1. check pars

        // 2. check body
        assert(body.is_app() && body.decl().name().str() == "tospace");
        z3::expr exp_or = body.arg(0);
        if (!exp_or.is_app() && exp_or.decl().name().str() !="or") {
                throw smt2exception("(or ...) is excepted in define-fun body", line(), pos());
        }
        if (exp_or.num_args()<2) {
                throw smt2exception("at least 2 rules is excepted in define-fun body", line(), pos());
        }
        z3::expr base_rule = exp_or.arg(0);
        // 2.1 check base rule
        check_base_rule(base_rule);

        pred.add_base_rule(base_rule);

        // 2.2 check recursive rules
        for (int i=1; i<exp_or.num_args(); i++) {
                // check recursive rule
                z3::expr rule_exp = exp_or.arg(i);
                pred_rule rule = check_rec_rule(rule_exp, f_name);
                pred.add_rec_rule(rule);
        }
}


/**
 * make eq or ne expr according to kind
 * @param kind : the kind ==, !=
 * @param args : the args
 * @param line, pos: the expr position
 */
z3::expr smt2parser::make_eq(sl_decl_kind kind, z3::expr_vector& args) {
        if (!check_args_bool(args) && !check_args_numeral(args) && !check_args_userdef(args))
                throw smt2exception("invalid s-expression, unexpected args type", line(),  pos());
        if (args.size() != 2)
                throw smt2exception("invalid s-expression, expected 2 args", line(),  pos());
        switch(kind) {
        case SLRD_OP_EQ:
                if (args[0].to_string() == args[1].to_string()) {
                   return z3_ctx().bool_val(true);
                }
                return args[0] == args[1];
        case SLRD_OP_DISTINCT:
                if (args[0].to_string() == "nil") {
                        z3::expr nil = z3_ctx().constant("nil", args[1].get_sort());
                        return args[1] != nil;
                }

                if(args[1].to_string() == "nil") {
                        z3::expr nil = z3_ctx().constant("nil", args[0].get_sort());
                        return args[0] != nil;
                }
                return args[0] != args[1];
        default: break;
        }
        throw smt2exception("invalid s-expression, expected = or distinct", line(),  pos());
}
/**
 * make compare expr according to kind
 * @param kind : the kind >=, <=, >, <
 * @param args : the args
 * @param line, pos: the expr position
 */
z3::expr smt2parser::make_compare(sl_decl_kind kind, z3::expr_vector& args) {
        if (!check_args_numeral(args))
                throw smt2exception("invalid s-expression, unexpected args type", line(),  pos());
        if (args.size() != 2)
                throw smt2exception("invalid s-expression, expected 2 args", line(),  pos());
        switch(kind) {
        case SLRD_OP_LE:
                return args[0] <= args[1];
        case SLRD_OP_GE:
                return args[0] >= args[1];
        case SLRD_OP_LT:
                return args[0] < args[1];
        case SLRD_OP_GT:
                return args[0] > args[1];
        default: break;
        }
        throw smt2exception("invalid s-expression, expected <, <= , >, >=", line(),  pos());
}

/**
 * make arith expr according to kind
 * @param kind : the kind -, +, *
 * @param args : the args
 * @param line, pos: the expr position
 */
z3::expr smt2parser::make_arith(sl_decl_kind kind, z3::expr_vector& args) {
        if (!check_args_numeral(args))
                throw smt2exception("invalid s-expression, unexpected args type", line(),  pos());

        z3::expr res = args[0];
        switch(kind) {
        case SLRD_OP_SUB:
                if (args.size() == 1) {
                        // (- 1)
                        res = -args[0];
                } else {
                        // (- 1 1)
                        for (unsigned i=1; i<args.size(); i++) {
                                res = res - args[i];
                        }
                }
                return res;
        case SLRD_OP_ADD:
                return sum(args);
        case SLRD_OP_MUL:
                for (unsigned i=1; i<args.size(); i++) {
                        res = res * args[i];
                }
                return res;
        default: break;
        }
        throw smt2exception("invalid s-expression, expected +, -, *", line(), pos());
}
/**
 * make logic expr according to kind
 * @param kind : the kind and , or , not , =>
 * @param args : the args
 * @param line, pos: the expr position
 */
z3::expr smt2parser::make_logic(sl_decl_kind kind, z3::expr_vector& args){
        if (!check_args_bool(args))
                throw smt2exception("invalid s-expression, unexpected args type", line(),  pos());
        switch(kind) {
        case SLRD_OP_NOT:
                if (args.size() > 1)
                        throw smt2exception("invalid s-expression, expected 1 arg", line(),  pos());
                return !args[0];
        case SLRD_OP_IMPLIES:
                if (args.size() != 2)
                        throw smt2exception("invalid s-expression, expected 2 args", line(),  pos());
                return implies(args[0], args[1]);
        case SLRD_OP_AND:
                return mk_and(args);
        case SLRD_OP_OR:
                return mk_or(args);
        default: break;
        }
        throw smt2exception("invalid s-expression, expected and, or, not, =>", line(),  pos());
}

/**
 * make user_def function app expression
 * @param f : user_def function declaration
 * @param args : actual parameters
 * @return expr : f(args)
 */
z3::expr smt2parser::make_user_def(z3::func_decl f, z3::expr_vector& args) {
        int num = f.arity();
        if (f.arity() != args.size()) {
                std::string info = logger().string_format("invalid s-expression, for func '%s', given parameter number: %d, expected parameter number: %d", f.name().str().c_str(), args.size(), f.arity());
                throw smt2exception(info, line(), pos());
        }
        bool has_nil = false;
        for (int i=0; i<num; i++) {
                if (args[i].to_string() == "nil") {
                        has_nil = true;
                }
                if ( args[i].get_sort().to_string() != "Void" && f.domain(i).name() != args[i].get_sort().name()) {
                        // logger() << "domain: " << f.domain(i).name() << ", args: " << args[i].get_sort().name() << std::endl;
                        if (!(f.domain(i).to_string() == "Real" && args[i].is_numeral())){
                                std::string info = logger().string_format("invalid s-expression, for %s argument %d mismatch", f.name().str().c_str(), i);
                                throw smt2exception(info, line(),  pos());
                        }
                }
        }

        if (has_nil) {
                z3::expr_vector new_args(z3_ctx());
                for (int i=0; i<num; i++) {
                        if (args[i].to_string() == "nil") {
                                z3::sort nil_sort = f.domain(i);
                                new_args.push_back(z3_ctx().constant("nil", nil_sort));
                        } else {
                                new_args.push_back(args[i]);
                        }
                }
                return f(new_args);
        }
        return f(args);
}

/**
 * make ref app expression
 *  (ref (Fields A B) B (SetRef A))
 * @param: args : 2 parameters
 * @return : expr : the ref
 */
z3::expr smt2parser::make_ref(z3::expr_vector& args) {
        // check
        if (args.size() != 2) {
                std::string info = logger().string_format("invalid s-expression, 'ref' expected 2 parameters, given %d parameters", args.size());
                throw smt2exception(info, line(),  pos());
        }
        // (ref next Y)
        // domain
        z3::expr field = args[0]; // (next (Field T T))
        z3::sort field_sort = field.get_sort();
        if (!field_sort.is_array()) {
                std::string info = "invalid s-expression, 'ref' parameter 0 expected Field";
                throw smt2exception(info, line(),  pos());
        }
        z3::sort source_sort = field_sort.array_domain();
        z3::symbol source_sys = source_sort.name();
        if (m_sort_fields_table.find(source_sys) == m_sort_fields_table.end()) {
                std::string info =  logger().string_format("invalid s-expression, no declared fields for sort '%s'", source_sys.str().c_str());
                throw smt2exception(info, line(),  pos());
        }
        std::vector<z3::expr>& fields = m_sort_fields_table.find(source_sys)->second;

        if (!check_var_exist(fields, field)) {
                std::string info =  logger().string_format("invalid s-expression, no declared field '%s' for sort '%s'", field.to_string().c_str(), source_sys.str().c_str());
                throw smt2exception(info, line(),  pos());
        }
        z3::sort target_sort = field_sort.array_range();

        z3::expr target = args[1];
        // target.get_sort().is_arith();
        // target.is_arith();

        if (target.to_string() == "nil") {
                target = z3_ctx().constant("nil", target_sort);
        }

        if ((!target.get_sort().is_arith() || !target.is_arith()) && (target.get_sort().name() != target_sort.name())) {
                std::string info =  logger().string_format("invalid s-expression, field '%s' for sort '%s' expected point to sort '%s'", field.to_string().c_str(), source_sys.str().c_str(), target_sort.to_string().c_str());
                throw smt2exception(info, line(),  pos());
        }

        z3::sort void_sort = z3_ctx().uninterpreted_sort("Void");
        // range
        z3::sort range = z3_ctx().array_sort(source_sort, void_sort);
        z3::func_decl ref_f = z3_ctx().function("ref", field_sort, target_sort, range);
        // make_app
        return ref_f(field, target);
}

/**
 * make sref expresssion
 * (sref (ref ) (ref ) (SetRef A))
 * @param args : the parameters (ref ...)
 * @return expr : sref
 */
z3::expr smt2parser::make_sref(z3::expr_vector& args) {
        if (args.size() <2) {
                std::string info =  "invalid s-expression, for 'sref' >= 2 parameters is expected";
                throw smt2exception(info, line(),  pos());
        }
        if (!check_args_setref(args)) {
                std::string info =  "invalid s-expression, for 'sref' expected (ref A B) parameters";
                throw smt2exception(info, line(),  pos());
        }

        //
        z3::symbol sref_sys = z3_ctx().str_symbol("sref");
        z3::sort range = args[0].get_sort();
        z3::sort_vector domains(z3_ctx());
        for (unsigned i=0; i<args.size(); i++) {
                domains.push_back(range);
        }
        z3::func_decl sref_f = z3_ctx().function(sref_sys, domains, range);
        return sref_f(args);
}

/**
 * make pto expression
 * (pto E (SetRef E) (Space ))
 * @param args : the parameters E, (SetRef E)
 * @return expr : pto
 */
z3::expr smt2parser::make_pto(z3::expr_vector& args) {
        if (args.size() != 2) {
                std::string info =  "invalid s-expression, for 'pto'  2 parameters is expected";
                throw smt2exception(info, line(),  pos());
        }
        z3::expr source = args[0];
        z3::sort s_sort = source.get_sort();
        // source sort check
        if (s_sort.sort_kind() != Z3_UNINTERPRETED_SORT) {
                std::string info =  "invalid s-expression, for 'pto'  parameter 0 is unexpected sort, 'user def sort' is expcected";
                throw smt2exception(info, line(),  pos());
        }
        // dest sort check
        z3::expr dest = args[1];
        z3::sort d_sort = args[1].get_sort();

        if (!d_sort.is_array() || d_sort.array_range().to_string() != "Void" || d_sort.array_domain().to_string() != s_sort.to_string()) {
                std::string info = logger().string_format("invalid s-expression, for 'pto' parameter 1 is unexcepted sort, '(SetRef %s)' is excepted", s_sort.to_string().c_str());
                throw smt2exception(info, line(), pos());
        }

        z3::sort range = z3_ctx().uninterpreted_sort("Space");

        z3::func_decl pto_f = z3_ctx().function("pto", s_sort, d_sort, range);
        return pto_f(source, dest);
}

/**
 * make ssep expression
 * (ssep (Space ) (Space ) (Space ))
 * @param args : the parameters (Space)
 * @return expr : ssep
 */
z3::expr smt2parser::make_ssep(z3::expr_vector &args) {
        if (args.size() <2) {
                std::string info =  "invalid s-expression, for 'ssep' >= 2 parameters is expected";
                throw smt2exception(info, line(),  pos());
        }
        // check args are Space
        if (!check_args_space(args)) {
                std::string info =  "invalid s-expression, for 'ssep' expected (Space) parameters";
                throw smt2exception(info, line(),  pos());
        }
        z3::symbol ssep_sys = z3_ctx().str_symbol("ssep");
        z3::sort range = z3_ctx().uninterpreted_sort("Space");
        z3::sort_vector domains(z3_ctx());
        for (unsigned i=0; i<args.size(); i++) {
                domains.push_back(range);
        }
        z3::func_decl ssep_f = z3_ctx().function(ssep_sys, domains, range);
        return ssep_f(args);
}

/**
 * make exists (exists (parameters) (expr))
 * @
 */
z3::expr smt2parser::make_exists(z3::expr_vector& args) {
        // parameters
        z3::expr_vector pars(z3_ctx());
        int spos = m_var_frame_stack.back();
        int epos = m_sorted_var_stack.size();
        int num = epos - spos;
        assert(num > 0);
        for (int i=spos; i<epos; i++) {
                pars.push_back(m_sorted_var_stack[i]);
        }
        // erase variables
        m_sorted_var_stack.erase(m_sorted_var_stack.begin()+spos, m_sorted_var_stack.end());
        m_var_frame_stack.pop_back();
        //
        if (args.size() != 1 || !args[0].get_sort().is_bool()) {
                std::string info =  "invalid s-expression, the body of 'exists' expected only one bool expression";
                throw smt2exception(info, line(),  pos());
        }
        z3::expr body = args[0];

        return exists(pars, body);
}


/**
 * make expr according to the $func_sym
 * @param func_sym : function name
 * @param args : actual args
 * @param line, pos: current position
 * @return expr : app
 */
z3::expr smt2parser::make_app(z3::symbol& func_sym, z3::expr_vector& args){

        //
        std::string func = func_sym.str();
        std::map<std::string, sl_decl_kind> name_kind_table;

        name_kind_table["true"] = SLRD_OP_TRUE;
        name_kind_table["false"] = SLRD_OP_FALSE;
        name_kind_table["="] = SLRD_OP_EQ;
        name_kind_table["distinct"] = SLRD_OP_DISTINCT;
        name_kind_table["and"] = SLRD_OP_AND;
        name_kind_table["or"] = SLRD_OP_OR;
        name_kind_table["not"] = SLRD_OP_NOT;
        name_kind_table["=>"] = SLRD_OP_IMPLIES;
        name_kind_table["<="] = SLRD_OP_LE;
        name_kind_table[">="] = SLRD_OP_GE;
        name_kind_table["<"] = SLRD_OP_LT;
        name_kind_table[">"] = SLRD_OP_GT;
        name_kind_table["+"] = SLRD_OP_ADD;
        name_kind_table["-"] = SLRD_OP_SUB;
        name_kind_table["*"] = SLRD_OP_MUL;
        name_kind_table["emp"] = SLRD_OP_EMP;
        name_kind_table["ref"] = SLRD_OP_REF;
        name_kind_table["sref"] = SLRD_OP_SREF;
        name_kind_table["pto"] = SLRD_OP_PTO;
        name_kind_table["ssep"] = SLRD_OP_SSEP;
        name_kind_table["exists"] = SLRD_OP_EXISTS;
        name_kind_table["nil"] = SLRD_OP_NIL;



        if (name_kind_table.find(func) == name_kind_table.end()) {
                std::string info = logger().string_format("invalid s-expression, '%s' unexpected func name", func.c_str());
                throw smt2exception(info, line(),  pos());
        }


        sl_decl_kind kind = name_kind_table[func];

        switch(kind) {
        case SLRD_OP_TRUE:
                return z3_ctx().bool_val(true);
        case SLRD_OP_FALSE:
                return z3_ctx().bool_val(false);

        case SLRD_OP_AND:
        case SLRD_OP_OR:
        case SLRD_OP_NOT:
        case SLRD_OP_IMPLIES:
                return make_logic(kind, args);
        case SLRD_OP_EQ:
        case SLRD_OP_DISTINCT:
                return make_eq(kind, args);
        case SLRD_OP_GE:
        case SLRD_OP_LE:
        case SLRD_OP_GT:
        case SLRD_OP_LT:
                return make_compare(kind, args);
        case SLRD_OP_ADD:
        case SLRD_OP_SUB:
        case SLRD_OP_MUL:
                return make_arith(kind, args);
        case SLRD_OP_EMP: {
                // emp
                z3::sort space = z3_ctx().uninterpreted_sort("Space");
                return z3_ctx().constant("emp", space);
        }
        case SLRD_OP_REF:
                return make_ref(args);
        case SLRD_OP_SREF:
                return make_sref(args);
        case SLRD_OP_PTO:
                return make_pto(args);
        case SLRD_OP_SSEP:
                return make_ssep(args);
        case SLRD_OP_EXISTS:
                return make_exists(args);
        case SLRD_OP_NIL: {
                // nil
                z3::sort void_sort = z3_ctx().uninterpreted_sort("Void");
                return z3_ctx().constant("nil", void_sort);
        }
        default: {
                std::string info = logger().string_format("invalid s-expression, '%s' unsupported func name", func.c_str());
                throw smt2exception(info, line(),  pos());
        }
        }
}


/**
 * parse expr (app_name args)
 */
void smt2parser::parse_expr() {
        logger().log_out_ln("parse expr start.");
        unsigned stack_pos  = m_expr_stack.size();
        unsigned num_frames = 0;


        /*
          logger() << "varstack\n";
          for (int i=0; i<m_sorted_var_stack.size(); i++) {
          logger() << "var:"<<i<<" :" << m_sorted_var_stack[i] << std::endl;
          }
          logger() << "varstack\n";
        */

        do {
                next();
                switch (curr()) {
                case smt2scanner::LEFT_PAREN: {
                        m_expr_frame_stack.push_back(m_expr_stack.size());
                        num_frames++;
                        break;
                }
                case smt2scanner::RIGHT_PAREN: {
                        if (num_frames == 0)
                                throw smt2exception("invalid s-expression, unexpected ')'", line(),  pos());
                        num_frames--;
                        unsigned spos = m_expr_frame_stack.back();
                        unsigned epos = m_expr_stack.size();
                        assert(epos >= spos);
                        unsigned num  = epos - spos;
                        z3::expr exp = z3_ctx().int_val(1);

                        // make apply
                        // 1. get fun_declare
                        if(m_symbol_stack.size()<=0) {
                                throw smt2exception("invalid empty s-expression", line(),  pos());
                        }
                        z3::symbol func_sym = m_symbol_stack.back();

                        if (num == 0) {
                                std::string info = logger().string_format("invalid s-expression, for %s arguments missing", func_sym.str().c_str());
                                throw smt2exception(info, line(),  pos());
                        }
                        // 2. get arguments
                        z3::expr_vector args(z3_ctx());

                        for (int i=spos; i<epos; i++) {
                                args.push_back(m_expr_stack[i]);
                        }

                        if ( m_builtin_func_table.find(func_sym) != m_builtin_func_table.end()) {
                                z3::func_decl fun = m_builtin_func_table.find(func_sym)->second;
                                // make user-def app
                                exp = make_user_def(fun, args);
                        } else {
                                // make built-in app
                                // logger() << "built-in function " << func_sym << std::endl;
                                exp = make_app(func_sym, args);
                        }

                        m_symbol_stack.pop_back();

                        m_expr_stack.erase(m_expr_stack.begin()+spos, m_expr_stack.end());

                        m_expr_stack.push_back(exp);


                        m_expr_frame_stack.pop_back();
                        break;
                }
                case smt2scanner::SYMBOL_TOKEN:{
                        z3::symbol sym = curr_id();

                        int index = check_var_exist(m_sorted_var_stack, sym);
                        // if (m_symbol_var_table.find(sym) != m_symbol_var_table.end()) {
                        if (index > -1){
                                // variable
                                z3::expr var = m_sorted_var_stack[index];
                                m_expr_stack.push_back(var);
                        } else if(m_builtin_func_table.find(sym) != m_builtin_func_table.end()){
                                // 0-arity func, constant
                                z3::func_decl f = m_builtin_func_table.find(sym)->second;
                                if (f.arity()==0) {
                                        z3::expr_vector args(z3_ctx());
                                        z3::expr val = make_app(sym, args);
                                        m_expr_stack.push_back(val);
                                } else {
                                        // user def func
                                        m_symbol_stack.push_back(sym);
                                }
                        } else {
                                if (sym == m_exists) {
                                        bool empty = parse_parameters();
                                        if (empty) {
                                                throw smt2exception("invalid s-expression, for 'exits' unexpected empty arguments", line(),  pos());
                                        }
                                }
                                m_symbol_stack.push_back(sym);
                        }
                        break;
                }
                case smt2scanner::KEYWORD_TOKEN:
                        // std::cout << "\n keyword ";
                        // :sorts ...
                        break;
                case smt2scanner::STRING_TOKEN:
                        m_expr_stack.push_back(z3_ctx().string_val(m_scanner.get_string().c_str()));
                        // std::cout << "\n string: " << m_scanner.get_string();
                        break;
                case smt2scanner::INT_TOKEN:
                case smt2scanner::FLOAT_TOKEN:
                case smt2scanner::BV_TOKEN:
                        // std::cout << "\n number:" << curr_numeral();
                        m_expr_stack.push_back(curr_numeral());
                        break;
                case smt2scanner::EOF_TOKEN:
                        throw smt2exception("invalid s-expression, unexpected end of file", line(),  pos());
                        break;
                default:
                        throw smt2exception("invalid s-expression, unexpected input", line(),  pos());
                        break;
                }
        } while (num_frames > 0);
        assert(m_expr_stack.size() == stack_pos + 1);

        logger().log_format("expr stack size: %d\n", m_expr_stack.size());
        logger().log_format("expr frame stack size: %d\n", m_expr_frame_stack.size());
        logger() << "the last expr:\n" << m_expr_stack.back() << std::endl;
        logger().log_out_ln("parse expr end.");
}



/**
 * parse sort
 *     : (sort)  (Field 2)=>(Field Int Bool)
 * @return sort:
 */
z3::sort smt2parser::parse_sort() {
        next();
        if (curr_is_identifier()) {
                z3::symbol sort_s = curr_id();
                if (m_builtin_sort_table.find(sort_s) != m_builtin_sort_table.end()) {
                        return m_builtin_sort_table.find(sort_s)->second.first;
                } else {
                        throw smt2exception("invalid parametes, unexpected sort name", line(),  pos());
                }
        }

        if (curr_is_lparen())  {
                next();
                check_identifier("invalid parametes, unsupported sort!");
                z3::symbol par_sort_s = curr_id();
                if (m_builtin_sort_table.find(par_sort_s) != m_builtin_sort_table.end()) {
                        unsigned num_par = m_builtin_sort_table.find(par_sort_s)->second.second;
                        z3::sort res_sort = z3_ctx().bool_sort();
                        if (num_par == 1) {
                                // set_ref
                                next();
                                check_identifier("invalid parametes, unsupported sort!");
                                z3::symbol domain_sy = curr_id();
                                if (m_builtin_sort_table.find(domain_sy) == m_builtin_sort_table.end()) {
                                        throw smt2exception("invalid parametes, unsupported sort", line(),  pos());
                                }
                                z3::sort domain_sort = m_builtin_sort_table.find(domain_sy)->second.first;
                                z3::symbol void_sy = z3_ctx().str_symbol("Void");
                                z3::sort void_sort = m_builtin_sort_table.find(void_sy)->second.first;
                                z3::sort set_ref_sort = z3_ctx().array_sort(domain_sort, void_sort);
                                res_sort = set_ref_sort;
                                // return set_ref_sort;
                        } else if (num_par == 2) {
                                // field
                                next();
                                check_identifier("invalid parametes, unsupported sort!");
                                z3::symbol domain_sy = curr_id();
                                if (m_builtin_sort_table.find(domain_sy) == m_builtin_sort_table.end()) {
                                        throw smt2exception("invalid parametes, unsupported sort", line(),  pos());
                                }
                                z3::sort domain_sort = m_builtin_sort_table.find(domain_sy)->second.first;
                                next();
                                check_identifier("invalid parametes, unsupported sort!");
                                z3::symbol range_sy = curr_id();
                                if (m_builtin_sort_table.find(range_sy) == m_builtin_sort_table.end()) {
                                        throw smt2exception("invalid parametes, unsupported sort", line(),  pos());
                                }
                                z3::sort range_sort = m_builtin_sort_table.find(range_sy)->second.first;
                                z3::sort field_sort = z3_ctx().array_sort(domain_sort, range_sort);

                                res_sort = field_sort;
                                //return field_sort;
                        } else {
                                throw smt2exception("invalid parametes, unsupported sort", line(),  pos());
                        }

                        next();
                        if (curr_is_rparen()) {
                                return res_sort;
                        }
                        throw smt2exception("invalid sort, expected ')'", line(),  pos());
                }
                throw smt2exception("invalid parametes, unexpected sort name", line(),  pos());
        }
}

/**
 * parse parameters ((var sort) ())
 *     the parameters as variables are pushed in the $m_sorted_var_stack
 *     the $m_var_frame_stack.back() stores the start position
 * @return empty : if empty, return true
 *                 otherwise, return false
 */
bool smt2parser::parse_parameters() {
        unsigned stack_pos  = m_sorted_var_stack.size();
        int num_frames = 0;
        z3::expr var = z3_ctx().bool_val(true);
        bool empty = true;
        m_var_frame_stack.push_back(m_sorted_var_stack.size());
        do {
                next();
                switch (curr()) {
                case smt2scanner::LEFT_PAREN: {
                        num_frames++;
                        break;
                }
                case smt2scanner::RIGHT_PAREN:{
                        // check empty
                        num_frames--;
                        if (!empty) {
                                // push var
                                if (num_frames > 0) {
                                        m_sorted_var_stack.push_back(var);
                                }
                        } else {
                                m_var_frame_stack.pop_back();
                        }
                        break;
                }
                case smt2scanner::SYMBOL_TOKEN:{
                        if (num_frames == 1) {
                                throw smt2exception("invalid parameters, excepted '('", line(), pos());
                        }
                        empty = false;
                        z3::symbol var_s = curr_id();
                        z3::sort var_sort = parse_sort();
                        var = z3_ctx().constant(var_s, var_sort);
                        break;
                }
                default:
                        throw smt2exception("invalid parametess, unexpected variable name", line(),  pos());
                }
        } while(num_frames > 0);
        logger() << "parse parameters end.\n";
        return empty;
}



/**
 * parse cmd (set_logic LOGIC_NAME)
 * set the m_logic
 * do some init according to the logic
 */
void smt2parser::parse_set_logic() {
        logger() << "parse set logic start. \n";
        assert(curr_is_identifier());
        assert(m_set_logic ==  curr_id());
        next();
        check_identifier("invalid logic name, symbol expected");
        z3::symbol logic_id = curr_id();

        logger() << "parse logic :" << logic_id << std::endl;

        next();
        if (!curr_is_rparen())
                check_identifier("invalid set-logic, ')' expected");

        m_logic_name = logic_id.str();


        logger() << "parse set logic end. \n";

}

/**
 * parse cmd (declare-sort SORTNAE PARAMETER_NUM)
 * push the sort into $m_builtin_sort_table
 */
void smt2parser::parse_declare_sort() {
        logger() << "parse declare sort start. \n";
        assert(curr_is_identifier());
        assert(m_declare_sort ==  curr_id());
        next();
        check_identifier("invalid sort name, symbol expected");
        z3::symbol sort_name = curr_id();
        next();
        check_int("invalid declare-sort parameter, integer expcected");
        z3::expr num = curr_numeral();
        next();
        check_rparen("invalid declare-sort, ')' expcected");

        // insert new sort
        z3::sort sort_user = z3_ctx().uninterpreted_sort(sort_name);
        unsigned num_par = num.get_numeral_uint();

        logger() << "declare-sort: " << sort_user <<" " <<num_par << std::endl;
        if (m_builtin_sort_table.find(sort_name) != m_builtin_sort_table.end()) {
                std::string info = logger().string_format("redeclare sort for %s, unexpected sort name", logger().value_to_str(sort_name).c_str());
                throw smt2exception(info, line(),  pos());
        }
        m_builtin_sort_table.insert(std::pair<z3::symbol, std::pair<z3::sort, unsigned> >(sort_name, std::pair<z3::sort, unsigned>(sort_user, num_par)));
        // std::cout << "parse declare sort end.\n";
        logger() << "parse declare sort end. \n";
}

/**
 * parse cmd (declare-fun FUN_NAME (parameters) SORT_NAME)
 * parse fields variable and variable, push them into $m_sort_fields_table and $m_sorted_var_stack respectively
 * TODO: process the fun_declare
 */
void smt2parser::parse_declare_fun() {

        logger() << "parse declare-fun start.\n";

        assert(curr_is_identifier());
        assert(m_declare_fun ==  curr_id());
        next(); // fun_name
        check_identifier("invalid function name, symbol expcected");

        z3::symbol fun_name = curr_id();

        logger() << "parse fun name: " << fun_name << std::endl;

        // next(); // (
        bool empty = parse_parameters();

        logger() << "parser parameters, empty: " << empty << std::endl;

        z3::sort range = parse_sort();
        next(); // )
        check_rparen("invalid declare-fun, ')' expcected");

        if (empty) {
                // insert var
                z3::expr var_cons = z3_ctx().constant(fun_name, range);
                if (range.is_array()) {
                        // set_ref or field
                        if (range.array_range().to_string() == "Void") {
                                // set_ref NONE
                        } else {
                                z3::sort main_sort = range.array_domain();
                                if (main_sort.sort_kind() != Z3_UNINTERPRETED_SORT) {
                                        std::string info = logger().string_format("invalid field for sort '%s', unexpected sort name.", logger().value_to_str(main_sort).c_str());
                                        throw smt2exception(info, line(),  pos());
                                }
                                z3::sort field_sort = range.array_range();

                                if (m_sort_fields_table.find(main_sort.name() ) == m_sort_fields_table.end()) {
                                        // insert
                                        std::vector<z3::expr> fields;
                                        fields.push_back(var_cons);
                                        m_sort_fields_table.insert(std::pair<z3::symbol, std::vector<z3::expr>>(main_sort.name(), fields));
                                } else {
                                        // find and insert
                                        std::vector<z3::expr>& fields = m_sort_fields_table.find(main_sort.name())->second;
                                        if (check_var_exist(fields, var_cons)) {
                                                // std::cout << logger().value_to_str(main_sort) << std::endl;
                                                std::string info = logger().string_format("redeclare field '%s' for sort '%s', unexpected field name.", logger().value_to_str(var_cons).c_str(), logger().value_to_str(main_sort).c_str());

                                                throw smt2exception(info, line(),  pos());
                                        }
                                        fields.push_back(var_cons);
                                        // std::cout << "fields: " << fields.size() << std::endl;
                                }
                        }
                        logger() << "new filed: " << var_cons << std::endl;
                } else {
                        // var
                        if (check_var_exist(m_sorted_var_stack, var_cons)) {
                                std::string info = logger().string_format("redeclare variable '%s', unexpected sort name.", logger().value_to_str(var_cons).c_str());
                                throw smt2exception(info, line(),  pos());
                        }
                }
                // insert
                m_sorted_var_stack.push_back(var_cons);
                logger() << "new variable: " << var_cons << std::endl;
        } else {
                // fun-declare
        }

        logger() << "parse declare-fun end.\n";
}

/**
 * parse cmd (define-func FUN_NAME (parameters) SORT_NAME (body))
 * parse fun_declare, push it into $m_builtin_func_table [UNDO]
 * parse body, translate it into predicate define [UNDO]
 */
void smt2parser::parse_define_fun() {
        logger() << "parse define-fun start.\n";
        assert(curr_is_identifier());
        assert(m_define_fun ==  curr_id());
        next(); // fun_name
        check_identifier("invalid function/constant definition, symbol expected");
        z3::symbol fun_name = curr_id();

        logger() << "parse fun name: " << fun_name << std::endl;

        bool empty = parse_parameters();

        if (empty) {
                // error
                std::string info = logger().string_format("invalid define-fun for %s, unexpected empty parameter.", fun_name.str().c_str());
                throw smt2exception(info, line(),  pos());
        }



        // parse the parameters
        unsigned spos = m_var_frame_stack.back();
        unsigned epos = m_sorted_var_stack.size();
        unsigned num = epos - spos;

        z3::expr_vector args(z3_ctx());
        z3::sort_vector domain(z3_ctx());

        for (unsigned i=spos; i<epos; i++) {
                args.push_back(m_sorted_var_stack[i]);
                domain.push_back(m_sorted_var_stack[i].get_sort());
        }


        logger()  << "parse the number of parameters : " << num << std::endl;

        z3::sort range = parse_sort();

        logger() << "parse range type: " << range << std::endl;

        // make func_decl
        z3::func_decl fun = z3_ctx().function(fun_name, domain, range);

        logger() << "make func decl: " << fun << std::endl;

        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(fun_name, fun));

        // parse body -> rules
        parse_expr();
        // closed paren
        next();
        check_rparen("invalid define-fun, excepted ')'");

        z3::expr body = m_expr_stack.back();

        m_expr_stack.pop_back();

        // new predicate
        // args, base_rule, rec_rules
        z3::expr null(z3_ctx());
        predicate pred(fun, args, null);
        check_pred(body, pred);

        m_ctx.add_predicate(pred);

        // std::cout << pred << std::endl;

        m_sorted_var_stack.erase(m_sorted_var_stack.begin()+spos, m_sorted_var_stack.end());
        m_var_frame_stack.pop_back();
        logger() << "parse define-fun end.";

}

/**
 * parse (asssert formula)
 *
 */
void smt2parser::parse_assert() {
        logger() << "parse assert start.\n";

        assert(curr_is_identifier());
        assert(m_assert ==  curr_id());
        parse_expr();

        //
        z3::expr formula = m_expr_stack.back();
        if (m_ctx.is_no_formula()){
                m_ctx.set_negf(formula);
        } else if (m_ctx.is_sat()) {
                if (formula.decl().name().str() != "not") {
                        std::string info = "invalid assert, the second formula must be negative.";
                        throw smt2exception(info, line(),  pos());
                }
                m_ctx.set_posf(formula.arg(0));
        }
        else {
                std::string info = "invalid assert, support at most 2 assertions.";
                throw smt2exception(info, line(),  pos());
        }

        m_expr_stack.pop_back();

        next();
        check_rparen("invalid assert, excepted ')'");

        logger() << "parse assert end.\n";
}

/**
 * parse (check-sat)
 */
void smt2parser::parse_check_sat() {
        assert(curr_is_identifier());
        assert(m_check_sat ==  curr_id());

        next();
        check_rparen("invalid check-sat, excepted ')'");

        // logger() << "solve the sat ...\n";
        if (sol == NULL) {
                sol = solverfactory::get_solver(m_ctx);
        }

        // std::cout << "DIVISION: " << m_ctx.division << std::endl;
        // exit(-1);
        if (sol != NULL) {
                sol->solve();
        }
}

void smt2parser::parse_set_info() {
    assert(curr_is_identifier());
    assert(m_set_info ==  curr_id());
    next();
    next();
    next();
    check_rparen("invalid get-model, excepted ')'");
}


/**
 * parse (get-model)
 */
void smt2parser::parse_get_model() {
        assert(curr_is_identifier());
        assert(m_get_model ==  curr_id());
        next();
        check_rparen("invalid get-model, excepted ')'");
        if (sol != NULL&& m_ctx.is_sat()  && sol->check_sat() != z3::unsat) {
                //std::cout <<"Model:\n" <<;
                sol->get_model();
        }
}

void smt2parser::parse_cmd() {

        check_lparen_next("invalid command, '(' expected.");
        check_identifier("invalid command, symbol expected");
        z3::symbol s = curr_id();

        if (s == m_set_logic) {
                parse_set_logic();
                return;
        }

        if (s == m_declare_sort) {
                parse_declare_sort();
                return;
        }

        if (s == m_declare_fun) {
                parse_declare_fun();
                return;
        }

        if (s == m_define_fun) {
                parse_define_fun();
                return;
        }

        if (s == m_assert) {
                parse_assert();
                return;
        }

        if (s == m_check_sat) {
                parse_check_sat();
                return;
        }

        if (s == m_get_model) {
                parse_get_model();
                return;
        }

        if (s == m_set_info) {
            parse_set_info();
        }

}

/**
 * parse the file
 */
bool smt2parser::operator()() {
        m_num_bindings    = 0;
        bool found_errors = false;
        try {
                scan_core();
        }
        catch (smt2exception & ex) {
                error(ex.get_msg().c_str());
                if (!sync_after_error())
                        return false;
                found_errors = true;
        }


        while (true) {
                try {
                        m_num_open_paren = 0;
                        while (true) {
                                switch (curr()) {
                                case smt2scanner::LEFT_PAREN:
                                        parse_cmd();
                                        break;
                                case smt2scanner::EOF_TOKEN:
                                        return !found_errors;
                                default:
                                        throw smt2exception("invalid command, '(' expected");
                                        break;
                                }
                                next();
                        }
                }
                catch (smt2exception & ex) {
                        // error(ex.get_msg().c_str());
                        // std::cout << ex.get_msg() << std::endl;
                        // break;
                        throw ex;
                }
        }
}


/**
 * init the theory, add some built-in sorts and funs
 */
void smt2parser::init_theory() {
        // built-in sorts

        z3::symbol bool_sym = z3_ctx().str_symbol("Bool");
        z3::sort B = z3_ctx().bool_sort();
        z3::symbol int_sym = z3_ctx().str_symbol("Int");
        z3::sort I = z3_ctx().int_sort();
        z3::symbol rat_sym = z3_ctx().str_symbol("Rat");
        z3::sort R = z3_ctx().real_sort();

        z3::symbol void_sym = z3_ctx().str_symbol("Void");
        z3::sort V = z3_ctx().uninterpreted_sort("Void");

        z3::symbol field_sym = z3_ctx().str_symbol("Field");
        z3::sort F =  z3_ctx().uninterpreted_sort("Field");

        z3::symbol set_ref_sym = z3_ctx().str_symbol("SetRef");
        z3::sort S =  z3_ctx().uninterpreted_sort("SetRef");

        z3::symbol space_sym = z3_ctx().str_symbol("Space");
        z3::sort SP =  z3_ctx().uninterpreted_sort("Space");

        z3::symbol set_int_sym = z3_ctx().str_symbol("SetInt");
        z3::sort set_int = z3_ctx().uninterpreted_sort("SetInt");

        // insert builtin sort
        m_builtin_sort_table.insert(std::pair<z3::symbol, std::pair<z3::sort, unsigned> >(bool_sym, std::pair<z3::sort, unsigned>(B, 0)));
        m_builtin_sort_table.insert(std::pair<z3::symbol, std::pair<z3::sort, unsigned> >(int_sym, std::pair<z3::sort, unsigned>(I, 0)));
        m_builtin_sort_table.insert(std::pair<z3::symbol, std::pair<z3::sort, unsigned> >(rat_sym, std::pair<z3::sort, unsigned>(R, 0)));
        m_builtin_sort_table.insert(std::pair<z3::symbol, std::pair<z3::sort, unsigned> >(void_sym, std::pair<z3::sort, unsigned>(V, 0)));
        m_builtin_sort_table.insert(std::pair<z3::symbol, std::pair<z3::sort, unsigned> >(field_sym, std::pair<z3::sort, unsigned>(F, 2)));
        m_builtin_sort_table.insert(std::pair<z3::symbol, std::pair<z3::sort, unsigned> >(set_ref_sym, std::pair<z3::sort, unsigned>(S, 1)));
        m_builtin_sort_table.insert(std::pair<z3::symbol, std::pair<z3::sort, unsigned> >(space_sym, std::pair<z3::sort, unsigned>(SP, 0)));
        m_builtin_sort_table.insert(std::pair<z3::symbol, std::pair<z3::sort, unsigned> >(set_int_sym, std::pair<z3::sort, unsigned>(set_int, 0)));

        // built-in funs

        z3::func_decl true_f = z3_ctx().function("true", 0, 0, B);
        z3::symbol true_s =  z3_ctx().str_symbol("true");

        z3::func_decl false_f = z3_ctx().function("false", 0, 0, B);
        z3::symbol false_s =  z3_ctx().str_symbol("false");

        z3::func_decl emp_f = z3_ctx().function("emp", 0, 0, B);
        z3::symbol emp_s =  z3_ctx().str_symbol("emp");

        z3::func_decl nil_f = z3_ctx().function("nil", 0, 0, V);
        z3::symbol nil_s =  z3_ctx().str_symbol("nil");

        z3::func_decl tobool_f = z3_ctx().function("tobool", SP, B);
        z3::symbol tobool_s = z3_ctx().str_symbol("tobool");

        z3::symbol tospace_s = z3_ctx().str_symbol("tospace");
        z3::func_decl tospace_f = z3_ctx().function("tospace", B, SP);

        z3::func_decl emptyset = z3_ctx().function("emptyset", 0, 0, set_int);
        z3::symbol emptyset_s = z3_ctx().str_symbol("emptyset");

        z3::func_decl set = z3_ctx().function("set", I, set_int);
        z3::symbol set_s = z3_ctx().str_symbol("set");

        z3::func_decl setunion = z3_ctx().function("setunion", set_int, set_int, set_int);
        z3::symbol setunion_s = z3_ctx().str_symbol("setunion");

        z3::func_decl setintersect = z3_ctx().function("setintersect", set_int, set_int, set_int);
        z3::symbol setintersect_s = z3_ctx().str_symbol("setintersect");

        z3::func_decl setminus = z3_ctx().function("setminus", set_int, set_int, set_int);
        z3::symbol setminus_s = z3_ctx().str_symbol("setminus");

        z3::func_decl subset = z3_ctx().function("subset", set_int, set_int, B);
        z3::symbol subset_s = z3_ctx().str_symbol("subset");

        z3::func_decl psubset = z3_ctx().function("psubset", set_int, set_int, B);
        z3::symbol psubset_s = z3_ctx().str_symbol("psubset");

        z3::func_decl equalset = z3_ctx().function("equalset", set_int, set_int, B);
        z3::symbol equalset_s = z3_ctx().str_symbol("equalset");

        z3::func_decl belongsto = z3_ctx().function("belongsto", I, set_int, B);
        z3::symbol belongsto_s = z3_ctx().str_symbol("belongsto");

        z3::func_decl min = z3_ctx().function("min", set_int, I);
        z3::symbol min_s = z3_ctx().str_symbol("min");

        z3::func_decl max = z3_ctx().function("max", set_int, I);
        z3::symbol max_s = z3_ctx().str_symbol("max");


        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(true_s, true_f));
        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(false_s, false_f));
        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(emp_s, emp_f));
        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(nil_s, nil_f));
        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(tospace_s, tospace_f));
        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(tobool_s, tobool_f));
        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(emptyset_s, emptyset));
        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(set_s, set));
        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(setunion_s, setunion));
        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(setintersect_s, setintersect));
        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(setminus_s, setminus));
        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(subset_s, subset));
        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(psubset_s, psubset));
        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(equalset_s, equalset));
        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(belongsto_s, belongsto));
        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(min_s, min));
        m_builtin_func_table.insert(std::pair<z3::symbol, z3::func_decl>(max_s, max));

}


smt2parser::smt2parser(smt2context & ctx, std::istream & is):
        m_ctx(ctx),
        sol(0),
        m_scanner(ctx, is),
        m_curr(smt2scanner::NULL_TOKEN),
        m_exists(ctx.z3_context().str_symbol("exists")),
        m_assert(ctx.z3_context().str_symbol("assert")),
        m_check_sat(ctx.z3_context().str_symbol("check-sat")),
        m_get_model(ctx.z3_context().str_symbol("get-model")),
        m_define_fun(ctx.z3_context().str_symbol("define-fun")),
        m_declare_fun(ctx.z3_context().str_symbol("declare-fun")),
        m_declare_sort(ctx.z3_context().str_symbol("declare-sort")),
        m_set_logic(ctx.z3_context().str_symbol("set-logic")),
        m_set_info(ctx.z3_context().str_symbol("set-info")),
        m_num_open_paren(0)
{
        init_theory();
}

smt2parser::~smt2parser() {
        if (sol != NULL) {
                delete sol;
        }
}
