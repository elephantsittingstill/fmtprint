#include <stdio.h>

typedef enum { STATE_TEXT, STATE_PERCENT, STATE_SPECIFIER } ParseState;

void parse_format_switch( const char *fstring ) {
	int i = 0;
	ParseState state = STATE_TEXT;

	while ( fstring[i] ) {
		char c = fstring[i];

		switch ( state ) {
		case STATE_TEXT:
			if ( c == '%' ) {
				state = STATE_PERCENT;
			} else {
				putchar( c );
			}
			i++;
			break;

		case STATE_PERCENT:
			if ( c == '%' ) {
				putchar( '%' );
				state = STATE_TEXT;
			} else {
				state = STATE_SPECIFIER;
				continue;
			}
			i++;
			break;

		case STATE_SPECIFIER:
			if ( c == 's' ) {
				printf( "[STR]" );
			} else if ( c == 'd' ) {
				printf( "[INT]" );
			} else {
				printf( "[UNKNOWN:%c]", c );
			}
			state = STATE_TEXT;
			i++;
			break;
		}
	}

	printf( "\n" );
}

void parse_format_goto( const char *fstring ) {
	int i = 0;

	static void *dispatch[] = { &&state_text, &&state_percent,
								&&state_specifier };

	ParseState state = STATE_TEXT;

	goto *dispatch[state];

state_text:
	if ( !fstring[i] )
		goto end;
	if ( fstring[i] == '%' ) {
		state = STATE_PERCENT;
		i++;
		goto *dispatch[state];
	} else {
		putchar( fstring[i++] );
		goto state_text;
	}

state_percent:
	if ( !fstring[i] )
		goto end;
	if ( fstring[i] == '%' ) {
		putchar( '%' );
		i++;
		state = STATE_TEXT;
		goto *dispatch[state];
	} else {
		state = STATE_SPECIFIER;
		goto *dispatch[state];
	}

state_specifier:
	if ( !fstring[i] )
		goto end;
	switch ( fstring[i] ) {
	case 's':
		printf( "[STR]" );
		break;
	case 'd':
		printf( "[INT]" );
		break;
	default:
		printf( "[UNKNOWN]:%c", fstring[i] );
		break;
	}
	i++;
	state = STATE_TEXT;
	goto *dispatch[state];

end:
	printf( "\n" );
}

int main( int argc, char **argv ) {
	int i = 0;
	const char *fstring = "Hello %s, you are %d years old. %s, you are the "
						  "youngest person ever!";
	while ( i < 1000 ) {
		if ( argc > 1 ) {
			parse_format_switch( fstring );
		} else {
			parse_format_goto( fstring );
		}
		i++;
	}
	return 0;
}
