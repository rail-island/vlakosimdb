#include "callfn.h"

void __stdcall neco(void* name){}

void *callfunction(void **params,void *function) {
/*  while (*params) {
    neco(*params);
    params++;
  }*/
  void *res;
//  asm ("jmp label @1: mov eax,params push dword ptr [eax] add params,0x04 @2:mov edx,params cmp dword ptr [edx],0x00 jnz @1n   mov eax,function call dword ptr [eax]    mov res,eax ");

  #pragma inline
  __asm {
    jmp label;
    begin:
    mov eax,params
    push dword ptr [eax]
    add params,0x04

    label:
    mov edx,params
    cmp dword ptr [edx],0x00
    jnz begin

    mov eax,function
    call dword ptr [eax]
    mov res,eax
  }
  return res;
}


