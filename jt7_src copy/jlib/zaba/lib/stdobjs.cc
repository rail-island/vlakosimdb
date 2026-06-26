bool Number_op_add_1_Number(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbNumber *right=dynamic_cast<zbNumber*>(params[0]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:Number.op:add,expected Number,found %s",typeid(*params[0]).name()));return;}
 
 

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.op:add");return;}
  
zbObject *res=createNumber(thiz->op_add(right->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_op_sub_1_Number(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbNumber *right=dynamic_cast<zbNumber*>(params[0]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:Number.op:sub,expected Number,found %s",typeid(*params[0]).name()));return;}
 
 

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.op:sub");return;}
  
zbObject *res=createNumber(thiz->op_sub(right->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_op_mul_1_Number(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbNumber *right=dynamic_cast<zbNumber*>(params[0]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:Number.op:mul,expected Number,found %s",typeid(*params[0]).name()));return;}
 
 

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.op:mul");return;}
  
zbObject *res=createNumber(thiz->op_mul(right->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_op_div_1_Number(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbNumber *right=dynamic_cast<zbNumber*>(params[0]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:Number.op:div,expected Number,found %s",typeid(*params[0]).name()));return;}
 
 

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.op:div");return;}
  
zbObject *res=createNumber(thiz->op_div(right->cvalue(),ir),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_op_mod_1_Number(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbNumber *right=dynamic_cast<zbNumber*>(params[0]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:Number.op:mod,expected Number,found %s",typeid(*params[0]).name()));return;}
 
 

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.op:mod");return;}
  
zbObject *res=createNumber(thiz->op_mod(right->cvalue(),ir),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_op_eq_1_Number(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbNumber *right=dynamic_cast<zbNumber*>(params[0]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:Number.op:eq,expected Number,found %s",typeid(*params[0]).name()));return;}
 
 

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.op:eq");return;}
  
zbObject *res=createLogical(thiz->op_eq(right->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_op_neq_1_Number(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbNumber *right=dynamic_cast<zbNumber*>(params[0]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:Number.op:neq,expected Number,found %s",typeid(*params[0]).name()));return;}
 
 

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.op:neq");return;}
  
zbObject *res=createLogical(thiz->op_neq(right->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_op_lt_1_Number(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbNumber *right=dynamic_cast<zbNumber*>(params[0]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:Number.op:lt,expected Number,found %s",typeid(*params[0]).name()));return;}
 
 

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.op:lt");return;}
  
zbObject *res=createLogical(thiz->op_lt(right->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_op_gt_1_Number(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbNumber *right=dynamic_cast<zbNumber*>(params[0]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:Number.op:gt,expected Number,found %s",typeid(*params[0]).name()));return;}
 
 

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.op:gt");return;}
  
zbObject *res=createLogical(thiz->op_gt(right->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_op_let_1_Number(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbNumber *right=dynamic_cast<zbNumber*>(params[0]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:Number.op:let,expected Number,found %s",typeid(*params[0]).name()));return;}
 
 

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.op:let");return;}
  
zbObject *res=createLogical(thiz->op_let(right->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_op_get_1_Number(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbNumber *right=dynamic_cast<zbNumber*>(params[0]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:Number.op:get,expected Number,found %s",typeid(*params[0]).name()));return;}
 
 

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.op:get");return;}
  
zbObject *res=createLogical(thiz->op_get(right->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_op_inc_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.op:inc");return;}
  
zbObject *res=createNumber(thiz->op_inc(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_op_dec_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.op:dec");return;}
  
zbObject *res=createNumber(thiz->op_dec(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_op_minus_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.op:minus");return;}
  
zbObject *res=createNumber(thiz->op_minus(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_op_new_1_(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *n=dynamic_cast<zbObject*>(params[0]);
  if (! n) return false;// {ir->error(fmtstr("ArgError:Number.op:new,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 
zbObject *res=newnumber(n,ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_sin_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.sin");return;}
  
zbObject *res=createNumber(thiz->sin(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_cos_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.cos");return;}
  
zbObject *res=createNumber(thiz->cos(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_tan_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.tan");return;}
  
zbObject *res=createNumber(thiz->tan(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_asin_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.asin");return;}
  
zbObject *res=createNumber(thiz->asin(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_acos_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.acos");return;}
  
zbObject *res=createNumber(thiz->acos(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_atan_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.atan");return;}
  
zbObject *res=createNumber(thiz->atan(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_log_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.log");return;}
  
zbObject *res=createNumber(thiz->log(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_exp_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.exp");return;}
  
zbObject *res=createNumber(thiz->exp(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_power_1_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *exponent=params[0];
 
 

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.power");return;}
  
zbObject *res=createNumber(thiz->power(exponent->tofloat()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_abs_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.abs");return;}
  
zbObject *res=createNumber(thiz->abs(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_sgn_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.sgn");return;}
  
zbObject *res=createNumber(thiz->sgn(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Number_round_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbNumber *thiz=dynamic_cast<zbNumber*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Number.round");return;}
  
zbObject *res=createNumber(thiz->round(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Number_rand_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=createNumber(frand01(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Number_rand_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *range=params[0];
 
 
zbObject *res=createNumber(rangerand(range->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Number_compare_2_float_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value1=params[0];
 
 

   zbObject *value2=params[1];
 
 
zbObject *res=createNumber(cmpnums(value1->tofloat(),value2->tofloat()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Math_get_PI_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=createNumber(getpi(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Math_get_E_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=createNumber(gete(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Math_min_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>65535) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=createNumber(minimum(params+0,parcnt-0),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Math_max_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>65535) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=createNumber(maximum(params+0,parcnt-0),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MultiIndex_op_get_index_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbMultiIndex *thiz=dynamic_cast<zbMultiIndex*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MultiIndex.op:get:index");return;}
  
zbObject *res=createNumber(thiz->opgetindex(index->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MultiIndex_op_set_index_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

   zbObject *right=params[1];
 
 

    zbMultiIndex *thiz=dynamic_cast<zbMultiIndex*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MultiIndex.op:set:index");return;}
  
zbObject *res=createNumber(thiz->opsetindex(index->toint(),right->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MultiIndex_reverse_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMultiIndex *thiz=dynamic_cast<zbMultiIndex*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MultiIndex.reverse");return;}
  
zbObject *res=createVoid(ir->rt);thiz->reverse();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MultiIndex_add_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *what=params[0];
 
 

    zbMultiIndex *thiz=dynamic_cast<zbMultiIndex*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MultiIndex.add");return;}
  
zbObject *res=createVoid(ir->rt);thiz->add(what->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MultiIndex_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newmultiindex(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MultiIndex_op_new_1_Array(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbArray *a=dynamic_cast<zbArray*>(params[0]);
  if (! a) return false;// {ir->error(fmtstr("ArgError:MultiIndex.op:new,expected Array,found %s",typeid(*params[0]).name()));return;}
 
 
zbObject *res=newmultiindex(a,ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MultiIndex_toArray_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMultiIndex *thiz=dynamic_cast<zbMultiIndex*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MultiIndex.toArray");return;}
  
zbObject *res=thiz->toArray()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MultiIndex_clone_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMultiIndex *thiz=dynamic_cast<zbMultiIndex*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MultiIndex.clone");return;}
  
zbObject *res=thiz->clone()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CharMap_get_source_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbCharMap *thiz=dynamic_cast<zbCharMap*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CharMap.get:source");return;}
  
zbObject *res=createString(thiz->getsource(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CharMap_set_source_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbCharMap *thiz=dynamic_cast<zbCharMap*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CharMap.set:source");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setsource(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CharMap_get_target_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbCharMap *thiz=dynamic_cast<zbCharMap*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CharMap.get:target");return;}
  
zbObject *res=createString(thiz->gettarget(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CharMap_set_target_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbCharMap *thiz=dynamic_cast<zbCharMap*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CharMap.set:target");return;}
  
zbObject *res=createVoid(ir->rt);thiz->settarget(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CharMap_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newcharmap(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_op_add_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *right=params[0];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.op:add");return;}
  
zbObject *res=createString(thiz->op_add(right->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_op_eq_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *right=params[0];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.op:eq");return;}
  
zbObject *res=createLogical(thiz->op_eq(right->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_op_neq_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *right=params[0];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.op:neq");return;}
  
zbObject *res=createLogical(thiz->op_neq(right->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_op_lt_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *right=params[0];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.op:lt");return;}
  
zbObject *res=createLogical(thiz->op_lt(right->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_op_gt_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *right=params[0];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.op:gt");return;}
  
zbObject *res=createLogical(thiz->op_gt(right->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_op_let_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *right=params[0];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.op:let");return;}
  
zbObject *res=createLogical(thiz->op_let(right->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_op_get_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *right=params[0];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.op:get");return;}
  
zbObject *res=createLogical(thiz->op_get(right->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_op_new_1_Object(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *obj=dynamic_cast<zbObject*>(params[0]);
  if (! obj) return false;// {ir->error(fmtstr("ArgError:String.op:new,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 
zbObject *res=newstring(obj,ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_get_length_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.get:length");return;}
  
zbObject *res=createNumber(thiz->length(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_op_get_index_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.op:get:index");return;}
  
zbObject *res=createString(thiz->charAt(index->toint(),ir),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_beginsWith_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *s=params[0];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.beginsWith");return;}
  
zbObject *res=createLogical(thiz->beginsWith(s->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_endsWith_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *s=params[0];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.endsWith");return;}
  
zbObject *res=createLogical(thiz->endsWith(s->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_charAt_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.charAt");return;}
  
zbObject *res=createString(thiz->charAt(index->toint(),ir),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_substr_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *from=params[0];
 
 

   zbObject *count=params[1];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.substr");return;}
  
zbObject *res=createString(thiz->substr(from->toint(),count->toint(),ir),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_substring_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *from=params[0];
 
 

   zbObject *end=params[1];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.substring");return;}
  
zbObject *res=createString(thiz->substring(from->toint(),end->toint(),ir),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_toUpperCase_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.toUpperCase");return;}
  
zbObject *res=createString(thiz->toUpperCase(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_toLowerCase_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.toLowerCase");return;}
  
zbObject *res=createString(thiz->toLowerCase(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_trim_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.trim");return;}
  
zbObject *res=createString(thiz->trim(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_indexOf_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *substr=params[0];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.indexOf");return;}
  
zbObject *res=createNumber(thiz->indexOf(substr->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_mapIndexOf_2_string_CharMap(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *substr=params[0];
 
 
zbCharMap *map=dynamic_cast<zbCharMap*>(params[1]);
  if (! map) return false;// {ir->error(fmtstr("ArgError:String.mapIndexOf,expected CharMap,found %s",typeid(*params[1]).name()));return;}
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.mapIndexOf");return;}
  
zbObject *res=createNumber(thiz->mapIndexOf(substr->tostring(),map),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_split_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *separator=params[0];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.split");return;}
  
zbObject *res=thiz->split(separator->tostring())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_save_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *file=params[0];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.save");return;}
  
zbObject *res=createVoid(ir->rt);thiz->save(file->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_mapBeginsWith_2_string_CharMap(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *s=params[0];
 
 
zbCharMap *map=dynamic_cast<zbCharMap*>(params[1]);
  if (! map) return false;// {ir->error(fmtstr("ArgError:String.mapBeginsWith,expected CharMap,found %s",typeid(*params[1]).name()));return;}
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.mapBeginsWith");return;}
  
zbObject *res=createLogical(thiz->mapBeginsWith(s->tostring(),map),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_replace_2_RegExp_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);
zbRegExp *regex=dynamic_cast<zbRegExp*>(params[0]);
  if (! regex) return false;// {ir->error(fmtstr("ArgError:String.replace,expected RegExp,found %s",typeid(*params[0]).name()));return;}
 
 

   zbObject *replace_with=params[1];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.replace");return;}
  
zbObject *res=createString(thiz->replace(regex,replace_with->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_replaceAll_2_RegExp_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);
zbRegExp *regex=dynamic_cast<zbRegExp*>(params[0]);
  if (! regex) return false;// {ir->error(fmtstr("ArgError:String.replaceAll,expected RegExp,found %s",typeid(*params[0]).name()));return;}
 
 

   zbObject *replace_with=params[1];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.replaceAll");return;}
  
zbObject *res=createString(thiz->replaceAll(regex,replace_with->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_replace_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *origin=params[0];
 
 

   zbObject *replace_with=params[1];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.replace");return;}
  
zbObject *res=createString(thiz->replace(origin->tostring(),replace_with->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_replaceAll_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *origin=params[0];
 
 

   zbObject *replace_with=params[1];
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.replaceAll");return;}
  
zbObject *res=createString(thiz->replaceAll(origin->tostring(),replace_with->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_translate_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.translate");return;}
  
zbObject *res=createString(thiz->translate(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool String_mapReplace_1_CharMap(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbCharMap *map=dynamic_cast<zbCharMap*>(params[0]);
  if (! map) return false;// {ir->error(fmtstr("ArgError:String.mapReplace,expected CharMap,found %s",typeid(*params[0]).name()));return;}
 
 

    zbString *thiz=dynamic_cast<zbString*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:String.mapReplace");return;}
  
zbObject *res=createString(thiz->mapReplace(map),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_String_compare_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value1=params[0];
 
 

   zbObject *value2=params[1];
 
 
zbObject *res=createNumber(cmpstrs(value1->tostring(),value2->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ArrayIterator_getValue_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbArrayIterator *thiz=dynamic_cast<zbArrayIterator*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ArrayIterator.getValue");return;}
  
zbObject *res=thiz->getValue()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ArrayIterator_moveNext_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbArrayIterator *thiz=dynamic_cast<zbArrayIterator*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ArrayIterator.moveNext");return;}
  
zbObject *res=thiz->moveNext()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_op_get_index_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.op:get:index");return;}
  
zbObject *res=thiz->getelem(index->toint())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_op_set_index_2_int_Object(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 
zbObject *right=dynamic_cast<zbObject*>(params[1]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:Array.op:set:index,expected Object,found %s",typeid(*params[1]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.op:set:index");return;}
  
zbObject *res=thiz->setelem(index->toint(),right,ir)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_push_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>65535) return false;
  zbObject **params=ir->getparams(parcnt);

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.push");return;}
  
zbObject *res=createVoid(ir->rt);thiz->additems(params+0,parcnt-0);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_get_length_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.get:length");return;}
  
zbObject *res=createNumber(thiz->getcount(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_get_count_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.get:count");return;}
  
zbObject *res=createNumber(thiz->getcount(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_pop_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.pop");return;}
  
zbObject *res=thiz->pop(ir)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_reverse_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.reverse");return;}
  
zbObject *res=createVoid(ir->rt);thiz->reverse();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_shift_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.shift");return;}
  
zbObject *res=thiz->shift(ir)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_join_2_InlineMethod_String(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);
zbInlineMethod *func=dynamic_cast<zbInlineMethod*>(params[0]);
  if (! func) return false;// {ir->error(fmtstr("ArgError:Array.join,expected InlineMethod,found %s",typeid(*params[0]).name()));return;}
 
 
zbString *separator=dynamic_cast<zbString*>(params[1]);
  if (! separator) return false;// {ir->error(fmtstr("ArgError:Array.join,expected String,found %s",typeid(*params[1]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.join");return;}
  
zbObject *res=thiz->join(func->cvalue(),separator->cvalue(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_join_1_String(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbString *separator=dynamic_cast<zbString*>(params[0]);
  if (! separator) return false;// {ir->error(fmtstr("ArgError:Array.join,expected String,found %s",typeid(*params[0]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.join");return;}
  
zbObject *res=thiz->join(separator->cvalue(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_sort_1_InlineMethod(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbInlineMethod *comparator=dynamic_cast<zbInlineMethod*>(params[0]);
  if (! comparator) return false;// {ir->error(fmtstr("ArgError:Array.sort,expected InlineMethod,found %s",typeid(*params[0]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.sort");return;}
  
zbObject *res=createVoid(ir->rt);thiz->inlsort(comparator,ir);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_sort_1_Object(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *comparator=dynamic_cast<zbObject*>(params[0]);
  if (! comparator) return false;// {ir->error(fmtstr("ArgError:Array.sort,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.sort");return;}
  
zbObject *res=createVoid(ir->rt);thiz->sort(comparator,ir);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_sort_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.sort");return;}
  
zbObject *res=createVoid(ir->rt);thiz->sort();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newarray(ir->rt,params+0,parcnt-0)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_erase_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

   zbObject *count=params[1];
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.erase");return;}
  
zbObject *res=createVoid(ir->rt);thiz->erase(index->toint(),count->toint(),ir);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_erase_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.erase");return;}
  
zbObject *res=createVoid(ir->rt);thiz->erase(index->toint(),ir);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_insert_2_int_(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 
zbObject *what=dynamic_cast<zbObject*>(params[1]);
  if (! what) return false;// {ir->error(fmtstr("ArgError:Array.insert,expected Object,found %s",typeid(*params[1]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.insert");return;}
  
zbObject *res=createVoid(ir->rt);thiz->insert(index->toint(),what);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_search_1_(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *what=dynamic_cast<zbObject*>(params[0]);
  if (! what) return false;// {ir->error(fmtstr("ArgError:Array.search,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.search");return;}
  
zbObject *res=createNumber(thiz->search(what),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_remove_1_(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *what=dynamic_cast<zbObject*>(params[0]);
  if (! what) return false;// {ir->error(fmtstr("ArgError:Array.remove,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.remove");return;}
  
zbObject *res=createVoid(ir->rt);thiz->remove(what);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_removeRef_1_(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *what=dynamic_cast<zbObject*>(params[0]);
  if (! what) return false;// {ir->error(fmtstr("ArgError:Array.removeRef,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.removeRef");return;}
  
zbObject *res=createVoid(ir->rt);thiz->removeRef(what);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_reOrder_1_MultiIndex(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbMultiIndex *newOrder=dynamic_cast<zbMultiIndex*>(params[0]);
  if (! newOrder) return false;// {ir->error(fmtstr("ArgError:Array.reOrder,expected MultiIndex,found %s",typeid(*params[0]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.reOrder");return;}
  
zbObject *res=createVoid(ir->rt);thiz->reOrder(newOrder,ir);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_clear_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.clear");return;}
  
zbObject *res=createVoid(ir->rt);thiz->clear();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_get_last_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.get:last");return;}
  
zbObject *res=thiz->getlast()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_getIterator_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.getIterator");return;}
  
zbObject *res=thiz->getIterator()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_clone_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.clone");return;}
  
zbObject *res=thiz->clone()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_assign_1_Array(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbArray *array=dynamic_cast<zbArray*>(params[0]);
  if (! array) return false;// {ir->error(fmtstr("ArgError:Array.assign,expected Array,found %s",typeid(*params[0]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.assign");return;}
  
zbObject *res=createVoid(ir->rt);thiz->assign(array);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Array_sequence_3_float_float_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *low=params[0];
 
 

   zbObject *high=params[1];
 
 

   zbObject *step=params[2];
 
 
zbObject *res=newsequence(low->tofloat(),high->tofloat(),step->tofloat(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_makeInts_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.makeInts");return;}
  
zbObject *res=createVoid(ir->rt);thiz->makeInts();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_makeFloats_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.makeFloats");return;}
  
zbObject *res=createVoid(ir->rt);thiz->makeFloats();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_makeStrings_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.makeStrings");return;}
  
zbObject *res=createVoid(ir->rt);thiz->makeStrings();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_makeBools_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.makeBools");return;}
  
zbObject *res=createVoid(ir->rt);thiz->makeBools();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_searchIf_1_(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *condition=dynamic_cast<zbObject*>(params[0]);
  if (! condition) return false;// {ir->error(fmtstr("ArgError:Array.searchIf,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.searchIf");return;}
  
zbObject *res=thiz->searchIf(condition)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_searchIfIndex_1_(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *condition=dynamic_cast<zbObject*>(params[0]);
  if (! condition) return false;// {ir->error(fmtstr("ArgError:Array.searchIfIndex,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.searchIfIndex");return;}
  
zbObject *res=thiz->searchIfIndex(condition)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_searchIfValue_1_(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *condition=dynamic_cast<zbObject*>(params[0]);
  if (! condition) return false;// {ir->error(fmtstr("ArgError:Array.searchIfValue,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.searchIfValue");return;}
  
zbObject *res=thiz->searchIfValue(condition)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_searchRef_1_(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *ref=dynamic_cast<zbObject*>(params[0]);
  if (! ref) return false;// {ir->error(fmtstr("ArgError:Array.searchRef,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.searchRef");return;}
  
zbObject *res=createNumber(thiz->searchRef(ref),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_searchInt_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *num=params[0];
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.searchInt");return;}
  
zbObject *res=createNumber(thiz->searchInt(num->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_searchString_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *str=params[0];
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.searchString");return;}
  
zbObject *res=createNumber(thiz->searchString(str->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_searchFloat_1_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *num=params[0];
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.searchFloat");return;}
  
zbObject *res=createNumber(thiz->searchFloat(num->tofloat()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_searchExtrem_2_InlineMethod_InlineMethod(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);
zbInlineMethod *extrem=dynamic_cast<zbInlineMethod*>(params[0]);
  if (! extrem) return false;// {ir->error(fmtstr("ArgError:Array.searchExtrem,expected InlineMethod,found %s",typeid(*params[0]).name()));return;}
 
 
zbInlineMethod *forall=dynamic_cast<zbInlineMethod*>(params[1]);
  if (! forall) return false;// {ir->error(fmtstr("ArgError:Array.searchExtrem,expected InlineMethod,found %s",typeid(*params[1]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.searchExtrem");return;}
  
zbObject *res=thiz->searchExtrem(extrem,forall)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_collect_1_InlineMethod(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbInlineMethod *condition=dynamic_cast<zbInlineMethod*>(params[0]);
  if (! condition) return false;// {ir->error(fmtstr("ArgError:Array.collect,expected InlineMethod,found %s",typeid(*params[0]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.collect");return;}
  
zbObject *res=thiz->collect(condition)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_convert_1_InlineMethod(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbInlineMethod *convertion=dynamic_cast<zbInlineMethod*>(params[0]);
  if (! convertion) return false;// {ir->error(fmtstr("ArgError:Array.convert,expected InlineMethod,found %s",typeid(*params[0]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.convert");return;}
  
zbObject *res=thiz->convert(convertion)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_addArray_1_Array(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbArray *array=dynamic_cast<zbArray*>(params[0]);
  if (! array) return false;// {ir->error(fmtstr("ArgError:Array.addArray,expected Array,found %s",typeid(*params[0]).name()));return;}
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.addArray");return;}
  
zbObject *res=createVoid(ir->rt);thiz->addArray(array);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_exchange_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *idnex1=params[0];
 
 

   zbObject *index2=params[1];
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.exchange");return;}
  
zbObject *res=createVoid(ir->rt);thiz->exchange(idnex1->toint(),index2->toint(),ir);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_moveUp_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.moveUp");return;}
  
zbObject *res=createLogical(thiz->moveUp(index->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_moveDown_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.moveDown");return;}
  
zbObject *res=createLogical(thiz->moveDown(index->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Array_segment_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *from=params[0];
 
 

   zbObject *len=params[1];
 
 

    zbArray *thiz=dynamic_cast<zbArray*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Array.segment");return;}
  
zbObject *res=thiz->segment(from->toint(),len->toint())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Matrix_op_get_index_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>65535) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMatrix *thiz=dynamic_cast<zbMatrix*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Matrix.op:get:index");return;}
  
zbObject *res=thiz->opgetindex(ir,params+0,parcnt-0)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Matrix_op_set_index_1_Object(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>65535) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *right=dynamic_cast<zbObject*>(params[0]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:Matrix.op:set:index,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbMatrix *thiz=dynamic_cast<zbMatrix*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Matrix.op:set:index");return;}
  
zbObject *res=thiz->opsetindex(params,parcnt,ir)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Matrix_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>65535) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newmatrix(ir->rt,params+0,parcnt-0)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Matrix_fill_1_(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *value=dynamic_cast<zbObject*>(params[0]);
  if (! value) return false;// {ir->error(fmtstr("ArgError:Matrix.fill,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbMatrix *thiz=dynamic_cast<zbMatrix*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Matrix.fill");return;}
  
zbObject *res=createVoid(ir->rt);thiz->fill(value);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Matrix_search_1_(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *what=dynamic_cast<zbObject*>(params[0]);
  if (! what) return false;// {ir->error(fmtstr("ArgError:Matrix.search,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbMatrix *thiz=dynamic_cast<zbMatrix*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Matrix.search");return;}
  
zbObject *res=thiz->search(what)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Matrix_clone_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMatrix *thiz=dynamic_cast<zbMatrix*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Matrix.clone");return;}
  
zbObject *res=thiz->clone()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StringBuffer_add_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *what=params[0];
 
 

    zbStringBuffer *thiz=dynamic_cast<zbStringBuffer*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StringBuffer.add");return;}
  
zbObject *res=createVoid(ir->rt);thiz->add(what->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StringBuffer_erase_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *begin=params[0];
 
 

   zbObject *end=params[1];
 
 

    zbStringBuffer *thiz=dynamic_cast<zbStringBuffer*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StringBuffer.erase");return;}
  
zbObject *res=createVoid(ir->rt);thiz->erase(begin->toint(),end->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StringBuffer_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newstringbuf(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool InlineMethod_op_call_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>65535) return false;
  zbObject **params=ir->getparams(parcnt);

    zbInlineMethod *thiz=dynamic_cast<zbInlineMethod*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:InlineMethod.op:call");return;}
  
zbObject *res=thiz->opcall(ir,params+0,parcnt-0)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool System_createArray_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>65535) return false;
  zbObject **params=ir->getparams(parcnt);

    zbSystem *thiz=dynamic_cast<zbSystem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:System.createArray");return;}
  
zbObject *res=thiz->createarray(params+0,parcnt-0)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool System_getStackTrace_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbSystem *thiz=dynamic_cast<zbSystem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:System.getStackTrace");return;}
  
zbObject *res=createString(thiz->getStackTrace(ir),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool System_createDelegate_2_string_(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 
zbObject *object=dynamic_cast<zbObject*>(params[1]);
  if (! object) return false;// {ir->error(fmtstr("ArgError:System.createDelegate,expected Object,found %s",typeid(*params[1]).name()));return;}
 
 

    zbSystem *thiz=dynamic_cast<zbSystem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:System.createDelegate");return;}
  
zbObject *res=thiz->createDelegate(name->tostring(),object)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool System_get_interpretState_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbSystem *thiz=dynamic_cast<zbSystem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:System.get:interpretState");return;}
  
zbObject *res=thiz->getinterpretState(ir)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool System_op_get_prop_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbSystem *thiz=dynamic_cast<zbSystem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:System.op:get:prop");return;}
  
zbObject *res=thiz->opgetprop(name->tostring())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool System_op_set_prop_2_string_Object(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 
zbObject *right=dynamic_cast<zbObject*>(params[1]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:System.op:set:prop,expected Object,found %s",typeid(*params[1]).name()));return;}
 
 

    zbSystem *thiz=dynamic_cast<zbSystem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:System.op:set:prop");return;}
  
zbObject *res=createVoid(ir->rt);thiz->opsetprop(name->tostring(),right);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool System_op_get_index_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbSystem *thiz=dynamic_cast<zbSystem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:System.op:get:index");return;}
  
zbObject *res=thiz->opgetprop(name->tostring())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool System_createInlineMethod_3___string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *locals=dynamic_cast<zbObject*>(params[0]);
  if (! locals) return false;// {ir->error(fmtstr("ArgError:System.createInlineMethod,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 
zbObject *obj=dynamic_cast<zbObject*>(params[1]);
  if (! obj) return false;// {ir->error(fmtstr("ArgError:System.createInlineMethod,expected Object,found %s",typeid(*params[1]).name()));return;}
 
 

   zbObject *name=params[2];
 
 

    zbSystem *thiz=dynamic_cast<zbSystem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:System.createInlineMethod");return;}
  
zbObject *res=thiz->createInlineMethod(locals,obj,name->tostring())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool System_get_clock_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbSystem *thiz=dynamic_cast<zbSystem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:System.get:clock");return;}
  
zbObject *res=createNumber(thiz->getclock(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool System_command_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *command=params[0];
 
 

    zbSystem *thiz=dynamic_cast<zbSystem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:System.command");return;}
  
zbObject *res=createNumber(thiz->command(command->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool System_createInterruptFunction_2__string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>65535) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *obj=dynamic_cast<zbObject*>(params[0]);
  if (! obj) return false;// {ir->error(fmtstr("ArgError:System.createInterruptFunction,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

   zbObject *name=params[1];
 
 

    zbSystem *thiz=dynamic_cast<zbSystem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:System.createInterruptFunction");return;}
  
zbObject *res=thiz->createInterruptFunction(obj,name->tostring(),params+2,parcnt-2)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_get_day_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.get:day");return;}
  
zbObject *res=createNumber(thiz->getday(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_set_day_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.set:day");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setday(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_get_month_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.get:month");return;}
  
zbObject *res=createNumber(thiz->getmonth(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_set_month_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.set:month");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setmonth(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_get_year_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.get:year");return;}
  
zbObject *res=createNumber(thiz->getyear(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_set_year_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.set:year");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setyear(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_get_hours_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.get:hours");return;}
  
zbObject *res=createNumber(thiz->gethours(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_set_hours_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.set:hours");return;}
  
zbObject *res=createVoid(ir->rt);thiz->sethours(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_get_minutes_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.get:minutes");return;}
  
zbObject *res=createNumber(thiz->getminutes(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_set_minutes_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.set:minutes");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setminutes(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_get_seconds_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.get:seconds");return;}
  
zbObject *res=createNumber(thiz->getseconds(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_set_seconds_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.set:seconds");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setseconds(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_get_ticks_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.get:ticks");return;}
  
zbObject *res=createNumber(thiz->getticks(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_set_ticks_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.set:ticks");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setticks(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_get_sortString_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.get:sortString");return;}
  
zbObject *res=createString(thiz->getsortString(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_get_asString_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.get:asString");return;}
  
zbObject *res=createString(thiz->getasString(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_set_asString_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.set:asString");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setasString(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newdate(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_op_new_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 
zbObject *res=newdate(value->tostring(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Date_format_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *fmt=params[0];
 
 

    zbDate *thiz=dynamic_cast<zbDate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Date.format");return;}
  
zbObject *res=createString(thiz->format(fmt->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool File_isFile_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbFile *thiz=dynamic_cast<zbFile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:File.isFile");return;}
  
zbObject *res=createLogical(thiz->isFile(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool File_isDirectory_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbFile *thiz=dynamic_cast<zbFile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:File.isDirectory");return;}
  
zbObject *res=createLogical(thiz->isDirectory(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool File_op_div_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbFile *thiz=dynamic_cast<zbFile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:File.op:div");return;}
  
zbObject *res=createFile(thiz->opdiv(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool File_listFiles_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *dir=params[0];
 
 

    zbFile *thiz=dynamic_cast<zbFile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:File.listFiles");return;}
  
zbObject *res=thiz->listFiles(dir->tostring())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_File_get_root_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=getrootdir(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_File_list_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *mask=params[0];
 
 
zbObject *res=fulllistfiles(mask->tostring(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool File_mkdir_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbFile *thiz=dynamic_cast<zbFile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:File.mkdir");return;}
  
zbObject *res=createVoid(ir->rt);thiz->mkdir();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool File_mkdirs_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbFile *thiz=dynamic_cast<zbFile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:File.mkdirs");return;}
  
zbObject *res=createVoid(ir->rt);thiz->mkdirs();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool File_relTo_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *directory=params[0];
 
 

    zbFile *thiz=dynamic_cast<zbFile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:File.relTo");return;}
  
zbObject *res=createFile(thiz->relTo(directory->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool File_getDirectory_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbFile *thiz=dynamic_cast<zbFile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:File.getDirectory");return;}
  
zbObject *res=createFile(thiz->getDirectory(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool File_getFile_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbFile *thiz=dynamic_cast<zbFile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:File.getFile");return;}
  
zbObject *res=createFile(thiz->getFile(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool File_op_mod_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *ext=params[0];
 
 

    zbFile *thiz=dynamic_cast<zbFile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:File.op:mod");return;}
  
zbObject *res=createFile(thiz->opmod(ext->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool File_load_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbFile *thiz=dynamic_cast<zbFile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:File.load");return;}
  
zbObject *res=createString(thiz->load(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool File_copyTo_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *newname=params[0];
 
 

    zbFile *thiz=dynamic_cast<zbFile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:File.copyTo");return;}
  
zbObject *res=createVoid(ir->rt);thiz->copyTo(newname->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool File_renameTo_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *newname=params[0];
 
 

    zbFile *thiz=dynamic_cast<zbFile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:File.renameTo");return;}
  
zbObject *res=createVoid(ir->rt);thiz->renameTo(newname->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool File_hasExt_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *ext=params[0];
 
 

    zbFile *thiz=dynamic_cast<zbFile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:File.hasExt");return;}
  
zbObject *res=createLogical(thiz->hasExt(ext->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool File_remove_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbFile *thiz=dynamic_cast<zbFile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:File.remove");return;}
  
zbObject *res=createVoid(ir->rt);thiz->remove();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool File_op_new_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 
zbObject *res=newfile(name->tostring(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StoreIterator_moveNext_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStoreIterator *thiz=dynamic_cast<zbStoreIterator*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StoreIterator.moveNext");return;}
  
zbObject *res=createVoid(ir->rt);thiz->moveNext();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StoreIterator_getValue_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStoreIterator *thiz=dynamic_cast<zbStoreIterator*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StoreIterator.getValue");return;}
  
zbObject *res=thiz->getValue()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Store_op_get_prop_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbStore *thiz=dynamic_cast<zbStore*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Store.op:get:prop");return;}
  
zbObject *res=thiz->opgetprop(name->tostring())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Store_op_set_prop_2_string_Object(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 
zbObject *right=dynamic_cast<zbObject*>(params[1]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:Store.op:set:prop,expected Object,found %s",typeid(*params[1]).name()));return;}
 
 

    zbStore *thiz=dynamic_cast<zbStore*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Store.op:set:prop");return;}
  
zbObject *res=createVoid(ir->rt);thiz->opsetprop(name->tostring(),right);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Store_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newstore(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Store_op_get_index_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbStore *thiz=dynamic_cast<zbStore*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Store.op:get:index");return;}
  
zbObject *res=thiz->idxgetprop(name->tostring())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Store_op_set_index_2_string_Object(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 
zbObject *right=dynamic_cast<zbObject*>(params[1]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:Store.op:set:index,expected Object,found %s",typeid(*params[1]).name()));return;}
 
 

    zbStore *thiz=dynamic_cast<zbStore*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Store.op:set:index");return;}
  
zbObject *res=thiz->idxsetprop(name->tostring(),right)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Store_getIterator_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStore *thiz=dynamic_cast<zbStore*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Store.getIterator");return;}
  
zbObject *res=thiz->getIterator()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RefStore_op_get_index_1_(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *ref=dynamic_cast<zbObject*>(params[0]);
  if (! ref) return false;// {ir->error(fmtstr("ArgError:RefStore.op:get:index,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbRefStore *thiz=dynamic_cast<zbRefStore*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RefStore.op:get:index");return;}
  
zbObject *res=thiz->getitem(ref)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RefStore_op_set_index_2__Object(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *ref=dynamic_cast<zbObject*>(params[0]);
  if (! ref) return false;// {ir->error(fmtstr("ArgError:RefStore.op:set:index,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 
zbObject *right=dynamic_cast<zbObject*>(params[1]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:RefStore.op:set:index,expected Object,found %s",typeid(*params[1]).name()));return;}
 
 

    zbRefStore *thiz=dynamic_cast<zbRefStore*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RefStore.op:set:index");return;}
  
zbObject *res=thiz->setitem(ref,right)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RefStore_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newrefstore(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RefStore_get_keys_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbRefStore *thiz=dynamic_cast<zbRefStore*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RefStore.get:keys");return;}
  
zbObject *res=thiz->getkeys()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Exception_op_new_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *msg=params[0];
 
 
zbObject *res=newexception(msg->tostring(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool VarListIterator_moveNext_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbVarListIterator *thiz=dynamic_cast<zbVarListIterator*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:VarListIterator.moveNext");return;}
  
zbObject *res=createVoid(ir->rt);thiz->moveNext();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool VarListIterator_getValue_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbVarListIterator *thiz=dynamic_cast<zbVarListIterator*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:VarListIterator.getValue");return;}
  
zbObject *res=thiz->getValue()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool VarList_op_get_prop_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbVarList *thiz=dynamic_cast<zbVarList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:VarList.op:get:prop");return;}
  
zbObject *res=createString(thiz->opgetprop(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool VarList_op_set_prop_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

   zbObject *right=params[1];
 
 

    zbVarList *thiz=dynamic_cast<zbVarList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:VarList.op:set:prop");return;}
  
zbObject *res=createVoid(ir->rt);thiz->opsetprop(name->tostring(),right->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool VarList_getIterator_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbVarList *thiz=dynamic_cast<zbVarList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:VarList.getIterator");return;}
  
zbObject *res=thiz->getIterator()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool VarList_clear_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbVarList *thiz=dynamic_cast<zbVarList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:VarList.clear");return;}
  
zbObject *res=createVoid(ir->rt);thiz->clear();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool VarList_eraseVar_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbVarList *thiz=dynamic_cast<zbVarList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:VarList.eraseVar");return;}
  
zbObject *res=createVoid(ir->rt);thiz->eraseVar(name->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool VarList_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newvarlist(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLIterator_getValue_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbXMLIterator *thiz=dynamic_cast<zbXMLIterator*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLIterator.getValue");return;}
  
zbObject *res=thiz->getValue()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLIterator_moveNext_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbXMLIterator *thiz=dynamic_cast<zbXMLIterator*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLIterator.moveNext");return;}
  
zbObject *res=createVoid(ir->rt);thiz->moveNext();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_op_get_index_1_XMLPath(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLPath *index=dynamic_cast<zbXMLPath*>(params[0]);
  if (! index) return false;// {ir->error(fmtstr("ArgError:XMLNode.op:get:index,expected XMLPath,found %s",typeid(*params[0]).name()));return;}
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.op:get:index");return;}
  
zbObject *res=createXMLNode(thiz->getsubpath(index),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_op_get_index_1_MultiIndex(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbMultiIndex *index=dynamic_cast<zbMultiIndex*>(params[0]);
  if (! index) return false;// {ir->error(fmtstr("ArgError:XMLNode.op:get:index,expected MultiIndex,found %s",typeid(*params[0]).name()));return;}
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.op:get:index");return;}
  
zbObject *res=thiz->nodeAt(index->cvalue(),ir)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_op_get_index_1_String(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbString *index=dynamic_cast<zbString*>(params[0]);
  if (! index) return false;// {ir->error(fmtstr("ArgError:XMLNode.op:get:index,expected String,found %s",typeid(*params[0]).name()));return;}
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.op:get:index");return;}
  
zbObject *res=createXMLNode(thiz->getSub(index->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_get_name_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.get:name");return;}
  
zbObject *res=createString(thiz->getname(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_set_name_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.set:name");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setname(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_get_text_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.get:text");return;}
  
zbObject *res=createString(thiz->gettext(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_set_text_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.set:text");return;}
  
zbObject *res=createVoid(ir->rt);thiz->settext(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_op_get_index_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.op:get:index");return;}
  
zbObject *res=createXMLNode(thiz->at(index->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_getSub_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.getSub");return;}
  
zbObject *res=createXMLNode(thiz->getSub(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_at_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.at");return;}
  
zbObject *res=createXMLNode(thiz->at(index->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_add_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.add");return;}
  
zbObject *res=createXMLNode(thiz->add(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_add_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.add");return;}
  
zbObject *res=createXMLNode(thiz->add(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_forceSub_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.forceSub");return;}
  
zbObject *res=createXMLNode(thiz->forceSub(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_insert_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *pos=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.insert");return;}
  
zbObject *res=createXMLNode(thiz->insert(pos->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_eraseAt_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.eraseAt");return;}
  
zbObject *res=createVoid(ir->rt);thiz->eraseAt(index->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_eraseSub_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.eraseSub");return;}
  
zbObject *res=createVoid(ir->rt);thiz->eraseSub(name->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_assign_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *src=dynamic_cast<zbXMLNode*>(params[0]);
  if (! src) return false;// {ir->error(fmtstr("ArgError:XMLNode.assign,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.assign");return;}
  
zbObject *res=createVoid(ir->rt);thiz->assign(src->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_get_count_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.get:count");return;}
  
zbObject *res=createNumber(thiz->getcount(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_get_varCount_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.get:varCount");return;}
  
zbObject *res=createNumber(thiz->getvarCount(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_varAt_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.varAt");return;}
  
zbObject *res=createString(thiz->varAt(index->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_varNameAt_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.varNameAt");return;}
  
zbObject *res=createString(thiz->varNameAt(index->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_getVar_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.getVar");return;}
  
zbObject *res=createString(thiz->getVar(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_setVar_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

   zbObject *value=params[1];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.setVar");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setVar(name->tostring(),value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_XMLNode_load_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *file=params[0];
 
 
zbObject *res=createXMLNode(loadxml(file->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_sort_1_InlineMethod(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbInlineMethod *comparator=dynamic_cast<zbInlineMethod*>(params[0]);
  if (! comparator) return false;// {ir->error(fmtstr("ArgError:XMLNode.sort,expected InlineMethod,found %s",typeid(*params[0]).name()));return;}
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.sort");return;}
  
zbObject *res=createVoid(ir->rt);thiz->inlsort(comparator,ir);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_op_get_prop_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.op:get:prop");return;}
  
zbObject *res=createString(thiz->getVar(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_op_set_prop_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

   zbObject *right=params[1];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.op:set:prop");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setVar(name->tostring(),right->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_op_get_elem_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.op:get:elem");return;}
  
zbObject *res=createXMLNode(thiz->getSub(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newXMLNode(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_save_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *file=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.save");return;}
  
zbObject *res=createVoid(ir->rt);thiz->save(file->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_clear_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.clear");return;}
  
zbObject *res=createVoid(ir->rt);thiz->clear();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_load_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *file=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.load");return;}
  
zbObject *res=createVoid(ir->rt);thiz->load(file->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_saveVars_1_VarList(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbVarList *vars=dynamic_cast<zbVarList*>(params[0]);
  if (! vars) return false;// {ir->error(fmtstr("ArgError:XMLNode.saveVars,expected VarList,found %s",typeid(*params[0]).name()));return;}
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.saveVars");return;}
  
zbObject *res=createVoid(ir->rt);thiz->saveVars(vars);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_loadVars_1_VarList(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbVarList *vars=dynamic_cast<zbVarList*>(params[0]);
  if (! vars) return false;// {ir->error(fmtstr("ArgError:XMLNode.loadVars,expected VarList,found %s",typeid(*params[0]).name()));return;}
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.loadVars");return;}
  
zbObject *res=createVoid(ir->rt);thiz->loadVars(vars);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_clearVars_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.clearVars");return;}
  
zbObject *res=createVoid(ir->rt);thiz->clearVars();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_clearSubs_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.clearSubs");return;}
  
zbObject *res=createVoid(ir->rt);thiz->clearSubs();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_eraseVar_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.eraseVar");return;}
  
zbObject *res=createVoid(ir->rt);thiz->eraseVar(name->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_nodeAt_1_MultiIndex(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbMultiIndex *index=dynamic_cast<zbMultiIndex*>(params[0]);
  if (! index) return false;// {ir->error(fmtstr("ArgError:XMLNode.nodeAt,expected MultiIndex,found %s",typeid(*params[0]).name()));return;}
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.nodeAt");return;}
  
zbObject *res=thiz->nodeAt(index->cvalue(),ir)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_op_eq_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *right=dynamic_cast<zbXMLNode*>(params[0]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:XMLNode.op:eq,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.op:eq");return;}
  
zbObject *res=createLogical(thiz->opeq(right),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_op_neq_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *right=dynamic_cast<zbXMLNode*>(params[0]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:XMLNode.op:neq,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.op:neq");return;}
  
zbObject *res=createLogical(thiz->opneq(right),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_equalTo_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[0]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:XMLNode.equalTo,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.equalTo");return;}
  
zbObject *res=createLogical(thiz->equalTo(xml->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_weekEqualTo_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[0]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:XMLNode.weekEqualTo,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.weekEqualTo");return;}
  
zbObject *res=createLogical(thiz->weekEqualTo(xml->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_isVar_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.isVar");return;}
  
zbObject *res=createLogical(thiz->isVar(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_isSub_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.isSub");return;}
  
zbObject *res=createLogical(thiz->isSub(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_get_parent_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.get:parent");return;}
  
zbObject *res=thiz->getparent()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_remove_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.remove");return;}
  
zbObject *res=createVoid(ir->rt);thiz->remove();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_get_childs_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.get:childs");return;}
  
zbObject *res=thiz->getchilds()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_get_varNames_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.get:varNames");return;}
  
zbObject *res=thiz->getvarNames()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_exists_1_XMLPath(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLPath *path=dynamic_cast<zbXMLPath*>(params[0]);
  if (! path) return false;// {ir->error(fmtstr("ArgError:XMLNode.exists,expected XMLPath,found %s",typeid(*params[0]).name()));return;}
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.exists");return;}
  
zbObject *res=createLogical(thiz->exists(path),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_exchange_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index1=params[0];
 
 

   zbObject *index2=params[1];
 
 

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.exchange");return;}
  
zbObject *res=createLogical(thiz->exchange(index1->toint(),index2->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_getIterator_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.getIterator");return;}
  
zbObject *res=thiz->getIterator()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLNode_clone_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbXMLNode *thiz=dynamic_cast<zbXMLNode*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:XMLNode.clone");return;}
  
zbObject *res=thiz->clone()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool XMLPath_op_new_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *path=params[0];
 
 
zbObject *res=newxmlpath(path->tostring(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Delegate_op_call_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>65535) return false;
  zbObject **params=ir->getparams(parcnt);

    zbDelegate *thiz=dynamic_cast<zbDelegate*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Delegate.op:call");return;}
  
zbObject *res=thiz->opcall(ir,params+0,parcnt-0)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool QuickSorter_push_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>65535) return false;
  zbObject **params=ir->getparams(parcnt);

    zbQuickSorter *thiz=dynamic_cast<zbQuickSorter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:QuickSorter.push");return;}
  
zbObject *res=createVoid(ir->rt);thiz->push(params+0,parcnt-0);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool QuickSorter_sort_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbQuickSorter *thiz=dynamic_cast<zbQuickSorter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:QuickSorter.sort");return;}
  
zbObject *res=thiz->sort()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool QuickSorter_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newquicksorter(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CharMixer_compileFile_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *file=params[0];
 
 
try {
    zbCharMixer *thiz=dynamic_cast<zbCharMixer*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CharMixer.compileFile");return;}
  
zbObject *res=createVoid(ir->rt);thiz->compileFile(file->tostring(),ir);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  } catch(JException &e) {
    ir->nativeerror(e.getmsg());
    return true;
  }
  
  return true;

}
bool CharMixer_run_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
try {
    zbCharMixer *thiz=dynamic_cast<zbCharMixer*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CharMixer.run");return;}
  
zbObject *res=createString(thiz->run(ir),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  } catch(JException &e) {
    ir->nativeerror(e.getmsg());
    return true;
  }
  
  return true;

}
bool CharMixer_get_globals_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbCharMixer *thiz=dynamic_cast<zbCharMixer*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CharMixer.get:globals");return;}
  
zbObject *res=thiz->getglobals()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CharMixer_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newcharmixer(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CharMixer_op_get_prop_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbCharMixer *thiz=dynamic_cast<zbCharMixer*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CharMixer.op:get:prop");return;}
  
zbObject *res=thiz->getvar(name->tostring())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CharMixer_op_set_prop_2_string_Object(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 
zbObject *right=dynamic_cast<zbObject*>(params[1]);
  if (! right) return false;// {ir->error(fmtstr("ArgError:CharMixer.op:set:prop,expected Object,found %s",typeid(*params[1]).name()));return;}
 
 

    zbCharMixer *thiz=dynamic_cast<zbCharMixer*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CharMixer.op:set:prop");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setvar(name->tostring(),right);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CharMixer_hasSub_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbCharMixer *thiz=dynamic_cast<zbCharMixer*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CharMixer.hasSub");return;}
  
zbObject *res=createLogical(thiz->hasSub(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CharMixer_callSub_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbCharMixer *thiz=dynamic_cast<zbCharMixer*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CharMixer.callSub");return;}
  
zbObject *res=createString(thiz->callSub(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RegExp_op_new_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *regex=params[0];
 
 
zbObject *res=newregexp(regex->tostring(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RegExp_match_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *text=params[0];
 
 

    zbRegExp *thiz=dynamic_cast<zbRegExp*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RegExp.match");return;}
  
zbObject *res=thiz->match(text->tostring())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RegExp_op_mul_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *text=params[0];
 
 

    zbRegExp *thiz=dynamic_cast<zbRegExp*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RegExp.op:mul");return;}
  
zbObject *res=thiz->match(text->tostring())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RegResult_replace_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *replace_string=params[0];
 
 

    zbRegResult *thiz=dynamic_cast<zbRegResult*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RegResult.replace");return;}
  
zbObject *res=createString(thiz->replace(replace_string->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RegResult_op_get_index_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbRegResult *thiz=dynamic_cast<zbRegResult*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RegResult.op:get:index");return;}
  
zbObject *res=createString(thiz->getstr(index->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RegResult_get_match_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbRegResult *thiz=dynamic_cast<zbRegResult*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RegResult.get:match");return;}
  
zbObject *res=createString(thiz->getmatch(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RegResult_get_text_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbRegResult *thiz=dynamic_cast<zbRegResult*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RegResult.get:text");return;}
  
zbObject *res=createString(thiz->gettext(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RegResult_begin_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbRegResult *thiz=dynamic_cast<zbRegResult*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RegResult.begin");return;}
  
zbObject *res=createNumber(thiz->begin(index->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RegResult_end_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbRegResult *thiz=dynamic_cast<zbRegResult*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RegResult.end");return;}
  
zbObject *res=createNumber(thiz->end(index->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RegResult_get_count_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbRegResult *thiz=dynamic_cast<zbRegResult*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RegResult.get:count");return;}
  
zbObject *res=createNumber(thiz->getcount(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool InterruptFunction_op_call_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>65535) return false;
  zbObject **params=ir->getparams(parcnt);

    zbInterruptFunction *thiz=dynamic_cast<zbInterruptFunction*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:InterruptFunction.op:call");return;}
  
zbObject *res=thiz->opcall()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}






void init_stdobjs_rt(Runtime &rt) {
rt.addsub("Number_op_add_1_Number",Number_op_add_1_Number);
rt.addsub("Number_op_sub_1_Number",Number_op_sub_1_Number);
rt.addsub("Number_op_mul_1_Number",Number_op_mul_1_Number);
rt.addsub("Number_op_div_1_Number",Number_op_div_1_Number);
rt.addsub("Number_op_mod_1_Number",Number_op_mod_1_Number);
rt.addsub("Number_op_eq_1_Number",Number_op_eq_1_Number);
rt.addsub("Number_op_neq_1_Number",Number_op_neq_1_Number);
rt.addsub("Number_op_lt_1_Number",Number_op_lt_1_Number);
rt.addsub("Number_op_gt_1_Number",Number_op_gt_1_Number);
rt.addsub("Number_op_let_1_Number",Number_op_let_1_Number);
rt.addsub("Number_op_get_1_Number",Number_op_get_1_Number);
rt.addsub("Number_op_inc_0",Number_op_inc_0);
rt.addsub("Number_op_dec_0",Number_op_dec_0);
rt.addsub("Number_op_minus_0",Number_op_minus_0);
rt.addsub("Number_op_new_1_",Number_op_new_1_);
rt.addsub("Number_sin_0",Number_sin_0);
rt.addsub("Number_cos_0",Number_cos_0);
rt.addsub("Number_tan_0",Number_tan_0);
rt.addsub("Number_asin_0",Number_asin_0);
rt.addsub("Number_acos_0",Number_acos_0);
rt.addsub("Number_atan_0",Number_atan_0);
rt.addsub("Number_log_0",Number_log_0);
rt.addsub("Number_exp_0",Number_exp_0);
rt.addsub("Number_power_1_float",Number_power_1_float);
rt.addsub("Number_abs_0",Number_abs_0);
rt.addsub("Number_sgn_0",Number_sgn_0);
rt.addsub("Number_round_0",Number_round_0);
rt.addsub("static_Number_rand_0",static_Number_rand_0);
rt.addsub("static_Number_rand_1_int",static_Number_rand_1_int);
rt.addsub("static_Number_compare_2_float_float",static_Number_compare_2_float_float);
rt.addsub("static_Math_get_PI_0",static_Math_get_PI_0);
rt.addsub("static_Math_get_E_0",static_Math_get_E_0);
rt.addsub("static_Math_min_0",static_Math_min_0);
rt.addsub("static_Math_max_0",static_Math_max_0);
rt.addsub("MultiIndex_op_get_index_1_int",MultiIndex_op_get_index_1_int);
rt.addsub("MultiIndex_op_set_index_2_int_int",MultiIndex_op_set_index_2_int_int);
rt.addsub("MultiIndex_reverse_0",MultiIndex_reverse_0);
rt.addsub("MultiIndex_add_1_int",MultiIndex_add_1_int);
rt.addsub("MultiIndex_op_new_0",MultiIndex_op_new_0);
rt.addsub("MultiIndex_op_new_1_Array",MultiIndex_op_new_1_Array);
rt.addsub("MultiIndex_toArray_0",MultiIndex_toArray_0);
rt.addsub("MultiIndex_clone_0",MultiIndex_clone_0);
rt.addsub("CharMap_get_source_0",CharMap_get_source_0);
rt.addsub("CharMap_set_source_1_string",CharMap_set_source_1_string);
rt.addsub("CharMap_get_target_0",CharMap_get_target_0);
rt.addsub("CharMap_set_target_1_string",CharMap_set_target_1_string);
rt.addsub("CharMap_op_new_0",CharMap_op_new_0);
rt.addsub("String_op_add_1_string",String_op_add_1_string);
rt.addsub("String_op_eq_1_string",String_op_eq_1_string);
rt.addsub("String_op_neq_1_string",String_op_neq_1_string);
rt.addsub("String_op_lt_1_string",String_op_lt_1_string);
rt.addsub("String_op_gt_1_string",String_op_gt_1_string);
rt.addsub("String_op_let_1_string",String_op_let_1_string);
rt.addsub("String_op_get_1_string",String_op_get_1_string);
rt.addsub("String_op_new_1_Object",String_op_new_1_Object);
rt.addsub("String_get_length_0",String_get_length_0);
rt.addsub("String_op_get_index_1_int",String_op_get_index_1_int);
rt.addsub("String_beginsWith_1_string",String_beginsWith_1_string);
rt.addsub("String_endsWith_1_string",String_endsWith_1_string);
rt.addsub("String_charAt_1_int",String_charAt_1_int);
rt.addsub("String_substr_2_int_int",String_substr_2_int_int);
rt.addsub("String_substring_2_int_int",String_substring_2_int_int);
rt.addsub("String_toUpperCase_0",String_toUpperCase_0);
rt.addsub("String_toLowerCase_0",String_toLowerCase_0);
rt.addsub("String_trim_0",String_trim_0);
rt.addsub("String_indexOf_1_string",String_indexOf_1_string);
rt.addsub("String_mapIndexOf_2_string_CharMap",String_mapIndexOf_2_string_CharMap);
rt.addsub("String_split_1_string",String_split_1_string);
rt.addsub("String_save_1_string",String_save_1_string);
rt.addsub("String_mapBeginsWith_2_string_CharMap",String_mapBeginsWith_2_string_CharMap);
rt.addsub("String_replace_2_RegExp_string",String_replace_2_RegExp_string);
rt.addsub("String_replaceAll_2_RegExp_string",String_replaceAll_2_RegExp_string);
rt.addsub("String_replace_2_string_string",String_replace_2_string_string);
rt.addsub("String_replaceAll_2_string_string",String_replaceAll_2_string_string);
rt.addsub("String_translate_0",String_translate_0);
rt.addsub("String_mapReplace_1_CharMap",String_mapReplace_1_CharMap);
rt.addsub("static_String_compare_2_string_string",static_String_compare_2_string_string);
rt.addsub("ArrayIterator_getValue_0",ArrayIterator_getValue_0);
rt.addsub("ArrayIterator_moveNext_0",ArrayIterator_moveNext_0);
rt.addsub("Array_op_get_index_1_int",Array_op_get_index_1_int);
rt.addsub("Array_op_set_index_2_int_Object",Array_op_set_index_2_int_Object);
rt.addsub("Array_push_0",Array_push_0);
rt.addsub("Array_get_length_0",Array_get_length_0);
rt.addsub("Array_get_count_0",Array_get_count_0);
rt.addsub("Array_pop_0",Array_pop_0);
rt.addsub("Array_reverse_0",Array_reverse_0);
rt.addsub("Array_shift_0",Array_shift_0);
rt.addsub("Array_join_2_InlineMethod_String",Array_join_2_InlineMethod_String);
rt.addsub("Array_join_1_String",Array_join_1_String);
rt.addsub("Array_sort_1_InlineMethod",Array_sort_1_InlineMethod);
rt.addsub("Array_sort_1_Object",Array_sort_1_Object);
rt.addsub("Array_sort_0",Array_sort_0);
rt.addsub("Array_op_new_0",Array_op_new_0);
rt.addsub("Array_erase_2_int_int",Array_erase_2_int_int);
rt.addsub("Array_erase_1_int",Array_erase_1_int);
rt.addsub("Array_insert_2_int_",Array_insert_2_int_);
rt.addsub("Array_search_1_",Array_search_1_);
rt.addsub("Array_remove_1_",Array_remove_1_);
rt.addsub("Array_removeRef_1_",Array_removeRef_1_);
rt.addsub("Array_reOrder_1_MultiIndex",Array_reOrder_1_MultiIndex);
rt.addsub("Array_clear_0",Array_clear_0);
rt.addsub("Array_get_last_0",Array_get_last_0);
rt.addsub("Array_getIterator_0",Array_getIterator_0);
rt.addsub("Array_clone_0",Array_clone_0);
rt.addsub("Array_assign_1_Array",Array_assign_1_Array);
rt.addsub("static_Array_sequence_3_float_float_float",static_Array_sequence_3_float_float_float);
rt.addsub("Array_makeInts_0",Array_makeInts_0);
rt.addsub("Array_makeFloats_0",Array_makeFloats_0);
rt.addsub("Array_makeStrings_0",Array_makeStrings_0);
rt.addsub("Array_makeBools_0",Array_makeBools_0);
rt.addsub("Array_searchIf_1_",Array_searchIf_1_);
rt.addsub("Array_searchIfIndex_1_",Array_searchIfIndex_1_);
rt.addsub("Array_searchIfValue_1_",Array_searchIfValue_1_);
rt.addsub("Array_searchRef_1_",Array_searchRef_1_);
rt.addsub("Array_searchInt_1_int",Array_searchInt_1_int);
rt.addsub("Array_searchString_1_string",Array_searchString_1_string);
rt.addsub("Array_searchFloat_1_float",Array_searchFloat_1_float);
rt.addsub("Array_searchExtrem_2_InlineMethod_InlineMethod",Array_searchExtrem_2_InlineMethod_InlineMethod);
rt.addsub("Array_collect_1_InlineMethod",Array_collect_1_InlineMethod);
rt.addsub("Array_convert_1_InlineMethod",Array_convert_1_InlineMethod);
rt.addsub("Array_addArray_1_Array",Array_addArray_1_Array);
rt.addsub("Array_exchange_2_int_int",Array_exchange_2_int_int);
rt.addsub("Array_moveUp_1_int",Array_moveUp_1_int);
rt.addsub("Array_moveDown_1_int",Array_moveDown_1_int);
rt.addsub("Array_segment_2_int_int",Array_segment_2_int_int);
rt.addsub("Matrix_op_get_index_0",Matrix_op_get_index_0);
rt.addsub("Matrix_op_set_index_1_Object",Matrix_op_set_index_1_Object);
rt.addsub("Matrix_op_new_0",Matrix_op_new_0);
rt.addsub("Matrix_fill_1_",Matrix_fill_1_);
rt.addsub("Matrix_search_1_",Matrix_search_1_);
rt.addsub("Matrix_clone_0",Matrix_clone_0);
rt.addsub("StringBuffer_add_1_string",StringBuffer_add_1_string);
rt.addsub("StringBuffer_erase_2_int_int",StringBuffer_erase_2_int_int);
rt.addsub("StringBuffer_op_new_0",StringBuffer_op_new_0);
rt.addsub("InlineMethod_op_call_0",InlineMethod_op_call_0);
rt.addsub("System_createArray_0",System_createArray_0);
rt.addsub("System_getStackTrace_0",System_getStackTrace_0);
rt.addsub("System_createDelegate_2_string_",System_createDelegate_2_string_);
rt.addsub("System_get_interpretState_0",System_get_interpretState_0);
rt.addsub("System_op_get_prop_1_string",System_op_get_prop_1_string);
rt.addsub("System_op_set_prop_2_string_Object",System_op_set_prop_2_string_Object);
rt.addsub("System_op_get_index_1_string",System_op_get_index_1_string);
rt.addsub("System_createInlineMethod_3___string",System_createInlineMethod_3___string);
rt.addsub("System_get_clock_0",System_get_clock_0);
rt.addsub("System_command_1_string",System_command_1_string);
rt.addsub("System_createInterruptFunction_2__string",System_createInterruptFunction_2__string);
rt.addsub("Date_get_day_0",Date_get_day_0);
rt.addsub("Date_set_day_1_int",Date_set_day_1_int);
rt.addsub("Date_get_month_0",Date_get_month_0);
rt.addsub("Date_set_month_1_int",Date_set_month_1_int);
rt.addsub("Date_get_year_0",Date_get_year_0);
rt.addsub("Date_set_year_1_int",Date_set_year_1_int);
rt.addsub("Date_get_hours_0",Date_get_hours_0);
rt.addsub("Date_set_hours_1_int",Date_set_hours_1_int);
rt.addsub("Date_get_minutes_0",Date_get_minutes_0);
rt.addsub("Date_set_minutes_1_int",Date_set_minutes_1_int);
rt.addsub("Date_get_seconds_0",Date_get_seconds_0);
rt.addsub("Date_set_seconds_1_int",Date_set_seconds_1_int);
rt.addsub("Date_get_ticks_0",Date_get_ticks_0);
rt.addsub("Date_set_ticks_1_int",Date_set_ticks_1_int);
rt.addsub("Date_get_sortString_0",Date_get_sortString_0);
rt.addsub("Date_get_asString_0",Date_get_asString_0);
rt.addsub("Date_set_asString_1_string",Date_set_asString_1_string);
rt.addsub("Date_op_new_0",Date_op_new_0);
rt.addsub("Date_op_new_1_string",Date_op_new_1_string);
rt.addsub("Date_format_1_string",Date_format_1_string);
rt.addsub("File_isFile_0",File_isFile_0);
rt.addsub("File_isDirectory_0",File_isDirectory_0);
rt.addsub("File_op_div_1_string",File_op_div_1_string);
rt.addsub("File_listFiles_1_string",File_listFiles_1_string);
rt.addsub("static_File_get_root_0",static_File_get_root_0);
rt.addsub("static_File_list_1_string",static_File_list_1_string);
rt.addsub("File_mkdir_0",File_mkdir_0);
rt.addsub("File_mkdirs_0",File_mkdirs_0);
rt.addsub("File_relTo_1_string",File_relTo_1_string);
rt.addsub("File_getDirectory_0",File_getDirectory_0);
rt.addsub("File_getFile_0",File_getFile_0);
rt.addsub("File_op_mod_1_string",File_op_mod_1_string);
rt.addsub("File_load_0",File_load_0);
rt.addsub("File_copyTo_1_string",File_copyTo_1_string);
rt.addsub("File_renameTo_1_string",File_renameTo_1_string);
rt.addsub("File_hasExt_1_string",File_hasExt_1_string);
rt.addsub("File_remove_0",File_remove_0);
rt.addsub("File_op_new_1_string",File_op_new_1_string);
rt.addsub("StoreIterator_moveNext_0",StoreIterator_moveNext_0);
rt.addsub("StoreIterator_getValue_0",StoreIterator_getValue_0);
rt.addsub("Store_op_get_prop_1_string",Store_op_get_prop_1_string);
rt.addsub("Store_op_set_prop_2_string_Object",Store_op_set_prop_2_string_Object);
rt.addsub("Store_op_new_0",Store_op_new_0);
rt.addsub("Store_op_get_index_1_string",Store_op_get_index_1_string);
rt.addsub("Store_op_set_index_2_string_Object",Store_op_set_index_2_string_Object);
rt.addsub("Store_getIterator_0",Store_getIterator_0);
rt.addsub("RefStore_op_get_index_1_",RefStore_op_get_index_1_);
rt.addsub("RefStore_op_set_index_2__Object",RefStore_op_set_index_2__Object);
rt.addsub("RefStore_op_new_0",RefStore_op_new_0);
rt.addsub("RefStore_get_keys_0",RefStore_get_keys_0);
rt.addsub("Exception_op_new_1_string",Exception_op_new_1_string);
rt.addsub("VarListIterator_moveNext_0",VarListIterator_moveNext_0);
rt.addsub("VarListIterator_getValue_0",VarListIterator_getValue_0);
rt.addsub("VarList_op_get_prop_1_string",VarList_op_get_prop_1_string);
rt.addsub("VarList_op_set_prop_2_string_string",VarList_op_set_prop_2_string_string);
rt.addsub("VarList_getIterator_0",VarList_getIterator_0);
rt.addsub("VarList_clear_0",VarList_clear_0);
rt.addsub("VarList_eraseVar_1_string",VarList_eraseVar_1_string);
rt.addsub("VarList_op_new_0",VarList_op_new_0);
rt.addsub("XMLIterator_getValue_0",XMLIterator_getValue_0);
rt.addsub("XMLIterator_moveNext_0",XMLIterator_moveNext_0);
rt.addsub("XMLNode_op_get_index_1_XMLPath",XMLNode_op_get_index_1_XMLPath);
rt.addsub("XMLNode_op_get_index_1_MultiIndex",XMLNode_op_get_index_1_MultiIndex);
rt.addsub("XMLNode_op_get_index_1_String",XMLNode_op_get_index_1_String);
rt.addsub("XMLNode_get_name_0",XMLNode_get_name_0);
rt.addsub("XMLNode_set_name_1_string",XMLNode_set_name_1_string);
rt.addsub("XMLNode_get_text_0",XMLNode_get_text_0);
rt.addsub("XMLNode_set_text_1_string",XMLNode_set_text_1_string);
rt.addsub("XMLNode_op_get_index_1_int",XMLNode_op_get_index_1_int);
rt.addsub("XMLNode_getSub_1_string",XMLNode_getSub_1_string);
rt.addsub("XMLNode_at_1_int",XMLNode_at_1_int);
rt.addsub("XMLNode_add_0",XMLNode_add_0);
rt.addsub("XMLNode_add_1_string",XMLNode_add_1_string);
rt.addsub("XMLNode_forceSub_1_string",XMLNode_forceSub_1_string);
rt.addsub("XMLNode_insert_1_int",XMLNode_insert_1_int);
rt.addsub("XMLNode_eraseAt_1_int",XMLNode_eraseAt_1_int);
rt.addsub("XMLNode_eraseSub_1_string",XMLNode_eraseSub_1_string);
rt.addsub("XMLNode_assign_1_XMLNode",XMLNode_assign_1_XMLNode);
rt.addsub("XMLNode_get_count_0",XMLNode_get_count_0);
rt.addsub("XMLNode_get_varCount_0",XMLNode_get_varCount_0);
rt.addsub("XMLNode_varAt_1_int",XMLNode_varAt_1_int);
rt.addsub("XMLNode_varNameAt_1_int",XMLNode_varNameAt_1_int);
rt.addsub("XMLNode_getVar_1_string",XMLNode_getVar_1_string);
rt.addsub("XMLNode_setVar_2_string_string",XMLNode_setVar_2_string_string);
rt.addsub("static_XMLNode_load_1_string",static_XMLNode_load_1_string);
rt.addsub("XMLNode_sort_1_InlineMethod",XMLNode_sort_1_InlineMethod);
rt.addsub("XMLNode_op_get_prop_1_string",XMLNode_op_get_prop_1_string);
rt.addsub("XMLNode_op_set_prop_2_string_string",XMLNode_op_set_prop_2_string_string);
rt.addsub("XMLNode_op_get_elem_1_string",XMLNode_op_get_elem_1_string);
rt.addsub("XMLNode_op_new_0",XMLNode_op_new_0);
rt.addsub("XMLNode_save_1_string",XMLNode_save_1_string);
rt.addsub("XMLNode_clear_0",XMLNode_clear_0);
rt.addsub("XMLNode_load_1_string",XMLNode_load_1_string);
rt.addsub("XMLNode_saveVars_1_VarList",XMLNode_saveVars_1_VarList);
rt.addsub("XMLNode_loadVars_1_VarList",XMLNode_loadVars_1_VarList);
rt.addsub("XMLNode_clearVars_0",XMLNode_clearVars_0);
rt.addsub("XMLNode_clearSubs_0",XMLNode_clearSubs_0);
rt.addsub("XMLNode_eraseVar_1_string",XMLNode_eraseVar_1_string);
rt.addsub("XMLNode_nodeAt_1_MultiIndex",XMLNode_nodeAt_1_MultiIndex);
rt.addsub("XMLNode_op_eq_1_XMLNode",XMLNode_op_eq_1_XMLNode);
rt.addsub("XMLNode_op_neq_1_XMLNode",XMLNode_op_neq_1_XMLNode);
rt.addsub("XMLNode_equalTo_1_XMLNode",XMLNode_equalTo_1_XMLNode);
rt.addsub("XMLNode_weekEqualTo_1_XMLNode",XMLNode_weekEqualTo_1_XMLNode);
rt.addsub("XMLNode_isVar_1_string",XMLNode_isVar_1_string);
rt.addsub("XMLNode_isSub_1_string",XMLNode_isSub_1_string);
rt.addsub("XMLNode_get_parent_0",XMLNode_get_parent_0);
rt.addsub("XMLNode_remove_0",XMLNode_remove_0);
rt.addsub("XMLNode_get_childs_0",XMLNode_get_childs_0);
rt.addsub("XMLNode_get_varNames_0",XMLNode_get_varNames_0);
rt.addsub("XMLNode_exists_1_XMLPath",XMLNode_exists_1_XMLPath);
rt.addsub("XMLNode_exchange_2_int_int",XMLNode_exchange_2_int_int);
rt.addsub("XMLNode_getIterator_0",XMLNode_getIterator_0);
rt.addsub("XMLNode_clone_0",XMLNode_clone_0);
rt.addsub("XMLPath_op_new_1_string",XMLPath_op_new_1_string);
rt.addsub("Delegate_op_call_0",Delegate_op_call_0);
rt.addsub("QuickSorter_push_0",QuickSorter_push_0);
rt.addsub("QuickSorter_sort_0",QuickSorter_sort_0);
rt.addsub("QuickSorter_op_new_0",QuickSorter_op_new_0);
rt.addsub("CharMixer_compileFile_1_string",CharMixer_compileFile_1_string);
rt.addsub("CharMixer_run_0",CharMixer_run_0);
rt.addsub("CharMixer_get_globals_0",CharMixer_get_globals_0);
rt.addsub("CharMixer_op_new_0",CharMixer_op_new_0);
rt.addsub("CharMixer_op_get_prop_1_string",CharMixer_op_get_prop_1_string);
rt.addsub("CharMixer_op_set_prop_2_string_Object",CharMixer_op_set_prop_2_string_Object);
rt.addsub("CharMixer_hasSub_1_string",CharMixer_hasSub_1_string);
rt.addsub("CharMixer_callSub_1_string",CharMixer_callSub_1_string);
rt.addsub("RegExp_op_new_1_string",RegExp_op_new_1_string);
rt.addsub("RegExp_match_1_string",RegExp_match_1_string);
rt.addsub("RegExp_op_mul_1_string",RegExp_op_mul_1_string);
rt.addsub("RegResult_replace_1_string",RegResult_replace_1_string);
rt.addsub("RegResult_op_get_index_1_int",RegResult_op_get_index_1_int);
rt.addsub("RegResult_get_match_0",RegResult_get_match_0);
rt.addsub("RegResult_get_text_0",RegResult_get_text_0);
rt.addsub("RegResult_begin_1_int",RegResult_begin_1_int);
rt.addsub("RegResult_end_1_int",RegResult_end_1_int);
rt.addsub("RegResult_get_count_0",RegResult_get_count_0);
rt.addsub("InterruptFunction_op_call_0",InterruptFunction_op_call_0);

}

