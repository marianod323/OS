#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int str_len(char* str){
	int i = 0;
	while(str[i] != '\0') i++;
	return i;
}

bool funny_compare(char* str){
	int len = str_len(str), i;
	char str_i[len];

	for(i = 0; i < len; i++){
		str_i[i] = str[len-i-1];
	}

	for(i = 0; i < len-1; i++){
		if(abs((int)str[i+1] - (int)str[i]) != abs((int)str_i[i+1] - (int)str_i[i]))
			return false;
	}

	return true;

}

int main(){
	char str[20];
	gets(str);
	printf(funny_compare(str) ? "true\n" : "false\n");
}
