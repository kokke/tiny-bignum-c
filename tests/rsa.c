/*
  message m = 123

  P = 61                  <-- 1st prime, keep secret and destroy after generating E and D
  Q = 53                  <-- 2nd prime, keep secret and destroy after generating E and D
  N = P * Q = 3233        <-- modulo factor, give to others

  T = totient(N)          <-- used for key generation
    = (P - 1) * (Q - 1)
    = 3120

  E = 1 < E < totient(N)  <-- public exponent, give to others
  E is chosen to be 17

  find a number D such that ((E * D) / T) % T == 1
  D is chosen to be 2753  <-- private exponent, keep secret


  encrypt(T) = (T ^ E) mod N     where T is the clear-text message
  decrypt(C) = (C ^ D) mod N     where C is the encrypted cipher


  Public key consists of  (N, E)
  Private key consists of (N, D)


  RSA wikipedia example (with small-ish factors):

    public key  : n = 3233, e = 17
    private key : n = 3233, d = 2753
    message     : n = 123

    cipher = (123 ^ 17)   % 3233 = 855
    clear  = (855 ^ 2753) % 3233 = 123  

*/


#include <stdio.h>
#include <string.h> /* for memcpy */
#include "bn.h"

/* O(log n) */
void pow_mod_faster(struct bn* a, struct bn* b, struct bn* n, struct bn* res)
{
  bignum_from_int(res, 1); /* r = 1 */

  struct bn tmpa;
  struct bn tmpb;
  struct bn tmp;
  bignum_assign(&tmpa, a);
  bignum_assign(&tmpb, b);

  while (1)
  {
    if (tmpb.array[0] & 1)     /* if (b % 2) */
    {
      bignum_mul(res, &tmpa, &tmp);  /*   r = r * a % m */
      bignum_mod(&tmp, n, res);
    }
    bignum_rshift(&tmpb, &tmp, 1); /* b /= 2 */
    bignum_assign(&tmpb, &tmp);

    if (bignum_is_zero(&tmpb))
      break;

    bignum_mul(&tmpa, &tmpa, &tmp);
    bignum_mod(&tmp, n, &tmpa);
  }
}

static void test_rsa_1(void)
{
  /* Testing with very small and simple terms */
  char buf[8192];
  struct bn M, C, E, D, N;


  const int p = 11;
  const int q = 13;
  const int n = p * q;
//int t = (p - 1) * (q - 1);
  const int e = 7;
  const int d = 103;
  const int m = 9;
  const int c = 48;
  int m_result, c_result;

  bignum_init(&M);
  bignum_init(&C);
  bignum_init(&D);
  bignum_init(&E);
  bignum_init(&N);

  bignum_from_int(&D, d);
  bignum_from_int(&C, 48);
  bignum_from_int(&N, n);

  printf("\n");

  printf("  Encrypting message m = %d \n", m);
  printf("  %d ^ %d mod %d = %d ? \n", m, e, n, c);
  bignum_from_int(&M, m);
  bignum_from_int(&E, e);
  bignum_from_int(&N, n);
  pow_mod_faster(&M, &E, &N, &C);
  c_result = bignum_to_int(&C);
  bignum_to_string(&C, buf, sizeof(buf));
  printf("  %d ^ %d mod %d = %d \n", m, e, n, c_result);
  printf("  %d ^ %d mod %d = %s \n", m, e, n, buf);

  printf("\n");

  printf("  Decrypting message c = %d \n", c);
  printf("  %d ^ %d mod %d = %d ? \n", c, d, n, m);
  pow_mod_faster(&C, &D, &N, &M);
  m_result = bignum_to_int(&M);
  bignum_to_string(&M, buf, sizeof(buf));
  printf("  %d ^ %d mod %d = %d \n", c, d, n, m_result);
  printf("  %d ^ %d mod %d = %s \n", c, d, n, buf);

  printf("\n");
}





void test_rsa_2(void)
{
  char buf[8192];
  struct bn M, C, E, D, N;


  const int p = 61;
  const int q = 53;
  const int n = p * q;
//int t = (p - 1) * (q - 1);
  const int e = 17;
  const int d = 2753;
  const int m = 123;
  const int c = 855;
  int m_result, c_result;

  bignum_init(&M);
  bignum_init(&C);
  bignum_init(&D);
  bignum_init(&E);
  bignum_init(&N);

  bignum_from_int(&D, d);
  bignum_from_int(&C, 1892);
  bignum_from_int(&N, n);

  printf("\n");

  printf("  Encrypting message m = %d \n", m);
  printf("  %d ^ %d mod %d = %d ? \n", m, e, n, c);
  bignum_from_int(&M, m);
  bignum_from_int(&E, e);
  bignum_from_int(&N, n);
  pow_mod_faster(&M, &E, &N, &C);
  c_result = bignum_to_int(&C);
  bignum_to_string(&C, buf, sizeof(buf));
  printf("  %d ^ %d mod %d = %d \n", m, e, n, c_result);
  printf("  %d ^ %d mod %d = %s \n", m, e, n, buf);

  printf("\n");

  printf("  Decrypting message c = %d \n", c);
  printf("  %d ^ %d mod %d = %d ? \n", c, d, n, m);
  pow_mod_faster(&C, &D, &N, &M);
  m_result = bignum_to_int(&M);
  bignum_to_string(&M, buf, sizeof(buf));
  printf("  %d ^ %d mod %d = %s \n", c, d, n, buf);
  printf("  %d ^ %d mod %d = %d \n", c, d, n, m_result);

  printf("\n");
}


void test_rsa_3(void)
{
  char buf[8192];
  struct bn M, C, E, D, N;


  const int p = 2053;
  const int q = 8209;
  const int n = p * q;
//int t = (p - 1) * (q - 1);
  const int e = 17;
  const int d = 2753;
  const int m = 123;
  const int c = 14837949;
  int m_result, c_result;

  bignum_init(&M);
  bignum_init(&C);
  bignum_init(&D);
  bignum_init(&E);
  bignum_init(&N);

  bignum_from_int(&D, d);
  bignum_from_int(&C, c);
  bignum_from_int(&N, n);

  printf("\n");

  printf("  Encrypting message m = %d \n", m);
  printf("  %d ^ %d mod %d = %d ? \n", m, e, n, c);
  bignum_from_int(&M, m);
  bignum_from_int(&E, e);
  bignum_from_int(&N, n);
  pow_mod_faster(&M, &E, &N, &C);
  c_result = bignum_to_int(&C);
  bignum_to_string(&C, buf, sizeof(buf));
  printf("  %d ^ %d mod %d = %d \n", m, e, n, c_result);
  printf("  %d ^ %d mod %d = %s \n", m, e, n, buf);

  printf("\n");

  printf("  Decrypting message c = %d \n", c);
  printf("  %d ^ %d mod %d = %d ? \n", c, d, n, m);
  pow_mod_faster(&C, &D, &N, &M);
  m_result = bignum_to_int(&M);
  bignum_to_string(&M, buf, sizeof(buf));
  printf("  %d ^ %d mod %d = %s \n", c, d, n, buf);
  printf("  %d ^ %d mod %d = %d \n", c, d, n, m_result);

  printf("\n");
}




static void test_rsa1024(void)
{
  char public[]  = "a15f36fc7f8d188057fc51751962a5977118fa2ad4ced249c039ce36c8d1bd275273f1edd821892fa75680b1ae38749fff9268bf06b3c2af02bbdb52a0d05c2ae2384aa1002391c4b16b87caea8296cfd43757bb51373412e8fe5df2e56370505b692cf8d966e3f16bc62629874a0464a9710e4a0718637a68442e0eb1648ec5";
  char private[] = "3f5cc8956a6bf773e598604faf71097e265d5d55560c038c0bdb66ba222e20ac80f69fc6f93769cb795440e2037b8d67898d6e6d9b6f180169fc6348d5761ac9e81f6b8879529bc07c28dc92609eb8a4d15ac4ba3168a331403c689b1e82f62518c38601d58fd628fcb7009f139fb98e61ef7a23bee4e3d50af709638c24133d";
  char buf[8192];

  struct bn n; /* public  key */
  struct bn d; /* private key */
  struct bn e; /* public exponent */
  struct bn m; /* clear text message */
  struct bn c; /* cipher text */

  //int len_pub = strlen(public);
  //int len_prv = strlen(private);

  int x = 54321;

  bignum_init(&n);
  bignum_init(&d);
  bignum_init(&e);
  bignum_init(&m);
  bignum_init(&c);

  bignum_from_string(&n, public,  256);
  bignum_from_string(&d, private, 256);
  bignum_from_int(&e, 65537);
  bignum_init(&m);
  bignum_init(&c);

  bignum_from_int(&m, x);
  bignum_to_string(&m, buf, sizeof(buf));
  printf("m = %s \n", buf);

//printf("  Copied %d bytes into m\n", i);

  printf("  Encrypting number x = %d \n", x);
  pow_mod_faster(&m, &e, &n, &c);
  printf("  Done...\n\n");

  bignum_to_string(&c, buf, sizeof(buf));
  printf("  Decrypting cipher text '");
  int i = 0;
  while (buf[i] != 0)
  {
    printf("%c", buf[i]);
    i += 1;
  }
  printf("'\n");

  /* Clear m */
  bignum_init(&m); 

  pow_mod_faster(&c, &d, &n, &m);
  printf("  Done...\n\n");


  bignum_to_string(&m, buf, sizeof(buf));
  printf("m = %s \n", buf);
}


int main()
{
  printf("\n");
  printf("Testing RSA encryption implemented with bignum. \n");



  test_rsa_1();
  test_rsa_2();
  test_rsa_3();

  test_rsa1024();

  printf("\n");
  printf("\n");



  return 0;
}



#if 0
/* O(n) */
void pow_mod_fast(struct bn* b, struct bn* e, struct bn* m, struct bn* res)
{
/*
  Algorithm in Python / Pseudo-code :

    def pow_mod2(b, e, m):
      if m == 1:
        return 0
      c = 1
      while e > 0:
        c = (c * b) % m
        e -= 1
      return c
*/

  struct bn tmp;
  bignum_from_int(&tmp, 1);

  bignum_init(res); // c = 0

  if (bignum_cmp(&tmp, m) == EQUAL)
  {
    return;  // return 0
  }

  bignum_inc(res); // c = 1

  while (!bignum_is_zero(e))
  {
    bignum_mul(res, b, &tmp);
    bignum_mod(&tmp, m, res);
    bignum_dec(e);
  }
}

void pow_mod_naive(struct bn* b, struct bn* e, struct bn* m, struct bn* res)
{
/*
  Algorithm in Python / Pseudo-Code:

    def pow_mod(b, e, m):
      res = 0
      if m != 1:
        res = 1
        b = b % m
        while e > 0:
          if e & 1:
            res *= b
            res %= m
          e /= 2
          b *= b
          b %= m
      return res
*/ 
  struct bn one;
  bignum_init(&one);
  bignum_inc(&one);

  if (bignum_cmp(&one, m) == EQUAL)      // if m == 1:
  {                                      // {
    bignum_init(res);                    //   return 0
  }                                      // }
  else                                   // else:
  {                                      // {
    struct bn tmp;                       //
    struct bn two;
    bignum_init(&two);
    bignum_inc(&two); bignum_inc(&two);
    bignum_init(res);                    //
    bignum_inc(res);                     //   result = 1
    bignum_mod(b, m, &tmp);              //   b = b % m
    bignum_assign(b, &tmp);              //
                                         //   while e > 0:
    while (!bignum_is_zero(e))           //   {  
    {                                    //
      bignum_and(e, &one, &tmp);         //   
      if (!bignum_is_zero(&tmp))         //     if e & 1:
      {                                  //     {
        bignum_mul(res, b, &tmp);        //
        bignum_assign(res, &tmp);        //       result *= b
        bignum_mod(res, m, &tmp);        //
        bignum_assign(res, &tmp);        //       result %= b
      }                                  //
      bignum_div(e, &two, &tmp);         //     }
      bignum_assign(e, &tmp);            //     e /= 2
      bignum_mul(b, b, &tmp);            //
      bignum_assign(b, &tmp);            //     b *= b
    }                                    //   }
                                         //   return result
  }                                      // }
}
#endif



