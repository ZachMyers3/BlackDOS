#include "blackdos.h"

/*General functions*/
void handleInterrupt21(int, int, int, int);
void printChar(char);
int isChar(char*);
int isCapital(char*);
int getLength(char*);
int getSpaceIndex(char*);
int mod(int, int);
int div(int, int);
void compareString(char*, char*);
void error(int);
void stop();
/*part 1 functions*/
void printString(char*);
void readString(char*);
void writeInt(int);
void readInt(int*);
void clearScreen(int, int);
/*part 2 functions*/
void readSector(char*, int);
void readFile(char*, char*, int*);
void runProgram(char*, int);
/*part 3 functions*/
void writeSector(char*, int);
void deleteFile(char*);
void writeFile(char*, char*, int);
int checkDir(char*);

void main()
{
	char test[7];
	makeInterrupt21();
	/* clearscreen and change colors */
	interrupt(33, 12, 1, 10, 0);

	/* graphical header */
	interrupt(33, 0, "  ____  _            _    ____   ___  ____  \r\n\0", 0, 0);
	interrupt(33, 0, " | __ )| | __ _  ___| | _|  _ \\ / _ \\/ ___| \r\n\0", 0, 0);
	interrupt(33, 0, " |  _ \\| |/ _` |/ __| |/ | | | | | | \\___ \\ \r\n\0", 0, 0);
	interrupt(33, 0, " | |_) | | (_| | (__|   <| |_| | |_| |___) |\r\n\0", 0, 0);
	interrupt(33, 0, " |____/|_|\\__,_|\\___|_|\\_|____/ \\___/|____/ \r\n\0", 0, 0);
	interrupt(33, 0, "\r\n\0", 0, 0);
	interrupt(33, 0, "V. 1.02, C. 2016. Based on a project by M. Black. \r\n\0", 0, 0);
	interrupt(33, 0, "Author: Eric Harvey.\r\n\r\n\0", 0, 0);

	/* load the shell */
	PRINTS("Welcome to the BlackDOS shell\r\n\n\0");
	interrupt(33, 4, "Shell\0", 2, 0);
	interrupt(33, 0, "Error if this executes.\r\n\0", 0, 0);

	/* hang the system */
	while (1);
}

/*******************************************************************************
*
*   General Functions
*
*******************************************************************************/

void handleInterrupt21(int ax, int bx, int cx, int dx)
{
	if (ax == 0) printString(bx);
	else if (ax == 1) readString(bx);
	else if (ax == 2) readSector(bx, cx);
	else if (ax == 3) readFile(bx, cx);
	else if (ax == 4) runProgram(bx, cx);
	else if (ax == 5) stop();
	else if (ax == 6) writeSector(bx, cx);
	else if (ax == 7) deleteFile(bx);
	else if (ax == 8) writeFile(bx, cx, dx);
	else if (ax == 11) interrupt(25, 0, 0, 0, 0); //reboot
	else if (ax == 12) clearScreen(bx, cx);
	else if (ax == 13) writeInt(bx);
	else if (ax == 14) readInt(bx);
	else if (ax == 15) error(bx);

	else printString("Invalid interrupt\0");
}

/* print individual chars */
void printChar(char ch)
{
	char* chars[2];
	chars[0] = ch;
	chars[1] = '\0';
	printString(chars);
}

/* check for char datatype */
int isChar(char* test)
{
	if ((test >= 32) && (test <= 126))
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

int getLength(char* string)
{
	int i = 0;
	while (string[i] != 0)
	{
		i++;
	}
	return i;
}

/* function for "%" operator */
int mod(int a, int b)
{
	int x = a;
	while (x >= b) x = x - b;
	return x;
}

/* function for "/" operatior */
int div(int a, int b)
{
	int q = 0;
	while (q * b <= a) q++;
	return (q - 1);
}

/* To check two string as a match */
int stringCompare(char* indexed, char* fileName)
{
	int i, match = 0;
	/* check first 6 chars (loadFile max name length) */
	for (i = 0; i < 6; i++)
	{
		/* ignore special chars*/
		if (indexed[i] == 0x0 || indexed[i] == '\r' || indexed[i] == '\n') break;
		if (indexed[i] == fileName[i]) match = 1;
		else
		{
			match = 0;
			break;
		}
	}
	return match;
}

/* prints error messages */
void error(int bx)
{
	char errMsg0[18], errMsg1[17], errMsg2[13], errMsg3[17];

	errMsg0[0] = 70; errMsg0[1] = 105; errMsg0[2] = 108; errMsg0[3] = 101;
	errMsg0[4] = 32; errMsg0[5] = 110; errMsg0[6] = 111; errMsg0[7] = 116;
	errMsg0[8] = 32; errMsg0[9] = 102; errMsg0[10] = 111; errMsg0[11] = 117;
	errMsg0[12] = 110; errMsg0[13] = 100; errMsg0[14] = 46; errMsg0[15] = 13;
	errMsg0[16] = 10; errMsg0[17] = 0;
	errMsg1[0] = 66; errMsg1[1] = 97; errMsg1[2] = 100; errMsg1[3] = 32;
	errMsg1[4] = 102; errMsg1[5] = 105; errMsg1[6] = 108; errMsg1[7] = 101;
	errMsg1[8] = 32; errMsg1[9] = 110; errMsg1[10] = 97; errMsg1[11] = 109;
	errMsg1[12] = 101; errMsg1[13] = 46; errMsg1[14] = 13; errMsg1[15] = 10;
	errMsg1[16] = 0;
	errMsg2[0] = 68; errMsg2[1] = 105; errMsg2[2] = 115; errMsg2[3] = 107;
	errMsg2[4] = 32; errMsg2[5] = 102; errMsg2[6] = 117; errMsg2[7] = 108;
	errMsg2[8] = 108; errMsg2[9] = 46; errMsg2[10] = 13; errMsg2[11] = 10;
	errMsg2[12] = 0;
	errMsg3[0] = 71; errMsg3[1] = 101; errMsg3[2] = 110; errMsg3[3] = 101;
	errMsg3[4] = 114; errMsg3[5] = 97; errMsg3[6] = 108; errMsg3[7] = 32;
	errMsg3[8] = 101; errMsg3[9] = 114; errMsg3[10] = 114; errMsg3[11] = 111;
	errMsg3[12] = 114; errMsg3[13] = 46; errMsg3[14] = 13; errMsg3[15] = 10;
	errMsg3[16] = 0;

	switch (bx) {
	case 0: printString(errMsg0); break;
	case 1: printString(errMsg1); break;
	case 2: printString(errMsg2); break;
	default: printString(errMsg3);
	}
}

/* to hang the system */
void stop()
{
	launchProgram(8192);
}

/*******************************************************************************
*
*   Part 1: System Calls
*
*******************************************************************************/

/* To print a given string to console */
void printString(char* string)
{
	int i = 0;
	char c = string[i];

	while (c != 0)
	{
		interrupt(0x10, 0xE * 256 + c, 0, 0, 0);
		c = string[++i];
	}
}


/* To read in a given string from user input */
void readString(char* string)
{
	int enter = 0xD;
	int backsp = 0x8;
	int i = 0;

	while (1)
	{
		/* interrupt to store keystroke */
		char c = interrupt(0x16, 0, 0, 0);
		if (c == enter)
		{
			/* when enter is pressed print a new line
			   and return since user is finished */
			interrupt(0x10, 0xE * 256 + '\r', 0, 0, 0);
			interrupt(0x10, 0xE * 256 + '\n', 0, 0, 0);
			string[i] = 0x0;
			return;
		}
		else if (c == backsp)
		{
			/* i > 0 check to make sure there is something to delete */
			if (i > 0)
			{
				/* properly handle backspace printing */
				string[i] = 0x0;
				i--;
				interrupt(0x10, 0xE * 256 + 0x8, 0, 0, 0);
				i++;
				interrupt(0x10, 0xE * 256 + 0x0, 0, 0, 0);
				i--;
				interrupt(0x10, 0xE * 256 + 0x8, 0, 0, 0);
			}
		}
		else
		{
			/* add all pressed keys to the string and dispaly */
			string[i] = c;
			interrupt(0x10, 0xE * 256 + c, 0, 0, 0);
			i++;
		}
	}
}


/* To print a given 5 digit integer to console */
void writeInt(int x)
{
	char number[6], *d;
	int q = x, r;

	if (x < 1)
	{
		d = number; *d = 0x30; d++; *d = 0x0; d--;
	}
	else
	{
		d = number + 5;
		*d = 0x0; d--;

		while (q > 0)
		{
			r = mod(q, 10); q = div(q, 10);
			*d = r + 48; d--;
		}
		d++;
	}

	printString(d);
}

/* To read a given 5 digit integer from user input */
void readInt(int* number)
{
	int result = 0;
	int i = 0;

	char line[6];
	readString(line);

	/* while not the end of the "integer string" ('0') */
	for (; line[i] != '\0'; ++i)
	{
		/* convert from ASCII to int */
		result = result * 10 + line[i] - '0';
	}

	*number = result;
}

/* To clear the screen and change text and background colors */
void clearScreen(int bx, int cx)
{
	int i;

	/* Print a bunch of new lines to clear the screen */
	for (i = 0; i < 24; i++)
	{
		printString("\r\n");
	}

	/* Check for valid background and text colors */
	if (bx > 8 || cx > 16) return;

	else
	{
		/* interrupt to reposition cursor to the top left */
		interrupt(0x10, 0x200, 0, 0, 0);
		if (bx > 0 && cx > 0)
		{
			/* call interrupt to change display colors with given input*/
			interrupt(0x10, 0x600, 0x1000 * (bx - 1) + 0x100 * (cx - 1), 0, 0x184F);
		}
	}
}

/*******************************************************************************
*
*   Part 2: Loading and Executing Programs
*
*******************************************************************************/

/* To read a given sector on the disk */
void readSector(char* buffer, int sector)
{
	/* convert to relative sector */
	int relSecNo = mod(sector, 18) + 1;
	int head = mod(div(sector, 18), 2);
	int track = div(sector, 36);

	/* call interrupt read sector */
	interrupt(0x13, 2 * 256 + 1, buffer, track * 256 + relSecNo, head * 256 + 0);
}

/* To read a file at a given sector and load it to a buffer */
void readFile(char* fileName, char* buffer)
{
	char directory[512];
	char foundFile[6];
	int i, j, k, l, fileStart, fileEnd, found;

	/* read in the directory sector*/
	readSector(directory, 2);

	/* loop through the directory */
	for (i = 0; i < 16; i++)
	{
		fileStart = i * 32;
		fileEnd = fileStart + 6;

		k = 0;
		for (j = fileStart; j < fileEnd; j++)
		{
			foundFile[k] = directory[j];
			k++;
		}
		foundFile[6] = '\0';

		/* check if the file matches the given file name */
		if (stringCompare(foundFile, fileName))
		{
			found = 1;

			/* get the sectors */
			for (l = fileEnd; directory[l] != 0x0; l++)
			{
				readSector(buffer, directory[l]);
				buffer = buffer + 512;
			}
		}
	}

	if (!found)
	{
		error(0);
		return;
	}
}

/* To load program to memory and execute it */
void runProgram(char* fileName, int segment)
{
	char buffer[13312];
	int i;

	/* load the file into a buffer */
	readFile(fileName, buffer);
	/* multiple to derive the base location of the indicated segment */
	segment = segment * 0x1000;
	for (i = 0; i < 13312; i++)
	{
		/* transfer the file from the buffer to memory */
		putInMemory(segment, i, buffer[i]);
	}
	launchProgram(segment);
}

/*******************************************************************************
*
*   Part 3: Shell and Writing to Disk
*
*******************************************************************************/

void writeSector(char* buffer, int sector)
{
	/* To read a given sector on the disk */
	int relSecNo = mod(sector, 18) + 1;
	int head = mod(div(sector, 18), 2);
	int track = div(sector, 36);

	/* call interrupt read sector */
	interrupt(0x13, 3 * 256 + 1, buffer, track * 256 + relSecNo, head * 256 + 0);
}

void deleteFile(char* fileName)
{
	char map[512];
	char directory[512];
	char currentFile[7];
	int i, j, k, l, fileStart, fileEnd, index, found = 0;

	/*Load the Directory and Map to 512 byte character arrays*/
	readSector(map, 1);
	readSector(directory, 2);

	/*Search through the directory and try to find the file name.*/
	for (i = 0; i < 16; i++)
	{
		fileStart = i * 32;
		fileEnd = fileStart + 6;
		k = 0;
		for (j = fileStart; j < fileEnd; j++)
		{
			currentFile[k] = directory[j];
			k++;
		}
		currentFile[6] = '\0';

		if (stringCompare(currentFile, fileName))
		{
			found = 1;
			directory[fileStart] = 0x0;
			for (l = fileEnd; directory[l] != 0x0; l++)
			{
				index = directory[l];
				map[index + 1] = 0x00;
			}
		}
	}
	/* handle "file not found" */
	if (!found)
	{
		error(0);
		return;
	}
	/* Write the character arrays back to their appropriate sectors */
	writeSector(map, 1);
	writeSector(directory, 2);
}

void writeFile(char* fileName, char* buffer, int numOfSectors)
{
	char map[512];
	char directory[512];
	char content[512];
	int i, j, k, l, m, n, p, fileStart, fileEnd, sectorsAssigned = 0;
	int namelength = getLength(fileName);
	int contentLength = getLength(buffer);

	/* load the map and directory */
	readSector(map, 1);
	readSector(directory, 2);

	/* search the directory */
	for (i = 0; i < 16; i++)
	{
		fileStart = i * 32;
		fileEnd = fileStart + 6;

		if (!checkDir(fileName))
		{
			error(1);
			return;
		}

		/* Find a free directory entry */
		if (directory[fileStart] == 0x00)
		{

			for (j = 0; j < namelength + 1; j++)
			{
				directory[fileStart + j] = fileName[j];
				for (k = namelength; k < 7; k++)
				{
					directory[fileStart + k] = 0x00;
				}
			}

			p = fileEnd;
			/* serach the diskmap */
			for (l = 0; l < 512 && sectorsAssigned < numOfSectors; l++)
			{
				/* check for free sector */
				if (map[l] != 0xFF)
				{
					if (map[l] == 0xFF && sectorsAssigned == 511)
					{
						error(2);
						return;
					}

					n = 0;
					map[l] = 0xFF;
					/* Write 512 bytes from the buffer holding the file to that sector */
					for (m = sectorsAssigned * 512; m < 512 + (sectorsAssigned * 512); m++)
					{
						content[n] = buffer[m];
						n++;
					}
					sectorsAssigned++;
					writeSector(content, l);
					m = 0;
					directory[p] = l;
					p++;
				}
			}
			break;
		}
	}
	writeSector(directory, 2);
	writeSector(map, 1);
}

/* Check the directory to see if the fileName already exists */
int checkDir(char* fileName)
{
	char directory[512];
	char currentFile[7];
	int i, j, k, fileStart, fileEnd;

	readSector(directory, 2);
	for (i = 0; i < 16; i++)
	{
		fileStart = i * 32;
		fileEnd = fileStart + 6;
		k = 0;
		for (j = fileStart; j < fileEnd; j++)
		{
			currentFile[k] = directory[j];
			k++;
		}
		currentFile[6] = '\0';

		if (stringCompare(currentFile, fileName))
		{
			return 0;
		}
	}
	return 1;
}
