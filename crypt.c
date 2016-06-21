#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define KEY_SIZE 32
#define BUFF_SIZE 8192

unsigned int holdrand = 0;

static void Srand (unsigned int seed) {
  holdrand = seed;
}

static int Rand (void) {
  return(((holdrand = holdrand * 214013L + 2531011L) >> 16) & 0x7fff);
}

char* genere_key(void) {
  int i;
  static char key[KEY_SIZE+1];
  const char charset[] = 
    "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "123456789";
  
  for(i = 0; i < KEY_SIZE; i++) {
    key[i] = charset[Rand() % (sizeof(charset) - 1)];
  }
  key[KEY_SIZE] = '\0';

  return key;
}

void crypt_buffer(unsigned char *bufferin, unsigned char *bufferout,  size_t size, char *key) {
  size_t i;
  int j;

  j = 0;
  for(i = 0; i < size; i++) {
    if(j >= KEY_SIZE)
      j = 0;
    bufferout[i] = bufferin[i] ^ key[j];
    j++;
  }
}

void crypt_file(unsigned char *filename, unsigned char *bufferin, size_t size) {
  char *key;
  unsigned char bufferout[BUFF_SIZE];

  unsigned int begin = 1354320000; // 01/12/2012
  unsigned int end   = 1356998400; // 01/01/2013
  unsigned int i = begin;

  for(; i < end; i++) {

      Srand(i);

      key = genere_key();

      crypt_buffer(bufferin, bufferout, size, key);

        if (i % 100000 == 0) {
            printf("[+] trying key : %s\n", key);
        }

      if (bufferout[0] == 'B' && bufferout[1] == 'Z' && bufferout[2] == 'h') {
          char path[128];
          FILE *out;
          printf("[+] File header may be bzip2ped... Used key : %s\n", key);

          snprintf(path, sizeof(path)-1, "%s.%i", filename, i);
          if((out = fopen(path, "w")) == NULL) {
              perror("[-] fopen (out) ");
              exit(EXIT_FAILURE);
          }
          fwrite(bufferout, 1, size, out);
          fclose(out);
      }

  }

}

int main(int argc, char **argv) {
  FILE *in;


  if(argc != 2) {
    printf("[-] Usage : %s <file>\n", argv[0]);
    return EXIT_FAILURE;
  }


  if((in = fopen(argv[1], "r")) == NULL) {
    perror("[-] fopen (in) ");
    return EXIT_FAILURE;
  }

  unsigned char buffer[BUFF_SIZE];
  size_t size;
  size = fread(buffer, 1, BUFF_SIZE, in);
  fclose(in);

  if (size == BUFF_SIZE) {
      exit(5); // BUFFER TOO SHORT !
  }

  crypt_file(argv[1], buffer, size);

  printf("[+] DONE.\n");
  return EXIT_SUCCESS;
}
