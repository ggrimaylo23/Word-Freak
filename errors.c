#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "errors.h"

//error check when read system call fails
//takes in the error num in order to know what to write to stderr
void read_error_handle(int error_num)
{
    int bytesWritten;

    if (error_num == EBADF)
    {
        char error[] = "The file descriptor is not valid or isn't open for reading\n";
        bytesWritten = write(STDERR_FILENO, error, strlen(error));
        //write the error to stderr, if that fails use perror to write it to stderr

        if (bytesWritten == -1)
            perror("The file descriptor is not valid or isn't open for reading\n");
    }
    else if (error_num == EINTR)
    {
        char error[] = "The system call was interrupted by a signal before any data was read\n";
        bytesWritten = write(STDERR_FILENO, error, strlen(error));

        if (bytesWritten == -1)
            perror("The system call was interrupted by a signal before any data was read\n");
    }
    else           //if the value of error_num was none of these, then some other error occurred
    {
        char error[] = "Some other error occurred while reading\n";
        bytesWritten = write(STDERR_FILENO, error, strlen(error));

        if (bytesWritten == -1)
            perror("Some other error occurred while reading\n");
    }
}

//error handles the write system call
//takes in the error number in order to know what to write to stderr
void close_error_handle(int error_num)
{
    int bytesWritten;
    if (error_num == EBADF)
    {
        char error[] = "The file descriptor isn't valid for close\n";
        bytesWritten = write(STDERR_FILENO, error, strlen(error));

        if (bytesWritten == -1)
            perror("The file descriptor isn't valid for close\n");       
    }
    else if (error_num == EINTR)
    {
        char error[] = "The system call close was interrupted by a signal\n";
        bytesWritten = write(STDERR_FILENO, error, strlen(error));

        if (bytesWritten == -1)
            perror("The system call was interrupted by a signal\n");
    }
    else
    {
        char error[] = "Some other error occurred while closing the file descriptor\n";
        bytesWritten = write(STDERR_FILENO, error, strlen(error));

        if (bytesWritten == -1)
            perror("Some other error occurred while closing the file descriptor\n");
    }
}

//error handles the open system call
//takes in the error number in order to know what to write to stderr
void open_error_handle(int error_num)
{
    int bytesWritten;
    if (error_num == EACCES)
    {
        char error[] = "The access to the file is not allowed\n";
        bytesWritten = write(STDERR_FILENO, error, strlen(error));

        if (bytesWritten == -1)
            perror("The access to the file is not allowed\n");
    }
    else if (error_num == ENOENT)
    {
        char error[] = "The file doesn't exist\n";
        bytesWritten = write(STDERR_FILENO, error, strlen(error));

        if (bytesWritten == -1)
            perror("The file doesn't exist\n");
    }
    else if (error_num == EOVERFLOW)
    {
        char error[] = "The file is too large to be opened\n";
        bytesWritten = write(STDERR_FILENO, error, strlen(error));

        if (bytesWritten == -1)
            perror("The file is too large to be opened\n");
    }
    else
    {
        char error[] = "Some other error occurred when opening the file\n";
        bytesWritten = write(STDERR_FILENO, error, strlen(error));

        if (bytesWritten == -1)
            perror("Some other error occured when opening the file\n");
    }
}

//error handles the write system call
//takes in the error number in order to know what to write to stderr
void write_error_handle(int error_num)
{
    int bytesWritten;
    if (error_num == EBADF)
    {
        char error[] = "The file descriptor isn't valid for write\n";
        bytesWritten = write(STDERR_FILENO, error, strlen(error));

        if (bytesWritten == -1)
            perror("The file descriptor isn't valid for write\n");
    }
    else if (error_num == EFBIG)
    {
        char error[] = "Attempting to write from a file that exceeds the maximum file size\n";
        bytesWritten = write(STDERR_FILENO, error, strlen(error));

        if (bytesWritten == -1)
            perror("Attempting to write from a file that exceeds the maximum file size\n");
    }
    else if (error_num == EINTR)
    {
        char error[] = "Write was interrupted by a signal\n";
        bytesWritten = write(STDERR_FILENO, error, strlen(error));

        if (bytesWritten == -1)
            perror("Write was interrupted by a signal\n");
    }
    else
    {
        char error[] = "Some other error occurred for write\n";
        bytesWritten = write(STDERR_FILENO, error, strlen(error));

        if (bytesWritten == -1)
            perror("Some other error occurred for write\n");
    }   
}
