#include "lexer.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] ) {
	/*read_fstring( "Hello, %s.\n", "friend" );*/
	parse_format_switch( "%-#17.6s.\n" );

	return EXIT_SUCCESS;
}
