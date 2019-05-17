//#pragma comment(lib, "libcrypto.lib")
#define _CRT_SECURE_NO_WARNINGS

#include <openssl/des.h>
#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>

#define ENC 1
#define DEC 0
#define MAX =10; 

int main(int argc, char* argv[]){
	FILE *fp,*fp1, *fp2; //fp=plain fp1=enc, fp2=dec 
	int i = 0, k = 1, check = 0, num = 2, num1 = 0;
	DES_cblock key = { 0, };
	DES_key_schedule schedule;
	DES_LONG counter[2] = { 0x00000000,0x00000000 };
	DES_LONG plain_text[2] = { 0, };

// open file 
	fp = fopen("plain.txt", "w");
	fp1 = fopen("kimtaejungEnc.txt", "w");
	fp2 = fopen("kimtaejungDec.txt", "w");

	if ( fp==NULL ||fp1 == NULL || fp2 == NULL ) {
		printf("File Open Error!!");
		system("pause");
		return 1;

	}

// key value get & check
	if (argc < 8) {
		printf("Error : Unvalid key \n");
		system("pause");
		return 1;
	}
	printf("key : ");
	for (i = 0; i < 8; i++) {
		key[i] = strtol(argv[k], NULL, 16);
		printf("%x", key[i]);
		k++;
		
	}
	printf("\n");

// counter value get & check 
	if (argc < 10) {
		printf("Error : Unvalid counter \n");
		return 1;
	}
	printf("counter : ");
	for (i = 0; i < 2; i++) {
		counter[i] = strtoul(argv[k], NULL, 16);
		printf("%x", counter[i]);
		k++;
	}
	printf("\n\n");


// key -> schedule change 
	if ((k = DES_set_key_checked(&key, &schedule)) != 0)
		printf("\nkey error : %d \n", k);//-1: parity -2 : weak key 
//plain_text input 
	
	while (k == 0) {
	
		plain_text[0] = 0x00000000;
		plain_text[1] = 0x00000000;
		printf("Input plain_text(ex : ffffffff 0f0f0f0f0f )\n *** default set is 00000000 00000000 ***\n ==> ");
		num=scanf("%x %x", &plain_text[0], &plain_text[1]);
		printf("plain_text : 0x%x 0x%x\n", plain_text[0], plain_text[1]);
		if(num==2)
			fprintf(fp, "%x %x\n", plain_text[0], plain_text[1]);
		else {
			printf("Unvalid input. Try again\n");
			continue;
		}
		printf("Input more (0) / Enough(1) : ");
		scanf("%d", &k);	
	
	}
	fclose(fp);

// DES CTR 
	fp = fopen("plain.txt", "r");
	if (fp == NULL ) {
		printf("File Open Error!!");
		system("pause");
		return 1;
	}
	while (!feof(fp)) {		
	 //get plain_text 
		plain_text[0] = 0x00000000;
		plain_text[1] = 0x00000000;
 		check =  fscanf(fp,"%x %x ", plain_text, plain_text+1);
		if (check == -1) 
			printf("Plain_text Error \n");
		check = 0;
		printf("plain_text : 0x%x 0x%x\n", plain_text[0], plain_text[1]);

	//Encryption & file
		DES_encrypt1(&counter[0], &schedule, ENC);
		*counter = *counter + 0x1;
		plain_text[0] = plain_text[0] ^ counter[0];
		plain_text[1] = plain_text[1] ^ counter[1];
		printf("DES Encryption: 0x%x 0x%x \n", plain_text[0], plain_text[1]);
		check = fprintf(fp1, "%x %x\n", plain_text[0], plain_text[1]);
		if (check == -1)
			printf("Encryption File Error  \n");
		check = 0;
	
	//Decryption & file 
		plain_text[0] = plain_text[0] ^ counter[0];
		plain_text[1] = plain_text[1] ^ counter[1];
		printf("DES Decryption: 0x%x 0x%x \n\n", plain_text[0], plain_text[1]);
		check=fprintf(fp2, "%x %x\n", plain_text[0], plain_text[1]);
		if (check == -1)
			printf("Decryption File Error  \n");
		check = 0;
	}

// close file 
	fclose(fp);
	fclose(fp1);
	fclose(fp2);
	
	system("pause");
	return 0;
}
