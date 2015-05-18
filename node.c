#include "node.h"
#include "symboltable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include  <unistd.h>

Item variable_table[1000];
Item variable_scope[1000];
Item struct_table[1000];
Item function_table[1000];

int current_scope;

void print_error(int error_type, int lineno){
	switch(error_type){
		case 1: printf("Error type 1 at line %d: Undefined variable\n", lineno); break;
		case 2: printf("Error type 2 at line %d: Undefined function\n", lineno); break;
		case 3: printf("Error type 3 at line %d: Redefined variable\n", lineno); break;
		case 4: printf("Error type 4 at line %d: Redefined function\n", lineno); break;
		case 5: printf("Error type 5 at line %d: Type mismatched\n", lineno); break;
		case 6: printf("Error type 6 at line %d: The left-hand side of an assignment must be a variable\n", lineno); break;
		case 7: printf("Error type 7 at line %d: Operands type mismatched\n", lineno); break;
		case 8: printf("Error type 8 at line %d: The return type mismatched\n", lineno); break;
		case 9: printf("Error type 9 at line %d: The method is not applicable to the defined function\n", lineno); break;
		case 10: printf("Error type 10 at line %d: The symbol before \"[]\" must be an array\n", lineno); break;
		case 11: printf("Error type 11 at line %d: The symbol before \"()\" must be a function\n", lineno); break;
		case 12: printf("Error type 12 at line %d: Operands type mistaken\n", lineno); break;
		case 13: printf("Error type 13 at line %d: Illegal use of \".\"\n", lineno); break;
		case 14: printf("Error type 14 at line %d: Un-existed field\n", lineno); break;
		case 15: printf("Error type 15 at line %d: Redefined field or Illegal initialization\n", lineno); break;
		case 16: printf("Error type 16 at line %d: Duplicated name\n", lineno); break;
		case 17: printf("Error type 17 at line %d: Undefined struct\n", lineno); break;
		case 18: printf("Error type 18 at line %d: Undefined function\n", lineno); break;
		case 19: printf("Error type 19 at line %d: Inconsistent declaration of function\n", lineno); break;
	}
}


node* construct(char n[], int lno, int f){
	//printf("construct%s\n", n);
	node *p = (node*)malloc(sizeof(node));
	memset(p->name, 0, 20);
	memcpy(p->name, n, 20);
	p->lineno = lno;
	p->flag = f;
	p->numOfChildren = 0;
	int i;
	for(i = 0; i < 10; i ++)
		p->children[i] = NULL;
	//printf("CON\n");
	return p;
}

void add_child(node **parent, node *child){
	if(child != NULL){
		(*parent)->children[(*parent)->numOfChildren] = child;
		(*parent)->numOfChildren ++;
	}
}

void print_tree(node *p, int n){//n yong yu biao ji kong ge ge shu
	if(p->numOfChildren != 0){
		//printf("%d\n", p->numOfChildren);		
		int i;
		for(i = 0; i < n; i ++)
			printf(" ");
		if(p->flag == 0){
			if(p->lineno != 0) printf("%s(%d)\n", p->name, p->lineno);
			else printf("%s\n", p->name);
		}
		else if(p->flag == 1){//INT
			printf("INT: ");
			printf("%d\n", p->val.type_int);
		}	
		else if(p->flag == 2){//FLOAT
			printf("FLOAT: ");
			printf("%f\n", p->val.type_float);
		}
		else if(p->flag == 3){//ID
			printf("ID: ");
			printf("%s\n", p->val.type_ID);
		}
		else if(p->flag == 4){//TYPE
			printf("TYPE: ");
			printf("%s\n", p->val.type_Type);
		}
		
		int j;
		for(j = 0; j < p->numOfChildren; j ++)
			print_tree(p->children[j], n + 1);				
       	}
	else{
		int i;
		for(i = 0; i < n; i ++)
			printf(" ");
		if(p->flag == 0){
			if(p->lineno != 0) printf("%s(%d)\n", p->name, p->lineno);
			else printf("%s\n", p->name);
		}
		else if(p->flag == 1){//INT
			printf("INT: ");
			printf("%d\n", p->val.type_int);
		}	
		else if(p->flag == 2){//FLOAT
			printf("FLOAT: ");
			printf("%f\n", p->val.type_float);
		}
		else if(p->flag == 3){//ID
			printf("ID: ");
			printf("%s\n", p->val.type_ID);
		}
		else if(p->flag == 4){//TYPE
			printf("TYPE: ");
			printf("%s\n", p->val.type_Type);
		}
	}
}
/* lab 2 from here */
int compare_FieldList(FieldList fl1, FieldList fl2){// bi jiao han shu xing can lie biao lei xing shi fou yi zhi
	while(fl1 != NULL && fl2 != NULL){
		if(fl1 -> type -> kind == BASIC){
			if(fl2 -> type -> kind == BASIC && fl1 -> type -> u.basic == fl2 -> type -> u.basic){
				fl1 = fl1 -> tail;
				fl2 = fl2 -> tail;
			}
			else return 0;
		}
		else if(fl1 -> type -> kind == ARRAY){
			if(fl2 -> type -> kind == ARRAY){
				if(compare_ARRAY(fl1 -> type, fl2 -> type) == 1){
					fl1 = fl1 -> tail;
					fl2 = fl2 -> tail;
				}
				else return 0;
			}
			else return 0;
		}
		else if(fl1 -> type -> kind == STRUCTURE){
			if(fl2 -> type -> kind == STRUCTURE){
				if(compare_STRUCTURE(fl1 -> type, fl2 -> type) == 1){
					fl1 = fl1 -> tail;
					fl2 = fl2 -> tail;
				}
				else return 0;
			}
			else return 0;
		}
		else{
			printf("wtf\n");
			return 0;
		}
	}
	if(fl1 == NULL && fl2 == NULL)
		return 1;
	else return 0;	
}

int compare_STRUCTURE(Type s1, Type s2){// bi jiao han shu fan hui lei xing ji han shu can shu de jie gou ti, zhi neng xing ru struct A a 
	if(strcmp(s1 -> u.structure.name, s2 -> u.structure.name) == 0)
		return 1;
	else return 0;
}

int compare_ARRAY(Type a1, Type a2){// ************** iterative algorithm **************
	if(a1 -> u.array.size == a2 -> u.array.size){
		if(a1 -> u.array.elem -> kind == BASIC && a2 -> u.array.elem -> kind == BASIC){// di gui chu kou
			if(a1 -> u.array.elem -> u.basic == a2 -> u.array.elem -> u.basic)
				return 1;
			else return 0;
		}
		else if(a1 -> u.array.elem -> kind != a2 -> u.array.elem -> kind) return 0;
		else{
			return compare_ARRAY(a1 -> u.array.elem, a2 -> u.array.elem);// or return compare_ARRAY??
		}
	}
	else return 0;
}
int compare_RET(Type ret1, Type ret2){
	if(ret1 == NULL || ret2 == NULL){
		return 0;
	}
	else{
		if(ret1 -> kind == BASIC){
			if(ret2 -> kind == BASIC && ret1 -> u.basic == ret2 -> u.basic)
				return 1;
			else return 0;
		}

		else if(ret1 -> kind == ARRAY){// qi shi bu ke neng you zhe zhong qing kuang
			if(ret2 -> kind == ARRAY){
				if(compare_ARRAY(ret1, ret2) == 1)
					return 1;
				else return 0;
			}
			else return 0;
		}
		else if(ret1 -> kind == STRUCTURE){
			//////////////////////////////han shu fan hui zhi lei xing bu ke neng shi ni ming jie gou ti
			if(ret2 -> kind == STRUCTURE){
				if(compare_STRUCTURE(ret1, ret2) == 1)
					return 1;
				else return 0;
			}
			else return 0;
		}
	}
}
int check_function_table(Type func){
	if(func -> kind != FUNCTION){
		printf("Unexpected bug!\n");
		return 0;
	}
	else{
		char *name;
		name = (char*)malloc(sizeof(20));
		strcpy(name, (const char*)func -> u.function.name);
		int location = DJB_hash(name);
		if(function_table[location] == NULL)
			return 1;
		else{
			Item it = function_table[location];
			for(; it != NULL; it = it -> nextr){
				if(strcmp(it -> name, func -> u.function.name) == 0){
					Type ty = it -> type;
					if(ty -> u.function.def_or_not == 1 && func -> u.function.def_or_not == 1){// han shu chong ding yi
						print_error(4, func -> u.function.lineno);
						return 0;
					}
					else{
						//bijiao fanhuizhi  bijiao can shu lie biao
						if(compare_RET(func -> u.function.RET, ty -> u.function.RET) == 1 && compare_FieldList(func -> u.function.args, ty -> u.function.args) == 1){/////////////
							if(ty -> u.function.def_or_not == 0){
								ty -> u.function.def_or_not = func -> u.function.def_or_not;
								return 0;// no need to fill the table
							}
							else{
								printf("declaration after definition\n");
								return 0;
							}
						}
						else{
							print_error(19, func -> u.function.lineno);
							return 0;
						} 
					}
				}
			}

			return 1;// mei you zhao dao tong ming han shu, xu yao cha ru biao zhong
		}
	}
}
int check_variable_table(char *na, int sco){// cha zhao zhi ding ceng shu zhi ding ming zi de bian liang
	char* name;
	name = (char*)malloc(sizeof(20));
	strcpy(name, (const char*)na);
	int location = DJB_hash(name);
	if(variable_table[location] == NULL)
		return 1;
	else{
		Item it = variable_table[location];
		for(; it != NULL; it = it -> nextr){
			if(strcmp(it -> name, na) == 0 && it -> scope == sco){
				return 0;
			}
		}

		return 1;
	}
}

Type check_struct_table(char *name){
	char *na;
	na = (char*)malloc(sizeof(20));
	strcpy(na, (const char*)name);

	int location = DJB_hash(na);
	if(struct_table[location] == NULL)
		return NULL;
	else{
		Item it = struct_table[location];
		for(; it != NULL; it = it -> nextr){
			if(strcmp(it -> name, name) == 0){
				Type type = it -> type;
				return type;
			}
		}
		return NULL;
	}
	return NULL;
}


void visit_DecList_in_struct(node *p, Type ty, FieldList fl){
	//printf("sbhz\n");
	node *q = p -> children[0] -> children[0];//VarDec
	//printf("%d\n", p -> children[0] -> numOfChildren);
	char *NAME;
	NAME = (char*)malloc(sizeof(20));
	name_visit_VarDec(q, NAME);
	fl -> name = (char*)malloc(sizeof(20));
	strcpy(fl -> name, (const char*)NAME);
	fl -> type = type_visit_VarDec(q, ty);
	//printf("DecList%s\n", fl -> name);
	if(p -> numOfChildren == 1){// DecList -> Dec
		fl -> tail = NULL;
	}
	else{// DecList -> Dec DecList
		//printf("DecList%s\n", fl -> name);
		fl -> tail = (FieldList)malloc(sizeof(FieldList_));//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
		visit_DecList_in_struct(p -> children[2], ty, fl -> tail);
		//printf("name%s\n", fl -> name);
	}
	if(p -> children[0] -> numOfChildren == 1){
		
	}
	else{
		print_error(15, q -> lineno);// jie gou ti ding yi zhong bu ke yi fu zhi
	}
}

FieldList visit_DefList_in_struct(node *p){// return structure's attrs           ***********iterative algorithm *************8
	//printf("ru guo bu shu chu zhe ju jiu hui chu xian duan cuo wu, wo ye bu zhi dao wei shen me\n");
	//usleep(1000);
	if(p -> children[1] == NULL){// Def
		//printf("\n");
		//printf("%s\n", p -> name);
		//printf("sbhz\n");
		Type type = visit_Specifier(p ->children[0] -> children[0]);
		/*Type type = (Type)malloc(sizeof(Type_));
		type -> kind = BASIC;
		type -> u.basic = 1;*/
		//printf("sbhz\n");
		FieldList fl = (FieldList)malloc(sizeof(FieldList_));

		visit_DecList_in_struct(p -> children[0] -> children[1], type, fl);

		return fl;
	}
	else{// Def DefList
		Type type = visit_Specifier(p ->children[0] -> children[0]);
		FieldList fl = (FieldList)malloc(sizeof(FieldList_));
		//printf("shenmegui\n");
		visit_DecList_in_struct(p -> children[0] -> children[1], type, fl);
		//printf("shenmegui\n");
		FieldList rfl = fl;//*************** it's very critical*****************888
		for(; fl -> tail != NULL; fl = fl -> tail){ }// ding wei zhi Def fan hui lian biao de wei bu
		fl -> tail = visit_DefList_in_struct(p -> children[1]);//***********************************************
		//printf("shenmegui\n");
		return rfl;
	}
}
void name_visit_VarDec(node *p, char *name){
	if(p -> numOfChildren == 1){
		strcpy(name, (const char*)p -> children[0] -> val.type_ID);
	}
	else{
		name_visit_VarDec(p -> children[0], name);
	}
}

Type type_visit_VarDec(node *p, Type ty){
	if(p -> numOfChildren == 1){// it's not an array
		Type type = ty;
		return type;
	}
	else{// array
		Type type = (Type)malloc(sizeof(Type_));
		type -> kind = ARRAY;
		type -> u.array.size = p -> children[2] -> val.type_int;
		type -> u.array.elem = type_visit_VarDec(p -> children[0], ty);
		return type;
	}
}

void visit_VarList(node *p, FieldList fl){/***********************************************************************************/
	node *q0 = p -> children[0] -> children[0];// node q0 is Specifier
	node *q1 = p -> children[0] -> children[1];// node q1 is VarDec
	Type type = visit_Specifier(q0);
	char* NAME = (char*)malloc(sizeof(20));
	name_visit_VarDec(q1, NAME);
	fl -> name = (char*)malloc(sizeof(20));
	strcpy(fl -> name, (const char*)NAME);
	fl -> type = type_visit_VarDec(q1, type);
	if(p -> numOfChildren == 1){
		fl -> tail = NULL;
	}
	else{
		fl -> tail = (FieldList)malloc(sizeof(FieldList_));//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
		visit_VarList(p -> children[2], fl -> tail);
	}
}

Type visit_StructSpecifier(node *p){
	if(strcmp(p -> children[1] -> name, "Tag") == 0){// need to check the struct_table
		node *q = p -> children[1];
		if(q -> children[0] -> flag == 3){
			Type type = (Type)malloc(sizeof(Type_));
			type = check_struct_table(q -> children[0] -> val.type_ID);

			if(type == NULL){// this structure has not been defined, there is an error
				print_error(17, q -> children[0] -> lineno);
			}

			return type;
		}
		else{
			printf("There is an unexpected bug! \n");
			return NULL;
		}
	}
	else if(strcmp(p -> children[1] -> name, "OptTag") == 0){// definition of an structure
		//printf("shenmegui\n");
		node *q = p -> children[1];// OptTag
		if(strcmp(q -> children[0] -> name, "ID") == 0){//need to fill the table
			if(check_struct_table(q -> children[0] -> val.type_ID) != NULL){//error: structure redefinition
				print_error(16, q -> children[0] -> lineno);
				return NULL;
			}
			else{// first time definition
				int res = 0;
				res = check_variable_table(q -> children[0] -> val.type_ID, 0);// only global variables'scope equals 0
				if(res == 1){// no variable defined before has the same name as the structure
					
					Type type = (Type)malloc(sizeof(Type_));
					type -> kind = STRUCTURE;
					type -> u.structure.name = (char*)malloc(sizeof(20));

					strcpy(type -> u.structure.name, (const char*)q -> children[0] -> val.type_ID);
					type -> u.structure.attrs = visit_DefList_in_struct(p -> children[3]);
					//xu yao cha kan shi fou you chong fu de yu
	
					int flag = 1;
					FieldList fil = type -> u.structure.attrs;
				    if(fil -> tail != NULL){
				    	FieldList fil1 = type -> u.structure.attrs -> tail;
						for(; fil != NULL; fil = fil -> tail){
							for(fil1 = fil -> tail; fil1 != NULL; fil1 = fil1 -> tail){
								if(strcmp(fil -> name, fil1 -> name) == 0){
									flag = 0;
									break;
								}
								//printf("%s %s\n",fil -> name, fil1 -> name );
				 			}
						}
				    }
				    else flag =1;
					//printf("%d", flag);
					if(flag){
						//printf("shenmegui\n");

						Item item = (Item)malloc(sizeof(Item_));
						item -> name = (char*)malloc(sizeof(20));
						strcpy(item -> name, (const char*)q -> children[0] -> val.type_ID);
						item -> type = type;
						item -> nextc = NULL;
						item -> nextr = NULL;
						item -> scope = 0;
						insert_struct_table(item, struct_table);
						return type;
					}
					else{// cun zai ming zi xiang tong de yu
						print_error(15, p -> lineno);//ding wei cun zai wen ti//////////////////////////////////////////////////
						return NULL;
					}
				}
				else{
					print_error(16, q -> children[0] -> lineno);
					return NULL;
				}
			}
		}
	
	}

	else{//annoymous structure, no need to fill the table
		Type type = (Type)malloc(sizeof(Type_));
		type -> kind = STRUCTURE;
		type -> u.structure.name = NULL;
		type -> u.structure.attrs = visit_DefList_in_struct(p -> children[2]);


		int flag = 1;
		FieldList fil = type -> u.structure.attrs;
		if(fil -> tail != NULL){
			FieldList fil1 = type -> u.structure.attrs -> tail;
			for(; fil != NULL; fil = fil -> tail){
				for( fil1 = fil -> tail; fil1 != NULL; fil1 = fil1 -> tail){
					if(strcmp(fil -> name, fil1 -> name) == 0){
						flag = 0;
						break;
					}
				 }
			}
			//printf("%d", flag);
		}
		else flag =1;
		//printf("%d", flag);
		if(flag){
			return type;
		}
		else{// cun zai ming zi xiang tong de yu
			print_error(15, p -> lineno);//ding wei cun zai wen ti//////////////////////////////////////////////////
			return NULL;
		}
		return type;
	}
}

Type visit_Specifier(node *p){
	if(strcmp(p -> children[0] -> name, "TYPE") == 0){
		//printf("sbhz\n");
		Type type = (Type)malloc(sizeof(Type_));
		type -> kind = BASIC;
		if(strcmp(p -> children[0] -> val.type_Type, "float") == 0){
			type -> u.basic = 1;
		}
		else if(strcmp(p -> children[0] -> val.type_Type, "int") == 0){
			type -> u.basic = 0;
		}

		return type;
	}
	else if(strcmp(p -> children[0] -> name, "StructSpecifier") == 0){
		//printf("shenmegui\n");
		return visit_StructSpecifier(p -> children[0]);
	}
}

int compare_Args(FieldList fl, node* p, int scope){
	if(fl != NULL){
		if( p -> numOfChildren == 1){
			Type type = visit_Exp(p -> children[0], scope);
			if(fl -> tail != NULL){
				return 0;
			}

			else{
				return compare_RET(fl -> type, type);
			}
		}
		else{
			Type type = visit_Exp(p -> children[0], scope);
		
			if(compare_RET(fl -> type, type) == 1){
				return compare_Args(fl -> tail, p -> children[2], scope);
			}	
		else return 0;
		}
	}
	else return 0;
}

Type visit_Exp(node *p, int scope){// ke neng return NULL!!!!!!!!!!!!!
	//printf("hahaha\n");
	if(p -> numOfChildren == 3){
		//case 1 Exp = Exp
		
		if(strcmp(p -> children[1] -> name, "ASSIGN") == 0){
			//printf("hahaha\n");
			Type t1 = visit_Exp(p -> children[0], scope);
			Type t2 = visit_Exp(p -> children[2], scope);
			if(compare_RET(t1, t2) != 1){
				if(t1 != NULL && t2 != NULL){
					print_error(5, p -> lineno);// lei xing bu pe pei
				}
			}
			else if(t1 -> left_or_right == 0){
				print_error(6, p -> lineno);
			}
			Type type = (Type)malloc(sizeof(Type_));
			type -> kind = BASIC;
			type -> u.basic = 0;
			type -> left_or_right = 0;
			return type;
		}
		//case 2 Exp AND Exp
		if(strcmp(p -> children[1] -> name, "AND") == 0){
			Type t1 = visit_Exp(p -> children[0], scope);
			Type t2 = visit_Exp(p -> children[2], scope);

			if(t1 -> kind != BASIC || t1 -> u.basic != 0 ||t2 -> kind != BASIC || t2 -> u.basic != 0 ){
				print_error(7, p -> lineno);// lei xing bu pe pei
				//return NULL;
			}

			Type type = (Type)malloc(sizeof(Type_));
			type -> kind = BASIC;
			type -> u.basic = 0;
			type -> left_or_right = 0;

			return type;
		}
		//case 3 Exp OR Exp
		if(strcmp(p -> children[1] -> name, "OR") == 0){
			Type t1 = visit_Exp(p -> children[0], scope);
			Type t2 = visit_Exp(p -> children[2], scope);

			if(t1 -> kind != BASIC || t1 -> u.basic != 0 ||t2 -> kind != BASIC || t2 -> u.basic != 0 ){
				print_error(7, p -> lineno);// lei xing bu pe pei
				//return NULL;
			}

			Type type = (Type)malloc(sizeof(Type_));
			type -> kind = BASIC;
			type -> u.basic = 0;
			type -> left_or_right = 0;

			return type;
		}
		//case 4 Exp RELOP Exp
		if(strcmp(p -> children[1] -> name, "RELOP") == 0){
			Type t1 = visit_Exp(p -> children[0], scope);
			Type t2 = visit_Exp(p -> children[2], scope);

			if(compare_RET(t1, t2) != 1 || t1 -> kind != BASIC || t2 -> kind != BASIC){
				if(t1 != NULL && t2 != NULL)
					print_error(7, p -> lineno);// lei xing bu pe pei
			}

			Type type = (Type)malloc(sizeof(Type_));
			type -> kind = BASIC;
			type -> u.basic = 0;
			type -> left_or_right = 0;

			return type;
		}
		//case 5 Exp + Exp
		if(strcmp(p -> children[1] -> name, "PLUS") == 0){
			Type t1 = visit_Exp(p -> children[0], scope);
			Type t2 = visit_Exp(p -> children[2], scope);

			if(compare_RET(t1, t2) != 1 || t1 -> kind != BASIC || t2 -> kind != BASIC){
				print_error(7, p -> lineno);// lei xing bu pe pei
				return NULL;
			}
			Type type = (Type)malloc(sizeof(Type_));
			type -> kind = BASIC;
			type -> u.basic = t1 -> u.basic;
			type -> left_or_right = 0;

			return type;
		}
		//case 6 Exp - Exp
		if(strcmp(p -> children[1] -> name, "MINUS") == 0){
			Type t1 = visit_Exp(p -> children[0], scope);
			Type t2 = visit_Exp(p -> children[2], scope);

			if(compare_RET(t1, t2) != 1 || t1 -> kind != BASIC || t2 -> kind != BASIC){
				print_error(7, p -> lineno);// lei xing bu pe pei
				return NULL;
			}

			Type type = (Type)malloc(sizeof(Type_));
			type -> kind = BASIC;
			type -> u.basic = t1 -> u.basic;
			type -> left_or_right = 0;

			return type;
		}
		//case 7 Exp * Exp
		if(strcmp(p -> children[1] -> name, "STAR") == 0){
			Type t1 = visit_Exp(p -> children[0], scope);
			Type t2 = visit_Exp(p -> children[2], scope);

			if(compare_RET(t1, t2) != 1 || t1 -> kind != BASIC || t2 -> kind != BASIC){
				print_error(7, p -> lineno);// lei xing bu pe pei
				return NULL;
			}

			Type type = (Type)malloc(sizeof(Type_));
			type -> kind = BASIC;
			type -> u.basic = t1 -> u.basic;
			type -> left_or_right = 0;

			return type;
		}
		//case 8 Exp / Exp
		if(strcmp(p -> children[1] -> name, "DIV") == 0){
			Type t1 = visit_Exp(p -> children[0], scope);
			Type t2 = visit_Exp(p -> children[2], scope);

			if(compare_RET(t1, t2) != 1 || t1 -> kind != BASIC || t2 -> kind != BASIC){
				print_error(7, p -> lineno);// lei xing bu pe pei
				return NULL;
			}

			Type type = (Type)malloc(sizeof(Type_));
			type -> kind = BASIC;
			type -> u.basic = t1 -> u.basic;
			type -> left_or_right = 0;

			return type;
		}
		//case 9 LP Exp RP	
		if(strcmp(p -> children[0] -> name, "LP") == 0){
			return visit_Exp(p -> children[1], scope);
		}
		//case 13 ID LP RP
		if(strcmp(p -> children[2] -> name, "RP") == 0){
			char *name;
			name = (char*)malloc(sizeof(20));
			strcpy(name, (const char*)p -> children[0] -> val.type_ID);
			int location = DJB_hash(name);
			Type rt = NULL;
			if(function_table[location] == NULL){
				print_error(2, p -> lineno);
				return NULL;
			}
			else{
				Item it = function_table[location];
				for(; it != NULL; it = it -> nextr){
					if(strcmp(p -> children[0] -> val.type_ID, it -> name) == 0){
						if(it -> type -> u.function.args != NULL){
							print_error(9, p -> lineno);
						}
						else{
							rt = it -> type -> u.function.RET;
						}
						break;
					}
				}
				if(it == NULL){
					print_error(2, p -> lineno);
				}

				return rt;
			}
		}
		//case 15 Exp DOT ID
		if(strcmp(p -> children[1] -> name, "DOT") == 0){
			Type type = visit_Exp(p -> children[0], scope);

			if(type -> kind != STRUCTURE){
				print_error(13, p -> lineno);
				return NULL;
			}
			else{
				FieldList fl;
				fl = type -> u.structure.attrs;
				for(;fl != NULL; fl = fl -> tail){
					if(strcmp(fl -> name, p -> children[2] -> val.type_ID) == 0){
						break;
					}
				}
				if(fl == NULL){
					print_error(14, p -> lineno);
					return NULL;
				}
				else{
					Type t = fl -> type;
					t -> left_or_right = 1;
					return t;
				}
			}
		}
	}

	if(p -> numOfChildren == 1){
		//printf("%s\n", p -> children[0] -> name);
		//case 16 ID
		if(strcmp(p -> children[0] -> name, "ID") == 0){
			char* name;
			name = (char*)malloc(sizeof(20));
			strcpy(name, (const char*)p -> children[0] -> val.type_ID);

			int res = 1;
			int depth = scope;
			//printf("%d\n", depth);
			for(; depth >= 0; depth --){
				res = check_variable_table(name, depth);
				if(res == 0){
					break;
				}
			}
			//printf("%d\n", res);
			if(res){
				print_error(1, p -> lineno);
				return NULL;
			}
			else{
				Item it = variable_scope[depth];
				for(; it != NULL; it = it -> nextc){
					if(strcmp(it -> name, p -> children[0] -> val.type_ID) == 0){
						Type type = it -> type;
						type -> left_or_right = 1;
						return type;
					}
				}
			}
		}
		//case 17 INT
		if(strcmp(p -> children[0] -> name, "INT") == 0){
			Type type = (Type)malloc(sizeof(Type_));
			type -> kind = BASIC;
			type -> u.basic = 0;
			type -> left_or_right = 0;
			return type;
		}
		//case 18 FLOAT
		if(strcmp(p -> children[0] -> name, "FLOAT") == 0){
			Type type = (Type)malloc(sizeof(Type_));
			type -> kind = BASIC;
			type -> u.basic = 1;
			type -> left_or_right = 0;

			return type;
		}
	}
	if(p -> numOfChildren == 2){
		//case 10 MINUS Exp
		if(strcmp(p -> children[0] -> name, "MINUS") == 0){
			Type t = visit_Exp(p -> children[1], scope);
			if(t -> kind != BASIC){
				print_error(7, p -> lineno);// lei xing bu pe pei
				return NULL;
			}
			Type type = (Type)malloc(sizeof(Type_));
			type -> kind = BASIC;
			type -> u.basic = t -> u.basic;
			type -> left_or_right = 0;

			return type;
		}
		//case 11 NOT Exp
		if(strcmp(p -> children[0] -> name, "NOT") == 0){
			Type t = visit_Exp(p -> children[1], scope);
			if(t -> kind != BASIC || t -> u.basic != 0){
				print_error(7, p -> lineno);// lei xing bu pe pei
				return NULL;
			}
			Type type = (Type)malloc(sizeof(Type_));
			type -> kind = BASIC;
			type -> u.basic = 0;
			type -> left_or_right = 0;

			return type;
		}
	}
	
	if(p -> numOfChildren == 4){
		//case 12 ID LP Args RP
		if(strcmp(p -> children[2] -> name, "Args") == 0){
			Type rt = NULL;
			char *name;
			name = (char*)malloc(sizeof(20));
			strcpy(name, (const char*)p -> children[0] -> val.type_ID);
			int location = DJB_hash(name);
			if(function_table[location] == NULL){
				if(check_variable_table(p -> children[0] -> val.type_ID, scope) == 0 || check_struct_table(p -> children[0] -> val.type_ID) != NULL){
					print_error(11, p -> lineno);
					return NULL;
				}
				else{
					print_error(2, p -> lineno);
					return NULL;
				}
			}
			else{
				Item it = function_table[location];
				for(; it != NULL; it = it -> nextr){
					if(strcmp(p -> children[0] -> val.type_ID, it -> name) == 0){
						int res;
						res = compare_Args(it -> type -> u.function.args, p -> children[2], scope);//**********************

						if(res == 0){
							print_error(9, p -> lineno);
							return NULL;
						}
						else{
							rt = it -> type -> u.function.RET;
						}
						break;
					}
				}
				if(it == NULL){
					if(check_variable_table(p -> children[0] -> val.type_ID, scope) == 0 || check_struct_table(p -> children[0] -> val.type_ID) != NULL){
						print_error(11, p -> lineno);
					}
					else print_error(2, p -> lineno);

					return NULL;
				}

				return rt;
			}
		}
	
		//case 14 Exp LB Exp RB
		if(strcmp(p -> children[1] -> name, "LB") == 0){
			Type t1 = visit_Exp(p -> children[0], scope);
			Type t2 = visit_Exp(p -> children[2], scope);

			if(t1 -> kind != ARRAY){
				print_error(10, p -> lineno);

				if(t2 -> kind != BASIC || t2 -> u.basic != 0){
					print_error(12, p -> lineno);
				}
				return NULL;
			}

			if(t2 -> kind != BASIC || t2 -> u.basic != 0){
				print_error(12, p -> lineno);
				return NULL;
			}

			Type type = t1 -> u.array.elem;
			type -> left_or_right = 1;

			return type;

		}
	}
	
	
}

void visit_ExtDecList(node *p, Type t){
	if(t == NULL){// there is some error with the type, so there is no need to fill the variables table
		// do nothing
		//printf("ding yi lei xing you cuo wu\n");
	}
	else{// fill the variables table
		Item item = (Item)malloc(sizeof(Item_));
		char* NAME;
		NAME = (char*)malloc(sizeof(20));
		name_visit_VarDec(p -> children[0], NAME);
		//printf("%s\n", NAME);
		item -> name = (char*)malloc(sizeof(20));
		strcpy(item -> name, (const char*)NAME);// p -> children[0] VarDec
		item -> type = type_visit_VarDec(p -> children[0], t);
		item -> nextr = NULL;
		item -> nextc = NULL;
		item -> scope = 0;
			
		int res;
		res = check_variable_table(item -> name, 0);
		if(res == 0){
			print_error(3, p -> children[0] -> lineno);
		}
		else{
			insert_var_table(item, variable_table, variable_scope, 0);
		}
		if(p -> numOfChildren == 1){
			// do nothing
		}
		else{
			visit_ExtDecList(p -> children[2], t);
		}
	}
}

Type visit_FunDec(node *p, int def, Type t){
	Type type = (Type)malloc(sizeof(Type_));
	type -> kind = FUNCTION;
	type -> u.function.RET = t;
	type -> u.function.name = (char*)malloc(sizeof(20));
	strcpy(type -> u.function.name, (const char*)p -> children[0] -> val.type_ID);
	if(strcmp(p -> children[2] -> name, "RP") == 0)// no arguments
		type -> u.function.args = NULL;
	else if(strcmp(p -> children[2] -> name, "VarList") == 0){
		FieldList fieldlist = (FieldList)malloc(sizeof(FieldList_));
		visit_VarList(p -> children[2], fieldlist);////////////////////////
		type -> u.function.args = fieldlist;
	}
	else{
		printf("There is an unexpected bug! \n");
		return NULL;
	}
	type -> u.function.def_or_not = def;
	type -> u.function.lineno = p -> children[2] -> lineno;

	int res;
	res = check_function_table(type);
	
	if(res == 1){

		Item item = (Item)malloc(sizeof(Item_));
		item -> name = (char*)malloc(sizeof(20));
		strcpy(item -> name, (const char*)p -> children[0] -> val.type_ID);
		item -> type = type;
		item -> nextr = NULL;
		item -> nextc = NULL;
		item -> scope = 0;

		insert_func_table(item, function_table);//////////////
		
		if(def == 1){// xu yao jiang xin can cha bian liang biao, ceng shu wei 1
			FieldList flist = type -> u.function.args;
			for(; flist != NULL; flist = flist -> tail){
				if(check_variable_table(flist -> name, 1) == 0){
					print_error(3, p -> lineno);// han shu xing can chu xian chong ming
				}
				else{
					Item item = (Item)malloc(sizeof(Item_));
					item -> name = (char*)malloc(sizeof(20));
					strcpy(item -> name, (const char*)flist -> name);
					item -> type = flist -> type;
					item -> nextc = NULL;
					item -> nextr = NULL;
					item -> scope = 1;
					insert_var_table(item, variable_table, variable_scope, 1);
				}
			}
		}
		else{
			FieldList flist = type -> u.function.args;
			for(; flist != NULL; flist = flist -> tail){
				if(check_variable_table(flist -> name, 1) == 0){
					print_error(3, p -> lineno);// han shu xing can chu xian chong ming
				}
				else{
					Item item = (Item)malloc(sizeof(Item_));
					item -> name = (char*)malloc(sizeof(20));
					strcpy(item -> name, (const char*)flist -> name);
					item -> type = flist -> type;
					item -> nextc = NULL;
					item -> nextr = NULL;
					item -> scope = 1;
					insert_var_table(item, variable_table, variable_scope, 90);//di 90 ceng zhuan men yong lai chu li han shu shengming
				}
			}
			//printf("lll\n");
			if(variable_scope[90] != NULL){
				//printf("dayuzi\n");
				delete_scope(90, variable_table, variable_scope);//han shu sheng ming xing can cha wan hou li ke shan chu
			}
		}
	}

	return type;
	
}

void visit_DecList(node *p, Type ty, int scope){// zai ci chu cha bian liang biao
	char *name;
	name = (char*)malloc(sizeof(20));
	name_visit_VarDec(p -> children[0] -> children[0], name);
	//printf("hahaha\n");
	int res = 0;
	res = check_variable_table(name, scope);
	if(res == 1 && check_struct_table(name) == NULL){// yu jie gou ti ye bu neng tong ming
			//printf("hahaha\n");
		Item item = (Item)malloc(sizeof(Item_));
		item -> name = (char*)malloc(sizeof(20));
		strcpy(item -> name, (const char*)name);
		item -> type = type_visit_VarDec(p -> children[0] -> children[0], ty);
		item -> nextr = NULL;
		item -> nextc = NULL;
		item -> scope = scope;
		insert_var_table(item, variable_table, variable_scope, scope);// fill the table
	//printf("hahaha\n");
		if(p -> children[0] -> numOfChildren == 3){
			Type t1, t2;
			t1 = type_visit_VarDec(p -> children[0] -> children[0], ty);
			t2 = visit_Exp(p -> children[0] -> children[2], scope);
			if(compare_RET(t1, t2) == 0 ){//////////////////////////////////////////////zhe ge han shu ming bu shi hen he shi
				if(t1 != NULL && t2 != NULL)
					print_error(5, p -> children[0] -> children[2] -> lineno);// fu zhi cao zuo lei xing bu pi pei 
			}
		}
			//printf("hahaha\n");
	}
	else{
		print_error(3, p -> children[0] -> children[0] -> lineno);
	}
	
	if(p -> numOfChildren == 3){
		visit_DecList(p -> children[2], ty, scope);
	}
}

void visit_DefList(node *p, int scope){//***********iterative algorithm*************
	node* q = p -> children[0] -> children[0];// Specifier
	node* q0 = p -> children[0] -> children[1];// DecList
	Type type = visit_Specifier(q);
	visit_DecList(q0, type, scope);
	if(p -> children[1] == NULL){
		//do nothing
	}
	else{
		visit_DefList(p -> children[1], scope);
	}
}

void visit_Stmt(node *p, int scope, Type type){
	//case 1 Exp SEMI
	//printf("%s\n", p -> children[0] -> name);
	if(strcmp(p -> children[0] -> name, "Exp") == 0){
		//printf("hahaha\n");
		visit_Exp(p -> children[0], scope);
	}
	//case 2 CompSt
	if(strcmp(p -> children[0] -> name, "CompSt") == 0){
		visit_CompSt(p -> children[0], scope, type);
	}
	//case 3 RETURN Exp SEMI
	if(strcmp(p -> children[0] -> name, "RETURN") == 0){
		//printf("lol\n");
		Type t1 = visit_Exp(p -> children[1], scope);
		if(compare_RET(t1, type) != 1){
			print_error(8, p -> lineno);
		}
		//??????????????????? t2 cong na li chuan guo lai?
	}
	//case 4 IF LP Exp RP Stmt
	if(strcmp(p -> children[0] -> name, "IF") == 0 && p -> numOfChildren == 5){
		Type  type = visit_Exp(p -> children[2], scope);
		/*dui fan hui zhi you he yao qiu */
		
		visit_Stmt(p -> children[4], scope, type);
	}
	//case 5 IF LP Exp RP Stmt ELSE Stmt
	if(strcmp(p -> children[0] -> name, "IF") == 0 && p -> numOfChildren == 7){
		Type  type = visit_Exp(p -> children[2], scope);
		/*dui fan hui zhi you he yao qiu */
		
		visit_Stmt(p -> children[6], scope, type);
	}
	//case 6 WHILE LP Exp RP Stmt
	if(strcmp(p -> children[0] -> name, "WHILE") == 0){
		Type  type = visit_Exp(p -> children[2], scope);
		/*dui fan hui zhi you he yao qiu */
		
		visit_Stmt(p -> children[4], scope, type);
	}
}

void visit_StmtList(node *p, int scope, Type type){
	//printf("hahaha\n");
	if(p -> children[1] == NULL){
		node *q = p -> children[0];// Stmt
		visit_Stmt(q, scope, type);
	}
	else{
		visit_StmtList(p -> children[1], scope, type);
	}
}

void visit_CompSt(node *p, int scope, Type type){
	scope ++;
	if(p -> numOfChildren >= 3){
		//printf("%s\n", p -> children[1] -> name);
		if(strcmp(p -> children[1] -> name, "DefList") == 0)
		visit_DefList(p -> children[1], scope);
	
		else if(strcmp(p -> children[1] -> name, "StmList") == 0)
			visit_StmtList(p -> children[1], scope, type);
		if(strcmp(p -> children[2] -> name, "StmList") == 0)
			visit_StmtList(p -> children[2], scope, type);

	}
	delete_scope(scope, variable_table, variable_scope);
	scope --;
}

void visit_ExtDef(node *p){
	if(strcmp(p -> children[1] -> name, "ExtDecList") == 0){// definition of variables
		Type type;
		type = visit_Specifier(p -> children[0]);
		visit_ExtDecList(p -> children[1], type);// need to insert variables into variables table here
	}
	else if(strcmp(p -> children[1] -> name, "SEMI") == 0){// handle the definition of a structure
		visit_Specifier(p -> children[0]);
	}
	else if(strcmp(p -> children[1] -> name, "FunDec") == 0 && strcmp(p -> children[2] -> name, "CompSt") == 0){// definition of a function
		Type type;
		type = visit_Specifier(p -> children[0]);// han shu fan hui zhi bu ke neng shi shu zu lei xing...
		visit_FunDec(p -> children[1], 1, type);//hanshu xing can xiang dang yu shen du wei 1 de ju bu bian liang
		visit_CompSt(p -> children[2], 0, type);
		/*visit_CompSt kai tou xu jiang dang qian ceng shu jia yi ran hou xiang dang qian ceng jia bian liang, chu li wan hou
		shan chu zhe xie bian liang bing jiang dang qian ceng jian yi*/
	}
	else if(strcmp(p -> children[1] -> name, "FunDec") == 0 && strcmp(p -> children[2] -> name, "SEMI") == 0){// declaration of a function
		//Type type;
		//type = 
		Type type;
		type = visit_Specifier(p -> children[0]);
		visit_FunDec(p -> children[1], 0, type);//hanshu xing can xiang dang yu shen du wei 1 de ju bu bian liang
	}
}

void visit_tree(node* p){
	if(p -> numOfChildren != 0){
		if(strcmp(p -> name, "ExtDef") == 0){
			visit_ExtDef(p);
		}
		int j;
		for(j = 0; j < p -> numOfChildren; j ++){
			visit_tree(p -> children[j]);
		}
	}
	else{

	}
}

void check_fun_def(){
	int i;
	for( i = 0; i < 1000; i ++){
		if(function_table[i] != NULL){
			Item it = function_table[i];
			for(; it != NULL; it = it -> nextr){
				if(it -> type -> u.function.def_or_not == 0){
					print_error(18, it -> type -> u.function.lineno);
				}
			}
		}
	}
}
void yuyifenxi(node* p){
	init_table(variable_table);
	init_table(variable_scope);
	init_table(struct_table);
	init_table(function_table);


	visit_tree(p);
	check_fun_def();
}