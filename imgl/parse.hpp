#pragma once

namespace imgl {
namespace n_parse {
	
	inline bool is_whitespace_c (char c) {	return c == ' ' || c == '\t'; }
	inline bool is_newline_c (char c) {	return c == '\n' || c == '\r'; }
	inline bool is_digit_c (char c) {		return c >= '0' && c <= '9'; }

	inline bool character (char** pcur, char c) {
		if (**pcur != c)
			return false;
		*pcur += 1;
		return true;
	}

	inline bool whitespace (char** pcur) {
		char* cur = *pcur;

		char c = *cur;
		if (!is_whitespace_c(c))
			return false;
		
		while (is_whitespace_c(*cur))
			++cur;

		*pcur = cur;
		return true;
	}

	inline bool newline (char** pcur) {
		char* cur = *pcur;

		char c = *cur;
		if (!is_newline_c(c))
			return false;

		++cur;

		char c2 = *cur;
		if (is_newline_c(c2) && c2 != c) {
			++cur; // skip '\n\r' and '\r\n'
		}

		*pcur = cur;
		return true;
	}

	inline bool uint (char** pcur, unsigned int* out) {
		char* cur = *pcur;
		
		if (!is_digit_c(*cur))
			return false;

		unsigned int i = 0;

		while (is_digit_c(*cur)) {
			char c = *cur;

			i *= 10;
			i += c -'0';

			++cur;
		}

		*out = i;
		*pcur = cur;
		return true;
	}
	inline bool sint (char** pcur, int* out) {
		char* cur = *pcur;

		bool negative = false;

		if (		*cur == '-' ) {
			negative = true;
			++cur;
		} else if ( *cur == '+' ) {
			negative = false;
			++cur;
		}

		whitespace(&cur);

		unsigned int i;

		if (!uint(&cur, &i))
			return false;

		*out = negative ? -(int)i : (int)i;
		*pcur = cur;
		return true;
	}

	inline bool float32 (char** pcur, float* out) {
		char* cur = *pcur;

		char* endptr;
		float ret = strtof(cur, &endptr);
		
		if (endptr == cur)
			return false;

		cur = endptr;

		*out = ret;
		*pcur = cur;
		return true;
	}

}
}
