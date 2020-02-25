/*
 * Program Name: Cyphercrack
 * Created By: Thomas Osgood
 *
 * Description:
 * 	Program to take cyphertext and attempt to determine
 * 	what the plaintext is using basic cryptography
 * 	techniques / decryption methods.
 *
 * Built On:
 * 	PoP!_Os (Debian Based Linux)
 *
 * To Compile:
 * 	gcc -o cyphercrack cypherCrack.c
 */

/* Required Includes */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* MACRO Definitions */
#define KILOBYTE 1000
#define MAXLEN 100

/* Function Prototypes */
int countE(char plaintext[], int size);
int getFileSize(char *filename);
int strSize(char text[]);
char *encryptCaesar(char plain[], int size, char key);
char *readFileText(char *filename, int size);
void buildHistogram(char crypt[], int size);
void testCaesar(char cypher[], int size);

/* Main Function */
int main(void) {	
	/* Initialize Variables */
	char cypher[MAXLEN] = {'\0'};
	char inp;
	int i = 0;
	int cypherSize = 0;

	/* Get Cyphertext From End-User */
	printf("Enter cypher text:\n");
	fgets(cypher, MAXLEN, stdin);

	/* Calculate Length Of Cyphertext */
	/* cypherSize = sizeof(cypher) / sizeof(cypher[0]); */
	cypherSize = strSize(cypher);

	/* Turn Cyphertext Lowercase */
	for ( i = 0; i < cypherSize; i++) {
		cypher[i] = tolower(cypher[i]);
	}

	encryptCaesar(cypher, cypherSize, 'd');

	;
	/* Display Cypthertext */
	printf("\nCypher Text:\n%s\nCypher Size: %d\n", cypher, cypherSize);

	/* Attempt To Break Cypher Using Caesar Brute-Force */
	testCaesar(encryptCaesar(cypher,cypherSize,'d'), cypherSize);

	/* Build Histogram For Cypher */
	buildHistogram(cypher, cypherSize);
	system("python3 graphHist.py");

	/* Return EXIT_SUCCESS */
	return 0;
}

/* Sub-Functions */

/*
 * Function Name: buildHistogram
 * Description:
 * 	Function to get a count of all letters in
 * 	the alphabet for the encrypted message.
 * Input(s):
 * 	crypt - encrypted text to count
 * 	size - size of the encrypted text
 * Return(s):
 * 	None
 */
void buildHistogram(char crypt[], int size) {
	char *histogramFile = "histogram.txt";
	FILE *fp;
	int baseLetter = (int)'a';
	int endLetter = (int)'z';
	int i = 0, x = 0;
	int count = 0;

	fp = fopen(histogramFile, "w");
	for (i = baseLetter; i <= endLetter; i++) {
		count = 0;
		for (x = 0; x < size; x++) {
			if ((int)crypt[x] == i) {
				count++;
			}
		}
		fprintf(fp, "%c:%d\n", (char)i, count);
	}
	fclose(fp);

	printf("[+] Histogram Written To %s ...\n", histogramFile);


	return;
}

/*
 * Function Name: countE
 * Description:
 * 	Function to count the number of times the
 * 	letter E shows up in a string.
 * Input(s):
 * 	plaintext - string to count Es in.
 * 	size - length of plaintext
 * Return(s):
 * 	count - number of E's in the string.
 */
int countE(char plaintext[], int size) {
	/* Initialize Local Variables */
	int count = 0;
	int i = 0;
	int target = (int)'e';

	/* Loop Through Text & Count E's */
	for ( i = 0; i < size; i++ ) {
		/* E Has Been Found; Increment Counter */
		if ( (int)plaintext[i] == target )
			count++;

		/* End Of String Found; Exit Loop */
		if ( plaintext[i] == '\n')
			break;
	}

	/* Return Number Of E's */
	return count;
}

/*
 * Function Name: getFileSize
 * Description:
 * 	Function to take a file and get the
 *	size of it.
 * Input(s):
 * 	filename - name of file
 * Return(s):
 * 	size - file size (bytes)
 */
int getFileSize(char *filename) {
	int size = 0;
	FILE *fp;
	fp = fopen(filename, "r");
	fseek(fp, SEEK_SET, SEEK_END);
	size = ftell(fp);
	fclose(fp);
	return size;
}

/*
 * Function Name: strSize
 * Description:
 * 	Function to go through a string and determine the
 * 	actual size of the string.
 * Input(s):
 * 	text - string to get size of
 * Return(s):
 * 	count - size of string (int)
 */
int strSize(char text[]) {
	/* Initialize Local Variables */
	int count = 0;
	int i = 0;

	/* Loop Through Text Until NULL or Newline Found */
	while ((text[i] != '\0') && (text[i] != '\n')) {
		count++;
		i++;
	}

	/* Account For Newline Found */
	if ( text[i] == '\n')
		count++;

	/* Return Size Of String */
	return count;
}

/*
 * Function Name: testCaesar
 * Description:
 * 	Function that takes a cypher text and
 * 	attempts to break it using the brute-force
 * 	technique for a Caesar Cypher.
 * Input(s):
 * 	cypher - cypher text to decrypt
 * 	size - size of text array (int)
 * Return(s):
 * 	None
 */
void testCaesar(char cypher[], int size) {
	/* Initialize Local Variables */
	FILE *fp;
	char *filename = "caesar_output.txt";
	char plaintext[size];
	int baseletter = (int)'a';
	int lastletter = (int)'z'; 
	int i = 0, x = 0;
	int numE = 0;
	int ptsize = 0;

	for (i = 0; i < size; i++) {
		plaintext[i] = '\0';
	}

	/* Open File For APPENDING */
	fp = fopen(filename,"w");

	/* Loop Through Alphabet */
	for ( x = 0; x < 26; x++ ) {
		/* Loop Through Cyphertext */
		for ( i = 0; i < size; i++ ) {
			/*
			 * If cyphertext in position is not a letter from a-z
			 * keep it the same, otherwise add the x value to the
			 * cyphertext letter
			 *
			 * Creates plaintext results and saves them in a text
			 * file named caesar_output.txt
			 */
			if ( ((int)cypher[i] < baseletter) || ((int)cypher[i] > lastletter) ) {
				plaintext[i] = cypher[i];
			} else {
				plaintext[i] = (char)(((((int)cypher[i] - baseletter) + x) % 26) + baseletter);
			}
		}
		ptsize = sizeof(plaintext) / sizeof(plaintext[0]);
		numE = countE(plaintext, ptsize);
		
		/* Only Display Probable Options */
		if (numE > 0) {
			printf("Plaintext:\n%s\n",plaintext);
			fprintf(fp, "[%c][%d] %s", (char)x+baseletter, numE, plaintext);
		}
	}

	/* Close File */
	fclose(fp);
	return;
}


/*
 * Function Name: encryptCaesar
 * Description:
 * 	Function that takes plain text and
 * 	encrypts it using the Caesar Cypher.
 * Input(s):
 * 	Plain - plain text to encrypt
 * 	size - size of text array (int)
 * 	key - cypher key (char)
 * Return(s):
 * 	None
 */
char *encryptCaesar(char plain[], int size, char key) {
	int i = 0;
	int baseletter = (int)'a';
	int lastletter = (int)'z';
	char *cypher = (char *) malloc (size * sizeof(char));
	int l_key = ((int)tolower(key)) - baseletter;

	for (i = 0; i < size; i++) {
		plain[i] = tolower(plain[i]);
		if ((plain[i] < baseletter) || (plain[i] > lastletter)) {
			*cypher = plain[i];
		} else {
			*cypher = (((plain[i] - baseletter) + l_key) % 26) + baseletter;
		}
		cypher++;
	}

	for (; i > 0; i--) {
		cypher--;
	}

	printf("\nENCRYPTED:\n%s\n", cypher);
	return cypher;
}

/*
 * Function Name: readFileText
 * Description:
 * 	Read all text in a file and return it
 * 	in a "char *" 
 * Input(s):
 * 	filename - name of the file to read
 * 	size - size of file to read
 * Return(s):
 * 	contents - contents of read file
 */
char *readFileText(char *filename, int size) {
	FILE *fp;
	char c = '\0';
	char *contents = (char *) calloc (size, sizeof(char));
	int i = 0;
	fp = fopen(filename, "r");
	
	if (fp == NULL) {
		printf("[!] ERROR OPENING %s ...\n", filename);
		contents = "[!] ERROR OPENING FILE... CONTENTS UNAVAILABLE";
		return contents;
	}

	while ( (c = fgetc(fp)) != EOF ) {
		contents[i] = c;
		i++;
	}

	fclose(fp);

	return contents;
}
