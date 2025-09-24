#include <cctype>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "scanner.hpp"

Scanner::Scanner(const char *input) : cur(input), loc {1, 0, 0} {}

/**
 * Advance the iterator by the given number ignoring the line.
 */
void Scanner::advance(int by) {
    cur += by;
    loc.pos += by;
    loc.col += by;
}

/**
 * Advance the iterator to the next line.
 */
void Scanner::newline() {
    ++loc.line;
    loc.col = 0;
}

/**
 * Consume any leading whitespace, advancing the iterator.
 */
void Scanner::consume_space() {
    while(*cur) {
        switch(*cur) {
            case '\n':
                newline();
                break;

            case '\r':
            case '\f': case '\v':
            case '\t': case ' ':
                ++loc.col;
                break;

            default: return;
        }
        ++loc.pos;
        ++cur;
    }
}

/**
* Attempt to consume a comment. Stops at the end of the input if the comment
* is not closed.
*/
void Scanner::consume_comment() {
    // Single-line comment
    if(cur[0] == '-' && cur[1] == '-') {
        advance(2);
        while(*cur) {
            if(cur[0] == '\n') {
                newline();
                advance();
                break;
            }
            advance();
        }
        return;
    }

    // Multi-line comment
    if(cur[0] == '{' && cur[1] == '-') {
        advance(2);
        do {
            consume_space();
            if(cur[0] == '-' && cur[1] == '}') {
                advance(2);
                return;
            }
            advance();
        } while(*cur);
    }
}

/**
 * Character which can be part of the rest of an identifier
 */
bool isrest(char c) {
    return isalnum(c) || c == '_' || c == '\'';
}

Token Scanner::next_token() {
    // Consume leading space and comments
    while(true) {
        if(cur[0] == '\0') {
            return Token { loc, 0, TokenType::TOK_EOF };
        }
        // Break if these don't advance the iterator
        auto pos = loc.pos;
        consume_space();
        consume_comment();
        if(pos == loc.pos) break;
    }

    // Cache because it'll change with scanning
    Loc tokloc = loc;

    // Check for identifiers
    if(std::isalpha(cur[0])) {
        // First consume the entire identifier to avoid false positives
        // on prefixes
        const char *start = cur;
        int len = 0;
        do {
            ++len;
            advance();
        } while(isrest(cur[0]));

        // Check for keywords using an O(log n) algorithm
        // not and or true false if then else fi while do od skip

        #define TREE(kw, tt, L, R) \
            do { \
                int dir = strncmp(start, kw, len); \
                printf("Test %d, %s | %d\n", len, kw, dir); \
                if(dir == 0) { \
                    advance(len); \
                    return Token { tokloc, len, TokenType::tt }; \
                } \
                else if(dir < 0) { L; } \
                else { R; } \
            } while(0)

        #define LEAF(kw, tt) TREE(kw, tt, {}, {})

        TREE("not", OP_NOT,
            TREE("false", KW_FALSE,
                TREE("do", KW_DO,
                    LEAF("and", OP_AND),
                    LEAF("else", KW_ELSE)
                ),
                TREE("fi", KW_FI,
                    LEAF("if", KW_IF), {}
                )
            ),
            TREE("then", KW_THEN,
                TREE("or", KW_OD,
                    LEAF("od", KW_OD),
                    LEAF("skip", KW_SKIP)
                ),
                TREE("true", KW_TRUE,
                    LEAF("while", KW_WHILE), {}
                )
            )
        );

        #undef TREE
        #undef LEAF

        // None of the keywords, must be an identifier
        return Token { tokloc, len, TokenType::IDENT };
    }

    // Check for operators
    switch(cur[0]) {
        case '+':
            advance(1);
            return Token { tokloc, 1, TokenType::OP_ADD };
        case '-':
            advance(1);
            return Token { tokloc, 1, TokenType::OP_SUB };
        case '*':
            advance(1);
            return Token { tokloc, 1, TokenType::OP_MUL };

        case ':':
            if(cur[1] == '=') {
                advance(2);
                return Token { tokloc, 2, TokenType::OP_ASSIGN };
            }
            return Token { tokloc, 0, TokenType::ERROR };

        case '=':
            advance(1);
            return Token { tokloc, 1, TokenType::OP_EQ };

        case '>':
            if(cur[1] == '=') {
                advance(2);
                return Token { tokloc, 2, TokenType::OP_GE };
            }
            advance(1);
            return Token { tokloc, 1, TokenType::OP_GT };
        case '<':
            if(cur[1] == '=') {
                advance(2);
                return Token { tokloc, 2, TokenType::OP_LE };
            }
            advance(1);
            return Token { tokloc, 1, TokenType::OP_LT };

        case ';':
            advance(1);
            return Token { tokloc, 1, TokenType::OP_SEMI };

        case '[':
            advance(1);
            return Token { tokloc, 1, TokenType::OP_LBRACK };
        case ']':
            advance(1);
            return Token { tokloc, 1, TokenType::OP_RBRACK };
    }

    // Check for a number
    char *end = nullptr;
    int64_t value = std::strtoll(cur, &end, 10);
    if(end != cur) {
        advance(end - cur);
        return Token {
            tokloc, static_cast<int>(end - cur),
            TokenType::INTEGER, value
        };
    }
    return Token { tokloc, 0, TokenType::ERROR };
}
