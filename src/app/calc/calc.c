#include "../../include/stdio.h"

#define INVALID -0x7fffffff

int strtol(char *s, char **endp, int base);

char *skip_space(char *p);
int get_num(char **pp, int priority);

void HariMain(void) {
    int i;
    char s[30], *p;

    cmdline(s, 30);
    for (p = s; *p > ' '; p++) {
    }  // Skip until space
    i = get_num(&p, 9);
    if (i == INVALID) {
        puts("error!\n");
    } else {
        sprintf(s, "= %d = 0x%x\n", i, i);
        puts(s);
    }
    exit();
}

char *skip_space(char *p) {
    for (; *p == ' '; p++) {
    }  // Skip until space
    return p;
}

int get_num(char **pp, int priority) {
    char *p = *pp;
    int i = INVALID, j;
    p = skip_space(p);

    // Single sect compute
    if (*p == '+') {
        p = skip_space(p + 1);
        i = get_num(&p, 0);
    } else if (*p == '-') {
        p = skip_space(p + 1);
        i = get_num(&p, 0);
        if (i != INVALID) {
            i = -i;
        }
    } else if (*p == '~') {
        p = skip_space(p + 1);
        i = get_num(&p, 0);
        if (i != INVALID) {
            i = ~i;
        }
    } else if (*p == '(') {  // round bracket
        p = skip_space(p + 1);
        i = get_num(&p, 9);
        if (*p == ')') {
            p = skip_space(p + 1);
        } else {
            i = INVALID;
        }
    } else if ('0' <= *p && *p <= '9') {  // num
        i = strtol(p, &p, 0);
    } else {  // Error
        i = INVALID;
    }

    // couple sects compute
    for (;;) {
        if (i == INVALID) {
            break;
        }
        p = skip_space(p);
        if (*p == '+' && priority > 2) {
            p = skip_space(p + 1);
            j = get_num(&p, 2);
            if (j != INVALID) {
                i += j;
            } else {
                i = INVALID;
            }
        } else if (*p == '-' && priority > 2) {
            p = skip_space(p + 1);
            j = get_num(&p, 2);
            if (j != INVALID) {
                i -= j;
            } else {
                i = INVALID;
            }
        } else if (*p == '*' && priority > 1) {
            p = skip_space(p + 1);
            j = get_num(&p, 1);
            if (j != INVALID) {
                i *= j;
            } else {
                i = INVALID;
            }
        } else if (*p == '/' && priority > 1) {
            p = skip_space(p + 1);
            j = get_num(&p, 1);
            if (j != INVALID && j != 0) {
                i /= j;
            } else {
                i = INVALID;
            }
        } else if (*p == '%' && priority > 1) {
            p = skip_space(p + 1);
            j = get_num(&p, 1);
            if (j != INVALID && j != 0) {
                i %= j;
            } else {
                i = INVALID;
            }
        } else if (*p == '<' && p[1] == '<' && priority > 3) {
            p = skip_space(p + 2);
            j = get_num(&p, 3);
            if (j != INVALID && j != 0) {
                i <<= j;
            } else {
                i = INVALID;
            }
        } else if (*p == '>' && p[1] == '>' && priority > 3) {
            p = skip_space(p + 2);
            j = get_num(&p, 3);
            if (j != INVALID && j != 0) {
                i >>= j;
            } else {
                i = INVALID;
            }
        } else if (*p == '&' && priority > 4) {
            p = skip_space(p + 1);
            j = get_num(&p, 4);
            if (j != INVALID) {
                i &= j;
            } else {
                i = INVALID;
            }
        } else if (*p == '^' && priority > 5) {
            p = skip_space(p + 1);
            j = get_num(&p, 5);
            if (j != INVALID) {
                i ^= j;
            } else {
                i = INVALID;
            }
        } else if (*p == '|' && priority > 6) {
            p = skip_space(p + 1);
            j = get_num(&p, 6);
            if (j != INVALID) {
                i |= j;
            } else {
                i = INVALID;
            }
        } else {
            break;
        }
    }
    p = skip_space(p);
    *pp = p;
    return i;
}
