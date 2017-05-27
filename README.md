# Unix-shell-Interpreter
Implemented Shell Interpreter using pipe, fork, system and dup2 calls

INSTRUCTION TO RUN FILE : NamruthaShell.c
// This file supports execution of commands separated by 'pipe'.
*********************************************************************************
Steps For Execution :
---------------------------------------------------------------------------------
1. gcc NamruthaShell.c -o NamruthaShell
2. ./NamruthaShell

//Following shell will be displayed :
$Namrutha -->

3. Enter the command , preferably commands separated by pipes.

NOTE-1:  Provide space before and after the pipe command.
PROPER USAGE   : ls -l | tail -2 | cut -b 1-10
IMPROPER USAGE : ls -l|tail -2| cut -b 1-10

NOTE-2:  Do not enclose arguments in quotes.
PROPER USAGE   : cat file1.txt | grep text_format | cut -b 1-10
IMPROPER USAGE : cat "file1.txt" | grep 'text_format' | cut -b '1-10'

4. Enter 'exit' to come out of the shell interpreter($Namrutha -->).
USAGE : $Namrutha --> exit

*********************************************************************************
Command samples :
---------------------------------------------------------------------------------
1. ls -l
2. cat filename
3. cat filename | grep pattern | wc -l
4. ls - l | grep at | wc
*********************************************************************************
LIMITATIONS :
---------------------------------------------------------------------------------
* No support for more than 3 commands separated by pipes.
* Space has to be provided before and after the pipe.
