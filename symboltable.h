#ifndef _SYMBOLTABLE_H_
#define _SYMBOLTABLE_H_

typedef struct Type_* Type;
typedef struct Type_ Type_;
typedef struct FieldList_* FieldList;
typedef struct FieldList_ FieldList_;
typedef struct Item_* Item;
typedef struct Item_ Item_;

struct Type_
{
	enum { BASIC, ARRAY, STRUCTURE, FUNCTION } kind;
	union
	{
		// 基本类型
		int basic;// 0 stands for int while 1 stands for float
		// 数组类型信息包括元素类型与数组大小构成
		struct { Type elem; int size; } array;
		// 结构体类型信息是一个链表, name works for an instance of the structure. you need to know the name of the structure
		struct { char *name; FieldList attrs;} structure;
		// the information of a function contains the type of return value and the arguments list
		/*def_or_not = 1 means the function has been defined
		  *def_or_not = 0 means the function has been declared but is without definition
		  *lineno indicates the line number of the first declaration, it works when you need to print the error 18 */
		struct{ char *name; Type RET; FieldList args; int def_or_not; int lineno;} function;
	} u;//to record more concrete information of a type. There is no need to have a name in it cause no function can be verified as a variable 

	int left_or_right;// 0 biao shi zhi wei you zhi
};


struct FieldList_
{
	char* name; // 域的名字
	Type type; // 域的类型
	FieldList tail; // 下一个域
};

struct Item_// the definition of a item of hash table
{
	char* name;//variable's name or structure's name or function's name
	Type type;//BASIC or ARRAY or STRUCTURE or FUNCTION with concrete information
	Item nextr;// using open hashing 
	Item nextc;// point to the next item in the same scope, only works when the item is a variable
	int scope;// to record which scope an item is in
};

Item htable[100];

void init_table(Item ht[]);
void insert_var_table(Item item, Item ht[], Item st[], int scope);
void insert_struct_table(Item item, Item ht[]);
void insert_func_table(Item item, Item ht[]);
void delete_scope(int scope, Item ht[], Item st[]); 

int DJB_hash(char *str);

#endif