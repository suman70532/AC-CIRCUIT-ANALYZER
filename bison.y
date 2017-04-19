
%{
#include <stdio.h>
#include <math.h>
#include "graph.h"
extern FILE *yyin;
extern FILE *yyout;
%}

/* declare tokens */
%union{
	char* temp;
}
%type<temp> a L R RR II VV C I V END_OF_FILE

%token L R RR II VV C I V END_OF_FILE

%%
a :	  L 	{
				struct element fg = set_elementLC($1);
				fg.ele = 1;
				struct element* ad = &g[fg.left.name][fg.right.name];

				if(ad->present != 1)
					*ad = fg;
				else
				{
					while(ad->next != NULL)
					{
						ad = ad->next;
					}
					ad->next = malloc(sizeof(struct element));
					*ad->next = fg;
				}

				struct element* add = &g[fg.right.name][fg.left.name];

				if(add->present != 1)
					*add = fg;
				else
				{
					while(add->next != NULL)
					{
						add = add->next;
					}
					add->next = malloc(sizeof(struct element));
					*add->next = fg;
				}
				//printg();

				//exit(0);
			}
	| R 	{
				struct element fg = set_elementR($1);
				fg.ele = 0; 
				struct element* ad = &g[fg.left.name][fg.right.name];

				if(ad->present != 1)
					*ad = fg;
				else
				{
					while(ad->next != NULL)
					{
						ad = ad->next;
					}
					ad->next = malloc(sizeof(struct element));
					*ad->next = fg;
				}

				struct element* add = &g[fg.right.name][fg.left.name];

				if(add->present != 1)
					*add = fg;
				else
				{
					while(add->next != NULL)
					{
						add = add->next;
					}
					add->next = malloc(sizeof(struct element));
					*add->next = fg;
				}
				//printg();

				//exit(0);
			}
	| RR 	{
				struct element fg = set_elementRR($1);
				fg.ele = 0; fg.unit = '.';
				struct element* ad = &g[fg.left.name][fg.right.name];

				if(ad->present != 1)
					*ad = fg;
				else
				{
					while(ad->next != NULL)
					{
						ad = ad->next;
					}
					ad->next = malloc(sizeof(struct element));
					*ad->next = fg;
				}

				struct element* add = &g[fg.right.name][fg.left.name];

				if(add->present != 1)
					*add = fg;
				else
				{
					while(add->next != NULL)
					{
						add = add->next;
					}
					add->next = malloc(sizeof(struct element));
					*add->next = fg;
				}
				//printg();

				//exit(0);
			}

	| C 	{
				struct element fg = set_elementLC($1);
				fg.ele = 2; 
				struct element* ad = &g[fg.left.name][fg.right.name];

				if(ad->present != 1)
					*ad = fg;
				else
				{
					while(ad->next != NULL)
					{
						ad = ad->next;
					}
					ad->next = malloc(sizeof(struct element));
					*ad->next = fg;
				}

				struct element* add = &g[fg.right.name][fg.left.name];

				if(add->present != 1)
					*add = fg;
				else
				{
					while(add->next != NULL)
					{
						add = add->next;
					}
					add->next = malloc(sizeof(struct element));
					*add->next = fg;
				}
				//printg();

				//exit(0);
			}
	| I 	{
				struct element fg = set_elementIV($1);
				fg.ele = 3; 
				struct element* ad = &g[fg.left.name][fg.right.name];

				if(ad->present != 1)
					*ad = fg;
				else
				{
					while(ad->next != NULL)
					{
						ad = ad->next;
					}
					ad->next = malloc(sizeof(struct element));
					*ad->next = fg;
				}

				struct element* add = &g[fg.right.name][fg.left.name];

				if(add->present != 1)
					*add = fg;
				else
				{
					while(add->next != NULL)
					{
						add = add->next;
					}
					add->next = malloc(sizeof(struct element));
					*add->next = fg;
				}
				//printg();

				//exit(0);
			}
	| V 	{
				struct element fg = set_elementIV($1);
				fg.ele = 4; 
				struct element* ad = &g[fg.left.name][fg.right.name];

				if(ad->present != 1)
					*ad = fg;
				else
				{
					while(ad->next != NULL)
					{
						ad = ad->next;
					}
					ad->next = malloc(sizeof(struct element));
					*ad->next = fg;
				}
				struct element* add = &g[fg.right.name][fg.left.name];

				if(add->present != 1)
					*add = fg;
				else
				{
					while(add->next != NULL)
					{
						add = add->next;
					}
					add->next = malloc(sizeof(struct element));
					*add->next = fg;
				}
				//printg();

				//exit(0);
			}

	| II 	{
				struct element fg = set_elementIV($1);
				fg.ele = 3; fg.damping = 0;
				struct element* ad = &g[fg.left.name][fg.right.name];

				if(ad->present != 1)
					*ad = fg;
				else
				{
					while(ad->next != NULL)
					{
						ad = ad->next;
					}
					ad->next = malloc(sizeof(struct element));
					*ad->next = fg;
				}

				struct element* add = &g[fg.right.name][fg.left.name];

				if(add->present != 1)
					*add = fg;
				else
				{
					while(add->next != NULL)
					{
						add = add->next;
					}
					add->next = malloc(sizeof(struct element));
					*add->next = fg;
				}
				//printg();

				//exit(0);
			}
	| VV 	{
				struct element fg = set_elementIV($1);
				fg.ele = 4; fg.damping = 0;
				struct element* ad = &g[fg.left.name][fg.right.name];

				if(ad->present != 1)
					*ad = fg;
				else
				{
					while(ad->next != NULL)
					{
						ad = ad->next;
					}
					ad->next = malloc(sizeof(struct element));
					*ad->next = fg;
				}
				struct element* add = &g[fg.right.name][fg.left.name];

				if(add->present != 1)
					*add = fg;
				else
				{
					while(add->next != NULL)
					{
						add = add->next;
					}
					add->next = malloc(sizeof(struct element));
					*add->next = fg;
				}
				//printg();

				//exit(0);
			}
	| END_OF_FILE     {printcircuit();}

	| a L 	{
				struct element fg = set_elementLC($2);
				fg.ele = 1;
				struct element* ad = &g[fg.left.name][fg.right.name];

				if(ad->present != 1)
					*ad = fg;
				else
				{
					while(ad->next != NULL)
					{
						ad = ad->next;
					}
					ad->next = malloc(sizeof(struct element));
					*ad->next = fg;
				}

				struct element* add = &g[fg.right.name][fg.left.name];

				if(add->present != 1)
					*add = fg;
				else
				{
					while(add->next != NULL)
					{
						add = add->next;
					}
					add->next = malloc(sizeof(struct element));
					*add->next = fg;
				}
				//printg();

				//exit(0);
			}
	| a R 	{	
				struct element fg = set_elementR($2);
				fg.ele = 0; 
				struct element* ad = &g[fg.left.name][fg.right.name];

				if(ad->present != 1)
					*ad = fg;
				else
				{	//printf("===========================Detected on top ====================================\n")
					//print(*ad);
					while(ad->next != NULL)
					{
						ad = ad->next;
					}
					ad->next = malloc(sizeof(struct element));
					*ad->next = fg;
				}

				struct element* add = &g[fg.right.name][fg.left.name];

				if(add->present != 1)
					*add = fg;
				else
				{
					while(add->next != NULL)
					{
						add = add->next;
					}
					add->next = malloc(sizeof(struct element));
					*(add->next) = fg;
				}

				//printg();

				//exit(0);
			}
	| a RR 	{
				struct element fg = set_elementRR($2);
				fg.ele = 0; fg.unit = '.';
				struct element* ad = &g[fg.left.name][fg.right.name];

				if(ad->present != 1)
					*ad = fg;
				else
				{	//printf("===========================Detected on top ====================================\n")
					//print(*ad);
					while(ad->next != NULL)
					{
						ad = ad->next;
					}
					ad->next = malloc(sizeof(struct element));
					*ad->next = fg;
				}

				struct element* add = &g[fg.right.name][fg.left.name];

				if(add->present != 1)
					*add = fg;
				else
				{
					while(add->next != NULL)
					{
						add = add->next;
					}
					add->next = malloc(sizeof(struct element));
					*(add->next) = fg;
				}

				//printg();

				//exit(0);
			}

	
	| a C 	{
				struct element fg = set_elementLC($2);
				fg.ele = 2; 
				struct element* ad = &g[fg.left.name][fg.right.name];

				if(ad->present != 1)
					*ad = fg;
				else
				{
					while(ad->next != NULL)
					{
						ad = ad->next;
					}
					ad->next = malloc(sizeof(struct element));
					*ad->next = fg;
				}

				struct element* add = &g[fg.right.name][fg.left.name];

				if(add->present != 1)
					*add = fg;
				else
				{
					while(add->next != NULL)
					{
						add = add->next;
					}
					add->next = malloc(sizeof(struct element));
					*add->next = fg;
				}
				//printg();

				//exit(0);
			}
	| a I 	{
				struct element fg = set_elementIV($2);
				fg.ele = 3; 
				struct element* ad = &g[fg.left.name][fg.right.name];

				if(ad->present != 1)
					*ad = fg;
				else
				{
					while(ad->next != NULL)
					{
						ad = ad->next;
					}
					ad->next = malloc(sizeof(struct element));
					*ad->next = fg;
				}

				struct element* add = &g[fg.right.name][fg.left.name];

				if(add->present != 1)
					*add = fg;
				else
				{
					while(add->next != NULL)
					{
						add = add->next;
					}
					add->next = malloc(sizeof(struct element));
					*add->next = fg;
				}
				//printg();

				//exit(0);
			}
	| a V 	{
				struct element fg = set_elementIV($2);
				fg.ele = 4; 
				struct element* ad = &g[fg.left.name][fg.right.name];

				if(ad->present != 1)
					*ad = fg;
				else
				{
					while(ad->next != NULL)
					{
						ad = ad->next;
					}
					ad->next = malloc(sizeof(struct element));
					*ad->next = fg;
				}

				struct element* add = &g[fg.right.name][fg.left.name];

				if(add->present != 1)
					*add = fg;
				else
				{
					while(add->next != NULL)
					{
						add = add->next;
					}
					add->next = malloc(sizeof(struct element));
					*add->next = fg;
				}
				//printg();

				//exit(0);
			}

	| a II 	{
				struct element fg = set_elementIV($2);
				fg.ele = 3; fg.damping = 0;
				struct element* ad = &g[fg.left.name][fg.right.name];

				if(ad->present != 1)
					*ad = fg;
				else
				{
					while(ad->next != NULL)
					{
						ad = ad->next;
					}
					ad->next = malloc(sizeof(struct element));
					*ad->next = fg;
				}

				struct element* add = &g[fg.right.name][fg.left.name];

				if(add->present != 1)
					*add = fg;
				else
				{
					while(add->next != NULL)
					{
						add = add->next;
					}
					add->next = malloc(sizeof(struct element));
					*add->next = fg;
				}
				//printg();

				//exit(0);
			}
	| a VV 	{
				struct element fg = set_elementIV($2);
				fg.ele = 4; fg.damping = 0;
				struct element* ad = &g[fg.left.name][fg.right.name];

				if(ad->present != 1)
					*ad = fg;
				else
				{
					while(ad->next != NULL)
					{
						ad = ad->next;
					}
					ad->next = malloc(sizeof(struct element));
					*ad->next = fg;
				}

				struct element* add = &g[fg.right.name][fg.left.name];

				if(add->present != 1)
					*add = fg;
				else
				{
					while(add->next != NULL)
					{
						add = add->next;
					}
					add->next = malloc(sizeof(struct element));
					*add->next = fg;
				}
				//printg();

				//exit(0);
			}
	
	| a END_OF_FILE {printcircuit();}
%%

int main(int argc, char **argv)
{
  
  yyin = fopen(argv[1], "r");
  filename = argv[2];
  resfile = argv[3];

  printf("----------------------Invalid inputs and errors shall be listed below--------------------------- \n");
  if(yyin != NULL)
	do{
  yyparse();
} while(!feof(yyin));


fclose(yyin);
  return 1;	
}

yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
}
