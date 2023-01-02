#include "../include/errno.h"
#include "../include/limits.h"

unsigned long strtoul0(const char **ps, int base, char *err_flag);

long strtol(const char *s, const char **endp, int base) {
	const char *s0 = s, *s1;
	char sign = 0, err_flag;
	unsigned long val;
	while (*s != '\0' && *s <= ' ')
		s++;
	if (*s == '-') {
		sign = 1;
		s++;
	}
	while (*s != '\0' && *s <= ' ')
		s++;
	s1 = s;
	val = strtoul0(&s, base, &err_flag);
	if (s == s1)
		s = s0;
	if (endp)
		*endp = s;
	if (val > LONG_MAX) {
		err_flag = 1;
		val = LONG_MAX;
		if (sign)
			val = LONG_MIN;
	}
	if (err_flag == 0 && sign != 0)
		val = - val;
	if (err_flag)
		errno = ERANGE;
	return val;
}
