/* 	sendto:tuf18653@temple.edu 
*/
//  FILE:	  	stego.c (implementations, no main() function)
//  AUTHOR:	  	Dau Ting Lam
//	DATE:	  	10/10/2014
//  FOR:	  	Assignment 6 Steganography
// 	CLASS:    	CIS 2107, Section 2
//	INSTRUCTOR: John Fiore
//  TA:       	Chen Shen
//
//  notes: Tested working on Mac OS X's gcc Terminal
/***********************************************************/

#include "stego.h"	

/* 	Function to: check if the arguments are correct
*	Returns:  int depending on succes/fail */
int usage(int argc, char**argv, int expct, int reading) {

	if (argc < expct) {											// Too little command-line arguments
    printf("error usage: %s filename is missing correct arguments\n", argv[0]);
    return FILE_OPEN_FAIL;
  	}
  	
  	if ((fopen(argv[reading], "r"))==NULL) {					// Missing image file
    printf("error opening %s for reading\n", argv[reading]);
    return FILE_OPEN_FAIL;
  	}

  	return FILE_OPEN_SUCCESS;									// otherwise, success
}


/* 	Function to: write 8-bit (byte) of char m from message, into output
*	Returns: void */
void write_hidden_byte(FILE *infp, FILE *outfp, char m) {
    int c, i;
    int one = 0x01, or;
    int at[8]; 
    int mask = 0x80; 
      
    i = 0;
    while (mask>0) {
    	at[8-i] = ((m & mask) > 0 ? 1:0); 						// at[] will get either 1 or 0
    	mask >>= 1; 											// shifting to right
    	i++;
    }

    while ((c=fgetc(infp)) != EOF) {
      	for (i = 8; (i >= 0) && (c=fgetc(infp)); --i) {
        	if (at[i]) {
          	or = c | one;
            fputc(or, outfp);
        }
        else {
            if ((unsigned char)c & one)
              fputc(c-1, outfp);
            else
              fputc(c, outfp);
          }
        }
    }
}


/* 	Function to: read hidden message in pixel data
*	Returns:  char string of hidden message */
char* read_hidden_msg(FILE *infp, int leng) {
	int i;
	char * msg;
	msg = malloc((leng+1) * sizeof(char));

	for (i = 0; i < leng; ++i)									// loop until end of length
	{
		msg[i] = read_byte(infp);								// read char every 8-bit (byte)
	}
	
	msg[i+1] = '\0';											// end string with null char
	return msg;
}


/* 	Function to: read the length of hidden message
*	Returns:  int of size of hidden message */
unsigned int read_length(FILE *infp) {
	int i, c;
	unsigned int tmp = 0x01, and;
	unsigned int rtn = 0x00;

	for (i = 0; i < 8 && ((c=fgetc(infp))); ++i)				// get 8 least-significant bit inputs
	{	
		unsigned int or = 0x00000000;
		and = (unsigned int)c & tmp;
		//printf("[%d \t",and);
		or = or|and;
		or <<= 7-i;
		rtn = rtn|or;
		//printf("now ret is: %d, i=%d, or=%d ]\n",rtn, i, or);
	}

	return rtn;													// return our length
}


/* 	Function to: read 8-bits (byte) for char
*	Returns:  unsigned char from each 8-bit (byte) */
unsigned char read_byte(FILE *infp) {
	int i, c;
	unsigned char tmp = 0x01, and;
	unsigned char rtn = 0x00;

	for (i = 0; i < 8 && ((c=fgetc(infp))); ++i)				// get 8 least-significant bit inputs
	{	
		unsigned char or = 0x00000000;
		and = (unsigned char)c & tmp;
		//printf("[%d \t",and);
		or = or|and;
		or<<=7-i;
		rtn = rtn|or;
		//printf("now ret is: %d, i=%d, or=%d ]\n",rtn, i, or);
	}

	return rtn;													// return our ASCII value char
}


/* 	Function to: check the header when parsing
*	Returns:  int depending on if correct ppm header's conditions are met/matched */
int parse_header(FILE *fp) {
	int i;
	int state = STATE_HEADER;	// initial state, starting in Header
	int linecnt = 0;
	int c, next;
	int w=0, h=0, colorval = 0;	// initial value, meaning not initialized/set yet

	while (((c=fgetc(fp)) != EOF) && (state == STATE_HEADER)) { // Not EOF and Still in Header
		if ((linecnt == 0) && (c == 'P') && ((next=fgetc(fp)) == '6')) { // Condition #1: Must be 'P6'
			linecnt++;	// Condition #1 passed
		}

		else if ((linecnt > 0) && (c=='#')) { 	// Comments
			char cmt[1000];
			fgets(cmt, 1000, fp);
			linecnt++;
		}

		else if (isspace(c)) {
			if ((c == '\n') && (~colorval)) { //color value not encountered
				linecnt++;
				if ((colorval==255) && (w*h >= 255)) // all conditions matched/met. 
					state = STATE_PXDATA;	// Moving out of Header, into Pixel Data
			}
		}

		else if (isdigit(c) && (w == 0)) { 	// width not initialized
			int x = 0;
			int sum[4];						// holder to sum up width
			sum[x] = c - '0';				// convert to the ASCII value char 
			while (((c=fgetc(fp)) != EOF) && (isdigit(c))) { // if this part of the number; no whitespace yet
				if (isdigit(c)) {
				x++;
				sum[x] = c - '0';			// convert to the ASCII value char 
				}
			}
			ungetc(c, fp);	// not a digit; put back into stream

			int i;
			for (i = 0; i <= x; ++i) {
				w += sum[i] * power(10, x-i);		//sum up the digits in correct decimal position
			}
			linecnt++;
		}

		else if (isdigit(c) && (h == 0)) { 	// height not initialized
			int x = 0;
			int sum[4];						// holder to sum up width
			sum[x] = c - '0';				// convert to the ASCII value char
			while (((c=fgetc(fp)) != EOF) && (isdigit(c))) { 	// if this part of the number; no whitespace yet
				if (isdigit(c)) {
				x++;
				sum[x] = c - '0';			// convert to the ASCII value char
				}
			}
			ungetc(c, fp);					// not a digit; put back into stream

			int i;
			for (i = 0; i <= x; ++i) {
				h += sum[i] * power(10, x-i);		//sum up the digits in correct decimal position
			}
			linecnt++;
		}

		else if (isdigit(c) && (colorval == 0)) { // color value not initialized
			int x = 0;
			int sum[4];
			sum[x] = c - '0';
			while (((c=fgetc(fp)) != EOF) && (isdigit(c))) { // if this part of the number; no whitespace yet
				if (isdigit(c)) {
				x++;
				sum[x] = c - '0';
				}
			}
			ungetc(c, fp);

			int i;
			for (i = 0; i <= x; ++i) {
				colorval += sum[i] * power(10, x-i);	//sum up the digits in correct decimal position
			}
			linecnt++;
		}
		
		else {		// Everything else are Invalid; not correct header format
			ungetc(c, fp);
			state = STATE_INVALID;
		} 

	}


	if (state==STATE_PXDATA) {	// successfully parsed header
		ungetc(c, fp);
		return PARSE_HEADER_SUCCESS;
	}
	else {						// unsuccessful parsed
		return PARSE_HEADER_FAIL;
		printf("error invalid header: header fail to match requirements! \n");
	}


}


/* 	Function to: custom power function exclusive for int
*	Returns:  int after result of calculation */
int power(int base, unsigned int exp) {
    int i, result = 1;
    for (i = 0; i < exp; i++)
        result *= base;
    return result;
}


/* 	Function to: count length of string
*	Returns:  int of string length */
int string_length (char* s) {
  	char *e;
  	e=s;
  	while (*e!='\0')
    e++;
  return e-s;
}


/* 	Function to: copy header from input to output (get and put char); very similar to parse_header()
*	Returns:  int correct or incorrect header format */
int copy_header(FILE *fp, FILE *outfp) {
	int i;	
	int state = STATE_HEADER;
	int linecnt = 0;
	int c, next;
	int w=0, h=0, colorval = 0; // not yet initialized

	while (((c=fgetc(fp)) != EOF) && (state == STATE_HEADER)) {

		if ((linecnt == 0) && (c == 'P') && ((next=fgetc(fp)) == '6')) {
			fputc(c, outfp); 
			fputc(next, outfp); 
			linecnt++;
		}

		else if ((linecnt > 0) && (c=='#')) {
			char cmt[1000];
			fgets(cmt, 1000, fp);
			linecnt++;
		}

		else if (isspace(c)) {
			fputc(c, outfp);
			if ((c == '\n') && (~colorval)) { //color value not encountered
				linecnt++;
				if ((colorval==255) && (w*h >= 255)) // all conditions matched/met
					state = STATE_PXDATA; // entering pixal data
			}
		}

		else if (isdigit(c) && (w == 0)) { // width
			fputc(c, outfp);
			int x = 0;
			int sum[4];
			sum[x] = c - '0';				// get the ASCII char 
			while (((c=fgetc(fp)) != EOF) && (isdigit(c))) {
				fputc(c, outfp);
				if (isdigit(c)) {
				x++;
				sum[x] = c - '0';
				}
			}
			ungetc(c, fp);

			int i;
			for (i = 0; i <= x; ++i) {
				w += sum[i] * power(10, x-i);
			}
			linecnt++;
		}

		else if (isdigit(c) && (h == 0)) { // height
			fputc(c, outfp);
			int x = 0;
			int sum[4];
			sum[x] = c - '0';
			while (((c=fgetc(fp)) != EOF) && (isdigit(c))) {
				fputc(c, outfp);
				if (isdigit(c)) {
				x++;
				sum[x] = c - '0';
				}
			}
			ungetc(c, fp);

			int i;
			for (i = 0; i <= x; ++i) {
				h += sum[i] * power(10, x-i);		//sum up the digits in correct decimal position
			}
			linecnt++;
		}

		else if (isdigit(c) && (colorval == 0)) { // color value
			fputc(c, outfp);
			int x = 0;
			int sum[4];
			sum[x] = c - '0';
			while (((c=fgetc(fp)) != EOF) && (isdigit(c))) {
				fputc(c, outfp);
				if (isdigit(c)) {
				x++;
				sum[x] = c - '0';
				}
			}
			ungetc(c, fp);

			int i;
			for (i = 0; i <= x; ++i) {
				colorval += sum[i] * power(10, x-i);	//sum up the digits in correct decimal position
			}
			linecnt++;
		}
		
		else {
			ungetc(c, fp);
			state = STATE_INVALID;
		} 

	}


	if (state==STATE_PXDATA) {
		ungetc(c, fp);
		return PARSE_HEADER_SUCCESS;
	}
	else {
		return PARSE_HEADER_FAIL;
		printf("error invalid header: header fail to match requirements! \n");
	}


}

