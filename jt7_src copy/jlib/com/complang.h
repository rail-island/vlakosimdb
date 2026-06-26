#ifndef complangH
#define complangH

#ifndef EXTERNALLANG
  #define LANG_TEXT(name) language_texts_##name
  #define LANG_STR(name,str) str
  #define CREATEAPPLANG
  #define DELETEAPPLANG
  #define LANGLOADFORM(name)
  #define LANGLOADTHISFORM
#else
#endif

#define LANG_STRING(name) LANG_STR(name,language_string_##name)
#define DEF_LANG_STRING(name,value) const char language_string_##name[]=value;
#define DECLARE_LANG_STRING(name) extern const char language_string_##name[];


#endif
