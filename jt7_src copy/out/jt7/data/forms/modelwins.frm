<?xml version="1.0" encoding="Windows-1250"?>
<data>
<window clienth="300" clientw="250" closeonesc="1" name="window1" onclose="" onok="" position="center" rep.name="model.colorizefrm" state="normal" text="$Texture">
<edit focused="1" h="25" name="edit1" onchange="" root="@texx1" text="Edit" w="100" x="50" y="30"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@texy1" text="Edit" w="100" x="50" y="70"/>
<edit focused="1" h="25" name="edit3" onchange="" root="@texx2" text="Edit" w="100" x="50" y="110"/>
<edit focused="1" h="25" name="edit4" onchange="" root="@texy2" text="Edit" w="100" x="50" y="150"/>
<label h="25" name="label1" root="" text="X1" w="30" x="10" y="30"/>
<label h="25" name="label2" root="" text="Y1" w="30" x="10" y="70"/>
<label h="25" name="label3" root="" text="X2" w="30" x="10" y="110"/>
<label h="25" name="label4" root="" text="Y2" w="30" x="10" y="150"/>
<button focused="" h="25" name="button1" onclick="@ok" text="$OK" w="100" x="10" y="260"/>
<button focused="" h="25" name="button2" onclick="@cancel" text="$Cancel" w="100" x="120" y="260"/>
<label h="25" name="label5" root="" text="$Texture" w="40" x="10" y="190"/>
<edit focused="1" h="25" hint="" name="edit5" onchange="" root="@texture" text="Edit" w="180" x="50" y="190"/>
<checkbox checked="0" focused="1" h="15" name="checkbox1" onclick="" root="@transparent" text="$Transparency" w="100" x="20" y="230"/>
</window>
<window clienth="260" clientw="500" closeonesc="1" name="window2" onclose="" onok="" position="center" rep.name="model.inssimplemodelform" state="normal" text="$Simple-model">
<label h="25" name="label1" root="" text="$Length" w="30" x="20" y="50"/>
<label h="25" name="label2" root="" text="$Width" w="30" x="20" y="80"/>
<label h="25" name="label3" root="" text="$Height" w="30" x="20" y="110"/>
<edit focused="1" h="25" name="edit1" onchange="" root="@xsize" text="" w="100" x="60" y="50"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@zsize" text="" w="100" x="60" y="80"/>
<edit focused="1" h="25" name="edit3" onchange="" root="@ysize" text="" w="100" x="60" y="110"/>
<button focused="" h="25" name="button1" onclick="@ok" text="$OK" w="100" x="20" y="220"/>
<button focused="" h="25" name="button2" onclick="@cancel" text="$Cancel" w="100" x="130" y="220"/>
<edit focused="1" h="25" name="edit4" onchange="" root="@name" text="Edit" w="100" x="60" y="140"/>
<label h="25" name="label4" root="" text="$Name" w="30" x="20" y="140"/>
<label h="25" name="label5" root="" text="$Begin-none-1-2" w="100" x="170" y="20"/>
<edit focused="1" h="25" name="edit5" onchange="" root="@x0" text="Edit" w="100" x="170" y="50"/>
<edit focused="1" h="25" name="edit6" onchange="" root="@z0" text="Edit" w="100" x="170" y="80"/>
<edit focused="1" h="25" name="edit7" onchange="" root="@y0" text="Edit" w="100" x="170" y="110"/>
<label h="25" name="label6" root="" text="^ $texture" w="100" x="290" y="150"/>
<packedlist cur="" focused="1" h="25" items="$Only-left;$Only-right;$Both-the-same;$Both-but-different;$None-texture" load.items="" name="packedlist1" onchange="" root="@texx" saveas="index" w="100" x="290" y="50"/>
<packedlist cur="" focused="1" h="25" items="$Only-bottom;$Only-top;$Both-the-same;$Both-the-different;$None-texture" load.items="" name="packedlist2" onchange="" root="@texy" saveas="index" w="100" x="290" y="110"/>
<packedlist cur="" focused="1" h="25" items="$Only-back;$Only-fore;$Both-the-same;$Both-the-different;$None-texture" load.items="" name="packedlist3" onchange="" root="@texz" saveas="index" w="100" x="290" y="80"/>
<label h="25" name="label7" root="" text="$Texture-detail-level" w="100" x="20" y="180"/>
<edit focused="1" h="25" name="edit8" onchange="" root="@texdetail" text="Edit" w="100" x="130" y="180"/>
<checkbox checked="0" focused="1" h="15" name="checkbox1" onclick="" root="@treemodel" text="$Tree-model" w="100" x="290" y="190"/>
</window>
<window clienth="200" clientw="290" closeonesc="1" name="window3" onclose="" onok="" position="center" rep.name="model.selresizefrm" state="normal" text="$Image-size">
<label h="35" name="label1" root="" text="$image-size-percents" w="240" x="20" y="20"/>
<label h="15" name="label2" root="" text="$Width" w="40" x="20" y="80"/>
<label h="15" name="label3" root="" text="$Height" w="40" x="20" y="120"/>
<edit focused="1" h="25" name="edit1" onchange="" root="@x" text="Edit" w="100" x="80" y="80"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@y" text="Edit" w="100" x="80" y="120"/>
<button focused="" h="25" name="button1" onclick="@ok" text="$OK" w="100" x="30" y="160"/>
<button focused="" h="25" name="button2" onclick="@cancel" text="$Cancel" w="100" x="140" y="160"/>
</window>
<window clienth="240" clientw="160" closeonesc="1" name="window4" onclose="" onok="" position="center" rep.name="model.runscript" state="normal" text="$Run-script">
<button focused="" h="25" name="button1" onclick="@ok" text="$OK" w="100" x="20" y="170"/>
<button focused="" h="25" name="button2" onclick="@cancel" text="$Cancel" w="100" x="20" y="200"/>
<listbox cur="0" focused="" h="150" items="" load.items="@scripts" name="listbox1" onchange="" root="@cur" saveas="index" w="120" x="20" y="10"/>
</window>
<window clienth="190" clientw="270" closeonesc="1" name="window5" onclose="" onok="" position="center" rep.name="poed.newbitmap" state="normal" text="$New-bitmap">
<edit focused="1" h="25" name="edit1" onchange="" root="@w" text="" w="100" x="140" y="30"/>
<label h="25" name="label1" root="" text="$Width" w="100" x="30" y="30"/>
<label h="25" name="label2" root="" text="$Height" w="100" x="30" y="60"/>
<label h="25" name="label3" root="" text="$Name" w="100" x="30" y="90"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@h" text="" w="100" x="140" y="60"/>
<edit focused="1" h="25" name="edit3" onchange="" root="@name" text="" w="100" x="140" y="90"/>
<button focused="" h="25" name="button1" onclick="@ok" text="$OK" w="100" x="30" y="140"/>
<button focused="" h="25" name="button2" onclick="@cancel" text="$Cancel" w="100" x="140" y="140"/>
</window>
<window clienth="330" clientw="420" closeonesc="1" name="window6" onclose="" onok="" position="center" rep.name="poed.editbmpsfrm" state="normal" text="$Edit-bitmaps">
<listbox cur="0" focused="" h="190" items="" load.items="" name="bitmapfiles" onchange="bitmapchange()" root="" saveas="index" w="150" x="20" y="40"/>
<button focused="" h="25" name="button1" onclick="addbitmappicture()" text="$Add" w="100" x="190" y="40"/>
<button focused="" h="25" name="button2" onclick="erasebitmappicture()" text="$Delete" w="100" x="190" y="70"/>
<button focused="" h="25" name="button3" onclick="editbitmappicture()" text="$Edit" w="100" x="190" y="100"/>
<paintbox doublebuffer="0" erasebkgnd="" h="100" name="bitmappainter" onpaint="paintbitmap(canvas)" w="100" x="190" y="150"/>
</window>
<window clienth="260" clientw="350" closeonesc="1" name="window7" onclose="" onok="ok()" position="center" rep.name="poed.newobject" state="normal" text="$New-object">
<listbox cur="0" focused="" h="200" items="" load.items="" name="types" onchange="" root="" saveas="index" w="160" x="30" y="30"/>
<button focused="" h="25" name="button1" onclick="ok()" text="$OK" w="100" x="220" y="30"/>
<button focused="" h="25" name="button2" onclick="cancel()" text="$Cancel" w="100" x="220" y="60"/>
</window>
<window clienth="330" clientw="470" closeonesc="1" layout="1" name="window8" onclose="" onok="" position="center" rep.name="poed.newobjectfrm" state="normal" text="$New-object">
<pager>
<page text="$Common">
<button focused="" h="25" name="button1" onclick="okstd()" text="$OK" w="100" x="20" y="170"/>
<button focused="" h="25" name="button2" onclick="cancel()" text="$Cancel" w="100" x="20" y="200"/>
<listbox cur="0" focused="" h="150" items="" load.items="" name="scripts" onchange="" root="" saveas="" w="120" x="20" y="10"/>
</page>
<page text="$Skeleton">
<button focused="" h="25" name="button1" onclick="okskel()" text="$OK" w="100" x="20" y="170"/>
<button focused="" h="25" name="button2" onclick="cancel()" text="$Cancel" w="100" x="20" y="200"/>
<listbox cur="0" focused="" h="150" items="" load.items="" name="skeletons" onchange="" root="" saveas="" w="120" x="20" y="10"/>
</page>
<page text="$Template">
<button focused="" h="25" name="button1" onclick="oktpl()" text="$OK" w="100" x="20" y="170"/>
<button focused="" h="25" name="button2" onclick="cancel()" text="$Cancel" w="100" x="20" y="200"/>
<listbox cur="0" focused="" h="150" items="" load.items="" name="tpldirs" onchange="dirclick()" root="" saveas="" w="110" x="20" y="10"/>
<listbox cur="0" focused="" h="150" items="" load.items="" name="tpls" onchange="tplclick()" root="" saveas="" w="110" x="140" y="10"/>
<wraplabel h="150" name="tplcomment" root="" text="" w="120" x="260" y="10"/>
</page>
</pager>
</window>
</data>
