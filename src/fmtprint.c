#include "internal.h"

void fmtprint( const char *fstring, ... ) {

	va_list args;
	va_start( args, fstring );

#if USE_COMPUTED_GOTO
	parse_format_switch( &fstring, &args );
#else
	parse_format_goto( &fstring, &args );
#endif

	return;
}
