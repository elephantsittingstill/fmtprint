#include "../include/fmtprint.h"

int main() {
	fmtprint( "Hex with no hash flag: %x\n"
			  "Hex with hash flag: %#x\n",
			  42, 42 );
}
