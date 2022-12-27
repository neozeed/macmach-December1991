


	/*
	 * lex program to construct the final yacc input
	 * that is the uil compiler.
 	 *
	 * phase 0
	 *
	 * stdin is a data stream with object definitions. This 
	 * is parsed into an internal representation
	 * 
	 * phase 1
	 *
	 * stdin is reset to point at a template .y file 'grammar.y'
	 * this has three markers in it.  As each marker is encountered
	 * the parsed phase 1 data is spit to stdout.
	 *
	 */

%a 9999						/* transitions ? */
%o 9999						/* output table size? */

%{

#include <stdio.h>

#define MAX_STRINGS 20

#define TRUE  1
#define FALSE 0

#define Phase_1_Echo	if (phase == 1) printf ("%s", yytext)

typedef struct _node
{
    char * strings[MAX_STRINGS];
    int    num_strings;
}
    DefinitionRec, *Definition;

Definition table[1000];

int phase		= 0;
int keyword_count;
int line_num		= 0;
int in_comment		= 0;
int in_include		= 0;
int in_definition	= 0;
int state		= 0;
int used_defs		= 0;

%}



%%


[\n]		{				/* for error reporting */
		    Phase_1_Echo;
		    line_num++;
		}

[ \t]		{				/* pass white space */
		    Phase_1_Echo;
		}


"/\*"		{
		    in_comment++;
		    /* printf ("\nenter comment, count = %d\n", in_comment); */
		    Phase_1_Echo;
		}

"\*/"		{
		    in_comment--;
		    if (in_comment < 0)
			printf ("\n\nERROR, too many end comments\n\n");
		    Phase_1_Echo;
		}

"{"		{
		    if ((phase == 0) && (in_comment == 0))
		    {
			Definition node = 
				(Definition) malloc (sizeof (DefinitionRec));

			in_definition = TRUE;

			table[used_defs] = node; 

			node->num_strings = 0;
		    }

		    Phase_1_Echo;
		}

"}"		{
		    if ((phase == 0) && (in_comment == 0))
		    {
			in_definition = TRUE;

			used_defs++;
		    }

		    Phase_1_Echo;
		}

^\$include	{
		    if ((phase == 1) && (in_comment == 0))
		    {
		        in_include = TRUE;
		        state = 0;
		    }
		    else
		    {
			if (in_comment != 0)
			    printf (
				"\n\nERROR, include in comment nested %d\n", 
				in_comment);
			else
			    printf ("\n\nERROR, found marker in phase 0\n\n");
		    }
		}

[a-zA-Z0-9_]+	{
		    if (in_comment == 0)
		    {
			if ((phase == 1) && (in_include == TRUE))
			{
			    switch (state)
			    {
				case 0:
				    if (strcmp (yytext, "a") == 0)
				    {
					state++;	/* get number */
				    }

				    if (strcmp (yytext, "b") == 0)
				    {
					spit_b ();
				        in_include = FALSE;
				    }

				    if (strcmp (yytext, "c") == 0)
				    {
					spit_c ();
				        in_include = FALSE;
				    }
				    break;

				case 1:
				    sscanf (yytext, "%d", &keyword_count);
				    spit_a (keyword_count);
				    in_include = FALSE;
				    break;
			    }
			}
			else
		            Phase_1_Echo;

			if ((phase == 0) && (in_definition == TRUE))
			{
			    Definition node = table[used_defs];

			    node->strings[node->num_strings] = 	
			        (char *) strcpy (
				    malloc (strlen (yytext) + 1), 
				    (char *) yytext);

			    if (++(node->num_strings) == MAX_STRINGS)
			    {
				printf (
			"\nERROR, line %d, too many strings\n", line_num);
			    }
			}
		    }
		    else			/* inside comment */
		        Phase_1_Echo;
		}


.	{
	    Phase_1_Echo;
 	}


%%

#ifdef NOPE

/* copy up to the next marker */

void
copy_to_marker (f, marker, value)
    FILE *f;
    char * marker;
    int * value;
{
    char line[1000];
    int ml = 0;
    int i = 0;

    if (marker != NULL) ml = strlen (marker);

    for ( ; ; )
    {
	i = -1;
	do 
	{ 
	    line[++i] = getc (f); 
	}
	    while (line[i] != '\n' && line[i] != EOF);

	if (line[i] == EOF)
	{
	    return;
	}

	line[i] = '\0';

	if ((marker != NULL) && (strncmp (line, marker, ml) == 0))
	{
	    sscanf (&(line[ml]), "%d", value);

	    return;
	}
	else
	    printf ("%s\n", line);
    } 
}
#endif


yywrap ()
{
    FILE *f;

    if (phase == 0)
    {
	phase = 1;

	fclose (yyin);
        yyin = fopen ("grammar.y", "r");

    	if (yyin == NULL)
	{
    	    printf ("ERROR, can't open template file grammar.y\n");
	    return (1);
	}
	
	return (0);				/* now read template */
    }
    else
	return (1);
}



spit_a (keyword_count)
    int keyword_count;
{
    int i, j;
    Definition node;

    printf ("\n");

    for (i=0; i<used_defs; i++)
    {
	node = table[i];

	printf ("%%token\t%s\t\t\t\t%d\t/* keyword ", 
		node->strings[0], keyword_count++);

/*
	for (j=3; j<node->num_strings; j++)
		printf ("%s ", node->strings[j]);
*/
	printf ("*/\n");
    } 
}


spit_b ()
{
    int i;
    Definition node;
    char * lead;

    lead = "    : ";
    printf ("\n");

    for (i=0; i<used_defs; i++)
    {
	node = table[i];

	printf (
	    "%s%s\n\t{ $$ = gz_yynullval; $$ . b_type = sym_k_%s_object; }\n",
	    lead, node->strings[0], node->strings[2]); 

	lead = "    | ";
    } 
}

spit_c ()
{
    int i;
    Definition node;
    char * lead;

    lead = "    | ";
    printf ("\n");

    for (i=0; i<used_defs; i++)
    {
	node = table[i];

	printf (
	    "%s%s\n", lead, node->strings[0]); 
    } 
}
