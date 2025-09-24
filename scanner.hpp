#include <cstdint>

/**
 * Stream location metadata.
 */
struct Loc {
    int line, col, pos;
};

/**
 * Type of a token.
 */
enum class TokenType {
    ERROR = 0,
    TOK_EOF = 1,
    IDENT = 0x10,
    INTEGER = 0x11,

    KEYWORD = 0x20,
    KW_TRUE = KEYWORD | 0,
    KW_FALSE = KEYWORD | 1,
    KW_IF = KEYWORD | 2,
    KW_THEN = KEYWORD | 3,
    KW_ELSE = KEYWORD | 4,
    KW_FI = KEYWORD | 5,
    KW_WHILE = KEYWORD | 6,
    KW_DO = KEYWORD | 7,
    KW_OD = KEYWORD | 8,
    KW_SKIP = KEYWORD | 9,

    ARITH_OP = 0x30,
    OP_ADD = ARITH_OP | 0,
    OP_SUB = ARITH_OP | 1,
    OP_MUL = ARITH_OP | 2,

    BOOL_OP = 0x40,
    OP_NOT = BOOL_OP | 0,
    OP_AND = BOOL_OP | 1,
    OP_OR = BOOL_OP | 2,

    REL_OP = 0x50,
    OP_EQ = REL_OP | 0,
    OP_LT = REL_OP | 1,
    OP_GT = REL_OP | 2,
    OP_LE = REL_OP | 3,
    OP_GE = REL_OP | 4,

    PUNC_OP = 0x60,
    OP_ASSIGN = PUNC_OP | 0,
    OP_SEMI = PUNC_OP | 1,
    OP_LBRACK = PUNC_OP | 2,
    OP_RBRACK = PUNC_OP | 3
};

/**
 * Language token.
 */
struct Token {
    Loc loc;
    int length;
    TokenType type;
    int64_t value;

    Token(Loc loc, int length, TokenType type, int64_t value = 0)
        : loc(loc), length(length), type(type), value(value) {}
};

class Scanner {
    private:
        const char *cur;

        void advance(int by=1);
        void newline();
        void consume_space();
        void consume_comment();

    public:
        Loc loc;

        Scanner(const char *input);

        Token next_token();
};
