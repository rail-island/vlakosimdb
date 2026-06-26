#include <jlib.h>
#pragma hdrstop

#include "awstable.h"
#include <jmath/analgeom.h>

namespace aws {

void Table::savedata(XMLNode xml) {
  if (!getroot(xml).isdeep()) return;
  XMLNode d=xml.getat(getroot(xml));
  d.clear();
  for(int i=0;i<cols.count;i++) {
    TableColItem &col=cols[i];
    XMLNode c=d.add("col");
    c.setvar("w",col.wi);
    c.setvar("id",col.id);
    c.setvar("visible",col.visible);
  }
}
int Table::findcol(STRING id) {
  for(int i=0;i<cols.count;i++) if (id==cols[i].id) return i;
  return -1;
}
void Table::showloadeddata(XMLPath root) {
  if (!root.isdeep()) return;
  XMLNode d=loadeddata;
  int dcnt=d.count();
  if (dcnt==cols.count) {
    for(int i=0;i<dcnt;i++) {
      XMLNode x=d[i];
      int actidx=findcol(x.svar("id"));
      if (actidx>=0 && actidx!=i) cols.exchange(actidx,i);
      cols[i].wi=x.ivar("w");
      cols[i].visible=x.ivar("visible");
    }
  }
  countcols();
  repainthdr();
}

void Table::loaddata(XMLNode xml) {
  if (getroot(xml).isdeep()) {
    XMLNode d=xml.getat(getroot(xml));
    loadeddata.assign(d);
    showloadeddata(getroot(xml));
  }
}

void Table::repaint() {
  PaintControl::repaint();
  content->repaint();
}
void Table::repaintline(int line) {
  content->repaintline(line);
}

int TableContent::getcellwi(int x) {
  return table->cols[table->vismap[x]].wi;// cellwi;
}
int TableContent::getcellleft(int x) {
  return table->cols[table->vismap[x]].left;
//  return x*cellwi;
}

void Table::onsize(int w,int h) {
  content->setpos(Point(leftwi+borderx,tophi+bordery),Size(w-leftwi-2*borderx,h-tophi-2*bordery));
}
void Table::update() {
  changed_model();
  content->repaint();
}
void TableContent::repaintline(int line) {
  repaint(Rect(0,line*cellhi,getsize().w,cellhi));
}

void Table::gotocell(Point cell) {
  content->repaint(content->getcellrect(actcellx,actcelly));
  actcellx=cell.x;
  actcelly=cell.y;
  content->repaint(content->getcellrect(actcellx,actcelly));
}

void TableContent::onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event) {
  ScrolledPainter::onmousedown(x,y,button,shift,event);
  if (button==mb_left) {
    Point mpt=mousetocell(Point(x,y));
    if (mpt.x>=0 && mpt.y>=0) {
      table->gotocell(mpt);
      table->cellclicked(table->cols[table->vismap[mpt.x]].id,mpt.y,shift);
      if (table->cols[table->vismap[mpt.x]].type==tct_check) {
        table->cellchecked(table->cols[table->vismap[mpt.x]].id,mpt.y,shift);
      }
    }
    table->setfocus();
  }
}

void Table::setselcol(string id) {

}
void Table::setselrow(int row) {
  gotocell(Point(actcellx,row));
  content->scrollinview(content->getcellrect(actcellx,actcelly));
}

Point TableContent::mousetocell(Point m) {
  Point res(-1,-1);
  for(int i=0;i<colcnt;i++) if (m.x>=getcellleft(i) && m.x<=getcellleft(i)+getcellwi(i)) res.x=i;
  res.y=m.y/cellhi;
  if (res.y>=rowcnt) res.y=-1;
  return res;
}

void Table::onmousedown(int x,int y,MouseButton button,ShiftState shift,awsEvent &event) {
  PaintControl::onmousedown(x,y,button,shift,event);
  mdownx=x;mdowny=y;smalldist=true;
  if (button==mb_left) {
    setfocus();
    if (y>=bordery && y<bordery+tophi) {
      x+=content->getscrollpos().x;
      for(int i=0;i<viscols;i++) {
        TableColItem &col=cols[vismap[i]];
        if (x>=col.left+col.wi-resizehdr && x<=col.left+col.wi+resizehdr) {
          resizingcol=i;return;
        }
      }
      for(int i=0;i<viscols;i++) {
        TableColItem &col=cols[vismap[i]];
        if (x>=col.left && x<col.left+col.wi) {
          movingcol=i;
          return;
        }
      }
    }
  }
}

void Table::onmouseup(int x,int y,MouseButton button,ShiftState shift,awsEvent &event) {
  PaintControl::onmouseup(x,y,button,shift,event);
  if (movingcol>=0 || resizingcol>=0) content->repaint();
  if (resizingcol>=0) content->setbounds(content->rowcnt,viscols);
  if (movingcol>=0 && smalldist) colclicked(cols[vismap[movingcol]].id);
  resizingcol=-1;movingcol=-1;
}
void Table::onmousemove(int x,int y,ShiftState shift,awsEvent &event) {
  PaintControl::onmousemove(x,y,shift,event);
  if (jmath::distance(XYREC(x,y),XYREC(mdownx,mdowny))>maxsmalldist) smalldist=false;
  if (resizingcol>=0) {
    x+=content->getscrollpos().x;
    TableColItem &col=cols[vismap[resizingcol]];
    col.wi=x-col.left;
    if (col.wi<mincolsize) col.wi=mincolsize;
    countcols();
    repainthdr();
  } else if (movingcol>=0) {
    x+=content->getscrollpos().x;
    TableColItem &col=cols[vismap[movingcol]];
    if (movingcol>0 && x<col.left && x<cols[vismap[movingcol-1]].left+col.wi) {
      cols.exchange(vismap[movingcol],vismap[movingcol-1]);
      movingcol--;
      countcols();
      repainthdr();
      return;
    }
    if (movingcol<viscols-1 && x>col.right() && x>cols[vismap[movingcol+1]].right()-col.wi) {
      cols.exchange(vismap[movingcol],vismap[movingcol+1]);
      movingcol++;
      countcols();
      repainthdr();
      return;
    }
  }
}

void Table::onmousewheel(int x,int y,int delta,ShiftState shift,awsEvent &event) {
  Point pt=content->getscrollpos();
  content->setscrollpos(pt+Point(0,-delta));
}

void Table::onkeydown(awsKey key,awsEvent &event) {
  Rect cell2=content->getcellrect(actcellx,actcelly);
  bool pnt=false;
  if (key==kt_down) actcelly++,pnt=true;
  if (key==kt_up) actcelly--,pnt=true;
  if (key==kt_right) actcellx++,pnt=true;
  if (key==kt_left) actcellx--,pnt=true;
  if (key==kt_pgup) actcelly-=content->getsize().h/content->cellhi,actcelly++,pnt=true;
  if (key==kt_pgdn) actcelly+=content->getsize().h/content->cellhi,actcelly--,pnt=true;
  if (key==kt_home) actcellx=0,pnt=true;
  if (key==kt_end) actcellx=content->colcnt-1,pnt=true;
  if (key==kt_space) {
    if (cols[vismap[actcellx]].type==tct_check) {
      cellchecked(cols[vismap[actcellx]].id,actcelly,ShiftState());
      pnt=true;
    }
  }
  if (key(kt_pgup,ks_ctrl)) actcelly=0,pnt=true;
  if (key(kt_pgdn,ks_ctrl)) actcelly=content->rowcnt-1,pnt=true;
  if (pnt) {
    if (actcelly>=content->rowcnt) actcelly=content->rowcnt-1;
    if (actcellx>=content->colcnt) actcellx=content->colcnt-1;
    if (actcellx<0) actcellx=0;
    if (actcelly<0) actcelly=0;

    Rect cellact=content->getcellrect(actcellx,actcelly);
    content->scrollinview(cellact);
    content->repaint(cell2),content->repaint(cellact);
  }
  cellclicked(cols[vismap[actcellx]].id,actcelly,ShiftState());
  PaintControl::onkeydown(key,event);
}

void TableContent::onpaint(Canvas &cnv,awsEvent &event) {
  Rect clip=cnv.clip();
  TableModel *model=table->getmodel();
  int xs=colcnt,ys=rowcnt;
  int actx=0,acty=0;
//  cnv.bg(table->bgcolor).fg(clnnone).rect(clip.a.x-1,clip.a.y-1,clip.b.x+1,clip.b.y+1).fg(clnblack);
  cnv.bg(table->bgcolor).fg(clnnone).rect(clip.a.x,clip.a.y,clip.b.x+1,clip.b.y+1).fg(clnblack);

  int ybeg=clip.a.y/cellhi;
  int yend=clip.b.y/cellhi+1;

//  ybeg=-1,yend=0xFFFF;

  if (ybeg<0) ybeg=0;
  if (yend>=ys) yend=ys-1;

  for(int y=ybeg;y<=yend;y++) {
    int acty=y*cellhi;
    actx=0;
//    table->drawingline=y;
//    table->linecoloranswer=clnnone;
    int color=table->getlinecolor(y);
    for(int x=0;x<xs;x++) {
      if (color!=clnnone) {
        cnv.bg(color);
        cnv.fg(clnnone).rect(actx+cbx,acty+cby,actx+getcellwi(x),acty+cellhi).fg(clnblack);
      }
      if (x==table->actcellx && y==table->actcelly) {
//        cnv.bg(table->selbg).fg(clnnone).
//        rect(actx+cbx,acty+cby,actx+getcellwi(x),acty+cellhi).fg(clnblack);
        cnv.bg(table->selbg).penwi(2).
        rect(actx+cbx,acty+cby,actx+getcellwi(x),acty+cellhi).fg(clnblack).penwi(0);
      }
      string text=model->getcelltext(table->cols[table->vismap[x]].id,y);
      TableColType type=table->cols[table->vismap[x]].type;
      if (type==tct_text) cnv.centertext(Rect(actx+cellspacing,acty,table->cols[table->vismap[x]].wi,cellhi),text,false,true,true);
      if (type==tct_check) cnv.drawcheck(actx+cellspacing+8,acty+cellhi/2,text.ivalue());
      actx+=getcellwi(x);
    }
  }
  actx=0;
  for(int i=0;i<=xs;i++) {
    if (actx>=clip.a.x && actx<=clip.b.x) cnv.line(actx,clip.a.y,actx,min(cellhi*ys,clip.b.y));
    if (i<xs) actx+=getcellwi(i);
  }
  acty=0;
  for(int i=0;i<=ys;i++) {
    if (acty>=clip.a.y && acty<=clip.b.y) cnv.line(clip.a.x,acty,min(actx,clip.b.x),acty);
    if (i<ys) acty+=cellhi;
  }
}

Table::Table(Component *owner,System *system):PaintControl(owner,system) {
  loadeddata=XMLNode::create();
  content=new TableContent(this,system,this);
  mdownx=0;mdowny=0;
//  scr=new ScrollBox(this,system);
//  scr->setsdp("flat","1");
//  content->setparent(this);
  content->setparent(this);
//  scr->setparent(this);
  tophi=20;leftwi=0;
  borderx=bordery=2;
  resizehdr=2;
  mincolsize=20;
  maxsmalldist=3;
  resizingcol=-1;
  movingcol=-1;
  bgcolor=clnwhite;
  selbg=clnaqua;
  actcellx=actcelly=0;
  content->setpos(borderx,bordery);
  content->addhandler(this);
}

void Table::on_notify(NotifyEvent &ev) {
  if (ev(content,"onhscroll")) repainthdr();
}

void Table::countcols() {
  colswi=0;viscols=0;
  for(int i=0;i<cols.count;i++) if (cols[i].visible) cols[i].left=colswi,colswi+=cols[i].wi,vismap[viscols++]=i;
}

void Table::onpaint(Canvas &cnv,awsEvent &event) {
  Rect r=Rect(Point(0,0),getsize());
  cnv.bg(clnltgray).rect(r);
  cnv.drawframe(r,true);
  int dx=content->getscrollpos().x;
  for(int i=0;i<cols.count;i++) if (cols[i].visible) {
    Rect r=Rect(leftwi+cols[i].left+borderx-dx,bordery,cols[i].wi,tophi);
    r.a.y+=2;
    cnv.drawframe(r,false);
    cnv.centertext(r,cols[i].text);
  }
}

void TableContent::setbounds(int rows,int cols) {
  rowcnt=rows;colcnt=cols;
  setinsize(Size(table->colswi+cbx,cellhi*rowcnt+cby));
  Size s=getsize();
  int x=0;
}

void Table::colclicked(STRING id) {
  ColClickEvent ev(id);donotify(ev,"oncolclick",ev_colclick.get());
}

int Table::getlinecolor(int line) {
  GetLineColorEvent ev(line);donotify(ev,"ongetlinecolor",ev_getlinecolor.get());return ev.color;
}

void Table::cellclicked(STRING id,int line,ShiftState shift) {
  CellClickEvent ev(id,line,shift);
  donotify(ev,"oncellclick",ev_cellclick.get());
}

void Table::cellchecked(STRING id,int line,ShiftState shift) {
  CellClickEvent ev(id,line,shift);donotify(ev,"oncellcheck",ev_cellcheck.get());
}


void Table::loadprops(XMLNode xml,Form *form) {
  PaintControl::loadprops(xml,form);
  ev_colclick.load(xml,"oncolclick");
  ev_cellclick.load(xml,"oncellclick");
  ev_getlinecolor.load(xml,"ongetlinecolor");
  ev_cellcheck.load(xml,"oncellcheck");
}

void Table::changed_model() {
  TableModel *model=getmodel();
  if (!model) {content->setbounds(0,0);return;}

  TCntVec<TableCol> newcols;
  model->getcols(newcols);

  for(int i=0;i<cols.count;) {
    int idx=newcols.vsearchi(cols[i].id);
    if (idx>=0) {
      strcpy(cols[i].text,newcols[idx].text);
      newcols.erase(idx);
      i++;
    } else cols.erase(i);
  }
  for(int i=0;i<newcols.count;i++) {
    cols.add(TableColItem(newcols[i],50));
  }
  showloadeddata(getroot());
  countcols();

  content->setbounds(model->getrowcount(),viscols);
}

rtti_define(ColClickEvent)
rtti_define(CellClickEvent)
rtti_define(GetLineColorEvent)

void init_aws_table_rtti() {
  using namespace rtti;
  rtti_impl(ColClickEvent)
  rtti_impl(CellClickEvent)
  rtti_impl(GetLineColorEvent)

  ColClickEventClass->init_rec<ColClickEvent>("ColClickEvent");
  CellClickEventClass->init_rec<ColClickEvent>("CellClickEvent");
  GetLineColorEventClass->init_rec<GetLineColorEvent>("GetLineColorEvent");

  rtti_array_fld(ColClickEvent,id,Char);

  rtti_array_fld(CellClickEvent,id,Char);
  rtti_def_fld(CellClickEvent,line,Int);
  rtti_def_fld(CellClickEvent,shift,ShiftState);

  rtti_def_fld(GetLineColorEvent,line,Int);
  rtti_def_fld(GetLineColorEvent,color,Color);
  GetLineColorEventClass->setattr("color",1);
}

}

