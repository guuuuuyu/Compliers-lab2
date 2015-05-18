%option yylineno

%{
   #include <stdlib.h>
   #include <stdio.h>
   #include "syntax.tab.h"
   #include "node.h"
   int flex_error = 0;
   int yycolumn = 1;
   #define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
   yylloc.first_column = yycolumn; \
   yylloc.last_column = yycolumn + yyleng - 1; \
   yycolumn += yyleng;
%}

INT		[1-9][0-9]*|0
FLOAT		([0-9]+\.[0-9]+)|([0-9]*\.[0-9]+[Ee][+-]?[0-9]+)
ID		[a-zA-Z][_0-9a-zA-Z]*
OCT		(0[0-7]+)
HEX		(0[Xx][1-9a-fA-F][1-9a-fA-F]*)|0x0
RELOP		>|<|>=|<=|==|!=
TYPE		int|float

%%

{INT}		{
			yylval.no = (node*) malloc(sizeof(node));
			yylval.no->val.type_int = atoi(yytext);		
			return INT;	
//printf("Integer Value %d\n", atoi(yytext));	
		}
{OCT}           {
			yylval.no = (node*) malloc(sizeof(node));
			int i;
			int value = 0;
			for(i = 1; yytext[i] != '\0'; i ++){
				value = value * 8 + (yytext[i] - '0');
			}
			yylval.no->val.type_int = value;
			return OCT;
		}
{FLOAT}		{
			yylval.no = (node*) malloc(sizeof(node));			
			yylval.no->val.type_float = atof(yytext);
			return FLOAT;
//printf("FLOAT\n");
		}
{HEX}		{
			yylval.no = (node*) malloc(sizeof(node));
			int i;
			int value = 0;
			for(i = 2; yytext[i] != '\0'; i ++){
				value = value * 16 + (yytext[i] - '0');
			}
			yylval.no->val.type_int = value;
			return HEX;
		}
";"		{
			//printf("lineno: %d\n", yylineno);
			return SEMI;			
//printf(";\n");
		}
","		{	
			return COMMA;			
//printf(",\n");
		}
"="		{
			return ASSIGN;			
//printf("=\n");
		}
"+"		{
			return PLUS;			
//printf("+\n");
		}
"-"		{
			return MINUS;			
//printf("-\n");
		}
"*"		{
			return STAR;			
//printf("*\n");
		}
"/"		{
			return DIV;			
//printf("/\n");
		}
"&&"		{
			return AND;			
//printf("&&\n");
		}
"||"		{
			return OR;			
//printf("||\n");
		}
"."		{
			return DOT;			
//printf(".\n");
		}
"!"		{
			return NOT;			
//printf("!\n");
		}
"("		{
			return LP;			
//printf("(\n");
		}
")"		{
			return RP;
			//printf(")\n");
		}
"["		{
			return LB;
			//printf("[\n");
		}
"]"		{
			return RB;			
//printf("]\n");
		}
"{"		{
			return LC;
			//printf("{\n");
		}
"}"		{
                        return RC;
			//printf("}\n");
		}
{RELOP}		{ 	
			return RELOP;
			//printf("RELOP\n");
		}

int		{
			yylval.no = (node*) malloc(sizeof(node));		
			memset(yylval.no->val.type_Type,0,20);
            		memcpy(yylval.no->val.type_Type,yytext,20);// val.type_Type = "int"
			return TYPE;
			//printf("int\n");
		}
float		{
			yylval.no = (node*) malloc(sizeof(node));		
			memset(yylval.no->val.type_Type,0,20);
            		memcpy(yylval.no->val.type_Type,yytext,20);
			return TYPE;
			//printf("float\n");
		}

struct		{
			return STRUCT;
			//printf("struct\n");
		}
return 		{
			return RETURN;
			//printf("return\n");
		}
if		{
			return IF;
			//printf("if\n");
		}
else		{
			return ELSE;
			//printf("else\n");
		}
while		{
			return WHILE;
			//printf("while\n");
		}
{ID}		{ 		
 			yylval.no = construct("ID", yylineno, 3);		
			memset(yylval.no->val.type_ID,0,20);
            		memcpy(yylval.no->val.type_ID,yytext,20);
			return ID;
			//printf("ID\n");
		}
[ \t]+		{}
\n              {}

"//"   		{ 
				char ch = input();
				while (ch != '\n')
					ch = input();
		}
"/*"		{
				char ch;
				while(1){
					ch = input();
					if(ch != '*') continue;
					else{
						char ch0 = input();
						if(ch0 == '/'){
							ch = input();
							break;
						}
					}
				}
		}
.              {
			printf("Error type A at Line %d: Mysterious characters \'%s\'\n",
			yylineno, yytext);
			flex_error = 1;
		}		
%%
