#include <jlib.h>
#pragma hdrstop

#include "awshtmlview.h"

namespace aws {

const char typical_styles[]="\
<?xml version='1.0' encoding='Windows-1250'?>\
<styles>\
<basestyle font.name='Times New Roman' font.size='12'/>\
<style name='h1' wrap='1' font.size='24' font.bold='1'/>\
<style name='h2' wrap='1' font.size='20' font.bold='1'/>\
<style name='h3' wrap='1' font.size='16' font.bold='1'/>\
<style name='h4' wrap='1' font.size='14' font.bold='1'/>\
<style name='h5' wrap='1' font.size='12' font.bold='1'/>\
<style name='h6' wrap='1' font.size='10' font.bold='1'/>\
<style name='b' font.bold='1'/>\
<style name='i' font.italic='1'/>\
<style name='u' font.underline='1'/>\
<style name='tt' font.name='Courier New'/>\
<style name='a' font.color='blue' font.underline='1' link='href'/>\
</styles>\
";

rtti_define(FindPictureEvent)
rtti_define(LinkEvent)

void init_aws_html_rtti() {
  using namespace rtti;
  rtti_impl(FindPictureEvent)
  rtti_impl(LinkEvent)

  FindPictureEventClass->init_rec<FindPictureEvent>("FindPictureEvent");
  LinkEventClass->init_rec<LinkEvent>("LinkEvent");

  rtti_array_fld(FindPictureEvent,src,Char);
  rtti_array_fld(LinkEvent,href,Char);
}

Picture *HTMLViewMain::findpic(string src) {
  return view->findpic(src);
}

static void decodehtmlentity(gram::Analyser &act,STROUT &value) {
  if (act.begjmp("&gt;")) value<<'>';
  else if (act.begjmp("&lt;")) value<<'<';
  else if (act.begjmp("&quot;")) value<<'"';
  else if (act.begjmp("&apos;")) value<<'\'';
  else if (act.begjmp("&amp;")) value<<'&';
  else value<<*act,act++;
}
string htmldecode(string htmltext) {
  gram::Analyser a(htmltext.c_str());
  MEMSTROUT res;
  while (*a) {
    if (*a=='&') decodehtmlentity(a,res);else res<<*a,a++;
  }
  return res.str();
}

void HTMLStyle::load(XMLNode xml) {
  if (xml.isvar("font.bold")) bold=xml.ivar("font.bold");
  if (xml.isvar("font.italic")) italic=xml.ivar("font.italic");
  if (xml.isvar("font.underline")) underline=xml.ivar("font.underline");
  if (xml.isvar("font.strikeout")) strikeout=xml.ivar("font.strikeout");
  if (xml.isvar("font.size")) fontsize=xml.ivar("font.size");
  if (xml.isvar("font.color")) color=gr::strtonativecolor(xml.cvar("font.color")),iscolor=true;
  link=xml.svar("link");
  fontname=xml.svar("font.name");
  name=xml.svar("name");
  wrap=xml.ivar("wrap");
}

void HTMLStyle::updatefont(Font *font) {
  if (bold>=0) font->setbold(bold);
  if (italic>=0) font->setitalic(italic);
  if (underline>=0) font->setunderline(underline);
  if (strikeout>=0) font->setstrikeout(strikeout);
  if (fontsize>=0) font->setsize(fontsize);
  if (fontname!="") font->setname(fontname);
  if (iscolor) font->setcolor(color);
}

HTMLCanvas::HTMLCanvas(SPCanvas dest,Canvas draw,int wi,System *system,HTMLData *main):draw(draw),wi(wi),system(system),main(main),dest(dest) {
  maxhi;
  x=y=10;
  x0=10;
  center=0;
  beginline=true;
  Font *fnt=system->createfont();main->getbasestyle()->updatefont(fnt);
  setfont(fnt);nullcnt=0;
}

void HTMLViewMain::loadstyles(XMLNode xml) {
  basestyle.load(xml["basestyle"]);
  for(int i=0;i<xml.count();i++) if (xml[i].getname()=="style") {
    HTMLStyle *style=new HTMLStyle();
    style->load(xml[i]);
    styles.add(style);
  }
}

void HTMLPoly::repaint(HTMLCanvas &cnv) {
  viewrect=Rect(cnv.getpt(),cnv.getpt());
  for(int i=0;i<nodes.count;i++) {
    nodes[i]->draw(cnv);
    viewrect+=nodes[i]->viewrect;
  }
}

void HTMLFmt::parse(gram::Analyser &a) {
  parsevars(a,vars);
  if (*a=='>') a++;
  poly.parse(a);
}

void HTMLCanvas::nullbegin() {
  nullcnt++;
}
void HTMLCanvas::nullend() {
  nullcnt--;
}

void HTMLNode::draw(HTMLCanvas &cnv) {
  repaint(cnv);
/*  cnv.nullbegin();
  HTMLCanvasState state=cnv.getstate();
  repaint(cnv);
  cnv.setstate(state);
  cnv.nullend();
  if (!cnv.isnull()) {
    repaint(cnv);
  }*/
}

string HTMLList::gethref(int x,int y) {
  for(int i=0;i<items.count;i++) {
    string res=items[i]->gethref(x,y);
    if (res!="") return res;
  }
  return "";
}

string HTMLPoly::gethref(int x,int y) {
  for(int i=0;i<nodes.count;i++) {
    string res=nodes[i]->gethref(x,y);
    if (res!="") return res;
  }
  return "";
}

string HTMLFmt::gethref(int x,int y) {
  if (!viewrect.contain(Point(x,y))) return "";
  if (style && style->link!="") {
    string href=vars.svar(style->link);
    if (href!="") return href;
  }
  return poly.gethref(x,y);
}

void HTMLFmt::repaint(HTMLCanvas &cnv) {
  Font *fnt=cnv.clonefont();
  if (style) {
    style->updatefont(fnt);
    cnv.pushfont();
    cnv.setfont(fnt);
    if (style->wrapbegin()) cnv.wrap();
    poly.draw(cnv);
    if (style->wrapend()) cnv.wrap();
    cnv.popfont();
  } else poly.draw(cnv);
  viewrect=poly.viewrect;
}
/*
void HTMLHeading::draw(HTMLCanvas &cnv) {
  cnv.pushfont();
  cnv.heading(type);
  cnv.wrap();
  cnv.drawword(word);
  cnv.wrap();
  cnv.popfont();
}
*/

void HTMLNode::parsevars(gram::Analyser &a,VarList &vars) {
  while(a.jsp(),*a && *a!='>') {
    bstring name;
    while (a.is_xmlidf()) name<<*a,a++;
    a.jsp();
    string value;
    if (*a=='=') {
      a++;a.jsp();
      if (*a=='"') {a++;value=a.readcstring('"');}
      else if (*a=='\'') {a++;value=a.readcstring('\'');}
      else if (!a.is_white()) {
        bstring bval;
        while (!a.is_white()) bval<<*a,a++;
        value=bval;
      } else {
        jumptag(a);
        return;
      }
    } else value="1";
    vars.setvar(name,htmldecode(value));
  }
}

void HTMLCanvas::listmark() {
  int ymid=dest->gettexthi("A")/2;
  int rad=3;
  line.setfgcolor(clnblack);
  line.drawellipse(x-8-rad,-ymid-rad,x-8+rad,-ymid+rad);
}

void HTMLCanvas::hline() {
  draw.fg(clnblack);
  draw.line(10,y+10,wi-10,y+10);
  y+=20;
}

void HTMLList::repaint(HTMLCanvas &cnv) {
  viewrect=Rect(cnv.getpt(),cnv.getpt());
  int x0=cnv.getx0();
  cnv.setx0(x0+16);
  cnv.wrap();
  for(int i=0;i<items.count;i++) {
    cnv.listmark();
    items[i]->draw(cnv);
    viewrect+=items[i]->viewrect;
    cnv.wrap();
  }
  cnv.setx0(x0);
  cnv.wrap();
}

void HTMLList::parse(gram::Analyser &a) {
  jumptag(a);
  HTMLPoly *item=NULL;
  a.jw();
  string end;
  while (*a) {
    if (a.begins("<li")) {
      if (item) items.add(item);
      item=new HTMLPoly(main);
      a++;jumptag(a);
    } else if (end=="ul") {
      if (item) items.add(item);
      return;
    } else {
      if (!item) item=new HTMLPoly(main);
      end=item->parse(a);
    }
  }
/*  while (*a && !a.begins("<li") && !a.begins("</ul")) a++;
  if (a.begjmp("</ul>")) return;
  if (!*a) return;
  if (a.begins("<li"))
  }
*/
}

void HTMLImage::parse(gram::Analyser &a) {
  VarList vars;
  parsevars(a,vars);
  string src=vars.svar("src");
  if (src!="") image=main->findpic(src);
  jumptag(a);
}

string HTMLPoly::parse(gram::Analyser &a) {
  nodes.clear();
  while (*a) {
    if (a.is_white()) {
      a.jw();addspace();continue;
    }
    if (*a=='<') {
      a++;
      if (*a=='/') {//ukoncujici tag
        a++;
        string etag=a.readidf();
        jumptag(a);
        if (etag=="li" || etag=="td" || etag=="tr") continue;
        if (etag=="center") {
          addnode(new HTMLCenterOff(main));
          continue;
        }
        return etag;
      }
      string tag=a.readidf();
      if (tag=="br") {
        addnode(new HTMLWrap(main));
        jumptag(a);
      } else if (tag=="center") {
        addnode(new HTMLCenterOn(main));
        jumptag(a);
      } else if (tag=="img") {
        HTMLImage *img=new HTMLImage(main);
        img->parse(a);
        addnode(img);
      } else if (tag=="hr") {
        addnode(new HTMLLine(main));
        jumptag(a);
      } else if (tag=="ul") {
        HTMLList *list=new HTMLList(main);
        list->parse(a);
        addnode(list);
      } else if (tag=="li" || tag=="td") {
        while (*a!='<') a--;
        return "";
      } else {
        HTMLFmt *fmt=new HTMLFmt(tag,main);
        fmt->parse(a);
        addnode(fmt);
      }
/*      if (a.begjmp("br>") || a.begjmp("br/>")) {
        addnode(new HTMLWrap());
      } else if (a.begjmp("h1>")) {
        string txt=parseto(a,"</h1>");
        addnode(new HTMLHeading(1,txt));
      } else {
        while (*a && *a!='>') a++;
      }*/
    } else {
      bstring wrd;
      while (*a && *a!='<' && !a.is_white()) {
        wrd<<*a;a++;
      }
      addnode(new HTMLWord(htmldecode(wrd),main));
    }
  }
  return "";
}

Rect HTMLCanvas::drawspace() {
  Rect res;
  res.a.x=x;res.a.y=y;res.b.y=y+maxhi;
  if (!beginline) {
    line.drawtext(x,-maxhi," ");
    x+=dest->gettextwi(" ");
  }
  res.b.x=x;
  return res;
}

void HTMLCanvas::wrap() {
  int drawx=0;
  if (iscenter()) drawx=(wi-x)/2;
  line.draw(Canvas(draw,drawx,y+maxhi));
  line.clear();x=x0;y+=maxhi;
  maxhi=0;beginline=true;
}
/*
void HTMLCanvas::heading(int type) {
  font=system->createfont();
  switch (type) {
    case 1:font->setsize(24);break;
  }
  cnv.font(font);
}
*/

Rect HTMLCanvas::drawword(string word) {
  int w=dest->gettextwi(word);
  if (x+w>wi) wrap();
  Rect res;
  res.a.x=x;res.a.y=y;
  int hi=dest->gettexthi(word);
  if (!nullcnt) line.drawtext(x,-hi,word);
  x+=w;
  if (hi>maxhi) maxhi=hi;
  beginline=false;
  res.b.x=x;res.b.y=y+maxhi;
  return res;
}

Rect HTMLCanvas::drawimg(Picture *img) {
  int w=img->getw();
  if (x+w>wi) wrap();
  Rect res;
  res.a.x=x;res.a.y=y;
  int hi=img->geth();
  if (!nullcnt) Canvas(&line,0,0).picture(x,-hi,img);
  x+=w;
  if (hi>maxhi) maxhi=hi;
  beginline=false;
  res.b.x=x;res.b.y=y+maxhi;
  return res;
}

HTMLView::HTMLView(Component *owner,System *system):ScrolledPainter(owner,system),html(&main),main(this) {
  painted=false;fmtw=0;//setdoublebuffer(2);
  XMLNode xml=XMLNode::create();
  xml.loadfromtext(typical_styles);
  main.loadstyles(xml);
}

void HTMLView::onsize(int w,int h) {
  if (w!=fmtw) updated();
  ScrolledPainter::onsize(w,h);
}

void HTMLView::onpaint(Canvas &cnv,awsEvent &event) {
  cnv.bg(clnwhite).fg(clnnone).rect(cnv.clip());
  if (!painted) {
    int hi=format(cnv);
    setinsize(Size(fmtw-32,hi));
  }
  meta.draw(cnv);
}

int HTMLView::format(Canvas &cnv) {
  fmtw=getsize().w;
  painted=true;

  meta.clear();

  HTMLCanvas hc(cnv.cnv,Canvas(&meta,0,0),fmtw-32,system,&main);
  html.draw(hc);

  return hc.gethi();
}

void HTMLView::onmousemove(int x,int y,ShiftState shift,awsEvent &event) {
  ScrolledPainter::onmousemove(x,y,shift,event);
  string href=html.gethref(x,y);
//  if (href!="") changecursor(cr_wait);else changecursor(cr_default);
  if (href!="") setcursor(cr_hand);
  else setcursor(cr_default);
}

void HTMLView::onlink(string href) {
  LinkEvent le(href);
  donotify(le,"onlink",ev_link.get());
}

Picture *HTMLView::findpic(string src) {
  if (Picture *res=searchpicture(src)) return res;
  FindPictureEvent fe(src);
  donotify(fe,"onfindpicture",ev_findpicture.get());
  return searchpicture(src);
}

void HTMLView::loadprops(XMLNode xml,Form *form) {
  ScrolledPainter::loadprops(xml,form);
  ev_findpicture.load(xml,"onfindpicture");
  ev_link.load(xml,"onlink");
}

void HTMLView::onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event) {
  ScrolledPainter::onmousedown(x,y,button,shift,event);
  string href=html.gethref(x,y);
  if (href!="") onlink(href);
//  system->settitle(href);
//  setcursor(cr_hand);
//  if (href!="") jsystem->showinfo(href);
}


}


