#include <jlib.h>
#pragma hdrstop

#include "gparticle.h"

namespace jtyc {

struct TexIdRec {
  void *ctxid;
  unsigned int texid;
  bool match(void *c) {return c==ctxid;}
};

static TCntVec<TexIdRec> texids;

//static unsigned int texid;

const float minlife=0.05;

static unsigned int counttexture() {
  int texWidth = 256;
  int texHeight = 256;
  GLubyte *texPixels, *p;
  int texSize;

  unsigned int texid=0;
  glGenTextures(1,&texid);
  if (!texid) throw JGLError("setCheckTexture()");
  glBindTexture(GL_TEXTURE_2D,texid);

  texSize = texWidth*texHeight*4*sizeof(GLubyte);
  texPixels = (GLubyte *) malloc(texSize);
  if (texPixels == NULL) {
    return 0;
  }

  p = texPixels;
  for (int i=0; i<texHeight;i++) {
    for (int j=0; j<texWidth;j++) {
      GLuint dist = hypot(float(i - (texHeight / 2)),float(j - (texWidth / 2)));
      float color = 255-(dist*1.8);
      if (color < 0) color = 0;
      p[0] = color;
      p[1] = color;
      p[2] = color;
      p[3] = color;
      p+=4;
    }
  }

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texPixels);

  free(texPixels);

  return texid;
}

static unsigned int gettexture(void *ctxid) {
  TexIdRec *res=texids.vsearchv(ctxid);
  if (!res) {
    unsigned int texid=counttexture();
    TexIdRec &a=texids.add();
    a.ctxid=ctxid;
    a.texid=texid;
    return texid;
  }
  return res->texid;
}


void ParticleEngine::render(RenderContext &rc) {
//  bool blend=glIsEnabled(GL_BLEND);
//  bool istex=glIsEnabled(GL_TEXTURE_2D);
//  return;
//  glPushAttrib(GL_BLEND|GL_TEXTURE_2D);
  rc.push();
  rc.setblending(true);
  rc.settexture2d(true);
//  glPushAttrib(GL_ALL_ATTRIB_BITS);
  rc.setlighting(false);
//  glDisable(GL_LIGHTING);
  rc.setculling(false);
  glColor4f(1,1,1,1);
//  glEnable(GL_BLEND);
//  glShadeModel(GL_SMOOTH);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  unsigned int texid=gettexture(rc.ctxid);
//  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,texid);

  for(int loop=0;loop<particle.count;loop++) {
    if (particle[loop].life<minlife) continue;
    float x=particle[loop].x; // Grab Our Particle X Position
    float y=particle[loop].y; // Grab Our Particle Y Position
    float z=particle[loop].z; // Grab Our Particle Z Position

//    glColor4f(0.5f,0.5f,1.f,particle[loop].life);

    float lf=1-particle[loop].life;
    glColor4f(
      data.start_r+lf*(data.end_r-data.start_r),
      data.start_g+lf*(data.end_g-data.start_g),
      data.start_b+lf*(data.end_b-data.start_b),
      data.start_a+lf*(data.end_a-data.start_a)
    );


    glBegin(GL_TRIANGLE_STRIP); // Build Quad From A Triangle Strip
    glTexCoord2f(1,1); glVertex3f(x+data.elem_size,y+data.elem_size,z); // Top Right
    glTexCoord2f(0,1); glVertex3f(x-data.elem_size,y+data.elem_size,z); // Bottom Right
    glTexCoord2f(1,0); glVertex3f(x+data.elem_size,y-data.elem_size,z); // Top Left
    glTexCoord2f(0,0); glVertex3f(x-data.elem_size,y-data.elem_size,z); // Bottom Left
    glEnd(); // Done Building Triangle Strip

    glBegin(GL_TRIANGLE_STRIP); // Build Quad From A Triangle Strip
    glTexCoord2f(1,1); glVertex3f(x,y+data.elem_size,z+data.elem_size); // Top Right
    glTexCoord2f(0,1); glVertex3f(x,y+data.elem_size,z-data.elem_size); // Bottom Right
    glTexCoord2f(1,0); glVertex3f(x,y-data.elem_size,z+data.elem_size); // Top Left
    glTexCoord2f(0,0); glVertex3f(x,y-data.elem_size,z-data.elem_size); // Bottom Left
    glEnd(); // Done Building Triangle Strip

    glBegin(GL_TRIANGLE_STRIP); // Build Quad From A Triangle Strip
    glTexCoord2f(1,1); glVertex3f(x+data.elem_size,y,z+data.elem_size); // Top Right
    glTexCoord2f(0,1); glVertex3f(x+data.elem_size,y,z-data.elem_size); // Bottom Right
    glTexCoord2f(1,0); glVertex3f(x-data.elem_size,y,z+data.elem_size); // Top Left
    glTexCoord2f(0,0); glVertex3f(x-data.elem_size,y,z-data.elem_size); // Bottom Left
    glEnd(); // Done Building Triangle Strip

  }
//  rc.lighting=l;
//  if (!blend) glDisable(GL_BLEND);
//  if (!istex) glDisable(GL_TEXTURE_2D);
//  rc.setlighting(l);
//  glPopAttrib();
//  glColor4f(1,1,1,1);
  rc.pop();
}

void ParticleEngine::iterate(double deltatime) {
  float vk=pow(data.slow_down,deltatime);
  for(int loop=0;loop<particle.count;loop++) {
    particle[loop].x+=particle[loop].xi*deltatime;  // Move On The X Axis By X Speed
    particle[loop].y+=particle[loop].yi*deltatime+data.rise_const*deltatime;  // Move On The Y Axis By Y Speed
    particle[loop].z+=particle[loop].zi*deltatime;  // Move On The Z Axis By Z Speed

    // Slow down the particles
    particle[loop].xi*=vk;
    particle[loop].yi*=vk;
    particle[loop].zi*=vk;

    particle[loop].life-=particle[loop].fade*deltatime; // Reduce Particles Life By 'Fade'

    if (data.auto_alive && particle[loop].life<minlife) particle[loop].init(*this);
  }
}

void ParticleItem::init(ParticleEngine &eng) {
//  x=eng.x0+eng.data.x0;
//  y=eng.y0+eng.data.y0;
//  z=eng.z0+eng.data.z0;

  x=eng.x0;
  y=eng.y0;
  z=eng.z0;

  life=1.0f; // Give It New Life
  fade=eng.data.fade_min+frand()*(eng.data.fade_max-eng.data.fade_min);
//  fade=float(rand()%100)/10000 + 0.005f; // Random Fade Value
/*
  float V = (float((rand()%9))+1);
  float Angle = float(rand()%360);

  xi = sin(Angle) * V;
  yi = cos(Angle) * V;
  zi = ((rand()%10)-5)/5;
*/
  xi=eng.vx+eng.data.vx_min+frand()*(eng.data.vx_max-eng.data.vx_min);
  yi=eng.vy+eng.data.vy_min+frand()*(eng.data.vy_max-eng.data.vy_min);
  zi=eng.vz+eng.data.vz_min+frand()*(eng.data.vz_max-eng.data.vz_min);
}

void ParticleEngine::translate(float dx,float dy,float dz) {
  for(int i=0;i<particle.count;i++) particle[i].x+=dx,particle[i].y+=dy,particle[i].z+=dz;
}

void ParticleEngine::start() {
  if (data.count!=particle.count) {
    particle.wantcapa(data.count);
    particle.count=data.count;
  }
  for(int i=0;i<particle.count;i++) particle[i].init(*this);
}

static int life_cmp(const void *p_a,const void *p_b) {
  ParticleItem *a=(ParticleItem*)p_a,*b=(ParticleItem*)p_b;
  float res=b->life-a->life;
  if (res>0) return 1;
  if (res<0) return -1;
  return 0;
}

void ParticleEngine::impuls() {
  qsort(particle.buf,particle.count,sizeof(ParticleItem),life_cmp);
  for(int i=(1-data.impuls_rate)*particle.count;i<particle.count;i++) particle[i].init(*this);
//  for(int i=0;i<particle.count;i++) if (particle[i].life<minlife) particle[i].init(*this);
}

void ParticleData::load(XMLNode xml) {
  start_r=xml.fvar("start_r");
  start_g=xml.fvar("start_g");
  start_b=xml.fvar("start_b");
  if (xml.svar("start_a")!="") start_a=xml.fvar("start_a");
  end_r=xml.fvar("end_r");
  end_g=xml.fvar("end_g");
  end_b=xml.fvar("end_b");
  end_a=xml.fvar("end_a");
  vx_min=xml.fvar("vx_min");
  vx_max=xml.fvar("vx_max");
  vy_min=xml.fvar("vy_min");
  vy_max=xml.fvar("vy_max");
  vz_min=xml.fvar("vz_min");
  vz_max=xml.fvar("vz_max");
//  x0=xml.fvar("x0");
//  y0=xml.fvar("y0");
//  z0=xml.fvar("z0");
  elem_size=xml.fvar("elem_size");
  count=xml.ivar("count");
  slow_down=xml.fvar("slow_down");
  fade_min=xml.fvar("fade_min");
  fade_max=xml.fvar("fade_max");
  auto_alive=xml.ivar("auto_alive");
  interval=xml.fvar("interval");
  impuls_rate=xml.fvar("impuls_rate");
  rise_const=xml.fvar("rise_const");
}

ParticleData::ParticleData() {
  start_r=1;
  start_g=1;
  start_b=1;
  start_a=1;
  end_r=1;
  end_g=1;
  end_b=1;
  end_a=0;
  vx_min=-1;
  vx_max=1;
  vy_min=-1;
  vy_max=1;
  vz_min=-1;
  vz_max=1;
//  x0=0;
//  y0=0;
//  z0=0;
  elem_size=0.2;
  count=100;
  slow_down=0.5;
  fade_min=0.1;
  fade_max=0.3;
  impuls_rate=0.4;
  rise_const=0;
  auto_alive=true;
}



}


