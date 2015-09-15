/*
 * ptag.c - This is a utility to add OR remove a tag from 
	    a given process
 *
 * 
 * Usage: make -> ./ptag PID (-a or -r) tag
 *
 * Note: if a process has more than one 1 for the same string
 *       when you attmpet to remove it, both tags will be removed
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>

int main(int argc, char **argv)
{
				
	int func = 1;
	int size = 0;
	int pid;
	int option;
	int aFlag = 0;
	int rFlag = 0;
	char *label;

	if (argc > 4 || argc < 2)
	{
		printf("Please use the correct amount of arguments\n");
		exit(1);
	}

	while ((option = getopt(argc, argv, "a:r:")) !=-1)
	{
		switch(option)
		{
			/*
			 * case a: adding a tag to a proccess
 			 * case r: remvoing a tag from a process
			 *
			 */
			case 'a':
				label = optarg;
				aFlag = 1;
				func = 1;
				size = strlen(optarg);
				break;
	
			case 'r':
				if (aFlag == 1)
				{
					break;
				}
				label = optarg;
				rFlag = 1;
				size = strlen(optarg);
				func = 0;
				break;
				
		}
	
	}

	/*
	 * Since our PID is not an option argument it gets moved
	 * To the end of the argument list
	 */	
	pid = atoi(argv[argc-1]);
			
	syscall(337, pid, func, label, size);
	return 0;
}

