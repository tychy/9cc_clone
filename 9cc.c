#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum{
	TK_NUM = 156,
	TK_EOF,
};
typedef struct{
	int ty;
	int val;
	char *input;
} Token;
//suppose maximum token size 100
Token tokens[100];

//report error
void error(char *fmt,...){
	va_list ap;
	va_start(ap,fmt);
	vfprintf(stderr,fmt,ap);
	fprintf(stderr,"\n");
	exit(1);
}
void tokenize(char *p){
	int i = 0;
	while (*p){
		if (isspace(*p)){
			p++;
			continue;
		}
		if (*p=='+'||*p=='-'){
			tokens[i].ty = *p;
			tokens[i].input = p;
			i++;
			p++;
			continue;
		}
		if(isdigit(*p)){
			tokens[i].ty = TK_NUM;
			tokens[i].input = p;
			tokens[i].val = strtol(p,&p,10);
			i++;
			continue;
		}
		error("cannot tokenize: %s",p);
		exit(1);
	}
	tokens[i].ty = TK_EOF;
	tokens[i].input = p;
}

int main(int argc,char **argv){
	if(argc!=2){
		fprintf(stderr,"invalid number of arg\n");
		return 1;
	}
	tokenize(argv[1]);
	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");
	if(tokens[0].ty != TK_NUM)
		error("first item must be integer");
	printf("	mov rax, %d\n",tokens[0].val);
	
	int i = 1;
	while (tokens[i].ty != TK_EOF){
		if (tokens[i].ty == '+'){
			i++;
			if (tokens[i].ty != TK_NUM)
				error("unexpected token: %s",tokens[i].input);
			printf("	add rax, %d\n",tokens[i].val);
			i++;
			continue;
		}
		if (tokens[i].ty == '-'){
			i++;
			if (tokens[i].ty != TK_NUM)
				error("unexpected token: %s\n",tokens[i].input);
			printf("	sub rax, %d\n",tokens[i].val);
			i++;
			continue;
		}
		error("unexpected token: %s\n",tokens[i].input);
		
		}
	
	printf("	ret\n");
	return 0;
}

