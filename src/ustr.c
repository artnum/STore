/**
 * \brief helper functions to get string in a we want them (unicode nfc normalized)
 */
#include "ustr.h"
#include "mem.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unicode/ustring.h>
#include <unicode/unorm2.h>

void ustr_dispose (const UStr string) {
  if (string.content != NULL) {
    mem_free(string.content);
  }
}

UStr ustr_fromUChar (const UChar * string, const size_t len) {
  UStr result = {NULL, 0, 0};
  UChar * copy = NULL;
    
  if (string == NULL) { goto fail_and_return; }
  if (len <= 0) { goto fail_and_return; }

  if ((copy = mem_alloc(sizeof(*copy) * (len + 1))) == NULL) {
    goto fail_and_return;
  }

  u_strncpy(copy, string, len);
  copy[len] = '\0';

  result.content = copy;
  result.length = (int32_t)len + 1;
  return result;
  
 fail_and_return:
  if (copy != NULL) { mem_free(copy); }
  result.content = NULL;
  result.length = 0;
  return result;
}

UStr ustr_fromUTF8 (const char * string, const size_t len) {
  UStr result = {NULL, 0, 0};
  UChar * dest = NULL;
  int32_t destCapacity = 0;
  int32_t pDestLength = 0;
  UErrorCode errCode = U_ZERO_ERROR; // must be init
  
  if (string == NULL) { goto fail_and_return; }
  if (len <= 0) { goto fail_and_return; }

  /* preflight */
  u_strFromUTF8(dest, destCapacity, &pDestLength, string, (int32_t)len, &errCode);
  if (pDestLength <= 0 || errCode != U_BUFFER_OVERFLOW_ERROR) {
    goto fail_and_return;
  }
  if((dest = mem_alloc(sizeof(*dest) * (pDestLength + 1))) == NULL) {
    goto fail_and_return;
  }

  destCapacity = pDestLength;
  errCode = U_ZERO_ERROR;
  u_strFromUTF8(dest, destCapacity, &pDestLength, string, (int32_t)len, &errCode);
  if (pDestLength <= 0 || U_FAILURE(errCode)) {
    goto fail_and_return;
  }

  dest[len] = '\0';
  result.content = dest;
  result.length = pDestLength;

  return result;
  
 fail_and_return:
  if (dest != NULL) { mem_free(dest); }
  return result;
}

UStr ustr_normalize (const UStr string) {
  UStr result = {NULL, 0, 0};
  UChar * nfced = NULL;
  int32_t destLength = 0;
  UErrorCode eGetInstance = U_ZERO_ERROR;
  UErrorCode eNorm = U_ZERO_ERROR;
  const UNormalizer2 * normalizer = unorm2_getNFCInstance(&eGetInstance);

  if (U_FAILURE(eGetInstance)) {
    goto fail_and_return;
  }
  
  if (string.length == 0 || string.content == NULL) {
    goto fail_and_return;
  }

  /* preflight */
  destLength = unorm2_normalize(normalizer, string.content, string.length, nfced, destLength, &eNorm);
  if (eNorm != U_BUFFER_OVERFLOW_ERROR) {
    goto fail_and_return;
  }
  if ((nfced = mem_alloc(sizeof(*nfced) * (destLength + 1))) == NULL) {
    goto fail_and_return;
  }

  eNorm = U_ZERO_ERROR;
  destLength = unorm2_normalize(normalizer, string.content, string.length, nfced, destLength, &eNorm);
  if (U_FAILURE(eNorm)) {
    goto fail_and_return;
  }

  nfced[destLength] = '\0';
  result.content = nfced;
  result.length = destLength;
  result.nfced = 1;

  return result;

 fail_and_return:
  if (nfced != NULL) { mem_free(nfced); }
  
  result.content = NULL;
  result.length = 0;
  result.nfced = 0;
  return result; 
}
