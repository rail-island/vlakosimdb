#ifndef callfnH
#define callfnH

#ifdef __cplusplus
extern "C" {
#endif

void *callfunction(void **params,void *function);
//zavola funkci function(__stdcall),preda paramrty params az do NULL,vrati jeji vysledek

#ifdef __cplusplus
}
#endif


#endif