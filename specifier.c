#include "lexer.h"
#include "string.h"

void apply_modifiers( String *s, FormatToken *t, const char *raw, int len ) {
	enum { NUMERIC, TEXT, CHAR } Type;

	char temp[64];

	int left_flag = 0;
	int zero_flag = 0;
	int sepr_flag = 0;
	int hash_flag = 0;

	int i = 0;

	int is_negative = 0;
	int precision_zeros = 0;

	char *ptr = t->flags;
	while ( *ptr ) {
		switch ( *ptr ) {
		case '-':
			left_flag = 1;
			break;
		case '0':
			zero_flag = 1;
			break;
		case '\'':
			sepr_flag = 1;
			break;
		case '#':
			hash_flag = 1;
			break;
		}
		ptr++;
	}

	switch ( t->specifier ) {
	case 'd':
	case 'i':
		/* signed int logic */
		is_negative = ( raw[0] == '-' );
		if ( is_negative ) {
			temp[i++] = '-';
			raw++;
			len--;
		}
	case 'u':
	case 'x':
	case 'o':
		/* unsigned int logic */
		/* precision handling */
		if ( t->precision > len ) {
			precision_zeros = t->precision - len;
		}
		{
			int j = 0;
			while ( j < precision_zeros ) {
				temp[i++] = '0';
				j++;
			}
		}
		break;
	default:
		break;
	}

	/* -- Copy raw string to temp --  */
	{
		int j = 0;
		while ( j < len ) {
			temp[i++] = raw[j];
			j++;
		}
	}

	temp[i] = '\0';
	int total_len = i;

	/* -- Width padding -- */
	int padding = 0;
	if ( t->width > total_len ) {
		padding = t->width - total_len;
	}

	if ( padding > 0 ) {
		if ( left_flag ) {
			/* Left-align: pad spaces on right */
			append_cstr_String( s, temp );
			int j = 0;
			while ( j < padding ) {
				append_char_String( s, ' ' );
				j++;
			}
		} else {
			/* Right-align: pad with '0' of ' ' */
			if ( ( zero_flag && t->precision == 0 ) &&
				 ( t->specifier != 's' && t->specifier != 'c' ) ) {
				/* Inset padding after sign */
				String temp_str;
				init_String( &temp_str );
				append_char_String( &temp_str, temp[0] ); /* The sign */
				int j = 0;
				while ( j < padding ) {
					append_char_String( &temp_str, '0' );
					j++;
				}
				append_cstr_String( &temp_str, temp + 1 );
				append_cstr_String( s, temp_str.string );
				free_String( &temp_str );
			} else {
				/* Pad with spaces on left */
				int j = 0;
				while ( j < padding ) {
					append_char_String( s, ' ' );
					j++;
				}
				append_cstr_String( s, temp );
			}
		}
	} else {
		append_cstr_String( s, temp );
	}
}

void old_parse_specifier_d( String *s, FormatToken *t, va_list *args ) {
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

void parse_specifier_d( String *s, FormatToken *t, va_list *args ) {
	char raw[32];
	int value = va_arg( *args, int );
	int len = format_raw_int( raw, value );
	apply_modifiers( s, t, raw, len );
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

	/* precision equals the precision if defined, if not, will default to 3
	 */
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
void parse_specifier_x( FormatToken *t, va_list *args );
void parse_specifier_s( FormatToken *t, va_list *args );
void parse_specifier_c( FormatToken *t, va_list *args );
void parse_specifier_p( FormatToken *t, va_list *args );

int format_raw_int( char *buf, int value ) {
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
	int len = i;

	/* reverse the string */
	int j = 0;
	while ( temp[j] ) {
		buf[j] = temp[i - 1];
		j++;
		i--;
	}

	return len;
}
