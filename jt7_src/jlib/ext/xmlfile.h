#ifndef xmlfileH
#define xmlfileH

#include "xmlnode.h"

namespace jlib {

void loadXMLnode(XMLNode node,string text);
void saveXMLnode(XMLNode xml,STROUT &fw);

string xmlencode(string plaintext);
string htmlencode(string plaintext);
string xmldecode(string xmltext);
string htmldecode(string htmltext);

}

#endif
