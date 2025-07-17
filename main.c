#include "lexer.h"
#include <stdlib.h>

int main( int argc, char *argv[] ) {
	parse_format( "%-#17.6s%c%d.\n", "hello ", 'a', 420 );
	parse_format( "%f\n", 3.762345 );
	parse_format( "%u\n", 243762345 );
	parse_format( "%o\n", 0132 );

	return EXIT_SUCCESS;
}
