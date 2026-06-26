#ifndef xmlloadH
#define xmlloadH

#include "xmlnode.h"

namespace jlib {

void loadXMLnode(XMLNode node,STRING text);
void saveXMLnode(XMLNode xml,STROUT &fw);
void csaveXMLnode(XMLNode xml,STROUT &fw,string prefix);

}

#endif
