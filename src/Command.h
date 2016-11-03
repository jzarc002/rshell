#ifndef __COMMANDS__
#define __COMMANDS__

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
using namespace std;

/*
BASE: Abstract Base Class where Command and Connectors inherit from
-flag1 stores whether or not the first command was successful
-flag0 stores if we are looking at the first command in the line
*/
class Base {
    public:
    
    virtual void getInput() = 0;
    string input;
    int flag1, flag0;
};

/* 
COMMAND: class will parse the input, and execute the commands
-connectors store the string value of all the connectors in order
-userCommands stores all commands
*/
class Command : public Base {
    private:
    vector<string> connectors; 
    vector<string> userCommands; 
    char** command;
    
    public:
    bool exitflag;
    bool enter;
    bool commentflag;
    
    Command() {
        enter = false;
        exitflag = false;
        commentflag = false;
        flag0 = -1;
    }
    
    Command(string s) { 
        enter = false;
        exitflag = false;
        commentflag = false;
        flag0 = -1;
        input = s;
    }
    
    /*Checks if user entered newline*/
    bool checkEnter() {
        return enter;
    }
    
    /* 
    Here we get the input from the user, parse the input
    and store all of the commands
    */
    void getInput() {
        
        //GETLOGIN AND HOSTNAME
        char* user;
        char host[300];
        
        if ((gethostname(host, 100)) == -1){
		    cout << "gethostname failed" << endl;
    	}
    	if ((user = getlogin()) == NULL){
    		cout << "getlogin failed" << endl;
    	}
	
	    cout << user << "@" << host;
        
        enter = false;
        exitflag = false;
        commentflag = false;
        
        string s;
        cout << "$ ";
        getline(cin, s);
        input = s;
        
        if(s.empty() == true){
            enter = true;
        }
        
        //Adds all connectors to a vector
        for(unsigned int i = 0; i < s.size(); ++i){
            if(s[i] == '&'){
                if(s[i + 1] == '&'){
                    connectors.push_back("AND");
                }
            }
            else if(s[i] == '|'){
                if(s[i + 1] == '|'){
                    connectors.push_back("OR");
                }
            }
            else if(s[i] == ';'){
                connectors.push_back("SEMICOLON");
            }
            else if(s[i] == '#'){
                connectors.push_back("COMMENT");
                i = 1000;
            }
        }
        
        //Adds all parsed commands to a vector
        int y = 0;
        char *str = const_cast<char *>(s.c_str());   
        char *tok;
        tok = strtok(str, "&&||;#" );  
        while(tok != NULL){
            if(tok[0] != ' '){
                userCommands.push_back(tok);
            }
            else{
                y = 0;
                while(tok[y] == ' '){
                    ++y;
                }
                //cout << tok + y << endl;
                userCommands.push_back(tok + y);
            }
            tok = strtok(NULL, "&&||;#");
        }
        // for(unsigned i = 0; i < userCommands.size(); ++i){
        //     cout << userCommands.at(i) << endl;
        // }
    }
    
    
    /* 
    This function checks the type of connector between two commands.
    With a flag it will determine whether or not to execute the 
    second command.
    */
    void callConnectors() {
        
        unsigned j = 0;
        
        /* Case for extra connectors at end and/or beginning */
        if ( (connectors.size() >= userCommands.size()) && (connectors.empty() != true) ){
            while(input[j] == ' '){
                ++j;
            }
            if (input[j] == '#'){
                return;
            }
            if(input[j] == '&' || input[j] == ';' || input[j] == '|'){
                cout << "SYNTAX ERROR" << endl;
                return;
            }
            else if(input[input.size() - 1] == '&' || input[input.size() - 1] == ';' || input[input.size() - 1] == '|'){
                cout << "SYNTAX ERROR" << endl;
                return;
            }
        }
        
        //Checks if first connector is a comment but there's a command before it
        for(unsigned k = 0; k < connectors.size(); ++k){
            if(connectors.at(k) == "COMMENT"){
                if(k == 0){
                    if(userCommands.at(0)[0] != '#'){
                        //connectors.erase(connectors.begin());
                        commentflag = true;
                    }
                }
            }
        }
        
        unsigned i = 0;
        
        /*While there is still commands, we will enter this loop*/
        while(userCommands.empty() != true){
            
            /* Single command*/
            if(connectors.empty() == true){ 
                execute();
                userCommands.erase(userCommands.begin());
            }
            /* Multiple commands */
            else if(commentflag == true){
                execute();
                userCommands.erase(userCommands.begin());
                return;
            }
            else if(connectors.empty() != true){ 
                
                /* Next connector is &&*/
                if(connectors.at(i) == "AND"){
                    
                    /* Check if first command */
                    if(flag0 == -1){ 
                        execute();
                        connectors.erase(connectors.begin());
                        userCommands.erase(userCommands.begin());
                        
                        /* Executes second command */
                        if(userCommands.empty() != true && flag1 == 1){
                            execute();
                            userCommands.erase(userCommands.begin());
                        }
                        /* Doesn't execute second command */
                        else if(userCommands.empty() != true){
                            userCommands.erase(userCommands.begin());
                            flag1 = 0;
                        }
                    }
                    /* Executes next command if previous one was successful */
                    else if(flag1 == 1) {
                        execute();
                        connectors.erase(connectors.begin());
                        userCommands.erase(userCommands.begin());
                    }
                    /* Doesn't execute next command because previous one was
                    not successful*/
                    else{ 
                        connectors.erase(connectors.begin());
                        userCommands.erase(userCommands.begin());
                        flag1 = 0;
                    }
                    
                    /* Increment first command flag */ 
                    ++flag0;
                }
                
                /* Next connector is || */
                else if(connectors.at(i) == "OR"){
                    
                    /* Checks if first command */
                    if(flag0 == -1){ 
                        execute();
                        connectors.erase(connectors.begin());
                        userCommands.erase(userCommands.begin());
                        
                        /* Executes second command */
                        if(userCommands.empty() != true && flag1 == 0){
                            execute();
                            userCommands.erase(userCommands.begin());
                        }
                        /* Doesn't execute second command */
                        else if(userCommands.empty() != true){
                            userCommands.erase(userCommands.begin());
                            flag1 = 0;
                        }
                    }
                    /* Executes next command if previous one failed */
                    else if(flag1 == 0) {
                        execute();
                        connectors.erase(connectors.begin());
                        userCommands.erase(userCommands.begin());
                    }
                    /* Doesn't execute next command because previous 
                    one was successful*/
                    else{ 
                        connectors.erase(connectors.begin());
                        userCommands.erase(userCommands.begin());
                        flag1 = 0;
                    }
                    
                    /* Increment first command flag */
                    ++flag0;
                }
                /* Next connector is ; */
                else if(connectors.at(i) == "SEMICOLON"){
                    
                    /* Checks if first command */
                    if(flag0 == -1){
                        execute();
                        connectors.erase(connectors.begin());
                        userCommands.erase(userCommands.begin());
                        
                        /* Executes second command */
                        if(userCommands.empty() != true){
                            execute();
                            userCommands.erase(userCommands.begin());
                        }
                    }
                    /* Executes next command */
                    else{
                        execute();
                        connectors.erase(connectors.begin());
                        userCommands.erase(userCommands.begin());
                    }
                    ++flag0;
                }
                /* Next onnector is a comment */
                else{ 
                    
                    /* Clear vector */
                    while(userCommands.empty() != true){
                        userCommands.pop_back();
                    }
                }
                ++flag0;
            }
        }
    }
    
    /* 
    This function will execute the first command in userCommands vector
    */
    void execute() {
        //CHECKING TO SEE IF FIRST OR ANY COMMAND RUNS
        if(userCommands.at(0) == "exit"){
            exitflag = true;
            exit(0);
        }
        
        //Checking to see whether or not there was userInput
        if(userCommands.at(0) == ""){
            return;
        }
        
        /*Further parsing the userCommand.at(0), removing spaces and 
        creating a vector of strings to be converted into char*/
        vector <string> everyWord;
        string userInput = userCommands.at(0);
        for(unsigned i = 0; i < userInput.size(); ++i){
            int posSpace = userInput.find(' ');
            string word1 = userInput.substr(0, posSpace);
            everyWord.push_back(word1);
            userInput.erase(0, everyWord.at(i).size()+1);
        }
        if(everyWord.size() == 0){
            everyWord.push_back(userInput);
        }
        
        /*Creating char array charCommands of the vector everyWord 
        to use with execvp()*/
        char** charCommands = new char*[everyWord.size() + 1];
        for(size_t i = 0; i < everyWord.size(); ++i){
            charCommands[i] = new char[everyWord.at(i).size()+1];
 		    strcpy(charCommands[i], everyWord.at(i).c_str());
        }
        charCommands[everyWord.size()] = NULL;

        
        //STARTING EXECUTION PROCESS
        int status = 0;
        
        //Forking into child process
        pid_t pID = fork();   
        if(pID < 0){
            perror("Fork failed to execute");
            exit(0);
        }
        //Checking to see if it is the child process
        if(pID == 0){ 
            //Passing in the char array charCommands
            if(execvp(charCommands[0], charCommands) < 0){
                //if -1 returned, execution failed
                flag1 = 0;
                perror("Command not recognized or execution failed");
                exit(0);
            }
        }//If not the child process
        else{
            //Checking to see if parent process waited for child
            if(waitpid(pID, &status, 0) != pID){
                perror("waitpid() error");
            }
        }
        //Increment first flag that connector checks
        flag1 = 1;
    }
};

#endif
