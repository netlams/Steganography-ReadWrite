# Steganography-WriteRead
For Computer Sys. and Low-Level Programming. Read/write a secret message to an image file (PPM).

<h2>Language written in:</h2> 
<p>C</p>

<h2>Compilation Platform:</h2>
<p>Linux / MAC OS X</p>

<h2>Description:</h2>
<p>This project comprises of two programs: one which hides a short, secret message in an image file, and a
second program which reads the hidden message back out of the image. Hidden messages are a maximum of 255 characters. </p>
<p>At the moment this project can only accept PPM files due to its simplicity. In the PPM folder, you will find two identical looking image files (TU.ppm and TU_modified.ppm). As identical as they may look, the second file actually contains a secret message in its bit values.</p>

<h2>Usage:</h2>
<p><strong>ReadMsg:</strong> This program accepts a command-line argument, the name of the file containing the hidden message.
ReadMsg will parse the file(PPM header and its body data). Hidden in the least-significant bits of the pixel data, it will find the length of the message, which is hidden in a single byte spread across the first 8 data bytes of the picture. It will then read the hidden message and print it to the screen.</p>
<p><strong>WriteMsg:</strong> This program via command-line arguments, the secret message to write, the name of the PPM file, and the name of output PPM file. It will parse the PPM header and write it to the output file. It will write the length of the secret to the beginning of the PPM data. Then it will hide the message after it (the least significant bits of pixel data in the image. So, for example, if we're going to hide a particular byte b in the image, the 8 bits of b will be spread across the least significant bits of 8 bytes of the image file (i.e., hidden in the data for 2 2/3 pixels).</p>

<h2>Testing:</h2>
<p>Compile ReadMsg.c to run the message reading program. See screen shot. Arguments: (1) name of image file.</p>
<p>Compile WriteMsg.c to run the message hiding program. See screen shot. Arguments: (3) message, name of input file, name of output file.</p>

