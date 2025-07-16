#include "lexer.h"
#include "buffer.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

#define isalpha( c ) ( ( 'a' <= c && c <= 'z' ) || ( 'A' <= c && c <= 'Z' ) )
#define isnum( c ) ( '0' <= c && c <= '9' )
#define tonum( c ) ( c - '0' )
#define isflag( c )                                                            \
	( ( c == '-' ) || ( c == '0' ) || ( c == '\'' ) ||                         \
	  ( c == 35 ) ) /* 35 for hashtag symbol */

void parse_format( const char *fstring, ... ) {

	va_list args;
	va_start( args, fstring );

#if USE_COMPUTED_GOTO
	parse_format_switch( &fstring, &args );
#else
	parse_format_goto( &fstring, &args );
#endif

	return;
}

#if USE_COMPUTED_GOTO
void parse_format_switch( const char **fstring, va_list *args ) {
	FormatToken t = { 0 };

	String s = { 0 };
	init_String( &s );

	State state = STATE_TEXT;

	while ( **fstring ) {
		char c = **fstring;

		switch ( state ) {
		case STATE_TEXT:
			if ( c == '%' ) {
				state = STATE_PERCENT;
				( *fstring )++;
				break;
			} else {
				append_char_String( &s, c );
			}

			( *fstring )++;
			break;

		case STATE_PERCENT:
			if ( c == '%' ) {
				append_char_String( &s, c );
				state = STATE_TEXT;
			} else {
				state = STATE_FLAGS;
				continue;
			}

			( *fstring )++;
			break;

		case STATE_FLAGS:
			if ( isflag( c ) ) {
				append_cstr_String( &s, "[FLAGS]" );
				parse_flags( &t, fstring );
				state = STATE_WIDTH;
				continue;
			} else {
				state = STATE_WIDTH;
				continue;
			}

		case STATE_WIDTH:
			if ( isnum( c ) ) {
				append_cstr_String( &s, "[WIDTH]" );
				parse_width( &t, fstring );
				state = STATE_PRECISION;
				continue;
			} else {
				state = STATE_PRECISION;
				continue;
			}

		case STATE_PRECISION:
			if ( c == '.' ) {
				append_cstr_String( &s, "[PRECISION]" );
				parse_precision( &t, fstring );
				state = STATE_SPECIFIER;
				continue;
			} else {
				state = STATE_SPECIFIER;
				continue;
			}

		case STATE_SPECIFIER:
			t.specifier = c;
			/* parse_specifier(&t, &fstring, args) */
			/*if ( c == 's' ) {
				append_cstr_String( &s, "[STR]" );
			} else if ( c == 'd' ) {
				append_cstr_String( &s, "[INT]" );
			} else if ( c == 'c' ) {
				append_cstr_String( &s, "[CHAR]" );
			} else {
				append_cstr_String( &s, "[UNKNOWN]" );
			}*/

			parse_specifier_token( &t, &s, fstring, args );

			state = STATE_TEXT;
			( *fstring )++;
			break;
		}
	}

	print_fstring( s.string, s.length );
	free_String( &s );
	printf( "flags: %s\n"
			"width: %d\n"
			"precision: %d\n"
			"specifier: %c\n",
			t.flags, t.width, t.precision, t.specifier );
}
#endif

void print_fstring( const char *s, size_t len ) {
#ifdef WINDOWS
	len = (unsigned int)len;
#endif
	PLATFORM_WRITE( 1, s, len );
}

void parse_specifier_token( FormatToken *t, String *s, const char **fstring,
							va_list *args ) {
	int value;
	const char *string;
	char ch;

	switch ( **fstring ) {
	case 's':
		string = va_arg( *args, const char * );
		append_cstr_String( s, string );
		return;
	case 'd':
		value = va_arg( *args, int );
		const char *d_string = parse_specifier_d( t, args );
		append_cstr_String( s, d_string );
	case 'c':
		ch = va_arg( *args, int );
		append_char_String( s, ch );
	}
}

/* flags: '-', '0', ''', '#' */
void parse_flags( FormatToken *t, const char **fstring ) {
	int i = 0;
	while ( **fstring ) {
		if ( i > 3 ) {
			fprintf( stderr, "ERROR: flag overflow in formatted string\n" );
			exit( -1 );
		}

		char c = **fstring;

		if ( isflag( c ) ) {
			t->flags[i++] = c;
			( *fstring )++;
		} else {
			break;
		}
	}
	return;
}

/* width: number */
void parse_width( FormatToken *t, const char **fstring ) {
	int result = 0;
	while ( isnum( **fstring ) ) {
		if ( result > 0 ) {
			result = ( ( result * 10 ) + tonum( **fstring ) );
		} else {
			result += tonum( **fstring );
		}

		( *fstring )++;
	}

	t->width = result;
	return;
}

/* precision: '.n' */
void parse_precision( FormatToken *t, const char **fstring ) {
	int result = 0;
	( *fstring )++;

	while ( isnum( **fstring ) ) {
		if ( result > 0 ) {
			result += ( ( result * 10 ) + tonum( **fstring ) );
		} else {
			result += tonum( **fstring );
		}

		( *fstring )++;
	}

	t->precision = result;
	return;
}

char *parse_specifier_d( FormatToken *t, va_list *args ) { return "penis"; }
