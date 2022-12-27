




	/*
	 * lex program to construct the keyword data for the UIL compiler
 	 *
	 * stdin is the file 'tokens.dat'
	 *
	 * it is parsed and results in three output files
	 *
	 * UilKeyDef.h in phase 0
	 * UilKeyTok.h in phase 1
	 * UilTokName.h in phase 2
	 */

%a 9999						/* transitions ? */
%o 9999						/* output table size? */

%{

#include <stdio.h>
#include <ctype.h>

#define TRUE  1
#define FALSE 0
#define MAX_TOKEN_LEN 100
#define MAX_TOKENS 1000
#define MAX_CLASSES 100

void quit ();
void add_token ();
int  is_all_digits ();

typedef struct 
{
    char name 		[MAX_TOKEN_LEN];
    char id   		[MAX_TOKEN_LEN];
}
    ClassRec, *Class;

typedef struct 
{
    int  class;
    char arg_name 	[MAX_TOKEN_LEN];
    char id   		[MAX_TOKEN_LEN];
    int  id_num;
    char alias 		[MAX_TOKEN_LEN];
/*    char urm_name 	[MAX_TOKEN_LEN]; */
    char tk_name 	[MAX_TOKEN_LEN];
    char lit_name	[MAX_TOKEN_LEN];
    int  in_keytab;
}
    TokenRec, *Token;

ClassRec class[MAX_CLASSES];
TokenRec   loc_token[MAX_TOKENS];
TokenRec   token[MAX_TOKENS];
TokenRec   temp_token;

FILE * out;

int phase		= 0;
int sub_phase		= 0;
int line_num		= 0;
int in_subclass		= 0;
int in_literal		= FALSE;
int in_token		= 0;
int in_class		= 0;
int state		= 0;
int class_used		= 0;
int token_used		= 0;
int max_token_len	= 0;
int print_class_header  = FALSE;
int current_class	= 0;
int need_to_add		= FALSE;
int tokens_in_token_dat = 0;
char cur_file [100] = {
	't', 'o', 'k', 'e', 'n', '.', 'd', 'a', 't', '\0' };

%}



%%

[\n]		{
		    line_num++;

		    if (need_to_add) 
		    {
			strcpy (temp_token.lit_name, temp_token.arg_name);
			add_token (token, &temp_token, &token_used, TRUE);

		    	if (in_subclass)
			    fprintf (out, 
				"#define\ttkn_k_sub_%s\t%s\n", 
				temp_token.alias,
				temp_token.id);
		    }

		    need_to_add = FALSE;
		    in_token = FALSE;
		    in_subclass = FALSE;
		}

[}]		{
		    if (( ! in_literal) &&
		    	(need_to_add)) 
			{
			    strcpy (temp_token.lit_name, temp_token.arg_name);
			    add_token (token, &temp_token, &token_used, TRUE);

		    	    if (in_subclass)
			    	fprintf (out, 
				    "#define\ttkn_k_sub_%s\t%s\n", 
				    temp_token.alias,
				    temp_token.id);
			}

		    need_to_add = FALSE;
		    in_token = FALSE;
		    in_subclass = FALSE;
		}

^\t		{   in_token = TRUE;		/* start token definition */
		    state = 0;
		} 


^"/\*"		{				/* start class definition */
		    if ((phase == 0) && 
			(sub_phase == 0))
		    {
		    	in_class = TRUE;
		    	state = 0;
		    }
		}

"'"		{
		    if (in_literal) 
			in_literal = FALSE;
		    else
			in_literal = TRUE;
		}

^[\t]*"{"	{				/* pickup class resources */
		    if (( ! in_literal) &&
			(phase == 0) && 
			(sub_phase != 0))
		    {
			temp_token.arg_name[0] = '\0';
			temp_token.alias[0] = '\0';
/*			temp_token.urm_name[0] = '\0'; */
			temp_token.tk_name[0] = '\0';

		    	in_subclass = TRUE;
		    	state = 0;
		    }
		}

[a-zA-Z0-9_]+	{
		    if (strlen (yytext) >= MAX_TOKEN_LEN)
		    	quit (0, yytext);

		    if (( ! in_literal) && (phase == 0))
		    {
		    	if (in_class == TRUE)
		    	{
			    switch (state)
			    {
			    	case 0:
				    if (class_used >= MAX_CLASSES)
					quit (1, yytext);

				    strcpy (class[class_used].name, yytext);
				    break;

			    	case 1:
				    strcpy (class[class_used].id, yytext);

				    current_class = class_used;
				    class_used++;
				    in_class = FALSE;
				    break;
			    }

			    state++;
			}

			if (in_token == TRUE)	/* pickup token arg_name & id */
			{
			    int i;

			    switch (state)
			    {
			    	case 0:			/* get argument name */
				    need_to_add = TRUE;

				    i = strlen (yytext);

				    if (i > max_token_len) max_token_len = i;

				    strcpy (temp_token.arg_name, yytext);

				    temp_token.in_keytab = FALSE;

				    tokens_in_token_dat++;
	
				    break;

				case 1:			/* get alias */
				    strcpy (temp_token.alias, yytext);
				    break;

			    	case 2:			/* get sym-code */
				    strcpy (temp_token.id, yytext);

				    sscanf (yytext, "%d", &(temp_token.id_num));
				    break;

				default:
				    printf ("\n\nERROR parsing %s line %d\n",
					cur_file,
					line_num);
				    break;
			    }

			    state++;
			}

			if (in_subclass)
			{
			    int i;

			    if (print_class_header)
			    {
				fprintf (out, 
				    "\n/*    For class: %s    */\n\n", 
		    		    class[sub_phase-1].name);

			  	print_class_header = FALSE;
			    }

			    i = strlen (yytext);

			    if (i > max_token_len) max_token_len = i;

			    switch (state)
			    {
			    	case 0:
				    need_to_add = TRUE;

				    temp_token.arg_name[0] = '\0';
				    temp_token.alias[0] = '\0';
/*				    temp_token.urm_name[0] = '\0'; */
				    temp_token.tk_name[0] = '\0';

				    strcpy (temp_token.arg_name, yytext);

				    temp_token.in_keytab = TRUE;
				    break;

				case 1:			/* sym code */
				    strcpy (temp_token.id, yytext);

				    sscanf (yytext, "%d", &(temp_token.id_num));
				    break;

				case 2:			/* alias */
				    strcpy (temp_token.alias, yytext);
				    break;
/*
 *			    	case 3:			 urm name 
 *      			    strcpy (temp_token.urm_name, yytext);
 *
 *				    break;
 */
			    	case 3:			/* tk name */
				    strcpy (temp_token.tk_name, yytext);
				    break;

				default:
				    printf ("\n\nERROR syntax %s line %d\n", 
					cur_file,
					line_num);
				    break;
			    }

			    state++;
			}
		    }
		}



.		{				/* swallow */
		}

%%


void
quit (code, c)
    int code;
    char c;
{
    switch (code)
    {
	case 0:
	    printf ("ERROR, %s, token '%s' too long\n", cur_file, c);
	    break;

	case 1:
	    printf ("ERROR, %s class %s overflowed class table\n", cur_file, c);
	    break;

	case 2:
	    printf ("ERROR, %s token %s overflowed token table\n", cur_file, c);
	    break;

	default:
	    printf ("Exit, %s unknown error, line %d.\n", cur_file, line_num);
	    break;
    }

    exit (0);
}


void
stats ()
{
    printf ("Used %d of %d classes\n", class_used, MAX_CLASSES);
    printf ("Used %d of %d tokens\n", token_used, MAX_TOKENS);
}


void
lower_case (c)
    char * c;
{
    for ( ; *c != '\0'; c++)
	if (isupper (*c))
#ifdef NO_TOUPPER
	    *c = tolower (*c);
#else
	    *c = _tolower (*c);
#endif /* NO_TOUPPER */
}

void
upper_case (c)
    char * c;
{
    for ( ; *c != '\0'; c++)
	if (islower (*c))
#ifdef NO_TOUPPER
	    *c = toupper (*c);
#else
	    *c = _toupper (*c);
#endif /* NO_TOUPPER */
}

int
is_all_digits (c)
    char *c;
{
    for ( ; *c != '\0'; c++)
	if ( ! isdigit (*c))
	    return (FALSE);

    return (TRUE);
}



void
add_token (token, t, tu, do_class)		/* keep sorted by lit name */
    Token token;
    Token t;
    int *tu;
    int do_class;
{
    int i, j, k;

    if (*tu >= MAX_TOKENS)
	quit (2, t->arg_name);

    for (j=0; j<*tu; j++)		/* for each token */
    {
	if (strcmp (token[j].lit_name, t->lit_name) > 0)	/* goes here */
  	{
	    for (i=(*tu)++; i>j; i--)	/* make hole */
		token[i] = token[i-1];

	    token[j] = *t;			/* insert it */

	    if (do_class)
	    token[j].class = current_class;
/*
	     printf ("add token %s, %d tokens\n", t->arg_name, *tu); 
*/
	    return;
	}
    }

    /*
     * if we get there then it goes at the end of the list 
     */

    token[*tu] = *t;
    if (do_class)
    token[*tu].class = current_class;

    (*tu)++;
/*
    printf ("add token %s, %d tokens\n", t->lit_name, *tu); 
*/
}





void
phase_0_class_literals (f)
    FILE *f;
{
    int i;

    fprintf (f, "#define key_defs 1\n\n/*\n *    Class literals\n */\n\n");

    for (i=0; i<class_used; i++)
    	fprintf (f, "#define\ttkn_k_class_%s\t%s\n", 
		class[i].name, class[i].id);
}



void
phase_0_number_literals (f)
    FILE *f;
{
    int i;
    char lowbuf[100];

    fprintf (f, "\n/*\n *    Token number literals\n */\n\n");

    for (i=0; i<token_used; i++)
	 {
	 strcpy (lowbuf, token[i].arg_name);
	 lower_case (lowbuf);
	 fprintf (f, "#define\ttkn_k_num_%s\t%s\n", 
		     lowbuf,
		     token[i].id);
	 }
}


static char tbuf[100];

char *
transform (c)
    char * c;
{
    int i, j, saw_underbar = FALSE;

    for (i=0, j=0; c[i] != '\0'; i++)
    {
	if (c[i] == '_')
	{
	    saw_underbar = TRUE;
	}
	else
	{
	    if (islower (c[i]) && saw_underbar)
	    {
		saw_underbar = FALSE;
#ifdef NO_TOUPPER
	    	tbuf[j++] = toupper (c[i]);
#else
	    	tbuf[j++] = _toupper (c[i]);
#endif /* NO_TOUPPER */

	    }
	    else
	    	tbuf[j++] = c[i];
	}
    }

    tbuf[j] = '\0';

    return (tbuf);
}

static 
    FILE	    *grammar;
    unsigned char   grammar_file_open = FALSE;

int
in_grammar_dat (token)
    Token token;
{
unsigned char	    char_idx,found;
char		    nxt_char;

    if (grammar_file_open)
	rewind(grammar);
    else
        {
	grammar = fopen("grammar.dat", "r");
	grammar_file_open = TRUE;
	};

    char_idx = 0;

    for (nxt_char = fgetc(grammar);
	feof(grammar) == 0;
	nxt_char = fgetc(grammar))

	{
	if (nxt_char == token -> lit_name[char_idx])
	    {
	    if (char_idx == MAX_TOKEN_LEN)
		return (TRUE);
	    else
		if (token -> lit_name[char_idx + 1] == 0)
		    return (TRUE);
            char_idx++;
	    }
	else
	    char_idx = 0;
	}
    return (FALSE);
}



void
phase_1 (f, upper)
    FILE *f;
    int upper;
{
    int i, tu = 0;
    char *lit, *sub, *arg_name, *urm;
    char lit_buf[100], arg_buf[100], urm_buf[100], sub_buf[100], low_buf[100];


    if ( ! upper)
    {
        fprintf (f, "#define key_keytable 1\n\n");
        fprintf (f, "/*    Include the public key definitions.    */\n\n");
        fprintf (f, "#include\t\"UilKeyDef.h\"\n\n\n");
        fprintf (f, "/*    Include the URM literal definitions.    */\n\n");
        fprintf (f, "#include\t\"UilUrmEnum.h\"\n\n");
        fprintf (f, "/*    Number of keyword entries in the table    */\n\n");
        fprintf (f, "#define\tkey_k_keyword_count (_number (key_table))\n\n");
        fprintf (f, "/*    case sensitive keyword table    */\n\n");
        fprintf (f, "key_keytable_entry_type key_table [] =\n");
    }
    else
    {
        fprintf (f, "\n");
        fprintf (f, "/*    case insensitive keyword table    */\n\n");
        fprintf (f, "key_keytable_entry_type key_table_case_ins [] =\n");
    }

    fprintf (f, "{\n");

    if ( ! upper)
    {
	for (i=0; i<token_used; i++)
	{
	    temp_token = token[i];

	    if ( ! temp_token.in_keytab) 		/* is in tokens.dat */
	    	if ( ! in_grammar_dat (&temp_token))  /* but not grammar.dat */
		    lower_case (temp_token.lit_name);

	    add_token (loc_token, &temp_token, &tu, FALSE);
	}
    }
    else
    {
	for (i=0; i<token_used; i++)
	{
	    temp_token = token[i];

	    upper_case (temp_token.lit_name);

	    add_token (loc_token, &temp_token, &tu, FALSE);
	}
    }

    for (i=0; i<tu; i++)
    {
	lit = lit_buf;
	sub = sub_buf;
/*	urm = urm_buf; */
	arg_name = arg_buf;

	switch (loc_token[i].class)
 	{
	    case 5:				/* keyword */
	    case 8:				/* reserved */
		strcpy (lit_buf, loc_token[i].lit_name);
		strcpy (sub_buf, "0");
/*		strcpy (urm_buf, "0"); */
		strcpy (arg_buf, loc_token[i].arg_name);
		break;

	    case 0:				/* argument */
	    case 1:				/* charset */
	    case 7:				/* reason */

		if ( ! loc_token[i].in_keytab) continue;

		strcpy (lit_buf, loc_token[i].lit_name);

		strcpy (sub_buf, "tkn_k_sub_");
		strcat (sub_buf, loc_token[i].arg_name);

		strcpy (arg_buf, class[loc_token[i].class].name);
		strcat (arg_buf, "_name");
		upper_case (arg_buf);

		switch (loc_token[i].class)
		{
		    case 1:			/* charset */
/*			strcpy (urm_buf, "0"); */
			break;

		    case 7:			/* reason */
		    default:			/* argument */
/*
 *			strcpy (urm_buf, 
 *		  		(loc_token[i].urm_name[0] == '\0' ? 
 *				    loc_token[i].arg_name : loc_token[i].urm_n<ame));
 *			urm = transform (urm_buf);
 */
			break;
		}
		break;

	    case 2:				/* color */
	    case 3:				/* font */
	    case 4:				/* ident */
	    case 6:				/* literal */
	    case 9:				/* special */
	    case 10:				/* unused */
		continue;
		break;

	    default:
		strcpy (lit_buf, "?");
		strcpy (sub_buf, "?");
		strcpy (arg_buf, "?");
/*		strcpy (urm_buf, "?"); */
		break;
	}

	strcpy (low_buf, arg_name);
	lower_case (low_buf);
    	fprintf (f, 
	  "{ tkn_k_class_%s, %s, %d, tkn_k_num_%s, \"%s\" },\n",
	    class[loc_token[i].class].name,
	    sub,
	    strlen (lit),
	    low_buf,
	    lit);

/*    	fprintf (f, 
 *	  "{ tkn_k_class_%s, %s, %d, tkn_k_num_%s, \"%s\",  %s },\n",
 *	    class[loc_token[i].class].name,
 *	    sub,
 *	    strlen (lit),
 *	    low_buf,
 *	    lit,
 *	    urm);
 */
    }

    fprintf (f, "};\n");
}



void
phase_2 (f)
    FILE *f;
{
    char b[100];
    int i, tok_num, found;

    fprintf (f, "#define tok_names 1\n\n");
    fprintf (f, "/*    Unknown token    */\n\n");
    fprintf (f, "#define\ttok_k_unknown\t\"UNKNOWN_TOKEN\"\n\n");
    fprintf (f, "/*    Token name table    */\n\n");
    fprintf (f, "static char * tok_token_name_table [] = \n");
    fprintf (f, "{\n");

    for (tok_num = 0; tok_num < tokens_in_token_dat; tok_num++)
    {
	/*
	 * search for the token with this number
	 * go backwards through table so that we see synonyms before
	 * the real symbol
	 */

	found = FALSE;

    	for (i = token_used - 1; ((i >= 0) && ( ! found)); i--)
    	{
	    if ( ! token[i].in_keytab) 		/* had to be in tokens.dat */
	    {
	    	if (token[i].id_num == tok_num)	/* ascending order */
		{
		    found = TRUE;

	    	    strcpy (b, token[i].arg_name);

	    	    upper_case (b);

    	    	    fprintf (f, "    \"%s\",\n", b);
		}
	    }
	}
	
	if ( ! found)
	    fprintf (f, "    tok_k_unknown,\n");
    }

    fprintf (f, "};\n");
}




yywrap ()
{
    char buf[100];
    char fname[100];

    switch (phase)
    {
	case 0:
	    if (sub_phase == 0)
	    {
	    	out = fopen ("UilKeyDef.h", "w");

	        if (out == NULL)
	  	    printf ("ERROR, couldn't open UilKeyDef.h\n");
		else
		    printf ("Created UilKeyDef.h\n");

	        phase_0_class_literals (out);
	        phase_0_number_literals (out);

		fprintf (out, "\n/*\n *    Subclass literals\n */\n\n");
	    }

	    if (sub_phase < class_used)
	    {
		do
		{
	    	    strcpy (fname, class[sub_phase].name);
	    	    strcat (fname, ".dat");

	    	    fclose (yyin);
		    strcpy (cur_file, fname);
		    line_num = 0;
	    	    yyin = fopen (fname, "r");

		    if (yyin == NULL)
		    {
			printf ("WARNING, couldn't open %s\n", fname);
			sub_phase++;
		    }
		    else
		        printf ("Process file %s\n", fname);
		}
		while (yyin == NULL);

		current_class = sub_phase;

	        sub_phase++;

		print_class_header = TRUE;

	        return (0);
	    }
	    else
	    {
		fprintf (out, "\n/*    Maximum length of a keyword    */\n");
	   	fprintf (out, "\n#define\tkey_k_keyword_max_length\t%d\n",
			max_token_len);

		fprintf (out, "\n/*    Keyword table entry structure    */\n\n");
		fprintf (out, "typedef struct \n");
		fprintf (out, "{\n");
		fprintf (out, "    unsigned char         b_class;\n");
		fprintf (out, "    unsigned short int    b_subclass;\n");
		fprintf (out, "    unsigned char         b_length;\n");
		fprintf (out, "    unsigned char         b_token;\n");
		fprintf (out, "    char                  * at_name;\n");
/*		fprintf (out, "    unsigned short int    w_urm_value;\n"); */
		fprintf (out, "}  key_keytable_entry_type;\n");

		fclose (out);

		/*
		 * end phase 0, start phase 1.  
	 	 */

		phase++;

	    	out = fopen ("UilKeyTab.h", "w");

	        if (out == NULL)
	  	    printf ("ERROR, couldn't open UilKeyTab.h\n");
		else
		    printf ("Created UilKeyTab.h\n");

		phase_1 (out, FALSE);
		phase_1 (out, TRUE);

		fclose (out);

		/*
		 * end phase 1, start phase 2.  
	 	 */

		phase++;

	    	out = fopen ("UilTokName.h", "w");

	        if (out == NULL)
	  	    printf ("ERROR, couldn't open UilTokName.h\n");
		else
		    printf ("Created UilTokName.h\n");

		phase_2 (out);

		fclose (out);

		stats ();

	        return (1);
	    }
	    break;

	case 1:
	    break;
    }

    return (1);
}


