#include "string.h"
#include <stdio.h>
#include <string.h>

void free_String( String *string ) {
	if ( string == NULL ) {
		fprintf( stderr, "ERROR: string is NULL\n" );
		exit( -1 );
	}

	free( string->string );
	string->string = NULL;
	string->length = 0;
	string->capacity = 0;
}

void init_String( String *s ) {
	s->string = malloc( 8 );
	if ( !s->string ) {
		fprintf( stderr, "ERROR: failure to malloc string in init_String\n" );
		exit( -1 );
	}

	s->string[0] = '\0';
	s->length = 0;
	s->capacity = 8;
}

void append_cstr_String( String *s, const char *suffix ) {
	size_t suffix_len = strlen( suffix );
	size_t required = s->length + suffix_len + 1;

	if ( required > s->capacity ) {
		size_t new_capacity = s->capacity ? s->capacity : 8;
		while ( new_capacity < required ) {
			new_capacity *= 2;
		}

		char *new_string = realloc( s->string, new_capacity );
		if ( !new_string ) {
			fprintf( stderr, "ERROR: failure to realloc new_string in "
							 "append_cstr_String\n" );
			exit( -1 );
		}

		s->string = new_string;
		s->capacity = new_capacity;
	}

	memcpy( s->string + s->length, suffix, suffix_len + 1 );
	s->length += suffix_len;
}

void append_char_String( String *s, const char c ) {
	size_t required = s->length + 2; /* +2 for the char being added and
									  * the null terminator at the end */

	if ( required > s->capacity ) {
		size_t new_capacity = s->capacity ? s->capacity : 8;
		while ( new_capacity < required ) {
			new_capacity *= 2;
		}

		char *new_string = realloc( s->string, new_capacity );
		if ( !new_string ) {
			fprintf( stderr, "ERROR: failure to realloc new_string in "
							 "append_cstr_String\n" );
			exit( -1 );
		}

		s->string = new_string;
		s->capacity = new_capacity;
	}

	s->string[s->length] = c;
	s->length += 1;
	s->string[s->length] = '\0';
}
