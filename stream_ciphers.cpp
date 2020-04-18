#include <iostream>
#include <cassert>
#include <string.h>
#include <bitset>
#include <cmath>
#include <cstring>
#include <cctype>

using namespace std;

char *encode(char *plaintext, unsigned long key);

char *decode(char *ciphertext, unsigned long key);

int main();

int main() {
	//testing encode
	char test1[]{ "Hello world!"};

	cout << "\"" << test1 << "\"" << endl;

	char *ciphertext{encode(test1, 51323)};

	cout << "\"" << ciphertext << "\"" << endl;

	//testing decode
	char test2[]{"l=]V]&9!>trOu*9"};

	cout << "\"" << test2 << "\"" << endl;

	char *plaintext{decode(test2 , 51323)};

	cout << "\"" << plaintext << "\"" << endl;

}

char *encode(char *plaintext, unsigned long key){

	//initialize a new array to store cipher text

	int length {0};

	while(plaintext[length] != '\0'){

		++length;

	}

	//account for plain text that is not a multiple of 4 bytes

	int add{0};

	if(length % 4 != 0)
		add = 4 - (length % 4);

	unsigned char ciphertext[length + add];

	unsigned char temparr[length + add];

	for(int i = 0; i < length; i++){

		temparr[i] = plaintext[i];

	}

	for(int i = 0; i < add; i++){

		temparr[length + i] = '\0';

		ciphertext[length + i] = '\0';
	}

	//converting the key to binary

	bool nkey[64];

	for(int a = 0; a < 64; ++a){

		nkey[a] = key % 2;

		key /= 2;

	}


	//declare a 256 entry state array
	unsigned char S[256];

	//initialize the array with the numbers 0 through 255
	for(int a = 0; a < 256; ++a){
		S[a] = a;
	}

	//randomizing the key
	int i{0};
	int j{0};
	char temp = ' ';

	for(int x = 0; x < 256; ++x){

		unsigned long k = i % 64;

		j = (j + S[i] + nkey[k]) % 256;

		temp = S[i];

		S[i] = S[j];

		S[j] = temp;

		i = (i + 1) % 256;

	}

	//exclusive X-OR every byte to encode
	for(int y = 0; y < length; ++y){

		i = (i + 1) % 256;

		j = ( j + S[i]) % 256;

		temp = S[i];

		S[i] = S[j];

		S[j] = temp;

		int r = (S[i] + S[j]) % 256;

		char R = S[r];

		ciphertext[y] = temparr[y]^R;

	}

	//binary to text encoding (ASCII armor)

	char printable = '!';

	//convert to base 85 number and convert all char to printable char

	int group_4 = (length + add) / 4;

	char* armortext = new char[5*group_4 + 1];

	unsigned int currTotal = 0;

	int remainder{0};

	for(int k = 0; k < group_4; ++k){

		for(int i = 0; i < 4 ; ++i){
			if(i != 3){
				currTotal = (currTotal + ciphertext[k*4 + i]) << (8U);
			} else{
				currTotal = (currTotal + ciphertext[k*4 + i]);
			}
		}

		for(int j = 4; j >= 0; --j){
			remainder = currTotal % 85;
			armortext[5*k + j] = remainder + printable;
			currTotal /= 85;

		}
	}

	//update the last char to '\0'

	armortext[5*group_4] = '\0';

	return armortext;

}

char *decode(char *ciphertext, unsigned long key){

	int length {0};

	while(ciphertext[length] != '\0'){
		++length;
	}

	//initialize the a char array to store the plain text

	int group_5 = length / 5;

	unsigned char* plaintext = new unsigned char [4*group_5];

	plaintext[4*group_5] = '\0';

	char* originaltext = new char [4*group_5];

	//undo the process to make each character printable
	for(int a = 0; a < length; a++ ){
		ciphertext[a] = ciphertext[a] - '!';
	}


	//undo the base-85 conversion
	for(int b = 0; b < group_5; ++b){

		unsigned int currTotal{0};

		for(int i = 4; i >= 0; --i){
			currTotal += (ciphertext[5*b + (4-i)] * pow(85,i));
		}

		for(int j{0}; j < 4; ++j){
			plaintext[4*b + j] = (currTotal << (8*j)) >> (24U);
		}

	}

	//converting the key to binary
	bool nkey[64];

	for(int a = 0; a < 64; ++a){

		nkey[a] = key % 2;

		key /= 2;

	}

	//declare a 256 entry state array
	unsigned char S[256];

	//initialize the array with the numbers 0 through 255
	for(int a = 0; a < 256; ++a){
		S[a] = a;
	}

	//randomizing the key
	int i{0};
	int j{0};
	char temp = ' ';

	for(int x = 0; x < 256; ++x){

		unsigned long k = i % 64;

		j = (j + S[i] + nkey[k]) % 256;

		temp = S[i];

		S[i] = S[j];

		S[j] = temp;

		i = (i + 1) % 256;

	}

	//exclusive X-OR every byte to encode
	for(int y = 0; y < 4*group_5 ; ++y){

		i = (i + 1) % 256;

		j = ( j + S[i]) % 256;

		temp = S[i];

		S[i] = S[j];

		S[j] = temp;

		int r = (S[i] + S[j]) % 256;

		char R = S[r];

		originaltext[y] = plaintext[y]^R;

	}

	cout << "The original text is: ";

	originaltext[4*group_5] = '\0';

	return originaltext;
}
