#ifndef USTR_H__
#define USTR_H__

#include <unicode/utypes.h>

/**
 * Contains UChar * (from libicu) and its length. Use it as opaque structure.
 */
typedef struct _s_UStr {
  UChar * content; /**< A valid UChar */
  int32_t length; /**< its length */
  int nfced; /**< set to 0 if not normalized (NFC) and !0 if has been */
} UStr;

/**
 * \fn void ustr_dispose (UStr string)
 * \brief Free allocated resources for this UStr
 *
 * \param[in] string String to be disposed
 */
void ustr_dispose (const UStr string);

/**
 * \fn UStr ustr_fromUTF8 (const char * string, size_t len)
 * \brief Convert a C String into a UStr
 *
 * \param[in] string Pointer to an input string, must be not NULL
 * \param[in] len Length of the input string, must be bigger than 0
 * \return an UStr
 */
UStr ustr_fromUTF8 (const char * string, const size_t len);

/**
 * \fn UStr ustr_normalize (const UStr string)
 * \brief Return a new normalized (unicode NFC) UStr
 * 
 * \param[in] string An Ustr to be normalized
 * \return a normalized UStr
 */
UStr ustr_normalize (const UStr string);

#endif /* USTR_H__ */
