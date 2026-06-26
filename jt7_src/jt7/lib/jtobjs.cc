bool Game_op_new_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *w=params[0];
 
 

   zbObject *h=params[1];
 
 
zbObject *res=newgame(w->toint(),h->toint(),ir)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_op_new_1_Archive(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbArchive *ar=dynamic_cast<zbArchive*>(params[0]);
  if (! ar) return false;// {ir->error(fmtstr("ArgError:Game.op:new,expected Archive,found %s",typeid(*params[0]).name()));return;}
 
 
zbObject *res=loadgame(ar->cvalue(),ir)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_mapX_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:mapX");return;}
  
zbObject *res=createNumber(thiz->getmapX(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_mapY_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:mapY");return;}
  
zbObject *res=createNumber(thiz->getmapY(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_vertexX_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:vertexX");return;}
  
zbObject *res=createNumber(thiz->getvertexX(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_vertexY_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:vertexY");return;}
  
zbObject *res=createNumber(thiz->getvertexY(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_mouseX_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:mouseX");return;}
  
zbObject *res=createNumber(thiz->getmouseX(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_mouseY_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:mouseY");return;}
  
zbObject *res=createNumber(thiz->getmouseY(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_mouseInMiddle_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:mouseInMiddle");return;}
  
zbObject *res=createLogical(thiz->getmouseInMiddle(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_drawCustomCursor_4_int_int_string_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<4 || parcnt>4) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *type=params[2];
 
 

   zbObject *dir=params[3];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.drawCustomCursor");return;}
  
zbObject *res=createVoid(ir->rt);thiz->drawCustomCursor(x->toint(),y->toint(),type->tostring(),dir->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_drawCustomCursors_6_int_int_int_int_string_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<6 || parcnt>6) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x1=params[0];
 
 

   zbObject *y1=params[1];
 
 

   zbObject *x2=params[2];
 
 

   zbObject *y2=params[3];
 
 

   zbObject *type=params[4];
 
 

   zbObject *dir=params[5];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.drawCustomCursors");return;}
  
zbObject *res=createVoid(ir->rt);thiz->drawCustomCursors(x1->toint(),y1->toint(),x2->toint(),y2->toint(),type->tostring(),dir->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_drawCursor_3_int_int_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *color=params[2];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.drawCursor");return;}
  
zbObject *res=createVoid(ir->rt);thiz->drawCursor(x->toint(),y->toint(),color->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_drawComplexCursor_3_int_int_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *color=params[2];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.drawComplexCursor");return;}
  
zbObject *res=createVoid(ir->rt);thiz->drawComplexCursor(x->toint(),y->toint(),color->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_drawPointCursor_3_int_int_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *color=params[2];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.drawPointCursor");return;}
  
zbObject *res=createVoid(ir->rt);thiz->drawPointCursor(x->toint(),y->toint(),color->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_drawPointCursors_5_int_int_int_int_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x1=params[0];
 
 

   zbObject *y1=params[1];
 
 

   zbObject *x2=params[2];
 
 

   zbObject *y2=params[3];
 
 

   zbObject *color=params[4];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.drawPointCursors");return;}
  
zbObject *res=createVoid(ir->rt);thiz->drawPointCursors(x1->toint(),y1->toint(),x2->toint(),y2->toint(),color->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_drawSqLineCursor_5_int_int_bool_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *vertical=params[2];
 
 

   zbObject *framecolor=params[3];
 
 

   zbObject *linecolor=params[4];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.drawSqLineCursor");return;}
  
zbObject *res=createVoid(ir->rt);thiz->drawSqLineCursor(x->toint(),y->toint(),vertical->tobool(),framecolor->tostring(),linecolor->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_handler_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:handler");return;}
  
zbObject *res=thiz->gethandler()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_set_handler_1_Object(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *value=dynamic_cast<zbObject*>(params[0]);
  if (! value) return false;// {ir->error(fmtstr("ArgError:Game.set:handler,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.set:handler");return;}
  
zbObject *res=createVoid(ir->rt);thiz->sethandler(value);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_end_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.end");return;}
  
zbObject *res=createVoid(ir->rt);thiz->end();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_save_1_Archive(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbArchive *ar=dynamic_cast<zbArchive*>(params[0]);
  if (! ar) return false;// {ir->error(fmtstr("ArgError:Game.save,expected Archive,found %s",typeid(*params[0]).name()));return;}
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.save");return;}
  
zbObject *res=createVoid(ir->rt);thiz->save(ar->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_block_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *what=params[0];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.block");return;}
  
zbObject *res=createVoid(ir->rt);thiz->block(what->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_unblock_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *what=params[0];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.unblock");return;}
  
zbObject *res=createVoid(ir->rt);thiz->unblock(what->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_buildDepo_5_int_int_int_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x1=params[0];
 
 

   zbObject *y1=params[1];
 
 

   zbObject *x2=params[2];
 
 

   zbObject *y2=params[3];
 
 

   zbObject *dir=params[4];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.buildDepo");return;}
  
zbObject *res=createVoid(ir->rt);thiz->buildDepo(x1->toint(),y1->toint(),x2->toint(),y2->toint(),dir->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_buildRStation_5_int_int_int_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x1=params[0];
 
 

   zbObject *y1=params[1];
 
 

   zbObject *x2=params[2];
 
 

   zbObject *y2=params[3];
 
 

   zbObject *dir=params[4];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.buildRStation");return;}
  
zbObject *res=createVoid(ir->rt);thiz->buildRStation(x1->toint(),y1->toint(),x2->toint(),y2->toint(),dir->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_buildBusStop_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.buildBusStop");return;}
  
zbObject *res=createVoid(ir->rt);thiz->buildBusStop(x->toint(),y->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_staticAt_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *mapx=params[0];
 
 

   zbObject *mapy=params[1];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.staticAt");return;}
  
zbObject *res=thiz->staticAt(mapx->toint(),mapy->toint())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_mobileAt_2_float_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *mousex=params[0];
 
 

   zbObject *mousey=params[1];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.mobileAt");return;}
  
zbObject *res=thiz->mobileAt(mousex->tofloat(),mousey->tofloat())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_statics_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:statics");return;}
  
zbObject *res=thiz->getstatics()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_mobiles_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:mobiles");return;}
  
zbObject *res=thiz->getmobiles()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_objectType_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *id=params[0];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.objectType");return;}
  
zbObject *res=createString(thiz->objectType(id->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_objectFromId_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *id=params[0];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.objectFromId");return;}
  
zbObject *res=thiz->objectFromId(id->toint())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_objectFromName_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.objectFromName");return;}
  
zbObject *res=thiz->objectFromName(name->tostring())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_generateCircles_5_int_float_float_float_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *steps=params[0];
 
 

   zbObject *mindisp=params[1];
 
 

   zbObject *maxdisp=params[2];
 
 

   zbObject *minrad=params[3];
 
 

   zbObject *maxrad=params[4];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.generateCircles");return;}
  
zbObject *res=createVoid(ir->rt);thiz->generateCircles(steps->toint(),mindisp->tofloat(),maxdisp->tofloat(),minrad->tofloat(),maxrad->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_clearHeights_1_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.clearHeights");return;}
  
zbObject *res=createVoid(ir->rt);thiz->clearHeights(value->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_buildSemaphore_3_int_int_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *type=params[2];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.buildSemaphore");return;}
  
zbObject *res=createVoid(ir->rt);thiz->buildSemaphore(x->toint(),y->toint(),type->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_machines_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:machines");return;}
  
zbObject *res=thiz->getmachines()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_motocars_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:motocars");return;}
  
zbObject *res=thiz->getmotocars()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_saving_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:saving");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createXMLNode(thiz->getlink()->getsaving(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_loaded_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.loaded");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->loaded();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_getHi_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.getHi");return;}
  
zbObject *res=createNumber(thiz->getHi(x->toint(),y->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_setHi_5_int_int_int_int_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *w=params[2];
 
 

   zbObject *h=params[3];
 
 

   zbObject *value=params[4];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.setHi");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setHi(x->toint(),y->toint(),w->toint(),h->toint(),value->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_repairHeights_5_int_int_int_int_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *w=params[2];
 
 

   zbObject *h=params[3];
 
 

   zbObject *mhd=params[4];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.repairHeights");return;}
  
zbObject *res=createVoid(ir->rt);thiz->repairHeights(x->toint(),y->toint(),w->toint(),h->toint(),mhd->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_eraseRail_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *mapx=params[0];
 
 

   zbObject *mapy=params[1];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.eraseRail");return;}
  
zbObject *res=createVoid(ir->rt);thiz->eraseRail(mapx->toint(),mapy->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_fillWater_3_int_int_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *vx=params[0];
 
 

   zbObject *vy=params[1];
 
 

   zbObject *waterhi=params[2];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.fillWater");return;}
  
zbObject *res=createVoid(ir->rt);thiz->fillWater(vx->toint(),vy->toint(),waterhi->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_dryWater_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *vx=params[0];
 
 

   zbObject *vy=params[1];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.dryWater");return;}
  
zbObject *res=createVoid(ir->rt);thiz->dryWater(vx->toint(),vy->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_buildBridgeExact_7_int_int_int_bool_float_float_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<7 || parcnt>7) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *len=params[2];
 
 

   zbObject *ver=params[3];
 
 

   zbObject *height=params[4];
 
 

   zbObject *dhisum=params[5];
 
 

   zbObject *what=params[6];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.buildBridgeExact");return;}
  
zbObject *res=createVoid(ir->rt);thiz->buildBridgeExact(x->toint(),y->toint(),len->toint(),ver->tobool(),height->tofloat(),dhisum->tofloat(),what->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_buildBridge_7_int_int_int_bool_float_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<7 || parcnt>7) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *len=params[2];
 
 

   zbObject *ver=params[3];
 
 

   zbObject *dhielem=params[4];
 
 

   zbObject *dhimode=params[5];
 
 

   zbObject *what=params[6];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.buildBridge");return;}
  
zbObject *res=createVoid(ir->rt);thiz->buildBridge(x->toint(),y->toint(),len->toint(),ver->tobool(),dhielem->tofloat(),dhimode->tostring(),what->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_buildTunnelExact_7_int_int_int_bool_float_float_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<7 || parcnt>7) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *len=params[2];
 
 

   zbObject *ver=params[3];
 
 

   zbObject *height=params[4];
 
 

   zbObject *dhisum=params[5];
 
 

   zbObject *what=params[6];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.buildTunnelExact");return;}
  
zbObject *res=createVoid(ir->rt);thiz->buildTunnelExact(x->toint(),y->toint(),len->toint(),ver->tobool(),height->tofloat(),dhisum->tofloat(),what->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_buildTunnel_7_int_int_int_bool_float_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<7 || parcnt>7) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *len=params[2];
 
 

   zbObject *ver=params[3];
 
 

   zbObject *dhielem=params[4];
 
 

   zbObject *dhimode=params[5];
 
 

   zbObject *what=params[6];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.buildTunnel");return;}
  
zbObject *res=createVoid(ir->rt);thiz->buildTunnel(x->toint(),y->toint(),len->toint(),ver->tobool(),dhielem->tofloat(),dhimode->tostring(),what->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_bridgeTypes_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:bridgeTypes");return;}
  
zbObject *res=thiz->getbridgeTypes()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_tunnelTypes_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:tunnelTypes");return;}
  
zbObject *res=thiz->gettunnelTypes()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_semaphoreTypes_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:semaphoreTypes");return;}
  
zbObject *res=thiz->getsemaphoreTypes()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_stationTypes_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:stationTypes");return;}
  
zbObject *res=thiz->getstationTypes()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_depoTypes_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:depoTypes");return;}
  
zbObject *res=thiz->getdepoTypes()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_aircraftTypes_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:aircraftTypes");return;}
  
zbObject *res=thiz->getaircraftTypes()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_unWatch_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.unWatch");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->unwatch();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_clearMobiles_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.clearMobiles");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->clearmobiles();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_dryAll_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.dryAll");return;}
  
zbObject *res=createVoid(ir->rt);thiz->dryAll();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_plant_3_int_int_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *tree=params[2];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.plant");return;}
  
zbObject *res=createVoid(ir->rt);thiz->plant(x->toint(),y->toint(),tree->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_treeTypes_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:treeTypes");return;}
  
zbObject *res=thiz->gettreeTypes()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_buildingTypes_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:buildingTypes");return;}
  
zbObject *res=thiz->getbuildingTypes()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_buildBuilding_4_int_int_int_jtBuilding(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<4 || parcnt>4) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *dir=params[2];
 
 
zbjtBuilding *type=dynamic_cast<zbjtBuilding*>(params[3]);
  if (! type) return false;// {ir->error(fmtstr("ArgError:Game.buildBuilding,expected jtBuilding,found %s",typeid(*params[3]).name()));return;}
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.buildBuilding");return;}
  
zbObject *res=createVoid(ir->rt);thiz->buildBuilding(x->toint(),y->toint(),dir->toint(),type);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_buildBuildingExact_5_int_int_int_float_jtBuilding(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *dir=params[2];
 
 

   zbObject *hi=params[3];
 
 
zbjtBuilding *type=dynamic_cast<zbjtBuilding*>(params[4]);
  if (! type) return false;// {ir->error(fmtstr("ArgError:Game.buildBuildingExact,expected jtBuilding,found %s",typeid(*params[4]).name()));return;}
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.buildBuildingExact");return;}
  
zbObject *res=createVoid(ir->rt);thiz->buildBuildingExact(x->toint(),y->toint(),dir->toint(),hi->tofloat(),type);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_terrainTypes_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:terrainTypes");return;}
  
zbObject *res=thiz->getterrainTypes()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_setTerrain_3_int_int_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *terrain=params[2];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.setTerrain");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setTerrain(x->toint(),y->toint(),terrain->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_railTypes_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:railTypes");return;}
  
zbObject *res=thiz->getrailTypes()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_roadTypes_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:roadTypes");return;}
  
zbObject *res=thiz->getroadTypes()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_busStopTypes_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:busStopTypes");return;}
  
zbObject *res=thiz->getbusStopTypes()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_setRail_3_int_int_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *rail=params[2];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.setRail");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setRail(x->toint(),y->toint(),rail->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_buildRunway_4_int_int_bool_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<4 || parcnt>4) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *ver=params[2];
 
 

   zbObject *len=params[3];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.buildRunway");return;}
  
zbObject *res=createVoid(ir->rt);thiz->buildRunway(x->toint(),y->toint(),ver->tobool(),len->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_ltop_3_float_float_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *h=params[2];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.ltop");return;}
  
zbObject *res=thiz->ltop(x->tofloat(),y->tofloat(),h->tofloat())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_drawCircle_5_float_float_float_float_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *xrad=params[2];
 
 

   zbObject *yrad=params[3];
 
 

   zbObject *color=params[4];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.drawCircle");return;}
  
zbObject *res=createVoid(ir->rt);thiz->drawCircle(x->tofloat(),y->tofloat(),xrad->tofloat(),yrad->tofloat(),color->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_drawMapCircle_5_float_float_float_float_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *xrad=params[2];
 
 

   zbObject *yrad=params[3];
 
 

   zbObject *color=params[4];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.drawMapCircle");return;}
  
zbObject *res=createVoid(ir->rt);thiz->drawMapCircle(x->tofloat(),y->tofloat(),xrad->tofloat(),yrad->tofloat(),color->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_screenMouseX_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:screenMouseX");return;}
  
zbObject *res=createNumber(thiz->getscreenMouseX(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_screenMouseY_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:screenMouseY");return;}
  
zbObject *res=createNumber(thiz->getscreenMouseY(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_viewportMouseX_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:viewportMouseX");return;}
  
zbObject *res=createNumber(thiz->getviewportMouseX(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_viewportMouseY_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:viewportMouseY");return;}
  
zbObject *res=createNumber(thiz->getviewportMouseY(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_riseCircle_4_float_float_float_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<4 || parcnt>4) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *rad=params[2];
 
 

   zbObject *dhi=params[3];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.riseCircle");return;}
  
zbObject *res=createVoid(ir->rt);thiz->riseCircle(x->tofloat(),y->tofloat(),rad->tofloat(),dhi->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_deltaTime_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:deltaTime");return;}
  
zbObject *res=createNumber(thiz->getdeltaTime(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_viewType_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:viewType");return;}
  
zbObject *res=createString(thiz->getviewType(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_set_viewType_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.set:viewType");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setviewType(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_lookAt_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.lookAt");return;}
  
zbObject *res=createVoid(ir->rt);thiz->lookAt(x->toint(),y->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_storeRRTemplate_5_int_int_int_int_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *w=params[2];
 
 

   zbObject *h=params[3];
 
 
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[4]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Game.storeRRTemplate,expected XMLNode,found %s",typeid(*params[4]).name()));return;}
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.storeRRTemplate");return;}
  
zbObject *res=createVoid(ir->rt);thiz->storeRRTemplate(x->toint(),y->toint(),w->toint(),h->toint(),xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_useRRTemplate_4_int_int_int_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<4 || parcnt>4) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *dir8=params[2];
 
 
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[3]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Game.useRRTemplate,expected XMLNode,found %s",typeid(*params[3]).name()));return;}
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.useRRTemplate");return;}
  
zbObject *res=createVoid(ir->rt);thiz->useRRTemplate(x->toint(),y->toint(),dir8->toint(),xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_drawRRTemplate_4_int_int_int_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<4 || parcnt>4) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *dir8=params[2];
 
 
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[3]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Game.drawRRTemplate,expected XMLNode,found %s",typeid(*params[3]).name()));return;}
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.drawRRTemplate");return;}
  
zbObject *res=createVoid(ir->rt);thiz->drawRRTemplate(x->toint(),y->toint(),dir8->toint(),xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_buildRoad_4_int_int_float_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<4 || parcnt>4) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *mapx=params[0];
 
 

   zbObject *mapy=params[1];
 
 

   zbObject *dhi=params[2];
 
 

   zbObject *mode=params[3];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.buildRoad");return;}
  
zbObject *res=createVoid(ir->rt);thiz->buildRoad(mapx->toint(),mapy->toint(),dhi->tofloat(),mode->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_eraseRoad_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *mapx=params[0];
 
 

   zbObject *mapy=params[1];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.eraseRoad");return;}
  
zbObject *res=createVoid(ir->rt);thiz->eraseRoad(mapx->toint(),mapy->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_machine0_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:machine0");return;}
  
zbObject *res=thiz->getmachine0()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_motocar0_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:motocar0");return;}
  
zbObject *res=thiz->getmotocar0()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_setRoad_3_int_int_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *rail=params[2];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.setRoad");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setRoad(x->toint(),y->toint(),rail->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_searchMachine_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.searchMachine");return;}
  
zbObject *res=thiz->searchMachine(name->tostring())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_get_loadMessages_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.get:loadMessages");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getloadmessages(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Game_addLoadMesasage_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *msg=params[0];
 
 

    zbGame *thiz=dynamic_cast<zbGame*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Game.addLoadMesasage");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->addloadmessage(msg->tostring());
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
bool RRBuilder_op_new_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *mapx=params[0];
 
 

   zbObject *mapy=params[1];
 
 
zbObject *res=newrrbuilder(mapx->toint(),mapy->toint(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RRBuilder_build_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *mapx=params[0];
 
 

   zbObject *mapy=params[1];
 
 

    zbRRBuilder *thiz=dynamic_cast<zbRRBuilder*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RRBuilder.build");return;}
  
zbObject *res=createVoid(ir->rt);thiz->build(mapx->toint(),mapy->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RRBuilder_get_X_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbRRBuilder *thiz=dynamic_cast<zbRRBuilder*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RRBuilder.get:X");return;}
  
zbObject *res=createNumber(thiz->getX(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RRBuilder_get_Y_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbRRBuilder *thiz=dynamic_cast<zbRRBuilder*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RRBuilder.get:Y");return;}
  
zbObject *res=createNumber(thiz->getY(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RRBuilder_get_dHeight_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbRRBuilder *thiz=dynamic_cast<zbRRBuilder*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RRBuilder.get:dHeight");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getdhi(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RRBuilder_set_dHeight_1_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbRRBuilder *thiz=dynamic_cast<zbRRBuilder*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RRBuilder.set:dHeight");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setdhi(value->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RRBuilder_get_mode_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbRRBuilder *thiz=dynamic_cast<zbRRBuilder*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RRBuilder.get:mode");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getmode(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RRBuilder_set_mode_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbRRBuilder *thiz=dynamic_cast<zbRRBuilder*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RRBuilder.set:mode");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setmode(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RRBuilder_get_autoFirstAndLast_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbRRBuilder *thiz=dynamic_cast<zbRRBuilder*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RRBuilder.get:autoFirstAndLast");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getautofirstandlast(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RRBuilder_set_autoFirstAndLast_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbRRBuilder *thiz=dynamic_cast<zbRRBuilder*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RRBuilder.set:autoFirstAndLast");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setautofirstandlast(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool GameObject_get_name_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGameObject *thiz=dynamic_cast<zbGameObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:GameObject.get:name");return;}
  
zbObject *res=createString(thiz->getname(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool GameObject_set_name_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbGameObject *thiz=dynamic_cast<zbGameObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:GameObject.set:name");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setname(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool GameObject_get_loadXML_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGameObject *thiz=dynamic_cast<zbGameObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:GameObject.get:loadXML");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createXMLNode(thiz->getlink()->getloadxml(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool GameObject_remove_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGameObject *thiz=dynamic_cast<zbGameObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:GameObject.remove");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->remove();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool GameObject_get_valid_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbGameObject *thiz=dynamic_cast<zbGameObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:GameObject.get:valid");return;}
  
zbObject *res=createLogical(thiz->valid(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ProxyGameObject_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newproxygameobject(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Mobile_get_name_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMobile *thiz=dynamic_cast<zbMobile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Mobile.get:name");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getname(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Mobile_watch_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMobile *thiz=dynamic_cast<zbMobile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Mobile.watch");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->watch();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Mobile_drive_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMobile *thiz=dynamic_cast<zbMobile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Mobile.drive");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->drive();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Mobile_wait_1_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *secs=params[0];
 
 

    zbMobile *thiz=dynamic_cast<zbMobile*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Mobile.wait");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->wait(secs->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Train_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newtrain(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Train_loadCompose_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[0]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Train.loadCompose,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbTrain *thiz=dynamic_cast<zbTrain*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Train.loadCompose");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->loadcompose(xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Train_saveCompose_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[0]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Train.saveCompose,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbTrain *thiz=dynamic_cast<zbTrain*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Train.saveCompose");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->savecompose(xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Train_goToXY_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

    zbTrain *thiz=dynamic_cast<zbTrain*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Train.goToXY");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->gotoxy(x->toint(),y->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Train_goTo_1_RStation(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbRStation *station=dynamic_cast<zbRStation*>(params[0]);
  if (! station) return false;// {ir->error(fmtstr("ArgError:Train.goTo,expected RStation,found %s",typeid(*params[0]).name()));return;}
 
 

    zbTrain *thiz=dynamic_cast<zbTrain*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Train.goTo");return;}
  
zbObject *res=createLogical(thiz->goTo(station),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Train_reverse_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTrain *thiz=dynamic_cast<zbTrain*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Train.reverse");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->reverse(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Train_run_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTrain *thiz=dynamic_cast<zbTrain*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Train.run");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->unstop();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Train_stop_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTrain *thiz=dynamic_cast<zbTrain*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Train.stop");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->stop();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Train_toDepo_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTrain *thiz=dynamic_cast<zbTrain*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Train.toDepo");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->todepo(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Train_ignore_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTrain *thiz=dynamic_cast<zbTrain*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Train.ignore");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->ignoresems();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Train_get_state_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTrain *thiz=dynamic_cast<zbTrain*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Train.get:state");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getstatestr(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Train_get_speed_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTrain *thiz=dynamic_cast<zbTrain*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Train.get:speed");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getspeed(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Train_get_priority_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTrain *thiz=dynamic_cast<zbTrain*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Train.get:priority");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getpriority(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Train_set_priority_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbTrain *thiz=dynamic_cast<zbTrain*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Train.set:priority");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setpriority(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Train_start_1_RDepo(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbRDepo *depo=dynamic_cast<zbRDepo*>(params[0]);
  if (! depo) return false;// {ir->error(fmtstr("ArgError:Train.start,expected RDepo,found %s",typeid(*params[0]).name()));return;}
 
 

    zbTrain *thiz=dynamic_cast<zbTrain*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Train.start");return;}
  
zbObject *res=createLogical(thiz->start(depo),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Station_get_name_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbStation *thiz=dynamic_cast<zbStation*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Station.get:name");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getname(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Station_set_name_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbStation *thiz=dynamic_cast<zbStation*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Station.set:name");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setname(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RStation_get_type_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbRStation *thiz=dynamic_cast<zbRStation*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RStation.get:type");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettype(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RStation_set_type_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbRStation *thiz=dynamic_cast<zbRStation*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RStation.set:type");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settype(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Aircraft_op_new_1_Airport(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbAirport *start=dynamic_cast<zbAirport*>(params[0]);
  if (! start) return false;// {ir->error(fmtstr("ArgError:Aircraft.op:new,expected Airport,found %s",typeid(*params[0]).name()));return;}
 
 
zbObject *res=newaircraft(start,ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Aircraft_goTo_1_Airport(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbAirport *airport=dynamic_cast<zbAirport*>(params[0]);
  if (! airport) return false;// {ir->error(fmtstr("ArgError:Aircraft.goTo,expected Airport,found %s",typeid(*params[0]).name()));return;}
 
 

    zbAircraft *thiz=dynamic_cast<zbAircraft*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Aircraft.goTo");return;}
  
zbObject *res=createLogical(thiz->goTo(airport),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Aircraft_get_type_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbAircraft *thiz=dynamic_cast<zbAircraft*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Aircraft.get:type");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettype(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Aircraft_set_type_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbAircraft *thiz=dynamic_cast<zbAircraft*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Aircraft.set:type");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settype(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool BusStop_get_type_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbBusStop *thiz=dynamic_cast<zbBusStop*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:BusStop.get:type");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettype(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool BusStop_set_type_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbBusStop *thiz=dynamic_cast<zbBusStop*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:BusStop.set:type");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settype(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Motocar_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newmotocar(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Motocar_goTo_1_BusStop(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbBusStop *stop=dynamic_cast<zbBusStop*>(params[0]);
  if (! stop) return false;// {ir->error(fmtstr("ArgError:Motocar.goTo,expected BusStop,found %s",typeid(*params[0]).name()));return;}
 
 

    zbMotocar *thiz=dynamic_cast<zbMotocar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Motocar.goTo");return;}
  
zbObject *res=createLogical(thiz->goTo(stop),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Motocar_start_1_BusStop(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbBusStop *stop=dynamic_cast<zbBusStop*>(params[0]);
  if (! stop) return false;// {ir->error(fmtstr("ArgError:Motocar.start,expected BusStop,found %s",typeid(*params[0]).name()));return;}
 
 

    zbMotocar *thiz=dynamic_cast<zbMotocar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Motocar.start");return;}
  
zbObject *res=createVoid(ir->rt);thiz->start(stop);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Motocar_get_type_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMotocar *thiz=dynamic_cast<zbMotocar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Motocar.get:type");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettype(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Motocar_set_type_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbMotocar *thiz=dynamic_cast<zbMotocar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Motocar.set:type");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settype(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Motocar_ignore_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMotocar *thiz=dynamic_cast<zbMotocar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Motocar.ignore");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->ignorecross();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Motocar_get_state_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMotocar *thiz=dynamic_cast<zbMotocar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Motocar.get:state");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getstatestr(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Motocar_get_speed_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMotocar *thiz=dynamic_cast<zbMotocar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Motocar.get:speed");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getspeed(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Motocar_run_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMotocar *thiz=dynamic_cast<zbMotocar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Motocar.run");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->unstop();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Motocar_stop_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMotocar *thiz=dynamic_cast<zbMotocar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Motocar.stop");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->stop();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Bridge_get_type_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbBridge *thiz=dynamic_cast<zbBridge*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Bridge.get:type");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettype(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Bridge_set_type_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbBridge *thiz=dynamic_cast<zbBridge*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Bridge.set:type");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settype(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Tunnel_get_type_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTunnel *thiz=dynamic_cast<zbTunnel*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Tunnel.get:type");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettype(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Tunnel_set_type_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbTunnel *thiz=dynamic_cast<zbTunnel*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Tunnel.set:type");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settype(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Building_get_type_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbBuilding *thiz=dynamic_cast<zbBuilding*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Building.get:type");return;}
  
zbObject *res=thiz->gettype()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}






void init_jtobjs_rt(Runtime &rt) {
rt.addsub("Game_op_new_2_int_int",Game_op_new_2_int_int);
rt.addsub("Game_op_new_1_Archive",Game_op_new_1_Archive);
rt.addsub("Game_get_mapX_0",Game_get_mapX_0);
rt.addsub("Game_get_mapY_0",Game_get_mapY_0);
rt.addsub("Game_get_vertexX_0",Game_get_vertexX_0);
rt.addsub("Game_get_vertexY_0",Game_get_vertexY_0);
rt.addsub("Game_get_mouseX_0",Game_get_mouseX_0);
rt.addsub("Game_get_mouseY_0",Game_get_mouseY_0);
rt.addsub("Game_get_mouseInMiddle_0",Game_get_mouseInMiddle_0);
rt.addsub("Game_drawCustomCursor_4_int_int_string_int",Game_drawCustomCursor_4_int_int_string_int);
rt.addsub("Game_drawCustomCursors_6_int_int_int_int_string_int",Game_drawCustomCursors_6_int_int_int_int_string_int);
rt.addsub("Game_drawCursor_3_int_int_string",Game_drawCursor_3_int_int_string);
rt.addsub("Game_drawComplexCursor_3_int_int_string",Game_drawComplexCursor_3_int_int_string);
rt.addsub("Game_drawPointCursor_3_int_int_string",Game_drawPointCursor_3_int_int_string);
rt.addsub("Game_drawPointCursors_5_int_int_int_int_string",Game_drawPointCursors_5_int_int_int_int_string);
rt.addsub("Game_drawSqLineCursor_5_int_int_bool_string_string",Game_drawSqLineCursor_5_int_int_bool_string_string);
rt.addsub("Game_get_handler_0",Game_get_handler_0);
rt.addsub("Game_set_handler_1_Object",Game_set_handler_1_Object);
rt.addsub("Game_end_0",Game_end_0);
rt.addsub("Game_save_1_Archive",Game_save_1_Archive);
rt.addsub("Game_block_1_string",Game_block_1_string);
rt.addsub("Game_unblock_1_string",Game_unblock_1_string);
rt.addsub("Game_buildDepo_5_int_int_int_int_int",Game_buildDepo_5_int_int_int_int_int);
rt.addsub("Game_buildRStation_5_int_int_int_int_int",Game_buildRStation_5_int_int_int_int_int);
rt.addsub("Game_buildBusStop_2_int_int",Game_buildBusStop_2_int_int);
rt.addsub("Game_staticAt_2_int_int",Game_staticAt_2_int_int);
rt.addsub("Game_mobileAt_2_float_float",Game_mobileAt_2_float_float);
rt.addsub("Game_get_statics_0",Game_get_statics_0);
rt.addsub("Game_get_mobiles_0",Game_get_mobiles_0);
rt.addsub("Game_objectType_1_int",Game_objectType_1_int);
rt.addsub("Game_objectFromId_1_int",Game_objectFromId_1_int);
rt.addsub("Game_objectFromName_1_string",Game_objectFromName_1_string);
rt.addsub("Game_generateCircles_5_int_float_float_float_float",Game_generateCircles_5_int_float_float_float_float);
rt.addsub("Game_clearHeights_1_float",Game_clearHeights_1_float);
rt.addsub("Game_buildSemaphore_3_int_int_string",Game_buildSemaphore_3_int_int_string);
rt.addsub("Game_get_machines_0",Game_get_machines_0);
rt.addsub("Game_get_motocars_0",Game_get_motocars_0);
rt.addsub("Game_get_saving_0",Game_get_saving_0);
rt.addsub("Game_loaded_0",Game_loaded_0);
rt.addsub("Game_getHi_2_int_int",Game_getHi_2_int_int);
rt.addsub("Game_setHi_5_int_int_int_int_float",Game_setHi_5_int_int_int_int_float);
rt.addsub("Game_repairHeights_5_int_int_int_int_float",Game_repairHeights_5_int_int_int_int_float);
rt.addsub("Game_eraseRail_2_int_int",Game_eraseRail_2_int_int);
rt.addsub("Game_fillWater_3_int_int_float",Game_fillWater_3_int_int_float);
rt.addsub("Game_dryWater_2_int_int",Game_dryWater_2_int_int);
rt.addsub("Game_buildBridgeExact_7_int_int_int_bool_float_float_string",Game_buildBridgeExact_7_int_int_int_bool_float_float_string);
rt.addsub("Game_buildBridge_7_int_int_int_bool_float_string_string",Game_buildBridge_7_int_int_int_bool_float_string_string);
rt.addsub("Game_buildTunnelExact_7_int_int_int_bool_float_float_string",Game_buildTunnelExact_7_int_int_int_bool_float_float_string);
rt.addsub("Game_buildTunnel_7_int_int_int_bool_float_string_string",Game_buildTunnel_7_int_int_int_bool_float_string_string);
rt.addsub("Game_get_bridgeTypes_0",Game_get_bridgeTypes_0);
rt.addsub("Game_get_tunnelTypes_0",Game_get_tunnelTypes_0);
rt.addsub("Game_get_semaphoreTypes_0",Game_get_semaphoreTypes_0);
rt.addsub("Game_get_stationTypes_0",Game_get_stationTypes_0);
rt.addsub("Game_get_depoTypes_0",Game_get_depoTypes_0);
rt.addsub("Game_get_aircraftTypes_0",Game_get_aircraftTypes_0);
rt.addsub("Game_unWatch_0",Game_unWatch_0);
rt.addsub("Game_clearMobiles_0",Game_clearMobiles_0);
rt.addsub("Game_dryAll_0",Game_dryAll_0);
rt.addsub("Game_plant_3_int_int_string",Game_plant_3_int_int_string);
rt.addsub("Game_get_treeTypes_0",Game_get_treeTypes_0);
rt.addsub("Game_get_buildingTypes_0",Game_get_buildingTypes_0);
rt.addsub("Game_buildBuilding_4_int_int_int_jtBuilding",Game_buildBuilding_4_int_int_int_jtBuilding);
rt.addsub("Game_buildBuildingExact_5_int_int_int_float_jtBuilding",Game_buildBuildingExact_5_int_int_int_float_jtBuilding);
rt.addsub("Game_get_terrainTypes_0",Game_get_terrainTypes_0);
rt.addsub("Game_setTerrain_3_int_int_string",Game_setTerrain_3_int_int_string);
rt.addsub("Game_get_railTypes_0",Game_get_railTypes_0);
rt.addsub("Game_get_roadTypes_0",Game_get_roadTypes_0);
rt.addsub("Game_get_busStopTypes_0",Game_get_busStopTypes_0);
rt.addsub("Game_setRail_3_int_int_string",Game_setRail_3_int_int_string);
rt.addsub("Game_buildRunway_4_int_int_bool_int",Game_buildRunway_4_int_int_bool_int);
rt.addsub("Game_ltop_3_float_float_float",Game_ltop_3_float_float_float);
rt.addsub("Game_drawCircle_5_float_float_float_float_string",Game_drawCircle_5_float_float_float_float_string);
rt.addsub("Game_drawMapCircle_5_float_float_float_float_string",Game_drawMapCircle_5_float_float_float_float_string);
rt.addsub("Game_get_screenMouseX_0",Game_get_screenMouseX_0);
rt.addsub("Game_get_screenMouseY_0",Game_get_screenMouseY_0);
rt.addsub("Game_get_viewportMouseX_0",Game_get_viewportMouseX_0);
rt.addsub("Game_get_viewportMouseY_0",Game_get_viewportMouseY_0);
rt.addsub("Game_riseCircle_4_float_float_float_float",Game_riseCircle_4_float_float_float_float);
rt.addsub("Game_get_deltaTime_0",Game_get_deltaTime_0);
rt.addsub("Game_get_viewType_0",Game_get_viewType_0);
rt.addsub("Game_set_viewType_1_string",Game_set_viewType_1_string);
rt.addsub("Game_lookAt_2_int_int",Game_lookAt_2_int_int);
rt.addsub("Game_storeRRTemplate_5_int_int_int_int_XMLNode",Game_storeRRTemplate_5_int_int_int_int_XMLNode);
rt.addsub("Game_useRRTemplate_4_int_int_int_XMLNode",Game_useRRTemplate_4_int_int_int_XMLNode);
rt.addsub("Game_drawRRTemplate_4_int_int_int_XMLNode",Game_drawRRTemplate_4_int_int_int_XMLNode);
rt.addsub("Game_buildRoad_4_int_int_float_string",Game_buildRoad_4_int_int_float_string);
rt.addsub("Game_eraseRoad_2_int_int",Game_eraseRoad_2_int_int);
rt.addsub("Game_get_machine0_0",Game_get_machine0_0);
rt.addsub("Game_get_motocar0_0",Game_get_motocar0_0);
rt.addsub("Game_setRoad_3_int_int_string",Game_setRoad_3_int_int_string);
rt.addsub("Game_searchMachine_1_string",Game_searchMachine_1_string);
rt.addsub("Game_get_loadMessages_0",Game_get_loadMessages_0);
rt.addsub("Game_addLoadMesasage_1_string",Game_addLoadMesasage_1_string);
rt.addsub("static_Textures_define_2_string_string",static_Textures_define_2_string_string);
rt.addsub("static_Textures_clear_0",static_Textures_clear_0);
rt.addsub("RRBuilder_op_new_2_int_int",RRBuilder_op_new_2_int_int);
rt.addsub("RRBuilder_build_2_int_int",RRBuilder_build_2_int_int);
rt.addsub("RRBuilder_get_X_0",RRBuilder_get_X_0);
rt.addsub("RRBuilder_get_Y_0",RRBuilder_get_Y_0);
rt.addsub("RRBuilder_get_dHeight_0",RRBuilder_get_dHeight_0);
rt.addsub("RRBuilder_set_dHeight_1_float",RRBuilder_set_dHeight_1_float);
rt.addsub("RRBuilder_get_mode_0",RRBuilder_get_mode_0);
rt.addsub("RRBuilder_set_mode_1_string",RRBuilder_set_mode_1_string);
rt.addsub("RRBuilder_get_autoFirstAndLast_0",RRBuilder_get_autoFirstAndLast_0);
rt.addsub("RRBuilder_set_autoFirstAndLast_1_bool",RRBuilder_set_autoFirstAndLast_1_bool);
rt.addsub("GameObject_get_name_0",GameObject_get_name_0);
rt.addsub("GameObject_set_name_1_string",GameObject_set_name_1_string);
rt.addsub("GameObject_get_loadXML_0",GameObject_get_loadXML_0);
rt.addsub("GameObject_remove_0",GameObject_remove_0);
rt.addsub("GameObject_get_valid_0",GameObject_get_valid_0);
rt.addsub("ProxyGameObject_op_new_0",ProxyGameObject_op_new_0);
rt.addsub("Mobile_get_name_0",Mobile_get_name_0);
rt.addsub("Mobile_watch_0",Mobile_watch_0);
rt.addsub("Mobile_drive_0",Mobile_drive_0);
rt.addsub("Mobile_wait_1_float",Mobile_wait_1_float);
rt.addsub("Train_op_new_0",Train_op_new_0);
rt.addsub("Train_loadCompose_1_XMLNode",Train_loadCompose_1_XMLNode);
rt.addsub("Train_saveCompose_1_XMLNode",Train_saveCompose_1_XMLNode);
rt.addsub("Train_goToXY_2_int_int",Train_goToXY_2_int_int);
rt.addsub("Train_goTo_1_RStation",Train_goTo_1_RStation);
rt.addsub("Train_reverse_0",Train_reverse_0);
rt.addsub("Train_run_0",Train_run_0);
rt.addsub("Train_stop_0",Train_stop_0);
rt.addsub("Train_toDepo_0",Train_toDepo_0);
rt.addsub("Train_ignore_0",Train_ignore_0);
rt.addsub("Train_get_state_0",Train_get_state_0);
rt.addsub("Train_get_speed_0",Train_get_speed_0);
rt.addsub("Train_get_priority_0",Train_get_priority_0);
rt.addsub("Train_set_priority_1_int",Train_set_priority_1_int);
rt.addsub("Train_start_1_RDepo",Train_start_1_RDepo);
rt.addsub("Station_get_name_0",Station_get_name_0);
rt.addsub("Station_set_name_1_string",Station_set_name_1_string);
rt.addsub("RStation_get_type_0",RStation_get_type_0);
rt.addsub("RStation_set_type_1_string",RStation_set_type_1_string);
rt.addsub("Aircraft_op_new_1_Airport",Aircraft_op_new_1_Airport);
rt.addsub("Aircraft_goTo_1_Airport",Aircraft_goTo_1_Airport);
rt.addsub("Aircraft_get_type_0",Aircraft_get_type_0);
rt.addsub("Aircraft_set_type_1_string",Aircraft_set_type_1_string);
rt.addsub("BusStop_get_type_0",BusStop_get_type_0);
rt.addsub("BusStop_set_type_1_string",BusStop_set_type_1_string);
rt.addsub("Motocar_op_new_0",Motocar_op_new_0);
rt.addsub("Motocar_goTo_1_BusStop",Motocar_goTo_1_BusStop);
rt.addsub("Motocar_start_1_BusStop",Motocar_start_1_BusStop);
rt.addsub("Motocar_get_type_0",Motocar_get_type_0);
rt.addsub("Motocar_set_type_1_string",Motocar_set_type_1_string);
rt.addsub("Motocar_ignore_0",Motocar_ignore_0);
rt.addsub("Motocar_get_state_0",Motocar_get_state_0);
rt.addsub("Motocar_get_speed_0",Motocar_get_speed_0);
rt.addsub("Motocar_run_0",Motocar_run_0);
rt.addsub("Motocar_stop_0",Motocar_stop_0);
rt.addsub("Bridge_get_type_0",Bridge_get_type_0);
rt.addsub("Bridge_set_type_1_string",Bridge_set_type_1_string);
rt.addsub("Tunnel_get_type_0",Tunnel_get_type_0);
rt.addsub("Tunnel_set_type_1_string",Tunnel_set_type_1_string);
rt.addsub("Building_get_type_0",Building_get_type_0);

}

