#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct {
  const char* start;
  const char* cur;
  int line;
} Scanner;

Scanner scanner;

void initScanner(const char* source) {
    scanner.start = source;
    scanner.cur = source;
    scanner.line = 1;
}

static bool isAtEnd() {
    return *scanner.cur == '\0';
}

static char advance() {
    scanner.cur++;
    return *(scanner.cur - 1);
}

static char peek() {
    return *scanner.cur;
}

static char npeek(int n) {
    if (isAtEnd()) return '\0';
    return *scanner.cur + n;
}

static bool match(char c) {
    if (isAtEnd() || *scanner.cur != c) return false;
    scanner.cur++;
    return true;
}

static Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int)(scanner.cur - scanner.start);
    token.line = scanner.line;
    return token;
}

static Token errorToken(char * msg) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = msg; 
    token.length = (int)strlen(msg);
    token.line = scanner.line;
    return token;
}

static void skipWhiteSpace(void) {
    for (;;) {
        char c = peek();
        switch (c) {
            case ' ':
            case '\t':
            case '\r':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            case '/':
                if (npeek(1) == '/') {
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else {
                    return; 
                }
                break;
            default:
                return;
        }
    }
}

Token scanToken(void) {
    skipWhiteSpace();
    scanner.start = scanner.cur;

    if (isAtEnd()) return makeToken(TOKEN_EOF);

    char c = advance();

    switch (c) {
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case ',': return makeToken(TOKEN_COMMA);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_STAR);
        case '!':
                  return makeToken(
                          match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
                  return makeToken(
                          match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
                  return makeToken(
                          match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
                  return makeToken(
                          match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    }
        

    return errorToken("Unexpected character.");
}
