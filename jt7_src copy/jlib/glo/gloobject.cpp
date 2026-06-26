#include <jlib.h>
#pragma hdrstop

#include "gloobject.h"
#include "glovrml.h"

namespace glo {

gloColor *glo_loadcolor(XMLNode xml,gloVariableList *vars) {
  if (glo_isvariable(xml.svar("r")) || glo_isvariable(xml.svar("g")) ||
   glo_isvariable(xml.svar("b")) || glo_isvariable(xml.svar("a"))) {
    gloVarColor *res=new gloVarColor(xml);
    if (res->defined(vars)) {
      gloColor4f *r2=new gloColor4f();
      r2->r=res->r->eval(vars);
      r2->g=res->g->eval(vars);
      r2->b=res->b->eval(vars);
      r2->a=res->a->eval(vars);
      r2->iscolor=true;
      delete res;
      return r2;
    }
    return res;
  }
  return new gloColor4f(xml);
}

gloVarColor::gloVarColor(XMLNode xml) {
  r=glo_parsefloat(xml.svar("r"));
  g=glo_parsefloat(xml.svar("g"));
  b=glo_parsefloat(xml.svar("b"));
  a=glo_parsefloat(xml.svar("a"));
}
bool gloVarColor::render(RenderContext &rc) {
  float rv=r->eval(rc.vars),gv=g->eval(rc.vars),bv=b->eval(rc.vars),av=a->eval(rc.vars);
  if (av==0) av=1;
  glColor4f(rv,gv,bv,av);
  return true;
}

gloObject *glo_load_object(XMLNode xml,PersistentObject *archive,gloVariableList *vars) {
  while (xml.svar("glomodel.loadchild")!="") xml=xml[xml.svar("glomodel.loadchild")];
  #define setres(type) {type *res=new type();res->load(xml,archive,vars);return res;}
  if (xml.getname()=="rotate") setres(gloRotate);
  if (xml.getname()=="translate") setres(gloTranslate);
  if (xml.getname()=="scale") setres(gloScale);
  if (xml.getname()=="polygon") setres(gloPolygon);
  if (xml.getname()=="polyline") setres(gloPolyLine);
  if (xml.getname()=="settings") setres(gloSettings);
  if (xml.getname()=="inline") setres(gloInline);
  if (xml.count()>=1) setres(gloPolyObject);
  #undef setres
  return NULL;
}

void gloModel::drawprojection(gloProjectionCanvas &cnv,int dir,float sx,float sy,float tx,float ty) {
  if (obj) obj->drawprojection(cnv,dir,sx,sy,tx,ty);
}

void gloPolyObject::drawprojection(gloProjectionCanvas &cnv,int dir,float sx,float sy,float tx,float ty) {
  for(int i=0;i<childs.count;i++) childs[i]->drawprojection(cnv,dir,sx,sy,tx,ty);
}

/*
FaceMode RenderContext::setfacemode(FaceMode value) {
  FaceMode res=fm;fm=value;
  switch (value) {
    case fm_fill:glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);break;
    case fm_line:glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);break;
    case fm_point:glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);break;
  }
  return res;
}
*/

void gloSettings::load(XMLNode xml,PersistentObject *archive,gloVariableList *vars) {
  int cnt=xml.count();
  if (cnt==0) return;
  if (cnt==1) obj=glo_load_object(xml[0],archive,vars);
  else {
    gloPolyObject *poly=new gloPolyObject();
    poly->load(xml,archive,vars);
    obj=poly;
  }
//  string fms=xml.svar("facemode");
//  if (fms=="line") fm=fm_line;
//  else if (fms=="point") fm=fm_point;
//  else fm=fm_fill;

}
void gloSettings::render(RenderContext &rc) {
  if (obj) {
//    FaceMode bakfm=rc.setfacemode(fm);
    obj->render(rc);
//    rc.setfacemode(bakfm);
  }
}

void gloMoreVerteces::loadverteces(XMLNode xml,gloVariableList *vars) {
  for(int i=0;i<xml.count();i++) {
    gloVertex *v=new gloVertex();
    verteces.add(v);
    v->load(xml[i],vars);
  }
}

void gloPolygon::load(XMLNode xml,PersistentObject *archive,gloVariableList *vars) {
  nx=xml.fvar("nx");
  ny=xml.fvar("ny");
  nz=xml.fvar("nz");
  denylight=xml.ivar("denylight");
  culling=xml.ivar("culling");
  color=glo_loadcolor(xml,vars);
  texture.load(xml,archive,vars);
  loadverteces(xml,vars);
  if (nx==0 && ny==0 && nz==0) {
    if (verteces.count>=3) {
      gloVertex3f &va=verteces[0]->position,&vb=verteces[1]->position,&vc=verteces[2]->position;
      fpoint3d a(va.x,va.y,va.z),b(vb.x,vb.y,vb.z),c(vc.x,vc.y,vc.z);
      fvector3d s1=b-a,s2=b-c;
      fvector3d n=s1%s2;
      nx=n.x;ny=n.y;nz=n.z;
    }
  }
}

gloObject *gloPolyObject::clone() {
  gloPolyObject *res=new gloPolyObject();
  for(int i=0;i<childs.count;i++) if (gloObject *o=childs[i]->clone()) res->childs.add(o);
  return res;
}

gloObject *gloInline::clone() {
  gloInline *res=new gloInline();
  res->obj=obj->clone();
  res->sx=sx;
  res->sy=sy;
  res->sz=sz;
  res->tx=tx;
  res->ty=ty;
  res->tz=tz;
  return res;
}

void gloMoreVerteces::assign(gloMoreVerteces &src) {
  verteces.clear();
  for(int i=0;i<src.verteces.count;i++) verteces.add(src.verteces[i]->clone());
}

gloVertex *gloVertex::clone() {
  gloVertex *res=new gloVertex();
  res->position=position;
  res->texcoord=texcoord;
  return res;
}

void gloPolygon::assign(gloPolygon &src) {
  gloMoreVerteces::assign(src);
  texture=src.texture;
  culling=src.culling;
  if (src.color) color=src.color->clone();
  nx=src.nx;ny=src.ny;nz=src.nz;
}

gloObject *gloPolygon::clone() {
  gloPolygon *res=new gloPolygon();
  res->assign(*this);
  return res;
}

void gloPolyLine::assign(gloPolyLine &src) {
  gloMoreVerteces::assign(src);
  texture=src.texture;
  closed=src.closed;
  linewi=src.linewi;
  polygon=src.polygon;
  frequency=src.frequency;
  if (src.color) color=src.color->clone();
}

gloObject *gloPolyLine::clone() {
  gloPolyLine *res=new gloPolyLine();
  res->assign(*this);
  return res;
}


void gloPolyObject::savetovrml(STROUT &fw,VRMLTextures *textures) {
  for(int i=0;i<childs.count;i++) childs[i]->savetovrml(fw,textures);
}
void gloMoreVerteces::savevrmlverteces(STROUT &fw) {
  for(int i=0;i<verteces.count;i++) {
    gloVertex *v=verteces[i];
    fw<<v->position.x<<"  "<<v->position.y<<" "<<v->position.z;
    if (i<verteces.count-1) fw<<",";
    fw<<"\n";
  }
}
void gloPolygon::savevrmltexcoords(STROUT &fw) {
  for(int i=0;i<verteces.count;i++) {
    gloVertex *v=verteces[i];
    fw<<v->texcoord.s<<"  "<<v->texcoord.t;
    if (i<verteces.count-1) fw<<",";
    fw<<"\n";
  }
}
void gloPolygon::_savetovrml(STROUT &fw,VRMLTextures *textures) {
  fw<<"Shape {\n";
  if (texture.isdefined()) {
    fw<<"appearance Appearance {\n";
    sFile texfile=textures->add(texture.tex);
    fw<<"texture ImageTexture { url \""<<textures->textdirname.c_str()<<"/"<<texfile.getfile().c_str()<<"\"}\n";
    fw<<"material Material {\n";
    fw<<"transparency 0\n";
    fw<<"}\n";
    fw<<"}\n";
  }
  fw<<"geometry IndexedFaceSet {\n";
  fw<<"coord Coordinate {\n";
  fw<<"point [\n";
  savevrmlverteces(fw);
  fw<<"]\n";
  fw<<"}\n";
  fw<<"coordIndex [";
  for(int i=0;i<verteces.count;i++) {
    fw<<i;
    if (i<verteces.count-1) fw<<",";
  }
  fw<<"]\n";
  fw<<"texCoord TextureCoordinate {\n";
  fw<<"point [\n";
  savevrmltexcoords(fw);
  fw<<"]\n";
  fw<<"}\n";
  fw<<"}\n";
  fw<<"}\n";
}
void gloPolyLine::savetovrml(STROUT &fw,VRMLTextures *textures) {
  fw<<"Shape {\n";
  fw<<"geometry IndexedLineSet {\n";
  fw<<"coord Coordinate {\n";
  fw<<"point [\n";
  savevrmlverteces(fw);
  fw<<"]\n";
  fw<<"}\n";
  fw<<"coordIndex [";
  for(int i=0;i<verteces.count;i++) {
    fw<<i;
    if (i<verteces.count-1) fw<<",";
  }
  if (closed) fw<<","<<0;
  fw<<"]\n";
  fw<<"}\n";
  fw<<"}\n";
}

void gloInline::savevrmltransforms(STROUT &fw) {
  fw<<"scale "<<sx<<" "<<sy<<" "<<sz<<"\n";
  fw<<"translation "<<tx<<" "<<ty<<" "<<tz<<"\n";
}

void gloTransformation::savetovrml(STROUT &fw,VRMLTextures *textures) {
  fw<<"Transform {\n";
  savevrmltransforms(fw);
  fw<<"children [\n";
  if (obj) obj->savetovrml(fw,textures);
  fw<<"]\n";
  fw<<"}\n";
}

void gloPolygon::savetovrml(STROUT &fw,VRMLTextures *textures) {
  if (culling) {
    _savetovrml(fw,textures);
  } else {
    _savetovrml(fw,textures);
    verteces.reverse();
    _savetovrml(fw,textures);
    verteces.reverse();
  }
}

void gloInline::transform(RenderContext &rc) {
  glTranslatef(tx,ty,tz);
  glScalef(sx,sy,sz);
}
void gloInline::load(XMLNode xml,PersistentObject *archive,gloVariableList *vars) {
  gloVariableList newvars;
  newvars.parent=vars;
  XMLNode vx=xml["args"];
  for(int i=0;i<vx.varcount();i++) newvars.setvar(vx.getvarname(i),vx.getvar(i));
  if (archive) {
    string name=xml.svar("name");
    PersistentObject *o=archive->openchildpath(name);
    if (o->hasfile("index.xml")) {
      MainXMLNode cxml;
      o->loadxml("index.xml",cxml);
      obj=glo_load_object(cxml,o,&newvars);
      sx=xml.fvar("sx");sy=xml.fvar("sy");sz=xml.fvar("sz");
      tx=xml.fvar("tx");ty=xml.fvar("ty");tz=xml.fvar("tz");
    }
  }
}

void gloTransformation::load(XMLNode xml,PersistentObject *archive,gloVariableList *vars) {
  int cnt=xml.count();
  if (cnt==0) return;
  if (cnt==1) obj=glo_load_object(xml[0],archive,vars);
  else {
    gloPolyObject *poly=new gloPolyObject();
    poly->load(xml,archive,vars);
    obj=poly;
  }
}

void gloTranslate::load(XMLNode xml,PersistentObject *archive,gloVariableList *vars) {
  gloTransformation::load(xml,archive,vars);
  x=xml.fvar("x");y=xml.fvar("y");z=xml.fvar("z");
}

void gloScale::load(XMLNode xml,PersistentObject *archive,gloVariableList *vars) {
  gloTransformation::load(xml,archive,vars);
  x=xml.fvar("x");y=xml.fvar("y");z=xml.fvar("z");
}

void gloRotate::load(XMLNode xml,PersistentObject *archive,gloVariableList *vars) {
  gloTransformation::load(xml,archive,vars);
  x=xml.fvar("x");y=xml.fvar("y");z=xml.fvar("z");
  angle=glo_parsefloat(xml.svar("angle"));
//  angle=xml.fvar("angle");
}

void gloBoundBox::extend(const gloBoundBox &box) {
  if (empty) {
    *this=box;
  } else {
    if (box.x1<x1) x1=box.x1;
    if (box.y1<y1) y1=box.y1;
    if (box.z1<z1) z1=box.z1;
    if (box.x2>x2) x2=box.x2;
    if (box.y2>y2) y2=box.y2;
    if (box.z2>z2) z2=box.z2;
  }
}

void gloBoundBox::extend(float x,float y,float z) {
  if (empty) {
    x1=x2=x;
    y1=y2=y;
    z1=z2=z;
    empty=false;
  } else {
    if (x<x1) x1=x;
    if (y<y1) y1=y;
    if (z<z1) z1=z;
    if (x>x2) x2=x;
    if (y>y2) y2=y;
    if (z>z2) z2=z;
  }
}

void gloBoundBox::translate(float x,float y,float z) {
  if (empty) return;
  x1+=x;y1+=y;z1+=z;
  x2+=x;y2+=y;z2+=z;
}

void gloBoundBox::scale(float x,float y,float z) {
  if (empty) return;
  x1*=x;y1*=y;z1*=z;
  x2*=x;y2*=y;z2*=z;
}

float hypot3(float x,float y,float z) {
  return sqrt(x*x+y*y+z*z);
}

void gloBoundBox::extendrotate() {
  if (empty) return;
  float a=hypot3(x1,y1,z1);
  float b=hypot3(x1,y1,z2);
  float c=hypot3(x1,y2,z1);
  float d=hypot3(x1,y2,z2);
  float e=hypot3(x2,y1,z1);
  float f=hypot3(x2,y1,z2);
  float g=hypot3(x2,y2,z1);
  float h=hypot3(x2,y2,z2);
  float r=a;
  if (b>r) r=b;
  if (c>r) r=c;
  if (d>r) r=d;
  if (e>r) r=e;
  if (f>r) r=f;
  if (g>r) r=g;
  if (h>r) r=h;
  x1=-r;y1=-r;z1=-r;
  x2=r;y2=r;z2=r;
}

gloBoundBox gloPolyObject::bound() {
  gloBoundBox res;
  for(int i=0;i<childs.count;i++) {
    gloBoundBox r=childs[i]->bound();
    res.extend(r);
  }
  return res;
}

void gloPolyObject::render(RenderContext &rc) {
  for(int i=0;i<childs.count;i++) childs[i]->render(rc);
}

void gloPolyObject::load(XMLNode xml,PersistentObject *archive,gloVariableList *vars) {
  for(int i=0;i<xml.count();i++)
    if (gloObject *obj=glo_load_object(xml[i],archive,vars))
      childs.add(obj);
}

void gloTransformation::render(RenderContext &rc) {
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  transform(rc);
  if (obj) obj->render(rc);
  glPopMatrix();
}

void gloMoreVerteces::renderverteces(RenderContext &rc,bool rendertex) {
  for(int i=0;i<verteces.count;i++) verteces[i]->render(rc,rendertex);
}

void gloPolygon::render(RenderContext &rc) {
  bool islight=rc.getlighting();
  glNormal3f(nx,ny,nz);
  rc.setculling(culling);
  if (denylight) rc.setlighting(false);
  if (!color || !color->render(rc)) glColor3f(1,1,1);
  TexApplyData tad;
  texture.beforeapply(tad,rc);
  glBegin(GL_POLYGON);
  renderverteces(rc,texture.isdefined());
  glEnd();
  texture.afterapply(tad,rc);
  rc.setlighting(islight);
}

void gloMoreVerteces::drawprojection(gloProjectionCanvas &cnv,int dir,float sx,float sy,float tx,float ty) {
  TCntVec<glopoint> pts;

//  const char *udirx[3]={"z","x","x"};
//  const char *udiry[3]={"y","z","y"};
  pts.wantcapa(verteces.count);
  for(int i=0;i<verteces.count;i++) {
    glopoint &pt=pts.add();
    switch (dir) {
      case 0:pt.x=verteces[i]->position.z*sx+tx;pt.y=verteces[i]->position.y*sy+ty;break;
      case 1:pt.x=verteces[i]->position.x*sx+tx;pt.y=verteces[i]->position.z*sy+ty;break;
      case 2:pt.x=verteces[i]->position.x*sx+tx;pt.y=verteces[i]->position.y*sy+ty;break;
    }
  }
/*
      pts.add(Point(
        round(polygon[i].fvar(udirx[dir])*sx+tx),
        round(polygon[i].fvar(udiry[dir])*sy+ty)
      ));
    if (ispolygon) cnv.polygon(pts.buf,pts.count);
    else {
      for(int i=1;i<pts.count;i++) cnv.line(pts[i-1].x,pts[i-1].y,pts[i].x,pts[i].y);
      if (polygon.ivar("closed") && pts.count>=2) cnv.line(pts[0].x,pts[0].y,pts.last().x,pts.last().y);
    }
*/
  drawprojshape(pts,cnv);
}

fpoint3d gloMoreVerteces::pos(RenderContext &rc,int index) {
  gloVertex3f res=verteces[index]->position;
  return fpoint3d(res.x,res.y,res.z);
}

void gloPolyLine::render(RenderContext &rc) {
  bool l=rc.setlighting(false);
  if (polygon) {
    if (!color->render(rc)) glColor3f(1,1,1);
    TexApplyData tad;
    texture.beforeapply(tad,rc);
    float t=0;
    for(int i=1;i<verteces.count;i++) {
      fvector3d u=pos(rc,i)-pos(rc,i-1);
      float dt=u.size()*frequency;
      glTexLine(rc,t,t+dt,linewi,pos(rc,i-1),pos(rc,i));
      t+=dt;
    }
    if (closed && verteces.count>=3) {
      fvector3d u=pos(rc,verteces.count-1)-pos(rc,0);
      float dt=u.size()*frequency;
      glTexLine(rc,t,t+dt,linewi,pos(rc,verteces.count-1),pos(rc,0));
    }
    texture.afterapply(tad,rc);
  } else {
    if (!color->render(rc)) glColor3f(1,1,1);
    glLineWidth(linewi);
    glBegin(GL_LINE_STRIP);
    renderverteces(rc,false);
    if (closed && verteces.count>0) verteces[0]->render(rc,false);
    glEnd();
//    glLineWidth(1);
  }
  rc.setlighting(l);
  glColor3f(1,1,1);
}
void gloPolyLine::load(XMLNode xml,PersistentObject *archive,gloVariableList *vars) {
  loadverteces(xml,vars);
  color=glo_loadcolor(xml,vars);
  closed=xml.ivar("closed");
  linewi=xml.fvar("linewi");
  polygon=xml.ivar("polygon");
  frequency=xml.fvar("frequency");
  if (frequency<=0.001) frequency=1;
  texture.load(xml,archive,vars);
}

void gloTexture::afterapply(TexApplyData &data,RenderContext &rc) {
  if (tex) {
    rc.pop();
//    glPopAttrib();
//    if (transparent && !data.alphatest) glDisable(GL_ALPHA_TEST);
//    if (!transparent && data.alphatest) glEnable(GL_ALPHA_TEST);
//    if (!data.enabled) glDisable(GL_TEXTURE_2D);
  }
}

void gloTexture::beforeapply(TexApplyData &data,RenderContext &rc) {
  if (tex) {
//    data.enabled=glIsEnabled(GL_TEXTURE_2D);
//    data.alphatest=glIsEnabled(GL_ALPHA_TEST);
//    glPushAttrib(GL_ALPHA_TEST|GL_TEXTURE_2D);
//    glPushAttrib(GL_ENABLE_BIT);//GL_ALL_ATTRIB_BITS
    glAlphaFunc(GL_GEQUAL,0.5);
    rc.push();
    rc.settexture2d(true);
    rc.setalphatest(transparent);
    tex->bind(rc.ctxid);
  }
}

void gloTexture::load(XMLNode xml,PersistentObject *archive,gloVariableList *vars) {
  name=xml.svar("texture");
  transparent=xml.ivar("texture.transparent");
  if (name!="") tex=texman->tryfindtexture(name,archive);else tex=NULL;
}

void gloVertex3f::render(RenderContext &rc) {
  glVertex3f(x,y,z);
}
void gloVertex::render(RenderContext &rc,bool hastexture) {
  if (hastexture) texcoord.render(rc);else if (color) color->render(rc);
  position.render(rc);
}
void gloVertex3f::load(XMLNode xml) {
  x=xml.fvar("x");y=xml.fvar("y");z=xml.fvar("z");
}
void gloVertex::load(XMLNode xml,gloVariableList *vars) {
  position.load(xml);
  color=glo_loadcolor(xml,vars);
  texcoord.load(xml);
}
void gloColor4f::load(XMLNode xml) {
  iscolor=xml.svar("r")!="" || xml.svar("g")!="" || xml.svar("b")!="";
  if (iscolor) {
    r=xml.fvar("r");g=xml.fvar("g");b=xml.fvar("b");
    if (xml.isvar("a")) a=xml.fvar("a");else a=1;
  }
//  if (iscolor) glColor4f(r,g,b,a);
}
bool gloColor4f::render(RenderContext &rc) {
  if (iscolor) glColor4f(r,g,b,a);
  return iscolor;
}

void gloModel::savetovrml(sFile file) {
  MEMSTROUT fw;
  fw<<"#VRML V2.0 utf8\n";
  fw<<"Viewpoint { position 0 0 50 }\n";
  VRMLTextures textures;
  textures.textdirname=(file.getfile()%"")+"_textures";
  textures.textdir=file.getdir()/textures.textdirname;
  if (obj) obj->savetovrml(fw,&textures);
  if (file.testext("wrz")) gzsavetext(file.c_str(),fw.str().c_str(),9);
  else savetextfile(file.c_str(),fw.str().c_str());
  textures.save();
}

void gloModel::render(RenderContext &rc) {
  if (disabled) return;
  if (obj) {
    try {
      obj->render(rc);
    } catch (...) {
      disabled=true;
      throw;
    }
  }
}

gloBoundBox gloInline::bound() {
  if (obj) {
    gloBoundBox res=obj->bound();
    res.translate(tx,ty,tz);
    res.scale(sx,sy,sz);
    return res;
  }
  return gloBoundBox();
}

gloBoundBox gloTranslate::bound() {
  if (obj) {
    gloBoundBox res=obj->bound();
    res.translate(x,y,z);
    return res;
  }
  return gloBoundBox();
}

gloBoundBox gloRotate::bound() {
  if (obj) {
    gloBoundBox res=obj->bound();
    res.extendrotate();
    return res;
  }
  return gloBoundBox();
}

gloBoundBox gloScale::bound() {
  if (obj) {
    gloBoundBox res=obj->bound();
    res.scale(x,y,z);
    return res;
  }
  return gloBoundBox();
}

void gloRotate::transform(RenderContext &rc) {
  if (angle) glRotatef(angle->eval(rc.vars),x,y,z);
}

void gloModel::translate(float dx,float dy,float dz) {
  if (obj) {
    gloTranslate *t=new gloTranslate();
    t->x=dx;t->y=dy;t->z=dz;t->obj=obj;
    obj.obj=t;
  }
}
void gloModel::scale(float fx,float fy,float fz) {
  if (obj) {
    gloScale *t=new gloScale();
    t->x=fx;t->y=fy;t->z=fz;t->obj=obj;
    obj.obj=t;
  }
}

gloBoundBox gloVertex::bound() {
  gloBoundBox res;
  res.extend(position.x,position.y,position.y);
  return res;
}

gloBoundBox gloMoreVerteces::bound() {
  gloBoundBox res=gloBoundBox();
  for(int i=0;i<verteces.count;i++) {
    res.extend(verteces[i]->bound());
  }
  return res;
}

void Extrems::repair(float x,float y,float z) {
  if (x<xmin) xmin=x;
  if (y<ymin) ymin=y;
  if (z<zmin) zmin=z;
  if (x>xmax) xmax=x;
  if (y>ymax) ymax=y;
  if (z>zmax) zmax=z;
}

void gloMoreVerteces::getextrems(Extrems &e) {
  for(int i=0;i<verteces.count;i++) verteces[i]->getextrems(e);
}

void gloPolyObject::getextrems(Extrems &e) {
  for(int i=0;i<childs.count;i++) childs[i]->getextrems(e);
}

float Extrems::diam() {
  float dx=xmin-xmax,dy=ymin-ymax,dz=zmin-zmax;
  float v=dx*dx+dy*dy+dz*dz;
  return sqrt(v);
}

static void vertexptf(fpoint3d pt) {
  glVertex3f(pt.x,pt.y,pt.z);
}

void glTexLine(RenderContext &rc,float t1,float t2,float linewi,fpoint3d &a,fpoint3d &b) {
  rc.setculling(false);
  fvector3d u=b-a;
  const float epsilon=0.01;
  fvector3d n;
  if (fabs(u.y)>epsilon || fabs(u.z)>epsilon) n=fvector3d(0,-u.z,u.y);
  else if (fabs(u.x)>epsilon || fabs(u.z)>epsilon) n=fvector3d(-u.z,0,u.x);
  else if (fabs(u.x)>epsilon || fabs(u.y)>epsilon) n=fvector3d(-u.y,u.x,0);
  else return;//line to small, needn't draw
  fvector3d m=n%u;//druha normala
  n=n.size1();m=m.size1();
  float w2=linewi/2;
  glBegin(GL_QUADS);
  glTexCoord2f(t1,0);vertexptf(a-m*w2);
  glTexCoord2f(t1,1);vertexptf(a+m*w2);
  glTexCoord2f(t2,1);vertexptf(b+m*w2);
  glTexCoord2f(t2,0);vertexptf(b-m*w2);

  glTexCoord2f(t1,0);vertexptf(a-n*w2);
  glTexCoord2f(t1,1);vertexptf(a+n*w2);
  glTexCoord2f(t2,1);vertexptf(b+n*w2);
  glTexCoord2f(t2,0);vertexptf(b-n*w2);
  glEnd();
}

bool RenderContext::setlighting(bool value) {
  bool res=rp.lighting;rp.lighting=value;
  if (res!=rp.lighting) {
    if (rp.lighting) glEnable(GL_LIGHTING),glEnable(GL_NORMALIZE);
    else glDisable(GL_LIGHTING),glDisable(GL_NORMALIZE);
  }
  return res;
}
bool RenderContext::setsmoothshade(bool value) {
  bool res=rp.smoothshade;rp.smoothshade=value;
  if (res!=rp.smoothshade) {
    if (rp.smoothshade) glShadeModel(GL_SMOOTH);
    else glShadeModel(GL_FLAT);
  }
  return res;
}
#define set_func(name,flag)\
bool RenderContext::set##name(bool value) {\
  bool res=rp.name;rp.name=value;\
  if (res!=rp.name) {\
    if (rp.name) glEnable(flag);else glDisable(flag);\
  }\
  return res;\
}

set_func(culling,GL_CULL_FACE)
set_func(blending,GL_BLEND)
set_func(alphatest,GL_ALPHA_TEST)
set_func(texture2d,GL_TEXTURE_2D)
set_func(depthtest,GL_DEPTH_TEST)

void RenderContext::push() {
  if (stacksize>=0xFF) throw JError("RenderContext::stack full");
  stack[stacksize++]=rp;
}
void RenderContext::pop() {
  if (stacksize<=0) throw JError("RenderContext::stack empty");
  RenderProps &top=stack[--stacksize];
  setculling(top.culling);
  setblending(top.blending);
  setalphatest(top.alphatest);
  settexture2d(top.texture2d);
  setlighting(top.lighting);
  setdepthtest(top.depthtest);
  setsmoothshade(top.smoothshade);
}

}

