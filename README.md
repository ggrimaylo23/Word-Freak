# Word-Freak
The program takes some files as input, counts how many times each word occurs across them all (considering all letters to be lower case), and writes those words and associated counts to an output file in alphabetical order. The words from all the input files will be counted together. The program can read input from standard input (words entered directly into the terminal or a file piped into the standard input using the bash's operator pipe), argv, and a file given as the environment variable (the environment variable is "WORD_FREAK". 
Example inputs: "cat example.txt | ./wordfreak", "./wordfreak bla bla bla bla" (press ctrl + d at the end), "./wordfreak example.txt example2.txt", "WORD_FREAK=example.txt ./wordfreak", "cat example.txt | WORD_FREAK=example2.txt ./wordfreak example3.txt example4.txt".
Words can only have letters in them. Example: "hel5oo" becomes "hel" and "oo". 
This program uses a binary search tree for each lettter a-z and stores each word and its respective count. 
The words are written to an output file (output.txt) alphabetically.
