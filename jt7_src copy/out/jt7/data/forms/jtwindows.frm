<?xml version="1.0" encoding="Windows-1250"?>
<data>
<window clienth="150" clientw="360" name="Window0" onclose="onstartclose()" position="center" rep.name="jtycon.startwin" text="VlakoSim">
<button focused="" h="25" name="button1" onclick="doquit()" text="$Quit" w="130" x="190" y="110"/>
<button focused="" h="25" name="button2" onclick="newgame()" text="$New $terrain" w="140" x="30" y="110"/>
<button focused="" h="25" name="button3" onclick="loadgame()" text="$Load $terrain" w="140" x="30" y="70"/>
<button focused="" h="25" name="button4" onclick="oglopt()" text="$OpenGL-options" w="130" x="190" y="70"/>
</window>
<window clienth="220" clientw="270" closeonesc="1" name="window1" onclose="" onok="dook()" position="center" rep.name="jtycon.openfile" state="normal" text="$Load $terrain">
<listbox focused="" h="140" items="" name="files" onchange="" root="" w="120" x="10" y="50"/>
<button focused="" h="25" name="button1" onclick="dook()" text="$Load" w="100" x="150" y="40"/>
<button focused="" h="25" name="button2" onclick="doclose()" text="$Cancel" w="100" x="150" y="70"/>
<label h="25" name="label1" root="" text="$Files" w="100" x="10" y="10"/>
</window>
<window clienth="150" clientw="230" closeonesc="1" name="window2" onclose="onclose()" onok="" position="center" rep.name="jtycon.rstfrm" state="normal" text="$Railway-station">
<button focused="" h="25" name="button1" onclick="dorename()" text="$Rename" w="100" x="10" y="10"/>
<button focused="" h="25" name="button2" onclick="doclose()" text="$Close" w="100" x="10" y="40"/>
<listbox focused="" h="100" items="" name="types" onchange="" root="" w="100" x="120" y="10"/>
<button focused="" h="25" name="button3" onclick="rebuild()" text="$Rebuild" w="100" x="10" y="70"/>
</window>
<window clienth="110" clientw="210" closeonesc="1" name="window3" onclose="" onok="" position="center" rep.name="jtycon.renamewin" state="normal" text="$Rename">
<edit focused="1" h="25" name="nameedit" onchange="" root="" text="Edit" w="170" x="20" y="30"/>
<label h="15" name="label1" root="" text="$Name" w="100" x="10" y="10"/>
<button focused="" h="25" name="button1" onclick="renameok()" text="$Ok" w="80" x="20" y="70"/>
<button focused="" h="25" name="button2" onclick="renameclose()" text="$Cancel" w="80" x="110" y="70"/>
</window>
<window clienth="180" clientw="230" closeonesc="1" name="window4" onclose="onclose()" onok="" position="center" rep.name="jtycon.rdepofrm" state="normal" text="$Depo">
<button focused="" h="25" name="button2" onclick="newtrain()" text="$New $train" w="100" x="120" y="120"/>
<listbox focused="" h="100" items="" name="lines" onchange="" root="" w="100" x="120" y="10"/>
<listbox cur="0" focused="" h="100" items="" load.items="" name="types" onchange="" root="" saveas="index" w="100" x="10" y="10"/>
<button focused="" h="25" name="button1" onclick="rebuild()" text="$Rebuild" w="100" x="10" y="120"/>
<button focused="" h="25" name="button3" onclick="dorename()" text="$Rename" w="100" x="10" y="150"/>
</window>
<window clienth="190" clientw="200" closeonesc="1" name="window5" onclose="onclose()" onok="" position="center" rep.name="jtycon.trainfrm" state="normal" text="$Train">
<button focused="" h="25" name="button1" onclick="startstop()" text="$Start/$Stop" w="70" x="10" y="10"/>
<button focused="" h="25" name="button2" onclick="ignore()" text="$Ignore" w="70" x="10" y="40"/>
<button focused="" h="25" name="button3" onclick="traintodepo()" text="$To-depo" w="70" x="90" y="10"/>
<button focused="" h="25" name="button4" onclick="trainreverse()" text="$Reverse" w="70" x="90" y="40"/>
<label h="15" name="state" root="" text="Label" w="70" x="10" y="150"/>
<label h="15" name="dest" root="" text="Label" w="70" x="10" y="130"/>
<button focused="" h="25" name="button5" onclick="watch()" text="$Look" w="70" x="10" y="70"/>
<label h="15" name="speed" root="" text="Label" w="70" x="90" y="150"/>
<button focused="" h="25" name="button6" onclick="remove()" text="$Delete" w="70" x="90" y="70"/>
<button focused="" h="25" name="button7" onclick="elsewhere()" text="$Elsewhere" w="70" x="90" y="100"/>
<button focused="" h="25" name="button8" onclick="drive()" text="$Drive" w="70" x="10" y="100"/>
<button focused="" h="15" name="button9" onclick="gotoxy()" text="$Place" w="70" x="90" y="130"/>
<label h="15" name="linename" root="" text="Label" w="70" x="10" y="170"/>
</window>
<window clienth="200" clientw="330" closeonesc="1" name="window6" onclose="onclose()" onok="" position="center" rep.name="jtycon.rrbldfrm" state="normal" text="$Railroad">
<button focused="" h="25" name="button1" onclick="rrbuild()" text="$Railroad" w="100" x="220" y="160"/>
<button focused="" h="25" name="button2" onclick="setdhi(-1)" text="&gt;" w="30" x="90" y="60"/>
<button focused="" h="25" name="button3" onclick="setdhi(-2)" text="&gt;&gt;" w="30" x="120" y="70"/>
<button focused="" h="25" name="button4" onclick="setdhi(-3)" text="&gt;&gt;&gt;" w="30" x="150" y="80"/>
<button focused="" h="25" name="button5" onclick="setdhi(-4)" text="&gt;&gt;&gt;&gt;" w="30" x="180" y="90"/>
<button focused="" h="25" name="button6" onclick="setdhi(0)" text="----" w="30" x="60" y="50"/>
<button focused="" h="25" name="button7" onclick="setdhi(1)" text="&lt;" w="30" x="90" y="40"/>
<button focused="" h="25" name="button8" onclick="setdhi(2)" text="&lt;&lt;" w="30" x="120" y="30"/>
<button focused="" h="25" name="button9" onclick="setdhi(3)" text="&lt;&lt;&lt;" w="30" x="150" y="20"/>
<button focused="" h="25" name="button10" onclick="setdhi(4)" text="&lt;&lt;&lt;&lt;" w="30" x="180" y="10"/>
<label h="15" name="label1" root="" text="$Change-heights" w="90" x="20" y="10"/>
<trackbar cur="50" frequency="1" h="90" max="100" min="0" name="hi" onchange="changeddhi()" vertical="1" w="30" x="20" y="30"/>
<button focused="" h="25" name="button13" onclick="blddepo()" text="$Depo" w="100" x="220" y="40"/>
<button focused="" h="25" name="button14" onclick="bldrst()" text="$Railway-station" w="100" x="220" y="100"/>
<button focused="" h="25" name="button15" onclick="bldsem()" text="$Semaphore" w="100" x="220" y="10"/>
<button focused="" h="25" name="button16" onclick="bldbridge()" text="$Bridge" w="100" x="220" y="130"/>
<button focused="" h="25" name="button17" onclick="bldtunnel()" text="$Tunnel" w="100" x="220" y="70"/>
<radiogroup cur="0" focused="" h="60" items="$max-hi;$min-hi;$fixed-hi" name="himode" onchange="" text="$Height" w="120" x="90" y="130"/>
<listbox cur="0" focused="" h="60" items="" load.items="" name="sems" onchange="" root="" saveas="index" w="70" x="10" y="130"/>
</window>
<window clienth="260" clientw="280" closeonesc="1" name="window7" onclose="" onok="" position="center" rep.name="jtycon.savefile" state="normal" text="$Save">
<listbox focused="" h="160" items="" name="files" onchange="savefilechange()" root="" w="130" x="20" y="40"/>
<button focused="" h="25" name="button1" onclick="dook()" text="$Save" w="100" x="160" y="40"/>
<button focused="" h="25" name="button2" onclick="doclose()" text="$Cancel" w="100" x="160" y="80"/>
<edit focused="1" h="25" name="filename" onchange="" root="" text="noname" w="130" x="20" y="210"/>
<label h="25" name="label1" root="" text="$Files" w="100" x="20" y="10"/>
</window>
<window clienth="220" clientw="270" closeonesc="1" name="window8" onclose="onclose()" onok="" position="center" rep.name="jtycon.linesfrm" state="normal" text="$Lines">
<listbox focused="" h="170" items="" name="lines" onchange="" root="" w="100" x="20" y="40"/>
<button focused="" h="25" name="button1" onclick="addrline()" text="$Add $train" w="100" x="130" y="40"/>
<button focused="" h="25" name="button2" onclick="eraseline()" text="$Delete" w="100" x="130" y="150"/>
<button focused="" h="25" name="button3" onclick="editline()" text="$Edit" w="100" x="130" y="180"/>
<label h="15" name="label1" root="" text="$Lines" w="100" x="20" y="20"/>
<button focused="" h="25" name="button4" onclick="addaline()" text="$Add $aircraft" w="100" x="130" y="70"/>
<button focused="" h="25" name="button5" onclick="addmline()" text="$Add $bus" w="100" x="130" y="100"/>
</window>
<window clienth="220" clientw="330" closeonesc="1" layout="1" name="window1" onclose="onclose()" onok="" position="center" rep.name="jtycon.rlinefrm" state="normal" text="$Line">
<pager>
<page text="$Common">
<button focused="" h="25" name="button5" onclick="dorename()" text="$Rename" w="100" x="20" y="20"/>
<radiogroup cur="" focused="" h="90" items="$Express;$Fast-train;$Slow-train;$Freight-train" name="priority" onchange="prichange()" root="" text="$Priority" w="100" x="20" y="60"/>
<trackbar cur="0" frequency="1" h="30" max="20" min="1" name="waitbar" onchange="waitchange()" vertical="0" w="130" x="130" y="90"/>
<label h="25" name="waitlabel" root="" text="" w="100" x="130" y="60"/>
</page>
<page text="$Stations">
<listbox focused="" h="100" items="" name="stations" onchange="changest()" root="" w="120" x="10" y="20"/>
<button focused="" h="25" name="button1" onclick="addst()" text="+" w="30" x="10" y="130"/>
<button focused="" h="25" name="button2" onclick="delst()" text="-" w="30" x="40" y="130"/>
<button focused="" h="25" name="button3" onclick="upst()" text="&lt;&lt;" w="30" x="70" y="130"/>
<button focused="" h="25" name="button4" onclick="downst()" text="&gt;&gt;" w="30" x="100" y="130"/>
<listbox cur="0" focused="" h="100" items="" load.items="" name="waypoints" onchange="changewp()" root="" saveas="index" w="120" x="150" y="20"/>
<button focused="" h="25" name="button5" onclick="addwp()" text="+" w="30" x="150" y="130"/>
<button focused="" h="25" name="button6" onclick="delwp()" text="-" w="30" x="180" y="130"/>
<button focused="" h="25" name="button7" onclick="upwp()" text="&lt;&lt;" w="30" x="210" y="130"/>
<button focused="" h="25" name="button8" onclick="downwp()" text="&gt;&gt;" w="30" x="240" y="130"/>
</page>
<page text="$Compose">
<listbox focused="" h="100" items="" name="compose" onchange="" root="" w="90" x="10" y="20"/>
<listbox focused="" h="100" items="" name="machines" onchange="" root="" w="100" x="200" y="20"/>
<button focused="" h="25" name="button1" onclick="addvagon()" text="+" w="30" x="10" y="130"/>
<button focused="" h="25" name="button2" onclick="delvagon()" text="-" w="30" x="40" y="130"/>
<button focused="" h="25" name="button3" onclick="upvagon()" text="&lt;&lt;" w="30" x="10" y="160"/>
<button focused="" h="25" name="button4" onclick="downvagon()" text="&gt;&gt;" w="30" x="40" y="160"/>
<button focused="" h="25" name="button5" onclick="reversevagon()" text="&lt;-&gt;" w="30" x="70" y="130"/>
<listbox cur="0" focused="" h="100" items="" load.items="" name="groups" onchange="groupclick()" root="" saveas="index" w="100" x="100" y="20"/>
</page>
<page text="$Trains">
<listbox cur="0" focused="" h="100" items="" load.items="" name="trains" onchange="" root="" saveas="index" w="100" x="20" y="20"/>
<button focused="" h="25" name="button1" onclick="showtrain()" text="$Show" w="100" x="20" y="130"/>
</page>
</pager>
</window>
<window clienth="160" clientw="250" closeonesc="1" name="window9" onclose="onclose()" onok="" position="center" rep.name="jtycon.terfrm" state="normal" text="$Terrain">
<button focused="" h="25" name="button1" onclick="setmode(&quot;set&quot;)" text="$Plane" w="100" x="10" y="20"/>
<button focused="" h="25" name="button2" onclick="setmode(&quot;up&quot;)" text="$Up" w="100" x="10" y="50"/>
<button focused="" h="25" name="button3" onclick="setmode(&quot;down&quot;)" text="$Down" w="100" x="10" y="80"/>
<button focused="" h="25" name="button4" onclick="setmode(&quot;get&quot;)" text="$Get-height" w="100" x="10" y="110"/>
<trackbar cur="20" frequency="1" h="100" max="100" min="0" name="dhi" onchange="" vertical="1" w="40" x="130" y="40"/>
<trackbar cur="40" frequency="1" h="100" max="100" min="0" name="mhd" onchange="" vertical="1" w="40" x="180" y="40"/>
<label h="15" name="label1" root="" text="dH" w="30" x="130" y="20"/>
<label h="15" name="label2" root="" text="MHD" w="30" x="180" y="20"/>
</window>
<window clienth="180" clientw="230" closeonesc="1" name="window10" onclose="onclose()" onok="" position="center" rep.name="jtycon.tergenfrm" state="normal" text="$Terrain">
<button focused="" h="25" name="button1" onclick="clear()" text="$Plane" w="80" x="10" y="10"/>
<button focused="" h="25" name="button2" onclick="circles()" text="$Circles" w="80" x="10" y="40"/>
<trackbar cur="10" frequency="1" h="80" max="100" min="0" name="count" onchange="" vertical="1" w="30" x="10" y="90"/>
<label h="15" name="label1" root="" text="$Count" w="40" x="10" y="70"/>
<trackbar cur="5" frequency="1" h="80" max="20" min="1" name="size" onchange="" vertical="1" w="30" x="80" y="90"/>
<label h="15" name="label2" root="" text="$Size" w="40" x="80" y="70"/>
<trackbar cur="20" frequency="1" h="80" max="100" min="1" name="height" onchange="" vertical="1" w="30" x="150" y="90"/>
<label h="15" name="label3" root="" text="$Height" w="40" x="150" y="70"/>
</window>
<window clienth="100" clientw="240" closeonesc="1" name="window11" onclose="" onok="" position="center" rep.name="jtycon.exitfrm" state="normal" text="$Query">
<button focused="" h="25" name="button1" onclick="exit()" text="$Quit" w="100" x="20" y="60"/>
<button focused="" h="25" name="button2" onclick="close()" text="$Cancel" w="100" x="120" y="60"/>
<label h="25" name="label1" root="" text="$Quit?" w="100" x="20" y="20"/>
</window>
<window clienth="100" clientw="240" closeonesc="1" name="window11" onclose="" onok="" position="center" rep.name="jtycon.quitfrm" state="normal" text="$Query">
<button focused="" h="25" name="button1" onclick="quit()" text="$Yes" w="100" x="20" y="60"/>
<button focused="" h="25" name="button2" onclick="close()" text="$Cancel" w="100" x="120" y="60"/>
<label h="25" name="label1" root="" text="$Quit-to-os" w="100" x="20" y="20"/>
</window>
<window clienth="90" clientw="140" closeonesc="1" name="window12" onclose="onclose()" onok="" position="center" rep.name="jtycon.waterfrm" state="normal" text="$Water">
<button focused="" h="25" name="button1" onclick="setwater()" text="$Make-water" w="100" x="20" y="20"/>
<button focused="" h="25" name="button2" onclick="setunwater()" text="$Make-dry" w="100" x="20" y="50"/>
</window>
<window clienth="170" clientw="230" closeonesc="1" name="window1" onclose="onclose()" onok="" position="center" rep.name="jtycon.bridgefrm" state="normal" text="$Bridge">
<listbox focused="" h="110" items="" name="types" onchange="" root="" w="90" x="20" y="40"/>
<label h="25" name="label1" root="" text="$Bridge-types" w="100" x="20" y="10"/>
<button focused="" h="25" name="button1" onclick="rebuild()" text="$Rebuild" w="100" x="120" y="40"/>
</window>
<window clienth="150" clientw="130" closeonesc="1" name="window13" onclose="onclose()" onok="" position="center" rep.name="jtycon.treefrm" state="normal" text="$Planting-trees">
<listbox focused="" h="110" items="" name="list" onchange="setstate()" root="" w="90" x="20" y="20"/>
</window>
<window clienth="170" clientw="150" closeonesc="1" name="window14" onclose="onclose()" onok="" position="center" rep.name="jtycon.bldfrm" state="normal" text="$Building">
<listbox cur="0" focused="" h="130" items="" load.items="" name="types" onchange="changetype()" root="" saveas="index" w="110" x="20" y="20"/>
</window>
<window clienth="130" clientw="320" closeonesc="1" name="window15" onclose="" onok="" position="center" rep.name="jtycon.oglfrm" state="normal" text="">
<button focused="" h="25" name="button1" onclick="@ok" text="$OK" w="100" x="50" y="90"/>
<button focused="" h="25" name="button2" onclick="@cancel" text="$Cancel" w="100" x="160" y="90"/>
<radiogroup cur="" focused="" h="50" items="$No;$Yes" name="radiogroup1" onchange="" root="@lighting" text="$Light" w="120" x="20" y="20"/>
</window>
<window clienth="170" clientw="150" closeonesc="1" name="window16" onclose="onclose()" onok="" position="center" rep.name="jtycon.tertypefrm" state="normal" text="$Terrain">
<listbox cur="0" focused="" h="140" items="" load.items="" name="list" onchange="setstate()" root="" saveas="index" w="110" x="20" y="10"/>
</window>
<window clienth="190" clientw="150" closeonesc="1" name="window17" onclose="onclose()" onok="" position="center" rep.name="jtycon.railtypefrm" state="normal" text="$Rail-type">
<listbox cur="0" focused="" h="150" items="" load.items="" name="list" onchange="setstate()" root="" saveas="index" w="110" x="20" y="20"/>
</window>
<window clienth="130" clientw="250" closeonesc="1" name="window18" onclose="onclose()" onok="" position="center" rep.name="jtycon.airportfrm" state="normal" text="$Airport">
<button focused="" h="25" name="button1" onclick="bldairport()" text="$Airport" w="100" x="20" y="20"/>
</window>
<window clienth="180" clientw="150" closeonesc="1" name="window19" onclose="onclose()" onok="" position="center" rep.name="jtycon.airportwin" state="normal" text="$Airport">
<button focused="" h="25" name="button1" onclick="newaircraft()" text="$New-aircraft" w="100" x="20" y="140"/>
<listbox cur="0" focused="" h="100" items="" load.items="" name="lines" onchange="" root="" saveas="index" w="100" x="20" y="20"/>
</window>
<window clienth="190" clientw="160" closeonesc="1" name="window20" onclose="onclose()" onok="" position="center" rep.name="jtycon.alinefrm" state="normal" text="">
<listbox cur="0" focused="" h="110" items="" load.items="" name="stations" onchange="" root="" saveas="index" w="120" x="20" y="20"/>
<button focused="" h="25" name="button1" onclick="addst()" text="+" w="30" x="20" y="140"/>
<button focused="" h="25" name="button2" onclick="delst()" text="-" w="30" x="50" y="140"/>
<button focused="" h="25" name="button3" onclick="upst()" text="&lt;&lt;" w="30" x="80" y="140"/>
<button focused="" h="25" name="button4" onclick="downst()" text="&gt;&gt;" w="30" x="110" y="140"/>
</window>
<window clienth="260" clientw="250" closeonesc="1" name="window21" onclose="onclose()" onok="" position="center" rep.name="jtycon.newgamefrm" state="normal" text="$Terrain">
<button focused="" h="25" name="button1" onclick="makeok()" text="$OK" w="100" x="20" y="220"/>
<button focused="" h="25" name="button2" onclick="makecancel()" text="$Cancel" w="100" x="130" y="220"/>
<radiogroup cur="" focused="" h="120" items="$Small 100x100;$Medium 200x200;$Big 500x500;$Giant 1000x10000;$Custom" name="size" onchange="" root="" text="$Size" w="210" x="20" y="20"/>
<edit focused="1" h="25" name="w" onchange="" root="" text="100" w="100" x="120" y="150"/>
<edit focused="1" h="25" name="h" onchange="" root="" text="100" w="100" x="120" y="180"/>
<label h="25" name="label1" root="" text="$Width" w="80" x="30" y="150"/>
<label h="25" name="label2" root="" text="$Height" w="80" x="30" y="180"/>
</window>
<window clienth="170" clientw="150" closeonesc="1" name="window22" onclose="onclose()" onok="" position="center" rep.name="jtycon.tercreatfrm" state="normal" text="$Terrain">
<button focused="" h="25" name="button1" onclick="mountains()" text="$Mountains" w="100" x="20" y="20"/>
<trackbar cur="0" frequency="1" h="90" max="100" min="0" name="size" onchange="changed()" vertical="1" w="30" x="20" y="70"/>
<trackbar cur="0" frequency="1" h="90" max="100" min="0" name="height" onchange="changed()" vertical="1" w="30" x="80" y="70"/>
<label h="15" name="label1" root="" text="$Size" w="40" x="20" y="50"/>
<label h="15" name="label2" root="" text="$Height" w="40" x="80" y="50"/>
</window>
<window clienth="260" clientw="220" closeonesc="1" name="window23" onclose="onclose()" onok="" position="center" rep.name="jtycon.trainsfrm" state="normal" text="$Trains">
<listbox cur="0" focused="" h="200" items="" load.items="" name="trains" onchange="" root="" saveas="index" w="180" x="20" y="20"/>
<button focused="" h="25" name="button1" onclick="showtrain()" text="$Show" w="80" x="20" y="230"/>
<button focused="" h="25" name="button2" onclick="update()" text="$Update" w="80" x="120" y="230"/>
</window>
<window clienth="200" clientw="280" closeonesc="1" name="window24" onclose="onclose()" onok="" position="center" rep.name="jtycon.rrtemplatefrm" state="normal" text="$Railroad-template">
<listbox cur="0" focused="" h="150" items="" load.items="" name="tpls" onchange="" root="" saveas="index" w="120" x="20" y="20"/>
<button focused="" h="25" name="button1" onclick="rename()" text="$Rename" w="100" x="160" y="20"/>
<button focused="" h="25" name="button2" onclick="newtpl()" text="$Add" w="100" x="160" y="50"/>
<button focused="" h="25" name="button3" onclick="remove()" text="$Delete" w="100" x="160" y="80"/>
<button focused="" h="25" name="button4" onclick="usetpl()" text="$Use" w="100" x="160" y="110"/>
</window>
<window clienth="230" clientw="340" closeonesc="1" name="window25" onclose="onclose()" onok="" position="center" rep.name="jtycon.roadfrm" state="normal" text="$Road">
<button focused="" h="25" name="button1" onclick="roadbuild()" text="$Road" w="100" x="220" y="10"/>
<button focused="" h="25" name="button2" onclick="setdhi(-1)" text="&gt;" w="30" x="90" y="60"/>
<button focused="" h="25" name="button3" onclick="setdhi(-2)" text="&gt;&gt;" w="30" x="120" y="70"/>
<button focused="" h="25" name="button4" onclick="setdhi(-3)" text="&gt;&gt;&gt;" w="30" x="150" y="80"/>
<button focused="" h="25" name="button5" onclick="setdhi(-4)" text="&gt;&gt;&gt;&gt;" w="30" x="180" y="90"/>
<button focused="" h="25" name="button6" onclick="setdhi(0)" text="----" w="30" x="60" y="50"/>
<button focused="" h="25" name="button7" onclick="setdhi(1)" text="&lt;" w="30" x="90" y="40"/>
<button focused="" h="25" name="button8" onclick="setdhi(2)" text="&lt;&lt;" w="30" x="120" y="30"/>
<button focused="" h="25" name="button9" onclick="setdhi(3)" text="&lt;&lt;&lt;" w="30" x="150" y="20"/>
<button focused="" h="25" name="button10" onclick="setdhi(4)" text="&lt;&lt;&lt;&lt;" w="30" x="180" y="10"/>
<label h="15" name="label1" root="" text="$Change-heights" w="90" x="20" y="10"/>
<trackbar cur="50" frequency="1" h="90" max="100" min="0" name="hi" onchange="changeddhi()" vertical="1" w="30" x="20" y="30"/>
<radiogroup cur="" focused="" h="70" items="$max-hi;$min-hi;$fixed-hi" name="himode" onchange="" root="" text="$Height" w="110" x="20" y="140"/>
<button focused="" h="25" name="button16" onclick="bldbridge()" text="$Bridge" w="100" x="220" y="70"/>
<button focused="" h="25" name="button17" onclick="bldtunnel()" text="$Tunnel" w="100" x="220" y="40"/>
<button focused="" h="25" name="button11" onclick="bldbusstop()" text="$Bus-stop" w="100" x="220" y="100"/>
</window>
<window clienth="190" clientw="280" closeonesc="1" name="window20" onclose="onclose()" onok="" position="center" rep.name="jtycon.mlinefrm" state="normal" text="">
<listbox cur="0" focused="" h="110" items="" load.items="" name="stations" onchange="" root="" saveas="index" w="120" x="20" y="20"/>
<button focused="" h="25" name="button1" onclick="addst()" text="+" w="30" x="20" y="140"/>
<button focused="" h="25" name="button2" onclick="delst()" text="-" w="30" x="50" y="140"/>
<button focused="" h="25" name="button3" onclick="upst()" text="&lt;&lt;" w="30" x="80" y="140"/>
<button focused="" h="25" name="button4" onclick="downst()" text="&gt;&gt;" w="30" x="110" y="140"/>
<listbox cur="0" focused="" h="110" items="" load.items="" name="types" onchange="changetype()" root="" saveas="" w="100" x="150" y="20"/>
</window>
<window clienth="200" clientw="250" closeonesc="1" name="window19" onclose="onclose()" onok="" position="center" rep.name="jtycon.busstopwin" state="normal" text="$Airport">
<button focused="" h="25" name="button1" onclick="newmotocar()" text="$New-motocar" w="100" x="20" y="130"/>
<listbox cur="0" focused="" h="100" items="" load.items="" name="lines" onchange="" root="" saveas="index" w="100" x="20" y="20"/>
<button focused="" h="25" name="button2" onclick="dorename()" text="$Rename" w="100" x="20" y="160"/>
<listbox cur="0" focused="" h="100" items="" load.items="" name="types" onchange="" root="" saveas="index" w="100" x="130" y="20"/>
<button focused="" h="25" name="button3" onclick="rebuild()" text="$Rebuild" w="100" x="130" y="130"/>
</window>
<window clienth="160" clientw="230" closeonesc="1" name="window26" onclose="onclose()" onok="" position="center" rep.name="jtycon.motocarfrm" state="normal" text="$Motocar">
<button focused="" h="25" name="button1" onclick="watch()" text="$Look" w="80" x="10" y="10"/>
<button focused="" h="25" name="button3" onclick="remove()" text="$Delete" w="80" x="10" y="70"/>
<button focused="" h="25" name="button4" onclick="elsewhere()" text="$Elsewhere" w="80" x="10" y="100"/>
<label h="15" name="dest" root="" text="Label" w="100" x="100" y="10"/>
<label h="15" name="state" root="" text="Label" w="100" x="100" y="30"/>
<label h="15" name="speed" root="" text="Label" w="100" x="100" y="50"/>
<label h="15" name="linename" root="" text="Label" w="100" x="100" y="70"/>
<button focused="" h="25" name="button5" onclick="ignore()" text="$Ignore" w="100" x="100" y="100"/>
<button focused="" h="25" name="button6" onclick="startstop()" text="$Start/$Stop" w="80" x="10" y="130"/>
</window>
<window clienth="180" clientw="160" closeonesc="1" name="window27" onclose="onclose()" onok="" position="center" rep.name="jtycon.roadtypefrm" state="normal" text="$Road">
<listbox cur="0" focused="" h="140" items="" load.items="" name="list" onchange="setstate()" root="" saveas="index" w="120" x="20" y="20"/>
</window>
<window clienth="170" clientw="260" closeonesc="1" name="window28" onclose="" onok="" position="center" rep.name="jtycon.tunnelfrm" state="normal" text="$Tunnel">
<button focused="" h="25" name="button1" onclick="rebuild()" text="$Rebuild" w="100" x="140" y="20"/>
<listbox cur="0" focused="" h="130" items="" load.items="" name="types" onchange="" root="" saveas="index" w="100" x="20" y="20"/>
</window>
</data>
