#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>
#include "errors.h"

//creates a new node in the tree with a name
//takes in the word_name in order to set it
//returns the pointer to the new node created
struct Node* new_node(char* word_name)
{
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));       //allocate space for the node

    if (temp == NULL)              //if malloc failed, write the error to stderr and exit, if write failed use perror
    {
        char error[] = "Malloc faied in new_node";
        int bytesWritten = write(STDERR_FILENO, error, strlen(error));
        if (bytesWritten == -1)
            perror("Malloc failed in new_node");
        exit(1);
    }

    strcpy(temp->word, word_name);              //put the word name into the node
    temp->left = NULL;                          //set the left an right children to NULL                     
    temp->right = NULL;
    temp->counter = 1;                          //initial counter of the word is 1
    return temp;                                //return the pointer to the node
}


//calculates the digits that a node's counter has
//used in a for loop to calculate the most digits in all of the 26 trees
//takes in the pointer to the node and the pointer to the amount of digits that keeps track of the most amount of digits
void most_digits(struct Node* tree, int* digits)
{
    int num_digits = 0;
    int num = 0;
    if (tree != NULL)           //traverse the tree while not null, calculate the digits for each node
    {
        most_digits(tree->left, digits);

        num = tree->counter;
        //calculate number of digits in the counter
        while (num != 0)        //while the number/10 is not 0, keep dividing by 10 and incrementing to find the number of digits in counter
        {
            num = num / 10;
            num_digits++;
        }

        if (num_digits > *digits)  //if the number of digits calculated is more than the most amount of digits currently stored in digits
                                        //pointer, set digits pointer to value of digits calculated
            *digits = num_digits;
        
        most_digits(tree->right, digits);
    }
}

//calculates the longest word in each node, used in for loop to calculate longest word for each tree
//takes in pointer to the node and the pointer to longest length that is updated
void longest_word(struct Node* tree, int* length_word)       
{
    int word_length;
    if (tree != NULL)       //traverse the tree while not null, calculate longest word for each node
    {
        longest_word(tree->left, length_word);

        word_length = get_word_length(tree);     //get the word length for the node's word
        if (word_length > *length_word)          //if the word length calculate is more than the longest word calculate so far, 
                                                        //set value of longest word to length of word calculated
            *length_word = word_length;
        
        longest_word(tree->right, length_word);
    }
}

//returns the length of the node's word, which is null terminated
//takes in the pointer to the node in order calculate its length
int get_word_length(struct Node* tree)
{
    return strlen(tree->word);
}

//writes the words of the tree to output.txt
//takes in the pointer to the tree to access its words, the file descriptor in order to write to output.txt, the length of the longest word
//and the most digits in the counter in orde to format output.txt
void write_tree_to_file(struct Node* tree, int fout, int longest_word, int most_digits)
{
    int writtenBytes;
    int errnum;                     //used in case a system calls fails in this function
                                   
    int num_spaces = longest_word;    //maximum number of spaces written will be the length of the longest word in order to format output.txt
    int write_spaces;

    char ch = ' ';
    char spaces[45];                //the most number of spaces can be 45 since that is the length of the longest english word
    int num_digits = 0;             //number of digits calculated in node's counter

    if (tree != NULL)              //traverse the tree while node isn't null
    {
        write_tree_to_file(tree->left, fout, longest_word, most_digits);      //call the function on the left node first

        writtenBytes = write(fout, tree->word, strlen(tree->word));     //write the node's word to output.txt
        if (writtenBytes == -1)        //if write failed, error handle it
        {
            errnum = errno;
            write_error_handle(errnum);
        }

        write_spaces = num_spaces - strlen(tree->word);      //the number of spaces written after each word will be the most amount of spaces
                                                                //calculated in all trees minus the length of the word so output.txt gets formatted
        for (int i = 0; i < write_spaces; i++)      //put these spaces into a string in order to use write
            spaces[i] = ch;

        writtenBytes = write(fout, spaces, write_spaces);    //now write spaces to output.txt
        if (writtenBytes == -1)                     //if write failed error handle it
        {
            errnum = errno;
            write_error_handle(errnum);
        }

        writtenBytes = write(fout, " :", 2);          //write the colon to output.txt
        if (writtenBytes == -1)                       //error handle again if it failed
        {
            errnum = errno;
            write_error_handle(errnum);
        }

        int num = tree->counter;
        do
        {
            num_digits++;
            num = num / 10;
        } while (num != 0);               //calculate the number of digits in the counter
        
        //subtract the number of digits from the most number of digits + 1 to determine the number of spaces to write after the colon
        int spaces_for_counter = (most_digits + 1) - num_digits;

        char spaces_before_counter[spaces_for_counter];

        for (int i = 0; i < spaces_for_counter; i++)        //put these spaces into another string
            spaces_before_counter[i] = ' ';

        spaces_before_counter[spaces_for_counter] = '\0';   //null terminate it

        char str[num_digits];
        char str2[spaces_for_counter];

        int align = num_digits;                         //use this in sprintf to right align the string

        sprintf(str2, "%s", spaces_before_counter);     //put these spaces into another string using sprintf
        sprintf(str, "%*d", align, tree->counter);      //put the counter into another string and right align it using sprintf

        writtenBytes = write(fout, str2, strlen(str2));     //write the spaces to output.txt, error handle if failed
        if (writtenBytes == -1)
        {
            errnum = errno;
            write_error_handle(errnum);
        }
        writtenBytes = write(fout, str, strlen(str));   //write the counter to output.txt, error handle if it failed
        if (writtenBytes == -1)
        {
            errnum = errno;
            write_error_handle(errnum);
        }
        writtenBytes = write(fout, " \n", 2);        //write a new line after, error handle if failed
        if (writtenBytes == -1)
        {
            errnum = errno;
            write_error_handle(errnum);
        }

        write_tree_to_file(tree->right, fout, longest_word, most_digits);
    }
}

//takes in a word and inserts it into appropriate place in the tree
//takes in a pointer to the tree and the name of the word to insert
//returns a pointer to the updated tree
struct Node* insert_word(struct Node* tree, char* word_name)
{
     if (tree == NULL)     //if the tree is empty, make the root of the tree have that word
        return new_node(word_name);

     if (strcmp(word_name, tree->word) < 0)     //if the word passed in is less alphabetically than the word of the current node, 
                                                        //add the word passed in to the left of the current node
        tree->left = insert_word(tree->left, word_name);
    else if (strcmp(word_name, tree->word) > 0)  //if the word passed in is bigger alphabetically, add it to the right
        tree->right = insert_word(tree->right, word_name);
    else if (strcmp(word_name, tree->word) == 0)   //if the word passed in is the same as the word in the node, increase the word's counter
        tree->counter++;

    return tree;
}

//convert the buffer read in to all lowercase letters, takes in the buffer to do this
void convert_buffer_to_lower(char* buffer)
{
    int i = 0;
    char ch = buffer[0];
    while (ch != '\0')      //while we haven't reached the end of the string, convert each letter to lowercase
    {
        buffer[i] = tolower(buffer[i]);
        i++;
        ch = buffer[i];
    }
}

//adds the words in buffer read in to the corresponding tree
//takes in the buffer and the array of 26 trees in order to do this
void add_words_in_buffer_to_tree(struct Node* trees[26], char* buffer)
{
    int i = 0;
    int j = 0;
    char word[46];       //largest word can be 45 characters since that's the biggest word in english
    char ch;
    //iterate while character is not NULL
    while ((ch = buffer[i]) != '\0')
    {
        j = 0;
        //if the character in buffer is a letter, add it to the word
        if (isalpha(ch))
        {
            while (isalpha(ch))
            {
                word[j] = ch;
                i++;
                j++;
                ch = buffer[i];
            }
            word[j] = '\0';           //null terminate the word
            //add word to tree
            add_word_to_tree(trees, word);
            memset(word, 0, sizeof(word));   //reset the contents of word so it can be used for next loop iteration   
        }
        else
        {           
            i++;        //if the character isn't a letter, iterate it to get the next character in buffer
        }
    }
}

//"hash function", takes in the array of trees and the word in order to add it to the appropriate tree
void add_word_to_tree(struct Node* trees[26], char word[45])
{
    char ch = word[0];      //get the first letter of the word
    //determine which tree to put the word based on the first letter, "hashing"

    if (ch == 'a')
        trees[0] = insert_word(trees[0], word);
    else if (ch == 'b')
        trees[1] = insert_word(trees[1], word);
    else if (ch == 'c')
        trees[2] = insert_word(trees[2], word);
    else if (ch == 'd')
        trees[3] = insert_word(trees[3], word);
    else if (ch == 'e')
        trees[4] = insert_word(trees[4], word);
    else if (ch == 'f')
        trees[5] = insert_word(trees[5], word);
    else if (ch == 'g')
        trees[6] = insert_word(trees[6], word);
    else if (ch == 'h')
        trees[7] = insert_word(trees[7], word);
    else if (ch == 'i')
        trees[8] = insert_word(trees[8], word);
    else if (ch == 'j')
        trees[9] = insert_word(trees[9], word);
    else if (ch == 'k')
        trees[10] = insert_word(trees[10], word);
    else if (ch == 'l')
        trees[11] = insert_word(trees[11], word);
    else if (ch == 'm')
        trees[12] = insert_word(trees[12], word);
    else if (ch == 'n')
        trees[13] = insert_word(trees[13], word);
    else if (ch == 'o')
        trees[14] = insert_word(trees[14], word);
    else if (ch == 'p')
        trees[15] = insert_word(trees[15], word);
    else if (ch == 'q')
        trees[16] = insert_word(trees[16], word);
    else if (ch == 'r')
        trees[17] = insert_word(trees[17], word);
    else if (ch == 's')
        trees[18] = insert_word(trees[18], word);
    else if (ch == 't')
         trees[19] = insert_word(trees[19], word);
    else if (ch == 'u')
        trees[20] = insert_word(trees[20], word);
    else if (ch == 'v')
        trees[21] = insert_word(trees[21], word);
    else if (ch == 'w')
        trees[22] = insert_word(trees[22], word);
    else if (ch == 'x')
        trees[23] = insert_word(trees[23], word);
    else if (ch == 'y')
        trees[24] = insert_word(trees[24], word);
    else if (ch == 'z')
        trees[25] = insert_word(trees[25], word);
}

//frees the tree
//takes in the pointer to the tree
void free_tree(struct Node* tree)
{
    if (tree == NULL)     //if the tree is empty, nothing to free
        return;

    //traverse the tree using postorder, free the left and the right before the root
    free_tree(tree->left);   
    free_tree(tree->right);
    free(tree);
}
