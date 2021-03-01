#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <unistd.h>

void splitString(std::string text, char d, std::vector<std::string>& result);
void vectorOfStringsToArrayOfCharArrays(std::vector<std::string>& list, char ***result);
void freeArrayOfCharArrays(char **array, size_t array_length);
void recordCommand(std::string command, std::vector<std::string>& history, int* counter); 

int main (int argc, char **argv)
{
    // Get list of paths to binary executables
    std::vector<std::string> os_path_list;
    char* os_path = getenv("PATH");
    splitString(os_path, ':', os_path_list);

    
    /************************************************************************************
     *   Example code - remove in actual program                                        *
     ************************************************************************************/
    // Shows how to loop over the directories in the PATH environment variable
    
    int i;
    for (i = 0; i < os_path_list.size(); i++)
    {
        printf("PATH[%2d]: %s\n", i, os_path_list[i].c_str());
    }
    
    /************************************************************************************
     *   End example code                                                               *
     ************************************************************************************/


    // Welcome message
    printf("Welcome to OSShell! Please enter your commands ('exit' to quit).\n");

    std::vector<std::string> command_list; // to store command user types in, split into its variour parameters
    char **command_list_exec; // command_list converted to an array of character arrays
    char **command_history; 
    int counter = 0;


    // Repeat:
    //  Print prompt for user input: "osshell> " (no newline)
    //  Get user input for next command
    //  If command is `exit` exit loop / quit program
    //  If command is `history` print previous N commands
    //  For all other commands, check if an executable by that name is in one of the PATH directories
    //   If yes, execute it
    //   If no, print error statement: "<command_name>: Error command not found" (do include newline)

    bool running = true; 
    while (running) {
    	/*
    	Recommended using the cis/stat library when checking if a file is available
    	this will allow `struct stat;` to query file vs dir, rwx perms, etc (linux specific)
    	Or std::filesystem
    	But this is 2017 std, and makefile is setting to 2011 (-std=c++11); would need to edit
    	Takes in string, can check "does this exist?" and if exists, "exec perms?"
    	*/
    	
    	//Make command var
    	std::string command; 
    	char* command_parts; 
    	
    	printf("Made command var\n"); 
    	
    	//Prompt and retrieve user input
    	printf("osshell> "); 
		std::getline(std::cin, command); 
		
		
    	//TODO use the splitString() method given by teacher, it's a delimiter splitter
    	//gives back a null-terminated list of strings in given char**
    	
    	printf("Prompted, recieved input\n"); 

    	
    	//splitString(command, ' ', &command_parts); 
    	
    	//printf("Split the string\n"); 
    	
    	std::cout << "[" << command << "]" << std::endl; 
        printf("Before Counter increment\n");
        counter = counter + 1;
        printf("Before recordCommand\n");
    	
        //recordCommand(command, &command_history, &counter); //TODO make this method, or write it here
    	printf("After recordCommand\n");
        
    	if(command == "exit") {
    		printf("exiting..."); 
    		running = false; 
    	} else if (command == "") {
    		//Do nothing, reprompt
    	} else if (command == "history") { //TODO split stringing
    		//TODO print (or clear) history up to 128 (or X)
    		
    		//printing command_list
    		
    		
    	} else {
            continue;
            /*
    		//TODO search for executable; PATH, or ., or /
            int i;
            for (i = 0; i < os_path_list.size(); i++)
            {
                if(command == os_path_list[i].c_str());{
                    
                    int pid = fork();

                    //Child Process (Thread)
                    if (pid == 0){
                        //Execute program
                    }


                }
                //printf("PATH[%2d]: %s\n", i, os_path_list[i].c_str());
            }


    		//Remember to thread this, carefully
            */
    	}
    	
    	printf("Made it through one loop\n"); 
    	
    }

    
    /************************************************************************************
     *   Example code - remove in actual program                                        *
     ************************************************************************************/
    /*
    // Shows how to split a command and prepare for the execv() function
    std::string example_command = "ls -lh";
    splitString(example_command, ' ', command_list);
    vectorOfStringsToArrayOfCharArrays(command_list, &command_list_exec);
    // use `command_list_exec` in the execv() function rather than looping and printing
    i = 0;
    while (command_list_exec[i] != NULL)
    {
        printf("CMD[%2d]: %s\n", i, command_list_exec[i]);
        i++;
    }
    // free memory for `command_list_exec`
    freeArrayOfCharArrays(command_list_exec, command_list.size() + 1);
    printf("------\n");

    // Second example command - reuse the `command_list` and `command_list_exec` variables
    example_command = "echo \"Hello world\" I am alive!";
    splitString(example_command, ' ', command_list);
    vectorOfStringsToArrayOfCharArrays(command_list, &command_list_exec);
    // use `command_list_exec` in the execv() function rather than looping and printing
    i = 0;
    while (command_list_exec[i] != NULL)
    {
        printf("CMD[%2d]: %s\n", i, command_list_exec[i]);
        i++;
    }
    // free memory for `command_list_exec`
    freeArrayOfCharArrays(command_list_exec, command_list.size() + 1);
    printf("------\n");
    */
    /************************************************************************************
     *   End example code                                                               *
     ************************************************************************************/

    return 0;
}

/*
   text: string to split
   d: character delimiter to split `text` on
   result: vector of strings - result will be stored here
*/
void splitString(std::string text, char d, std::vector<std::string>& result)
{
    enum states { NONE, IN_WORD, IN_STRING } state = NONE;

    int i;
    std::string token;
    result.clear();
    for (i = 0; i < text.length(); i++)
    {
        char c = text[i];
        switch (state) {
            case NONE:
                if (c != d)
                {
                    if (c == '\"')
                    {
                        state = IN_STRING;
                        token = "";
                    }
                    else
                    {
                        state = IN_WORD;
                        token = c;
                    }
                }
                break;
            case IN_WORD:
                if (c == d)
                {
                    result.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
            case IN_STRING:
                if (c == '\"')
                {
                    result.push_back(token);
                    state = NONE;
                }
                else
                {
                    token += c;
                }
                break;
        }
    }
    if (state != NONE)
    {
        result.push_back(token);
    }
}

/*
   list: vector of strings to convert to an array of character arrays
   result: pointer to an array of character arrays when the vector of strings is copied to
*/
void vectorOfStringsToArrayOfCharArrays(std::vector<std::string>& list, char ***result)
{
    int i;
    int result_length = list.size() + 1;
    *result = new char*[result_length];
    for (i = 0; i < list.size(); i++)
    {
        (*result)[i] = new char[list[i].length() + 1];
        strcpy((*result)[i], list[i].c_str());
    }
    (*result)[list.size()] = NULL;
}

/*
   array: list of strings (array of character arrays) to be freed
   array_length: number of strings in the list to free
*/
void freeArrayOfCharArrays(char **array, size_t array_length)
{
    int i;
    for (i = 0; i < array_length; i++)
    {
        if (array[i] != NULL)
        {
            delete[] array[i];
        }
    }
    delete[] array;
}

/*
    string: most recent command entered
    string*: list of past 128 commands
*/
void recordCommand(std::string command, std::vector<std::string>& history, int* counter){
	
	strcpy(*(history)[*counter - 1], command.c_str()); 
	
	//TODO in the printHistory, check if null, then loop until
	if(*(counter) == 128){
        *(counter) = 0;
    } 
}
