#include "blackdos.h"

int isChar(char*);
int isCapital(char*);
int getSpaceIndex(char*);
int getLength(char*);
void printDir();
void copyFile(char*, char*);
int stringCompare(char*, char*, int);

void main()
{
	char buffer[13312];
	char commandName[80];
	char content[140];
	char arg1[7];
	char arg2[7];
	int i, j, index, length;

	while(1)
	{

		PRINTS("cxxxx][===blackdos===>> \0");
		SCANS(commandName);

		if (stringCompare(commandName, "boot", 4)) interrupt(33,11,0,0,0);
		else if (stringCompare(commandName, "dir", 3)) printDir();
		else if (stringCompare(commandName, "run", 3)) interrupt(33,4,commandName+4,4,0);
		else if (stringCompare(commandName, "del", 3))
		{
			if(isCapital(commandName[4]))
			{
				interrupt(33,15,1,0,0);
				break;
			}
			interrupt(33,7,commandName+4,0,0);
		}
		else if (stringCompare(commandName, "cls", 3))
		{
			for(i = 0; i < 26; i++)
			{
				PRINTS("\r\n\0");
			}
			interrupt(33,12,0,0,0);
		}
		else if (stringCompare(commandName, "echo", 4))
		{
			PRINTS(commandName+5);
			PRINTS("\r\n\0");
		}
		else if (stringCompare(commandName,"copy", 4))
		{
			index = getSpaceIndex(commandName) + 1;
			length = getLength(commandName);

			if(isCapital(commandName[5] || isCapital(commandName[index])))
			{
				interrupt(33,15,1,0,0);
				break;
			}
			for(i = 5; i < index -1; i++)
			{
				arg1[i-5] = commandName[i];
			}
			for(i = index ; i < length+1; i++)
			{
				arg2[i-index] = commandName[i];
			}
			arg1[6] = '\0';
			arg2[6] = '\0';


			copyFile(arg1, arg2);
		}
		else if (stringCompare(commandName, "help", 4))
		{
			PRINTS("User manual to be added.\r\n\0");
		}
		else if(stringCompare(commandName,"tweet",5))
		{
			if(isCapital(commandName[6]))
			{
				interrupt(33,15,1,0,0);
				break;
			}
			PRINTS("Enter your tweet: ");
			SCANS(content);
			interrupt(33,8,commandName+6,content,1);
			PRINTS(buffer);
		}
		else if(stringCompare(commandName,"type",4))
		{
			interrupt(33,3,commandName+5,buffer,0);
			PRINTS(buffer);
			PRINTS("\r\n\0");
		}
		else PRINTS("Bad command or file name.\r\n\0");
	}
}

int isChar(char* test)
{
	if((test >= 32) && (test <= 126))
	{
		return 1;
	}
	return 0;
}


int isCapital(char* ch)
{
	if (ch >= 'A' && ch <= 'Z')
	{
		return 1;
	}
	else return 0;
}

int getSpaceIndex(char* string)
{
	int i = 5;
	while (string[i] != ' ')
	{
		i++;
	}
	return i;
}

int getLength(char* string)
{
  int i = 0;
  while (string[i] != 0)
  {
    i++;
  }
  return i;
}

/* Goes through the file system and prints each file */
void printDir()
{
  char directory[512];
  char currentFile[9];
	int i, k, j, fileStart, fileEnd;

	interrupt(33,2,directory,2,0,0);
	for(i = 0; i < 16; i++)
	{
		fileStart = i*32;
		fileEnd = fileStart+6;
		k=0;
		for(j=1;j<9;j++)
		{
			currentFile[j]=' ';
		}
		for(fileStart; fileStart < fileEnd; fileStart++)
		{
			if(directory[fileStart]!= 0x00)
			{
				currentFile[k]=directory[fileStart];
			}
			k++;
		}
		currentFile[6]='\r';
		currentFile[7]='\n';
		currentFile[8]='\0';
		if(isChar(currentFile[0]) && !isCapital(currentFile[0]))
		{
			PRINTS(currentFile);
		}
		currentFile[0]=0x00;
	}
}


void copyFile(char* fileName1,char* fileName2)
{
	char buffer[13312];
	char directory[512];
	int i, j, k, filestart, fileend, numOfSectors = 0;
	char currentFile[6];

	interrupt(33,2,directory,2,0);

	for(i=0; i < 16; i++)
	{
		filestart= i*32;
		fileend = filestart+6;
		for(j = 0; j < 6; j++)
		{
			currentFile[j]= directory[filestart+j];
		}
		if(stringCompare(currentFile,fileName1,6))
		{
			for(k=fileend;directory[k]!= 0x00;k++)
			{
				numOfSectors++;
			}
		}
	}
	interrupt(33,3,fileName1,buffer);
	interrupt(33,8,fileName2,buffer,numOfSectors);
}

int stringCompare(char* indexed, char* fileName, int length)
{
	int i;
	for(i = 0; i < length; i++)
	{
		if(indexed[i] != fileName[i])
		{
			return 0;
		}
	}
	return 1;
}
