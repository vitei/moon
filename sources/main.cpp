#include <iostream>
//#include ""


// Bison and Flex stuff
extern int yyparse();
extern "C" FILE *yyin;

int main(int argc, char *argv[])
{
	for(unsigned int i = 1; i < argc; i++)
	{
		if((yyin = fopen(argv[i], "r")))
		{
			yyparse();
			fclose(yyin);

			// ...
		}
		else
		{
			std::cerr << "Could not process file " << argv[i] << std::endl;
		}
	}

	return 0;
}
