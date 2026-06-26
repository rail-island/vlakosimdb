<?xml version="1.0" encoding="Windows-1250"?>
<data>
<window clienth="270" clientw="200" closeonesc="1" name="window1" onclose="" onok="" position="center" rep.name="poed.newlocofrm" state="normal" text="$New-locomotive">
<edit focused="1" h="25" name="edit1" onchange="" root="@len" text="" w="100" x="80" y="60"/>
<label h="25" name="label1" root="" text="$Length" w="50" x="20" y="60"/>
<label h="25" name="label2" root="" text="$Width" w="50" x="20" y="90"/>
<label h="25" name="label3" root="" text="$Height" w="50" x="20" y="120"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@name" text="" w="100" x="80" y="20"/>
<label h="25" name="label4" root="" text="$Name" w="50" x="20" y="20"/>
<edit focused="1" h="25" name="edit3" onchange="" root="@wi" text="" w="100" x="80" y="90"/>
<edit focused="1" h="25" name="edit4" onchange="" root="@hi" text="" w="100" x="80" y="120"/>
<button focused="" h="25" name="button1" onclick="@ok" text="$OK" w="80" x="20" y="230"/>
<button focused="" h="25" name="button2" onclick="@cancel" text="$Cancel" w="80" x="100" y="230"/>
<checkbox checked="0" focused="1" h="15" name="checkbox1" onclick="" root="@difx" text="$Different-begin-and-end" w="160" x="20" y="160"/>
<checkbox checked="0" focused="1" h="15" name="checkbox2" onclick="" root="@difz" text="$Different-left-and-right" w="140" x="20" y="180"/>
</window>
<window clienth="260" clientw="230" closeonesc="1" name="window2" onclose="" onok="" position="center" rep.name="poed.newbuildingfrm" state="normal" text="">
<button focused="" h="25" name="button1" onclick="@ok" text="$OK" w="100" x="10" y="220"/>
<button focused="" h="25" name="button2" onclick="@cancel" text="$Cancel" w="100" x="120" y="220"/>
<label h="25" name="label1" root="" text="$Length" w="70" x="20" y="20"/>
<edit focused="1" h="25" name="edit1" onchange="" root="@len" text="" w="100" x="110" y="20"/>
<label h="25" name="label2" root="" text="$Width" w="70" x="20" y="50"/>
<label h="25" name="label3" root="" text="$Height" w="70" x="20" y="80"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@wi" text="" w="100" x="110" y="50"/>
<edit focused="1" h="25" name="edit3" onchange="" root="@hi" text="" w="100" x="110" y="80"/>
<edit focused="1" h="25" name="edit4" onchange="" root="@name" text="" w="100" x="110" y="120"/>
<label h="25" name="label4" root="" text="$Name" w="70" x="20" y="120"/>
<checkbox checked="0" focused="1" h="25" name="checkbox1" onclick="" root="@eqsides" text="$Equal-sides" w="100" x="20" y="150"/>
<checkbox checked="0" focused="1" h="25" name="checkbox2" onclick="" root="@halfdetail" text="$Half-detail" w="100" x="20" y="170"/>
</window>
<window clienth="280" clientw="200" closeonesc="1" name="window1" onclose="" onok="" position="center" rep.name="poed.newmotocarfrm" state="normal" text="$New-motocar">
<edit focused="1" h="25" name="edit1" onchange="" root="@len" text="" w="100" x="80" y="60"/>
<label h="25" name="label1" root="" text="$Length" w="50" x="20" y="60"/>
<label h="25" name="label2" root="" text="$Width" w="50" x="20" y="90"/>
<label h="25" name="label3" root="" text="$Height" w="50" x="20" y="120"/>
<edit focused="1" h="25" name="edit2" onchange="" root="@name" text="" w="100" x="80" y="20"/>
<label h="25" name="label4" root="" text="$Name" w="50" x="20" y="20"/>
<edit focused="1" h="25" name="edit3" onchange="" root="@wi" text="" w="100" x="80" y="90"/>
<edit focused="1" h="25" name="edit4" onchange="" root="@hi" text="" w="100" x="80" y="120"/>
<button focused="" h="25" name="button1" onclick="@ok" text="$OK" w="80" x="20" y="240"/>
<button focused="" h="25" name="button2" onclick="@cancel" text="$Cancel" w="80" x="100" y="240"/>
<checkbox checked="0" focused="1" h="15" name="checkbox1" onclick="" root="@difx" text="$Different-begin-and-end" w="160" x="20" y="160"/>
<checkbox checked="0" focused="1" h="15" name="checkbox2" onclick="" root="@difz" text="$Different-left-and-right" w="140" x="20" y="180"/>
<label h="25" name="label1" root="" text="$Style" w="50" x="20" y="200"/>
<packedlist cur="" focused="1" h="25" items="$Solid-motocar;$Long-bus;$Two-vagon-tramway;$Two-part-tramway;$Three-part-tramway" load.items="" name="packedlist1" onchange="" root="@style" saveas="index" w="100" x="80" y="200"/>
</window>
</data>
