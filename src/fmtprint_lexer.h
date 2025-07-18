#ifndef lexer_h
#define lexer_h

#include "fmtprint_string.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#if defined( _WIN32 ) || defined( _WIN64 )
#include <io.h>
#define PLATFORM_WRITE _write
#define WINDOWS 1
#else
#include <unistd.h>
#define PLATFORM_WRITE write
#endif

#if defined( __GNUC__ ) || defined( __clang__ )
#define USE_COMPUTED_GOTO 1
void parse_format_switch( const char **fstring, va_list *args );
void parse_format_goto( const char **fstring, va_list *args );
#else
void parse_format_switch( const char *fstring );
#define USE_COMPUTED_GOTO 0
void parse_format_switch( const char *fstring );
#endif

#define isspec( c ) ( ( c == 'd' ) || ( c == 's' ) )

typedef enum State {
	STATE_TEXT,
	STATE_PERCENT,
	STATE_FLAGS,
	STATE_WIDTH,
	STATE_PRECISION,
	STATE_SPECIFIER
} State;

typedef struct FormatToken {
	char flags[4];
	uint32_t width;
	uint32_t precision;
	char specifier;
} FormatToken;

/* Wrapper function which uses either switch or computed goto depending on
 * compiler */
void parse_format( const char *fstring, ... );

/* Does the actual printing of the constructed string */
/* void print_fstring( const char *s, size_t len ); */

/* FUNCTIONS THAT DO LOGIC DEPENDING ON THE SPECIFIER */
void parse_specifier_token( FormatToken *t, String *s, const char **fstring,
							va_list *args );

void parse_specifier_d( String *s, FormatToken *t, va_list *args );
void parse_specifier_u( String *s, FormatToken *t, va_list *args );
void parse_specifier_f( String *s, FormatToken *t, va_list *args );
void parse_specifier_o( String *s, FormatToken *t, va_list *args );
void parse_specifier_s( String *s, FormatToken *t, va_list *args );
void parse_specifier_x( String *s, FormatToken *t, va_list *args );
void parse_specifier_c( String *s, FormatToken *t, va_list *args );
void parse_specifier_p( String *s, FormatToken *t, va_list *args );

/* These functions that put the appropriate raw string conversion into buf */
int format_raw_int( char *buf, int value );
int format_raw_unsigned( char *buf, unsigned int value );
int format_raw_octal( char *buf, unsigned int value );
int format_raw_hex( char *buf, unsigned int value );
int format_raw_float( FormatToken *t, char *buf, double value );
int format_raw_string( char *value );
int format_raw_char( char *buf, int value );
int format_raw_pointer( char *buf, void *ptr );

#endif
