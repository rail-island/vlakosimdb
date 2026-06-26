<?xml version="1.0" encoding="Windows-1250"?>
<data>
<place clienth="260" clientw="550" name="objpanel" rep.name="poed.locoed">
<label h="25" name="label1" root="" text="$Power" w="100" x="20" y="20"/>
<label h="25" name="label2" root="" text="$Max-speed" w="100" x="20" y="50"/>
<label h="25" name="label3" root="" text="$Weight" w="100" x="20" y="80"/>
<label h="25" name="label4" root="" text="$Name" w="100" x="20" y="110"/>
<edit focused="1" h="25" name="edit1" onchange="" root="@power" text="" w="100" x="140" y="20"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@maxspeed" text="" w="100" x="140" y="50"/>
<edit focused="1" h="25" name="edit3" onchange="" root="@weight" text="" w="100" x="140" y="80"/>
<edit focused="1" h="25" name="edit4" onchange="" root="@name" text="" w="100" x="140" y="110"/>
<label h="25" name="label5" root="" text="$Author" w="100" x="20" y="140"/>
<label h="25" name="label6" root="" text="$Authors-email" w="100" x="20" y="170"/>
<edit focused="1" h="25" name="edit5" onchange="" root="@author" text="" w="100" x="140" y="140"/>
<edit focused="1" h="25" name="edit6" onchange="" root="@email" text="" w="100" x="140" y="170"/>
<checkbox checked="0" focused="1" h="25" name="checkbox1" onclick="" root="@default" text="$Implicit" w="100" x="270" y="150"/>
<label h="25" name="label7" root="" text="$Title" w="100" x="20" y="200"/>
<edit focused="1" h="25" name="edit7" onchange="" root="@title" text="" w="100" x="140" y="200"/>
<label h="25" name="label8" root="" text="$Directory" w="100" x="270" y="20"/>
<edit focused="1" h="25" name="edit8" onchange="" root="@path" text="" w="100" x="380" y="20"/>
<label h="25" name="label9" root="" text="$Smoke:X" w="100" x="270" y="50"/>
<label h="25" name="label10" root="" text="$Smoke:Y" w="100" x="270" y="80"/>
<label h="25" name="label11" root="" text="$Smoke:$file" w="100" x="270" y="110"/>
<edit focused="1" h="25" name="edit9" onchange="" root="@smoke_x" text="" w="100" x="380" y="50"/>
<edit focused="1" h="25" name="edit10" onchange="" root="@smoke_y" text="" w="100" x="380" y="80"/>
<edit focused="1" h="25" name="edit11" onchange="" root="@smoke" text="" w="100" x="380" y="110"/>
</place>
<window clienth="240" clientw="280" closeonesc="1" name="window1" onclose="" onok="" position="center" rep.name="poed.includedlg" state="normal" text="$Insert">
<listbox cur="0" focused="" h="150" items="" load.items="" name="root" onchange="rootchange()" root="" saveas="index" w="110" x="20" y="40"/>
<listbox cur="0" focused="" h="150" items="" load.items="" name="child" onchange="childchange()" root="" saveas="index" w="110" x="140" y="40"/>
<button focused="" h="25" name="button1" onclick="ok()" text="$OK" w="100" x="30" y="200"/>
<button focused="" h="25" name="button2" onclick="cancel()" text="$Cancel" w="100" x="140" y="200"/>
<label h="25" name="label1" root="" text="Objekty na hlavní úrovni" w="100" x="10" y="10"/>
<label h="25" name="label2" root="" text="Vložené objekty" w="100" x="140" y="10"/>
</window>
<place clienth="300" clientw="570" name="window2" rep.name="ctrl.inlineobjs">
<listbox cur="0" focused="" h="210" items="" load.items="" name="inlinelist" onchange="changedinlinelist()" root="" saveas="" w="150" x="30" y="30"/>
<button focused="" h="25" name="button1" onclick="addinline()" text="$Add" w="100" x="210" y="30"/>
<button focused="" h="25" name="button2" onclick="editinline()" text="$Edit-link" w="100" x="210" y="60"/>
<button focused="" h="25" name="button3" onclick="eraseinline()" text="$Delete" w="100" x="210" y="90"/>
<edit focused="1" h="25" name="edit1" onchange="" root="@comment" text="" w="100" x="330" y="130"/>
<label h="25" name="label1" root="" text="$Label" w="100" x="210" y="130"/>
<radiogroup cur="" focused="" h="50" items="$Content;$Link" name="radiogroup1" onchange="" root="@what" text="$What-insert" w="110" x="210" y="170"/>
<radiogroup cur="" focused="" h="50" items="$Before-color;$After-color" name="radiogroup2" onchange="" root="@when" text="$When-insert" w="110" x="330" y="170"/>
<button focused="" h="25" name="button4" onclick="saveinline()" text="$Save" w="100" x="30" y="250"/>
<proplist h="170" name="inlineargs" onchange="" w="110" x="450" y="50"/>
<button focused="" h="25" name="button5" onclick="addinlinearg()" text="$Add" w="100" x="450" y="230"/>
<button focused="" h="25" name="button6" onclick="eraseinlinearg()" text="$Delete" w="100" x="450" y="260"/>
<label h="25" name="label2" root="" text="$Arguments" w="100" x="450" y="30"/>
</place>
<place clienth="600" clientw="660" name="place1" rep.name="ctrl.colorize">
<listbox cur="0" focused="" h="240" items="" load.items="" name="colorizelist" onchange="changecolorizelist()" root="" saveas="" w="160" x="10" y="30"/>
<button focused="" h="25" name="button1" onclick="addcolorize()" text="$Add" w="100" x="10" y="280"/>
<button focused="" h="25" name="button2" onclick="erasecolorize()" text="$Delete" w="100" x="10" y="310"/>
<label h="25" name="label1" root="" text="$Texture" w="60" x="180" y="140"/>
<edit focused="1" h="25" name="colorizetexture" onchange="" root="@texture" text="" w="100" x="250" y="140"/>
<checkbox checked="0" focused="1" h="25" name="checkbox1" onclick="" root="@transp" text="$Transposition" w="100" x="180" y="170"/>
<label h="25" name="label2" root="" text="$Scale X" w="60" x="180" y="200"/>
<label h="25" name="label3" root="" text="$Scale Y" w="60" x="180" y="230"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@xscale" text="" w="100" x="250" y="200"/>
<edit focused="1" h="25" name="edit3" onchange="" root="@yscale" text="" w="100" x="250" y="230"/>
<checkbox checked="0" focused="1" h="25" name="checkbox2" onclick="" root="@transparent" text="$Transparent-texture" w="100" x="180" y="260"/>
<checklist focused="" h="100" items="$From-left (x-inc);$From-right (x-dec);$From-bottom (y-inc);$From-top (y-dec);$To-viewer (z-inc);$Into-screen (z-dec)" name="colorizedirs" onchange="" oncheck="" root="@dirs" visiblecheck="1" w="140" x="180" y="30"/>
<button focused="" h="25" name="button3" onclick="saveactcolorize()" text="$Save" w="100" x="10" y="340"/>
<listbox cur="0" focused="" h="180" items="" load.items="" name="matfiles" onchange="matchange()" root="" saveas="" w="110" x="380" y="30"/>
<paintbox doublebuffer="0" erasebkgnd="" h="150" name="bitmappainter" onpaint="paintbitmap(canvas)" w="180" x="380" y="250"/>
<button focused="" h="25" name="button4" onclick="setcolorize()" text="&lt;&lt;" w="220" x="380" y="220"/>
<listbox cur="0" focused="" h="180" items="" load.items="" name="bitmapfiles" onchange="bitmapchange()" root="" saveas="index" w="100" x="500" y="30"/>
<edit focused="1" h="25" name="edit1" onchange="" root="@texx1" text="" w="100" x="250" y="300"/>
<edit focused="1" h="25" name="edit4" onchange="" root="@texy1" text="" w="100" x="250" y="330"/>
<edit focused="1" h="25" name="edit5" onchange="" root="@texx2" text="" w="100" x="250" y="360"/>
<edit focused="1" h="25" name="edit6" onchange="" root="@texy2" text="" w="100" x="250" y="390"/>
<label h="25" name="label4" root="" text="X1" w="50" x="190" y="300"/>
<label h="25" name="label5" root="" text="Y1" w="50" x="190" y="330"/>
<label h="25" name="label6" root="" text="X2" w="50" x="190" y="360"/>
<label h="25" name="label7" root="" text="Y2" w="50" x="190" y="390"/>
<button focused="" h="25" name="button5" onclick="addbitmappicture()" text="+" w="30" x="610" y="30"/>
<button focused="" h="25" name="button6" onclick="erasebitmappicture()" text="-" w="30" x="610" y="60"/>
<button focused="" h="25" name="button7" onclick="editbitmappicture()" text="*" w="30" x="610" y="90"/>
<checkbox checked="0" focused="1" h="25" name="checkbox3" onclick="" root="@material" text="$Material" w="100" x="10" y="370"/>
<checkbox checked="0" focused="1" h="25" name="checkbox4" onclick="" root="@stamp" text="$Stamp" w="100" x="10" y="390"/>
<edit focused="1" h="25" name="edit7" onchange="" root="@cond" text="" w="100" x="250" y="420"/>
<label h="25" name="label8" root="" text="$Condition" w="50" x="190" y="420"/>
<edit focused="1" h="25" name="edit8" onchange="" root="@rkoef" text="" w="100" x="250" y="450"/>
<edit focused="1" h="25" name="edit9" onchange="" root="@gkoef" text="" w="100" x="250" y="480"/>
<edit focused="1" h="25" name="edit10" onchange="" root="@bkoef" text="" w="100" x="250" y="510"/>
<label h="25" name="label9" root="" text="$Red 0-1" w="50" x="190" y="450"/>
<label h="25" name="label10" root="" text="$Green 0-1" w="50" x="190" y="480"/>
<label h="25" name="label11" root="" text="$Blue 0-1" w="50" x="190" y="510"/>
<edit focused="1" h="25" name="edit11" onchange="" root="@stampcntx" text="" w="100" x="70" y="410"/>
<edit focused="1" h="25" name="edit12" onchange="" root="@stampcnty" text="" w="100" x="70" y="440"/>
<edit focused="1" h="25" name="edit13" onchange="" root="@stampspacex" text="" w="100" x="70" y="470"/>
<edit focused="1" h="25" name="edit14" onchange="" root="@stampspacey" text="" w="100" x="70" y="500"/>
<label h="25" name="label12" root="" text="SCX" w="40" x="10" y="410"/>
<label h="25" name="label13" root="" text="SCY" w="40" x="10" y="440"/>
<label h="25" name="label14" root="" text="SSX" w="40" x="10" y="470"/>
<label h="25" name="label15" root="" text="SSY" w="40" x="10" y="500"/>
<label h="25" name="label16" root="" text="STd" w="40" x="10" y="530"/>
<edit focused="1" h="25" name="edit15" onchange="" root="@stampdist" text="" w="100" x="70" y="530"/>
<checkbox checked="0" focused="1" h="25" name="checkbox5" onclick="" root="@denylight" text="$Deny-light" w="100" x="190" y="540"/>
</place>
<place clienth="220" clientw="280" name="objpanel" rep.name="poed.semed">
<label h="25" name="label5" root="" text="$Author" w="100" x="20" y="20"/>
<label h="25" name="label6" root="" text="$Authors-email" w="100" x="20" y="50"/>
<edit focused="1" h="25" name="edit5" onchange="" root="@author" text="" w="100" x="140" y="20"/>
<edit focused="1" h="25" name="edit6" onchange="" root="@email" text="" w="100" x="140" y="50"/>
<edit focused="1" h="25" name="edit1" onchange="" root="@gradtime" text="" w="100" x="140" y="80"/>
<label h="25" name="label1" root="" text="$Lighting-time" w="100" x="20" y="80"/>
<button focused="" h="25" name="button1" onclick="setstate(&apos;stop&apos;)" text="$Stop" w="100" x="20" y="190"/>
<button focused="" h="25" name="button2" onclick="setstate(&apos;free&apos;)" text="$Free" w="100" x="140" y="190"/>
<label h="25" name="label2" root="" text="$Name" w="100" x="20" y="110"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@name" text="" w="100" x="140" y="110"/>
<checkbox checked="0" focused="1" h="25" name="checkbox1" onclick="" root="@default" text="$Implicit" w="100" x="20" y="170"/>
<label h="25" name="label7" root="" text="$Title" w="100" x="20" y="140"/>
<edit focused="1" h="25" name="edit7" onchange="" root="@title" text="" w="100" x="140" y="140"/>
</place>
<place clienth="230" clientw="310" name="place2" rep.name="poed.buildinged">
<label h="25" name="label5" root="" text="$Author" w="100" x="20" y="20"/>
<label h="25" name="label6" root="" text="$Authors-email" w="100" x="20" y="50"/>
<edit focused="1" h="25" name="edit5" onchange="" root="@author" text="" w="100" x="140" y="20"/>
<edit focused="1" h="25" name="edit6" onchange="" root="@email" text="" w="100" x="140" y="50"/>
<edit focused="1" h="25" name="edit1" onchange="" root="@w" text="" w="100" x="140" y="80"/>
<label h="25" name="label1" root="" text="$Length" w="100" x="20" y="80"/>
<label h="25" name="label2" root="" text="$Width" w="100" x="20" y="110"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@h" text="" w="100" x="140" y="110"/>
<label h="25" name="label7" root="" text="$Title" w="100" x="20" y="140"/>
<edit focused="1" h="25" name="edit7" onchange="" root="@title" text="" w="100" x="140" y="140"/>
</place>
<place clienth="230" clientw="300" name="place3" rep.name="poed.depoed">
<label h="25" name="label5" root="" text="$Author" w="100" x="20" y="20"/>
<label h="25" name="label6" root="" text="$Authors-email" w="100" x="20" y="50"/>
<edit focused="1" h="25" name="edit5" onchange="" root="@author" text="" w="100" x="140" y="20"/>
<edit focused="1" h="25" name="edit6" onchange="" root="@email" text="" w="100" x="140" y="50"/>
<edit focused="1" h="25" name="edit1" onchange="" root="@name" text="" w="100" x="140" y="80"/>
<label h="25" name="label1" root="" text="$Name" w="100" x="20" y="80"/>
<checkbox checked="0" focused="1" h="25" name="checkbox1" onclick="" root="@default" text="$Implicit" w="100" x="20" y="160"/>
<label h="25" name="label7" root="" text="$Title" w="100" x="20" y="110"/>
<edit focused="1" h="25" name="edit7" onchange="" root="@title" text="" w="100" x="140" y="110"/>
</place>
<place clienth="320" clientw="570" name="place4" rep.name="poed.tunneled">
<label h="25" name="label5" root="" text="$Author" w="100" x="20" y="20"/>
<label h="25" name="label6" root="" text="$Authors-email" w="100" x="20" y="50"/>
<edit focused="1" h="25" name="edit5" onchange="" root="@author" text="" w="100" x="140" y="20"/>
<edit focused="1" h="25" name="edit6" onchange="" root="@email" text="" w="100" x="140" y="50"/>
<edit focused="1" h="25" name="edit1" onchange="" root="@name" text="" w="100" x="140" y="80"/>
<label h="25" name="label1" root="" text="$Name" w="100" x="20" y="80"/>
<label h="25" name="label2" root="" text="$Hor-hole-space (0..10)" w="100" x="20" y="110"/>
<label h="25" name="label3" root="" text="$ver-hole-space (0..1)" w="100" x="20" y="140"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@hor" text="" w="100" x="140" y="110"/>
<edit focused="1" h="25" name="edit3" onchange="" root="@ver" text="" w="100" x="140" y="140"/>
<checkbox checked="0" focused="1" h="20" name="checkbox1" onclick="" root="@default" text="$Implicit" w="100" x="330" y="50"/>
<edit focused="1" h="25" name="edit4" onchange="" root="@top.texture" text="" w="100" x="140" y="170"/>
<edit focused="1" h="25" name="edit7" onchange="" root="@top.texsize" text="" w="70" x="250" y="170"/>
<edit focused="1" h="25" name="edit8" onchange="" root="@bot.texture" text="" w="100" x="140" y="200"/>
<edit focused="1" h="25" name="edit9" onchange="" root="@bot.texsize" text="" w="70" x="250" y="200"/>
<label h="25" name="label4" root="" text="$Texture $up, $scale" w="100" x="20" y="170"/>
<label h="25" name="label7" root="" text="$Texture $down, $scale" w="100" x="20" y="200"/>
<label h="25" name="label7" root="" text="$Title" w="100" x="330" y="20"/>
<edit focused="1" h="25" name="edit7" onchange="" root="@title" text="" w="100" x="440" y="20"/>
</place>
<place clienth="640" clientw="650" name="rstplace" rep.name="poed.rsted">
<label h="25" name="label5" root="" text="$Author" w="100" x="20" y="20"/>
<label h="25" name="label6" root="" text="$Authors-email" w="100" x="20" y="50"/>
<edit focused="1" h="25" name="edit5" onchange="" root="?&gt;rstation=@author" text="" w="100" x="140" y="20"/>
<edit focused="1" h="25" name="edit6" onchange="" root="?&gt;rstation=@email" text="" w="100" x="140" y="50"/>
<edit focused="1" h="25" name="edit1" onchange="" root="?&gt;rstation=@name" text="" w="100" x="140" y="80"/>
<label h="25" name="label1" root="" text="$Name" w="100" x="20" y="80"/>
<listbox cur="0" focused="" h="160" items="" load.items="" name="perons" onchange="peronchange()" root="" saveas="" w="100" x="20" y="180"/>
<button focused="" h="25" name="button1" onclick="addperon()" text="$Add" w="100" x="140" y="170"/>
<button focused="" h="25" name="button2" onclick="eraseperon()" text="$Delete" w="100" x="140" y="140"/>
<edit focused="1" h="25" name="edit2" onchange="" root="?&gt;peron=@cnt" text="" w="100" x="290" y="210"/>
<label h="25" name="label2" root="" text="$Track-count" w="120" x="140" y="210"/>
<edit focused="1" h="25" name="edit3" onchange="" root="?&gt;peron=@min" text="" w="100" x="290" y="240"/>
<edit focused="1" h="25" name="edit4" onchange="" root="?&gt;peron=@max" text="" w="100" x="290" y="270"/>
<edit focused="1" h="25" name="trackedit" onchange="" root="?&gt;peron=@item" text="" w="100" x="290" y="300"/>
<label h="25" name="label3" root="" text="$Min-dist-from-edge" w="100" x="140" y="240"/>
<label h="25" name="label4" root="" text="$Max-dist-from-edge" w="100" x="140" y="270"/>
<label h="25" name="label7" root="" text="$Track-name" w="100" x="140" y="300"/>
<button focused="" h="25" name="button3" onclick="moveupperon()" text="&lt;&lt;" w="100" x="250" y="140"/>
<button focused="" h="25" name="button4" onclick="movedownperon()" text="&gt;&gt;" w="100" x="250" y="170"/>
<listbox cur="0" focused="" h="160" items="" load.items="" name="items" onchange="itemchange()" root="" saveas="" w="100" x="20" y="360"/>
<button focused="" h="25" name="button5" onclick="eraseitem()" text="$Delete" w="100" x="140" y="360"/>
<button focused="" h="25" name="button6" onclick="additem()" text="$Add" w="100" x="140" y="390"/>
<button focused="" h="25" name="button7" onclick="moveupitem()" text="&lt;&lt;" w="100" x="250" y="360"/>
<button focused="" h="25" name="button8" onclick="movedownitem()" text="&gt;&gt;" w="100" x="250" y="390"/>
<label h="25" name="label8" root="" text="$Track-length" w="100" x="140" y="430"/>
<label h="25" name="label9" root="" text="$Min-dist-from-edge" w="100" x="140" y="460"/>
<label h="25" name="label10" root="" text="$Max-dist-from-edge" w="100" x="140" y="490"/>
<label h="25" name="label11" root="" text="$Model" w="100" x="140" y="520"/>
<edit focused="1" h="25" name="edit7" onchange="" root="?&gt;item=@cnt" text="" w="100" x="290" y="430"/>
<edit focused="1" h="25" name="edit8" onchange="" root="?&gt;item=@min" text="" w="100" x="290" y="460"/>
<edit focused="1" h="25" name="edit9" onchange="" root="?&gt;item=@max" text="" w="100" x="290" y="490"/>
<edit focused="1" h="25" name="modeledit1" onchange="" root="?&gt;item=@model" text="" w="100" x="290" y="520"/>
<button focused="" h="25" name="button9" onclick="browsemodel1()" text="..." w="50" x="410" y="520"/>
<label h="25" name="label12" root="" text="$Track-count" w="100" x="140" y="550"/>
<edit focused="1" h="25" name="edit11" onchange="" root="?&gt;item=@tracks" text="" w="100" x="290" y="550"/>
<label h="25" name="label13" root="" text="$Name" w="100" x="140" y="580"/>
<edit focused="1" h="25" name="edit12" onchange="" root="?&gt;item=@name" text="" w="100" x="290" y="580"/>
<listbox cur="0" focused="" h="100" items="" load.items="" name="models" onchange="modelchange()" root="" saveas="" w="100" x="270" y="20"/>
<button focused="" h="25" name="button6" onclick="addmodel()" text="$Add" w="70" x="390" y="20"/>
<button focused="" h="25" name="button7" onclick="erasemodel()" text="$Delete" w="70" x="470" y="20"/>
<button focused="" h="25" name="button8" onclick="browsemodel()" text="..." w="40" x="590" y="60"/>
<edit focused="1" h="25" name="modeledit" onchange="" root="?&gt;model=@model" text="" w="120" x="460" y="60"/>
<label h="25" name="label8" root="" text="$Model" w="60" x="390" y="60"/>
<label h="25" name="label9" root="" text="$Translate X" w="60" x="390" y="90"/>
<edit focused="1" h="25" name="edit8" onchange="" root="?&gt;model=@dz" text="" w="120" x="460" y="120"/>
<label h="25" name="label10" root="" text="$Translate Z" w="70" x="390" y="120"/>
<packedlist cur="" focused="1" h="25" items="0;90;180;270" load.items="" name="packedlist1" onchange="" root="?&gt;model=@dir" saveas="index" w="120" x="460" y="180"/>
<label h="25" name="label11" root="" text="$Rotate" w="70" x="390" y="180"/>
<edit focused="1" h="25" name="edit9" onchange="" root="?&gt;model=@dy" text="" w="120" x="460" y="150"/>
<label h="25" name="label12" root="" text="$Translate Y" w="70" x="390" y="150"/>
<edit focused="1" h="25" name="edit4" onchange="" root="?&gt;model=@w" text="" w="120" x="460" y="210"/>
<edit focused="1" h="25" name="edit10" onchange="" root="?&gt;model=@h" text="" w="120" x="460" y="240"/>
<label h="25" name="label4" root="" text="$Length (X)" w="70" x="390" y="210"/>
<label h="25" name="label7" root="" text="$Width (Z)" w="70" x="390" y="240"/>
<edit focused="1" h="25" name="edit13" onchange="" root="?&gt;model=@dx" text="" w="120" x="460" y="90"/>
<checkbox checked="0" focused="1" h="25" name="checkbox1" onclick="" root="?&gt;rstation=@default" text="$Implicit" w="100" x="20" y="140"/>
<label h="25" name="label7" root="" text="$Title" w="100" x="20" y="110"/>
<edit focused="1" h="25" name="edit7" onchange="" root="?&gt;rstation=@title" text="" w="100" x="140" y="110"/>
<label h="25" name="label14" root="" text="$Begin:min,max,$End:min,max" w="100" x="140" y="330"/>
<edit focused="1" h="25" name="edit14" onchange="" root="?&gt;peron=@bmin" text="" w="30" x="290" y="330"/>
<edit focused="1" h="25" name="edit15" onchange="" root="?&gt;peron=@bmax" text="" w="30" x="320" y="330"/>
<edit focused="1" h="25" name="edit16" onchange="" root="?&gt;peron=@emin" text="" w="30" x="350" y="330"/>
<edit focused="1" h="25" name="edit17" onchange="" root="?&gt;peron=@emax" text="" w="30" x="380" y="330"/>
<label h="25" name="label15" root="" text="$Begin:min,max,$End:min,max" w="100" x="140" y="610"/>
<edit focused="1" h="25" name="edit18" onchange="" root="?&gt;item=@bmin" text="" w="30" x="290" y="610"/>
<edit focused="1" h="25" name="edit19" onchange="" root="?&gt;item=@bmax" text="" w="30" x="320" y="610"/>
<edit focused="1" h="25" name="edit20" onchange="" root="?&gt;item=@emin" text="" w="30" x="350" y="610"/>
<edit focused="1" h="25" name="edit21" onchange="" root="?&gt;item=@emax" text="" w="30" x="380" y="610"/>
<checkbox checked="0" focused="1" h="25" name="checkbox2" onclick="" root="?&gt;rstation=@depo" text="$Depot" w="100" x="20" y="160"/>
</place>
<place clienth="360" clientw="670" name="bridgeplace" rep.name="poed.bridgeed">
<label h="25" name="label5" root="" text="$Author" w="100" x="20" y="20"/>
<label h="25" name="label6" root="" text="$Authors-email" w="100" x="20" y="50"/>
<edit focused="1" h="25" name="edit5" onchange="" root="?&gt;bridge=@author" text="" w="100" x="140" y="20"/>
<edit focused="1" h="25" name="edit6" onchange="" root="?&gt;bridge=@email" text="" w="100" x="140" y="50"/>
<edit focused="1" h="25" name="edit1" onchange="" root="?&gt;bridge=@name" text="" w="100" x="140" y="80"/>
<label h="25" name="label1" root="" text="$Name" w="100" x="20" y="80"/>
<listbox cur="0" focused="" h="160" items="" load.items="" name="items" onchange="itemchange()" root="" saveas="" w="100" x="20" y="170"/>
<button focused="" h="25" name="button1" onclick="additem()" text="$Add" w="100" x="140" y="170"/>
<button focused="" h="25" name="button2" onclick="eraseitem()" text="$Delete" w="100" x="140" y="140"/>
<edit focused="1" h="25" name="edit2" onchange="" root="?&gt;item=@cnt" text="" w="100" x="290" y="210"/>
<label h="25" name="label2" root="" text="$Length" w="120" x="140" y="210"/>
<edit focused="1" h="25" name="edit3" onchange="" root="?&gt;item=@min" text="" w="100" x="290" y="240"/>
<edit focused="1" h="25" name="edit4" onchange="" root="?&gt;item=@max" text="" w="100" x="290" y="270"/>
<edit focused="1" h="25" name="modeledit1" onchange="" root="?&gt;item=@model" text="" w="100" x="290" y="300"/>
<label h="25" name="label3" root="" text="$Min-dist-from-edge" w="100" x="140" y="240"/>
<label h="25" name="label4" root="" text="$Max-dist-from-edge" w="100" x="140" y="270"/>
<label h="25" name="label7" root="" text="$Model-name" w="100" x="140" y="300"/>
<button focused="" h="25" name="button3" onclick="moveupitem()" text="&lt;&lt;" w="100" x="250" y="140"/>
<button focused="" h="25" name="button4" onclick="movedownitem()" text="&gt;&gt;" w="100" x="250" y="170"/>
<button focused="" h="25" name="button5" onclick="browsemodel1()" text="..." w="60" x="400" y="300"/>
<listbox cur="0" focused="" h="100" items="" load.items="" name="models" onchange="modelchange()" root="" saveas="" w="100" x="270" y="20"/>
<button focused="" h="25" name="button6" onclick="addmodel()" text="$Add" w="70" x="390" y="20"/>
<button focused="" h="25" name="button7" onclick="erasemodel()" text="$Delete" w="70" x="470" y="20"/>
<button focused="" h="25" name="button8" onclick="browsemodel()" text="..." w="40" x="590" y="60"/>
<edit focused="1" h="25" name="modeledit" onchange="" root="?&gt;model=@model" text="" w="120" x="460" y="60"/>
<label h="25" name="label8" root="" text="$Model" w="60" x="390" y="60"/>
<edit focused="1" h="25" name="edit7" onchange="" root="?&gt;model=@dx" text="" w="120" x="460" y="90"/>
<label h="25" name="label9" root="" text="$Translate X" w="60" x="390" y="90"/>
<edit focused="1" h="25" name="edit8" onchange="" root="?&gt;model=@dz" text="" w="120" x="460" y="120"/>
<label h="25" name="label10" root="" text="$Translate Z" w="70" x="390" y="120"/>
<packedlist cur="" focused="1" h="25" items="0;90;180;270" load.items="" name="packedlist1" onchange="" root="?&gt;model=@dir" saveas="index" w="120" x="460" y="180"/>
<label h="25" name="label11" root="" text="$Rotate" w="70" x="390" y="180"/>
<edit focused="1" h="25" name="edit9" onchange="" root="?&gt;model=@dy" text="" w="120" x="460" y="150"/>
<label h="25" name="label12" root="" text="$Translate Y" w="70" x="390" y="150"/>
<edit focused="1" h="25" name="edit4" onchange="" root="?&gt;model=@w" text="" w="120" x="460" y="210"/>
<edit focused="1" h="25" name="edit10" onchange="" root="?&gt;model=@h" text="" w="120" x="460" y="240"/>
<label h="25" name="label4" root="" text="$Length (X)" w="70" x="390" y="210"/>
<label h="25" name="label7" root="" text="$Width (Z)" w="70" x="390" y="240"/>
<checkbox checked="0" focused="1" h="25" name="checkbox1" onclick="" root="?&gt;bridge=@default" text="$Implicit" w="100" x="20" y="140"/>
<label h="25" name="label7" root="" text="$Title" w="100" x="20" y="110"/>
<edit focused="1" h="25" name="edit7" onchange="" root="?&gt;bridge=@title" text="" w="100" x="140" y="110"/>
</place>
<window clienth="190" clientw="170" closeonesc="1" name="window3" onclose="" onok="" position="center" rep.name="form.getxyz" state="normal" text="$Coordinates">
<edit focused="1" h="25" name="edit1" onchange="" root="@x" text="" w="100" x="50" y="20"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@y" text="" w="100" x="50" y="50"/>
<edit focused="1" h="25" name="edit3" onchange="" root="@z" text="" w="100" x="50" y="80"/>
<label h="25" name="label1" root="" text="X" w="30" x="10" y="20"/>
<label h="25" name="label2" root="" text="Y" w="30" x="10" y="50"/>
<label h="25" name="label3" root="" text="Z" w="30" x="10" y="80"/>
<button focused="" h="25" name="button1" onclick="@ok" text="$OK" w="100" x="30" y="120"/>
<button focused="" h="25" name="button2" onclick="@cancel" text="$Cancel" w="100" x="30" y="150"/>
</window>
<place clienth="470" clientw="560" name="treesplace" rep.name="poed.treesed">
<listbox cur="0" focused="" h="170" items="" load.items="" name="items" onchange="listchange()" root="" saveas="" w="120" x="20" y="120"/>
<label h="25" name="label5" root="" text="$Author" w="100" x="20" y="20"/>
<label h="25" name="label6" root="" text="$Authors-email" w="100" x="20" y="50"/>
<edit focused="1" h="25" name="edit5" onchange="" root="?&gt;trees=@author" text="" w="100" x="150" y="20"/>
<edit focused="1" h="25" name="edit6" onchange="" root="?&gt;trees=@email" text="" w="100" x="150" y="50"/>
<button focused="" h="25" name="button1" onclick="additem()" text="$Add" w="100" x="150" y="80"/>
<button focused="" h="25" name="button2" onclick="eraseitem()" text="$Delete" w="100" x="260" y="80"/>
<edit focused="1" h="25" name="edit1" onchange="" root="?&gt;tree=@name" text="" w="100" x="250" y="110"/>
<label h="25" name="label1" root="" text="$Name" w="70" x="150" y="110"/>
<edit focused="1" h="25" name="edit2" onchange="" root="?&gt;tree=@w" text="" w="100" x="250" y="170"/>
<label h="25" name="label2" root="" text="$Width-1-is-sqsize" w="150" x="150" y="140"/>
<listbox cur="0" focused="" h="210" items="" load.items="" name="files" onchange="filechange()" root="" saveas="" w="140" x="370" y="40"/>
<label h="15" name="label3" root="" text="$Files-in-archive" w="150" x="370" y="10"/>
<button focused="" h="25" name="button3" onclick="setfile()" text="&lt;&lt;" w="100" x="370" y="260"/>
<label h="25" name="label4" root="" text="$File" w="80" x="150" y="200"/>
<edit focused="1" h="25" name="fileedit" onchange="" root="?&gt;tree=@file" text="" w="100" x="250" y="200"/>
<paintbox doublebuffer="0" erasebkgnd="" h="170" name="paintbox" onpaint="painttree(canvas)" w="170" x="20" y="300"/>
<button focused="" h="25" name="button4" onclick="erasefile()" text="$Delete" w="100" x="370" y="290"/>
<checkbox checked="0" focused="1" h="25" name="checkbox1" onclick="" root="?&gt;tree=@default" text="$Implicit" w="100" x="210" y="300"/>
<label h="25" name="label7" root="" text="$Title" w="100" x="150" y="230"/>
<edit focused="1" h="25" name="edit7" onchange="" root="?&gt;tree=@title" text="" w="100" x="250" y="230"/>
</place>
<place clienth="270" clientw="640" name="bldplace" rep.name="poed.blded" x="0" y="0">
<label h="25" name="label5" root="" text="$Author" w="100" x="20" y="20"/>
<label h="25" name="label6" root="" text="$Authors-email" w="100" x="20" y="50"/>
<edit focused="1" h="25" name="edit5" onchange="" root="?&gt;building=@author" text="" w="100" x="150" y="20"/>
<edit focused="1" h="25" name="edit6" onchange="" root="?&gt;building=@email" text="" w="100" x="150" y="50"/>
<listbox cur="0" focused="" h="100" items="" load.items="" name="models" onchange="modelchange()" root="" saveas="" w="100" x="270" y="20"/>
<button focused="" h="25" name="button6" onclick="addmodel()" text="$Add" w="70" x="390" y="20"/>
<button focused="" h="25" name="button7" onclick="erasemodel()" text="$Delete" w="70" x="470" y="20"/>
<button focused="" h="25" name="button8" onclick="browsemodel()" text="..." w="40" x="590" y="60"/>
<edit focused="1" h="25" name="modeledit" onchange="" root="?&gt;model=@model" text="" w="120" x="460" y="60"/>
<label h="25" name="label8" root="" text="$Model" w="60" x="390" y="60"/>
<edit focused="1" h="25" name="edit7" onchange="" root="?&gt;model=@dx" text="" w="120" x="460" y="90"/>
<label h="25" name="label9" root="" text="$Translate X" w="60" x="390" y="90"/>
<edit focused="1" h="25" name="edit8" onchange="" root="?&gt;model=@dz" text="" w="120" x="460" y="120"/>
<label h="25" name="label10" root="" text="$Translate Z" w="70" x="390" y="120"/>
<packedlist cur="" focused="1" h="25" items="0;90;180;270" load.items="" name="packedlist1" onchange="" root="?&gt;model=@dir" saveas="index" w="120" x="460" y="180"/>
<label h="25" name="label11" root="" text="$Rotate" w="70" x="390" y="180"/>
<edit focused="1" h="25" name="edit9" onchange="" root="?&gt;model=@dy" text="" w="120" x="460" y="150"/>
<label h="25" name="label12" root="" text="$Translate Y" w="70" x="390" y="150"/>
<label h="25" name="label1" root="" text="$Length (X)" w="100" x="20" y="130"/>
<label h="25" name="label2" root="" text="$Width (Z)" w="100" x="20" y="160"/>
<edit focused="1" h="25" name="edit1" onchange="" root="?&gt;building=@w" text="" w="100" x="150" y="130"/>
<edit focused="1" h="25" name="edit2" onchange="" root="?&gt;building=@h" text="" w="100" x="150" y="160"/>
<label h="25" name="label3" root="" text="$Name" w="100" x="20" y="80"/>
<edit focused="1" h="25" name="edit3" onchange="" root="?&gt;building=@name" text="" w="100" x="150" y="80"/>
<edit focused="1" h="25" name="edit4" onchange="" root="?&gt;model=@w" text="" w="120" x="460" y="210"/>
<edit focused="1" h="25" name="edit10" onchange="" root="?&gt;model=@h" text="" w="120" x="460" y="240"/>
<label h="25" name="label4" root="" text="$Length (X)" w="70" x="390" y="210"/>
<label h="25" name="label7" root="" text="$Width (Z)" w="70" x="390" y="240"/>
<label h="25" name="label7" root="" text="$Title" w="100" x="20" y="200"/>
<edit focused="1" h="25" name="edit7" onchange="" root="?&gt;building=@title" text="" w="100" x="150" y="200"/>
</place>
<place clienth="600" clientw="820" name="place5" rep.name="ctrl.stamps">
<listbox cur="0" focused="" h="150" items="" load.items="" name="stamplist" onchange="changestamplist()" root="" saveas="" w="120" x="20" y="60"/>
<label h="25" name="label1" root="" text="$Stamps" w="100" x="20" y="20"/>
<label h="25" name="label2" root="" text="$Available-stamps" w="100" x="340" y="20"/>
<listbox cur="0" focused="" h="150" items="" load.items="" name="bitmapfiles" onchange="bitmapchange()" root="" saveas="" w="110" x="340" y="60"/>
<edit focused="1" h="25" name="edit1" onchange="" root="@comment" text="" w="100" x="230" y="70"/>
<button focused="" h="25" name="button1" onclick="addbitmappicture()" text="$Add" w="100" x="460" y="60"/>
<button focused="" h="25" name="button2" onclick="editbitmappicture()" text="$Edit" w="100" x="460" y="90"/>
<button focused="" h="25" name="button3" onclick="addstamp()" text="$Add" w="100" x="20" y="220"/>
<button focused="" h="25" name="button4" onclick="erasestamp()" text="$Delete" w="100" x="20" y="250"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@x1" text="" w="60" x="200" y="230"/>
<label h="25" name="label3" root="" text="X1" w="30" x="160" y="230"/>
<label h="25" name="label4" root="" text="Y1" w="30" x="160" y="260"/>
<label h="25" name="label5" root="" text="X2" w="30" x="160" y="290"/>
<label h="25" name="label6" root="" text="Y2" w="30" x="160" y="320"/>
<edit focused="1" h="25" name="edit3" onchange="" root="@y1" text="" w="60" x="200" y="260"/>
<edit focused="1" h="25" name="edit4" onchange="" root="@x2" text="" w="60" x="200" y="290"/>
<edit focused="1" h="25" name="edit5" onchange="" root="@y2" text="" w="60" x="200" y="320"/>
<paintbox doublebuffer="0" erasebkgnd="" h="350" name="bitmappainter" onpaint="paintbitmap(canvas)" w="360" x="390" y="230"/>
<button focused="" h="25" name="button5" onclick="setstamp()" text="&lt;&lt;" w="50" x="320" y="350"/>
<edit focused="1" h="25" name="stamptexture" onchange="" root="@texture" text="" w="100" x="200" y="350"/>
<label h="25" name="label7" root="" text="$Texture" w="50" x="140" y="350"/>
<button focused="" h="25" name="button6" onclick="saveactstamp()" text="$Save" w="100" x="20" y="300"/>
<button focused="" h="25" name="button7" onclick="erasebitmappicture()" text="$Delete" w="100" x="460" y="120"/>
<checklist focused="" h="100" items="$From-left;$From-right;$From-bottom;$From-top;$To-viewer;$Into-screen" name="checklist1" onchange="" oncheck="" root="@dirs" visiblecheck="1" w="100" x="160" y="100"/>
<label h="25" name="label8" root="" text="$Comment" w="60" x="160" y="70"/>
</place>
<place clienth="460" clientw="560" name="place6" rep.name="ctrl.colorizes">
<listbox cur="0" focused="" h="150" items="" load.items="" name="colorizelist" onchange="changestamplist()" root="" saveas="" w="120" x="20" y="60"/>
<label h="25" name="label1" root="" text="$Colorize" w="100" x="20" y="20"/>
<label h="25" name="label2" root="" text="$Available-images" w="100" x="340" y="20"/>
<listbox cur="0" focused="" h="150" items="" load.items="" name="bitmapfiles" onchange="bitmapchange()" root="" saveas="" w="110" x="340" y="60"/>
<edit focused="1" h="25" name="edit1" onchange="" root="@comment" text="" w="100" x="230" y="60"/>
<button focused="" h="25" name="button1" onclick="addbitmappicture()" text="$Add" w="100" x="460" y="60"/>
<button focused="" h="25" name="button2" onclick="editbitmappicture()" text="$Edit" w="100" x="460" y="90"/>
<button focused="" h="25" name="button3" onclick="addcolorize()" text="$Add" w="100" x="20" y="220"/>
<button focused="" h="25" name="button4" onclick="erasecolorize()" text="$Delete" w="100" x="20" y="250"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@x1" text="" w="60" x="200" y="230"/>
<label h="25" name="label3" root="" text="X1" w="30" x="160" y="230"/>
<label h="25" name="label4" root="" text="Y1" w="30" x="160" y="260"/>
<label h="25" name="label5" root="" text="X2" w="30" x="160" y="290"/>
<label h="25" name="label6" root="" text="Y2" w="30" x="160" y="320"/>
<edit focused="1" h="25" name="edit3" onchange="" root="@y1" text="" w="60" x="200" y="260"/>
<edit focused="1" h="25" name="edit4" onchange="" root="@x2" text="" w="60" x="200" y="290"/>
<edit focused="1" h="25" name="edit5" onchange="" root="@y2" text="" w="60" x="200" y="320"/>
<paintbox doublebuffer="0" erasebkgnd="" h="350" name="bitmappainter" onpaint="paintbitmap(canvas)" w="360" x="390" y="230"/>
<button focused="" h="25" name="button5" onclick="setcolorize()" text="&lt;&lt;" w="50" x="320" y="350"/>
<edit focused="1" h="25" name="stamptexture" onchange="" root="@texture" text="" w="100" x="200" y="350"/>
<label h="25" name="label7" root="" text="$Texture" w="50" x="140" y="350"/>
<button focused="" h="25" name="button6" onclick="saveactcolorize()" text="$Save" w="100" x="20" y="300"/>
<button focused="" h="25" name="button7" onclick="erasebitmappicture()" text="$Delete" w="100" x="460" y="120"/>
<checklist focused="" h="100" items="$From-left;$From-right;$From-bottom;$From-top;$To-viewer;$Into-screen" name="checklist1" onchange="" oncheck="" root="@dirs" visiblecheck="1" w="100" x="160" y="100"/>
<label h="25" name="label8" root="" text="$Comment" w="60" x="160" y="60"/>
</place>
<place clienth="320" clientw="450" name="place7" rep.name="ctrl.shapes">
<listbox cur="0" focused="" h="200" items="" load.items="" name="shapelist" onchange="changedshapelist()" root="" saveas="" w="130" x="30" y="30"/>
<button focused="" h="25" name="button1" onclick="addshape()" text="$Add" w="100" x="190" y="30"/>
<button focused="" h="25" name="button2" onclick="eraseshape()" text="$Delete" w="100" x="190" y="60"/>
<button focused="" h="25" name="button3" onclick="renameshape()" text="$Rename" w="100" x="190" y="90"/>
<packedlist cur="" focused="1" h="25" items="clip;rotate" load.items="" name="packedlist1" onchange="" root="@type" saveas="value" w="100" x="190" y="130"/>
<trackbar cur="0" frequency="1" h="20" max="32" min="3" name="rotateverteces" onchange="changerotateverteces()" root="@rotateverteces" vertical="0" w="100" x="190" y="170"/>
<label h="15" name="rotatevertecescaption" root="" text="" w="100" x="190" y="200"/>
<button focused="" h="25" name="button4" onclick="editrotate()" text="$Edit-rotate" w="100" x="190" y="230"/>
<checkbox checked="0" focused="1" h="25" name="checkbox1" onclick="" root="@cullsafe" text="$Cull-safe" w="100" x="190" y="260"/>
</place>
<window clienth="480" clientw="680" closeonesc="1" name="window4" onclose="onclose()" onok="" position="center" rep.name="poed.rotatesolidwin" state="normal" text="$Rotate-solid">
<listbox cur="0" focused="" h="240" items="" load.items="" name="colorizelist" onchange="changecolorizelist()" root="" saveas="" w="160" x="10" y="30"/>
<button focused="" h="25" name="button1" onclick="addcolorize()" text="$Add" w="100" x="10" y="280"/>
<button focused="" h="25" name="button2" onclick="erasecolorize()" text="$Delete" w="100" x="10" y="310"/>
<label h="25" name="label1" root="" text="$Texture" w="60" x="180" y="140"/>
<edit focused="1" h="25" name="colorizetexture" onchange="" root="@texture" text="" w="100" x="250" y="140"/>
<checkbox checked="0" focused="1" h="25" name="checkbox1" onclick="" root="@transp" text="$Transposition" w="100" x="180" y="170"/>
<label h="25" name="label2" root="" text="$Scale X" w="60" x="180" y="200"/>
<label h="25" name="label3" root="" text="$Scale Y" w="60" x="180" y="230"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@xscale" text="" w="100" x="250" y="200"/>
<edit focused="1" h="25" name="edit3" onchange="" root="@yscale" text="" w="100" x="250" y="230"/>
<checkbox checked="0" focused="1" h="25" name="checkbox2" onclick="" root="@transparent" text="$Transparent-texture" w="100" x="180" y="260"/>
<button focused="" h="25" name="button3" onclick="saveactcolorize()" text="$Save" w="100" x="10" y="340"/>
<listbox cur="0" focused="" h="180" items="" load.items="" name="matfiles" onchange="matchange()" root="" saveas="" w="110" x="380" y="30"/>
<paintbox doublebuffer="0" erasebkgnd="" h="150" name="bitmappainter" onpaint="paintbitmap(canvas)" w="180" x="380" y="250"/>
<button focused="" h="25" name="button4" onclick="setcolorize()" text="&lt;&lt;" w="220" x="380" y="220"/>
<listbox cur="0" focused="" h="180" items="" load.items="" name="bitmapfiles" onchange="bitmapchange()" root="" saveas="index" w="100" x="500" y="30"/>
<button focused="" h="25" name="button5" onclick="addbitmappicture()" text="+" w="30" x="610" y="30"/>
<button focused="" h="25" name="button6" onclick="erasebitmappicture()" text="-" w="30" x="610" y="60"/>
<button focused="" h="25" name="button7" onclick="editbitmappicture()" text="*" w="30" x="610" y="90"/>
<checkbox checked="0" focused="1" h="25" name="checkbox3" onclick="" root="@material" text="$Material" w="100" x="10" y="380"/>
<edit focused="1" h="25" name="edit7" onchange="" root="@cond" text="" w="100" x="250" y="290"/>
<label h="25" name="label8" root="" text="$Condition" w="50" x="180" y="290"/>
<checkbox checked="0" focused="1" h="25" name="checkbox4" onclick="" root="@reflectx" text="X $Reflection" w="100" x="180" y="330"/>
<checkbox checked="0" focused="1" h="25" name="checkbox5" onclick="" root="@reflecty" text="Y $Reflection" w="100" x="180" y="350"/>
</window>
<place clienth="320" clientw="590" name="place8" rep.name="ctrl.noise">
<listbox cur="0" focused="" h="170" items="" load.items="" name="noiselist" onchange="changenoiselist()" root="" saveas="" w="140" x="20" y="20"/>
<button focused="" h="25" name="button1" onclick="addnoise()" text="$Add" w="100" x="170" y="20"/>
<button focused="" h="25" name="button2" onclick="erasenoise()" text="$Delete" w="100" x="170" y="50"/>
<listbox cur="0" focused="" h="190" items="" load.items="" name="noiseallimages" onchange="" root="" saveas="" w="100" x="460" y="20"/>
<listbox cur="0" focused="" h="180" items="" load.items="" name="noiseimages" onchange="" root="" saveas="" w="100" x="300" y="20"/>
<button focused="" h="25" name="button3" onclick="addnoiseimage()" text="&lt;&lt;" w="40" x="410" y="20"/>
<button focused="" h="25" name="button4" onclick="erasenoiseimage()" text="$Delete" w="80" x="310" y="210"/>
<trackbar cur="0" frequency="1" h="30" max="100" min="0" name="rad_r" onchange="updatecolor()" root="@dr" vertical="0" w="110" x="170" y="110"/>
<trackbar cur="0" frequency="1" h="30" max="100" min="0" name="rad_g" onchange="updatecolor()" root="@dg" vertical="0" w="110" x="170" y="140"/>
<trackbar cur="0" frequency="1" h="30" max="100" min="0" name="rad_b" onchange="updatecolor()" root="@db" vertical="0" w="110" x="170" y="170"/>
<label h="15" name="noisecolor" root="" text="" w="110" x="170" y="90"/>
<edit focused="1" h="25" name="edit1" onchange="" root="@name" text="" w="100" x="170" y="210"/>
<label h="25" name="label1" root="" text="$Name" w="60" x="100" y="210"/>
<edit focused="1" h="25" name="noisecoloredit" onchange="" root="@srccolor" text="" w="100" x="170" y="240"/>
<label h="25" name="label2" root="" text="$Color" w="60" x="100" y="240"/>
<button focused="" h="25" name="button5" onclick="getnoisecolor()" text="$Get" w="100" x="170" y="270"/>
</place>
<place clienth="260" clientw="550" name="objpanel" rep.name="poed.motocared">
<label h="25" name="label1" root="" text="$Power" w="100" x="20" y="20"/>
<label h="25" name="label2" root="" text="$Max-speed" w="100" x="20" y="50"/>
<label h="25" name="label3" root="" text="$Weight" w="100" x="20" y="80"/>
<label h="25" name="label4" root="" text="$Name" w="100" x="20" y="110"/>
<edit focused="1" h="25" name="edit1" onchange="" root="@power" text="" w="100" x="140" y="20"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@maxspeed" text="" w="100" x="140" y="50"/>
<edit focused="1" h="25" name="edit3" onchange="" root="@weight" text="" w="100" x="140" y="80"/>
<edit focused="1" h="25" name="edit4" onchange="" root="@name" text="" w="100" x="140" y="110"/>
<label h="25" name="label5" root="" text="$Author" w="100" x="20" y="140"/>
<label h="25" name="label6" root="" text="$Authors-email" w="100" x="20" y="170"/>
<edit focused="1" h="25" name="edit5" onchange="" root="@author" text="" w="100" x="140" y="140"/>
<edit focused="1" h="25" name="edit6" onchange="" root="@email" text="" w="100" x="140" y="170"/>
<checkbox checked="0" focused="1" h="25" name="checkbox1" onclick="" root="@default" text="$Implicit" w="100" x="270" y="140"/>
<label h="25" name="label7" root="" text="$Title" w="100" x="20" y="200"/>
<edit focused="1" h="25" name="edit7" onchange="" root="@title" text="" w="100" x="140" y="200"/>
<label h="25" name="label8" root="" text="$Directory" w="100" x="270" y="20"/>
<edit focused="1" h="25" name="edit8" onchange="" root="@path" text="" w="100" x="380" y="20"/>
<label h="25" name="label9" root="" text="$Pas-wi" w="100" x="270" y="50"/>
<label h="25" name="label10" root="" text="$Pas-y1" w="100" x="270" y="80"/>
<label h="25" name="label11" root="" text="$Pas-y2" w="100" x="270" y="110"/>
<edit focused="1" h="25" name="edit9" onchange="" root="@pas_wi" text="" w="100" x="380" y="50"/>
<edit focused="1" h="25" name="edit10" onchange="" root="@pas_y1" text="" w="100" x="380" y="80"/>
<edit focused="1" h="25" name="edit11" onchange="" root="@pas_y2" text="" w="100" x="380" y="110"/>
<edit focused="1" h="25" name="edit12" onchange="" root="@roadstyle" text="" w="100" x="380" y="170"/>
<label h="25" name="label12" root="" text="$Road-style" w="100" x="270" y="170"/>
<edit focused="1" h="25" name="edit13" onchange="" root="@tramspace" text="" w="100" x="380" y="200"/>
<label h="25" name="label13" root="" text="$Tramway-space" w="100" x="270" y="200"/>
</place>
<place clienth="260" clientw="550" name="objpanel" rep.name="poed.busstoped">
<label h="25" name="label4" root="" text="$Name" w="100" x="20" y="20"/>
<edit focused="1" h="25" name="edit4" onchange="" root="@name" text="" w="100" x="140" y="20"/>
<label h="25" name="label5" root="" text="$Author" w="100" x="20" y="50"/>
<label h="25" name="label6" root="" text="$Authors-email" w="100" x="20" y="80"/>
<edit focused="1" h="25" name="edit5" onchange="" root="@author" text="" w="100" x="140" y="50"/>
<edit focused="1" h="25" name="edit6" onchange="" root="@email" text="" w="100" x="140" y="80"/>
<checkbox checked="0" focused="1" h="25" name="checkbox1" onclick="" root="@default" text="$Implicit" w="100" x="20" y="150"/>
<label h="25" name="label7" root="" text="$Title" w="100" x="20" y="110"/>
<edit focused="1" h="25" name="edit7" onchange="" root="@title" text="" w="100" x="140" y="110"/>
<label h="25" name="label8" root="" text="$Directory" w="100" x="270" y="20"/>
<edit focused="1" h="25" name="edit8" onchange="" root="@path" text="" w="100" x="380" y="20"/>
</place>
<place clienth="220" clientw="390" name="place9" rep.name="poed.cabed"/>
<window clienth="130" clientw="250" closeonesc="1" name="window5" onclose="" onok="" position="center" rep.name="form.getxy" state="normal" text="$Coordinates">
<edit focused="1" h="25" name="edit1" onchange="" root="@x" text="" w="100" x="130" y="20"/>
<label h="25" name="label1" root="" text="X" w="50" x="20" y="20"/>
<label h="25" name="label2" root="" text="Y" w="50" x="20" y="50"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@y" text="" w="100" x="130" y="50"/>
<button focused="" h="25" name="button1" onclick="@ok" text="$OK" w="100" x="20" y="90"/>
<button focused="" h="25" name="button2" onclick="@cancel" text="$Cancel" w="100" x="130" y="90"/>
</window>
</data>
