#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include "errors.h"
#include "tree.h"

int main(int argc, char* argv[], char* envp[])
{
    extern int errno;

    int errnum = errno;                      //used to pass into error-handling functions
    struct Node* trees[26];                  //array of 26 BSTs
    for (int i = 0; i < 26; i++)             //for each BST, set them initialially to NULL
    {
        trees[i] = NULL;
    }
    
    //read from stdin first
    char input[1024]; 
    char temp1[1024] = "";
    int bytesRead;
    while ((bytesRead = read(STDIN_FILENO, input, 1024)) != 0)     //read until end of file is reached (ctrl d) 
    {
        if (bytesRead == -1)       //if read failed, error check it, stop reading
        {
            errnum = errno;
            read_error_handle(errnum);
            break;
        }

        input[bytesRead] = '\0';               //null terminate what we just read in
        strncat(temp1, input, bytesRead);      //add what we read in to a temp array
        convert_buffer_to_lower(temp1);        //convert it to lowercase
        memset(input, 0, sizeof(input));       //reset contents of input in order to use it for the next loop iteration
    
        if (isalpha(temp1[bytesRead - 1]))     //if the last character in the input was a letter (if word gets cut off)
        {
            //read from the file, see if that character is also a letter, if it is add it
            char temp_str[bytesRead + 46];
            char test_str[46];                     //longest word in english language is 45 letters so maximum num chars cut off is 45
            strncpy(temp_str, temp1, bytesRead);   //copy the input read in to temp string

            int count = 0;
            int read_status = read(STDIN_FILENO, test_str, 1);      //get next character from input file, convert it to lowercase
            if (read_status == -1)                 //if read failed error handle it
            {
                errnum = errno;
                read_error_handle(errnum);
            }
            
            convert_buffer_to_lower(test_str);

            while ((isalpha(test_str[0])) && (read_status != 0) && (read_status != -1))    //while the character we got is a letter 
                                                                                        //we haven't reached EOF, and read didn't fail, keep reading
            {
                count++;
                strncat(temp_str, test_str, 1);    //concatenate the character read in to temp_str

                read_status = read(STDIN_FILENO, test_str, 1);     //get the next character
                if (read_status == -1)          //if read failed, error handle and stop reading
                {
                    errnum = errno;
                    read_error_handle(errnum);
                    break;
                }

                convert_buffer_to_lower(test_str);       //convert the character we get to lowercase

            }
            temp_str[bytesRead + count] = '\0';                 //null terminate the string after reading in the characters
            add_words_in_buffer_to_tree(trees, temp_str);    //add the cut off word to the tree
            
            memset(temp_str, 0, sizeof(temp_str));          //reset contents in order to use them for the next loop iteration
            memset(test_str, 0, sizeof(test_str)); 

            if ((read_status == 0) || (read_status == -1))     //if we reached the end of file or read failed, stop reading
                break;  
        }
        else
        {                                                     //if the last character we read isn't a letter, no word was cut off, so add what
                                                                    //we read to the tree and null terminate it
            temp1[bytesRead] = '\0';
            add_words_in_buffer_to_tree(trees, temp1);
        }
        
        memset(temp1, 0, sizeof(temp1));                  //reset for use in next loop iteration
    }

    int close_status = close(STDIN_FILENO);             //close stdin, if it failed error handle it
    if (close_status == -1)
    {
        errnum = errno;
        close_error_handle(errnum);
    }

     if (strncmp(envp[0], "WORD_FREAK=", 10) == 0)         //if the user specified the environment variable WORD_FREAK, determine the file name
                                                                //they requested and read from it
    {
        char file_name[1024];
        int i = 11;
        int j = 0;
        char ch = envp[0][i];                             //start reading from the 11th position (after =) of the environment variable string in order
                                                                //to get the file name
        while (!isspace(ch))                              //while the character we're reading is still in the environment variable string. keep reading
        {
            file_name[j] = ch;
            j++;
            i++;
            ch = envp[0][i];
        }

        file_name[i - 1] = '\0';                          //null terminate the file name we just got

        int fenv = open(file_name, O_RDONLY);             //open the file name, if it failed error check it
        if (fenv == -1)
        {
            errnum = errno;
            open_error_handle(errnum);
        }

        char buff_env[1024];
        int num_bytes_read;
        while (((num_bytes_read = read(fenv, buff_env, 1024)) != 0) && (fenv != -1))
        {
            //while we haven't reached the end of the file and open didn't fail, keep reading from the file
            if (num_bytes_read == -1)       //if read fails, error check it and stop reading
            {
                errnum = errno;
                read_error_handle(errnum);
                break;
            }
            convert_buffer_to_lower(buff_env);      //convert what we read from the file to lowercase

            if (isalpha(buff_env[num_bytes_read - 1]))     //if the last character we read was a letter, a word was cut off, use the same algorithm as stdin
            {
                char temp[num_bytes_read + 46];
                char test[46];
                char ch;

                strncpy(temp, buff_env, num_bytes_read);
            
                int i = 0;
                int read_status = read(fenv, test, 1);

                if (read_status == -1)
                {
                    errnum = errno;
                    read_error_handle(errnum);
                    break;
                }
                convert_buffer_to_lower(test);
                while((isalpha(test[0])) && (read_status != 0) && (read_status != -1))
                {
                    i++;
                    strncat(temp, test, 1);
                    read_status = read(fenv, test, 1);
                    if (read_status == -1)
                    {
                        errnum = errno;
                        read_error_handle(errnum);
                    }
                    convert_buffer_to_lower(test);
                }
                temp[i + num_bytes_read] = '\0';
                add_words_in_buffer_to_tree(trees, temp);

                memset(temp, 0, sizeof(temp));
                memset(test, 0, sizeof(test));

            }
            else
            {
                buff_env[num_bytes_read] = '\0';
                add_words_in_buffer_to_tree(trees, buff_env);
            }

        }
        int close_status2 = close(fenv);      //close the file, if it fails error check it
        if (close_status2 == -1)
        {
            errnum = errno;
            close_error_handle(errnum);
        }
    } 

    int k = 1;
    while (argv[k] != NULL)       //read from arguments specified, start from argv[1]      
    {
        int fd = open(argv[k], O_RDONLY);       //open the file name specified, if it fails error check it and move on to the next file
        if (fd == -1) 
        {
            errnum = errno;
            open_error_handle(errnum);
        }

        char buffer[1024];
        int numBytes;

        while (((numBytes = read(fd, buffer, 1024)) != 0) && (fd != -1))   //while we haven't reached the end of the file and open didn't fail, read from the file
        {
            if (numBytes == -1)                //if read failed, error handle it and stop reading
            {
                errnum = errno;
                read_error_handle(errnum);
                break;
            }

            if (numBytes != -1)       //avoiding seg fault when buffer is uninitialized if read failed
                convert_buffer_to_lower(buffer);       //convert what we read from the file to lowercase

            if (isalpha(buffer[numBytes - 1]) && numBytes != -1)    //if the last character in what we read was a letter, a word was cut off, use same algorithm
            {
                char temp[numBytes + 46];
                char test[46];
                char ch;

                strncpy(temp, buffer, numBytes);
            
                int i = 0;
                int status = read(fd, test, 1);
                if (status == -1)
                {
                    errnum = errno;
                    read_error_handle(errnum);
                    break;
                }

                convert_buffer_to_lower(test);
                while ((isalpha(test[0])) && (status != 0)  && (status != -1))
                {
                    i++;

                    strncat(temp, test, 1);
                    status = read(fd, test, 1);
                    if (status == -1)
                    {
                        errnum = errno;
                        read_error_handle(errnum);
                        break;
                    }
                    convert_buffer_to_lower(test);
                }
                if (status == -1)
                    break;
                temp[i + numBytes] = '\0';
                add_words_in_buffer_to_tree(trees, temp);

                memset(temp, 0, sizeof(temp));
                memset(test, 0, sizeof(test));
            }
            else
            {
                if (numBytes != -1)
                {
                    buffer[numBytes] = '\0';
                    add_words_in_buffer_to_tree(trees, buffer);
                }
            }
        }
        memset(buffer, 0, sizeof(buffer));
        k++;

        int close_status3 = close(fd);        //close the file, if it fails error check it    
        if (close_status3 == -1)
        {
            errnum = errno;
            close_error_handle(errnum);
        }
    } 
    
    int word_length = 0;
    int digits = 0;
    for (int i = 0; i < 26; i++)               //determine the longest word and the most digits in the counter in all 26 trees
    {
        longest_word(trees[i], &word_length);
        most_digits(trees[i], &digits);
    }

    int fout = open("output.txt", O_WRONLY | O_TRUNC | O_CREAT, 0644);      //open output.txt for writing, create it if it doesn't exist and reset
                                                                                //it if it does
    if (fout == -1)                         //if open failed, error handle it
    {
        errnum = errno;
        open_error_handle(errnum);
    }
    else                                   //if it didn't we can write to it
    {
        for (int i = 0; i < 26; i++)
            write_tree_to_file(trees[i], fout, word_length, digits);        //write each BST's contents to output.txt
    }

    for (int i = 0; i < 26; i++)          //free the memory of all 26 trees
        free_tree(trees[i]);
        
    int status2 = close(fout);            //close output.txt, if it failed error handle
    if (status2 == -1)
    {
        errnum = errno;
        close_error_handle(errnum);
    }

    return 0;
}
