#ifndef gloobjectH
#define gloobjectH

#include <com/ptrto.h>
#include <opengl.h>
#include <ext/xmlnode.h>
#include "glotextures.h"
#include "glovars.h"
#include <ext/persistentobject.h>
#include <jmath/geom3d.h>

namespace glo {
struct VRMLTextures;

using namespace jmath;

struct gloBoundBox {
  float x1,y1,z1,x2,y2,z2;
  bool empty;
  gloBoundBox() {x1=y1=z1=x2=y2=z2=0;empty=true;}
  void extend(const gloBoundBox &box);
  void extend(float x,float y,float z);
  void extendrotate();
  void scale(float x,float y,float z);
  void translate(float x,float y,float z);
};

//enum FaceMode {fm_fill,fm_line,fm_point};
struct RenderProps {
  bool culling;
  bool lighting;
  bool texture2d;
  bool alphatest;
  bool blending;
  bool depthtest;
  bool smoothshade;
  RenderProps() {culling=lighting=texture2d=alphatest=blending=depthtest=smoothshade=false;}
};

struct RenderContext {
//  FaceMode fm;
  void *ctxid;
  gloVariableList *vars;
  RenderProps stack[0x100];
  int stacksize;

  RenderContext() {ctxid=NULL;vars=NULL;stacksize=0;}

//  FaceMode setfacemode(FaceMode value);
  bool setculling(bool value);
  bool setlighting(bool value);
  bool settexture2d(bool value);
  bool setalphatest(bool value);
  bool setblending(bool value);
  bool setdepthtest(bool value);
  bool setsmoothshade(bool value);

  bool gettexture2d() {return rp.texture2d;}
  bool getlighting() {return rp.lighting;}

  void push();
  void pop();
private:
  RenderProps rp;
  RenderContext(const RenderContext &src);
};

struct RenderContextReference:RCObject {
  RenderContext *rc;
  RenderContextReference() {rc=NULL;}
};

struct Extrems {
  float xmin,ymin,zmin;
  float xmax,ymax,zmax;
  void repair(float x,float y,float z);
  float diam();
  Extrems() {xmin=ymin=zmin=xmax=ymax=zmax=0;}
};

struct glopoint {
  float x,y;
};

struct gloProjectionCanvas {
  virtual void polygon(Array<glopoint> &pts)=0;
  virtual void polyline(Array<glopoint> &pts,bool closed)=0;
};

//toto jsou obecne tridy, ktere se nemusi vykreslovat zrovna nejrychleji
//rychle vykreslujuci se vyrobi volanim compile()
struct gloObject {
  virtual void render(RenderContext &rc) {}
  virtual ~gloObject() {}
  virtual void getextrems(Extrems &e) {}
  virtual void savetovrml(STROUT &fw,VRMLTextures *textures) {}
  virtual void drawprojection(gloProjectionCanvas &cnv,int dir,float sx,float sy,float tx,float ty) {}
  virtual void load(XMLNode xml,PersistentObject *archive,gloVariableList *vars) {}
  virtual gloBoundBox bound() {return gloBoundBox();}
  virtual gloObject *clone() {return NULL;}
};

gloObject *glo_load_object(XMLNode xml,PersistentObject *archive,gloVariableList *vars);

struct gloPolyObject:gloObject {
  IObjArray<gloObject> childs;
  void render(RenderContext &rc);
  void load(XMLNode xml,PersistentObject *archive,gloVariableList *vars);
  void getextrems(Extrems &e);
  void savetovrml(STROUT &fw,VRMLTextures *textures);
  void drawprojection(gloProjectionCanvas &cnv,int dir,float sx,float sy,float tx,float ty);
  gloBoundBox bound();
  gloObject *clone();
};

struct gloSettings:gloObject {
  PtrTo<gloObject> obj;
//  FaceMode fm;
//  gloSettings() {fm=fm_fill;}

  void load(XMLNode xml,PersistentObject *archive,gloVariableList *vars);
  void render(RenderContext &rc);
  void getextrems(Extrems &e) {obj->getextrems(e);}
  gloBoundBox bound() {if (obj) return obj->bound();return gloBoundBox();}
};

struct gloTransformation:gloObject {
  PtrTo<gloObject> obj;
  void render(RenderContext &rc);
  void load(XMLNode xml,PersistentObject *archive,gloVariableList *vars);
  virtual void transform(RenderContext &rc) {}
  void getextrems(Extrems &e) {if (obj) obj->getextrems(e);}
  void savetovrml(STROUT &fw,VRMLTextures *textures);
  virtual void savevrmltransforms(STROUT &fw) {}
};

struct gloInline:gloTransformation {
  float sx,sy,sz,tx,ty,tz;
  void transform(RenderContext &rc);
  void load(XMLNode xml,PersistentObject *archive,gloVariableList *vars);
  void savevrmltransforms(STROUT &fw);
  void getextrems(Extrems &e) {}
  gloObject *clone();
  gloBoundBox bound();
};

struct gloTranslate:gloTransformation {
  float x,y,z;
  gloTranslate() {x=y=z=0;}
  void transform(RenderContext &rc) {glTranslatef(x,y,z);}
  void load(XMLNode xml,PersistentObject *archive,gloVariableList *vars);
  gloBoundBox bound();
};

struct gloRotate:gloTransformation {
  PtrTo<gloFloatExpr> angle;
  float x,y,z;//vektor, podle ktereho se rotuje
//  float angle;//uhel ve stupnich
  gloRotate() {x=y=0;z=1;}
  void transform(RenderContext &rc);
  void load(XMLNode xml,PersistentObject *archive,gloVariableList *vars);
  gloBoundBox bound();
};

struct gloScale:gloTransformation {
  float x,y,z;
  gloScale() {x=y=z=0;}
  void transform(RenderContext &rc) {glScalef(x,y,z);}
  void load(XMLNode xml,PersistentObject *archive,gloVariableList *vars);
  gloBoundBox bound();
};

struct gloColor {
  virtual bool render(RenderContext &rc) {return false;}
  virtual gloColor *clone() {return NULL;}
};
gloColor *glo_loadcolor(XMLNode xml,gloVariableList *vars);

struct gloVarColor:gloColor {
  PtrTo<gloFloatExpr> r,g,b,a;
  gloVarColor(XMLNode xml);
  bool render(RenderContext &rc);
  bool defined(gloVariableList *vars) {return r->defined(vars) && g->defined(vars) && b->defined(vars) && a->defined(vars);}
};

struct gloColor4f:gloColor {
  float r,g,b,a;
  bool iscolor;//zda se barva nastavuje

  void load(XMLNode xml);
  bool render(RenderContext &rc);
  void init() {r=g=b=a=0;iscolor=false;}
  gloColor4f() {init();}
  gloColor4f(XMLNode xml) {init();load(xml);}
  gloColor *clone() {gloColor4f *res=new gloColor4f();res->r=r;res->g=g;res->b=b;res->iscolor=iscolor;return res;}
};

struct gloTexCoord2f {
  float s,t;

  void load(XMLNode xml) {s=xml.fvar("s");t=xml.fvar("t");}
  void render(RenderContext &rc) {glTexCoord2f(s,t);}
  gloTexCoord2f() {s=t=0;}
};

struct gloVertex3f {
  float x,y,z;

  void getextrems(Extrems &e) {e.repair(x,y,z);}
  void render(RenderContext &rc);
  void load(XMLNode xml);
  gloVertex3f() {x=y=z=0;}
};

struct gloVertex {
  gloVertex3f position;
  PtrTo<gloColor> color;
  gloTexCoord2f texcoord;

  void render(RenderContext &rc,bool hastexture);
  void load(XMLNode xml,gloVariableList *vars);
  void getextrems(Extrems &e) {position.getextrems(e);}
  gloVertex *clone();
  gloBoundBox bound();
};

struct TexApplyData {
  bool enabled,alphatest;
  TexApplyData() {enabled=alphatest=false;}
};

struct gloTexture {
  string name;
  bool transparent;
  RefBase<Texture> tex;
  bool isdefined() {return tex;}
  void load(XMLNode xml,PersistentObject *archive,gloVariableList *vars);
  void beforeapply(TexApplyData &data,RenderContext &rc);
  void afterapply(TexApplyData &data,RenderContext &rc);
  gloTexture() {transparent=false;}
};

/*
struct FaceMode {
  FaceMode() {mode=fm_fill;}
  void load(XMLNode xml);
  void begin();
  void end();
};
*/

struct gloMoreVerteces:gloObject {
  IObjArray<gloVertex> verteces;
  fpoint3d pos(RenderContext &rc,int index);
  void loadverteces(XMLNode xml,gloVariableList *vars);
  void renderverteces(RenderContext &rc,bool rendertex);
  void getextrems(Extrems &e);
  void drawprojection(gloProjectionCanvas &cnv,int dir,float sx,float sy,float tx,float ty);
  virtual void drawprojshape(Array<glopoint> pts,gloProjectionCanvas &cnv) {}
  void savevrmlverteces(STROUT &fw);
  gloBoundBox bound();
  void assign(gloMoreVerteces &src);
};

struct gloPolygon:gloMoreVerteces {
  PtrTo<gloColor> color;
  gloTexture texture;
  bool culling;
  bool denylight;
  float nx,ny,nz;

  void render(RenderContext &rc);
  gloPolygon() {culling=false;nx=ny=nz=0;}
  void load(XMLNode xml,PersistentObject *archive,gloVariableList *vars);
  void savetovrml(STROUT &fw,VRMLTextures *textures);
  void drawprojshape(Array<glopoint> pts,gloProjectionCanvas &cnv) {cnv.polygon(pts);}
  void savevrmltexcoords(STROUT &fw);
  gloObject *clone();
  void assign(gloPolygon &src);
private:
  void _savetovrml(STROUT &fw,VRMLTextures *textures);
};

struct gloPolyLine:gloMoreVerteces {
  PtrTo<gloColor> color;
  gloTexture texture;
  float linewi;
  bool closed;
  bool polygon;//rendered by polygons
  float frequency;//only if texture && polygon

  void render(RenderContext &rc);
  gloPolyLine() {linewi=1;closed=false;polygon=false;}
  void load(XMLNode xml,PersistentObject *archive,gloVariableList *vars);
  void drawprojshape(Array<glopoint> pts,gloProjectionCanvas &cnv) {cnv.polyline(pts,closed);}
  void savetovrml(STROUT &fw,VRMLTextures *textures);
  void assign(gloPolyLine &src);

  gloObject *clone();
};

struct gloModel:RCObject {
  PtrTo<gloObject> obj;
  bool disabled;//pokud se nepovede render, nastavi se disabled, aby se priste uz nezkouselo

  void load(XMLNode xml,PersistentObject *archive=NULL,gloVariableList *vars=NULL) {obj=glo_load_object(xml,archive,vars);disabled=false;}
  void render(RenderContext &rc);
  void getextrems(Extrems &e) {if (obj) obj->getextrems(e);}
  void translate(float dx,float dy,float dz);
  void scale(float fx,float fy,float fz);
  void savetovrml(sFile file);
  void drawprojection(gloProjectionCanvas &cnv,int dir,float sx,float sy,float tx,float ty);
  gloBoundBox bound() {if (obj) return obj->bound();return gloBoundBox();}
  gloModel(gloObject *aobj=NULL) {obj=aobj;disabled=false;} 
};

void glTexLine(RenderContext &rc,float t1,float t2,float linewi,fpoint3d &a,fpoint3d &b);

}

#endif
