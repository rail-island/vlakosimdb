bool Control_get_X_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.get:X");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getX(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_set_X_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.set:X");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->setX(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_get_Y_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.get:Y");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getY(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_set_Y_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.set:Y");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->setY(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_get_W_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.get:W");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getW(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_set_W_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.set:W");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->setW(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_get_H_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.get:H");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getH(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_set_H_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.set:H");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->setH(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_destroy_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.destroy");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->destroy();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_show_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.show");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->show();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_hide_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.hide");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->hide();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_get_visible_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.get:visible");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getvisible(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_set_visible_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.set:visible");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setvisible(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_get_enabled_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.get:enabled");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getenabled(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_set_enabled_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.set:enabled");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setenabled(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_get_parent_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.get:parent");return;}
  
zbObject *res=thiz->getparent()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_set_parent_1_Control(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbControl *value=dynamic_cast<zbControl*>(params[0]);
  if (! value) return false;// {ir->error(fmtstr("ArgError:Control.set:parent,expected Control,found %s",typeid(*params[0]).name()));return;}
 
 

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.set:parent");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setparent(value);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_get_font_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.get:font");return;}
  
zbObject *res=thiz->getfont()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_set_font_1_Font(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbFont *value=dynamic_cast<zbFont*>(params[0]);
  if (! value) return false;// {ir->error(fmtstr("ArgError:Control.set:font,expected Font,found %s",typeid(*params[0]).name()));return;}
 
 

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.set:font");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setfont(value);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_loadProps_2_XMLNode_Form(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[0]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Control.loadProps,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 
zbForm *form=dynamic_cast<zbForm*>(params[1]);
  if (! form) return false;// {ir->error(fmtstr("ArgError:Control.loadProps,expected Form,found %s",typeid(*params[1]).name()));return;}
 
 

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.loadProps");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->loadprops(xml->cvalue(),form->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_saveData_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[0]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Control.saveData,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.saveData");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->savedata(xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_loadData_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[0]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Control.loadData,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.loadData");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->loaddata(xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_clearChilds_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.clearChilds");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->clearchilds();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_get_tag_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.get:tag");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettag(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Control_set_tag_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbControl *thiz=dynamic_cast<zbControl*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Control.set:tag");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settag(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Window_alert_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>65535) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=alert(ir->rt,params+0,parcnt-0)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_get_text_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.get:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettext(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_set_text_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.set:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settext(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newwindow(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_showModal_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.showModal");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->showModal();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_show_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.show");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->show();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_denyClose_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.denyClose");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->denyclose();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_close_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.close");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->close();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_get_style_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.get:style");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getstyle(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_set_style_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.set:style");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setstyle(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_get_X_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.get:X");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getX(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_set_X_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.set:X");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->setX(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_get_Y_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.get:Y");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getY(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_set_Y_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.set:Y");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->setY(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_get_W_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.get:W");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getW(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_set_W_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.set:W");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->setW(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_get_H_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.get:H");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getH(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_set_H_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.set:H");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->setH(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Window_msgbox_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *msg=params[0];
 
 

   zbObject *buttons=params[1];
 
 
zbObject *res=createString(zbmsgbox(msg->tostring(),buttons->tostring(),ir),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Window_msgwin_3_string_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *title=params[0];
 
 

   zbObject *msg=params[1];
 
 

   zbObject *buttons=params[2];
 
 
zbObject *res=createNumber(zbmsgwin(title->tostring(),msg->tostring(),buttons->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Window_inputBox_3_string_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *title=params[0];
 
 

   zbObject *msg=params[1];
 
 

   zbObject *def=params[2];
 
 
zbObject *res=zbinputbox(title->tostring(),msg->tostring(),def->tostring(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Window_get_ppmmx_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=createNumber(getppmmx(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Window_get_ppmmy_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=createNumber(getppmmx(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_loadData_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[0]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Window.loadData,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.loadData");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->loaddata(xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Window_saveData_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[0]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Window.saveData,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbWindow *thiz=dynamic_cast<zbWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Window.saveData");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->savedata(xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Form_loadWindow_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbForm *thiz=dynamic_cast<zbForm*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Form.loadWindow");return;}
  
zbObject *res=thiz->loadWindow(name->tostring(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Form_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newform(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Form_op_new_1_Component(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbComponent *owner=dynamic_cast<zbComponent*>(params[0]);
  if (! owner) return false;// {ir->error(fmtstr("ArgError:Form.op:new,expected Component,found %s",typeid(*params[0]).name()));return;}
 
 
zbObject *res=newform(owner,ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Form_get_handler_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbForm *thiz=dynamic_cast<zbForm*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Form.get:handler");return;}
  
zbObject *res=thiz->gethandler()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Form_set_handler_1_Object(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *value=dynamic_cast<zbObject*>(params[0]);
  if (! value) return false;// {ir->error(fmtstr("ArgError:Form.set:handler,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbForm *thiz=dynamic_cast<zbForm*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Form.set:handler");return;}
  
zbObject *res=createVoid(ir->rt);thiz->sethandler(value);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Form_search_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbForm *thiz=dynamic_cast<zbForm*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Form.search");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createComponent(thiz->getlink()->search(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Form_op_get_prop_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbForm *thiz=dynamic_cast<zbForm*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Form.op:get:prop");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createComponent(thiz->getlink()->search(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Form_destroy_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbForm *thiz=dynamic_cast<zbForm*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Form.destroy");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->destroy();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Button_get_text_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbButton *thiz=dynamic_cast<zbButton*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Button.get:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettext(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Button_set_text_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbButton *thiz=dynamic_cast<zbButton*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Button.set:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settext(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Button_get_onclick_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbButton *thiz=dynamic_cast<zbButton*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Button.get:onclick");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getonclick(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Button_set_onclick_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbButton *thiz=dynamic_cast<zbButton*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Button.set:onclick");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->setonclick(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RadioGroup_get_text_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbRadioGroup *thiz=dynamic_cast<zbRadioGroup*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RadioGroup.get:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettext(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RadioGroup_set_text_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbRadioGroup *thiz=dynamic_cast<zbRadioGroup*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RadioGroup.set:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settext(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RadioGroup_get_items_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbRadioGroup *thiz=dynamic_cast<zbRadioGroup*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RadioGroup.get:items");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getitems(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RadioGroup_set_items_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbRadioGroup *thiz=dynamic_cast<zbRadioGroup*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RadioGroup.set:items");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setitems(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RadioGroup_get_cur_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbRadioGroup *thiz=dynamic_cast<zbRadioGroup*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RadioGroup.get:cur");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getcur(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool RadioGroup_set_cur_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbRadioGroup *thiz=dynamic_cast<zbRadioGroup*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:RadioGroup.set:cur");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setcur(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CheckBox_get_text_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbCheckBox *thiz=dynamic_cast<zbCheckBox*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CheckBox.get:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettext(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CheckBox_set_text_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbCheckBox *thiz=dynamic_cast<zbCheckBox*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CheckBox.set:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settext(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CheckBox_get_checked_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbCheckBox *thiz=dynamic_cast<zbCheckBox*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CheckBox.get:checked");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getchecked(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CheckBox_set_checked_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbCheckBox *thiz=dynamic_cast<zbCheckBox*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CheckBox.set:checked");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setchecked(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SpeedButton_get_text_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbSpeedButton *thiz=dynamic_cast<zbSpeedButton*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SpeedButton.get:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettext(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SpeedButton_set_text_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbSpeedButton *thiz=dynamic_cast<zbSpeedButton*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SpeedButton.set:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settext(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SpeedButton_get_onclick_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbSpeedButton *thiz=dynamic_cast<zbSpeedButton*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SpeedButton.get:onclick");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getonclick(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SpeedButton_set_onclick_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbSpeedButton *thiz=dynamic_cast<zbSpeedButton*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SpeedButton.set:onclick");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->setonclick(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SpeedButton_get_image_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbSpeedButton *thiz=dynamic_cast<zbSpeedButton*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SpeedButton.get:image");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createPicture(thiz->getlink()->getimage(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SpeedButton_set_image_1_Picture(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbPicture *value=dynamic_cast<zbPicture*>(params[0]);
  if (! value) return false;// {ir->error(fmtstr("ArgError:SpeedButton.set:image,expected Picture,found %s",typeid(*params[0]).name()));return;}
 
 

    zbSpeedButton *thiz=dynamic_cast<zbSpeedButton*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SpeedButton.set:image");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setimage(value->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SpeedButton_get_switched_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbSpeedButton *thiz=dynamic_cast<zbSpeedButton*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SpeedButton.get:switched");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getswitched(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SpeedButton_set_switched_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbSpeedButton *thiz=dynamic_cast<zbSpeedButton*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SpeedButton.set:switched");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setswitched(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SpeedButton_op_new_1_Form(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbForm *form=dynamic_cast<zbForm*>(params[0]);
  if (! form) return false;// {ir->error(fmtstr("ArgError:SpeedButton.op:new,expected Form,found %s",typeid(*params[0]).name()));return;}
 
 
zbObject *res=newspeedbutton(form,ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SpeedButton_get_color_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbSpeedButton *thiz=dynamic_cast<zbSpeedButton*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SpeedButton.get:color");return;}
  
zbObject *res=createString(thiz->getcolor(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SpeedButton_set_color_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbSpeedButton *thiz=dynamic_cast<zbSpeedButton*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SpeedButton.set:color");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setcolor(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SpeedGroup_get_cur_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbSpeedGroup *thiz=dynamic_cast<zbSpeedGroup*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SpeedGroup.get:cur");return;}
  
zbObject *res=createNumber(thiz->getcur(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SpeedGroup_set_cur_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbSpeedGroup *thiz=dynamic_cast<zbSpeedGroup*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SpeedGroup.set:cur");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setcur(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ColorTable_get_rows_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbColorTable *thiz=dynamic_cast<zbColorTable*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ColorTable.get:rows");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getrows(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ColorTable_set_rows_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbColorTable *thiz=dynamic_cast<zbColorTable*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ColorTable.set:rows");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setrows(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ColorTable_get_cols_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbColorTable *thiz=dynamic_cast<zbColorTable*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ColorTable.get:cols");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getcols(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ColorTable_set_cols_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbColorTable *thiz=dynamic_cast<zbColorTable*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ColorTable.set:cols");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setcols(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ColorTable_op_get_index_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *row=params[0];
 
 

   zbObject *col=params[1];
 
 

    zbColorTable *thiz=dynamic_cast<zbColorTable*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ColorTable.op:get:index");return;}
  
zbObject *res=createString(thiz->getcolor(row->toint(),col->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ColorTable_op_set_index_3_int_int_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *row=params[0];
 
 

   zbObject *col=params[1];
 
 

   zbObject *right=params[2];
 
 

    zbColorTable *thiz=dynamic_cast<zbColorTable*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ColorTable.op:set:index");return;}
  
zbObject *res=createString(thiz->setcolor(row->toint(),col->toint(),right->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ListBox_get_items_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbListBox *thiz=dynamic_cast<zbListBox*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ListBox.get:items");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getitems(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ListBox_set_items_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbListBox *thiz=dynamic_cast<zbListBox*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ListBox.set:items");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setitems(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ListBox_get_cur_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbListBox *thiz=dynamic_cast<zbListBox*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ListBox.get:cur");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getcur(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ListBox_set_cur_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbListBox *thiz=dynamic_cast<zbListBox*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ListBox.set:cur");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setcur(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ListBox_get_multiSelect_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbListBox *thiz=dynamic_cast<zbListBox*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ListBox.get:multiSelect");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getmultiselect(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ListBox_set_multiSelect_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbListBox *thiz=dynamic_cast<zbListBox*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ListBox.set:multiSelect");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setmultiselect(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ListBox_get_selected_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbListBox *thiz=dynamic_cast<zbListBox*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ListBox.get:selected");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getselected(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ListBox_set_selected_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbListBox *thiz=dynamic_cast<zbListBox*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ListBox.set:selected");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setselected(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PackedList_get_items_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPackedList *thiz=dynamic_cast<zbPackedList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PackedList.get:items");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getitems(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PackedList_set_items_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbPackedList *thiz=dynamic_cast<zbPackedList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PackedList.set:items");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setitems(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PackedList_get_cur_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPackedList *thiz=dynamic_cast<zbPackedList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PackedList.get:cur");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getcur(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PackedList_set_cur_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbPackedList *thiz=dynamic_cast<zbPackedList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PackedList.set:cur");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setcur(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Edit_get_text_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbEdit *thiz=dynamic_cast<zbEdit*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Edit.get:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettext(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Edit_set_text_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbEdit *thiz=dynamic_cast<zbEdit*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Edit.set:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settext(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Edit_get_modified_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbEdit *thiz=dynamic_cast<zbEdit*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Edit.get:modified");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getmodified(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Edit_set_modified_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbEdit *thiz=dynamic_cast<zbEdit*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Edit.set:modified");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setmodified(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Memo_get_text_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMemo *thiz=dynamic_cast<zbMemo*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Memo.get:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettext(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Memo_set_text_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbMemo *thiz=dynamic_cast<zbMemo*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Memo.set:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settext(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Memo_get_modified_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMemo *thiz=dynamic_cast<zbMemo*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Memo.get:modified");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getmodified(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Memo_set_modified_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbMemo *thiz=dynamic_cast<zbMemo*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Memo.set:modified");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setmodified(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Label_get_text_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbLabel *thiz=dynamic_cast<zbLabel*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Label.get:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettext(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Label_set_text_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbLabel *thiz=dynamic_cast<zbLabel*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Label.set:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settext(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool WrapLabel_get_text_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbWrapLabel *thiz=dynamic_cast<zbWrapLabel*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:WrapLabel.get:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettext(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool WrapLabel_set_text_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbWrapLabel *thiz=dynamic_cast<zbWrapLabel*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:WrapLabel.set:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settext(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TrackBar_get_min_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTrackBar *thiz=dynamic_cast<zbTrackBar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:TrackBar.get:min");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getmin(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TrackBar_set_min_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbTrackBar *thiz=dynamic_cast<zbTrackBar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:TrackBar.set:min");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setmin(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TrackBar_get_max_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTrackBar *thiz=dynamic_cast<zbTrackBar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:TrackBar.get:max");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getmax(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TrackBar_set_max_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbTrackBar *thiz=dynamic_cast<zbTrackBar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:TrackBar.set:max");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setmax(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TrackBar_get_cur_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTrackBar *thiz=dynamic_cast<zbTrackBar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:TrackBar.get:cur");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getcur(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TrackBar_set_cur_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbTrackBar *thiz=dynamic_cast<zbTrackBar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:TrackBar.set:cur");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setcur(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TrackBar_get_frequency_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTrackBar *thiz=dynamic_cast<zbTrackBar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:TrackBar.get:frequency");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getfrequency(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TrackBar_set_frequency_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbTrackBar *thiz=dynamic_cast<zbTrackBar*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:TrackBar.set:frequency");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setfrequency(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Table_set_model_1_TableModel(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbTableModel *value=dynamic_cast<zbTableModel*>(params[0]);
  if (! value) return false;// {ir->error(fmtstr("ArgError:Table.set:model,expected TableModel,found %s",typeid(*params[0]).name()));return;}
 
 

    zbTable *thiz=dynamic_cast<zbTable*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Table.set:model");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setmodel(value);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Table_get_selrow_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTable *thiz=dynamic_cast<zbTable*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Table.get:selrow");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getselrow(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Table_set_selrow_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbTable *thiz=dynamic_cast<zbTable*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Table.set:selrow");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setselrow(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Table_get_selcol_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTable *thiz=dynamic_cast<zbTable*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Table.get:selcol");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getselcol(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Table_set_selcol_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbTable *thiz=dynamic_cast<zbTable*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Table.set:selcol");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setselcol(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Table_repaint_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTable *thiz=dynamic_cast<zbTable*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Table.repaint");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->repaint();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Table_repaintLine_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *line=params[0];
 
 

    zbTable *thiz=dynamic_cast<zbTable*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Table.repaintLine");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->repaintline(line->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Table_update_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTable *thiz=dynamic_cast<zbTable*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Table.update");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->update();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CheckList_get_cur_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbCheckList *thiz=dynamic_cast<zbCheckList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CheckList.get:cur");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getcur(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CheckList_set_cur_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbCheckList *thiz=dynamic_cast<zbCheckList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CheckList.set:cur");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setcur(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CheckList_get_items_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbCheckList *thiz=dynamic_cast<zbCheckList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CheckList.get:items");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getitems(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CheckList_set_items_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbCheckList *thiz=dynamic_cast<zbCheckList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CheckList.set:items");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setitems(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CheckList_get_checked_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbCheckList *thiz=dynamic_cast<zbCheckList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CheckList.get:checked");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getchecked(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool CheckList_set_checked_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbCheckList *thiz=dynamic_cast<zbCheckList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:CheckList.set:checked");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setchecked(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TableCols_add_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *fldid=params[0];
 
 

   zbObject *text=params[1];
 
 

    zbTableCols *thiz=dynamic_cast<zbTableCols*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:TableCols.add");return;}
  
zbObject *res=createVoid(ir->rt);thiz->add(fldid->tostring(),text->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TableCols_add_3_string_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *fldid=params[0];
 
 

   zbObject *text=params[1];
 
 

   zbObject *type=params[2];
 
 

    zbTableCols *thiz=dynamic_cast<zbTableCols*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:TableCols.add");return;}
  
zbObject *res=createVoid(ir->rt);thiz->add(fldid->tostring(),text->tostring(),type->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Tree_set_source_1_TreeSource(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbTreeSource *value=dynamic_cast<zbTreeSource*>(params[0]);
  if (! value) return false;// {ir->error(fmtstr("ArgError:Tree.set:source,expected TreeSource,found %s",typeid(*params[0]).name()));return;}
 
 

    zbTree *thiz=dynamic_cast<zbTree*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Tree.set:source");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setsource(value);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Tree_update_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTree *thiz=dynamic_cast<zbTree*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Tree.update");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->update();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Tree_get_active_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTree *thiz=dynamic_cast<zbTree*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Tree.get:active");return;}
  
zbObject *res=thiz->getactive()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Tree_set_active_1_MultiIndex(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbMultiIndex *value=dynamic_cast<zbMultiIndex*>(params[0]);
  if (! value) return false;// {ir->error(fmtstr("ArgError:Tree.set:active,expected MultiIndex,found %s",typeid(*params[0]).name()));return;}
 
 

    zbTree *thiz=dynamic_cast<zbTree*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Tree.set:active");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setactive(value);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Tree_expand_1_MultiIndex(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbMultiIndex *item=dynamic_cast<zbMultiIndex*>(params[0]);
  if (! item) return false;// {ir->error(fmtstr("ArgError:Tree.expand,expected MultiIndex,found %s",typeid(*params[0]).name()));return;}
 
 

    zbTree *thiz=dynamic_cast<zbTree*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Tree.expand");return;}
  
zbObject *res=createVoid(ir->rt);thiz->expand(item);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Tree_collapse_1_MultiIndex(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbMultiIndex *item=dynamic_cast<zbMultiIndex*>(params[0]);
  if (! item) return false;// {ir->error(fmtstr("ArgError:Tree.collapse,expected MultiIndex,found %s",typeid(*params[0]).name()));return;}
 
 

    zbTree *thiz=dynamic_cast<zbTree*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Tree.collapse");return;}
  
zbObject *res=createVoid(ir->rt);thiz->collapse(item);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PaintBox_get_onpaint_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPaintBox *thiz=dynamic_cast<zbPaintBox*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PaintBox.get:onpaint");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getonpaint(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PaintBox_set_onpaint_1_String(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbString *value=dynamic_cast<zbString*>(params[0]);
  if (! value) return false;// {ir->error(fmtstr("ArgError:PaintBox.set:onpaint,expected String,found %s",typeid(*params[0]).name()));return;}
 
 

    zbPaintBox *thiz=dynamic_cast<zbPaintBox*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PaintBox.set:onpaint");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->setonpaint(value->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PaintBox_repaint_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPaintBox *thiz=dynamic_cast<zbPaintBox*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PaintBox.repaint");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->repaint();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ScrolledPainter_get_onpaint_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbScrolledPainter *thiz=dynamic_cast<zbScrolledPainter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ScrolledPainter.get:onpaint");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getonpaint(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ScrolledPainter_set_onpaint_1_String(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbString *value=dynamic_cast<zbString*>(params[0]);
  if (! value) return false;// {ir->error(fmtstr("ArgError:ScrolledPainter.set:onpaint,expected String,found %s",typeid(*params[0]).name()));return;}
 
 

    zbScrolledPainter *thiz=dynamic_cast<zbScrolledPainter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ScrolledPainter.set:onpaint");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->setonpaint(value->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ScrolledPainter_setInSize_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *w=params[0];
 
 

   zbObject *h=params[1];
 
 

    zbScrolledPainter *thiz=dynamic_cast<zbScrolledPainter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ScrolledPainter.setInSize");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->setInSize(w->toint(),h->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ScrolledPainter_repaint_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbScrolledPainter *thiz=dynamic_cast<zbScrolledPainter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ScrolledPainter.repaint");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->repaint();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PropertyList_save_1_VarList(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbVarList *vars=dynamic_cast<zbVarList*>(params[0]);
  if (! vars) return false;// {ir->error(fmtstr("ArgError:PropertyList.save,expected VarList,found %s",typeid(*params[0]).name()));return;}
 
 

    zbPropertyList *thiz=dynamic_cast<zbPropertyList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PropertyList.save");return;}
  
zbObject *res=createVoid(ir->rt);thiz->save(vars->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PropertyList_load_1_VarList(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbVarList *vars=dynamic_cast<zbVarList*>(params[0]);
  if (! vars) return false;// {ir->error(fmtstr("ArgError:PropertyList.load,expected VarList,found %s",typeid(*params[0]).name()));return;}
 
 

    zbPropertyList *thiz=dynamic_cast<zbPropertyList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PropertyList.load");return;}
  
zbObject *res=createVoid(ir->rt);thiz->load(vars->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PropertyList_addVar_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbPropertyList *thiz=dynamic_cast<zbPropertyList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PropertyList.addVar");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->addVar(name->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PropertyList_eraseVar_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPropertyList *thiz=dynamic_cast<zbPropertyList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PropertyList.eraseVar");return;}
  
zbObject *res=createVoid(ir->rt);thiz->eraseVar();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PropertyList_get_modified_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPropertyList *thiz=dynamic_cast<zbPropertyList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PropertyList.get:modified");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getmodified(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PropertyList_set_modified_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbPropertyList *thiz=dynamic_cast<zbPropertyList*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PropertyList.set:modified");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setmodified(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Pager_get_cur_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPager *thiz=dynamic_cast<zbPager*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Pager.get:cur");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getcur(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Pager_set_cur_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbPager *thiz=dynamic_cast<zbPager*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Pager.set:cur");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setcur(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Settings_createAll_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbSettings *thiz=dynamic_cast<zbSettings*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Settings.createAll");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->createall();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool HTMLView_get_text_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbHTMLView *thiz=dynamic_cast<zbHTMLView*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:HTMLView.get:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettext(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool HTMLView_set_text_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbHTMLView *thiz=dynamic_cast<zbHTMLView*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:HTMLView.set:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settext(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool HTMLView_searchedPicture_2_string_Picture(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *src=params[0];
 
 
zbPicture *picture=dynamic_cast<zbPicture*>(params[1]);
  if (! picture) return false;// {ir->error(fmtstr("ArgError:HTMLView.searchedPicture,expected Picture,found %s",typeid(*params[1]).name()));return;}
 
 

    zbHTMLView *thiz=dynamic_cast<zbHTMLView*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:HTMLView.searchedPicture");return;}
  
zbObject *res=createVoid(ir->rt);thiz->searchedPicture(src->tostring(),picture);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_line_4_int_int_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<4 || parcnt>4) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x1=params[0];
 
 

   zbObject *y1=params[1];
 
 

   zbObject *x2=params[2];
 
 

   zbObject *y2=params[3];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.line");return;}
  if(!thiz->getlink()) return false;
zbObject *res=thiz->addref();thiz->getlink()->line(x1->toint(),y1->toint(),x2->toint(),y2->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_rectangle_4_int_int_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<4 || parcnt>4) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x1=params[0];
 
 

   zbObject *y1=params[1];
 
 

   zbObject *x2=params[2];
 
 

   zbObject *y2=params[3];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.rectangle");return;}
  if(!thiz->getlink()) return false;
zbObject *res=thiz->addref();thiz->getlink()->rect(x1->toint(),y1->toint(),x2->toint(),y2->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_ellipse_4_int_int_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<4 || parcnt>4) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x1=params[0];
 
 

   zbObject *y1=params[1];
 
 

   zbObject *x2=params[2];
 
 

   zbObject *y2=params[3];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.ellipse");return;}
  if(!thiz->getlink()) return false;
zbObject *res=thiz->addref();thiz->getlink()->ellipse(x1->toint(),y1->toint(),x2->toint(),y2->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_fg_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *color=params[0];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.fg");return;}
  if(!thiz->getlink()) return false;
zbObject *res=thiz->addref();thiz->fg(color->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_bg_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *color=params[0];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.bg");return;}
  if(!thiz->getlink()) return false;
zbObject *res=thiz->addref();thiz->bg(color->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_text_3_int_int_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *str=params[2];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=thiz->addref();thiz->getlink()->text(x->toint(),y->toint(),str->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_textwi_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *str=params[0];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.textwi");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->textwi(str->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_texthi_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *str=params[0];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.texthi");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->texthi(str->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_font_1_Font(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbFont *font=dynamic_cast<zbFont*>(params[0]);
  if (! font) return false;// {ir->error(fmtstr("ArgError:Canvas.font,expected Font,found %s",typeid(*params[0]).name()));return;}
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.font");return;}
  if(!thiz->getlink()) return false;
zbObject *res=thiz->addref();thiz->font(font);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_penwi_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *wi=params[0];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.penwi");return;}
  if(!thiz->getlink()) return false;
zbObject *res=thiz->addref();thiz->getlink()->penwi(wi->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_drawFrame_5_int_int_int_int_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x1=params[0];
 
 

   zbObject *y1=params[1];
 
 

   zbObject *x2=params[2];
 
 

   zbObject *y2=params[3];
 
 

   zbObject *type=params[4];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.drawFrame");return;}
  
zbObject *res=thiz->addref();thiz->drawFrame(x1->toint(),y1->toint(),x2->toint(),y2->toint(),type->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_centerText_6_int_int_int_int_string_Store(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<6 || parcnt>6) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x1=params[0];
 
 

   zbObject *y1=params[1];
 
 

   zbObject *x2=params[2];
 
 

   zbObject *y2=params[3];
 
 

   zbObject *text=params[4];
 
 
zbStore *flags=dynamic_cast<zbStore*>(params[5]);
  if (! flags) return false;// {ir->error(fmtstr("ArgError:Canvas.centerText,expected Store,found %s",typeid(*params[5]).name()));return;}
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.centerText");return;}
  
zbObject *res=thiz->addref();thiz->centerText(x1->toint(),y1->toint(),x2->toint(),y2->toint(),text->tostring(),flags);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_centerText_5_int_int_int_int_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x1=params[0];
 
 

   zbObject *y1=params[1];
 
 

   zbObject *x2=params[2];
 
 

   zbObject *y2=params[3];
 
 

   zbObject *text=params[4];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.centerText");return;}
  
zbObject *res=thiz->addref();thiz->centerText(x1->toint(),y1->toint(),x2->toint(),y2->toint(),text->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_nextBand_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *hi=params[0];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.nextBand");return;}
  if(!thiz->getlink()) return false;
zbObject *res=thiz->nextBand(hi->toint())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_nextBand_2_int_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *hi=params[0];
 
 

   zbObject *type=params[1];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.nextBand");return;}
  if(!thiz->getlink()) return false;
zbObject *res=thiz->nextBand(hi->toint(),type->tostring())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_drawRadio_3_int_int_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *checked=params[2];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.drawRadio");return;}
  if(!thiz->getlink()) return false;
zbObject *res=thiz->addref();thiz->getlink()->drawradio(x->toint(),y->toint(),checked->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_drawCheck_3_int_int_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *checked=params[2];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.drawCheck");return;}
  if(!thiz->getlink()) return false;
zbObject *res=thiz->addref();thiz->getlink()->drawcheck(x->toint(),y->toint(),checked->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_op_get_index_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.op:get:index");return;}
  
zbObject *res=createString(thiz->getpixel(x->toint(),y->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_op_set_index_3_int_int_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *right=params[2];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.op:set:index");return;}
  
zbObject *res=createString(thiz->putpixel(x->toint(),y->toint(),right->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_drawSelection_4_int_int_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<4 || parcnt>4) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x1=params[0];
 
 

   zbObject *y1=params[1];
 
 

   zbObject *x2=params[2];
 
 

   zbObject *y2=params[3];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.drawSelection");return;}
  
zbObject *res=createVoid(ir->rt);thiz->drawSelection(x1->toint(),y1->toint(),x2->toint(),y2->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_randomLine_4_int_int_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<4 || parcnt>4) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x1=params[0];
 
 

   zbObject *y1=params[1];
 
 

   zbObject *x2=params[2];
 
 

   zbObject *y2=params[3];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.randomLine");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->randomline(x1->toint(),y1->toint(),x2->toint(),y2->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Canvas_floodFill_4_int_int_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<4 || parcnt>4) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *mode=params[2];
 
 

   zbObject *color=params[3];
 
 

    zbCanvas *thiz=dynamic_cast<zbCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Canvas.floodFill");return;}
  
zbObject *res=createVoid(ir->rt);thiz->floodfill(x->toint(),y->toint(),mode->tostring(),color->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_get_name_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbFont *thiz=dynamic_cast<zbFont*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Font.get:name");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getname(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_set_name_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbFont *thiz=dynamic_cast<zbFont*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Font.set:name");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setname(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_get_size_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbFont *thiz=dynamic_cast<zbFont*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Font.get:size");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getsize(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_set_size_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbFont *thiz=dynamic_cast<zbFont*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Font.set:size");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setsize(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_get_bold_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbFont *thiz=dynamic_cast<zbFont*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Font.get:bold");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getbold(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_set_bold_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbFont *thiz=dynamic_cast<zbFont*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Font.set:bold");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setbold(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_get_underline_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbFont *thiz=dynamic_cast<zbFont*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Font.get:underline");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getunderline(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_set_underline_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbFont *thiz=dynamic_cast<zbFont*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Font.set:underline");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setunderline(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_get_strikeout_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbFont *thiz=dynamic_cast<zbFont*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Font.get:strikeout");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getstrikeout(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_set_strikeout_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbFont *thiz=dynamic_cast<zbFont*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Font.set:strikeout");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setstrikeout(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_get_italic_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbFont *thiz=dynamic_cast<zbFont*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Font.get:italic");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getitalic(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_set_italic_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbFont *thiz=dynamic_cast<zbFont*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Font.set:italic");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setitalic(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_edit_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbFont *thiz=dynamic_cast<zbFont*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Font.edit");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->edit(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newfont(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_load_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[0]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Font.load,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbFont *thiz=dynamic_cast<zbFont*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Font.load");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->load(xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_save_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[0]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Font.save,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbFont *thiz=dynamic_cast<zbFont*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Font.save");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->save(xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_getHeight_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbFont *thiz=dynamic_cast<zbFont*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Font.getHeight");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getheight(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Font_getWidth_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *text=params[0];
 
 

    zbFont *thiz=dynamic_cast<zbFont*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Font.getWidth");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getwidth(text->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Picture_load_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *file=params[0];
 
 
try {
    zbPicture *thiz=dynamic_cast<zbPicture*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Picture.load");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->load(file->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  } catch(JException &e) {
    ir->nativeerror(e.getmsg());
    return true;
  }
  
  return true;

}
bool Picture_save_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *file=params[0];
 
 
try {
    zbPicture *thiz=dynamic_cast<zbPicture*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Picture.save");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->save(file->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  } catch(JException &e) {
    ir->nativeerror(e.getmsg());
    return true;
  }
  
  return true;

}
bool Picture_get_canvas_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPicture *thiz=dynamic_cast<zbPicture*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Picture.get:canvas");return;}
  
zbObject *res=thiz->getcanvas()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Picture_draw_3_int_int_Canvas(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 
zbCanvas *cnv=dynamic_cast<zbCanvas*>(params[2]);
  if (! cnv) return false;// {ir->error(fmtstr("ArgError:Picture.draw,expected Canvas,found %s",typeid(*params[2]).name()));return;}
 
 

    zbPicture *thiz=dynamic_cast<zbPicture*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Picture.draw");return;}
  
zbObject *res=createVoid(ir->rt);thiz->draw(x->toint(),y->toint(),cnv->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Picture_stretch_5_int_int_int_int_Canvas(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<5 || parcnt>5) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *w=params[2];
 
 

   zbObject *h=params[3];
 
 
zbCanvas *cnv=dynamic_cast<zbCanvas*>(params[4]);
  if (! cnv) return false;// {ir->error(fmtstr("ArgError:Picture.stretch,expected Canvas,found %s",typeid(*params[4]).name()));return;}
 
 

    zbPicture *thiz=dynamic_cast<zbPicture*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Picture.stretch");return;}
  
zbObject *res=createVoid(ir->rt);thiz->stretch(x->toint(),y->toint(),w->toint(),h->toint(),cnv->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Picture_stretchRectangle_9_int_int_int_int_int_int_int_int_Canvas(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<9 || parcnt>9) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *x=params[0];
 
 

   zbObject *y=params[1];
 
 

   zbObject *w=params[2];
 
 

   zbObject *h=params[3];
 
 

   zbObject *srcx=params[4];
 
 

   zbObject *srcy=params[5];
 
 

   zbObject *srcw=params[6];
 
 

   zbObject *srch=params[7];
 
 
zbCanvas *cnv=dynamic_cast<zbCanvas*>(params[8]);
  if (! cnv) return false;// {ir->error(fmtstr("ArgError:Picture.stretchRectangle,expected Canvas,found %s",typeid(*params[8]).name()));return;}
 
 

    zbPicture *thiz=dynamic_cast<zbPicture*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Picture.stretchRectangle");return;}
  
zbObject *res=createVoid(ir->rt);thiz->stretchRectangle(x->toint(),y->toint(),w->toint(),h->toint(),srcx->toint(),srcy->toint(),srcw->toint(),srch->toint(),cnv->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Picture_hflip_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPicture *thiz=dynamic_cast<zbPicture*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Picture.hflip");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->hflip();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Picture_vflip_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPicture *thiz=dynamic_cast<zbPicture*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Picture.vflip");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->vflip();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Picture_transp_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPicture *thiz=dynamic_cast<zbPicture*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Picture.transp");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->transp();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Picture_get_W_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPicture *thiz=dynamic_cast<zbPicture*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Picture.get:W");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getw(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Picture_set_W_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbPicture *thiz=dynamic_cast<zbPicture*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Picture.set:W");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setw(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Picture_get_H_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPicture *thiz=dynamic_cast<zbPicture*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Picture.get:H");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->geth(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Picture_set_H_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbPicture *thiz=dynamic_cast<zbPicture*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Picture.set:H");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->seth(value->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Picture_invert_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPicture *thiz=dynamic_cast<zbPicture*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Picture.invert");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->invert();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Picture_clone_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPicture *thiz=dynamic_cast<zbPicture*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Picture.clone");return;}
  
zbObject *res=thiz->clone()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Picture_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newpicture(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_run_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.run");return;}
  
zbObject *res=createVoid(ir->rt);thiz->runapp(ir);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_exit_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.exit");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->quit();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_loadResource_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *files=params[0];
 
 

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.loadResource");return;}
  
zbObject *res=createVoid(ir->rt);thiz->loadresource(files->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_get_options_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.get:options");return;}
  
zbObject *res=createXMLNode(thiz->getopts(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_addSearchPath_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *path=params[0];
 
 

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.addSearchPath");return;}
  
zbObject *res=createVoid(ir->rt);thiz->addSearchPath(path->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_setOptionFile_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *file=params[0];
 
 

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.setOptionFile");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setOptionFile(file->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_runXMLDialog_2_XMLNode_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *dialog=dynamic_cast<zbXMLNode*>(params[0]);
  if (! dialog) return false;// {ir->error(fmtstr("ArgError:ApplicationClass.runXMLDialog,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 
zbXMLNode *data=dynamic_cast<zbXMLNode*>(params[1]);
  if (! data) return false;// {ir->error(fmtstr("ArgError:ApplicationClass.runXMLDialog,expected XMLNode,found %s",typeid(*params[1]).name()));return;}
 
 

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.runXMLDialog");return;}
  
zbObject *res=createLogical(thiz->runxmldialog(dialog->cvalue(),data->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_findResource_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.findResource");return;}
  
zbObject *res=thiz->findresource(name->tostring(),ir)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_runSettingsDialog_2_string_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *dialog=params[0];
 
 
zbXMLNode *data=dynamic_cast<zbXMLNode*>(params[1]);
  if (! data) return false;// {ir->error(fmtstr("ArgError:ApplicationClass.runSettingsDialog,expected XMLNode,found %s",typeid(*params[1]).name()));return;}
 
 

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.runSettingsDialog");return;}
  
zbObject *res=createLogical(thiz->runsettingsdialog(dialog->tostring(),data->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_get_title_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.get:title");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettitle(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_set_title_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.set:title");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settitle(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_get_systemMenu_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.get:systemMenu");return;}
  
zbObject *res=thiz->getsysmenu(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_set_systemMenu_1_Object(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *value=dynamic_cast<zbObject*>(params[0]);
  if (! value) return false;// {ir->error(fmtstr("ArgError:ApplicationClass.set:systemMenu,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.set:systemMenu");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setsysmenu(value);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_get_handler_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.get:handler");return;}
  
zbObject *res=thiz->gethandler()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_set_handler_1_Object(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *value=dynamic_cast<zbObject*>(params[0]);
  if (! value) return false;// {ir->error(fmtstr("ArgError:ApplicationClass.set:handler,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.set:handler");return;}
  
zbObject *res=createVoid(ir->rt);thiz->sethandler(value);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_get_onIdle_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.get:onIdle");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getidle(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_set_onIdle_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.set:onIdle");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setidle(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_get_onDesktopMouseDown_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.get:onDesktopMouseDown");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getdesktopmousedown(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_set_onDesktopMouseDown_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.set:onDesktopMouseDown");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setdesktopmousedown(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_get_onDesktopMouseUp_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.get:onDesktopMouseUp");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getdesktopmouseup(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_set_onDesktopMouseUp_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.set:onDesktopMouseUp");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setdesktopmouseup(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_get_onDesktopMouseMove_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.get:onDesktopMouseMove");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getdesktopmousemove(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_set_onDesktopMouseMove_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.set:onDesktopMouseMove");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setdesktopmousemove(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_get_desktopW_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.get:desktopW");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getdesktopw(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_get_desktopH_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.get:desktopH");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getdesktoph(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ApplicationClass_get_shiftState_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbApplicationClass *thiz=dynamic_cast<zbApplicationClass*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ApplicationClass.get:shiftState");return;}
  
zbObject *res=thiz->getshiftState()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Printer_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newprinter(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Printer_getInfo_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPrinter *thiz=dynamic_cast<zbPrinter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Printer.getInfo");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createCanvas(thiz->getlink()->getinfo(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Printer_loadProperties_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[0]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Printer.loadProperties,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbPrinter *thiz=dynamic_cast<zbPrinter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Printer.loadProperties");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->loadprops(xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Printer_saveProperties_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[0]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Printer.saveProperties,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbPrinter *thiz=dynamic_cast<zbPrinter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Printer.saveProperties");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->saveprops(xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Printer_get_pghi_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPrinter *thiz=dynamic_cast<zbPrinter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Printer.get:pghi");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getpghi(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Printer_get_pgwi_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPrinter *thiz=dynamic_cast<zbPrinter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Printer.get:pgwi");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getpgwi(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Printer_beginPage_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPrinter *thiz=dynamic_cast<zbPrinter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Printer.beginPage");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createCanvas(thiz->getlink()->beginpage(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Printer_endPage_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPrinter *thiz=dynamic_cast<zbPrinter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Printer.endPage");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->endpage();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Printer_makeFont_1_Font(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbFont *src=dynamic_cast<zbFont*>(params[0]);
  if (! src) return false;// {ir->error(fmtstr("ArgError:Printer.makeFont,expected Font,found %s",typeid(*params[0]).name()));return;}
 
 

    zbPrinter *thiz=dynamic_cast<zbPrinter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Printer.makeFont");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createFont(thiz->getlink()->createprnfont(src->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Printer_setup_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPrinter *thiz=dynamic_cast<zbPrinter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Printer.setup");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setup();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Printer_detailSetup_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPrinter *thiz=dynamic_cast<zbPrinter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Printer.detailSetup");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->detailsetup();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Printer_beginDoc_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *title=params[0];
 
 

    zbPrinter *thiz=dynamic_cast<zbPrinter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Printer.beginDoc");return;}
  
zbObject *res=createVoid(ir->rt);thiz->beginDoc(title->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Printer_beginFileDoc_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *title=params[0];
 
 

   zbObject *file=params[1];
 
 

    zbPrinter *thiz=dynamic_cast<zbPrinter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Printer.beginFileDoc");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->begindoc(title->tostring(),file->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Printer_endDoc_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPrinter *thiz=dynamic_cast<zbPrinter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Printer.endDoc");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->enddoc();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Printer_destroy_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPrinter *thiz=dynamic_cast<zbPrinter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Printer.destroy");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->destroy();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Printer_get_ppmmx_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPrinter *thiz=dynamic_cast<zbPrinter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Printer.get:ppmmx");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getppmmx(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Printer_get_ppmmy_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPrinter *thiz=dynamic_cast<zbPrinter*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Printer.get:ppmmy");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getppmmy(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Console_print_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>65535) return false;
  zbObject **params=ir->getparams(parcnt);

    zbConsole *thiz=dynamic_cast<zbConsole*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Console.print");return;}
  
zbObject *res=createVoid(ir->rt);thiz->print(params+0,parcnt-0);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Console_println_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>65535) return false;
  zbObject **params=ir->getparams(parcnt);

    zbConsole *thiz=dynamic_cast<zbConsole*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Console.println");return;}
  
zbObject *res=createVoid(ir->rt);thiz->println(params+0,parcnt-0);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Console_readln_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbConsole *thiz=dynamic_cast<zbConsole*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Console.readln");return;}
  
zbObject *res=createString(thiz->readln(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Console_close_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbConsole *thiz=dynamic_cast<zbConsole*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Console.close");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->close();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Console_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newconsole(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Console_wait_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbConsole *thiz=dynamic_cast<zbConsole*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Console.wait");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->wait();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Console_get_std_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=getstdcon(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Console_destroy_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbConsole *thiz=dynamic_cast<zbConsole*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Console.destroy");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->destroy();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Key_op_eq_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *key=params[0];
 
 

    zbKey *thiz=dynamic_cast<zbKey*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Key.op:eq");return;}
  
zbObject *res=createLogical(thiz->opeq(key->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Key_op_neq_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *key=params[0];
 
 

    zbKey *thiz=dynamic_cast<zbKey*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Key.op:neq");return;}
  
zbObject *res=createLogical(thiz->opneq(key->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Key_op_new_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *keyname=params[0];
 
 
zbObject *res=newkey(keyname->tostring(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Key_get_shift_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbKey *thiz=dynamic_cast<zbKey*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Key.get:shift");return;}
  
zbObject *res=createLogical(thiz->getshift(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Key_get_alt_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbKey *thiz=dynamic_cast<zbKey*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Key.get:alt");return;}
  
zbObject *res=createLogical(thiz->getalt(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Key_get_ctrl_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbKey *thiz=dynamic_cast<zbKey*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Key.get:ctrl");return;}
  
zbObject *res=createLogical(thiz->getctrl(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Key_get_lshift_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbKey *thiz=dynamic_cast<zbKey*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Key.get:lshift");return;}
  
zbObject *res=createLogical(thiz->getlshift(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Key_get_rshift_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbKey *thiz=dynamic_cast<zbKey*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Key.get:rshift");return;}
  
zbObject *res=createLogical(thiz->getrshift(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Key_get_key_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbKey *thiz=dynamic_cast<zbKey*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Key.get:key");return;}
  
zbObject *res=createString(thiz->getkey(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ShiftState_get_shift_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbShiftState *thiz=dynamic_cast<zbShiftState*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ShiftState.get:shift");return;}
  
zbObject *res=createLogical(thiz->getshift(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ShiftState_get_alt_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbShiftState *thiz=dynamic_cast<zbShiftState*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ShiftState.get:alt");return;}
  
zbObject *res=createLogical(thiz->getalt(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ShiftState_get_ctrl_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbShiftState *thiz=dynamic_cast<zbShiftState*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ShiftState.get:ctrl");return;}
  
zbObject *res=createLogical(thiz->getctrl(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ShiftState_get_lshift_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbShiftState *thiz=dynamic_cast<zbShiftState*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ShiftState.get:lshift");return;}
  
zbObject *res=createLogical(thiz->getlshift(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ShiftState_get_rshift_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbShiftState *thiz=dynamic_cast<zbShiftState*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ShiftState.get:rshift");return;}
  
zbObject *res=createLogical(thiz->getrshift(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Timer_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newtimer(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Timer_get_handler_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTimer *thiz=dynamic_cast<zbTimer*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Timer.get:handler");return;}
  
zbObject *res=thiz->gethandler()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Timer_set_handler_1_Object(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *value=dynamic_cast<zbObject*>(params[0]);
  if (! value) return false;// {ir->error(fmtstr("ArgError:Timer.set:handler,expected Object,found %s",typeid(*params[0]).name()));return;}
 
 

    zbTimer *thiz=dynamic_cast<zbTimer*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Timer.set:handler");return;}
  
zbObject *res=createVoid(ir->rt);thiz->sethandler(value);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Timer_start_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTimer *thiz=dynamic_cast<zbTimer*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Timer.start");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->restart();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Timer_stop_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTimer *thiz=dynamic_cast<zbTimer*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Timer.stop");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->stop();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Timer_get_running_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTimer *thiz=dynamic_cast<zbTimer*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Timer.get:running");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->isrunning(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Timer_get_interval_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTimer *thiz=dynamic_cast<zbTimer*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Timer.get:interval");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getinterval(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Timer_set_interval_1_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbTimer *thiz=dynamic_cast<zbTimer*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Timer.set:interval");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setinterval(value->tofloat());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_Timer_timer_2_float_(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>65535) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *secs=params[0];
 
 
zbObject *handler=dynamic_cast<zbObject*>(params[1]);
  if (! handler) return false;// {ir->error(fmtstr("ArgError:Timer.timer,expected Object,found %s",typeid(*params[1]).name()));return;}
 
 
zbObject *res=createVoid(ir->rt);newsimpletimer(secs->tofloat(),handler,params+2,parcnt-2);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_OpenDialog_run_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *filter=params[0];
 
 
zbObject *res=runopendialog(filter->tostring(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool OpenDialog_execute_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbOpenDialog *thiz=dynamic_cast<zbOpenDialog*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:OpenDialog.execute");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->execute(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool OpenDialog_get_file_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbOpenDialog *thiz=dynamic_cast<zbOpenDialog*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:OpenDialog.get:file");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getfile(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool OpenDialog_set_file_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbOpenDialog *thiz=dynamic_cast<zbOpenDialog*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:OpenDialog.set:file");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setfile(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool OpenDialog_get_initialDir_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbOpenDialog *thiz=dynamic_cast<zbOpenDialog*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:OpenDialog.get:initialDir");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getinitialdir(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool OpenDialog_set_initialDir_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbOpenDialog *thiz=dynamic_cast<zbOpenDialog*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:OpenDialog.set:initialDir");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setinitialdir(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool OpenDialog_get_files_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbOpenDialog *thiz=dynamic_cast<zbOpenDialog*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:OpenDialog.get:files");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getfiles(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool OpenDialog_get_filter_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbOpenDialog *thiz=dynamic_cast<zbOpenDialog*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:OpenDialog.get:filter");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getfilter(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool OpenDialog_set_filter_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbOpenDialog *thiz=dynamic_cast<zbOpenDialog*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:OpenDialog.set:filter");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setfilter(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool OpenDialog_get_multiSelect_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbOpenDialog *thiz=dynamic_cast<zbOpenDialog*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:OpenDialog.get:multiSelect");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getmultiselect(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool OpenDialog_set_multiSelect_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbOpenDialog *thiz=dynamic_cast<zbOpenDialog*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:OpenDialog.set:multiSelect");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setmultiselect(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool OpenDialog_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newopendialog(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_SaveDialog_run_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *filter=params[0];
 
 
zbObject *res=runsavedialog(filter->tostring(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SaveDialog_execute_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbSaveDialog *thiz=dynamic_cast<zbSaveDialog*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SaveDialog.execute");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->execute(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SaveDialog_get_initialDir_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbSaveDialog *thiz=dynamic_cast<zbSaveDialog*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SaveDialog.get:initialDir");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getinitialdir(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SaveDialog_set_initialDir_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbSaveDialog *thiz=dynamic_cast<zbSaveDialog*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SaveDialog.set:initialDir");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setinitialdir(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SaveDialog_get_file_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbSaveDialog *thiz=dynamic_cast<zbSaveDialog*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SaveDialog.get:file");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getfile(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SaveDialog_set_file_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbSaveDialog *thiz=dynamic_cast<zbSaveDialog*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SaveDialog.set:file");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setfile(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SaveDialog_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newsavedialog(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SaveDialog_get_filter_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbSaveDialog *thiz=dynamic_cast<zbSaveDialog*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SaveDialog.get:filter");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getfilter(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SaveDialog_set_filter_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbSaveDialog *thiz=dynamic_cast<zbSaveDialog*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SaveDialog.set:filter");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setfilter(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool static_ColorDialog_run_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *color=params[0];
 
 
zbObject *res=runcolordialog(color->tostring(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PaneGrp_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newpanegrp(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PaneGrp_get_canvas_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
try {
    zbPaneGrp *thiz=dynamic_cast<zbPaneGrp*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PaneGrp.get:canvas");return;}
  
zbObject *res=thiz->getcanvas()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  } catch(JException &e) {
    ir->nativeerror(e.getmsg());
    return true;
  }
  
  return true;

}
bool PaneGrp_beginDoc_1_Canvas(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbCanvas *destCanvas=dynamic_cast<zbCanvas*>(params[0]);
  if (! destCanvas) return false;// {ir->error(fmtstr("ArgError:PaneGrp.beginDoc,expected Canvas,found %s",typeid(*params[0]).name()));return;}
 
 

    zbPaneGrp *thiz=dynamic_cast<zbPaneGrp*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PaneGrp.beginDoc");return;}
  
zbObject *res=createVoid(ir->rt);thiz->beginDoc(destCanvas);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PaneGrp_endDoc_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPaneGrp *thiz=dynamic_cast<zbPaneGrp*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PaneGrp.endDoc");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->enddoc();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PaneGrp_get_count_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPaneGrp *thiz=dynamic_cast<zbPaneGrp*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PaneGrp.get:count");return;}
  
zbObject *res=createNumber(thiz->getcount(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PaneGrp_op_get_index_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbPaneGrp *thiz=dynamic_cast<zbPaneGrp*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PaneGrp.op:get:index");return;}
  
zbObject *res=thiz->getpane(index->toint())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PaneGrp_sheetCount_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *pageHi=params[0];
 
 

    zbPaneGrp *thiz=dynamic_cast<zbPaneGrp*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PaneGrp.sheetCount");return;}
  
zbObject *res=createNumber(thiz->sheetCount(pageHi->toint()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PaneGrp_replaceText_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *oldtext=params[0];
 
 

   zbObject *newtext=params[1];
 
 

    zbPaneGrp *thiz=dynamic_cast<zbPaneGrp*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PaneGrp.replaceText");return;}
  
zbObject *res=createVoid(ir->rt);thiz->replaceText(oldtext->tostring(),newtext->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Pane_get_isFirst_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPane *thiz=dynamic_cast<zbPane*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Pane.get:isFirst");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->first(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Pane_get_isLast_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPane *thiz=dynamic_cast<zbPane*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Pane.get:isLast");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->last(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Pane_get_hi_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPane *thiz=dynamic_cast<zbPane*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Pane.get:hi");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->gethisum(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Pane_get_parent_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPane *thiz=dynamic_cast<zbPane*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Pane.get:parent");return;}
  
zbObject *res=thiz->getparent()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Pane_get_canvas_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPane *thiz=dynamic_cast<zbPane*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Pane.get:canvas");return;}
  
zbObject *res=thiz->getcanvas()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPage_addPane_1_Pane(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbPane *pane=dynamic_cast<zbPane*>(params[0]);
  if (! pane) return false;// {ir->error(fmtstr("ArgError:LogPage.addPane,expected Pane,found %s",typeid(*params[0]).name()));return;}
 
 

    zbLogPage *thiz=dynamic_cast<zbLogPage*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPage.addPane");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->addpane(pane->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPage_canAddPane_1_Pane(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbPane *pane=dynamic_cast<zbPane*>(params[0]);
  if (! pane) return false;// {ir->error(fmtstr("ArgError:LogPage.canAddPane,expected Pane,found %s",typeid(*params[0]).name()));return;}
 
 

    zbLogPage *thiz=dynamic_cast<zbLogPage*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPage.canAddPane");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->canaddpane(pane->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPage_get_hi_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbLogPage *thiz=dynamic_cast<zbLogPage*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPage.get:hi");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->gethi(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPage_get_isBlocked_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbLogPage *thiz=dynamic_cast<zbLogPage*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPage.get:isBlocked");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->isblocked(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPage_get_isFreeBegin_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbLogPage *thiz=dynamic_cast<zbLogPage*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPage.get:isFreeBegin");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->freebegin(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPage_get_isFreeEnd_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbLogPage *thiz=dynamic_cast<zbLogPage*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPage.get:isFreeEnd");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->freeend(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPage_get_isLeft_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbLogPage *thiz=dynamic_cast<zbLogPage*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPage.get:isLeft");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->isleft(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPage_addPaneGrp_1_PaneGrp(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbPaneGrp *panegrp=dynamic_cast<zbPaneGrp*>(params[0]);
  if (! panegrp) return false;// {ir->error(fmtstr("ArgError:LogPage.addPaneGrp,expected PaneGrp,found %s",typeid(*params[0]).name()));return;}
 
 

    zbLogPage *thiz=dynamic_cast<zbLogPage*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPage.addPaneGrp");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->addpanegrp(panegrp->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPage_getExtraPageCount_1_PaneGrp(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbPaneGrp *panes=dynamic_cast<zbPaneGrp*>(params[0]);
  if (! panes) return false;// {ir->error(fmtstr("ArgError:LogPage.getExtraPageCount,expected PaneGrp,found %s",typeid(*params[0]).name()));return;}
 
 

    zbLogPage *thiz=dynamic_cast<zbLogPage*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPage.getExtraPageCount");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getextrapagecount(panes->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPage_get_count_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbLogPage *thiz=dynamic_cast<zbLogPage*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPage.get:count");return;}
  
zbObject *res=createNumber(thiz->getcount(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPage_op_get_index_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbLogPage *thiz=dynamic_cast<zbLogPage*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPage.op:get:index");return;}
  
zbObject *res=thiz->getpane(index->toint())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPage_erasePane_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

   zbObject *count=params[1];
 
 

    zbLogPage *thiz=dynamic_cast<zbLogPage*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPage.erasePane");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->erasepane(index->toint(),count->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPage_insertPane_2_int_Pane(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 
zbPane *pane=dynamic_cast<zbPane*>(params[1]);
  if (! pane) return false;// {ir->error(fmtstr("ArgError:LogPage.insertPane,expected Pane,found %s",typeid(*params[1]).name()));return;}
 
 

    zbLogPage *thiz=dynamic_cast<zbLogPage*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPage.insertPane");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->insertpane(index->toint(),pane->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPage_draw_1_Canvas(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbCanvas *cnv=dynamic_cast<zbCanvas*>(params[0]);
  if (! cnv) return false;// {ir->error(fmtstr("ArgError:LogPage.draw,expected Canvas,found %s",typeid(*params[0]).name()));return;}
 
 

    zbLogPage *thiz=dynamic_cast<zbLogPage*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPage.draw");return;}
  
zbObject *res=createVoid(ir->rt);thiz->draw(cnv);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MetaCanvas_draw_1_Canvas(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbCanvas *cnv=dynamic_cast<zbCanvas*>(params[0]);
  if (! cnv) return false;// {ir->error(fmtstr("ArgError:MetaCanvas.draw,expected Canvas,found %s",typeid(*params[0]).name()));return;}
 
 

    zbMetaCanvas *thiz=dynamic_cast<zbMetaCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MetaCanvas.draw");return;}
  
zbObject *res=createVoid(ir->rt);thiz->draw(cnv);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MetaCanvas_replaceText_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *oldtext=params[0];
 
 

   zbObject *newtext=params[1];
 
 

    zbMetaCanvas *thiz=dynamic_cast<zbMetaCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MetaCanvas.replaceText");return;}
  
zbObject *res=createVoid(ir->rt);thiz->replaceText(oldtext->tostring(),newtext->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MetaCanvas_get_canvas_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMetaCanvas *thiz=dynamic_cast<zbMetaCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MetaCanvas.get:canvas");return;}
  
zbObject *res=thiz->getcanvas()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MetaCanvas_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newmetacanvas(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ResizeCanvas_op_new_3_Canvas_float_float(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);
zbCanvas *cnv=dynamic_cast<zbCanvas*>(params[0]);
  if (! cnv) return false;// {ir->error(fmtstr("ArgError:ResizeCanvas.op:new,expected Canvas,found %s",typeid(*params[0]).name()));return;}
 
 

   zbObject *kx=params[1];
 
 

   zbObject *ky=params[2];
 
 
zbObject *res=newresizecanvas(cnv,kx->tofloat(),ky->tofloat(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool ResizeCanvas_get_canvas_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbResizeCanvas *thiz=dynamic_cast<zbResizeCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:ResizeCanvas.get:canvas");return;}
  
zbObject *res=thiz->getcanvas()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SubCanvas_op_new_3_Canvas_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);
zbCanvas *cnv=dynamic_cast<zbCanvas*>(params[0]);
  if (! cnv) return false;// {ir->error(fmtstr("ArgError:SubCanvas.op:new,expected Canvas,found %s",typeid(*params[0]).name()));return;}
 
 

   zbObject *dx=params[1];
 
 

   zbObject *dy=params[2];
 
 
zbObject *res=newsubcanvas(cnv,dx->toint(),dy->toint(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool SubCanvas_get_canvas_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbSubCanvas *thiz=dynamic_cast<zbSubCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:SubCanvas.get:canvas");return;}
  
zbObject *res=thiz->getcanvas()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TextCanvas_op_new_2_Canvas_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);
zbCanvas *cnv=dynamic_cast<zbCanvas*>(params[0]);
  if (! cnv) return false;// {ir->error(fmtstr("ArgError:TextCanvas.op:new,expected Canvas,found %s",typeid(*params[0]).name()));return;}
 
 

   zbObject *wi=params[1];
 
 
zbObject *res=newtextcanvas(cnv,wi->toint(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TextCanvas_op_new_3_Canvas_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);
zbCanvas *cnv=dynamic_cast<zbCanvas*>(params[0]);
  if (! cnv) return false;// {ir->error(fmtstr("ArgError:TextCanvas.op:new,expected Canvas,found %s",typeid(*params[0]).name()));return;}
 
 

   zbObject *wi=params[1];
 
 

   zbObject *pagehi=params[2];
 
 
zbObject *res=newtextcanvas(cnv,wi->toint(),pagehi->toint(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TextCanvas_text_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *s=params[0];
 
 

    zbTextCanvas *thiz=dynamic_cast<zbTextCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:TextCanvas.text");return;}
  
zbObject *res=createVoid(ir->rt);thiz->text(s->tostring(),ir);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TextCanvas_font_1_Font(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbFont *font=dynamic_cast<zbFont*>(params[0]);
  if (! font) return false;// {ir->error(fmtstr("ArgError:TextCanvas.font,expected Font,found %s",typeid(*params[0]).name()));return;}
 
 

    zbTextCanvas *thiz=dynamic_cast<zbTextCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:TextCanvas.font");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->font(font->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TextCanvas_wrap_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTextCanvas *thiz=dynamic_cast<zbTextCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:TextCanvas.wrap");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->wrap();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TextCanvas_flush_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbTextCanvas *thiz=dynamic_cast<zbTextCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:TextCanvas.flush");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->flush();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TextCanvas_textElemX_3_int_int_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *space=params[0];
 
 

   zbObject *x=params[1];
 
 

   zbObject *s=params[2];
 
 

    zbTextCanvas *thiz=dynamic_cast<zbTextCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:TextCanvas.textElemX");return;}
  
zbObject *res=createVoid(ir->rt);thiz->textElemX(space->toint(),x->toint(),s->tostring(),ir);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool TextCanvas_textElem_2_int_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *space=params[0];
 
 

   zbObject *s=params[1];
 
 

    zbTextCanvas *thiz=dynamic_cast<zbTextCanvas*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:TextCanvas.textElem");return;}
  
zbObject *res=createVoid(ir->rt);thiz->textElem(space->toint(),s->tostring(),ir);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PageDistribution_get_sheetCount_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPageDistribution *thiz=dynamic_cast<zbPageDistribution*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PageDistribution.get:sheetCount");return;}
  
zbObject *res=createNumber(thiz->getsheetCount(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PageDistribution_get_freePageCount_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPageDistribution *thiz=dynamic_cast<zbPageDistribution*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PageDistribution.get:freePageCount");return;}
  
zbObject *res=createNumber(thiz->getfreePageCount(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PageDistribution_draw_2_int_Canvas(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *page=params[0];
 
 
zbCanvas *cnv=dynamic_cast<zbCanvas*>(params[1]);
  if (! cnv) return false;// {ir->error(fmtstr("ArgError:PageDistribution.draw,expected Canvas,found %s",typeid(*params[1]).name()));return;}
 
 

    zbPageDistribution *thiz=dynamic_cast<zbPageDistribution*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PageDistribution.draw");return;}
  
zbObject *res=createVoid(ir->rt);thiz->draw(page->toint(),cnv);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPages_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newlogpages(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPages_add_1_PaneGrp(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbPaneGrp *grp=dynamic_cast<zbPaneGrp*>(params[0]);
  if (! grp) return false;// {ir->error(fmtstr("ArgError:LogPages.add,expected PaneGrp,found %s",typeid(*params[0]).name()));return;}
 
 

    zbLogPages *thiz=dynamic_cast<zbLogPages*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPages.add");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->addpanegrp(grp->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPages_print_1_Printer(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbPrinter *prn=dynamic_cast<zbPrinter*>(params[0]);
  if (! prn) return false;// {ir->error(fmtstr("ArgError:LogPages.print,expected Printer,found %s",typeid(*params[0]).name()));return;}
 
 

    zbLogPages *thiz=dynamic_cast<zbLogPages*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPages.print");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->print(prn->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPages_loadPropsFrom_1_Printer(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbPrinter *prn=dynamic_cast<zbPrinter*>(params[0]);
  if (! prn) return false;// {ir->error(fmtstr("ArgError:LogPages.loadPropsFrom,expected Printer,found %s",typeid(*params[0]).name()));return;}
 
 

    zbLogPages *thiz=dynamic_cast<zbLogPages*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPages.loadPropsFrom");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setpropsfrom(prn->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPages_setPageSize_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *w=params[0];
 
 

   zbObject *h=params[1];
 
 

    zbLogPages *thiz=dynamic_cast<zbLogPages*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPages.setPageSize");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setsize(w->toint(),h->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPages_complete_2_Array_Array(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);
zbArray *ofsx=dynamic_cast<zbArray*>(params[0]);
  if (! ofsx) return false;// {ir->error(fmtstr("ArgError:LogPages.complete,expected Array,found %s",typeid(*params[0]).name()));return;}
 
 
zbArray *ofsy=dynamic_cast<zbArray*>(params[1]);
  if (! ofsy) return false;// {ir->error(fmtstr("ArgError:LogPages.complete,expected Array,found %s",typeid(*params[1]).name()));return;}
 
 

    zbLogPages *thiz=dynamic_cast<zbLogPages*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPages.complete");return;}
  
zbObject *res=thiz->complete(ofsx,ofsy)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPages_completeLines_2_Array_Array(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);
zbArray *ofsx=dynamic_cast<zbArray*>(params[0]);
  if (! ofsx) return false;// {ir->error(fmtstr("ArgError:LogPages.completeLines,expected Array,found %s",typeid(*params[0]).name()));return;}
 
 
zbArray *ofsy=dynamic_cast<zbArray*>(params[1]);
  if (! ofsy) return false;// {ir->error(fmtstr("ArgError:LogPages.completeLines,expected Array,found %s",typeid(*params[1]).name()));return;}
 
 

    zbLogPages *thiz=dynamic_cast<zbLogPages*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPages.completeLines");return;}
  
zbObject *res=thiz->completeLines(ofsx,ofsy)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPages_get_pageHi_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbLogPages *thiz=dynamic_cast<zbLogPages*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPages.get:pageHi");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getpghi(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPages_get_pageWi_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbLogPages *thiz=dynamic_cast<zbLogPages*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPages.get:pageWi");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->getpgwi(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPages_op_get_index_1_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *index=params[0];
 
 

    zbLogPages *thiz=dynamic_cast<zbLogPages*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPages.op:get:index");return;}
  
zbObject *res=thiz->getpage(index->toint())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPages_addPage_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbLogPages *thiz=dynamic_cast<zbLogPages*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPages.addPage");return;}
  
zbObject *res=thiz->addPage()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPages_get_count_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbLogPages *thiz=dynamic_cast<zbLogPages*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPages.get:count");return;}
  
zbObject *res=createNumber(thiz->getcount(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPages_get_last_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbLogPages *thiz=dynamic_cast<zbLogPages*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPages.get:last");return;}
  
zbObject *res=thiz->getlast()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool LogPages_searchPane_1_Pane(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbPane *pane=dynamic_cast<zbPane*>(params[0]);
  if (! pane) return false;// {ir->error(fmtstr("ArgError:LogPages.searchPane,expected Pane,found %s",typeid(*params[0]).name()));return;}
 
 

    zbLogPages *thiz=dynamic_cast<zbLogPages*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:LogPages.searchPane");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createNumber(thiz->getlink()->searchpane(pane->cvalue()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MenuItem_get_text_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMenuItem *thiz=dynamic_cast<zbMenuItem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MenuItem.get:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->gettext(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MenuItem_set_text_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbMenuItem *thiz=dynamic_cast<zbMenuItem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MenuItem.set:text");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->settext(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MenuItem_get_checked_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMenuItem *thiz=dynamic_cast<zbMenuItem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MenuItem.get:checked");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->getchecked(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MenuItem_set_checked_1_bool(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbMenuItem *thiz=dynamic_cast<zbMenuItem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MenuItem.set:checked");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setchecked(value->tobool());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MenuItem_get_shortcut_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMenuItem *thiz=dynamic_cast<zbMenuItem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MenuItem.get:shortcut");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getshortcut(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MenuItem_set_shortcut_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbMenuItem *thiz=dynamic_cast<zbMenuItem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MenuItem.set:shortcut");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setshortcut(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MenuItem_load_1_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[0]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:MenuItem.load,expected XMLNode,found %s",typeid(*params[0]).name()));return;}
 
 

    zbMenuItem *thiz=dynamic_cast<zbMenuItem*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MenuItem.load");return;}
  
zbObject *res=createVoid(ir->rt);thiz->load(xml);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MenuItem_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newmenuitem(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool WaitWindow_show_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbWaitWindow *thiz=dynamic_cast<zbWaitWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:WaitWindow.show");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->show();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool WaitWindow_hide_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbWaitWindow *thiz=dynamic_cast<zbWaitWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:WaitWindow.hide");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->hide();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool WaitWindow_get_msg_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbWaitWindow *thiz=dynamic_cast<zbWaitWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:WaitWindow.get:msg");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getmsg(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool WaitWindow_set_msg_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *value=params[0];
 
 

    zbWaitWindow *thiz=dynamic_cast<zbWaitWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:WaitWindow.set:msg");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->setmsg(value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool WaitWindow_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newwaitwindow(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool WaitWindow_destroy_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbWaitWindow *thiz=dynamic_cast<zbWaitWindow*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:WaitWindow.destroy");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->destroy();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MidiOut_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newmidiout(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MidiOut_open_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMidiOut *thiz=dynamic_cast<zbMidiOut*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MidiOut.open");return;}
  
zbObject *res=createVoid(ir->rt);thiz->open(ir);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MidiOut_message_3_int_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *status=params[0];
 
 

   zbObject *param1=params[1];
 
 

   zbObject *param2=params[2];
 
 

    zbMidiOut *thiz=dynamic_cast<zbMidiOut*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MidiOut.message");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->message(status->toint(),param1->toint(),param2->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MidiOut_close_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbMidiOut *thiz=dynamic_cast<zbMidiOut*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MidiOut.close");return;}
  
zbObject *res=createVoid(ir->rt);thiz->close();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MidiOut_noteOn_3_int_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *channel=params[0];
 
 

   zbObject *midihi=params[1];
 
 

   zbObject *velocity=params[2];
 
 

    zbMidiOut *thiz=dynamic_cast<zbMidiOut*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MidiOut.noteOn");return;}
  
zbObject *res=createVoid(ir->rt);thiz->noteOn(channel->toint(),midihi->toint(),velocity->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MidiOut_noteOff_3_int_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<3 || parcnt>3) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *channel=params[0];
 
 

   zbObject *midihi=params[1];
 
 

   zbObject *velocity=params[2];
 
 

    zbMidiOut *thiz=dynamic_cast<zbMidiOut*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MidiOut.noteOff");return;}
  
zbObject *res=createVoid(ir->rt);thiz->noteOff(channel->toint(),midihi->toint(),velocity->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool MidiOut_setInstrument_2_int_int(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *channel=params[0];
 
 

   zbObject *instrumentId=params[1];
 
 

    zbMidiOut *thiz=dynamic_cast<zbMidiOut*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:MidiOut.setInstrument");return;}
  
zbObject *res=createVoid(ir->rt);thiz->setInstrument(channel->toint(),instrumentId->toint());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}





struct zboverride_TableModel:zbTableModel {
  zboverride_TableModel(Runtime *rt,STRING intf):zbTableModel(rt,intf) {}
override int getRowCount() {
  Interpreter ir;ir.rt=rt;
  
  ir.push(this);
  if (trycallmethod(&ir,"getRowCount",0)) {
    ir.interpret();
    if (zbObject *obj=ir.gettop()) {
      return obj->toint();
      
    }
    throw zbError("Function should return value");
  }
  throw zbError("XError:method not found(getRowCount)");
}
override string getCellText(string fldid,int row) {
  Interpreter ir;ir.rt=rt;
  
  ir._push(createString(fldid,rt));
  
  ir._push(createNumber(row,rt));
  
  ir.push(this);
  if (trycallmethod(&ir,"getCellText",2)) {
    ir.interpret();
    if (zbObject *obj=ir.gettop()) {
      return obj->tostring();
      
    }
    throw zbError("Function should return value");
  }
  throw zbError("XError:method not found(getCellText)");
}
override void getCols(zbTableCols * cols) {
  Interpreter ir;ir.rt=rt;
  
  ir._push((cols->addref(),cols));
  
  ir.push(this);
  if (trycallmethod(&ir,"getCols",1)) {
    ir.interpret();
    if (zbObject *obj=ir.gettop()) {
      return;
      
    }
    throw zbError("Function should return value");
  }
  throw zbError("XError:method not found(getCols)");
}
};
struct TableModel_creator:InstanceCreator {
  zbObject *createInstance(Runtime *rt,STRING intf) {return new zboverride_TableModel(rt,intf);}
  TableModel_creator() {}
};


struct zboverride_TreeSource:zbTreeSource {
  zboverride_TreeSource(Runtime *rt,STRING intf):zbTreeSource(rt,intf) {}
override int getCount() {
  Interpreter ir;ir.rt=rt;
  
  ir.push(this);
  if (trycallmethod(&ir,"getCount",0)) {
    ir.interpret();
    if (zbObject *obj=ir.gettop()) {
      return obj->toint();
      
    }
    throw zbError("Function should return value");
  }
  throw zbError("XError:method not found(getCount)");
}
override string getText() {
  Interpreter ir;ir.rt=rt;
  
  ir.push(this);
  if (trycallmethod(&ir,"getText",0)) {
    ir.interpret();
    if (zbObject *obj=ir.gettop()) {
      return obj->tostring();
      
    }
    throw zbError("Function should return value");
  }
  throw zbError("XError:method not found(getText)");
}
override RefBase<zbTreeSource> getNodeAt(int index) {
  Interpreter ir;ir.rt=rt;
  
  ir._push(createNumber(index,rt));
  
  ir.push(this);
  if (trycallmethod(&ir,"getNodeAt",1)) {
    ir.interpret();
    if (zbObject *obj=ir.gettop()) {
      return dynamic_cast<zbTreeSource*>(obj);
      
    }
    throw zbError("Function should return value");
  }
  throw zbError("XError:method not found(getNodeAt)");
}
override bool equalTo(zbTreeSource * right) {
  Interpreter ir;ir.rt=rt;
  
  ir._push((right->addref(),right));
  
  ir.push(this);
  if (trycallmethod(&ir,"equalTo",1)) {
    ir.interpret();
    if (zbObject *obj=ir.gettop()) {
      return obj->tobool();
      
    }
    throw zbError("Function should return value");
  }
  throw zbError("XError:method not found(equalTo)");
}
};
struct TreeSource_creator:InstanceCreator {
  zbObject *createInstance(Runtime *rt,STRING intf) {return new zboverride_TreeSource(rt,intf);}
  TreeSource_creator() {}
};



void init_winobjs_rt(Runtime &rt) {
rt.addsub("Control_get_X_0",Control_get_X_0);
rt.addsub("Control_set_X_1_int",Control_set_X_1_int);
rt.addsub("Control_get_Y_0",Control_get_Y_0);
rt.addsub("Control_set_Y_1_int",Control_set_Y_1_int);
rt.addsub("Control_get_W_0",Control_get_W_0);
rt.addsub("Control_set_W_1_int",Control_set_W_1_int);
rt.addsub("Control_get_H_0",Control_get_H_0);
rt.addsub("Control_set_H_1_int",Control_set_H_1_int);
rt.addsub("Control_destroy_0",Control_destroy_0);
rt.addsub("Control_show_0",Control_show_0);
rt.addsub("Control_hide_0",Control_hide_0);
rt.addsub("Control_get_visible_0",Control_get_visible_0);
rt.addsub("Control_set_visible_1_bool",Control_set_visible_1_bool);
rt.addsub("Control_get_enabled_0",Control_get_enabled_0);
rt.addsub("Control_set_enabled_1_bool",Control_set_enabled_1_bool);
rt.addsub("Control_get_parent_0",Control_get_parent_0);
rt.addsub("Control_set_parent_1_Control",Control_set_parent_1_Control);
rt.addsub("Control_get_font_0",Control_get_font_0);
rt.addsub("Control_set_font_1_Font",Control_set_font_1_Font);
rt.addsub("Control_loadProps_2_XMLNode_Form",Control_loadProps_2_XMLNode_Form);
rt.addsub("Control_saveData_1_XMLNode",Control_saveData_1_XMLNode);
rt.addsub("Control_loadData_1_XMLNode",Control_loadData_1_XMLNode);
rt.addsub("Control_clearChilds_0",Control_clearChilds_0);
rt.addsub("Control_get_tag_0",Control_get_tag_0);
rt.addsub("Control_set_tag_1_string",Control_set_tag_1_string);
rt.addsub("static_Window_alert_0",static_Window_alert_0);
rt.addsub("Window_get_text_0",Window_get_text_0);
rt.addsub("Window_set_text_1_string",Window_set_text_1_string);
rt.addsub("Window_op_new_0",Window_op_new_0);
rt.addsub("Window_showModal_0",Window_showModal_0);
rt.addsub("Window_show_0",Window_show_0);
rt.addsub("Window_denyClose_0",Window_denyClose_0);
rt.addsub("Window_close_0",Window_close_0);
rt.addsub("Window_get_style_0",Window_get_style_0);
rt.addsub("Window_set_style_1_string",Window_set_style_1_string);
rt.addsub("Window_get_X_0",Window_get_X_0);
rt.addsub("Window_set_X_1_int",Window_set_X_1_int);
rt.addsub("Window_get_Y_0",Window_get_Y_0);
rt.addsub("Window_set_Y_1_int",Window_set_Y_1_int);
rt.addsub("Window_get_W_0",Window_get_W_0);
rt.addsub("Window_set_W_1_int",Window_set_W_1_int);
rt.addsub("Window_get_H_0",Window_get_H_0);
rt.addsub("Window_set_H_1_int",Window_set_H_1_int);
rt.addsub("static_Window_msgbox_2_string_string",static_Window_msgbox_2_string_string);
rt.addsub("static_Window_msgwin_3_string_string_string",static_Window_msgwin_3_string_string_string);
rt.addsub("static_Window_inputBox_3_string_string_string",static_Window_inputBox_3_string_string_string);
rt.addsub("static_Window_get_ppmmx_0",static_Window_get_ppmmx_0);
rt.addsub("static_Window_get_ppmmy_0",static_Window_get_ppmmy_0);
rt.addsub("Window_loadData_1_XMLNode",Window_loadData_1_XMLNode);
rt.addsub("Window_saveData_1_XMLNode",Window_saveData_1_XMLNode);
rt.addsub("Form_loadWindow_1_string",Form_loadWindow_1_string);
rt.addsub("Form_op_new_0",Form_op_new_0);
rt.addsub("Form_op_new_1_Component",Form_op_new_1_Component);
rt.addsub("Form_get_handler_0",Form_get_handler_0);
rt.addsub("Form_set_handler_1_Object",Form_set_handler_1_Object);
rt.addsub("Form_search_1_string",Form_search_1_string);
rt.addsub("Form_op_get_prop_1_string",Form_op_get_prop_1_string);
rt.addsub("Form_destroy_0",Form_destroy_0);
rt.addsub("Button_get_text_0",Button_get_text_0);
rt.addsub("Button_set_text_1_string",Button_set_text_1_string);
rt.addsub("Button_get_onclick_0",Button_get_onclick_0);
rt.addsub("Button_set_onclick_1_string",Button_set_onclick_1_string);
rt.addsub("RadioGroup_get_text_0",RadioGroup_get_text_0);
rt.addsub("RadioGroup_set_text_1_string",RadioGroup_set_text_1_string);
rt.addsub("RadioGroup_get_items_0",RadioGroup_get_items_0);
rt.addsub("RadioGroup_set_items_1_string",RadioGroup_set_items_1_string);
rt.addsub("RadioGroup_get_cur_0",RadioGroup_get_cur_0);
rt.addsub("RadioGroup_set_cur_1_int",RadioGroup_set_cur_1_int);
rt.addsub("CheckBox_get_text_0",CheckBox_get_text_0);
rt.addsub("CheckBox_set_text_1_string",CheckBox_set_text_1_string);
rt.addsub("CheckBox_get_checked_0",CheckBox_get_checked_0);
rt.addsub("CheckBox_set_checked_1_bool",CheckBox_set_checked_1_bool);
rt.addsub("SpeedButton_get_text_0",SpeedButton_get_text_0);
rt.addsub("SpeedButton_set_text_1_string",SpeedButton_set_text_1_string);
rt.addsub("SpeedButton_get_onclick_0",SpeedButton_get_onclick_0);
rt.addsub("SpeedButton_set_onclick_1_string",SpeedButton_set_onclick_1_string);
rt.addsub("SpeedButton_get_image_0",SpeedButton_get_image_0);
rt.addsub("SpeedButton_set_image_1_Picture",SpeedButton_set_image_1_Picture);
rt.addsub("SpeedButton_get_switched_0",SpeedButton_get_switched_0);
rt.addsub("SpeedButton_set_switched_1_bool",SpeedButton_set_switched_1_bool);
rt.addsub("SpeedButton_op_new_1_Form",SpeedButton_op_new_1_Form);
rt.addsub("SpeedButton_get_color_0",SpeedButton_get_color_0);
rt.addsub("SpeedButton_set_color_1_string",SpeedButton_set_color_1_string);
rt.addsub("SpeedGroup_get_cur_0",SpeedGroup_get_cur_0);
rt.addsub("SpeedGroup_set_cur_1_int",SpeedGroup_set_cur_1_int);
rt.addsub("ColorTable_get_rows_0",ColorTable_get_rows_0);
rt.addsub("ColorTable_set_rows_1_int",ColorTable_set_rows_1_int);
rt.addsub("ColorTable_get_cols_0",ColorTable_get_cols_0);
rt.addsub("ColorTable_set_cols_1_int",ColorTable_set_cols_1_int);
rt.addsub("ColorTable_op_get_index_2_int_int",ColorTable_op_get_index_2_int_int);
rt.addsub("ColorTable_op_set_index_3_int_int_string",ColorTable_op_set_index_3_int_int_string);
rt.addsub("ListBox_get_items_0",ListBox_get_items_0);
rt.addsub("ListBox_set_items_1_string",ListBox_set_items_1_string);
rt.addsub("ListBox_get_cur_0",ListBox_get_cur_0);
rt.addsub("ListBox_set_cur_1_int",ListBox_set_cur_1_int);
rt.addsub("ListBox_get_multiSelect_0",ListBox_get_multiSelect_0);
rt.addsub("ListBox_set_multiSelect_1_bool",ListBox_set_multiSelect_1_bool);
rt.addsub("ListBox_get_selected_0",ListBox_get_selected_0);
rt.addsub("ListBox_set_selected_1_string",ListBox_set_selected_1_string);
rt.addsub("PackedList_get_items_0",PackedList_get_items_0);
rt.addsub("PackedList_set_items_1_string",PackedList_set_items_1_string);
rt.addsub("PackedList_get_cur_0",PackedList_get_cur_0);
rt.addsub("PackedList_set_cur_1_int",PackedList_set_cur_1_int);
rt.addsub("Edit_get_text_0",Edit_get_text_0);
rt.addsub("Edit_set_text_1_string",Edit_set_text_1_string);
rt.addsub("Edit_get_modified_0",Edit_get_modified_0);
rt.addsub("Edit_set_modified_1_bool",Edit_set_modified_1_bool);
rt.addsub("Memo_get_text_0",Memo_get_text_0);
rt.addsub("Memo_set_text_1_string",Memo_set_text_1_string);
rt.addsub("Memo_get_modified_0",Memo_get_modified_0);
rt.addsub("Memo_set_modified_1_bool",Memo_set_modified_1_bool);
rt.addsub("Label_get_text_0",Label_get_text_0);
rt.addsub("Label_set_text_1_string",Label_set_text_1_string);
rt.addsub("WrapLabel_get_text_0",WrapLabel_get_text_0);
rt.addsub("WrapLabel_set_text_1_string",WrapLabel_set_text_1_string);
rt.addsub("TrackBar_get_min_0",TrackBar_get_min_0);
rt.addsub("TrackBar_set_min_1_int",TrackBar_set_min_1_int);
rt.addsub("TrackBar_get_max_0",TrackBar_get_max_0);
rt.addsub("TrackBar_set_max_1_int",TrackBar_set_max_1_int);
rt.addsub("TrackBar_get_cur_0",TrackBar_get_cur_0);
rt.addsub("TrackBar_set_cur_1_int",TrackBar_set_cur_1_int);
rt.addsub("TrackBar_get_frequency_0",TrackBar_get_frequency_0);
rt.addsub("TrackBar_set_frequency_1_int",TrackBar_set_frequency_1_int);
rt.addsub("Table_set_model_1_TableModel",Table_set_model_1_TableModel);
rt.addsub("Table_get_selrow_0",Table_get_selrow_0);
rt.addsub("Table_set_selrow_1_int",Table_set_selrow_1_int);
rt.addsub("Table_get_selcol_0",Table_get_selcol_0);
rt.addsub("Table_set_selcol_1_string",Table_set_selcol_1_string);
rt.addsub("Table_repaint_0",Table_repaint_0);
rt.addsub("Table_repaintLine_1_int",Table_repaintLine_1_int);
rt.addsub("Table_update_0",Table_update_0);
rt.addsub("CheckList_get_cur_0",CheckList_get_cur_0);
rt.addsub("CheckList_set_cur_1_int",CheckList_set_cur_1_int);
rt.addsub("CheckList_get_items_0",CheckList_get_items_0);
rt.addsub("CheckList_set_items_1_string",CheckList_set_items_1_string);
rt.addsub("CheckList_get_checked_0",CheckList_get_checked_0);
rt.addsub("CheckList_set_checked_1_string",CheckList_set_checked_1_string);
rt.addsub("TableCols_add_2_string_string",TableCols_add_2_string_string);
rt.addsub("TableCols_add_3_string_string_string",TableCols_add_3_string_string_string);
rt.addcreator("TableModel.creator",new TableModel_creator());
rt.addcreator("TreeSource.creator",new TreeSource_creator());
rt.addsub("Tree_set_source_1_TreeSource",Tree_set_source_1_TreeSource);
rt.addsub("Tree_update_0",Tree_update_0);
rt.addsub("Tree_get_active_0",Tree_get_active_0);
rt.addsub("Tree_set_active_1_MultiIndex",Tree_set_active_1_MultiIndex);
rt.addsub("Tree_expand_1_MultiIndex",Tree_expand_1_MultiIndex);
rt.addsub("Tree_collapse_1_MultiIndex",Tree_collapse_1_MultiIndex);
rt.addsub("PaintBox_get_onpaint_0",PaintBox_get_onpaint_0);
rt.addsub("PaintBox_set_onpaint_1_String",PaintBox_set_onpaint_1_String);
rt.addsub("PaintBox_repaint_0",PaintBox_repaint_0);
rt.addsub("ScrolledPainter_get_onpaint_0",ScrolledPainter_get_onpaint_0);
rt.addsub("ScrolledPainter_set_onpaint_1_String",ScrolledPainter_set_onpaint_1_String);
rt.addsub("ScrolledPainter_setInSize_2_int_int",ScrolledPainter_setInSize_2_int_int);
rt.addsub("ScrolledPainter_repaint_0",ScrolledPainter_repaint_0);
rt.addsub("PropertyList_save_1_VarList",PropertyList_save_1_VarList);
rt.addsub("PropertyList_load_1_VarList",PropertyList_load_1_VarList);
rt.addsub("PropertyList_addVar_1_string",PropertyList_addVar_1_string);
rt.addsub("PropertyList_eraseVar_0",PropertyList_eraseVar_0);
rt.addsub("PropertyList_get_modified_0",PropertyList_get_modified_0);
rt.addsub("PropertyList_set_modified_1_bool",PropertyList_set_modified_1_bool);
rt.addsub("Pager_get_cur_0",Pager_get_cur_0);
rt.addsub("Pager_set_cur_1_int",Pager_set_cur_1_int);
rt.addsub("Settings_createAll_0",Settings_createAll_0);
rt.addsub("HTMLView_get_text_0",HTMLView_get_text_0);
rt.addsub("HTMLView_set_text_1_string",HTMLView_set_text_1_string);
rt.addsub("HTMLView_searchedPicture_2_string_Picture",HTMLView_searchedPicture_2_string_Picture);
rt.addsub("Canvas_line_4_int_int_int_int",Canvas_line_4_int_int_int_int);
rt.addsub("Canvas_rectangle_4_int_int_int_int",Canvas_rectangle_4_int_int_int_int);
rt.addsub("Canvas_ellipse_4_int_int_int_int",Canvas_ellipse_4_int_int_int_int);
rt.addsub("Canvas_fg_1_string",Canvas_fg_1_string);
rt.addsub("Canvas_bg_1_string",Canvas_bg_1_string);
rt.addsub("Canvas_text_3_int_int_string",Canvas_text_3_int_int_string);
rt.addsub("Canvas_textwi_1_string",Canvas_textwi_1_string);
rt.addsub("Canvas_texthi_1_string",Canvas_texthi_1_string);
rt.addsub("Canvas_font_1_Font",Canvas_font_1_Font);
rt.addsub("Canvas_penwi_1_int",Canvas_penwi_1_int);
rt.addsub("Canvas_drawFrame_5_int_int_int_int_string",Canvas_drawFrame_5_int_int_int_int_string);
rt.addsub("Canvas_centerText_6_int_int_int_int_string_Store",Canvas_centerText_6_int_int_int_int_string_Store);
rt.addsub("Canvas_centerText_5_int_int_int_int_string",Canvas_centerText_5_int_int_int_int_string);
rt.addsub("Canvas_nextBand_1_int",Canvas_nextBand_1_int);
rt.addsub("Canvas_nextBand_2_int_string",Canvas_nextBand_2_int_string);
rt.addsub("Canvas_drawRadio_3_int_int_bool",Canvas_drawRadio_3_int_int_bool);
rt.addsub("Canvas_drawCheck_3_int_int_bool",Canvas_drawCheck_3_int_int_bool);
rt.addsub("Canvas_op_get_index_2_int_int",Canvas_op_get_index_2_int_int);
rt.addsub("Canvas_op_set_index_3_int_int_string",Canvas_op_set_index_3_int_int_string);
rt.addsub("Canvas_drawSelection_4_int_int_int_int",Canvas_drawSelection_4_int_int_int_int);
rt.addsub("Canvas_randomLine_4_int_int_int_int",Canvas_randomLine_4_int_int_int_int);
rt.addsub("Canvas_floodFill_4_int_int_string_string",Canvas_floodFill_4_int_int_string_string);
rt.addsub("Font_get_name_0",Font_get_name_0);
rt.addsub("Font_set_name_1_string",Font_set_name_1_string);
rt.addsub("Font_get_size_0",Font_get_size_0);
rt.addsub("Font_set_size_1_int",Font_set_size_1_int);
rt.addsub("Font_get_bold_0",Font_get_bold_0);
rt.addsub("Font_set_bold_1_bool",Font_set_bold_1_bool);
rt.addsub("Font_get_underline_0",Font_get_underline_0);
rt.addsub("Font_set_underline_1_bool",Font_set_underline_1_bool);
rt.addsub("Font_get_strikeout_0",Font_get_strikeout_0);
rt.addsub("Font_set_strikeout_1_bool",Font_set_strikeout_1_bool);
rt.addsub("Font_get_italic_0",Font_get_italic_0);
rt.addsub("Font_set_italic_1_bool",Font_set_italic_1_bool);
rt.addsub("Font_edit_0",Font_edit_0);
rt.addsub("Font_op_new_0",Font_op_new_0);
rt.addsub("Font_load_1_XMLNode",Font_load_1_XMLNode);
rt.addsub("Font_save_1_XMLNode",Font_save_1_XMLNode);
rt.addsub("Font_getHeight_0",Font_getHeight_0);
rt.addsub("Font_getWidth_1_string",Font_getWidth_1_string);
rt.addsub("Picture_load_1_string",Picture_load_1_string);
rt.addsub("Picture_save_1_string",Picture_save_1_string);
rt.addsub("Picture_get_canvas_0",Picture_get_canvas_0);
rt.addsub("Picture_draw_3_int_int_Canvas",Picture_draw_3_int_int_Canvas);
rt.addsub("Picture_stretch_5_int_int_int_int_Canvas",Picture_stretch_5_int_int_int_int_Canvas);
rt.addsub("Picture_stretchRectangle_9_int_int_int_int_int_int_int_int_Canvas",Picture_stretchRectangle_9_int_int_int_int_int_int_int_int_Canvas);
rt.addsub("Picture_hflip_0",Picture_hflip_0);
rt.addsub("Picture_vflip_0",Picture_vflip_0);
rt.addsub("Picture_transp_0",Picture_transp_0);
rt.addsub("Picture_get_W_0",Picture_get_W_0);
rt.addsub("Picture_set_W_1_int",Picture_set_W_1_int);
rt.addsub("Picture_get_H_0",Picture_get_H_0);
rt.addsub("Picture_set_H_1_int",Picture_set_H_1_int);
rt.addsub("Picture_invert_0",Picture_invert_0);
rt.addsub("Picture_clone_0",Picture_clone_0);
rt.addsub("Picture_op_new_0",Picture_op_new_0);
rt.addsub("ApplicationClass_run_0",ApplicationClass_run_0);
rt.addsub("ApplicationClass_exit_0",ApplicationClass_exit_0);
rt.addsub("ApplicationClass_loadResource_1_string",ApplicationClass_loadResource_1_string);
rt.addsub("ApplicationClass_get_options_0",ApplicationClass_get_options_0);
rt.addsub("ApplicationClass_addSearchPath_1_string",ApplicationClass_addSearchPath_1_string);
rt.addsub("ApplicationClass_setOptionFile_1_string",ApplicationClass_setOptionFile_1_string);
rt.addsub("ApplicationClass_runXMLDialog_2_XMLNode_XMLNode",ApplicationClass_runXMLDialog_2_XMLNode_XMLNode);
rt.addsub("ApplicationClass_findResource_1_string",ApplicationClass_findResource_1_string);
rt.addsub("ApplicationClass_runSettingsDialog_2_string_XMLNode",ApplicationClass_runSettingsDialog_2_string_XMLNode);
rt.addsub("ApplicationClass_get_title_0",ApplicationClass_get_title_0);
rt.addsub("ApplicationClass_set_title_1_string",ApplicationClass_set_title_1_string);
rt.addsub("ApplicationClass_get_systemMenu_0",ApplicationClass_get_systemMenu_0);
rt.addsub("ApplicationClass_set_systemMenu_1_Object",ApplicationClass_set_systemMenu_1_Object);
rt.addsub("ApplicationClass_get_handler_0",ApplicationClass_get_handler_0);
rt.addsub("ApplicationClass_set_handler_1_Object",ApplicationClass_set_handler_1_Object);
rt.addsub("ApplicationClass_get_onIdle_0",ApplicationClass_get_onIdle_0);
rt.addsub("ApplicationClass_set_onIdle_1_string",ApplicationClass_set_onIdle_1_string);
rt.addsub("ApplicationClass_get_onDesktopMouseDown_0",ApplicationClass_get_onDesktopMouseDown_0);
rt.addsub("ApplicationClass_set_onDesktopMouseDown_1_string",ApplicationClass_set_onDesktopMouseDown_1_string);
rt.addsub("ApplicationClass_get_onDesktopMouseUp_0",ApplicationClass_get_onDesktopMouseUp_0);
rt.addsub("ApplicationClass_set_onDesktopMouseUp_1_string",ApplicationClass_set_onDesktopMouseUp_1_string);
rt.addsub("ApplicationClass_get_onDesktopMouseMove_0",ApplicationClass_get_onDesktopMouseMove_0);
rt.addsub("ApplicationClass_set_onDesktopMouseMove_1_string",ApplicationClass_set_onDesktopMouseMove_1_string);
rt.addsub("ApplicationClass_get_desktopW_0",ApplicationClass_get_desktopW_0);
rt.addsub("ApplicationClass_get_desktopH_0",ApplicationClass_get_desktopH_0);
rt.addsub("ApplicationClass_get_shiftState_0",ApplicationClass_get_shiftState_0);
rt.addsub("Printer_op_new_0",Printer_op_new_0);
rt.addsub("Printer_getInfo_0",Printer_getInfo_0);
rt.addsub("Printer_loadProperties_1_XMLNode",Printer_loadProperties_1_XMLNode);
rt.addsub("Printer_saveProperties_1_XMLNode",Printer_saveProperties_1_XMLNode);
rt.addsub("Printer_get_pghi_0",Printer_get_pghi_0);
rt.addsub("Printer_get_pgwi_0",Printer_get_pgwi_0);
rt.addsub("Printer_beginPage_0",Printer_beginPage_0);
rt.addsub("Printer_endPage_0",Printer_endPage_0);
rt.addsub("Printer_makeFont_1_Font",Printer_makeFont_1_Font);
rt.addsub("Printer_setup_0",Printer_setup_0);
rt.addsub("Printer_detailSetup_0",Printer_detailSetup_0);
rt.addsub("Printer_beginDoc_1_string",Printer_beginDoc_1_string);
rt.addsub("Printer_beginFileDoc_2_string_string",Printer_beginFileDoc_2_string_string);
rt.addsub("Printer_endDoc_0",Printer_endDoc_0);
rt.addsub("Printer_destroy_0",Printer_destroy_0);
rt.addsub("Printer_get_ppmmx_0",Printer_get_ppmmx_0);
rt.addsub("Printer_get_ppmmy_0",Printer_get_ppmmy_0);
rt.addsub("Console_print_0",Console_print_0);
rt.addsub("Console_println_0",Console_println_0);
rt.addsub("Console_readln_0",Console_readln_0);
rt.addsub("Console_close_0",Console_close_0);
rt.addsub("Console_op_new_0",Console_op_new_0);
rt.addsub("Console_wait_0",Console_wait_0);
rt.addsub("static_Console_get_std_0",static_Console_get_std_0);
rt.addsub("Console_destroy_0",Console_destroy_0);
rt.addsub("Key_op_eq_1_string",Key_op_eq_1_string);
rt.addsub("Key_op_neq_1_string",Key_op_neq_1_string);
rt.addsub("Key_op_new_1_string",Key_op_new_1_string);
rt.addsub("Key_get_shift_0",Key_get_shift_0);
rt.addsub("Key_get_alt_0",Key_get_alt_0);
rt.addsub("Key_get_ctrl_0",Key_get_ctrl_0);
rt.addsub("Key_get_lshift_0",Key_get_lshift_0);
rt.addsub("Key_get_rshift_0",Key_get_rshift_0);
rt.addsub("Key_get_key_0",Key_get_key_0);
rt.addsub("ShiftState_get_shift_0",ShiftState_get_shift_0);
rt.addsub("ShiftState_get_alt_0",ShiftState_get_alt_0);
rt.addsub("ShiftState_get_ctrl_0",ShiftState_get_ctrl_0);
rt.addsub("ShiftState_get_lshift_0",ShiftState_get_lshift_0);
rt.addsub("ShiftState_get_rshift_0",ShiftState_get_rshift_0);
rt.addsub("Timer_op_new_0",Timer_op_new_0);
rt.addsub("Timer_get_handler_0",Timer_get_handler_0);
rt.addsub("Timer_set_handler_1_Object",Timer_set_handler_1_Object);
rt.addsub("Timer_start_0",Timer_start_0);
rt.addsub("Timer_stop_0",Timer_stop_0);
rt.addsub("Timer_get_running_0",Timer_get_running_0);
rt.addsub("Timer_get_interval_0",Timer_get_interval_0);
rt.addsub("Timer_set_interval_1_float",Timer_set_interval_1_float);
rt.addsub("static_Timer_timer_2_float_",static_Timer_timer_2_float_);
rt.addsub("static_OpenDialog_run_1_string",static_OpenDialog_run_1_string);
rt.addsub("OpenDialog_execute_0",OpenDialog_execute_0);
rt.addsub("OpenDialog_get_file_0",OpenDialog_get_file_0);
rt.addsub("OpenDialog_set_file_1_string",OpenDialog_set_file_1_string);
rt.addsub("OpenDialog_get_initialDir_0",OpenDialog_get_initialDir_0);
rt.addsub("OpenDialog_set_initialDir_1_string",OpenDialog_set_initialDir_1_string);
rt.addsub("OpenDialog_get_files_0",OpenDialog_get_files_0);
rt.addsub("OpenDialog_get_filter_0",OpenDialog_get_filter_0);
rt.addsub("OpenDialog_set_filter_1_string",OpenDialog_set_filter_1_string);
rt.addsub("OpenDialog_get_multiSelect_0",OpenDialog_get_multiSelect_0);
rt.addsub("OpenDialog_set_multiSelect_1_bool",OpenDialog_set_multiSelect_1_bool);
rt.addsub("OpenDialog_op_new_0",OpenDialog_op_new_0);
rt.addsub("static_SaveDialog_run_1_string",static_SaveDialog_run_1_string);
rt.addsub("SaveDialog_execute_0",SaveDialog_execute_0);
rt.addsub("SaveDialog_get_initialDir_0",SaveDialog_get_initialDir_0);
rt.addsub("SaveDialog_set_initialDir_1_string",SaveDialog_set_initialDir_1_string);
rt.addsub("SaveDialog_get_file_0",SaveDialog_get_file_0);
rt.addsub("SaveDialog_set_file_1_string",SaveDialog_set_file_1_string);
rt.addsub("SaveDialog_op_new_0",SaveDialog_op_new_0);
rt.addsub("SaveDialog_get_filter_0",SaveDialog_get_filter_0);
rt.addsub("SaveDialog_set_filter_1_string",SaveDialog_set_filter_1_string);
rt.addsub("static_ColorDialog_run_1_string",static_ColorDialog_run_1_string);
rt.addsub("PaneGrp_op_new_0",PaneGrp_op_new_0);
rt.addsub("PaneGrp_get_canvas_0",PaneGrp_get_canvas_0);
rt.addsub("PaneGrp_beginDoc_1_Canvas",PaneGrp_beginDoc_1_Canvas);
rt.addsub("PaneGrp_endDoc_0",PaneGrp_endDoc_0);
rt.addsub("PaneGrp_get_count_0",PaneGrp_get_count_0);
rt.addsub("PaneGrp_op_get_index_1_int",PaneGrp_op_get_index_1_int);
rt.addsub("PaneGrp_sheetCount_1_int",PaneGrp_sheetCount_1_int);
rt.addsub("PaneGrp_replaceText_2_string_string",PaneGrp_replaceText_2_string_string);
rt.addsub("Pane_get_isFirst_0",Pane_get_isFirst_0);
rt.addsub("Pane_get_isLast_0",Pane_get_isLast_0);
rt.addsub("Pane_get_hi_0",Pane_get_hi_0);
rt.addsub("Pane_get_parent_0",Pane_get_parent_0);
rt.addsub("Pane_get_canvas_0",Pane_get_canvas_0);
rt.addsub("LogPage_addPane_1_Pane",LogPage_addPane_1_Pane);
rt.addsub("LogPage_canAddPane_1_Pane",LogPage_canAddPane_1_Pane);
rt.addsub("LogPage_get_hi_0",LogPage_get_hi_0);
rt.addsub("LogPage_get_isBlocked_0",LogPage_get_isBlocked_0);
rt.addsub("LogPage_get_isFreeBegin_0",LogPage_get_isFreeBegin_0);
rt.addsub("LogPage_get_isFreeEnd_0",LogPage_get_isFreeEnd_0);
rt.addsub("LogPage_get_isLeft_0",LogPage_get_isLeft_0);
rt.addsub("LogPage_addPaneGrp_1_PaneGrp",LogPage_addPaneGrp_1_PaneGrp);
rt.addsub("LogPage_getExtraPageCount_1_PaneGrp",LogPage_getExtraPageCount_1_PaneGrp);
rt.addsub("LogPage_get_count_0",LogPage_get_count_0);
rt.addsub("LogPage_op_get_index_1_int",LogPage_op_get_index_1_int);
rt.addsub("LogPage_erasePane_2_int_int",LogPage_erasePane_2_int_int);
rt.addsub("LogPage_insertPane_2_int_Pane",LogPage_insertPane_2_int_Pane);
rt.addsub("LogPage_draw_1_Canvas",LogPage_draw_1_Canvas);
rt.addsub("MetaCanvas_draw_1_Canvas",MetaCanvas_draw_1_Canvas);
rt.addsub("MetaCanvas_replaceText_2_string_string",MetaCanvas_replaceText_2_string_string);
rt.addsub("MetaCanvas_get_canvas_0",MetaCanvas_get_canvas_0);
rt.addsub("MetaCanvas_op_new_0",MetaCanvas_op_new_0);
rt.addsub("ResizeCanvas_op_new_3_Canvas_float_float",ResizeCanvas_op_new_3_Canvas_float_float);
rt.addsub("ResizeCanvas_get_canvas_0",ResizeCanvas_get_canvas_0);
rt.addsub("SubCanvas_op_new_3_Canvas_int_int",SubCanvas_op_new_3_Canvas_int_int);
rt.addsub("SubCanvas_get_canvas_0",SubCanvas_get_canvas_0);
rt.addsub("TextCanvas_op_new_2_Canvas_int",TextCanvas_op_new_2_Canvas_int);
rt.addsub("TextCanvas_op_new_3_Canvas_int_int",TextCanvas_op_new_3_Canvas_int_int);
rt.addsub("TextCanvas_text_1_string",TextCanvas_text_1_string);
rt.addsub("TextCanvas_font_1_Font",TextCanvas_font_1_Font);
rt.addsub("TextCanvas_wrap_0",TextCanvas_wrap_0);
rt.addsub("TextCanvas_flush_0",TextCanvas_flush_0);
rt.addsub("TextCanvas_textElemX_3_int_int_string",TextCanvas_textElemX_3_int_int_string);
rt.addsub("TextCanvas_textElem_2_int_string",TextCanvas_textElem_2_int_string);
rt.addsub("PageDistribution_get_sheetCount_0",PageDistribution_get_sheetCount_0);
rt.addsub("PageDistribution_get_freePageCount_0",PageDistribution_get_freePageCount_0);
rt.addsub("PageDistribution_draw_2_int_Canvas",PageDistribution_draw_2_int_Canvas);
rt.addsub("LogPages_op_new_0",LogPages_op_new_0);
rt.addsub("LogPages_add_1_PaneGrp",LogPages_add_1_PaneGrp);
rt.addsub("LogPages_print_1_Printer",LogPages_print_1_Printer);
rt.addsub("LogPages_loadPropsFrom_1_Printer",LogPages_loadPropsFrom_1_Printer);
rt.addsub("LogPages_setPageSize_2_int_int",LogPages_setPageSize_2_int_int);
rt.addsub("LogPages_complete_2_Array_Array",LogPages_complete_2_Array_Array);
rt.addsub("LogPages_completeLines_2_Array_Array",LogPages_completeLines_2_Array_Array);
rt.addsub("LogPages_get_pageHi_0",LogPages_get_pageHi_0);
rt.addsub("LogPages_get_pageWi_0",LogPages_get_pageWi_0);
rt.addsub("LogPages_op_get_index_1_int",LogPages_op_get_index_1_int);
rt.addsub("LogPages_addPage_0",LogPages_addPage_0);
rt.addsub("LogPages_get_count_0",LogPages_get_count_0);
rt.addsub("LogPages_get_last_0",LogPages_get_last_0);
rt.addsub("LogPages_searchPane_1_Pane",LogPages_searchPane_1_Pane);
rt.addsub("MenuItem_get_text_0",MenuItem_get_text_0);
rt.addsub("MenuItem_set_text_1_string",MenuItem_set_text_1_string);
rt.addsub("MenuItem_get_checked_0",MenuItem_get_checked_0);
rt.addsub("MenuItem_set_checked_1_bool",MenuItem_set_checked_1_bool);
rt.addsub("MenuItem_get_shortcut_0",MenuItem_get_shortcut_0);
rt.addsub("MenuItem_set_shortcut_1_string",MenuItem_set_shortcut_1_string);
rt.addsub("MenuItem_load_1_XMLNode",MenuItem_load_1_XMLNode);
rt.addsub("MenuItem_op_new_0",MenuItem_op_new_0);
rt.addsub("WaitWindow_show_0",WaitWindow_show_0);
rt.addsub("WaitWindow_hide_0",WaitWindow_hide_0);
rt.addsub("WaitWindow_get_msg_0",WaitWindow_get_msg_0);
rt.addsub("WaitWindow_set_msg_1_string",WaitWindow_set_msg_1_string);
rt.addsub("WaitWindow_op_new_0",WaitWindow_op_new_0);
rt.addsub("WaitWindow_destroy_0",WaitWindow_destroy_0);
rt.addsub("MidiOut_op_new_0",MidiOut_op_new_0);
rt.addsub("MidiOut_open_0",MidiOut_open_0);
rt.addsub("MidiOut_message_3_int_int_int",MidiOut_message_3_int_int_int);
rt.addsub("MidiOut_close_0",MidiOut_close_0);
rt.addsub("MidiOut_noteOn_3_int_int_int",MidiOut_noteOn_3_int_int_int);
rt.addsub("MidiOut_noteOff_3_int_int_int",MidiOut_noteOff_3_int_int_int);
rt.addsub("MidiOut_setInstrument_2_int_int",MidiOut_setInstrument_2_int_int);

}

