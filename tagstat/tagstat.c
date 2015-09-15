/* 
 * tagstat.c
 *
 * Read from /proc/tag and prints out all the proccess
 * and their respective tags
 *
 * Assumes that my LKM for ptag has been install
 * and it will return a segfault if it's not
 *
 * Make and then run with ./tagstat 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
	int n;
	char tags[1000];
	memset(tags, 0, 700);
	FILE *file = fopen("/proc/ptag", "r");
	
	if (file == NULL)
		printf("Please install LKM");
	fread(tags, 2, 1000, file);
	printf("%s", tags);
	
	return 0;
}
