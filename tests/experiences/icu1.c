#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unicode/unorm2.h>
#include <unicode/ustring.h>

int main (int argc, char ** argv) {
  const char * cstring = "@#æßŋđ[{“»«”¢œþĸ#½þ¶ßæ«ðđŋ“âsâsdfg âsfd^asdf^asd^^^asdfâŝd-caaaå0aaāāaaȧȧ";
  char * cstring2 = NULL;
  UChar * string = NULL;
  UChar * string2 = NULL;
  int32_t dest = 0;
  UErrorCode errCode = U_ZERO_ERROR;
  UErrorCode errCodeInst = U_ZERO_ERROR;
  int32_t norm = 0;
  const UNormalizer2 * normalizer = unorm2_getNFCInstance(&errCodeInst);

    
  u_strFromUTF8(string, dest, &dest, cstring, strlen(cstring), &errCode);
  if (errCode != U_BUFFER_OVERFLOW_ERROR) {
    fprintf(stderr, "Error 1: %s \n", u_errorName(errCode));
    exit(-1);
  }
  string = malloc(sizeof(*string) * dest+1);
  if (string == NULL) { fprintf(stderr, "Error 2\n"); exit(-1); }
  errCode = U_ZERO_ERROR;
  u_strFromUTF8(string, dest+1, &dest, cstring, strlen(cstring), &errCode);
  if (U_FAILURE(errCode)) {
    free(string);
    fprintf(stderr, "Error 3: %s\n", u_errorName(errCode));
    exit(-1);
  }
  


  errCode = U_ZERO_ERROR;
  norm = unorm2_normalize(normalizer, string, u_strlen(string), string2, norm, &errCode);
  if (errCode != U_BUFFER_OVERFLOW_ERROR) {
    free(string);
    fprintf(stderr, "Error B.1: %s\n", u_errorName(errCode));
    exit(-1);
  }
  string2 = malloc(sizeof(*string2) * norm+1);
  if (string2 == NULL) {
    free(string);
    fprintf(stderr, "Error B.2\n");
    exit(-1);
  }
  errCode = U_ZERO_ERROR;
  norm = unorm2_normalize(normalizer, string, u_strlen(string), string2, norm+1, &errCode);
  if (U_FAILURE(errCode)) {
    free(string);
    free(string2);
    fprintf(stderr, "Error B.3: %s\n", u_errorName(errCode));
    exit(-1);
  }
    
  
  dest = 0;
  errCode = U_ZERO_ERROR;
  u_strToUTF8(cstring2, dest, &dest, string2, -1, &errCode);
  if (errCode != U_BUFFER_OVERFLOW_ERROR) {
    free(string);
    free(string2);
    fprintf(stderr, "Error 4: %s\n", u_errorName(errCode));
    exit(-1);
  }
  cstring2 = malloc(sizeof(*cstring2) * dest);
  if (cstring2 == NULL) { free(string); fprintf(stderr, "Error 5\n"); exit(-1); }
  errCode = U_ZERO_ERROR;
  u_strToUTF8(cstring2, dest, &dest, string2, -1, &errCode);
  if (U_FAILURE(errCode)) {
    free(cstring2);
    free(string);
    free(string2);
    fprintf(stderr, "Error 6: %s\n", u_errorName(errCode));
    exit(-1);
  }

  printf("%s\n%s\n%d\n",cstring, cstring2, strcmp(cstring2, cstring));
  free(cstring2);
  free(string);
  free(string2);  
  return 0;
}
