#include "../include/fmtprint.h"

int main() {
	fmtprint( "The following lines will all have .7 precision:\n"
			  "Int: %.7d\nUnsigned: %.7u\nOctal: %.7o\nHex: %.7x\n"
			  "Float: %.7f\nString: %.7s\nChar: %.7c\n",
			  42069345, 42069345, 42069345, 42069345, 4.20693457,
			  "Hello There!", 'A' );
}
