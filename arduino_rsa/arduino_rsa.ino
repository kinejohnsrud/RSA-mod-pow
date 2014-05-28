
int led = 13;
unsigned long time_spent;
unsigned long time_begin;
unsigned long time_end;
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
int ebin[] = {1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1};
int elength = sizeof(ebin)/sizeof(ebin[0]);
int dbin[] = {1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1};
int dlength = sizeof(dbin)/sizeof(dbin[0]);


void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  unsigned long C = binaryMethod(m, ebin, elength, n);
  digitalWrite(led, HIGH);
  
  generateRSAValues();
  Serial.println("** Results **\n");
	
  //**************** SIMPLE ****************
  time_begin = millis();
  for (int i = 0; i < 1000; ++i){
    C = modPow(m, e, n);
    M = modPow(C, d, n);
  }
  time_end = millis();

  Serial.print("C: ");
  Serial.println(C);
  Serial.print("M: ");
  Serial.println(M);
  time_spent = time_end - time_begin;
  Serial.print("Simple: ");
  Serial.println(time_spent);

  //**************** BINARY ****************
  time_begin = millis();
  for (int i = 0; i < 1000; ++i){
    C = binaryMethod(m, ebin, elength, n);
    M = binaryMethod(C, dbin, dlength, n);
  }
  time_end = millis();

  Serial.print("C: ");
  Serial.println(C);
  Serial.print("M: ");
  Serial.println(M);
  time_spent = time_end - time_begin;
  Serial.print("Binary: ");
  Serial.println(time_spent);

  //************ RECODING BINARY ************
  time_begin = millis();
  for (int i = 0; i < 1000; ++i){
    C = recodingBinaryMethod(m, ebin, elength, n);
    M = recodingBinaryMethod(C, dbin, dlength, n);
  }
  time_end = millis();

  Serial.print("C: ");
  Serial.println(C);
  Serial.print("M: ");
  Serial.println(M);
  time_spent = time_end - time_begin;
  Serial.print("Recoding Binary: ");
  Serial.println(time_spent);
  
  //************ Montgomery ************
  r = 65443;
  //r = 41957;
  //r = 13577;
  //r = 16;
  time_begin = millis();
  for (int i = 0; i < 1000; ++i){
    C = montgomeryExp(m, ebin, elength, n);
    M = montgomeryExp(C, dbin, dlength, n);
  }
  time_end = millis();

  Serial.print("C: ");
  Serial.println(C);
  Serial.print("M: ");
  Serial.println(M);
  time_spent = time_end - time_begin;
  Serial.print("Montgomery: ");
  Serial.println(time_spent);

  digitalWrite(led, LOW);  
}

void generateRSAValues(){
	m = 1234;
	p = 233;
	q = 223;
	phi = (p-1)*(q-1);
	n = p*q;
	e = 50555;
	d = mul_inv(e, phi);
	
        Serial.print("m: ");
        Serial.println(m);
        Serial.print("p: ");
        Serial.println(p);
        Serial.print("q: ");
        Serial.println(q);
        Serial.print("n: ");
        Serial.println(n);
        Serial.print("phi: ");
        Serial.println(phi);
        Serial.print("e: ");
        Serial.println(e);
        Serial.print("elength: ");
        Serial.println(elength);
        Serial.print("d: ");
        Serial.println(d);
        Serial.print("dlength: ");
        Serial.println(dlength);
	return;
}

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

unsigned long monPro(unsigned int abar, unsigned int bbar, unsigned int mod, unsigned int modprime){
	unsigned long t = abar * bbar;
	unsigned long m = (t * modprime) % r;
	unsigned long u = (t + m * mod) / r;
	if (u>=mod) return u-mod;
	return u;
}


unsigned long montgomeryExp(unsigned long base, int binary[], int length, unsigned int mod){
	unsigned int rinv = mul_inv(r, mod);
	unsigned int modprime = (r*rinv-1)/mod;
	unsigned int basebar = (base * r) % mod;

	unsigned int xbar = (1*r) % mod;
	for (int i = 0; i < length; ++i){
		xbar = monPro(xbar, xbar, mod, modprime);
		if(binary[i]==1){
			xbar = monPro(basebar, xbar, mod, modprime);
		}
	}
	unsigned int x = monPro(xbar, 1, mod, modprime);
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
