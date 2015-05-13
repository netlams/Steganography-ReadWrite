/* 	sendto:tuf18653@temple.edu 
*/
//  FILE:	  	stego.h (declarations and constants)
//  AUTHOR:	  	Dau Ting Lam
//	DATE:	  	10/10/2014
//  FOR:	  	Assignment 6 Steganography
// 	CLASS:    	CIS 2107, Section 2
//	INSTRUCTOR: John Fiore
//  TA:       	Chen Shen
//
//  notes: Tested working on Mac OS X's gcc Terminal
/***********************************************************/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define FILE_OPEN_SUCCESS 1
#define FILE_OPEN_FAIL 0
#define PARSE_HEADER_SUCCESS 1
#define PARSE_HEADER_FAIL 0
#define EXIT_SUCCESS 0

#define CORRECT_READ_ARG 2
#define CORRECT_WRITE_ARG 4

#define STATE_HEADER 1
#define STATE_INVALID 2
#define STATE_PXDATA 3

/* Function to determine if arguments match requirements. */
int usage(int, char**, int, int);
/* Function to parse header, returns 1 for success and 0 for fail */
int parse_header(FILE *);
/* Function to copy header of input to output, returns 1 for success and 0 for fail */
int copy_header(FILE *, FILE *);
/* Function to write hidden byte (from single char) on output file */
void write_hidden_byte(FILE*, FILE*, char);
/* Function to read hidden message from file, returns string of char */
char* read_hidden_msg(FILE *, int);
/* Function to read length of hidden message, returns int of length */
unsigned int read_length(FILE *);
/* Function to read 8-bit (byte), returns single char */
unsigned char read_byte(FILE *);

/* custom functions */
int power(int, unsigned int);
int string_length (char* s);