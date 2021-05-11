#include "Mcc.h"
#include <stdio.h>
#include <string.h>

// the file read by the lexer.
static FILE *fileInput = 0;
static char *fileInputPath = 0;

// is this character a whitespace
static int isWhitespace(char c) {
    // characters that are whitespaces:
    // regular spaces ' '
    // horizontal tabulations '\t'
    // vertical tabulations '\v'
    // carriage return '\r'
    // line feed '\n'
    // form feed '\f'
    return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v'
           || c == '\f'
        ? 1 : 0;
}

// is this character a digit
static int isDigit(char c) {
    // from 0 to 9, as usual
    return c >= '0' && c <= '9';
}

// is this character a letter
static int isLetter(char c) {
    // lowercase and uppercase letters
    return c >= 'A' && c <= 'z';
}

// is this character a letter or a digit
static int isAlpha(char c) {
    // a combination of both isDigit() and isLetter()
    return isLetter(c) || isDigit(c);
}

// peeks the next char in the file.
static U8 peekNextChar() {
    // reading a character
    U8 c = fgetc(fileInput);
    // we can undo that
    ungetc(c, fileInput);
    return c;
}

// opens a file for the lexer. returns 1 if it failed.
int LexOpenStream(const char *filename) {
    const size_t lengthName = strlen(filename);
    fileInput = fopen(filename, "r");	// readonly mode
    fileInputPath = malloc(lengthName + 1);
    memcpy(fileInputPath, filename, lengthName + 1);
    return fileInput ? 0 : 1;
}

// closes the file for the lexer. returns 1 if failed.
int LexCloseStream() {
    return fclose(fileInput) ? 1 : 0;
}

// reads the next token. Returns 0 if no tokens are left to read or an error
// occured.
int LexNextToken(struct LexToken *dest) {
    I c = fgetc(fileInput);
    
    while (isWhitespace(c)) {
        c = fgetc(fileInput);
    }
    
    switch (c) {	// lets see what token this is
    case EOF:
        dest->TokenType = LEX_TOKEN_EOF;
        // the value doesn't need to get zero'd out because we don't need it.
        break;
    case '+':
        dest->TokenType = LEX_TOKEN_PLUS;
        // same reason as the one for the end of file
        break;
    case '-':
        dest->TokenType = LEX_TOKEN_MINUS;
        break;
    case '*':
        dest->TokenType = LEX_TOKEN_STAR;
        break;
    case '/':
        dest->TokenType = LEX_TOKEN_SLASH;
        break;
    case '%':
        dest->TokenType = LEX_TOKEN_PERCENT;
        break;
    default:
        DiagnosticsAdd(DIAG_GRAVITY_ERR, "Found garbage character '%c' ('%d')",
            fileInputPath, ftell(fileInput), c, c);
        return 0;
    }
    
    return 1;
}