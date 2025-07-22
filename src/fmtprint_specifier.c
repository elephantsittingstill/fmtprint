#include "fmtprint_lexer.h"
#include "fmtprint_string.h"

void apply_modifiers( String *s, FormatToken *t, const char *raw, int len ) {

	char temp[128];

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
	case 'p':
	case 'x':
	case 'u':
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
	case 's':
	case 'c':
		zero_flag = 0;
		break;
	default:
		break;
	}

	if ( hash_flag && ( t->specifier == 'x' || t->specifier == 'p' ) ) {
		temp[i++] = '0';
		temp[i++] = 'x';
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
			if (( zero_flag && t->precision == 0 )) {
				/* Inset padding after sign */
				String temp_str;
				init_String( &temp_str );

				/* Include negative sign if present */
				if ( is_negative ) {
					append_char_String( &temp_str, temp[0] ); /* The sign */
				}

				int j = 0;
				while ( j < padding ) {
					append_char_String( &temp_str, '0' );
					j++;
				}

				/* Disclude negative sign if present */
				if ( is_negative ) {
					append_cstr_String( &temp_str, temp + 1 );
				} else {
					append_cstr_String( &temp_str, temp );
				}

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

void parse_specifier_d( String *s, FormatToken *t, va_list *args ) {
	char raw[32];
	int value = va_arg( *args, int );
	int len = format_raw_int( raw, value );
	apply_modifiers( s, t, raw, len );
}

void parse_specifier_u( String *s, FormatToken *t, va_list *args ) {
	char raw[32];
	unsigned int value = va_arg( *args, unsigned int );
	int len = format_raw_unsigned( raw, value );
	apply_modifiers( s, t, raw, len );
}

void parse_specifier_o( String *s, FormatToken *t, va_list *args ) {
	char raw[32];
	unsigned int value = va_arg( *args, unsigned int );
	int len = format_raw_octal( raw, value );
	apply_modifiers( s, t, raw, len );
}

void parse_specifier_x( String *s, FormatToken *t, va_list *args ) {
	char raw[32];
	unsigned int value = va_arg( *args, unsigned int );
	int len = format_raw_hex( raw, value );
	apply_modifiers( s, t, raw, len );
}

void parse_specifier_f( String *s, FormatToken *t, va_list *args ) {
	char raw[32];
	double value = va_arg( *args, double );
	int len = format_raw_float( t, raw, value );
	apply_modifiers( s, t, raw, len );
}

void parse_specifier_s( String *s, FormatToken *t, va_list *args ) {
	char *value = va_arg( *args, char * );
	int len = format_raw_string( value );
	apply_modifiers( s, t, value, len );
}

void parse_specifier_c( String *s, FormatToken *t, va_list *args ) {
	char raw[2];
	int value = va_arg( *args, int );
	int len = format_raw_char( raw, value );
	apply_modifiers( s, t, raw, len );
}

void parse_specifier_p( String *s, FormatToken *t, va_list *args ) {
	char raw[32];
	void *ptr = va_arg( *args, void * );
	int len = format_raw_pointer( raw, ptr );
	apply_modifiers( s, t, raw, len );
}

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

int format_raw_unsigned( char *buf, unsigned int value ) {
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

int format_raw_octal( char *buf, unsigned int value ) {
	char temp[12];
	int i = 0;

	if ( value == 0 ) {
		temp[0] = '0';
		temp[1] = '\0';
		return 2;
	}

	while ( value > 0 ) {
		temp[i++] = '0' + ( value & 0x7 );
		value >>= 3;
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

int format_raw_hex( char *buf, unsigned int value ) {
	char temp[12];
	int i = 0;

	if ( value == 0 ) {
		temp[0] = '0';
		temp[1] = '\0';
		return 2;
	}

	while ( value > 0 ) {
		if ( ( value & 0xf ) > 9 ) {
			temp[i++] = ( 'a' - 10 ) + ( value & 0xf );
		} else {
			temp[i++] = '0' + ( value & 0xf );
		}
		value >>= 4;
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

int format_raw_float( FormatToken *t, char *buf, double value ) {
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
	int dot_location = ( i - precision );
	int steps = i + 1;
	while ( j < steps ) {
		if ( j == dot_location && !dot_placed ) {
			buf[j++] = '.';
			dot_placed = 1;
			continue;
		}
		buf[j] = temp[i - 1];
		j++;
		i--;
	}

	buf[j] = '\0';

	return steps;
}

int format_raw_string( char *value ) {
	int i = 0;

	while ( value[i] ) {
		i++;
	}

	return i;
}

int format_raw_char( char *buf, int value ) {
	buf[0] = value;
	buf[1] = '\0';
	return 2;
}

int format_raw_pointer( char *buf, void *ptr ) {
	uintptr_t addr = (uintptr_t)ptr;
	char temp[32];
	int i = 0;

	if ( addr == 0 ) {
		temp[0] = '0';
		temp[1] = '\0';
		return 2;
	}

	while ( addr > 0 ) {
		if ( ( addr & 0xf ) > 9 ) {
			temp[i++] = ( 'a' - 10 ) + ( addr & 0xf );
		} else {
			temp[i++] = '0' + ( addr & 0xf );
		}
		addr >>= 4;
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
