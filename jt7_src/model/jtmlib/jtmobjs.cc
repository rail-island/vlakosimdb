bool ModelVariables_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newmodelvariables(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ModelVariables_setFloat_2_string_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

   zbObject *value=params[1];
 
 

    zbModelVariables *thiz=dynamic_cast<zbModelVariables*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ModelVariables.setFloat");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setvar(name->tostring(),value->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ModelVariables_setString_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

   zbObject *value=params[1];
 
 

    zbModelVariables *thiz=dynamic_cast<zbModelVariables*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ModelVariables.setString");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setvar(name->tostring(),value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ModelVariables_setGradString_4_string_string_string_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<4 || parcnt>4) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

   zbObject *from=params[1];
 
 

   zbObject *to=params[2];
 
 

   zbObject *t=params[3];
 
 

    zbModelVariables *thiz=dynamic_cast<zbModelVariables*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ModelVariables.setGradString");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setgradvar(name->tostring(),from->tostring(),to->tostring(),t->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ModelVariables_getString_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbModelVariables *thiz=dynamic_cast<zbModelVariables*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ModelVariables.getString");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->svar(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool jtObject_get_name_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbjtObject *thiz=dynamic_cast<zbjtObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:jtObject.get:name");return;}
  
zbObject *res=createString(thiz->getname(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool jtObject_get_title_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbjtObject *thiz=dynamic_cast<zbjtObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:jtObject.get:title");return;}
  
zbObject *res=createString(thiz->gettitle(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool jtObject_get_path_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbjtObject *thiz=dynamic_cast<zbjtObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:jtObject.get:path");return;}
  
zbObject *res=createString(thiz->getpath(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool jtObject_get_author_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbjtObject *thiz=dynamic_cast<zbjtObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:jtObject.get:author");return;}
  
zbObject *res=createString(thiz->getauthor(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool jtObject_get_data_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbjtObject *thiz=dynamic_cast<zbjtObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:jtObject.get:data");return;}
  
zbObject *res=thiz->getdata()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_jtObject_load_1_PersistentObject(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbPersistentObject *data=dynamic_cast<zbPersistentObject*>(params[0]);
  if (! data) return false;// {ir->error(fmtstr("ArgError:jtObject.load,expected PersistentObject,found %s",typeid(*params[0]).name()));return;}
 
 
zbObject *res=loadjtobject(data,ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool jtCab_render_2_RenderContext_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);
zbRenderContext *rc=dynamic_cast<zbRenderContext*>(params[0]);
  if (! rc) return false;// {ir->error(fmtstr("ArgError:jtCab.render,expected RenderContext,found %s",typeid(*params[0]).name()));return;}
 
 
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[1]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:jtCab.render,expected XMLNode,found %s",typeid(*params[1]).name()));return;}
 
 

    zbjtCab *thiz=dynamic_cast<zbjtCab*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:jtCab.render");return;}
  
zbObject *res=createVoid(ir->rt);thiz->render(rc,xml);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool jtBuilding_get_W_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbjtBuilding *thiz=dynamic_cast<zbjtBuilding*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:jtBuilding.get:W");return;}
  
zbObject *res=createNumber(thiz->getW(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool jtBuilding_get_H_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbjtBuilding *thiz=dynamic_cast<zbjtBuilding*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:jtBuilding.get:H");return;}
  
zbObject *res=createNumber(thiz->getH(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool jtSmoke_create_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbjtSmoke *thiz=dynamic_cast<zbjtSmoke*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:jtSmoke.create");return;}
  
zbObject *res=thiz->create()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SmokeInstance_render_1_RenderContext(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbRenderContext *rc=dynamic_cast<zbRenderContext*>(params[0]);
  if (! rc) return false;// {ir->error(fmtstr("ArgError:SmokeInstance.render,expected RenderContext,found %s",typeid(*params[0]).name()));return;}
 
 

    zbSmokeInstance *thiz=dynamic_cast<zbSmokeInstance*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SmokeInstance.render");return;}
  
zbObject *res=createVoid(ir->rt);thiz->render(rc);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SmokeInstance_iterate_7_float_float_float_float_float_float_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<7 || parcnt>7) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *deltatime=params[0];
 
 

   zbObject *x=params[1];
 
 

   zbObject *y=params[2];
 
 

   zbObject *z=params[3];
 
 

   zbObject *vx=params[4];
 
 

   zbObject *vy=params[5];
 
 

   zbObject *vz=params[6];
 
 

    zbSmokeInstance *thiz=dynamic_cast<zbSmokeInstance*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SmokeInstance.iterate");return;}
  
zbObject *res=createVoid(ir->rt);thiz->iterate(deltatime->tofloat(),x->tofloat(),y->tofloat(),z->tofloat(),vx->tofloat(),vy->tofloat(),vz->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SmokeInstance_translate_3_float_float_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *z=params[2];
 
 

    zbSmokeInstance *thiz=dynamic_cast<zbSmokeInstance*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SmokeInstance.translate");return;}
  
zbObject *res=createVoid(ir->rt);thiz->translate(x->tofloat(),y->tofloat(),z->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool jtStation_complete_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *trc=params[0];
 
 

   zbObject *trl=params[1];
 
 

    zbjtStation *thiz=dynamic_cast<zbjtStation*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:jtStation.complete");return;}
  
zbObject *res=thiz->complete(trc->toint(),trl->toint())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool jtBridge_complete_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *length=params[0];
 
 

    zbjtBridge *thiz=dynamic_cast<zbjtBridge*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:jtBridge.complete");return;}
  
zbObject *res=thiz->complete(length->toint())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StationInstance_render_5_float_float_float_RenderContext_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *z=params[2];
 
 
zbRenderContext *rc=dynamic_cast<zbRenderContext*>(params[3]);
  if (! rc) return false;// {ir->error(fmtstr("ArgError:StationInstance.render,expected RenderContext,found %s",typeid(*params[3]).name()));return;}
 
 

   zbObject *dir=params[4];
 
 

    zbStationInstance *thiz=dynamic_cast<zbStationInstance*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StationInstance.render");return;}
  
zbObject *res=createVoid(ir->rt);thiz->render(x->tofloat(),y->tofloat(),z->tofloat(),rc,dir->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool BridgeInstance_render_5_float_float_float_RenderContext_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *z=params[2];
 
 
zbRenderContext *rc=dynamic_cast<zbRenderContext*>(params[3]);
  if (! rc) return false;// {ir->error(fmtstr("ArgError:BridgeInstance.render,expected RenderContext,found %s",typeid(*params[3]).name()));return;}
 
 

   zbObject *dir=params[4];
 
 

    zbBridgeInstance *thiz=dynamic_cast<zbBridgeInstance*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:BridgeInstance.render");return;}
  
zbObject *res=createVoid(ir->rt);thiz->render(x->tofloat(),y->tofloat(),z->tofloat(),rc,dir->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool jtMachine_get_maxSpeed_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbjtMachine *thiz=dynamic_cast<zbjtMachine*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:jtMachine.get:maxSpeed");return;}
  
zbObject *res=createNumber(thiz->getmaxSpeed(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool jtMachine_get_weight_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbjtMachine *thiz=dynamic_cast<zbjtMachine*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:jtMachine.get:weight");return;}
  
zbObject *res=createNumber(thiz->getweight(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool jtMachine_get_power_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbjtMachine *thiz=dynamic_cast<zbjtMachine*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:jtMachine.get:power");return;}
  
zbObject *res=createNumber(thiz->getpower(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool jtMotocar_get_maxSpeed_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbjtMotocar *thiz=dynamic_cast<zbjtMotocar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:jtMotocar.get:maxSpeed");return;}
  
zbObject *res=createNumber(thiz->getmaxSpeed(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool jtMotocar_get_weight_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbjtMotocar *thiz=dynamic_cast<zbjtMotocar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:jtMotocar.get:weight");return;}
  
zbObject *res=createNumber(thiz->getweight(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool jtMotocar_get_power_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbjtMotocar *thiz=dynamic_cast<zbjtMotocar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:jtMotocar.get:power");return;}
  
zbObject *res=createNumber(thiz->getpower(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}






void init_jtmobjs_rt(Runtime &rt) {
rt.addsub("ModelVariables_op_new_0",ModelVariables_op_new_0);
rt.addsub("ModelVariables_setFloat_2_string_float",ModelVariables_setFloat_2_string_float);
rt.addsub("ModelVariables_setString_2_string_string",ModelVariables_setString_2_string_string);
rt.addsub("ModelVariables_setGradString_4_string_string_string_float",ModelVariables_setGradString_4_string_string_string_float);
rt.addsub("ModelVariables_getString_1_string",ModelVariables_getString_1_string);
rt.addsub("jtObject_get_name_0",jtObject_get_name_0);
rt.addsub("jtObject_get_title_0",jtObject_get_title_0);
rt.addsub("jtObject_get_path_0",jtObject_get_path_0);
rt.addsub("jtObject_get_author_0",jtObject_get_author_0);
rt.addsub("jtObject_get_data_0",jtObject_get_data_0);
rt.addsub("static_jtObject_load_1_PersistentObject",static_jtObject_load_1_PersistentObject);
rt.addsub("jtCab_render_2_RenderContext_XMLNode",jtCab_render_2_RenderContext_XMLNode);
rt.addsub("jtBuilding_get_W_0",jtBuilding_get_W_0);
rt.addsub("jtBuilding_get_H_0",jtBuilding_get_H_0);
rt.addsub("jtSmoke_create_0",jtSmoke_create_0);
rt.addsub("SmokeInstance_render_1_RenderContext",SmokeInstance_render_1_RenderContext);
rt.addsub("SmokeInstance_iterate_7_float_float_float_float_float_float_float",SmokeInstance_iterate_7_float_float_float_float_float_float_float);
rt.addsub("SmokeInstance_translate_3_float_float_float",SmokeInstance_translate_3_float_float_float);
rt.addsub("jtStation_complete_2_int_int",jtStation_complete_2_int_int);
rt.addsub("jtBridge_complete_1_int",jtBridge_complete_1_int);
rt.addsub("StationInstance_render_5_float_float_float_RenderContext_int",StationInstance_render_5_float_float_float_RenderContext_int);
rt.addsub("BridgeInstance_render_5_float_float_float_RenderContext_int",BridgeInstance_render_5_float_float_float_RenderContext_int);
rt.addsub("jtMachine_get_maxSpeed_0",jtMachine_get_maxSpeed_0);
rt.addsub("jtMachine_get_weight_0",jtMachine_get_weight_0);
rt.addsub("jtMachine_get_power_0",jtMachine_get_power_0);
rt.addsub("jtMotocar_get_maxSpeed_0",jtMotocar_get_maxSpeed_0);
rt.addsub("jtMotocar_get_weight_0",jtMotocar_get_weight_0);
rt.addsub("jtMotocar_get_power_0",jtMotocar_get_power_0);

}

