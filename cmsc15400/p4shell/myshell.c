#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
// Kameel Khabaz 
// CMSC Project 4: Shell 

void myPrint(char *msg)
{
    write(STDOUT_FILENO, msg, strlen(msg));
}

void printError() 
{
    char error_message[30] = "An error has occurred\n";
    myPrint(error_message);
}

void freePointerArray(char **arr, int arrlen) {
    for (int i = 0; i < arrlen; i++) {
        if (arr[i]) {
            free(arr[i]);
        }
    }
    free(arr);
}

int fileExist(char *filename) {
    // if file exist, return 1; else return 0
    FILE *fp = fopen(filename,"r");
    int exist = (fp != NULL);
    if (exist) {
        fclose(fp);
    }
  
    return exist;
}

char *skipSpaces(char *str) {
    // skip preceding spaces in string
    if (str == NULL) {
        return str;
    }
    while ((*str == ' ') || (*str == '\t')) {
        str++;
    }
    return str;
}

int last_instance(char *str, char val) {
    // return pos of last occurance of val in str, and -1 if not in str
    int pos = -1;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == val) {
            pos = i;
        }
    }
    return pos;
}

int contains(char *str, char val) {
    // return pos of 1st occurance of val in str, and -1 if not in str
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == val) {
            return i;
        }
    }
    return -1;
}

int bcontains(char *str, char val) {
    // return boolean for contains (0 if no contain, 1 otherwise)
    return !(contains(str,val) == -1);
}

int dirExists(char *dir_name) {
    // check if directory exists 
    int exist = 1;
    char cwd_buff[1024] = {0};
    char *cwd = getcwd(cwd_buff,1024);
    int ret = chdir(dir_name);
    if (ret) {
        exist = 0;
    }
    chdir(cwd);
    return exist; 
}

int equals(char *str1, char *str2) 
{
    // checks equality of 2 strings
    return !strcmp(str1,str2);
}

FILE *check_batch(int argc, char *argv[], int *batch)
{
    // check if we are running batch, int batch set to true if so, false if not
    FILE *fp;
    // Check for batch file
    if (argc == 1) {
        fp = stdin;
        *batch = 0;
    } else if (bcontains(argv[1],'>')) {
        // no input files
        *batch = 0;
    } else if (argc >= 3 && !bcontains(argv[1],'>') 
                         && !bcontains(argv[2],'>')) {
        // shell has two or more input files 
        printError();
        exit(1);
    } else {
        // argc >= 2 
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            printError();
            exit(1);
        }
        *batch = 1;
    }
    return fp;
}

void remove_nl(char *str) {
    // removes up to 1 newline from a dynamically allocated string
    // by setting a newline character (if it exists) to the null pointer
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {
            str[i] = '\0';
            return;
        }
        i++;
    }
}

int nameContainsInvalidDirectory(char *file_name) 
{
    // check if directory in file name is invalid, return 1 if so
    int last_slash = last_instance(file_name, '/');
    if (last_slash != -1) {
        // file contains directory, check if exist
        char *dir_name = (char *)malloc(last_slash + 1);
        dir_name = strncpy(dir_name, file_name, last_slash);
        dir_name[last_slash] = '\0';
        int dir_not_exist = !dirExists(dir_name);
        free(dir_name);
        if (dir_not_exist) {
            return 1;
        }
    }
    return 0; //either no directory or it is valid
}
        

int checkRedirectionErrors(char **cmd_argv, int cmd_argv_len, int redir_pos, 
                            char *redir_char)        
{
    // check redirection errors and returns 1 if there is error, 0 if not 
    // check advanced redirection
    
    // check if > or >+ occurs more than once
    int counter = 0;
    for (int i = 0; i < cmd_argv_len; i++) {
        if (equals(cmd_argv[i],redir_char)) {  
            counter++;
            if (counter > 1) {
                return 1;
            }
        }
    }
    // return nonzero val for err
    for (int i = 0; i < redir_pos; i++) {
        if (equals(cmd_argv[i],"pwd") || 
            equals(cmd_argv[i],"cd")  ||
            equals(cmd_argv[i],"exit")) {
            // If the command before redirection is a built-in command 
            // (for example, "cd > output"), you should also throw the 
            // error message
            return 1;
        }
    }
    if (cmd_argv_len == redir_pos + 1) {
        // if no output file specified
        return 1;
    }
    if (cmd_argv_len > redir_pos + 2) {
        // more than one output file specified 
        return 1;
    }

    char *file_name = cmd_argv[redir_pos + 1];

    if (nameContainsInvalidDirectory(file_name)) {
        return 1;
    }
    // if redir character is >+ then no error whether or not file already
    // exists 
    if (equals(redir_char, ">+")) {
        return 0;
    }
    // if redir character is > then we care if file exists 
    // output file is at position redir_pos + 1
    // check if output file exist, return 1 (error) if it does not 
    return fileExist(file_name);
}

void handleRedirection(char **cmd_argv, int cmd_argv_len, int redir_pos) 
{
    // handles redirections in the child process
    int output_fd = open(cmd_argv[redir_pos + 1], O_WRONLY | O_CREAT, S_IRWXU);
    dup2(output_fd, STDOUT_FILENO);
    close(output_fd);
}

char *make_rand_str(int length) {
    // make random string
    char chars[] = "abcdefghijklmnopqrstuvwxyz0123456789";
    int setlen = strlen(chars);
    char *str = (char *)malloc(sizeof(char) * length);
    for (int i = 0; i < length - 1; i++) {
        str[i] = chars[rand() % setlen];
    }
    str[length - 1] = '\0';
    return str;
}
char *prepareAdvancedRedirection(char **cmd_argv, int cmd_argv_len, 
                                                     int redir_pos)
{
    // check if file already exists. If so, copy the file to a temp file
    // and return a pointer to the name. If not, return NULL
    
    if (!fileExist(cmd_argv[redir_pos + 1])) {
        // file does not exist, standard redirection
        return NULL;
    } 
    FILE *fp = fopen(cmd_argv[redir_pos + 1], "r");
    // create string for temp file (ensure that file doesn't exist already)
    char *rand_fname = make_rand_str(10);
    while (fileExist(rand_fname)) {
        free(rand_fname);
        rand_fname = make_rand_str(10);
    }
    // create temp file
    FILE *temp = fopen(rand_fname,"w"); 
    char chr; 
    while ((chr = fgetc(fp)) != EOF) {
        fputc(chr, temp);
    }

    fclose(fp);
    fclose(temp);
    return rand_fname; // this temp file must be written after command runs 
}

void copy_file(char *file_source_name, char *file_write_name) {
    // copy the source file and append its contents to the destination file 
    FILE *fp_source = fopen(file_source_name, "r");
    FILE *fp_write = fopen(file_write_name, "a+");
    if (fp_source == NULL || fp_write == NULL) {
        printError();
        exit(1);
    }
    char chr;
    while ((chr = fgetc(fp_source)) != EOF) {
        fputc(chr, fp_write);
    }
    fclose(fp_source);
    fclose(fp_write);
}

void runCommand(char **cmd_argv, int cmd_argv_len)
{
    // run a command
    // check for exit
    int has_redir = 0;
    int has_adv_redir = 0;
    int redir_pos = 0; // will be nonzero if there is redirection
    for (int i = 0; i < cmd_argv_len; i++) {
        if ((has_redir = equals(cmd_argv[i],">"))) {
            redir_pos = i;
            if (checkRedirectionErrors(cmd_argv, cmd_argv_len, 
                                                redir_pos, ">")) {
                printError();
                return;
            } else {
                break;
            }
        } else if ((has_adv_redir = equals(cmd_argv[i],">+"))) {
            redir_pos = i;
            if (checkRedirectionErrors(cmd_argv, cmd_argv_len, 
                                               redir_pos, ">+")) {
                printError();
                return;
            } else {
                break;
            }
        }
    }
    // check built-in commands     
    if (equals(cmd_argv[0], "exit")) {
        if (cmd_argv_len > 1) {
            // invalid format;
            printError();
            return;
        }
        exit(0);
    } else if (equals(cmd_argv[0], "pwd")) {
        if (cmd_argv_len > 1) {
            // invalid format;
            printError();
            return;
        }
        char cwd_buff[1024] = {0};
        char *cwd = getcwd(cwd_buff,1024);
        myPrint(cwd);
        myPrint("\n");
        return;
    } else if (!strcmp(cmd_argv[0], "cd")) {    
        if (cmd_argv_len == 1) {
            // command is only cd
            chdir(getenv("HOME"));
        } else if (cmd_argv_len > 2) {
            // invalid cd format
            printError();
            return;
        } else {
            int ret = chdir(cmd_argv[1]);
            if (ret) {
                printError();
            }
        }
        return;
    }
    char *file_write_name = NULL;
    char *temp_name = NULL;
    if (has_adv_redir) {
        // must create temporary file if the 
        // file exists for advanced redirection
        temp_name = prepareAdvancedRedirection(cmd_argv, cmd_argv_len, 
                                                                    redir_pos); 
        // temp_name is NULL if file doesn't exist
        file_write_name = strdup(cmd_argv[redir_pos + 1]);
        // must delete the original file so that child can write to it
        remove(cmd_argv[redir_pos + 1]);
    }
    // run other commands
    pid_t ret = fork();
    if (ret == 0) {
        // child proccess, run command 
        if (has_redir) {
            // must handle redirection
            handleRedirection(cmd_argv, cmd_argv_len, redir_pos);
            // must truncate cmd_argv array as no longer need dest file
            cmd_argv[redir_pos] = NULL;
        } else if (has_adv_redir) {
            // must handle advanced redirection
            handleRedirection(cmd_argv, cmd_argv_len, redir_pos);
            // must truncate cmd_argv array as no longer need dest file
            cmd_argv[redir_pos] = NULL;
        }
        if (execvp(cmd_argv[0], cmd_argv) < 0) {
            printError();
            exit(1);
        }
    } else { 
        // parent process
        wait(NULL);
        if (has_adv_redir != 0 && file_write_name != NULL 
                                    && temp_name != NULL) {
            // has_adv_redir is NULL if file did not exist, so no need append
            // else, we need to append the temp stuff to the end of the file
            copy_file(temp_name, file_write_name);
            // delete temp file
            remove(temp_name);
            free(file_write_name);
            free(temp_name);
        }
    }
}

char **parseArguments(char  *cmd, int *cmd_argv_len) {
    // copies argument, parse the command for argv
    int max_size = 512; // maximum of 512 characters on a line
    char **cmd_argv = (char **)malloc(sizeof(char *) * max_size);
    char *command = strdup(cmd);
    char *cmd_delim = " \t";
    char *cmd_token; // store each arg in command
    char *ptr2;
    cmd_token = strtok_r(command, cmd_delim, &ptr2);
    *cmd_argv_len = 0;
    while (cmd_token != NULL &&  !equals(cmd_token,"\n")) { 
        remove_nl(cmd_token);
        // check if cmd_token contains > or >+ but has other stuff too,
        // in which case we need to separate them into 2 strings
        int pos_r = contains(cmd_token,'>');
        if (pos_r >= 0 && !(equals(cmd_token,">") || equals(cmd_token,">+"))) {
            // string contains > but not just >
            // must separate into different commands
            int token_len = strlen(cmd_token);
            cmd_token[pos_r] = '\0'; // remove that from string
            if (pos_r > 0) {
                // there is precending string
                cmd_argv[(*cmd_argv_len)++] = strdup(cmd_token);
             }
             // add > to argv
             int extra_len = 0;
             if (cmd_token[pos_r + 1] == '+') {
                // checking advanced redirection
                extra_len = 1;
                cmd_argv[(*cmd_argv_len)++] = strdup(">+");
             } else {
                cmd_argv[(*cmd_argv_len)++] = strdup(">");
             }
             int start_next_str = pos_r + extra_len + 1;
             if (token_len > start_next_str) {
                // there is succeeding string
                cmd_argv[(*cmd_argv_len)++] = strdup(cmd_token + 
                                                               start_next_str);
             }
        }  else {
           cmd_argv[(*cmd_argv_len)++] = strdup(cmd_token);
        }
        cmd_token = strtok_r(NULL, cmd_delim, &ptr2);
    }
    free(command);            
    return cmd_argv;
}

int main(int argc, char *argv[]) 
{
    char *pinput;
    int batch;
    FILE *fp = check_batch(argc, argv, &batch);

    while (1) {
        if (!batch) { 
            myPrint("myshell> ");
        }
        size_t len = 0;
        size_t nchars = getline(&pinput, &len, fp);
        if (nchars == -1 || !pinput) {
            // invalid read (like end of file)
            exit(0);
        }
        char *pinput_skip_check = skipSpaces(pinput);
        if (equals(pinput_skip_check,"\n")) {
            continue; // skip empty line 
        }
        myPrint(pinput);
        if (nchars > 513) {
            // if line is too long (512 chars + '\n' is max)
            printError();
            continue;
        }
        // begin parsing input 
        char *delim = ";";
        char *command; // will store each portion
        char *ptr1;
        command = strtok_r(pinput, delim, &ptr1);
        while(command != NULL && !equals(command,"\n")) {
            // now we have a command
            int cmd_argv_len;
            char **cmd_argv = parseArguments(command, &cmd_argv_len);
            // run the command
            if (cmd_argv_len > 0) {
                // actually have a command
                runCommand(cmd_argv, cmd_argv_len); 
            }
            command = strtok_r(NULL, delim, &ptr1);
            command = skipSpaces(command);
        }
    }
    fclose(fp);
}
