#include <iostream>
#include <sstream>
#include <fstream>
#include "smt2scanner.h"
#include "smt2parser.h"


using namespace std;

string token_str[] = { "NULL_TOKEN",
                       "LEFT_PAREN",
                       "RIGHT_PAREN",
                       "KEYWORD_TOKEN",
                       "SYMBOL_TOKEN",
                       "STRING_TOKEN",
                       "INT_TOKEN",
                       "BV_TOKEN",
                       "FLOAT_TOKEN",
                       "EOF_TOKEN"};


void test(std::string file_name) {
    try {
        fstream f;
        f.open(file_name);
        z3::context ctx;
        smt2context m_ctx(ctx, "log");
        //m_ctx.logger().disable();
        smt2parser parser(m_ctx, f);
        parser();
        f.close();
        // std::cout << "formula: " << m_ctx.get_negf() << std::endl;
    } catch(const smt2exception& e) {
        cout << e.get_msg() << endl;
    }
}


int main(int argc, char *argv[])
{
    std::string file_name = "samples/QF_SLID_LC_ENTL/dll-entl-06.smt";
    if (argc < 2) {
      std::cout << "Using: ComSpen input_file\n";
      exit(-1);
    }
    if (argc > 1) {
        file_name = argv[1];
    }
    test(file_name);
    return 0;
}
