#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

double time_spent;
unsigned int C;
unsigned int M;
unsigned int p;
unsigned int q;
unsigned int phi;
unsigned int n;
unsigned int d;
unsigned int e;
unsigned int m;
unsigned int r;

// Need to fix these...
int ebin[] = {1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1};
int elength = sizeof(ebin)/sizeof(ebin[0]);
int dbin[] = {1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1};
int dlength = sizeof(dbin)/sizeof(dbin[0]);


unsigned long mul_inv(unsigned long a, unsigned long b){
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

long gcd(unsigned long a, long long b){
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

unsigned long monPro(unsigned int abar, unsigned int bbar, unsigned int modprime, unsigned int r){
	unsigned long t = abar * bbar;
	unsigned long m = (t * modprime) % r;
	unsigned long u = (t + m * n) / r;
	if (u>=n) return u-n;
	return u;
}


unsigned long montgomeryExp(unsigned long base, int binary[], int length, unsigned int mod, unsigned int r){
	unsigned int rinv = mul_inv(r, n);
	unsigned int modprime = (r*rinv-1)/n;
	unsigned int basebar = (base * r) % n;
	unsigned int xbar = (1*r) % n;
	for (int i = 1; i < length; ++i){
		xbar = monPro(xbar, xbar, modprime, r);
		if(binary[i]==1) xbar = monPro(basebar, xbar, modprime, r);
	}
	unsigned int x = monPro(x, 1, modprime, r);
	return x;
}


unsigned long recodingBinaryMethod(unsigned int base, int binary[], int length, unsigned int mod){
	unsigned int baseInverse = mul_inv(base, n);
	int signedLength = length+1;
	int signedBinary[signedLength];
	signedConv(binary, signedBinary, length);
	unsigned long c = (signedBinary[0]==1) ? base : 1;
	for (int i = 1; i < signedLength; ++i){
		c = (c*c) % mod;
		if(signedBinary[i]==1) c = (c*base)%mod;
		else if(signedBinary[i]==-1) c = (c*baseInverse)%mod;
	}
	return c;
}

unsigned long binaryMethod(unsigned int base, int binary[], int length, unsigned int mod){
	unsigned long c = (binary[0]==1) ? base : 1;
	for (int i = 1; i < length; ++i){
		c = (c*c) % mod;
		if(binary[i]==1) c = (c*base)%mod;
	}
	return c;
}

unsigned long modPow(unsigned int base, unsigned int exponent, unsigned int mod){
	unsigned long c = 1;
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
	
	printf("m: %u\n",m);
	printf("p: %i\n",p);
	printf("q: %i\n",q);
	printf("n: %u\n",n);
	printf("phi: %u\n", phi);
	printf("e: %u\n",e);
	printf("e length: %i\n",elength);
	printf("d: %u\n",d);
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

    printf("\nC: %u\n",C);
    printf("M: %u\n",M);
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Simple: %f sec\n", time_spent);

    //**************** BINARY ****************
    begin = clock();
    for (int i = 0; i < 1000; ++i){
    	C = binaryMethod(m, ebin, elength, n);
		M = binaryMethod(C, dbin, dlength, n);
    }
	end = clock();
	
	printf("\nC: %u\n",C);
    printf("M: %u\n",M);
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Binary: %f sec\n", time_spent);

    //************ RECODING BINARY ************
    begin = clock();
    for (int i = 0; i < 1000; ++i){
		C = recodingBinaryMethod(m, ebin, elength, n);
		M = recodingBinaryMethod(C, dbin, dlength, n);
	}
	end = clock();
	
	printf("\nC: %u\n",C);
    printf("M: %u\n",M);
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Recoding Binary: %f sec\n", time_spent);
    
    //************ Montgomery ************
    r = 65536;
    begin = clock();
    for (int i = 0; i < 1000; ++i){
		C = montgomeryExp(m, ebin, elength, n, r);
		M = montgomeryExp(C, dbin, dlength, n, r);
	}
	end = clock();
	
	printf("\nC: %u\n",C);
    printf("M: %u\n",M);
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Montgomery: %f sec\n", time_spent);

    return 0;
}