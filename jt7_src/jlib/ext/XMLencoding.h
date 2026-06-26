#ifndef XMLencodingH
#define XMLencodingH

namespace jlib {

void XMLendencode(const char *s,const char *end,bool trim,STROUT &res);
void XMLencode(const char *s,bool trim,STROUT &res);
void XMLenddecode(const char *s,const char *end,bool trim,STROUT &res);
void XMLdecode(const char *s,bool trim,STROUT &res);
void XMLsavefield(STROUT &f,const char *name,const char *value,bool trim=false);
void XMLendsavefield(STROUT &f,const char *name,const char *value,const char *end,bool trim);
//zapise do f pole s koncem retezce end

}

#endif
