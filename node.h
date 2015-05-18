#ifndef _NODE_H_
#define _NODE_H_

#include "symboltable.h"

typedef struct Node{
        char name[20];
        int lineno;
        int flag;//1 stands for INT, 2 stands for FLOAT, 3 stands for ID, 4 stands for TYPE, 0 stands for others
        union{
        	int type_int;
        	float type_float;
        	char type_ID[20];
                char type_Type[20];// int or float
        }val;
        int numOfChildren;
        struct Node* children[10];
}node;

node* construct(char [], int, int);
void add_child(node **, node *);
void print_tree(node *, int);
void visit_tree(node *);//bian li sheng cheng de yu fa fen xi shu wan cheng yu yi fen xi
void visit_DecList(node *, Type, int);//
void visit_ExtDef(node *);
void visit_ExtDecList(node *, Type);// need to know the type of these variables when inserting them to the variables table
Type visit_Specifier(node *);
Type visit_StructSpecifier(node *);
Type visit_FunDec(node *, int, Type);// the integer indicates the function is defined or declared
Type check_struct_table(char *);
int check_variable_table(char *, int);// if there is no need to fill the variables table, return 0
void visit_DefList(node *, int);
void visit_VarList(node *, FieldList);
int check_function_table(Type);// if there is no need to fill the function table, return 0
int compare_RET(Type, Type);// xiang deng fan hui 1, bu xiang deng fan hui 0
int compare_ARRAY(Type, Type);
int compare_STRUCTURE(Type, Type);
int compare_FieldList(FieldList, FieldList);
Type type_visit_VarDec(node*, Type);
void name_visit_VarDec(node*, char*);
void visit_CompSt(node*, int, Type);
FieldList visit_DefList_in_struct(node*);
void visit_DecList_in_struct(node*, Type, FieldList);
void visit_StmtList(node*, int, Type);
Type visit_Exp(node*, int);
void check_fun_def();
int compare_Args(FieldList, node*, int);// xiang deng fan hui 1, bu xiang deng fan hui 0
#endif
