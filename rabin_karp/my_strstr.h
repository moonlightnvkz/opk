#ifndef MY_STRSTR_H
#define MY_STRSTR_H

#ifdef _DEBUG
#define unit_static
#else
#define unit_static static
#endif

char * my_strstr(char *haystack, const char *needle);

#endif // MY_STRSTR_H
