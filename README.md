### rshell

--------------

#UCR CS100 Assignment
Fall 2016

---------------------

Description of Command Shell 

- Basic and limited terminal

- Executes user's command input 

+ Does not read in or execute comments following # symbol

- Accepts Command Chaining 'echo name && mkdir test ; ls'

  + '&&': Executes previous then the latter command if previous is successful

  + '||': Executes following command only if the previous fails

  + ';' : Executes following command no matter what

+ Displays error messages if failed execution at any point

- Exits the terminal using the command 'exit'


# Known Bugs
- The command following || is not executed if the first command is not recognize 
- Include quotation marks when echoing

- No variations of the word exit can be used 'Exit' or 'EXIT'

- No built in bash commands work

