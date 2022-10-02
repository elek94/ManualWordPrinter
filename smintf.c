#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include "smintf.h"
#include <stdio.h>

int _write_integer(int, char*, int, int, char*);				 			// helper
int _length_count(const char* format, va_list second_arg); 					// helper
int _write_integer_count(int n, int num_bytes, int radix, char* prefix);	// helper

char* smintf(const char *format, ...) {

    va_list list_args, second_arg; 
	va_start(list_args, format);
	va_copy(second_arg, list_args);
     
	int num_bytes = _length_count(format, second_arg);
	char* output_str = malloc(num_bytes * sizeof(*output_str));
	int char_count = 0;

	for(int i = 0; format[i] != '\0'; i++) {
		
		if((format[i] == '%') && (format[i + 1] == '%')) { 								// display characters when % sign repeatedly exists and even number of times.
			output_str[char_count] = format[i];
			i += 1;
			char_count += 1;
		}
		else if((format[i] == '%') && (format[i + 1] == 'd')) { 						// decimal
			int nxt_dec = va_arg(list_args, int);   									// argument received.
			char_count = _write_integer(nxt_dec, output_str, char_count, 10, "");
			i += 1; 																	// accelerate i count to neglect specifier display.
		}
		else if((format[i] == '%') && (format[i + 1] == 's')) { 						// string	
			char* nxt_strng = va_arg(list_args, char*);
			for(int c = 0; nxt_strng[c] != '\0'; c++) {
				output_str[char_count] = nxt_strng[c];
				char_count += 1;
			}
			i += 1;
		}	
		else if((format[i] == '%') && (format[i + 1] == 'x')) { 						// hexa
			int nxt_hex = va_arg(list_args, int);
			char_count = _write_integer(nxt_hex, output_str, char_count, 16, "0x"); 	// call print_integer to display hexadecimal(16 base case) with "0x" prefix.
			i += 1;
		}
		else if((format[i] == '%') && (format[i + 1] == 'b')) { 						// binary
			int nxt_bin = va_arg(list_args, int);
			char_count = _write_integer(nxt_bin, output_str, char_count, 2, "0b");     	// call print_integer to display with binary(2 base case) with "0b" prefix.
			i += 1; 
		}
		else if((format[i] == '%') && (format[i + 1] == '$')) { 						// dollar currency: $
			
			int actual_curr = va_arg(list_args, int);
			if(actual_curr < 0) {
				char_count = _write_integer(actual_curr / 100, output_str, char_count, 10, "$");
				char_count = _write_integer(-1*((actual_curr % 100) / 10), output_str, char_count, 10, ".");
				char_count = _write_integer(-1*(actual_curr % 10), output_str, char_count, 10, "");
			}
			else {
				char_count = _write_integer(actual_curr / 100, output_str, char_count, 10, "$");
				char_count = _write_integer((actual_curr % 100) / 10, output_str, char_count, 10, ".");
				char_count = _write_integer(actual_curr % 10, output_str, char_count, 10, "");
			}
			i += 1;
		}
		else {
			output_str[char_count] = format[i];
			char_count += 1;
		}
	}	
	output_str[num_bytes - 1] = '\0';

	va_end(list_args);
	va_end(second_arg);

	return output_str;
}


int _length_count(const char* format, va_list second_arg) {

	int num_bytes = 0;

	for(int i = 0; format[i] != '\0'; i++) {
	
		if((format[i] == '%') && (format[i + 1] == '%')) { 						
			num_bytes += 1;
			i += 1;
		}
		else if((format[i] == '%') && (format[i + 1] == 'd')) {
			int nxt_dec = va_arg(second_arg, int);	
			num_bytes = _write_integer_count(nxt_dec, num_bytes, 10, "");
			i += 1;	
		}
		else if((format[i] == '%') && (format[i + 1] == 's')) {
			char* nxt_strng = va_arg(second_arg, char*);
			for(int c = 0; nxt_strng[c] != '\0'; c++) {
				num_bytes += 1;
			}
			i += 1;
		}
		else if((format[i] == '%') && (format[i + 1] == 'x')) {
			int nxt_hex = va_arg(second_arg, int);
			num_bytes = _write_integer_count(nxt_hex, num_bytes, 16, "0x");
			i += 1;
		}
		else if((format[i] == '%') && (format[i + 1] == 'b')) {
            int nxt_bin = va_arg(second_arg, int);
			num_bytes = _write_integer_count(nxt_bin, num_bytes, 2, "0b"); 
			i += 1; 
		}
		else if((format[i] == '%') && (format[i + 1] == '$')) {
			int actual_curr = va_arg(second_arg, int);
			if(actual_curr < 0) {
				num_bytes = _write_integer_count(actual_curr / 100, num_bytes, 10, "$");
				num_bytes = _write_integer_count(-1*((actual_curr % 100) / 10), num_bytes, 10, ".");
				num_bytes = _write_integer_count(-1*(actual_curr % 10), num_bytes, 10, "");
			}
			else {
				num_bytes = _write_integer_count(actual_curr / 100, num_bytes, 10, "$");
				num_bytes = _write_integer_count((actual_curr % 100) / 10, num_bytes, 10, ".");
				num_bytes = _write_integer_count(actual_curr % 10, num_bytes, 10, "");
			}
			i += 1;
		}
		else {
			num_bytes += 1;
		}
	}
	num_bytes += 1;


	return num_bytes;
}

int _write_integer(int n, char* output_str, int count, int radix, char* prefix) {

	unsigned int n_abs = n;

	if(n < 0) {
		output_str[count] = '-';
		n_abs = -n;
		count += 1;
	}

	 for(int idx = 0; prefix[idx] != '\0'; idx += 1) {
		 output_str[count] = prefix[idx];
		 count += 1;
	 }

	 int pow_val = 1;
	 for(int div = n_abs / radix; pow_val <= div; pow_val *= radix);

	 for(int save; pow_val != 0; pow_val /= radix) {
		 save = n_abs / pow_val;
		 if (save < 10) {
			 output_str[count] = '0' + save;
		 }
		 else {
			 output_str[count] = ('a' - 10) + save;
		 }
		 count += 1;
		 n_abs %= pow_val;
	 }
	 return count;
}

int _write_integer_count(int n, int num_bytes, int radix, char* prefix) {
    
    unsigned int n_abs = n;											// initially declare unsigned int n_abs to negative n to make absolute value and prevent overflow.

    if(n < 0) {
		num_bytes += 1;
		n_abs = -n;
	}
																	// loop for prefix.
    for(int idx_in_s = 0; prefix[idx_in_s] != '\0'; idx_in_s += 1) {
		num_bytes += 1;
	}

	int power = 1;
	for(int div = n_abs / radix; power <= div; power *= radix);
    
	while(power != 0) {
		num_bytes += 1;
		power /= radix;
	}

	return num_bytes;
}
