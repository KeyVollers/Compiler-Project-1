#include <iostream>

#include "scanner.hpp"

const char *to_string(TokenType tt) {
    switch(tt) {
        case TokenType::ERROR: return "ERROR";
        case TokenType::TOK_EOF: return "EOF";
        case TokenType::IDENT: return "IDENT";
        case TokenType::INTEGER: return "INTEGER";

        case TokenType::KW_TRUE: return "'true'";
        case TokenType::KW_FALSE: return "'false'";
        case TokenType::KW_IF: return "'if'";
        case TokenType::KW_THEN: return "'then'";
        case TokenType::KW_ELSE: return "'else'";
        case TokenType::KW_FI: return "'fi'";
        case TokenType::KW_WHILE: return "'while'";
        case TokenType::KW_DO: return "'do'";
        case TokenType::KW_OD: return "'od'";
        case TokenType::KW_SKIP: return "'skip'";

        case TokenType::OP_ADD: return "+";
        case TokenType::OP_SUB: return "-";
        case TokenType::OP_MUL: return "*";

        case TokenType::OP_NOT: return "'not'";
        case TokenType::OP_AND: return "'and'";
        case TokenType::OP_OR: return "'or'";

        case TokenType::OP_EQ: return "'='";
        case TokenType::OP_LT: return "'<'";
        case TokenType::OP_GT: return "'>'";
        case TokenType::OP_LE: return "'<='";
        case TokenType::OP_GE: return "'>='";

        case TokenType::OP_ASSIGN: return "':='";
        case TokenType::OP_SEMI: return "';'";
        case TokenType::OP_LBRACK: return "'['";
        case TokenType::OP_RBRACK: return "']'";

        default: return "<unknown>";
    }
}

void print_token(const Token &tok) {
    std::cout << "Token("
        << to_string(tok.type) << ", "
        << "("
            << tok.loc.line << ":"
            << tok.loc.col << "|"
            << tok.loc.pos << "), "
        << tok.value << ")" << std::endl;
}

void test_scanner(const char *input) {
    std::cout << "Test scanner(\"" << input << "\")" << std::endl;
    Scanner s = Scanner(input);
    Token tok = s.next_token();
    while(tok.type != TokenType::TOK_EOF && tok.type != TokenType::ERROR) {
        print_token(tok);
        tok = s.next_token();
    }
}

int main() {
    test_scanner("1 2 3");
    test_scanner("if true then\n  skip\nelse\n  10; x := 10\nfi");
    return 0;
}
