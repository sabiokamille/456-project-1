#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <errno.h>

int fileSize(FILE *fp){
	long size;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);
	return size;
}

void main(int argc, char *argv[]){
	//check if enough arguments have been input by user
	if (argc < 3)
	{
		printf("not enough arguments!\n");
		exit(0);
	} else
	{
		int next_opt;

		//short option string
		const char* const short_options = "s";
		//long option array
		const struct option long_options[] = {
			{"systemcalls", 0, NULL, 's'},
			{NULL, 0, NULL, 0}
		};

		while (1) //parse options until no more left
		{
				next_opt = getopt_long(argc, argv, short_options, long_options, NULL);
			if(next_opt < 0) //nore more options
			{
				break;
			}
			switch (next_opt)
			{
				case 's':;
					int fd = open(argv[optind], O_RDONLY, 0);
					if (fd < 0) 
					{
						perror(argv[optind]);
						exit(1);
					}
					else {
					int sz = lseek(fd, 0, SEEK_END);
					lseek(fd, 0, SEEK_SET);
					char *buffer = (char*) calloc(sz, sizeof(char));
                                        read(fd, buffer, sz);
					//int n;
					//while((n = read(fd, buffer, 1) > 0)){}
					close(fd);

					int count = 0;
					int match = 1;
					//for loop iterated through each substring input by user and searches
					//for it in the buffer containting the contents of the file input
					//by user
					for(int i = optind + 1; i < argc; i++)
					{
						int substringLength = strlen(argv[i]); //length of substring
						count = 0;
			
						//for loop to iterate through each character in the buffer to
						//compare with substring
						for(int j = 0; j < strlen(buffer) - substringLength; j++)
						{
							match = 1;

							//for loop to iterate through each letter in the 
							//substring and compare it to the buffer
							for (int k = 0; k < substringLength; k++)
							{
								if (tolower(buffer[j + k]) != tolower(argv[i][k]))
								{
									match = 0;
								}
							}
							if (match)
							{
								count++;
							}
						}
						printf("%d\n", count);
					}
					}
					exit(0);
				case '?':
					printf("Option not recognized\n");
					exit(1);
				default:
					abort();
			}
		}
		//initialize file pointer for file input by user and initialize
		//dynaimcally allocated char array to hold substrings of buffer
		//and initialize and fill buffer with contents of file
		FILE *fp;
		fp = fopen(argv[1],"r");
		if (fp == NULL)
		{
			perror(argv[1]);
			exit(0);
		}
		int size = fileSize(fp);
		char *buffer = (char*) calloc(size, sizeof(char)); 
		fread(buffer, size, 1, fp);
		//int n;
		//while (n = fread(buffer, 1,1,fp) > 0){}
		fclose(fp);
		if (!buffer)
		{
			printf("Memory allocation failed\n");
			exit(0);
		}
		int count = 0;
		int match = 1;
		//for loop iterated through each substring input by user and searches
		//for it in the buffer containting the contents of the file input
		//by user
		for(int i = 2; i < argc; i++){
			int substringLength = strlen(argv[i]); //length of substring
			count = 0;

			//for loop to iterate through each character in the buffer to
			//compare with substring
			for(int j = 0; j < strlen(buffer) - substringLength; j++)
			{
				match = 1;

				//for loop to iterate through each letter in the 
				//substring and compare it to the buffer
				for (int k = 0; k < substringLength; k++)
				{
					if (tolower(buffer[j + k]) != tolower(argv[i][k]))
					{
						match = 0;
					}
				}
				if (match)
				{
					count++;
				}
			}
			printf("%d\n", count);
		}
	}
	exit(0);
}

