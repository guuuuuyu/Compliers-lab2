%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "lex.yy.c"

extern node *root;
extern int flex_error;
int bison_error = 0;
%}
/* declared types */
%union{
	node* no;
};
/* declared tokens */
%token <no> INT ID FLOAT RETURN IF WHILE TYPE STRUCT OCT HEX
%token <no> COMMA SEMI LC RC error
%right <no> NOT ASSIGN
%left <no> AND OR RELOP PLUS MINUS STAR DIV LP RP LB RB DOT
%type  <no> Program ExtDefList ExtDef ExtDecList Specifier Exp  
%type  <no> StructSpecifier OptTag Tag VarDec FunDec VarList Args
%type  <no> ParamDec CompSt StmtList Stmt DefList Def DecList Dec
%nonassoc <no> LOWER_THAN_ELSE
%nonassoc <no> ELSE

%%
Program         :  ExtDefList   		{
							$$ = construct("Program", @$.first_line, 0);
                					add_child(&$$, $1);
							root = $$;
							if(flex_error == 0 && bison_error == 0) //print_tree(root, 0);
							yuyifenxi(root);
						}
		;
ExtDefList	:  ExtDef ExtDefList 		{
							$$ = construct("ExtDefList", @$.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
						}
		|  				{	$$ = NULL;
						}	
		;			
ExtDef 		:  Specifier ExtDecList SEMI	{
							$3 = construct("SEMI", @3.first_line, 0);
							$$ = construct("ExtDef", @$.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);				
						}
		|  Specifier SEMI		{
							$2 = construct("SEMI", @2.first_line, 0);
							$$ = construct("ExtDef", @$.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
						}
		|  Specifier FunDec CompSt 	{
							$$ = construct("ExtDef", @$.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		|	Specifier FunDec SEMI {
							$3 = construct("SEMI", @3.first_line, 0);
							$$ = construct("ExtDef", @$.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		;
ExtDecList	:  VarDec			{
							$$ = construct("ExtDecList", @$.first_line, 0);
							add_child(&$$, $1);
						}
		|  VarDec COMMA ExtDecList	{
							$$ = construct("ExtDecList", @$.first_line, 0);
							$2 = construct("COMMA", @2.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		;
Specifier	:  TYPE				{
							$1 = construct("TYPE",@1.first_line,4);							
							$$ = construct("Specifier", @$.first_line, 0);
							memset($1->val.type_Type, 0, 20);
							memcpy($1->val.type_Type, yylval.no->val.type_Type,20);
							add_child(&$$, $1);
						}
		|  StructSpecifier		{
							$$ = construct("Specifier", @$.first_line, 0);
							add_child(&$$, $1);
						}
		;
StructSpecifier :  STRUCT OptTag LC DefList RC	{	//printf("halaha\n");
							$1 = construct("STRUCT", @1.first_line, 0);
							$3 = construct("LC", @3.first_line, 0);
							$5 = construct("RC", @5.first_line, 0);
							$$ = construct("StructSpecifier", @$.first_line, 0);
							//printf("lalaha\n");
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
							add_child(&$$, $4);
							add_child(&$$, $5);
						}
		|  STRUCT Tag			{
							$$ = construct("StructSpecifier", @$.first_line, 0);
							$1 = construct("STRUCT", @1.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
						}
		;
OptTag		:  ID				{
							//$1 = construct("ID", @1.first_line, 3);
							$$ = construct("OptTag", @$.first_line, 0);
							//memset($1->val.type_ID, 0, 20);
							//memcpy($1->val.type_ID, yylval.no->val.type_ID,20);
							add_child(&$$, $1);
						}
		|				{	$$ = NULL;
						}
		;
Tag		:  ID				{
							//$1 = construct("ID", @1.first_line, 3);
							$$ = construct("Tag", @$.first_line, 0);
							//memset($1->val.type_ID, 0, 20);
							//memcpy($1->val.type_ID, yylval.no->val.type_ID,20);
							add_child(&$$, $1);
						}
		;
VarDec 		:  ID				{
							//$1 = construct("ID", @1.first_line, 3);
							$$ = construct("VarDec", @$.first_line, 0);
							//memset($1->val.type_ID, 0, 20);
							//memcpy($1->val.type_ID, yylval.no->val.type_ID,20);
							add_child(&$$, $1);
						}
		|  VarDec LB INT RB		{
							$$ = construct("VarDec", @$.first_line, 0);
							$2 = construct("LB", @2.first_line, 0);
							$4 = construct("RB", @4.first_line, 0);
							$3 = construct("INT", @3.first_line, 1);
							$3->val.type_int = yylval.no->val.type_int;
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
							add_child(&$$, $4);
						}
		;
FunDec		:  ID LP VarList RP		{
							//$1 = construct("ID", @1.first_line, 3);
							$$ = construct("FunDec", @$.first_line, 0);
							//memset($1->val.type_ID, 0, 20);
							//memcpy($1->val.type_ID, yylval.no->val.type_ID,20);
							$2 = construct("LP", @2.first_line, 0);
							$4 = construct("RP", @4.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
							add_child(&$$, $4);
						}
		|  ID LP RP			{
							//$1 = construct("ID", @1.first_line, 3);
							$$ = construct("FunDec", @$.first_line, 0);
							//memset($1->val.type_ID, 0, 20);
							//memcpy($1->val.type_ID, yylval.no->val.type_ID,20);
							$2 = construct("LP", @2.first_line, 0);
							$3 = construct("RP", @3.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		|  error RP                  // {printf("uhsadfiugdsaufg\n");}
		;
VarList		:  ParamDec COMMA VarList	{
							$$ = construct("VarList", @$.first_line, 0);
							$2 = construct("COMMA", @2.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		|  ParamDec			{
							$$ = construct("VarList", @$.first_line, 0);
							add_child(&$$, $1);
						}
		;
ParamDec	:  Specifier VarDec		{
							$$ = construct("ParamDec", @$.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
						}
		;
CompSt		:  LC DefList StmtList RC	{
							$1 = construct("LC", @1.first_line, 0);
							$4 = construct("RC", @4.first_line, 0);
							$$ = construct("CompSt", @$.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
							add_child(&$$, $4);
						}
		|  error RC			//{printf("error1\n");}
		//|  error SEMI
		|  error RP
		;
StmtList	:  Stmt StmtList		{
							$$ = construct("StmList", @$.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
						}
		|				{	$$ = NULL;
						}
		;
Stmt		:  Exp SEMI			{
							$$ = construct("Stmt", @$.first_line, 0);
							$2 = construct("SEMI", @2.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
             					}
		|  error SEMI			//{printf("error2\n");}	
		|  CompSt			{
							$$ = construct("Stmt", @$.first_line, 0);
							add_child(&$$, $1);	
						}
		|  RETURN Exp SEMI		{
							$$ = construct("Stmt", @$.first_line, 0);
							$1 = construct("RETURN", @1.first_line, 0);
							$3 = construct("SEMI", @3.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		|  IF LP Exp RP Stmt		{
							$1 = construct("IF", @1.first_line, 0);
							$2 = construct("LP", @2.first_line, 0);
							$4 = construct("RP", @4.first_line, 0);
							$$ = construct("Stmt", @$.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
							add_child(&$$, $4);
							add_child(&$$, $5);
						}
		|  IF LP Exp RP Stmt ELSE Stmt	{
							$1 = construct("IF", @1.first_line, 0);
							$2 = construct("LP", @2.first_line, 0);
							$4 = construct("RP", @4.first_line, 0);
							$6 = construct("ELSE", @6.first_line, 0);
							$$ = construct("Stmt", @$.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
							add_child(&$$, $4);
							add_child(&$$, $5);
							add_child(&$$, $6);
							add_child(&$$, $7);
						}
		|  WHILE LP Exp RP Stmt		{
							$1 = construct("WHILE", @1.first_line, 0);
							$2 = construct("LP", @2.first_line, 0);
							$4 = construct("RP", @4.first_line, 0);
							$$ = construct("Stmt", @$.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
							add_child(&$$, $4);
							add_child(&$$, $5);
						}
		;
DefList		:  Def DefList			{
							$$ = construct("DefList", @$.first_line, 0);
							add_child(&$$, $1);
							//printf("lalaha\n");
							add_child(&$$, $2);
							//printf("lalaha\n");
						}
		| 				{	$$ = NULL;
						}
		;
Def		:  Specifier DecList SEMI	{
							$$ = construct("Def", @$.first_line, 0);
							$3 = construct("SEMI", @3.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		;
DecList		:  Dec				{
							$$ = construct("DecList", @$.first_line, 0);
							add_child(&$$, $1);
						}
		|  Dec COMMA DecList		{
							$$ = construct("DecList", @$.first_line, 0);
							$2 = construct("COMMA", @2.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		;
Dec 		:  VarDec			{
							$$ = construct("Dec", @$.first_line, 0);
							add_child(&$$, $1);
						}
		|  VarDec ASSIGN Exp		{
							$$ = construct("Dec", @$.first_line, 0);
							$2 = construct("ASSIGN", @2.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		;
Exp		:  Exp ASSIGN Exp		{
							$$ = construct("Exp", @$.first_line, 0);
							$2 = construct("ASSIGN", @2.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		|  Exp AND Exp			{
							$$ = construct("Exp", @$.first_line, 0);
							$2 = construct("AND", @2.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		|  Exp OR Exp			{
							$$ = construct("Exp", @$.first_line, 0);
							$2 = construct("OR", @2.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		|  Exp RELOP Exp		{
							$$ = construct("Exp", @$.first_line, 0);
							$2 = construct("RELOP", @2.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		|  Exp PLUS Exp			{
							$$ = construct("Exp", @$.first_line, 0);
							$2 = construct("PLUS", @2.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		|  Exp MINUS Exp		{
							$$ = construct("Exp", @$.first_line, 0);
							$2 = construct("MINUS", @2.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		|  Exp STAR Exp			{
							$$ = construct("Exp", @$.first_line, 0);
							$2 = construct("STAR", @2.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		|  Exp DIV Exp			{
							$$ = construct("Exp", @$.first_line, 0);
							$2 = construct("DIV", @2.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
  		|  LP Exp RP			{
							$1 = construct("LP", @1.first_line, 0);
							$3 = construct("RP", @3.first_line, 0);
							$$ = construct("Exp", @$.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		|  MINUS Exp			{
							$1 = construct("MINUS", @1.first_line, 0);
							$$ = construct("Exp", @$.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
						}
		|  NOT Exp			{
							$1 = construct("NOT", @1.first_line, 0);
							$$ = construct("Exp", @$.first_line, 0);
							
							add_child(&$$, $1);
							add_child(&$$, $2);
						}
		|  ID LP Args RP		{// han shu diao yong
							//$1 = construct("ID", @1.first_line, 3);
							//memset($1->val.type_ID, 0, 20);
							//memcpy($1->val.type_ID, yylval.no->val.type_ID,20);
							$$ = construct("Exp", @$.first_line, 0);
							
							$2 = construct("LP", @2.first_line, 0);
							$4 = construct("RP", @4.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
							add_child(&$$, $4);
						}
		|  ID LP RP			{
							//$1 = construct("ID", @1.first_line, 3);
							//memset($1->val.type_ID, 0, 20);
							//memcpy($1->val.type_ID, yylval.no->val.type_ID,20);
							$$ = construct("Exp", @$.first_line, 0);
							
							$2 = construct("LP", @2.first_line, 0);
							$3 = construct("RP", @3.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		|  Exp LB Exp RB		{
							$$ = construct("Exp", @$.first_line, 0);
							$2 = construct("LB", @2.first_line, 0);
							$4 = construct("RB", @4.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
							add_child(&$$, $4);
		
						}
		|  Exp DOT ID			{
							$$ = construct("Exp", @$.first_line, 0);
							$2 = construct("DOT", @2.first_line, 0);
							//$3 = construct("ID", @3.first_line, 3);
							//memset($3->val.type_ID, 0, 20);
							//memcpy($3->val.type_ID, yylval.no->val.type_ID,20);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		|  ID				{
							//$1 = construct("ID", @1.first_line, 3);
							//memset($1->val.type_ID, 0, 20);
							//memcpy($1->val.type_ID, yylval.no->val.type_ID,20);
							$$ = construct("Exp", @$.first_line, 0);
					
							add_child(&$$, $1);
						}
		|  INT 				{
							$1 = construct("INT", @1.first_line, 1);
							$1->val.type_int = yylval.no->val.type_int;
							$$ = construct("Exp", @$.first_line, 0);
							
							add_child(&$$, $1);
						}
		|  FLOAT			{
							$1 = construct("FLOAT", @1.first_line, 2);
							$1->val.type_float = yylval.no->val.type_float;
							$$ = construct("Exp", @$.first_line, 0);
							
							add_child(&$$, $1);
						}
		|  OCT				{
							$1 = construct("INT", @1.first_line, 1);
							$1->val.type_int = yylval.no->val.type_int;
							$$ = construct("Exp", @$.first_line, 0);
							
							add_child(&$$, $1);
						}
		|  HEX				{	$1 = construct("INT", @1.first_line, 1);
							$1->val.type_int = yylval.no->val.type_int;
							$$ = construct("Exp", @$.first_line, 0);
							
							add_child(&$$, $1);
						}
		| error RP                      //{printf("error3\n");}
		| error RB
		;	
Args		:  Exp COMMA Args		{
							$$ = construct("Args", @$.first_line, 0);
							$2 = construct("COMMA", @2.first_line, 0);
							add_child(&$$, $1);
							add_child(&$$, $2);
							add_child(&$$, $3);
						}
		|  Exp				{
							$$ = construct("Args", @$.first_line, 0);
							add_child(&$$, $1);
						}
		;
%%
yyerror(char* msg) {
	printf("Error type B at line %d: %s \n", yylineno, msg);
	bison_error = 1;
}
