
struct INIFILEI {
  BACKINFO bi;
  virtual char * STDCALL getValue(const char *sect,const char *var)=0;//vrati ukazatel na hodnotu promenne nebo NULL,kdyz neex.
  virtual void STDCALL setValue(const char *sect,const char *var,const char *value)=0;//nastavi promennou var v sekci sect
};

