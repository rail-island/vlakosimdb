#ifndef makecodeH
#define makecodeH

namespace jlib {

namespace codesprivate {
inline int incp(register int z)
{
  if (z=='_') return '0';
  if (z=='9') return 'A';
  if (z=='Z') return '0';
  if (z>='A' && z<'Z') return z+1;
  if (z>='0' && z<'9') return z+1;
//  throw std::logicerror("REPAIRCODE::incp(0)");
}

template<class ParamT,bool(*iscode)(char *code,ParamT param)> static
bool probehni(ParamT param,char *cbeg,char *cact,char *cafterend)
//vrati true,pokud nalezlo pouzitelny kod,vola se rekurzivne
{
  if (cact>=cafterend) return not iscode(cbeg,param);//podm.zastaveni rekurze
  char act=*cact;
//  assert(act=='_' || isdigit(act) || (act>='A' && act<='Z'));
  do{
    if (probehni<ParamT,iscode>(param,cbeg,cact+1,cafterend)) return true;
    *cact=incp(*cact);
  } while (*cact!=act || (act=='_' && *cact=='0'));
}
}

template<class ParamT,bool(*iscode)(char *code,ParamT param)> static
void repaircode(ParamT param,char *code)
{
  char *e=strend(code);
//  if (e<=code) throw std::logicerror("RepairCode(0)");
  if (e[-1]=='_') codesprivate::probehni<ParamT,iscode>(param,code,code,e-1);else codesprivate::probehni<ParamT,iscode>(param,code,code,e);
}

void makecode(char *codemap,char *suggest,int partnum,...);
//vyrobi navrh kodu do suggest,parametry:napr
//makecode(mapupasc,s,3,"ahoj",3,"kamelot",3,"00",2) vyrobi do s navrh AHOKAM00

}

#endif
