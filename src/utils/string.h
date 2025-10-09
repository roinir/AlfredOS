#ifndef VGA_DRIVER 
#define VGA_DRIVER

unsigned int strlen(const char* str);

unsigned int strcmpbase(const char* str1, const char* str2, unsigned int is_case_sensetive);

unsigned int strcasecmp(const char* str1, const char* str2);

unsigned int strcmp(const char* str1, const char* str2);

unsigned int cmp_all_combs(unsigned char val1, unsigned char val2);

unsigned int cmp(unsigned char val1, unsigned char val2);

#endif
