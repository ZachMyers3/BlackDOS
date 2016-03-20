This is my completed version of part 2 of the project.

To test the current version:
  1. cd to the folder location.
  2. Allow compileOS.sh executable permissions.
  3. Run ./compileOS.sh from the terminal.
  4. Then call bochs to boot using "bochs -f osxterm.txt". The OS is now loaded

  The main() is currently setup to run the fibonacci program.
  It will execute and give a sequence of fib numbers up to the given input.

  The fib program is loaded to the disk from the compileOS script using ./loadFile
  It is then read using readSector, then the file is read using readFile
  Finally it's executed using runProgram.



New functions:

readSector:
  This is a function that reads a given sector on the disk.

readFile:
  This is a function that reads a file at a given sector and loads it to a buffer.

runProgram:
  This is to load a compiled program to memory and execute it.

compareString:
  This is a helper function for readFile.
  It checks two given strings char-by-char to determine if they are matching.

Error:
  A function for custom error handling

Stop:
  A function to hang the system, which stops it.
