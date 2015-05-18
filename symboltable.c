#include "symboltable.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

int DJB_hash(char *str)    
{    
    int hash = 5381;    
    
    while (*str)    
    {    
        hash += (hash << 5) + (*str++);    
    }    
    
    if(hash > 0)
    	return hash % 1000;
    else return (-hash) % 1000;   
}    


void init_table(Item ht[]){
	int i = 0;
	for(;i < 1000;i ++)
		ht[i] = NULL;
}

void insert_var_table(Item item, Item ht[], Item st[], int scope){
	char* name;
	name = (char*)malloc(sizeof(20));
	strcpy(name, (const char*)item -> name);
	int location = DJB_hash(name);
	//printf("%d\n", location);
	if(ht[location] == NULL){
		ht[location] = item;
		item -> nextr = NULL;
	}
	else{
		Item p = ht[location];
		ht[location] = item;// add the newest item to the head of the linklist in hash table
		item -> nextr = p;
	}

	if(st[scope] == NULL)
		st[scope] = item;
	else{
		Item p = st[scope];
		st[scope] = item;// add the newest item to the head of the linklist in scope table
		item -> nextc = p;
	}
	
}

void insert_struct_table(Item item, Item ht[]){
	char* name;
	name = (char*)malloc(sizeof(20));
	strcpy(name, (const char*)item -> name);

	int location = DJB_hash(name);
	if(ht[location] == NULL){
		ht[location] = item;
		item -> nextr = NULL;
	}
	else{
		Item p = ht[location];
		ht[location] = item;// add the newest item to the head of the linklist
		item -> nextr = p;
	}
}

void insert_func_table(Item item, Item ht[]){
	char* name;
	name = (char*)malloc(sizeof(20));
	strcpy(name, (const char*)item -> name);
	int location = DJB_hash(name);
	if(ht[location] == NULL){
		ht[location] = item;
		item -> nextr = NULL;
	}
	else{
		Item p = ht[location];
		ht[location] = item;// add the newest item to the head of the linklist
		item -> nextr = p;
	}
}

void delete_scope(int scope, Item ht[], Item st[]){//dang qian shan chu de ceng yi ding shi zui nei ceng
	if(st[scope] != NULL){
		Item it = st[scope];
		for( ; it != NULL; it = it -> nextc){// shan chu hash biao zhong dui ying de biao xiang
			char *name;
			name = (char*)malloc(sizeof(20));
			strcpy(name, (const char*)it -> name);
			int location = DJB_hash(name);
			ht[location] = ht[location] -> nextr;// zui nei ceng yi ding shi zui hou jia ru de, gu zai di yi xiang
		}
		it = st[scope];
		Item q;

		while(it != NULL){// destroy the linklist in scope table
			q = it -> nextc;
			free(it);
			it = q;
		}
	}
	st[scope] = NULL;// this is critical!!!
}