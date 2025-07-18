#include "lexer.h"
#include <stdlib.h>

int main() {
	int x = 14;
	int *ptr = &x;

	parse_format( "Value: %0p\n", ptr );

	return EXIT_SUCCESS;
}
