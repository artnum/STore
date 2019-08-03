#include "ustr.h"
#include "tests.h"
#include <stdlib.h>
#include <unicode/unorm2.h>
#include <unicode/ustring.h>

const char * UTF8Strings [] = {
			       "test", "abcd", "1234",
			      "ͽ뚬8䚯糒뛾נ阃⊒sܢ蓚谘qJஓ&𝢛󯁸𠪎뇚rŖ=D򄴩μ򁁉񢅝󒥡",
			      "췃鄪o󢑾㿷쳏󞝋񦒬ᙀ䧧ѿE񋰳􆉨䥯抹ӮDֻܵBᢛ(onί抔׋ꦆ",
			      "Lￗ\⌱$׵ü䈄찴􈺙➝񇮧㡵ˈښabIڷT猆𫽕羓#<愈Ⱦ򏦮ȧ8񅋦",
			      "FŨ͏󌩎ﬧ瞼񪬽𠯵ﮘ󯱭񖈦󍕵򥈤튵򇁔_(򽼸句ǈ+򏀟3ǟ̢`򮸯ሂñ󣁘",
			      "񜅜[X䦷􈠩򇷣򻳭桼@鮳뛄よ綃'玸񃌝lɷك󚰩WÆ򧨡爍򔦶n$概2",
			      "館牕潧卵Ȝ􆳻כזó蛙F̰鳳➷񷟼⊨+呷ًɌ会𮟌۲턋Ƭ􊫑ⓗㅻ򳿱U",
			      "𢅾𾛖ˆ�ت򹿁諾󘻟씻𸟂ÏᅱᕑWMԻ僮󨊚򐗵퀯򕂭񆢅黊Ȍa⤧ә󩻉ř®ؓ񭹩",
			      "𣏰򍗩⺯񑡾/z𚷟V󜐥񣑲񓔯'ӹИ񴗽󅘵~񗒺*ꠗqȊƛ哓]e򩘈dQ湺[졝",
			      "y/3ѧ񜃾엩䐂'΢7񌀿=̪Ʈǿ沽ִ𫴆Z꥙򔴃󺨋n&&^Ԉ򓢴Rn胈",
			      "Bꢰꙡ󷥂ꅕՃ󕽋ĵ鄤欪￟U甞໊긴⶘彶ܪɂᓜ󿾢ﳀ򇲓\􇍞@ॎǨi",
			      NULL
};

START_TEST(test_ustr_fromUTF8) {
  int i = 0;
  UStr str;
  UChar * test = NULL;
  UErrorCode errCode = U_ZERO_ERROR;
  int32_t length;
  while (UTF8Strings[i] != NULL) {
    str = ustr_fromUTF8(UTF8Strings[i], strlen(UTF8Strings[i]));

    test = malloc(str.length * sizeof(*test));

    test = u_strFromUTF8(test, str.length, &length, UTF8Strings[i], strlen(UTF8Strings[i]), &errCode);
    ck_assert(str.length == length);
    ck_assert(U_SUCCESS(errCode));
    ck_assert(u_strncmp(str.content, test, str.length) == 0);
    ck_assert(str.nfced == 0);
    
    free(test);
    ustr_dispose(str);
    i++;
  }
}
END_TEST

START_TEST(test_ustr_normalize) {
  int i = 0;
  UStr str1;
  UStr str2;
  UErrorCode errCode = U_ZERO_ERROR;
  while (UTF8Strings[i] != NULL) {
    str1 = ustr_fromUTF8(UTF8Strings[i], strlen(UTF8Strings[i]));
    str2 = ustr_normalize(str1);

    errCode = U_ZERO_ERROR;
    ck_assert(unorm_compare(str1.content, -1, str2.content, -1, 0, &errCode) == 0);
    ck_assert(str1.nfced == 0);
    ck_assert(str2.nfced != 0);
    
    ustr_dispose(str1);
    ustr_dispose(str2);
    i++;
  }
}
END_TEST

Suite * ustr_test_suite(void) {
  Suite * s = NULL;
  TCase * tc = NULL;

  s = suite_create("Unicode String Test");
  tc = tcase_create("Core");

  tcase_add_test(tc, test_ustr_fromUTF8);
  tcase_add_test(tc, test_ustr_normalize);
  suite_add_tcase(s, tc);
  
  return s;
}
