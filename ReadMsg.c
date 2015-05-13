/*  sendto:tuf18653@temple.edu 
*/
//  FILE:       ReadMsg.c (hidden message reader)
//  AUTHOR:     Dau Ting Lam
//  DATE:       10/10/2014
//  FOR:        Assignment 6 Steganography
//  CLASS:      CIS 2107, Section 2
//  INSTRUCTOR: John Fiore
//  TA:         Chen Shen
//
//  notes: Tested working on Mac OS X's gcc Terminal
/***********************************************************/

#include "stego.c"


int main(int argc, char **argv) {
  unsigned int status = 0;
  int length = 0;

  if ((status=usage(argc, argv, 2, 1)) == FILE_OPEN_SUCCESS) { // if correct arguments, i.e, having a input file

  FILE *fp = fopen(argv[1], "r");

  if ((status=parse_header(fp)) == PARSE_HEADER_SUCCESS) { 
      length = read_length(fp);
      printf("length: %d\n", length);
      char * msg;
      msg = read_hidden_msg(fp, length);
      printf("%s\n", msg);      // ouput hidden messaage
  } 

  fclose(fp);
  }

  return EXIT_SUCCESS;
}