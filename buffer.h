#include <stddef.h>

typedef struct OutputBuffer {
	char *buf;
	size_t capacity;
	size_t len;
} OutputBuffer;

void output_append( OutputBuffer *out, const char *src, int len );
void output_append_char( OutputBuffer *out, char c );
