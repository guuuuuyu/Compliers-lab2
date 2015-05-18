#include <stdlib.h>
#include <stdio.h>
#include "node.h"

extern FILE* yyin;
node *root = NULL;

int main(int argc, char** argv)
{
	if ( argc <= 1 ) return 1;
	FILE* f = fopen(argv[1], "r");
	if ( !f )
	{
		perror(argv[1]);
		return 1;
	}
	yyrestart(f);
	yyparse();
	return 0;
}


