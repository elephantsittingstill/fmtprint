#include "lexer.h"
#include "string.h"

void parse_specifier_d( String *s, FormatToken *t, va_list *args ) {
	int value = va_arg( *args, int );
	char temp[12];
	int isnegative = 0;
	int i = 0;

	if ( value == 0 ) {
		temp[0] = '0';
		temp[1] = '\0';
	}

	if ( value < 0 ) {
		isnegative = 1;
		value = -value;
	}

	while ( value > 0 ) {
		temp[i++] = '0' + ( value % 10 );
		value /= 10;
	}

	if ( isnegative ) {
		temp[i++] = '-';
	}

	temp[i] = '\0';

	/* reverse the string */
	int j = 0;
	char reversed[12];
	while ( temp[j] ) {
		reversed[j] = temp[i - 1];
		j++;
		i--;
	}

	append_cstr_String( s, reversed );
}

void parse_specifier_f( String *s, FormatToken *t, va_list *args ) {
	double value = va_arg( *args, double );
	char temp[12];
	int isnegative = 0;
	int dot_placed = 0;
	int i = 0;

	if ( value == 0 ) {
		temp[0] = '0';
		temp[1] = '\0';
	}

	if ( value < 0 ) {
		isnegative = 1;
		value = -value;
	}

	/* precision equals the precision if defined, if not, will default to 3 */
	int precision = ( t->precision > 0 ) ? t->precision : 3;

	/* shift the double by precision places */
	{
		int exp = 0;
		while ( exp < precision ) {
			value *= 10;
			exp++;
		}
	}

	/* now, we don't need the rest of the floating point,
	 * so convert the double into an int */
	int i_value = (int)value;

	while ( i_value > 0 ) {
		temp[i++] = '0' + ( i_value % 10 );
		i_value /= 10;
	}

	if ( isnegative ) {
		temp[i++] = '-';
	}

	temp[i] = '\0';

	/* reverse the string */
	int j = 0;
	char reversed[12];
	int dot_location = ( i - precision );
	int steps = i + 1;
	while ( j < steps ) {
		if ( j == dot_location && !dot_placed ) {
			reversed[j++] = '.';
			dot_placed = 1;
			continue;
		}
		reversed[j] = temp[i - 1];
		j++;
		i--;
	}

	reversed[j] = '\0';

	append_cstr_String( s, reversed );
}

void parse_specifier_u( String *s, FormatToken *t, va_list *args ) {
	unsigned int value = va_arg( *args, unsigned int );
	char temp[12];
	int i = 0;

	if ( value == 0 ) {
		temp[0] = '0';
		temp[1] = '\0';
	}

	while ( value > 0 ) {
		temp[i++] = '0' + ( value % 10 );
		value /= 10;
	}

	temp[i] = '\0';

	/* reverse the string */
	int j = 0;
	char reversed[12];
	while ( temp[j] ) {
		reversed[j] = temp[i - 1];
		j++;
		i--;
	}

	append_cstr_String( s, reversed );
}

void parse_specifier_o( String *s, FormatToken *t, va_list *args ) {
	unsigned int value = va_arg( *args, unsigned int );
	char temp[12];
	int i = 0;

	if ( value == 0 ) {
		temp[0] = '0';
		temp[1] = '\0';
		return;
	}

	while ( value > 0 ) {
		temp[i++] = '0' + ( value & 0x7 );
		value >>= 3;
	}

	temp[i] = '\0';

	/* reverse the string */
	int j = 0;
	char reversed[12];
	while ( temp[j] ) {
		reversed[j] = temp[i - 1];
		j++;
		i--;
	}

	append_cstr_String( s, reversed );
}

void parse_specifier_e( FormatToken *t, va_list *args );
void parse_specifier_g( FormatToken *t, va_list *args );
void parse_specifier_x( FormatToken *t, va_list *args );
void parse_specifier_s( FormatToken *t, va_list *args );
void parse_specifier_c( FormatToken *t, va_list *args );
void parse_specifier_p( FormatToken *t, va_list *args );
void parse_specifier_a( FormatToken *t, va_list *args );
void parse_specifier_n( FormatToken *t, va_list *args );
