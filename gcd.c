#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

long NUMBERS_SIZE = 100000;
long TREE_SIZE = 100;

void gcd(mpz_t **tree, int t, long *sizes)
{
  FILE *out = fopen("output.txt", "w");

  mpz_t square, rem;
  mpz_init(square);
  mpz_init(rem);

  mpz_t *remainders = tree[t];
  t--;

  while (t >= 0)
  {
    mpz_t *X = tree[t];
    t--;

    mpz_t *prep = malloc(sizeof(mpz_t) * sizes[t + 1]);
    for (long i = 0; i < sizes[t + 1]; i++)
    {
      mpz_set(square, X[i]);
      mpz_mul(square, square, square);
      mpz_set(rem, remainders[i / 2]);
      mpz_mod(rem, rem, square);
      mpz_init_set(prep[i], rem);
    }
    remainders = prep;
  }

  mpz_t p, q, divided;
  mpz_t *numbers = tree[0];
  long size = sizes[0];
  mpz_init(p);
  mpz_init(q);
  mpz_init(divided);

  for (long i = 0; i < size; i++)
  {
    mpz_fdiv_q(divided, remainders[i], numbers[i]);
    mpz_gcd(p, divided, numbers[i]);

    if (mpz_cmp_ui(p, 1) != 1)
    {
      continue;
    }
    mpz_fdiv_q(q, numbers[i], p);
    fprintf(out, "index: %ld\n", i);
    gmp_fprintf(out, "p: %Zx\n", p);
    gmp_fprintf(out, "q: %Zx\n", q);
    gmp_fprintf(out, "N: %Zx\n", numbers[i]);
  }
  fclose(out);
}

void read_numbers(mpz_t *numbers)
{
  FILE *fptr;
  char buffer[1024];
  fptr = fopen("moduli.txt", "r");

  long i = 0;
  while (fscanf(fptr, "%s", buffer) != EOF)
  {
    mpz_init_set_str(numbers[i], buffer, 16);
    i++;
    if (i == NUMBERS_SIZE)
    {
      break;
    }
  }
  fclose(fptr);
}

long product_tree(mpz_t *numbers, mpz_t **tree, long *sizes)
{
  long s = NUMBERS_SIZE;
  long j = 0;
  long t = 0;
  while (s > 1)
  {
    mpz_t *ptr = malloc(sizeof(mpz_t) * s);
    for (long i = 0; i < s; i++)
    {
      mpz_init_set(ptr[i], numbers[i]);
    }
    tree[t] = ptr;
    sizes[t] = s;
    t++;

    for (long i = 0; i < s - 1; i += 2)
    {
      mpz_t prod;
      mpz_init(prod);
      mpz_mul(prod, numbers[i], numbers[i + 1]);
      mpz_set(numbers[j], prod);
      j++;
    }

    if (s % 2 == 1)
    {
      mpz_set(numbers[j], numbers[s - 1]);
      s = s / 2 + 1;
    }
    else
    {
      s = s / 2;
    }
    j = 0;
  }

  mpz_t *ptr = malloc(sizeof(mpz_t));
  mpz_init_set(ptr[0], numbers[0]);
  tree[t] = ptr;
  sizes[t] = 1;

  return t;
}

int main(int argc, char const *argv[])
{
  mpz_t numbers[NUMBERS_SIZE];
  mpz_t *tree[TREE_SIZE];
  long sizes[TREE_SIZE];

  read_numbers(numbers);

  gcd(tree, product_tree(numbers, tree, sizes), sizes);

  return 0;
}