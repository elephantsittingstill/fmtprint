#include "lexer.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] ) {
	/*read_fstring( "Hello, %s.\n", "friend" );*/
	parse_format( "%-#17.6s%c.\n", "hello ", 'a' );

	return EXIT_SUCCESS;
}
