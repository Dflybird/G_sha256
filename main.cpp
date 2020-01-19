#include <iostream>
#include <cmath>
#include <bitset>

#define SHR(n, x) x>>n
#define ROTR(n, x) (x>>n)|(x>>(32-n))

#define CH(x,y,z) (x&y)^(~x&z)
#define MAJ(x,y,z) (x&y)^(x&z)^(y&z)
#define SIGMA0(x) ROTR(2,x)^ROTR(13,x)^ROTR(22,x)
#define SIGMA1(x) ROTR(6,x)^ROTR(11,x)^ROTR(25,x)
#define THETA0(x) ROTR(7,x)^ROTR(18,x)^SHR(3,x)
#define THETA1(x) ROTR(17,x)^ROTR(19,x)^SHR(10,x)

typedef unsigned long long WORD64;
typedef unsigned int WORD32;
typedef unsigned char BYTE;

// 32bit of the decimal part of the cube root of a prime number
void DCPN32(const unsigned int prime_num, unsigned int &nomal_num) {
  double num = pow(prime_num, (double)1/3);
  num = num - (unsigned int)num;
  //TODO
}

WORD32 _H[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
};

WORD32 _K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};


void fill_cleartext(const char &str, WORD64 size) {

}

void m_hash(const BYTE *str){
  WORD32 w[64], t1, t2, a, b, c, d, e, f, g, h, hash[8];

  for (int i = 0; i < 8; ++i) {
    hash[i] = _H[i];
  }

  for (int i = 0; i < 16; ++i) {
    for (int j = 0; j < 4; ++j) {
      WORD32 n = (3-j)*8;
      WORD32 temp = str[4*i+j]<<n;
      w[i] = w[i]^temp;
    }
  }

  for (int i = 16; i < 64; ++i) {
    w[i] = THETA0(w[i-15]) + THETA1(w[i-2]) + w[i-16] + w[i-7];
  }

  a = hash[0];
  b = hash[1];
  c = hash[2];
  d = hash[3];
  e = hash[4];
  f = hash[5];
  g = hash[6];
  h = hash[7];

  for (int i = 0; i < 64; ++i) {
    t1 = h + THETA1(e) + CH(e, f, g) + _K[i] + w[i];
    t2 = SIGMA0(a) + MAJ(a, b, c);
    h = g;
    g = f;
    f = e;
    e = d + t1;
    d = c;
    c = b;
    b = a;
    a = t1 + t2;
  }

//  hash[0] += a;
//  hash[1] += b;
//  hash[2] += c;
//  hash[3] += d;
//  hash[4] += e;
//  hash[5] += f;
//  hash[6] += g;
//  hash[7] += h;

//  hash[0] = (hash[0]+a) & 0xffffffff;
//  hash[1] = (hash[0]+b) & 0xffffffff;
//  hash[2] = (hash[0]+c) & 0xffffffff;
//  hash[3] = (hash[0]+d) & 0xffffffff;
//  hash[4] = (hash[0]+e) & 0xffffffff;
//  hash[5] = (hash[0]+f) & 0xffffffff;
//  hash[6] = (hash[0]+g) & 0xffffffff;
//  hash[7] = (hash[0]+h) & 0xffffffff;

//  WORD32 hash[8];
//  for (int i = 0; i < 8; ++i) {
//    hash[i] = a
//    for (int j = 0; j < 4; ++j) {
//      WORD32 n = (3-j)*8;
//      WORD32 temp = str[4*i+j]<<n;
//      w[i] = w[i]^temp;
//    }
//  }
  std::cout << std::hex << hash[0] << hash[1] << hash[2] << hash[3] << hash[4] << hash[5] << hash[6] << hash[7] << std::endl;
}

void m_sha256(const char *str, WORD64 size) {
  if (size < 56) {
    BYTE last_byte = 0x80;
    BYTE fill_text[64];
    int index = 0;
    for (int i = 0; i < size; ++i) {
      fill_text[i] = str[i];
      index = i;
    }
    fill_text[++index] = last_byte;
    for (int i = index; i < 56; ++i) {
      fill_text[i] = 0x00;
    }
    //补充字符原始长度
    WORD32 byte_len = 64;
    for (int i = 0; i < 8; ++i) {
      WORD32 n = 8*i;
      WORD32 temp = size<<(byte_len-n);
      fill_text[56+i] = temp>>n;
    }
    m_hash(fill_text);
  }



}

int main() {

//  std::cout << std::hex << i << std::endl;

//  BYTE b[4];
//  b[0] = 0b11111111;
//  b[1] = 0b11111111;
//  b[2] = 0b11111111;
//  b[3] = 0b11111111;
//
//  WORD32 w1 = 0;
//  for (int i = 0; i < 4; ++i) {
//    WORD32 n = (3-i)*8;
//    WORD32 temp = b[i]<<n;
//    w1 = w1^temp;
//  }
//
//
//  std::cout << std::bitset<32>(w1) << std::endl;

  char str[3];
  str[0] = '1';
  str[1] = '1';
  str[2] = '1';
//  m_sha256("a",1);
  m_sha256("111",3);
//  m_sha256(str,3);

  char str1[1];
  str1[0] = 'a';
//  m_sha256(str1,1);
//  std::cout << std::bitset<8>("a") << std::endl;
  return 0;
}
