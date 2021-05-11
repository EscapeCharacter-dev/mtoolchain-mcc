#ifndef MCC_TYPES_H
#define MCC_TYPES_H

enum {
    LEX_TOKEN_EOF,	// eof, -1
    LEX_TOKEN_PLUS,	// +
    LEX_TOKEN_MINUS,	// -
    LEX_TOKEN_STAR,	// *
    LEX_TOKEN_SLASH,	// /
    LEX_TOKEN_PERCENT,	// %
    DIAG_GRAVITY_NOTE,	// note (doesn't stop compilation)
    DIAG_GRAVITY_WARN,	// warning (can stop the compilation)
    DIAG_GRAVITY_ERR,	// error (stops the compilation)
    DIAG_GRAVITY_IERR,	// internal error (stops the compiler)
};

typedef signed char		I8;
typedef unsigned char		U8;
typedef signed short		I16;
typedef unsigned short		U16;
typedef signed int		I;
typedef unsigned int		U;
typedef signed long		I32;
typedef unsigned long		U32;
typedef signed long long	I64;
typedef unsigned long long	U64;

union ValueUnion {
    I8	AsI8;
    U8	AsU8;
    I16	AsI16;
    U16	AsU16;
    I	AsI;
    U	AsU;
    I32	AsI32;
    U32	AsU32;
    I64	AsI64;
    U64	AsU64;
};

// a token passed between the lexer and the parser.
struct LexToken {
    I			TokenType;	// the type of the token, enums
                                        // starting with LEX_TOKEN_
    union ValueUnion	Value;		// the value. you should use I (int) if
                                        // you have no clue what the type is.
};

// adds a new diagnostic
void DiagnosticsAdd(I gravity, const char *format, const char *filename,
    I fpos, ...);

// has there been any critical diagnostics?
int DiagnosticsAny();

// allows other parts of the compiler to override notes and warnings.
void DiagnosticsOverride(int onNoteStop, int onWarnStop);

// opens a file for the lexer to read input from. Returns 1 on error
int LexOpenStream(const char *filepath);

// closes the file the lexer was reading. Returns 1 on error
int LexCloseStream();

// reads a token, 1 on error
int LexNextToken(struct LexToken *buf);

#endif