#ifndef lexer_h
#define lexer_h

#include "string.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#if defined( _WIN32 ) || defined( _WIN64 )
#include <io.h>
#define PLATFORM_WRITE _write
#define WINDOWS 1
#else
#include <unistd.h>
#define PLATFORM_WRITE write
#endif

/*
 * Things to remember when including cross-compatibility to this project:
 * MSVC translates \n to \r\n in stdio functions such as printf,
 *		so must incorporate that translation myself since I will
 *		be using write()/_write() to print to stdout
 *		* THIS WILL HAPPEN RIGHT BEFORE I CALL *
 *		* THE sys_write() WRAPPER (emit_char())*
 * */

/*
 * Conceptual pipeline for my printf
 * 1. Format string
 * 2. State Machine (Lexer/Parser)
 * 3. FormatTokens (parsed, e.g. widgth = 4, specifier = 'd')
 * 4. Renderer:
 *		For each token, extract va_arg, and render the value into a
 *string.
 * 5. Output:
 *		- Write to buffer (snprintf style)
 *		- Write to stdout (printf style)
 *		- Write to a file descriptor (low-level I/O)
 * */

/*
 * SYNTAX for fstrings
 * %[parameter][flags][width][.precision][length]type
 *
 * parameter (n$)
 *		allows for using the same value multiple times in a format
 *string insteard of having to pass the value multiple times.
 *
 *		example:
 *		printf("%2d 1$d 2$#x;", 16, 17);
 *		output: 17 16 0x11;
 *
 * flags ('-', '+', ' ', '0', ''', '#')
 *		'-': left align the output
 *		'+': prepends a plus sign	// I do not plan on implenting this
 *		' ': prepends a space char	// I do not plan on implenting this
 *		'0': when 'width' is specified,
 *			 prepends with 0 instead of spaces
 *		''': the integer or exponent of a decimal
 *			 has a the thousands grouping seperator applied
 *		'#': for 'f', will output such as '123.' instead of '123'
 *			 for 'o', will add a leading '0'
 *			 for 'x' or 'X', will add a leading '0x' or '0X'
 *respectively
 *
 * width (n) (number)
 *		width field specifies the minimum number
 *		of chars to output. If number is higher than
 *		actual width of output, will prepend with spaces.
 *
 * precision field ('.n' or '.*')
 *		for floating-point types, determines the
 *		number of digits right of the decimal point
 *		to which the output should be rounded
 *
 *		for most other types, determines the max
 *		number of chars that should be output
 *
 *		example:
 *		printf("%.3f", "abcdef"); or
 *		printf("%.*f", 3, "abcdef");
 *		output:  abc
 *
 * length field (will look into later)
 *
 * type field
 *		'%': output a literal '%' character; does not
 *			 accept flags, width, precision or length fields
 *		'd': (signed) int formatted as decimal
 *		'u': unsigned int formatted as decimal
 *		'f': double formatted as fixed-point;
 *		'e': double formatted as in exponential notation
 *			 'd.dde±dd'
 *		'g': double formatted as either fixed point or exp
 *			 notation, based on its magnitude
 *		'x': unsigned int formatted as hexadecimal
 *		'o': unsigned int formatted as octal
 *		's': null-terminated string
 *		'c': char
 *		'p': pointer formatted in an implementation-defined way
 *		'a': double in hexadecimal notation
 *		'n': outputs nothing but writes the number of chars
 *			 written so far into an integer pointer parameter
 */

#if defined( __GNUC__ ) || defined( __clang__ )
#define USE_COMPUTED_GOTO 1
void parse_format_switch( const char *fstring );
void parse_format_goto( const char *fstring );
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
void parse_format( const char *fstring );

/* Does the actual printing of the constructed string */
void print_fstring( const char *s, size_t len );

/* Functions to fill the Token struct*/
void parse_flags( FormatToken *t, const char **fstring );
void parse_width( FormatToken *t, const char **fstring );
void parse_precision( FormatToken *t, const char **fstring );

/* FUNCTIONS THAT DO LOGIC DEPENDING ON THE SPECIFIER */
void parse_specifier_token( FormatToken *t, String *s, const char **fstring,
							va_list *args );
char *parse_specifier_d();
#endif
