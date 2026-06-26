bool Archive_list_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
try {
    zbArchive *thiz=dynamic_cast<zbArchive*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Archive.list");return;}
  
zbObject *res=thiz->list()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  } catch(JException &e) {
    ir->nativeerror(e.getmsg());
    return true;
  }
  
  return true;

}
bool Archive_ropen_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *file=params[0];
 
 
try {
    zbArchive *thiz=dynamic_cast<zbArchive*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Archive.ropen");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->open(file->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  } catch(JException &e) {
    ir->nativeerror(e.getmsg());
    return true;
  }
  
  return true;

}
bool Archive_wopen_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *file=params[0];
 
 
try {
    zbArchive *thiz=dynamic_cast<zbArchive*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Archive.wopen");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->create(file->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  } catch(JException &e) {
    ir->nativeerror(e.getmsg());
    return true;
  }
  
  return true;

}
bool Archive_loadXML_2_string_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *item=params[0];
 
 
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[1]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Archive.loadXML,expected XMLNode,found %s",typeid(*params[1]).name()));return;}
 
 
try {
    zbArchive *thiz=dynamic_cast<zbArchive*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Archive.loadXML");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->loadxml(item->tostring(),xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  } catch(JException &e) {
    ir->nativeerror(e.getmsg());
    return true;
  }
  
  return true;

}
bool Archive_saveXML_2_string_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *item=params[0];
 
 
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[1]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:Archive.saveXML,expected XMLNode,found %s",typeid(*params[1]).name()));return;}
 
 
try {
    zbArchive *thiz=dynamic_cast<zbArchive*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Archive.saveXML");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->savexml(item->tostring(),xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  } catch(JException &e) {
    ir->nativeerror(e.getmsg());
    return true;
  }
  
  return true;

}
bool Archive_loadText_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *item=params[0];
 
 
try {
    zbArchive *thiz=dynamic_cast<zbArchive*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Archive.loadText");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->loadtext(item->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  } catch(JException &e) {
    ir->nativeerror(e.getmsg());
    return true;
  }
  
  return true;

}
bool Archive_saveText_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *item=params[0];
 
 

   zbObject *text=params[1];
 
 
try {
    zbArchive *thiz=dynamic_cast<zbArchive*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Archive.saveText");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->savetext(item->tostring(),text->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  } catch(JException &e) {
    ir->nativeerror(e.getmsg());
    return true;
  }
  
  return true;

}
bool Archive_close_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
try {
    zbArchive *thiz=dynamic_cast<zbArchive*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Archive.close");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->close();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  } catch(JException &e) {
    ir->nativeerror(e.getmsg());
    return true;
  }
  
  return true;

}
bool Archive_op_new_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
zbObject *res=newarchive(ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Archive_unpack_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *file=params[0];
 
 

   zbObject *destPath=params[1];
 
 

    zbArchive *thiz=dynamic_cast<zbArchive*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Archive.unpack");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->unpackfile(file->tostring(),destPath->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool Archive_pack_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *file=params[0];
 
 

   zbObject *srcPath=params[1];
 
 

    zbArchive *thiz=dynamic_cast<zbArchive*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:Archive.pack");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->packfile(file->tostring(),srcPath->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_listChilds_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
try {
    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.listChilds");return;}
  
zbObject *res=createString(thiz->listChilds(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  } catch(JException &e) {
    ir->nativeerror(e.getmsg());
    return true;
  }
  
  return true;

}
bool PersistentObject_listFiles_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);
try {
    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.listFiles");return;}
  
zbObject *res=createString(thiz->listFiles(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  } catch(JException &e) {
    ir->nativeerror(e.getmsg());
    return true;
  }
  
  return true;

}
bool PersistentObject_op_new_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *file=params[0];
 
 
zbObject *res=newpersistentobject(file->tostring(),ir->rt)
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_openChild_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.openChild");return;}
  
zbObject *res=thiz->openChild(name->tostring())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_openChildPath_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.openChildPath");return;}
  
zbObject *res=thiz->openChildPath(name->tostring())
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_getFile_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 
try {
    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.getFile");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getfile(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  } catch(JException &e) {
    ir->nativeerror(e.getmsg());
    return true;
  }
  
  return true;

}
bool PersistentObject_closedGetFile_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *path=params[0];
 
 

   zbObject *name=params[1];
 
 

    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.closedGetFile");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->closedgetfile(path->tostring(),name->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_hasFile_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *file=params[0];
 
 

    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.hasFile");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->hasfile(file->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_hasChild_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *file=params[0];
 
 

    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.hasChild");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createLogical(thiz->getlink()->hasfile(file->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_setFile_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.setFile");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->setfile(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_closedSetFile_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *path=params[0];
 
 

   zbObject *name=params[1];
 
 

    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.closedSetFile");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->closedsetfile(path->tostring(),name->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_loadXML_2_string_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[1]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:PersistentObject.loadXML,expected XMLNode,found %s",typeid(*params[1]).name()));return;}
 
 

    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.loadXML");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->loadxml(name->tostring(),xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_saveXML_2_string_XMLNode(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 
zbXMLNode *xml=dynamic_cast<zbXMLNode*>(params[1]);
  if (! xml) return false;// {ir->error(fmtstr("ArgError:PersistentObject.saveXML,expected XMLNode,found %s",typeid(*params[1]).name()));return;}
 
 

    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.saveXML");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->savexml(name->tostring(),xml->cvalue());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_loadText_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.loadText");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->loadtext(name->tostring()),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_saveText_2_string_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<2 || parcnt>2) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *name=params[0];
 
 

   zbObject *value=params[1];
 
 

    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.saveText");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->savetext(name->tostring(),value->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_get_parent_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.get:parent");return;}
  
zbObject *res=thiz->getparent()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_flush_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.flush");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->flush();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_get_name_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.get:name");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createString(thiz->getlink()->getname(),ir->rt);
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_remove_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.remove");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->remove();
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_get_root_0(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<0 || parcnt>0) return false;
  zbObject **params=ir->getparams(parcnt);

    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.get:root");return;}
  
zbObject *res=thiz->getroot()
;
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}
bool PersistentObject_removeFile_1_string(Interpreter *ir,int parcnt) {
  ir->nativebegin();
  
  if (parcnt<1 || parcnt>1) return false;
  zbObject **params=ir->getparams(parcnt);

   zbObject *file=params[0];
 
 

    zbPersistentObject *thiz=dynamic_cast<zbPersistentObject*>(ir->getfnthis(parcnt));
  if (! thiz) return false;//{ir->error("ThisError:PersistentObject.removeFile");return;}
  if(!thiz->getlink()) return false;
zbObject *res=createVoid(ir->rt);thiz->getlink()->removefile(file->tostring());
  if (!ir->isnativeerror()) ir->setfnresult(res,parcnt);
  
  return true;

}






void init_extobjs_rt(Runtime &rt) {
rt.addsub("Archive_list_0",Archive_list_0);
rt.addsub("Archive_ropen_1_string",Archive_ropen_1_string);
rt.addsub("Archive_wopen_1_string",Archive_wopen_1_string);
rt.addsub("Archive_loadXML_2_string_XMLNode",Archive_loadXML_2_string_XMLNode);
rt.addsub("Archive_saveXML_2_string_XMLNode",Archive_saveXML_2_string_XMLNode);
rt.addsub("Archive_loadText_1_string",Archive_loadText_1_string);
rt.addsub("Archive_saveText_2_string_string",Archive_saveText_2_string_string);
rt.addsub("Archive_close_0",Archive_close_0);
rt.addsub("Archive_op_new_0",Archive_op_new_0);
rt.addsub("Archive_unpack_2_string_string",Archive_unpack_2_string_string);
rt.addsub("Archive_pack_2_string_string",Archive_pack_2_string_string);
rt.addsub("PersistentObject_listChilds_0",PersistentObject_listChilds_0);
rt.addsub("PersistentObject_listFiles_0",PersistentObject_listFiles_0);
rt.addsub("PersistentObject_op_new_1_string",PersistentObject_op_new_1_string);
rt.addsub("PersistentObject_openChild_1_string",PersistentObject_openChild_1_string);
rt.addsub("PersistentObject_openChildPath_1_string",PersistentObject_openChildPath_1_string);
rt.addsub("PersistentObject_getFile_1_string",PersistentObject_getFile_1_string);
rt.addsub("PersistentObject_closedGetFile_2_string_string",PersistentObject_closedGetFile_2_string_string);
rt.addsub("PersistentObject_hasFile_1_string",PersistentObject_hasFile_1_string);
rt.addsub("PersistentObject_hasChild_1_string",PersistentObject_hasChild_1_string);
rt.addsub("PersistentObject_setFile_1_string",PersistentObject_setFile_1_string);
rt.addsub("PersistentObject_closedSetFile_2_string_string",PersistentObject_closedSetFile_2_string_string);
rt.addsub("PersistentObject_loadXML_2_string_XMLNode",PersistentObject_loadXML_2_string_XMLNode);
rt.addsub("PersistentObject_saveXML_2_string_XMLNode",PersistentObject_saveXML_2_string_XMLNode);
rt.addsub("PersistentObject_loadText_1_string",PersistentObject_loadText_1_string);
rt.addsub("PersistentObject_saveText_2_string_string",PersistentObject_saveText_2_string_string);
rt.addsub("PersistentObject_get_parent_0",PersistentObject_get_parent_0);
rt.addsub("PersistentObject_flush_0",PersistentObject_flush_0);
rt.addsub("PersistentObject_get_name_0",PersistentObject_get_name_0);
rt.addsub("PersistentObject_remove_0",PersistentObject_remove_0);
rt.addsub("PersistentObject_get_root_0",PersistentObject_get_root_0);
rt.addsub("PersistentObject_removeFile_1_string",PersistentObject_removeFile_1_string);

}

