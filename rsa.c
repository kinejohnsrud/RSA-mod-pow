#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

double time_spent;
unsigned long C;
unsigned long M;
unsigned int p;
unsigned int q;
unsigned long phi;
unsigned long n;
unsigned long d;
long e;
long m;

// Need to fix these...
int ebin[] = {1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1};
int elength = sizeof(ebin)/sizeof(ebin[0]);
int dbin[] = {1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1};
int dlength = sizeof(dbin)/sizeof(dbin[0]);


long long mul_inv(long long a, long long b){
	long long b0 = b, t, q;
	long long x0 = 0, x1 = 1;
	if (b == 1) return 1;
	while (a > 1) {
		q = a / b;
		t = b, b = a % b, a = t;
		t = x0, x0 = x1 - q * x0, x1 = t;
	}
	if (x1 < 0) x1 += b0;
	return x1;
}

long gcd(long long a, long long b){
  if (a == 0) return b;
  return gcd (b%a, a);
}

void signedConv(int binary[], int signedBinary[], int length){
	int firstOne = 1;
	signedBinary[0] = 1;
	for (int i = length-1; i >= 0; --i){
		if (binary[i] == 1){
			if (firstOne == 1){
				signedBinary[i+1] = -1;
				firstOne = 0;
			}
			else signedBinary[i+1] = 0;
		}
		if (binary[i] == 0){
			if (firstOne == 1) signedBinary[i+1] = 0;
			else signedBinary[i+1] = -1;
		}
	}
}

unsigned long recodingBinaryMethod(unsigned long base, int binary[], int length, unsigned long mod){
	unsigned long baseInverse = mul_inv(base, n);
	int signedLength = length+1;
	int signedBinary[signedLength];
	signedConv(binary, signedBinary, length);
	unsigned long long c = (signedBinary[0]==1) ? base : 1;
	for (int i = 1; i < signedLength; ++i){
		c = (c*c) % mod;
		if(signedBinary[i]==1) c = (c*base)%mod;
		else if(signedBinary[i]==-1) c = (c*baseInverse)%mod;
	}
	return c;
}

unsigned long binaryMethod(unsigned long base, int binary[], int length, unsigned long mod){
	unsigned long long c = (binary[0]==1) ? base : 1;
	for (int i = 1; i < length; ++i){
		c = (c*c) % mod;
		if(binary[i]==1) c = (c*base)%mod;
	}
	return c;
}

unsigned long modPow(unsigned long base, unsigned long exponent, unsigned long mod){
	unsigned long long c = 1;
	for (long long i = 0; i < exponent; ++i){
		c = (c*base)%mod;
	}
	return c;
}

void generateRSAValues(){
	m = 1234;
	p = 233;
	q = 223;
	phi = (p-1)*(q-1);
	n = p*q;
	e = 50555;
	d = mul_inv(e, phi);
	
	printf("m: %lu\n",m);
	printf("p: %i\n",p);
	printf("q: %i\n",q);
	printf("n: %lu\n",n);
	printf("phi: %lu\n", phi);
	printf("e: %ld\n",e);
	printf("e length: %i\n",elength);
	printf("d: %ld\n",d);
	printf("d length: %i\n",dlength);
	return;
}



long encrypt(char* message){
	int length = strlen(message);
	int messageBytes[length];

	for (int i = 0; i < length; ++i){
		messageBytes[i] = message[i]-0;
	}
	return 0;
}

int main(){
	clock_t begin, end;
	generateRSAValues();
	printf("\n** Results **\n");
	
	//**************** SIMPLE ****************
	begin = clock();
	for (int i = 0; i < 1000; ++i){
    	C = modPow(m, e, n);
    	M = modPow(C, d, n);
    }
    end = clock();

    printf("\nC: %lu\n",C);
    printf("M: %lu\n",M);
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Simple: %f sec\n", time_spent);

    //**************** BINARY ****************
    begin = clock();
    for (int i = 0; i < 1000; ++i){
    	C = binaryMethod(m, ebin, elength, n);
		M = binaryMethod(C, dbin, dlength, n);
    }
	end = clock();
	
	printf("\nC: %lu\n",C);
    printf("M: %lu\n",M);
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Binary: %f sec\n", time_spent);

    //************ RECODING BINARY ************
    begin = clock();
    for (int i = 0; i < 1000; ++i){
		C = recodingBinaryMethod(m, ebin, elength, n);
		M = recodingBinaryMethod(C, dbin, dlength, n);
	}
	end = clock();
	
	printf("\nC: %lu\n",C);
    printf("M: %lu\n",M);
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Recoding Binary: %f sec\n", time_spent);
    
    return 0;
}