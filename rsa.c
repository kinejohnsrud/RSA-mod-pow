#include <stdio.h>
#include <string.h>
#include <math.h>

long p;
long q;
long long phi;
long long n;
long e;
long long d;
long long m;

int mul_inv(long long a, long long b){
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

long long modPow(long long base, long long exp, long long mod){
	long long c = 1;
	for (long long i = 0; i < exp; ++i){
		c = (c*base)%mod;
	}
	return c;
}

void generateRSA(){
	m = 8;
	p = 11;
	//p = 1073741783;
	q = 13;
	//q = 1073741717;
	phi = (p-1)*(q-1);
	n = p*q;
	//e=99999999989;
	//e = 65537;
	e = 7;
	
	printf("m: %lld\n",m);
	printf("p: %ld\n",p);
	printf("q: %ld\n",q);
	printf("n: %lld\n",n);
	printf("phi: %llu\n", phi);
	printf("e: %ld\n",e);
	d = mul_inv(e, phi);
	//d = 645922348764255065;
	//d = 103;
	printf("d: %lld\n",d);
	long long C = modPow(m, e, n);
	printf("C: %lld\n",C);
	long long M = modPow(C, d, n);
	printf("M: %lld\n",M);
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
    generateRSA();
    //long C = encrypt("message");
	//printf("%lld\n", C);
    return 0;
}