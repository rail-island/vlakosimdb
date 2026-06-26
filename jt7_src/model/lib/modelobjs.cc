bool static_ModelProcs_polyedrToPolygons_2_XMLNode_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *polyedr=dynamic_cast<zbXMLNode*>(params[0]);
  if (! polyedr) return false;// {ir->error(fmtstr("ArgError:ModelProcs.polyedrToPolygons,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 
zbXMLNode *polygons=dynamic_cast<zbXMLNode*>(params[1]);
  if (! polygons) return false;// {ir->error(fmtstr("ArgError:ModelProcs.polyedrToPolygons,expected XMLNode,found %s",typeid(*params[1]).name()));return;}
 
 
zbObject *res=createVoid(ir->rt);pedrtopolys(polyedr,polygons);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_ModelProcs_crook_2_XMLNode_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *polyedr=dynamic_cast<zbXMLNode*>(params[0]);
  if (! polyedr) return false;// {ir->error(fmtstr("ArgError:ModelProcs.crook,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

   zbObject *args=params[1];
 
 
zbObject *res=createVoid(ir->rt);crookalg(polyedr,args->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_ModelProcs_translate_4_XMLNode_float_float_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<4 || parcnt>4) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *shape=dynamic_cast<zbXMLNode*>(params[0]);
  if (! shape) return false;// {ir->error(fmtstr("ArgError:ModelProcs.translate,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

   zbObject *tx=params[1];
 
 

   zbObject *ty=params[2];
 
 

   zbObject *tz=params[3];
 
 
zbObject *res=createVoid(ir->rt);s_translate(shape->cvalue(),tx->tofloat(),ty->tofloat(),tz->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_ModelProcs_scale_4_XMLNode_float_float_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<4 || parcnt>4) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *shape=dynamic_cast<zbXMLNode*>(params[0]);
  if (! shape) return false;// {ir->error(fmtstr("ArgError:ModelProcs.scale,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

   zbObject *sx=params[1];
 
 

   zbObject *sy=params[2];
 
 

   zbObject *sz=params[3];
 
 
zbObject *res=createVoid(ir->rt);s_scale(shape->cvalue(),sx->tofloat(),sy->tofloat(),sz->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_ModelProcs_fillPattern_6_Canvas_int_int_int_int_Picture(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<6 || parcnt>6) return false;
  zbObject **params=ir->getparams(parcnt);
zbCanvas *cnv=dynamic_cast<zbCanvas*>(params[0]);
  if (! cnv) return false;// {ir->error(fmtstr("ArgError:ModelProcs.fillPattern,expected Canvas,found %s",typeid(*params[0]).name()));return;}
 
 

   zbObject *x1=params[1];
 
 

   zbObject *y1=params[2];
 
 

   zbObject *x2=params[3];
 
 

   zbObject *y2=params[4];
 
 
zbPicture *pattern=dynamic_cast<zbPicture*>(params[5]);
  if (! pattern) return false;// {ir->error(fmtstr("ArgError:ModelProcs.fillPattern,expected Picture,found %s",typeid(*params[5]).name()));return;}
 
 
zbObject *res=createVoid(ir->rt);fillPattern(cnv,x1->toint(),y1->toint(),x2->toint(),y2->toint(),pattern);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_ModelProcs_generateRotateSolid_4_XMLNode_XMLNode_int_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<4 || parcnt>4) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *pts=dynamic_cast<zbXMLNode*>(params[0]);
  if (! pts) return false;// {ir->error(fmtstr("ArgError:ModelProcs.generateRotateSolid,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 
zbXMLNode *pedr=dynamic_cast<zbXMLNode*>(params[1]);
  if (! pedr) return false;// {ir->error(fmtstr("ArgError:ModelProcs.generateRotateSolid,expected XMLNode,found %s",typeid(*params[1]).name()));return;}
 
 

   zbObject *quality=params[2];
 
 
zbXMLNode *colorize=dynamic_cast<zbXMLNode*>(params[3]);
  if (! colorize) return false;// {ir->error(fmtstr("ArgError:ModelProcs.generateRotateSolid,expected XMLNode,found %s",typeid(*params[3]).name()));return;}
 
 
zbObject *res=createVoid(ir->rt);generateRotateSolid(pts->cvalue(),pedr->cvalue(),quality->toint(),colorize->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_ModelProcs_pointInPolygon_3_float_float_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 
zbXMLNode *polygon=dynamic_cast<zbXMLNode*>(params[2]);
  if (! polygon) return false;// {ir->error(fmtstr("ArgError:ModelProcs.pointInPolygon,expected XMLNode,found %s",typeid(*params[2]).name()));return;}
 
 
zbObject *res=createLogical(pointinpolygon(x->tofloat(),y->tofloat(),polygon->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool GLView_get_renderContext_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGLView *thiz=dynamic_cast<zbGLView*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:GLView.get:renderContext");return;}
  
zbObject *res=thiz->getrenderContext()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool GLView_render_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGLView *thiz=dynamic_cast<zbGLView*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:GLView.render");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->render();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool GLModelView_get_model_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGLModelView *thiz=dynamic_cast<zbGLModelView*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:GLModelView.get:model");return;}
  
zbObject *res=thiz->getmodel()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool GLModelView_set_model_1_Object(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *value=dynamic_cast<zbObject*>(params[0]);
  if (! value) return false;// {ir->error(fmtstr("ArgError:GLModelView.set:model,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbGLModelView *thiz=dynamic_cast<zbGLModelView*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:GLModelView.set:model");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setmodel(value);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool GLModelView_get_vars_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGLModelView *thiz=dynamic_cast<zbGLModelView*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:GLModelView.get:vars");return;}
  
zbObject *res=thiz->getvars()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool GLModelView_set_vars_1_ModelVariables(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbModelVariables *value=dynamic_cast<zbModelVariables*>(params[0]);
  if (! value) return false;// {ir->error(fmtstr("ArgError:GLModelView.set:vars,expected ModelVariables,found %s",typeid(*params[0]).name()));return;}
 
 

    zbGLModelView *thiz=dynamic_cast<zbGLModelView*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:GLModelView.set:vars");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setvars(value);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool GLModelView_mapView_5_float_float_float_float_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *seeAngle=params[0];
 
 

   zbObject *viewAngle=params[1];
 
 

   zbObject *scale=params[2];
 
 

   zbObject *dx=params[3];
 
 

   zbObject *dy=params[4];
 
 

    zbGLModelView *thiz=dynamic_cast<zbGLModelView*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:GLModelView.mapView");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->mapview(seeAngle->tofloat(),viewAngle->tofloat(),scale->tofloat(),dx->tofloat(),dy->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool GLModelView_get_lighting_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGLModelView *thiz=dynamic_cast<zbGLModelView*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:GLModelView.get:lighting");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getlighting(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool GLModelView_set_lighting_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbGLModelView *thiz=dynamic_cast<zbGLModelView*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:GLModelView.set:lighting");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setlighting(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool GLModelView_setLight_5_float_float_float_float_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *ambient=params[0];
 
 

   zbObject *diffuse=params[1];
 
 

   zbObject *specular=params[2];
 
 

   zbObject *ydir=params[3];
 
 

   zbObject *xzangle=params[4];
 
 

    zbGLModelView *thiz=dynamic_cast<zbGLModelView*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:GLModelView.setLight");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setlight(ambient->tofloat(),diffuse->tofloat(),specular->tofloat(),ydir->tofloat(),xzangle->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Model_load_2_XMLNode_PersistentObject(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[0]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Model.load,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 
zbPersistentObject *textures=dynamic_cast<zbPersistentObject*>(params[1]);
  if (! textures) return false;// {ir->error(fmtstr("ArgError:Model.load,expected PersistentObject,found %s",typeid(*params[1]).name()));return;}
 
 

    zbModel *thiz=dynamic_cast<zbModel*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Model.load");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->load(xml->cvalue(),textures->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Model_load_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[0]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Model.load,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbModel *thiz=dynamic_cast<zbModel*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Model.load");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->load(xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Model_drawProjection_6_Canvas_int_float_float_float_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<6 || parcnt>6) return false;
  zbObject **params=ir->getparams(parcnt);
zbCanvas *canvas=dynamic_cast<zbCanvas*>(params[0]);
  if (! canvas) return false;// {ir->error(fmtstr("ArgError:Model.drawProjection,expected Canvas,found %s",typeid(*params[0]).name()));return;}
 
 

   zbObject *dir=params[1];
 
 

   zbObject *sx=params[2];
 
 

   zbObject *sy=params[3];
 
 

   zbObject *tx=params[4];
 
 

   zbObject *ty=params[5];
 
 

    zbModel *thiz=dynamic_cast<zbModel*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Model.drawProjection");return;}
  
zbObject *res=createVoid(ir->rt);thiz->drawProjection(canvas,dir->toint(),sx->tofloat(),sy->tofloat(),tx->tofloat(),ty->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Model_saveToVRML_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *file=params[0];
 
 

    zbModel *thiz=dynamic_cast<zbModel*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Model.saveToVRML");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->savetovrml(file->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Model_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newmodel(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Textures_define_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

   zbObject *file=params[1];
 
 
zbObject *res=createVoid(ir->rt);definetexture(name->tostring(),file->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Textures_clear_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=createVoid(ir->rt);cleartextures();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Textures_textureFile_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *texname=params[0];
 
 
zbObject *res=createString(gettexturefile(texname->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ColorAlg_addSide_12_int_float_float_float_float_string_bool_string_string_string_string_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<12 || parcnt>12) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *dir=params[0];
 
 

   zbObject *x1=params[1];
 
 

   zbObject *y1=params[2];
 
 

   zbObject *x2=params[3];
 
 

   zbObject *y2=params[4];
 
 

   zbObject *texname=params[5];
 
 

   zbObject *transparent=params[6];
 
 

   zbObject *condidion=params[7];
 
 

   zbObject *rkoef=params[8];
 
 

   zbObject *gkoef=params[9];
 
 

   zbObject *bkoef=params[10];
 
 

   zbObject *denylight=params[11];
 
 

    zbColorAlg *thiz=dynamic_cast<zbColorAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ColorAlg.addSide");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->addside(dir->toint(),x1->tofloat(),y1->tofloat(),x2->tofloat(),y2->tofloat(),texname->tostring(),transparent->tobool(),condidion->tostring(),rkoef->tostring(),gkoef->tostring(),bkoef->tostring(),denylight->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ColorAlg_addMaterialSide_13_int_float_float_float_float_string_bool_string_string_string_bool_string_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<13 || parcnt>13) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *dir=params[0];
 
 

   zbObject *dx=params[1];
 
 

   zbObject *dy=params[2];
 
 

   zbObject *xscale=params[3];
 
 

   zbObject *yscale=params[4];
 
 

   zbObject *texname=params[5];
 
 

   zbObject *transparent=params[6];
 
 

   zbObject *rkoef=params[7];
 
 

   zbObject *gkoef=params[8];
 
 

   zbObject *bkoef=params[9];
 
 

   zbObject *transp=params[10];
 
 

   zbObject *condidion=params[11];
 
 

   zbObject *denylight=params[12];
 
 

    zbColorAlg *thiz=dynamic_cast<zbColorAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ColorAlg.addMaterialSide");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->addmaterialside(dir->toint(),dx->tofloat(),dy->tofloat(),xscale->tofloat(),yscale->tofloat(),texname->tostring(),transparent->tobool(),rkoef->tostring(),gkoef->tostring(),bkoef->tostring(),transp->tobool(),condidion->tostring(),denylight->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ColorAlg_run_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *polygon=dynamic_cast<zbXMLNode*>(params[0]);
  if (! polygon) return false;// {ir->error(fmtstr("ArgError:ColorAlg.run,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbColorAlg *thiz=dynamic_cast<zbColorAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ColorAlg.run");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->run(polygon->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ColorAlg_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newcoloralg(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PedrAlg_clip_2_int_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *dir=params[0];
 
 
zbXMLNode *polygon=dynamic_cast<zbXMLNode*>(params[1]);
  if (! polygon) return false;// {ir->error(fmtstr("ArgError:PedrAlg.clip,expected XMLNode,found %s",typeid(*params[1]).name()));return;}
 
 

    zbPedrAlg *thiz=dynamic_cast<zbPedrAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PedrAlg.clip");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->addclip(dir->toint(),polygon->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PedrAlg_run_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *output=dynamic_cast<zbXMLNode*>(params[0]);
  if (! output) return false;// {ir->error(fmtstr("ArgError:PedrAlg.run,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbPedrAlg *thiz=dynamic_cast<zbPedrAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PedrAlg.run");return;}
  
zbObject *res=createVoid(ir->rt);thiz->run(output->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PedrAlg_modifyRun_2_XMLNode_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *input=dynamic_cast<zbXMLNode*>(params[0]);
  if (! input) return false;// {ir->error(fmtstr("ArgError:PedrAlg.modifyRun,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 
zbXMLNode *output=dynamic_cast<zbXMLNode*>(params[1]);
  if (! output) return false;// {ir->error(fmtstr("ArgError:PedrAlg.modifyRun,expected XMLNode,found %s",typeid(*params[1]).name()));return;}
 
 

    zbPedrAlg *thiz=dynamic_cast<zbPedrAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PedrAlg.modifyRun");return;}
  
zbObject *res=createVoid(ir->rt);thiz->modifyRun(input->cvalue(),output->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PedrAlg_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newpedralg(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_run_2_XMLNode_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[0]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:StampAlg.run,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 
zbXMLNode *newpoly=dynamic_cast<zbXMLNode*>(params[1]);
  if (! newpoly) return false;// {ir->error(fmtstr("ArgError:StampAlg.run,expected XMLNode,found %s",typeid(*params[1]).name()));return;}
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.run");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->run(xml->cvalue(),newpoly->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_texture_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:texture");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettexture(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_texture_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:texture");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settexture(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_dir_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:dir");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getdir(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_dir_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:dir");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setdir(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_x1_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:x1");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getx1(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_x1_1_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:x1");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setx1(value->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_y1_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:y1");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->gety1(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_y1_1_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:y1");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->sety1(value->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_x2_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:x2");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getx2(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_x2_1_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:x2");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setx2(value->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_y2_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:y2");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->gety2(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_y2_1_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:y2");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->sety2(value->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_maxdist_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:maxdist");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getmaxdist(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_maxdist_1_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:maxdist");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setmaxdist(value->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_reflectt_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:reflectt");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getreflectt(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_reflectt_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:reflectt");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setreflectt(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_reflects_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:reflects");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getreflects(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_reflects_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:reflects");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setreflects(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_transp_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:transp");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->gettransp(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_transp_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:transp");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settransp(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_condition_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:condition");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getcondition(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_condition_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:condition");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setcondition(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_material_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:material");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getmaterial(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_material_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:material");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setmaterial(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_xscale_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:xscale");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getxscale(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_xscale_1_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:xscale");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setxscale(value->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_yscale_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:yscale");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getyscale(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_yscale_1_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:yscale");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setyscale(value->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_rkoef_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:rkoef");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getrkoef(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_rkoef_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:rkoef");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setrkoef(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_gkoef_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:gkoef");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getgkoef(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_gkoef_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:gkoef");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setgkoef(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_bkoef_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:bkoef");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getbkoef(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_bkoef_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:bkoef");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setbkoef(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_get_dist_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.get:dist");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getdist(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_set_dist_1_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStampAlg *thiz=dynamic_cast<zbStampAlg*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:StampAlg.set:dist");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setdist(value->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool StampAlg_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newstampalg(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}






void init_modelobjs_rt(Runtime &rt) {
rt.addsub("static_ModelProcs_polyedrToPolygons_2_XMLNode_XMLNode",static_ModelProcs_polyedrToPolygons_2_XMLNode_XMLNode);
rt.addsub("static_ModelProcs_crook_2_XMLNode_string",static_ModelProcs_crook_2_XMLNode_string);
rt.addsub("static_ModelProcs_translate_4_XMLNode_float_float_float",static_ModelProcs_translate_4_XMLNode_float_float_float);
rt.addsub("static_ModelProcs_scale_4_XMLNode_float_float_float",static_ModelProcs_scale_4_XMLNode_float_float_float);
rt.addsub("static_ModelProcs_fillPattern_6_Canvas_int_int_int_int_Picture",static_ModelProcs_fillPattern_6_Canvas_int_int_int_int_Picture);
rt.addsub("static_ModelProcs_generateRotateSolid_4_XMLNode_XMLNode_int_XMLNode",static_ModelProcs_generateRotateSolid_4_XMLNode_XMLNode_int_XMLNode);
rt.addsub("static_ModelProcs_pointInPolygon_3_float_float_XMLNode",static_ModelProcs_pointInPolygon_3_float_float_XMLNode);
rt.addsub("GLView_get_renderContext_0",GLView_get_renderContext_0);
rt.addsub("GLView_render_0",GLView_render_0);
rt.addsub("GLModelView_get_model_0",GLModelView_get_model_0);
rt.addsub("GLModelView_set_model_1_Object",GLModelView_set_model_1_Object);
rt.addsub("GLModelView_get_vars_0",GLModelView_get_vars_0);
rt.addsub("GLModelView_set_vars_1_ModelVariables",GLModelView_set_vars_1_ModelVariables);
rt.addsub("GLModelView_mapView_5_float_float_float_float_float",GLModelView_mapView_5_float_float_float_float_float);
rt.addsub("GLModelView_get_lighting_0",GLModelView_get_lighting_0);
rt.addsub("GLModelView_set_lighting_1_bool",GLModelView_set_lighting_1_bool);
rt.addsub("GLModelView_setLight_5_float_float_float_float_float",GLModelView_setLight_5_float_float_float_float_float);
rt.addsub("Model_load_2_XMLNode_PersistentObject",Model_load_2_XMLNode_PersistentObject);
rt.addsub("Model_load_1_XMLNode",Model_load_1_XMLNode);
rt.addsub("Model_drawProjection_6_Canvas_int_float_float_float_float",Model_drawProjection_6_Canvas_int_float_float_float_float);
rt.addsub("Model_saveToVRML_1_string",Model_saveToVRML_1_string);
rt.addsub("Model_op_new_0",Model_op_new_0);
rt.addsub("static_Textures_define_2_string_string",static_Textures_define_2_string_string);
rt.addsub("static_Textures_clear_0",static_Textures_clear_0);
rt.addsub("static_Textures_textureFile_1_string",static_Textures_textureFile_1_string);
rt.addsub("ColorAlg_addSide_12_int_float_float_float_float_string_bool_string_string_string_string_bool",ColorAlg_addSide_12_int_float_float_float_float_string_bool_string_string_string_string_bool);
rt.addsub("ColorAlg_addMaterialSide_13_int_float_float_float_float_string_bool_string_string_string_bool_string_bool",ColorAlg_addMaterialSide_13_int_float_float_float_float_string_bool_string_string_string_bool_string_bool);
rt.addsub("ColorAlg_run_1_XMLNode",ColorAlg_run_1_XMLNode);
rt.addsub("ColorAlg_op_new_0",ColorAlg_op_new_0);
rt.addsub("PedrAlg_clip_2_int_XMLNode",PedrAlg_clip_2_int_XMLNode);
rt.addsub("PedrAlg_run_1_XMLNode",PedrAlg_run_1_XMLNode);
rt.addsub("PedrAlg_modifyRun_2_XMLNode_XMLNode",PedrAlg_modifyRun_2_XMLNode_XMLNode);
rt.addsub("PedrAlg_op_new_0",PedrAlg_op_new_0);
rt.addsub("StampAlg_run_2_XMLNode_XMLNode",StampAlg_run_2_XMLNode_XMLNode);
rt.addsub("StampAlg_get_texture_0",StampAlg_get_texture_0);
rt.addsub("StampAlg_set_texture_1_string",StampAlg_set_texture_1_string);
rt.addsub("StampAlg_get_dir_0",StampAlg_get_dir_0);
rt.addsub("StampAlg_set_dir_1_int",StampAlg_set_dir_1_int);
rt.addsub("StampAlg_get_x1_0",StampAlg_get_x1_0);
rt.addsub("StampAlg_set_x1_1_float",StampAlg_set_x1_1_float);
rt.addsub("StampAlg_get_y1_0",StampAlg_get_y1_0);
rt.addsub("StampAlg_set_y1_1_float",StampAlg_set_y1_1_float);
rt.addsub("StampAlg_get_x2_0",StampAlg_get_x2_0);
rt.addsub("StampAlg_set_x2_1_float",StampAlg_set_x2_1_float);
rt.addsub("StampAlg_get_y2_0",StampAlg_get_y2_0);
rt.addsub("StampAlg_set_y2_1_float",StampAlg_set_y2_1_float);
rt.addsub("StampAlg_get_maxdist_0",StampAlg_get_maxdist_0);
rt.addsub("StampAlg_set_maxdist_1_float",StampAlg_set_maxdist_1_float);
rt.addsub("StampAlg_get_reflectt_0",StampAlg_get_reflectt_0);
rt.addsub("StampAlg_set_reflectt_1_bool",StampAlg_set_reflectt_1_bool);
rt.addsub("StampAlg_get_reflects_0",StampAlg_get_reflects_0);
rt.addsub("StampAlg_set_reflects_1_bool",StampAlg_set_reflects_1_bool);
rt.addsub("StampAlg_get_transp_0",StampAlg_get_transp_0);
rt.addsub("StampAlg_set_transp_1_bool",StampAlg_set_transp_1_bool);
rt.addsub("StampAlg_get_condition_0",StampAlg_get_condition_0);
rt.addsub("StampAlg_set_condition_1_string",StampAlg_set_condition_1_string);
rt.addsub("StampAlg_get_material_0",StampAlg_get_material_0);
rt.addsub("StampAlg_set_material_1_bool",StampAlg_set_material_1_bool);
rt.addsub("StampAlg_get_xscale_0",StampAlg_get_xscale_0);
rt.addsub("StampAlg_set_xscale_1_float",StampAlg_set_xscale_1_float);
rt.addsub("StampAlg_get_yscale_0",StampAlg_get_yscale_0);
rt.addsub("StampAlg_set_yscale_1_float",StampAlg_set_yscale_1_float);
rt.addsub("StampAlg_get_rkoef_0",StampAlg_get_rkoef_0);
rt.addsub("StampAlg_set_rkoef_1_string",StampAlg_set_rkoef_1_string);
rt.addsub("StampAlg_get_gkoef_0",StampAlg_get_gkoef_0);
rt.addsub("StampAlg_set_gkoef_1_string",StampAlg_set_gkoef_1_string);
rt.addsub("StampAlg_get_bkoef_0",StampAlg_get_bkoef_0);
rt.addsub("StampAlg_set_bkoef_1_string",StampAlg_set_bkoef_1_string);
rt.addsub("StampAlg_get_dist_0",StampAlg_get_dist_0);
rt.addsub("StampAlg_set_dist_1_float",StampAlg_set_dist_1_float);
rt.addsub("StampAlg_op_new_0",StampAlg_op_new_0);

}

