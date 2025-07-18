#ifndef string_h
#define string_h

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char *string;
	size_t length;
	size_t capacity;
} String;

void init_String( String *s );
void append_cstr_String( String *s, const char *suffix );
void append_char_String( String *s, const char c );

String *concat_String( String first, String second );
char *toC_String( String string );
void free_String( String *string );

#endif
