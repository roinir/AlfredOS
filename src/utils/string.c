#include "string.h"

unsigned int strlen(const char* str){
	unsigned int i = 0;
	while (str[i] != '\0'){
		i++;
	}
	return i;
}

unsigned int strcmpbase(const char* str1, const char* str2, unsigned int is_case_sensetive){
	if (is_case_sensetive){
		while (*str1 != '\0' && *str2 != '\0'){
			if (cmp_all_combs(*str1, *str2)){
				return 1;
			}
			str1++;
			str2++;
		}	
	}
	else{
		while (*str1 != '\0' && *str2 != '\0'){
			if (cmp(*str1, *str2)){
				return 1;
			}
			str1++;
			str2++;
		}
	}
	return 0;
}

unsigned int strcasecmp(const char* str1, const char* str2){
	return strcmpbase(str1, str2, 1);
}

unsigned int strcmp(const char* str1, const char* str2){
	return strcmpbase(str1, str2, 0);
}

unsigned int cmp_all_combs(unsigned char val1, unsigned char val2){
	unsigned int val2_capital = (val2 >= 65 && val2 <= 90);	
	unsigned int val1_capital = (val1 >= 65 && val1 <= 90);

	if (val2_capital && !val1_capital){
		return cmp((val2_capital + 22), val1_capital);
	}
	else if (!val2_capital && val1_capital){
		return cmp(val2_capital, (val1_capital + 22));
	}
	return cmp(val2_capital, val1_capital);
}

unsigned int cmp(unsigned char val1, unsigned char val2){
	return val1 == val2;
}
