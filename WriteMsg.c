/*  sendto:tuf18653@temple.edu 
*/
//  FILE:       WriteMsg.c (hidden message writer)
//  AUTHOR:     Dau Ting Lam
//  DATE:       10/10/2014
//  FOR:        Assignment 6 Steganography
//  CLASS:      CIS 2107, Section 2
//  INSTRUCTOR: John Fiore
//  TA:         Chen Shen
//
//  notes: Tested working on Mac OS X's gcc Terminal. Some bugs opening image file, but opening with emacs, the data looks correct. 
//  Except for length, Unable to correctly write hidden message.
/***********************************************************/
#include "stego.c"

int main(int argc, char **argv) {
  int status;

  if ((status=usage(argc, argv, 3, 2)) == FILE_OPEN_SUCCESS)  { // checks if correct usage
    char* msg = argv[1];  // hidden message
    int length, i;

    printf("You entered\n");
    printf("Secret msg: %s\n", msg); 
    printf("Name of image file: %s\n", argv[2]); 
    printf("Name of output file: %s\n", argv[3]);  

    FILE *infp = fopen(argv[2], "r");
    FILE *outfp = fopen(argv[3], "w"); 

    status = copy_header(infp, outfp);
    length = string_length(msg);  // calculate length

    write_hidden_byte(infp, outfp, length);

    for (i = 0; i < length; ++i) { // attempts to write hidden message to output file
      write_hidden_byte(infp, outfp, msg[i]);
    }
    printf("\nCompleted\n"); 
  }

  return EXIT_SUCCESS;
}