#include "lexer.h"
#include "buffer.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

#define isalpha( c ) ( ( 'a' <= c && c <= 'z' ) || ( 'A' <= c && c <= 'Z' ) )
#define changestate( s, n )                                                    \
	char old = s;                                                              \
	s = n;                                                                     \
	printf( "State changed from %d to %d\n", old, n );

/* void read_fstring( char *fstring, ... ) {
	int i = 0;
	int c = fstring[i];
	State state = TEXT;

	while ( ( c = fstring[i] ) != '\0' ) {
		if ( c == '%' ) {
			printf( "%*c", i + 1, '^' );
			printf( "\nfound %% at char %d\nchar: %c\n", i, fstring[i] );
			changestate( state, START )
		}

		i++;
	}
 } */

void read_fstring( const char *fstring, ... ) {
	va_list args;
	va_start( args, fstring );

	String s = { 0 };
	init_String( &s );

	char *buf = malloc( sizeof( char ) * 8 );

	while ( *fstring ) {
		if ( *fstring == '%' ) {
			FormatToken token = parse_format_token( &fstring );
			render_token( buf, &token, &args );
			append_cstr_String( &s, buf );
		} else {
			/* output_append_char(); */
			append_char_String( &s, *fstring );
		}
		if ( ( *fstring + 1 ) == NULL )
			append_char_String( &s, *fstring );
		*fstring++;
	}

	puts( s.string );

	free_String( &s );
	free( buf );
}

int render_token( char *buf, FormatToken *token, va_list *args ) {
	/* sprintf's used in this function are merely placeholder */

	int written = 0;
	int value;
	const char *string;

	switch ( token->specifier ) {
	case 's':
		string = va_arg( *args, const char * );
		return sprintf( buf, "%s", string );
	case 'd':
		value = va_arg( *args, int );
		return sprintf( buf, "%d", value );
	default:
		written = sprintf( buf, "<?>" );
		return written;
	}
}

FormatToken parse_format_token( const char **fstring ) {
	FormatToken token = { 0 };
	( *fstring )++;

	while ( **fstring ) {
		char c = **fstring;
		if ( isspec( c ) ) {
			token.specifier = c;
			( *fstring )++;
			break;
		} else {
			break;
		}
	}
	return token;
}
