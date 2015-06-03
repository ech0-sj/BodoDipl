/*
 * Tools.c
 *
 * Created: 24.05.2015 18:37:33
 *  Author: ech0
 */ 
#include "Tools.h"

#include "string.h"


uint32_t ParseIpToArray( char* ipAstring, uint8_t** ipArray )
{
	int i;
	int oktettAsInt;
	char* ptr = NULL;
	
	ptr = strtok( ipAstring, ".");
	for( i = 0; (i < 4) && (ptr != NULL); i++ )
	{
		oktettAsInt = atoi( ptr );
		ipArray[i] = (uint8_t)oktettAsInt & 0xFF;
		ptr = strtok(NULL, ".");
	}
	if( i > 3 )
	{
		return 0; 
	}
	else
	{
		return -1;
	}
	
}


/* find the next word starting at 's', delimited by characters
 * in the string 'delim', and store up to 'len' bytes into *buf
 * returns pointer to immediately after the word, or NULL if done.
 gefunden: http://pjd-notes.blogspot.de/2011/09/alternative-to-strtok3-in-c.html
 
 example:
 char line[some_length];
 char argv[10][20];
 int argc;
 for (argc = 0; argc < 10; argc++) {
	 line = strwrd(line, argv[argc], sizeof(argv[argc]), " \t");
	 if (line == NULL)
	 break;
 }
 
 
 */
char *strwrd(char *s, char *buf, size_t len, char *delim)
{
    s += strspn(s, delim);
    int n = strcspn(s, delim);  /* count the span (spn) of bytes in */
    if (len-1 < n)              /* the complement (c) of *delim */
        n = len-1;
    memcpy(buf, s, n);
    buf[n] = 0;
    s += n;
    return (*s == 0) ? NULL : s;
}