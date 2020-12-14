<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.6.2">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="pinhead2">
<description>&lt;b&gt;Pin Header Connectors&lt;/b&gt;&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="1X02" urn="urn:adsk.eagle:footprint:22309/1" locally_modified="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-1.905" y1="1.27" x2="-0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="1.27" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="0.635" x2="0" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="-0.635" x2="-0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0.635" x2="-2.54" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="1.27" x2="-2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-0.635" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-1.27" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0" y1="0.635" x2="0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="1.27" x2="1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="1.27" x2="2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="0.635" x2="2.54" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-0.635" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-1.27" x2="0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="-1.27" x2="0" y2="-0.635" width="0.1524" layer="21"/>
<pad name="1" x="-1.27" y="0" drill="1.016" diameter="1.9812" shape="octagon" rot="R90"/>
<pad name="2" x="1.27" y="0" drill="1.016" diameter="1.9812" shape="octagon" rot="R90"/>
<text x="-2.6162" y="1.8288" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.54" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.524" y1="-0.254" x2="-1.016" y2="0.254" layer="51"/>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51"/>
</package>
<package name="1X02/90" urn="urn:adsk.eagle:footprint:22310/1" locally_modified="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-2.54" y1="-5.08" x2="0" y2="-5.08" width="0.1524" layer="21"/>
<wire x1="0" y1="-5.08" x2="0" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="0" y1="-2.54" x2="-2.54" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-2.54" x2="-2.54" y2="-5.08" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="3.81" x2="-1.27" y2="-1.905" width="0.762" layer="21"/>
<wire x1="0" y1="-5.08" x2="2.54" y2="-5.08" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-5.08" x2="2.54" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-2.54" x2="0" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="1.27" y1="3.81" x2="1.27" y2="-1.905" width="0.762" layer="21"/>
<pad name="1" x="-1.27" y="-3.81" drill="1.016" diameter="1.9812" shape="octagon" rot="R90"/>
<pad name="2" x="1.27" y="-3.81" drill="1.016" diameter="2.0066" shape="octagon" rot="R90"/>
<text x="-3.175" y="-3.81" size="1.27" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="4.445" y="-3.81" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-1.651" y1="-2.54" x2="-0.889" y2="-2.032" layer="21"/>
<rectangle x1="0.889" y1="-2.54" x2="1.651" y2="-2.032" layer="21"/>
</package>
<package name="1X03" urn="urn:adsk.eagle:footprint:22340/1" locally_modified="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-3.175" y1="1.27" x2="-1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="1.27" x2="-1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="0.635" x2="-1.27" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-0.635" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="0.635" x2="-0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="1.27" x2="0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="1.27" x2="1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="0.635" x2="1.27" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-0.635" x2="0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="-1.27" x2="-0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-1.27" x2="-1.27" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="0.635" x2="-3.81" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="1.27" x2="-3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="-0.635" x2="-3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-1.27" x2="-3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.27" y1="0.635" x2="1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="1.27" x2="3.175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="1.27" x2="3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="3.81" y1="0.635" x2="3.81" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="3.81" y1="-0.635" x2="3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-1.27" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-1.27" x2="1.27" y2="-0.635" width="0.1524" layer="21"/>
<pad name="1" x="-2.54" y="0" drill="1.016" diameter="1.9812" shape="octagon" rot="R90"/>
<pad name="2" x="0" y="0" drill="1.016" diameter="1.9812" shape="octagon" rot="R90"/>
<pad name="3" x="2.54" y="0" drill="1.016" diameter="1.9812" shape="octagon" rot="R90"/>
<text x="-3.8862" y="1.8288" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.81" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-0.254" y1="-0.254" x2="0.254" y2="0.254" layer="51"/>
<rectangle x1="-2.794" y1="-0.254" x2="-2.286" y2="0.254" layer="51"/>
<rectangle x1="2.286" y1="-0.254" x2="2.794" y2="0.254" layer="51"/>
</package>
<package name="1X03/90" urn="urn:adsk.eagle:footprint:22341/1" locally_modified="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-3.81" y1="-5.08" x2="-1.27" y2="-5.08" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-5.08" x2="-1.27" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-2.54" x2="-3.81" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="-2.54" x2="-3.81" y2="-5.08" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="3.81" x2="-2.54" y2="-1.905" width="0.762" layer="21"/>
<wire x1="-1.27" y1="-5.08" x2="1.27" y2="-5.08" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-5.08" x2="1.27" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-2.54" x2="-1.27" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="0" y1="3.81" x2="0" y2="-1.905" width="0.762" layer="21"/>
<wire x1="1.27" y1="-5.08" x2="3.81" y2="-5.08" width="0.1524" layer="21"/>
<wire x1="3.81" y1="-5.08" x2="3.81" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="3.81" y1="-2.54" x2="1.27" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="2.54" y1="3.81" x2="2.54" y2="-1.905" width="0.762" layer="21"/>
<pad name="1" x="-2.54" y="-3.81" drill="1.016" diameter="1.9812" shape="octagon" rot="R90"/>
<pad name="2" x="0" y="-3.81" drill="1.016" diameter="1.9812" shape="octagon" rot="R90"/>
<pad name="3" x="2.54" y="-3.81" drill="1.016" diameter="1.9812" shape="octagon" rot="R90"/>
<text x="-4.445" y="-3.81" size="1.27" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="5.715" y="-3.81" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-2.921" y1="-2.54" x2="-2.159" y2="-2.032" layer="21"/>
<rectangle x1="-0.381" y1="-2.54" x2="0.381" y2="-2.032" layer="21"/>
<rectangle x1="2.159" y1="-2.54" x2="2.921" y2="-2.032" layer="21"/>
</package>
<package name="1X03/90F">
<wire x1="-2.54" y1="1.12" x2="-2.54" y2="0.405" width="0.762" layer="21"/>
<wire x1="0" y1="1.12" x2="0" y2="0.405" width="0.762" layer="21"/>
<wire x1="2.54" y1="1.12" x2="2.54" y2="0.405" width="0.762" layer="21"/>
<pad name="1" x="-2.54" y="0" drill="1.016" diameter="1.9812" shape="octagon" rot="R90"/>
<pad name="2" x="0" y="0" drill="1.016" diameter="1.9812" shape="octagon" rot="R90"/>
<pad name="3" x="2.54" y="0" drill="1.016" diameter="1.9812" shape="octagon" rot="R90"/>
<text x="-4.445" y="0" size="1.27" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="5.715" y="0" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-2.921" y1="1.27" x2="-2.159" y2="1.778" layer="21"/>
<rectangle x1="-0.381" y1="1.27" x2="0.381" y2="1.778" layer="21"/>
<rectangle x1="2.159" y1="1.27" x2="2.921" y2="1.778" layer="21"/>
<wire x1="-4" y1="1.5" x2="-4" y2="9.5" width="0.127" layer="21"/>
<wire x1="-4" y1="9.5" x2="4" y2="9.5" width="0.127" layer="21"/>
<wire x1="4" y1="9.5" x2="4" y2="1.5" width="0.127" layer="21"/>
<wire x1="4" y1="1.5" x2="-4" y2="1.5" width="0.127" layer="21"/>
</package>
<package name="1X02/90F">
<wire x1="-1.31" y1="1.12" x2="-1.31" y2="0.024" width="0.762" layer="21"/>
<wire x1="1.23" y1="1.12" x2="1.23" y2="0.024" width="0.762" layer="21"/>
<pad name="1" x="-1.31" y="0" drill="1.016" diameter="1.9812" shape="octagon" rot="R90"/>
<pad name="2" x="1.23" y="0" drill="1.016" diameter="1.9812" shape="octagon" rot="R90"/>
<text x="-3.215" y="0" size="1.27" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<rectangle x1="-1.691" y1="1.27" x2="-0.929" y2="1.778" layer="21"/>
<rectangle x1="0.849" y1="1.27" x2="1.611" y2="1.778" layer="21"/>
<wire x1="-2.77" y1="1.5" x2="-2.77" y2="9.5" width="0.127" layer="21"/>
<wire x1="-2.77" y1="9.5" x2="2.73" y2="9.5" width="0.127" layer="21"/>
<wire x1="2.73" y1="9.5" x2="2.73" y2="1.5" width="0.127" layer="21"/>
<wire x1="2.73" y1="1.5" x2="-2.77" y2="1.5" width="0.127" layer="21"/>
<text x="4.285" y="0" size="1.27" layer="27" ratio="10" rot="R90">&gt;VALUE</text>
</package>
<package name="2X02" urn="urn:adsk.eagle:footprint:22337/1" locally_modified="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-2.54" y1="-1.905" x2="-1.905" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-2.54" x2="0" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="0" y1="-1.905" x2="0.635" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-2.54" x2="2.54" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-1.905" x2="-2.54" y2="1.905" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="1.905" x2="-1.905" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="2.54" x2="-0.635" y2="2.54" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="2.54" x2="0" y2="1.905" width="0.1524" layer="21"/>
<wire x1="0" y1="1.905" x2="0.635" y2="2.54" width="0.1524" layer="21"/>
<wire x1="0.635" y1="2.54" x2="1.905" y2="2.54" width="0.1524" layer="21"/>
<wire x1="1.905" y1="2.54" x2="2.54" y2="1.905" width="0.1524" layer="21"/>
<wire x1="0" y1="1.905" x2="0" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="2.54" y1="1.905" x2="2.54" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="0.635" y1="-2.54" x2="1.905" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-2.54" x2="-0.635" y2="-2.54" width="0.1524" layer="21"/>
<pad name="1" x="-1.27" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="2" x="-1.27" y="1.27" drill="1.016" shape="octagon"/>
<pad name="3" x="1.27" y="-1.27" drill="1.016" shape="octagon"/>
<pad name="4" x="1.27" y="1.27" drill="1.016" shape="octagon"/>
<text x="-2.54" y="3.175" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-2.54" y="-4.445" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="-1.524" y1="-1.524" x2="-1.016" y2="-1.016" layer="51"/>
<rectangle x1="-1.524" y1="1.016" x2="-1.016" y2="1.524" layer="51"/>
<rectangle x1="1.016" y1="1.016" x2="1.524" y2="1.524" layer="51"/>
<rectangle x1="1.016" y1="-1.524" x2="1.524" y2="-1.016" layer="51"/>
</package>
<package name="2X02/90" urn="urn:adsk.eagle:footprint:22338/1">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-2.54" y1="-1.905" x2="0" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="0" y1="-1.905" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="0.635" x2="-2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0.635" x2="-2.54" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="6.985" x2="-1.27" y2="1.27" width="0.762" layer="21"/>
<wire x1="0" y1="-1.905" x2="2.54" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-1.905" x2="2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="0.635" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="6.985" x2="1.27" y2="1.27" width="0.762" layer="21"/>
<pad name="2" x="-1.27" y="-3.81" drill="1.016" shape="octagon"/>
<pad name="4" x="1.27" y="-3.81" drill="1.016" shape="octagon"/>
<pad name="1" x="-1.27" y="-6.35" drill="1.016" shape="octagon"/>
<pad name="3" x="1.27" y="-6.35" drill="1.016" shape="octagon"/>
<text x="-3.175" y="-3.81" size="1.27" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="4.445" y="-3.81" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-1.651" y1="0.635" x2="-0.889" y2="1.143" layer="21"/>
<rectangle x1="0.889" y1="0.635" x2="1.651" y2="1.143" layer="21"/>
<rectangle x1="-1.651" y1="-2.921" x2="-0.889" y2="-1.905" layer="21"/>
<rectangle x1="0.889" y1="-2.921" x2="1.651" y2="-1.905" layer="21"/>
<rectangle x1="-1.651" y1="-5.461" x2="-0.889" y2="-4.699" layer="21"/>
<rectangle x1="-1.651" y1="-4.699" x2="-0.889" y2="-2.921" layer="51"/>
<rectangle x1="0.889" y1="-4.699" x2="1.651" y2="-2.921" layer="51"/>
<rectangle x1="0.889" y1="-5.461" x2="1.651" y2="-4.699" layer="21"/>
</package>
</packages>
<packages3d>
<package3d name="1X02" urn="urn:adsk.eagle:package:22435/2" type="model">
<description>PIN HEADER</description>
<packageinstances>
<packageinstance name="1X02"/>
</packageinstances>
</package3d>
<package3d name="1X02/90" urn="urn:adsk.eagle:package:22437/2" type="model">
<description>PIN HEADER</description>
<packageinstances>
<packageinstance name="1X02/90"/>
</packageinstances>
</package3d>
<package3d name="1X03" urn="urn:adsk.eagle:package:22458/2" type="model">
<description>PIN HEADER</description>
<packageinstances>
<packageinstance name="1X03"/>
</packageinstances>
</package3d>
<package3d name="1X03/90" urn="urn:adsk.eagle:package:22459/2" type="model">
<description>PIN HEADER</description>
<packageinstances>
<packageinstance name="1X03/90"/>
</packageinstances>
</package3d>
<package3d name="2X02" urn="urn:adsk.eagle:package:22455/2" type="model">
<description>PIN HEADER</description>
<packageinstances>
<packageinstance name="2X02"/>
</packageinstances>
</package3d>
<package3d name="2X02/90" urn="urn:adsk.eagle:package:22456/2" type="model">
<description>PIN HEADER</description>
<packageinstances>
<packageinstance name="2X02/90"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="PINHD2">
<wire x1="-6.35" y1="-2.54" x2="1.27" y2="-2.54" width="0.4064" layer="94"/>
<wire x1="1.27" y1="-2.54" x2="1.27" y2="5.08" width="0.4064" layer="94"/>
<wire x1="1.27" y1="5.08" x2="-6.35" y2="5.08" width="0.4064" layer="94"/>
<wire x1="-6.35" y1="5.08" x2="-6.35" y2="-2.54" width="0.4064" layer="94"/>
<text x="-6.35" y="5.715" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-5.08" size="1.778" layer="96">&gt;VALUE</text>
<pin name="1" x="-2.54" y="2.54" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="2" x="-2.54" y="0" visible="pad" length="short" direction="pas" function="dot"/>
</symbol>
<symbol name="PINHD3">
<wire x1="-6.35" y1="-5.08" x2="1.27" y2="-5.08" width="0.4064" layer="94"/>
<wire x1="1.27" y1="-5.08" x2="1.27" y2="5.08" width="0.4064" layer="94"/>
<wire x1="1.27" y1="5.08" x2="-6.35" y2="5.08" width="0.4064" layer="94"/>
<wire x1="-6.35" y1="5.08" x2="-6.35" y2="-5.08" width="0.4064" layer="94"/>
<text x="-6.35" y="5.715" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-7.62" size="1.778" layer="96">&gt;VALUE</text>
<pin name="1" x="-2.54" y="2.54" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="2" x="-2.54" y="0" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="3" x="-2.54" y="-2.54" visible="pad" length="short" direction="pas" function="dot"/>
</symbol>
<symbol name="PINH2X2">
<wire x1="-8.89" y1="-2.54" x2="6.35" y2="-2.54" width="0.4064" layer="94"/>
<wire x1="6.35" y1="-2.54" x2="6.35" y2="5.08" width="0.4064" layer="94"/>
<wire x1="6.35" y1="5.08" x2="-8.89" y2="5.08" width="0.4064" layer="94"/>
<wire x1="-8.89" y1="5.08" x2="-8.89" y2="-2.54" width="0.4064" layer="94"/>
<text x="-8.89" y="5.715" size="1.778" layer="95">&gt;NAME</text>
<text x="-8.89" y="-5.08" size="1.778" layer="96">&gt;VALUE</text>
<pin name="1" x="-5.08" y="2.54" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="2" x="2.54" y="2.54" visible="pad" length="short" direction="pas" function="dot" rot="R180"/>
<pin name="3" x="-5.08" y="0" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="4" x="2.54" y="0" visible="pad" length="short" direction="pas" function="dot" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="PINHD-1X2" prefix="JP" uservalue="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<gates>
<gate name="G$1" symbol="PINHD2" x="0" y="0"/>
</gates>
<devices>
<device name="" package="1X02">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:22435/2"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="98" constant="no"/>
</technology>
</technologies>
</device>
<device name="/90" package="1X02/90">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:22437/2"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="24" constant="no"/>
</technology>
</technologies>
</device>
<device name="1X02/90F" package="1X02/90F">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="PINHD-1X3" prefix="JP" uservalue="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<gates>
<gate name="A" symbol="PINHD3" x="0" y="0"/>
</gates>
<devices>
<device name="" package="1X03">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:22458/2"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="92" constant="no"/>
</technology>
</technologies>
</device>
<device name="/90" package="1X03/90">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:22459/2"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="17" constant="no"/>
</technology>
</technologies>
</device>
<device name="1X03/90F" package="1X03/90F">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="PINHD-2X2" prefix="JP" uservalue="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<gates>
<gate name="A" symbol="PINH2X2" x="0" y="0"/>
</gates>
<devices>
<device name="" package="2X02">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
<connect gate="A" pin="4" pad="4"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:22455/2"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="0" constant="no"/>
</technology>
</technologies>
</device>
<device name="/90" package="2X02/90">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
<connect gate="A" pin="4" pad="4"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:22456/2"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="0" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="JP7" library="pinhead2" deviceset="PINHD-1X2" device="/90" package3d_urn="urn:adsk.eagle:package:22437/2"/>
<part name="JP8" library="pinhead2" deviceset="PINHD-1X3" device="/90" package3d_urn="urn:adsk.eagle:package:22459/2"/>
<part name="JP9" library="pinhead2" deviceset="PINHD-1X3" device="1X03/90F"/>
<part name="JP10" library="pinhead2" deviceset="PINHD-1X2" device="1X02/90F"/>
<part name="JP11" library="pinhead2" deviceset="PINHD-2X2" device="" package3d_urn="urn:adsk.eagle:package:22455/2"/>
<part name="JP12" library="pinhead2" deviceset="PINHD-2X2" device="" package3d_urn="urn:adsk.eagle:package:22455/2"/>
<part name="JP13" library="pinhead2" deviceset="PINHD-2X2" device="" package3d_urn="urn:adsk.eagle:package:22455/2"/>
<part name="JP14" library="pinhead2" deviceset="PINHD-2X2" device="" package3d_urn="urn:adsk.eagle:package:22455/2"/>
<part name="JP1" library="pinhead2" deviceset="PINHD-2X2" device="" package3d_urn="urn:adsk.eagle:package:22455/2"/>
<part name="JP2" library="pinhead2" deviceset="PINHD-2X2" device="" package3d_urn="urn:adsk.eagle:package:22455/2"/>
<part name="JP3" library="pinhead2" deviceset="PINHD-2X2" device="" package3d_urn="urn:adsk.eagle:package:22455/2"/>
<part name="JP4" library="pinhead2" deviceset="PINHD-2X2" device="" package3d_urn="urn:adsk.eagle:package:22455/2"/>
<part name="JP5" library="pinhead2" deviceset="PINHD-2X2" device="" package3d_urn="urn:adsk.eagle:package:22455/2"/>
<part name="JP6" library="pinhead2" deviceset="PINHD-2X2" device="" package3d_urn="urn:adsk.eagle:package:22455/2"/>
<part name="JP15" library="pinhead2" deviceset="PINHD-2X2" device="" package3d_urn="urn:adsk.eagle:package:22455/2"/>
<part name="JP16" library="pinhead2" deviceset="PINHD-2X2" device="" package3d_urn="urn:adsk.eagle:package:22455/2"/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="JP7" gate="G$1" x="83.82" y="7.62" smashed="yes">
<attribute name="NAME" x="77.47" y="13.335" size="1.778" layer="95"/>
<attribute name="VALUE" x="77.47" y="2.54" size="1.778" layer="96"/>
</instance>
<instance part="JP8" gate="A" x="83.82" y="-27.94" smashed="yes">
<attribute name="NAME" x="77.47" y="-22.225" size="1.778" layer="95"/>
<attribute name="VALUE" x="77.47" y="-35.56" size="1.778" layer="96"/>
</instance>
<instance part="JP9" gate="A" x="0" y="-27.94" smashed="yes" rot="R180">
<attribute name="NAME" x="6.35" y="-33.655" size="1.778" layer="95" rot="R180"/>
<attribute name="VALUE" x="6.35" y="-20.32" size="1.778" layer="96" rot="R180"/>
</instance>
<instance part="JP10" gate="G$1" x="0" y="10.16" smashed="yes" rot="R180">
<attribute name="NAME" x="6.35" y="4.445" size="1.778" layer="95" rot="R180"/>
<attribute name="VALUE" x="6.35" y="15.24" size="1.778" layer="96" rot="R180"/>
</instance>
<instance part="JP11" gate="A" x="33.02" y="33.02" smashed="yes">
<attribute name="NAME" x="24.13" y="38.735" size="1.778" layer="95"/>
<attribute name="VALUE" x="24.13" y="27.94" size="1.778" layer="96"/>
</instance>
<instance part="JP12" gate="A" x="33.02" y="25.4" smashed="yes">
<attribute name="NAME" x="24.13" y="31.115" size="1.778" layer="95"/>
<attribute name="VALUE" x="24.13" y="20.32" size="1.778" layer="96"/>
</instance>
<instance part="JP13" gate="A" x="50.8" y="33.02" smashed="yes">
<attribute name="NAME" x="41.91" y="38.735" size="1.778" layer="95"/>
<attribute name="VALUE" x="41.91" y="27.94" size="1.778" layer="96"/>
</instance>
<instance part="JP14" gate="A" x="50.8" y="25.4" smashed="yes">
<attribute name="NAME" x="41.91" y="31.115" size="1.778" layer="95"/>
<attribute name="VALUE" x="41.91" y="20.32" size="1.778" layer="96"/>
</instance>
<instance part="JP1" gate="A" x="33.02" y="-7.62" smashed="yes">
<attribute name="NAME" x="24.13" y="-1.905" size="1.778" layer="95"/>
<attribute name="VALUE" x="24.13" y="-12.7" size="1.778" layer="96"/>
</instance>
<instance part="JP2" gate="A" x="33.02" y="-15.24" smashed="yes">
<attribute name="NAME" x="24.13" y="-9.525" size="1.778" layer="95"/>
<attribute name="VALUE" x="24.13" y="-20.32" size="1.778" layer="96"/>
</instance>
<instance part="JP3" gate="A" x="50.8" y="-7.62" smashed="yes">
<attribute name="NAME" x="41.91" y="-1.905" size="1.778" layer="95"/>
<attribute name="VALUE" x="41.91" y="-12.7" size="1.778" layer="96"/>
</instance>
<instance part="JP4" gate="A" x="50.8" y="-15.24" smashed="yes">
<attribute name="NAME" x="41.91" y="-9.525" size="1.778" layer="95"/>
<attribute name="VALUE" x="41.91" y="-20.32" size="1.778" layer="96"/>
</instance>
<instance part="JP5" gate="A" x="33.02" y="-48.26" smashed="yes">
<attribute name="NAME" x="24.13" y="-42.545" size="1.778" layer="95"/>
<attribute name="VALUE" x="24.13" y="-53.34" size="1.778" layer="96"/>
</instance>
<instance part="JP6" gate="A" x="33.02" y="-55.88" smashed="yes">
<attribute name="NAME" x="24.13" y="-50.165" size="1.778" layer="95"/>
<attribute name="VALUE" x="24.13" y="-60.96" size="1.778" layer="96"/>
</instance>
<instance part="JP15" gate="A" x="50.8" y="-48.26" smashed="yes">
<attribute name="NAME" x="41.91" y="-42.545" size="1.778" layer="95"/>
<attribute name="VALUE" x="41.91" y="-53.34" size="1.778" layer="96"/>
</instance>
<instance part="JP16" gate="A" x="50.8" y="-55.88" smashed="yes">
<attribute name="NAME" x="41.91" y="-50.165" size="1.778" layer="95"/>
<attribute name="VALUE" x="41.91" y="-60.96" size="1.778" layer="96"/>
</instance>
</instances>
<busses>
</busses>
<nets>
<net name="GND" class="0">
<segment>
<wire x1="27.94" y1="35.56" x2="22.86" y2="35.56" width="0.1524" layer="91"/>
<wire x1="22.86" y1="35.56" x2="22.86" y2="25.4" width="0.1524" layer="91"/>
<wire x1="22.86" y1="25.4" x2="27.94" y2="25.4" width="0.1524" layer="91"/>
<wire x1="22.86" y1="35.56" x2="22.86" y2="45.72" width="0.1524" layer="91"/>
<junction x="22.86" y="35.56"/>
<wire x1="53.34" y1="35.56" x2="58.42" y2="35.56" width="0.1524" layer="91"/>
<wire x1="58.42" y1="35.56" x2="58.42" y2="25.4" width="0.1524" layer="91"/>
<wire x1="58.42" y1="25.4" x2="53.34" y2="25.4" width="0.1524" layer="91"/>
<wire x1="22.86" y1="45.72" x2="58.42" y2="45.72" width="0.1524" layer="91"/>
<wire x1="58.42" y1="45.72" x2="58.42" y2="35.56" width="0.1524" layer="91"/>
<junction x="58.42" y="35.56"/>
<wire x1="27.94" y1="-5.08" x2="22.86" y2="-5.08" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-5.08" x2="22.86" y2="-15.24" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-15.24" x2="27.94" y2="-15.24" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-5.08" x2="22.86" y2="0" width="0.1524" layer="91"/>
<junction x="22.86" y="-5.08"/>
<wire x1="53.34" y1="-5.08" x2="58.42" y2="-5.08" width="0.1524" layer="91"/>
<wire x1="58.42" y1="-5.08" x2="58.42" y2="-15.24" width="0.1524" layer="91"/>
<wire x1="58.42" y1="-15.24" x2="53.34" y2="-15.24" width="0.1524" layer="91"/>
<wire x1="22.86" y1="0" x2="58.42" y2="0" width="0.1524" layer="91"/>
<wire x1="58.42" y1="0" x2="58.42" y2="-5.08" width="0.1524" layer="91"/>
<junction x="58.42" y="-5.08"/>
<wire x1="58.42" y1="25.4" x2="58.42" y2="10.16" width="0.1524" layer="91"/>
<junction x="58.42" y="25.4"/>
<junction x="58.42" y="0"/>
<wire x1="58.42" y1="10.16" x2="58.42" y2="0" width="0.1524" layer="91"/>
<wire x1="27.94" y1="-45.72" x2="22.86" y2="-45.72" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-45.72" x2="22.86" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-55.88" x2="27.94" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-45.72" x2="22.86" y2="-40.64" width="0.1524" layer="91"/>
<junction x="22.86" y="-45.72"/>
<wire x1="53.34" y1="-45.72" x2="58.42" y2="-45.72" width="0.1524" layer="91"/>
<wire x1="58.42" y1="-45.72" x2="58.42" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="58.42" y1="-55.88" x2="53.34" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="22.86" y1="-40.64" x2="58.42" y2="-40.64" width="0.1524" layer="91"/>
<wire x1="58.42" y1="-40.64" x2="58.42" y2="-45.72" width="0.1524" layer="91"/>
<junction x="58.42" y="-45.72"/>
<wire x1="58.42" y1="-15.24" x2="58.42" y2="-30.48" width="0.1524" layer="91"/>
<junction x="58.42" y="-15.24"/>
<junction x="58.42" y="-40.64"/>
<wire x1="58.42" y1="-30.48" x2="58.42" y2="-40.64" width="0.1524" layer="91"/>
<pinref part="JP7" gate="G$1" pin="1"/>
<wire x1="58.42" y1="10.16" x2="81.28" y2="10.16" width="0.1524" layer="91"/>
<junction x="58.42" y="10.16"/>
<pinref part="JP8" gate="A" pin="3"/>
<wire x1="81.28" y1="-30.48" x2="58.42" y2="-30.48" width="0.1524" layer="91"/>
<junction x="58.42" y="-30.48"/>
<pinref part="JP10" gate="G$1" pin="2"/>
<wire x1="2.54" y1="10.16" x2="58.42" y2="10.16" width="0.1524" layer="91"/>
<pinref part="JP9" gate="A" pin="1"/>
<wire x1="2.54" y1="-30.48" x2="58.42" y2="-30.48" width="0.1524" layer="91"/>
<pinref part="JP12" gate="A" pin="3"/>
<pinref part="JP11" gate="A" pin="1"/>
<pinref part="JP14" gate="A" pin="4"/>
<pinref part="JP13" gate="A" pin="2"/>
<pinref part="JP1" gate="A" pin="1"/>
<pinref part="JP2" gate="A" pin="3"/>
<pinref part="JP3" gate="A" pin="2"/>
<pinref part="JP4" gate="A" pin="4"/>
<pinref part="JP5" gate="A" pin="1"/>
<pinref part="JP6" gate="A" pin="3"/>
<pinref part="JP16" gate="A" pin="4"/>
<pinref part="JP15" gate="A" pin="2"/>
</segment>
</net>
<net name="VCC" class="0">
<segment>
<wire x1="45.72" y1="33.02" x2="40.64" y2="33.02" width="0.1524" layer="91"/>
<wire x1="40.64" y1="33.02" x2="35.56" y2="33.02" width="0.1524" layer="91"/>
<wire x1="35.56" y1="27.94" x2="40.64" y2="27.94" width="0.1524" layer="91"/>
<wire x1="40.64" y1="27.94" x2="45.72" y2="27.94" width="0.1524" layer="91"/>
<wire x1="40.64" y1="33.02" x2="40.64" y2="27.94" width="0.1524" layer="91"/>
<junction x="40.64" y="33.02"/>
<junction x="40.64" y="27.94"/>
<wire x1="45.72" y1="-7.62" x2="40.64" y2="-7.62" width="0.1524" layer="91"/>
<wire x1="40.64" y1="-7.62" x2="35.56" y2="-7.62" width="0.1524" layer="91"/>
<junction x="40.64" y="-7.62"/>
<wire x1="40.64" y1="27.94" x2="40.64" y2="7.62" width="0.1524" layer="91"/>
<wire x1="40.64" y1="7.62" x2="40.64" y2="-7.62" width="0.1524" layer="91"/>
<wire x1="35.56" y1="-12.7" x2="40.64" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="40.64" y1="-12.7" x2="45.72" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="40.64" y1="-7.62" x2="40.64" y2="-12.7" width="0.1524" layer="91"/>
<junction x="40.64" y="-12.7"/>
<wire x1="45.72" y1="-48.26" x2="40.64" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="40.64" y1="-48.26" x2="35.56" y2="-48.26" width="0.1524" layer="91"/>
<junction x="40.64" y="-48.26"/>
<wire x1="40.64" y1="-12.7" x2="40.64" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="35.56" y1="-53.34" x2="40.64" y2="-53.34" width="0.1524" layer="91"/>
<wire x1="40.64" y1="-53.34" x2="45.72" y2="-53.34" width="0.1524" layer="91"/>
<wire x1="40.64" y1="-48.26" x2="40.64" y2="-53.34" width="0.1524" layer="91"/>
<junction x="40.64" y="-53.34"/>
<pinref part="JP7" gate="G$1" pin="2"/>
<wire x1="81.28" y1="7.62" x2="40.64" y2="7.62" width="0.1524" layer="91"/>
<junction x="40.64" y="7.62"/>
<pinref part="JP10" gate="G$1" pin="1"/>
<wire x1="40.64" y1="7.62" x2="2.54" y2="7.62" width="0.1524" layer="91"/>
<pinref part="JP12" gate="A" pin="2"/>
<pinref part="JP11" gate="A" pin="4"/>
<pinref part="JP14" gate="A" pin="1"/>
<pinref part="JP13" gate="A" pin="3"/>
<pinref part="JP1" gate="A" pin="4"/>
<pinref part="JP2" gate="A" pin="2"/>
<pinref part="JP3" gate="A" pin="3"/>
<pinref part="JP4" gate="A" pin="1"/>
<pinref part="JP5" gate="A" pin="4"/>
<pinref part="JP6" gate="A" pin="2"/>
<pinref part="JP16" gate="A" pin="1"/>
<pinref part="JP15" gate="A" pin="3"/>
</segment>
</net>
<net name="SCL" class="0">
<segment>
<wire x1="35.56" y1="35.56" x2="35.56" y2="48.26" width="0.1524" layer="91"/>
<wire x1="35.56" y1="48.26" x2="60.96" y2="48.26" width="0.1524" layer="91"/>
<wire x1="60.96" y1="48.26" x2="60.96" y2="33.02" width="0.1524" layer="91"/>
<wire x1="60.96" y1="33.02" x2="53.34" y2="33.02" width="0.1524" layer="91"/>
<wire x1="60.96" y1="33.02" x2="60.96" y2="17.78" width="0.1524" layer="91"/>
<junction x="60.96" y="33.02"/>
<wire x1="60.96" y1="17.78" x2="45.72" y2="17.78" width="0.1524" layer="91"/>
<wire x1="45.72" y1="17.78" x2="45.72" y2="25.4" width="0.1524" layer="91"/>
<wire x1="45.72" y1="17.78" x2="20.32" y2="17.78" width="0.1524" layer="91"/>
<wire x1="20.32" y1="17.78" x2="20.32" y2="27.94" width="0.1524" layer="91"/>
<junction x="45.72" y="17.78"/>
<wire x1="20.32" y1="27.94" x2="27.94" y2="27.94" width="0.1524" layer="91"/>
<wire x1="35.56" y1="-5.08" x2="35.56" y2="2.54" width="0.1524" layer="91"/>
<wire x1="35.56" y1="2.54" x2="60.96" y2="2.54" width="0.1524" layer="91"/>
<wire x1="60.96" y1="2.54" x2="60.96" y2="-7.62" width="0.1524" layer="91"/>
<wire x1="60.96" y1="-7.62" x2="53.34" y2="-7.62" width="0.1524" layer="91"/>
<wire x1="60.96" y1="-7.62" x2="60.96" y2="-22.86" width="0.1524" layer="91"/>
<junction x="60.96" y="-7.62"/>
<wire x1="60.96" y1="-22.86" x2="45.72" y2="-22.86" width="0.1524" layer="91"/>
<wire x1="45.72" y1="-22.86" x2="45.72" y2="-15.24" width="0.1524" layer="91"/>
<wire x1="45.72" y1="-22.86" x2="20.32" y2="-22.86" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-22.86" x2="20.32" y2="-12.7" width="0.1524" layer="91"/>
<junction x="45.72" y="-22.86"/>
<wire x1="20.32" y1="-12.7" x2="27.94" y2="-12.7" width="0.1524" layer="91"/>
<wire x1="60.96" y1="17.78" x2="60.96" y2="2.54" width="0.1524" layer="91"/>
<junction x="60.96" y="17.78"/>
<junction x="60.96" y="2.54"/>
<wire x1="35.56" y1="-45.72" x2="35.56" y2="-38.1" width="0.1524" layer="91"/>
<wire x1="35.56" y1="-38.1" x2="60.96" y2="-38.1" width="0.1524" layer="91"/>
<wire x1="60.96" y1="-38.1" x2="60.96" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="60.96" y1="-48.26" x2="53.34" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="60.96" y1="-48.26" x2="60.96" y2="-63.5" width="0.1524" layer="91"/>
<junction x="60.96" y="-48.26"/>
<wire x1="60.96" y1="-63.5" x2="45.72" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="45.72" y1="-63.5" x2="45.72" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="45.72" y1="-63.5" x2="20.32" y2="-63.5" width="0.1524" layer="91"/>
<wire x1="20.32" y1="-63.5" x2="20.32" y2="-53.34" width="0.1524" layer="91"/>
<junction x="45.72" y="-63.5"/>
<wire x1="20.32" y1="-53.34" x2="27.94" y2="-53.34" width="0.1524" layer="91"/>
<junction x="60.96" y="-22.86"/>
<junction x="60.96" y="-38.1"/>
<wire x1="60.96" y1="-22.86" x2="60.96" y2="-25.4" width="0.1524" layer="91"/>
<pinref part="JP8" gate="A" pin="1"/>
<wire x1="60.96" y1="-25.4" x2="60.96" y2="-38.1" width="0.1524" layer="91"/>
<wire x1="81.28" y1="-25.4" x2="60.96" y2="-25.4" width="0.1524" layer="91"/>
<pinref part="JP9" gate="A" pin="3"/>
<wire x1="2.54" y1="-25.4" x2="60.96" y2="-25.4" width="0.1524" layer="91"/>
<junction x="60.96" y="-25.4"/>
<pinref part="JP12" gate="A" pin="1"/>
<pinref part="JP11" gate="A" pin="2"/>
<pinref part="JP14" gate="A" pin="3"/>
<pinref part="JP13" gate="A" pin="4"/>
<pinref part="JP1" gate="A" pin="2"/>
<pinref part="JP2" gate="A" pin="1"/>
<pinref part="JP3" gate="A" pin="4"/>
<pinref part="JP4" gate="A" pin="3"/>
<pinref part="JP5" gate="A" pin="2"/>
<pinref part="JP6" gate="A" pin="1"/>
<pinref part="JP16" gate="A" pin="3"/>
<pinref part="JP15" gate="A" pin="4"/>
</segment>
</net>
<net name="SDA" class="0">
<segment>
<wire x1="27.94" y1="33.02" x2="17.78" y2="33.02" width="0.1524" layer="91"/>
<wire x1="17.78" y1="33.02" x2="17.78" y2="50.8" width="0.1524" layer="91"/>
<wire x1="17.78" y1="50.8" x2="45.72" y2="50.8" width="0.1524" layer="91"/>
<wire x1="45.72" y1="50.8" x2="45.72" y2="35.56" width="0.1524" layer="91"/>
<wire x1="45.72" y1="50.8" x2="63.5" y2="50.8" width="0.1524" layer="91"/>
<wire x1="63.5" y1="50.8" x2="63.5" y2="27.94" width="0.1524" layer="91"/>
<junction x="45.72" y="50.8"/>
<wire x1="63.5" y1="27.94" x2="53.34" y2="27.94" width="0.1524" layer="91"/>
<wire x1="63.5" y1="27.94" x2="63.5" y2="15.24" width="0.1524" layer="91"/>
<junction x="63.5" y="27.94"/>
<wire x1="63.5" y1="15.24" x2="35.56" y2="15.24" width="0.1524" layer="91"/>
<wire x1="35.56" y1="15.24" x2="35.56" y2="25.4" width="0.1524" layer="91"/>
<wire x1="27.94" y1="-7.62" x2="17.78" y2="-7.62" width="0.1524" layer="91"/>
<wire x1="17.78" y1="-7.62" x2="17.78" y2="5.08" width="0.1524" layer="91"/>
<wire x1="17.78" y1="5.08" x2="45.72" y2="5.08" width="0.1524" layer="91"/>
<wire x1="45.72" y1="5.08" x2="45.72" y2="-5.08" width="0.1524" layer="91"/>
<wire x1="45.72" y1="5.08" x2="63.5" y2="5.08" width="0.1524" layer="91"/>
<wire x1="63.5" y1="5.08" x2="63.5" y2="-12.7" width="0.1524" layer="91"/>
<junction x="45.72" y="5.08"/>
<wire x1="63.5" y1="-12.7" x2="53.34" y2="-12.7" width="0.1524" layer="91"/>
<junction x="63.5" y="-12.7"/>
<wire x1="63.5" y1="15.24" x2="63.5" y2="5.08" width="0.1524" layer="91"/>
<junction x="63.5" y="15.24"/>
<junction x="63.5" y="5.08"/>
<wire x1="27.94" y1="-48.26" x2="17.78" y2="-48.26" width="0.1524" layer="91"/>
<wire x1="17.78" y1="-48.26" x2="17.78" y2="-35.56" width="0.1524" layer="91"/>
<wire x1="17.78" y1="-35.56" x2="45.72" y2="-35.56" width="0.1524" layer="91"/>
<wire x1="45.72" y1="-35.56" x2="45.72" y2="-45.72" width="0.1524" layer="91"/>
<wire x1="45.72" y1="-35.56" x2="63.5" y2="-35.56" width="0.1524" layer="91"/>
<wire x1="63.5" y1="-35.56" x2="63.5" y2="-53.34" width="0.1524" layer="91"/>
<junction x="45.72" y="-35.56"/>
<wire x1="63.5" y1="-53.34" x2="53.34" y2="-53.34" width="0.1524" layer="91"/>
<wire x1="63.5" y1="-53.34" x2="63.5" y2="-66.04" width="0.1524" layer="91"/>
<junction x="63.5" y="-53.34"/>
<wire x1="63.5" y1="-66.04" x2="35.56" y2="-66.04" width="0.1524" layer="91"/>
<wire x1="35.56" y1="-66.04" x2="35.56" y2="-55.88" width="0.1524" layer="91"/>
<wire x1="63.5" y1="-12.7" x2="63.5" y2="-27.94" width="0.1524" layer="91"/>
<junction x="63.5" y="-35.56"/>
<pinref part="JP8" gate="A" pin="2"/>
<pinref part="JP9" gate="A" pin="2"/>
<wire x1="63.5" y1="-27.94" x2="63.5" y2="-35.56" width="0.1524" layer="91"/>
<wire x1="2.54" y1="-27.94" x2="35.56" y2="-27.94" width="0.1524" layer="91"/>
<junction x="63.5" y="-27.94"/>
<wire x1="35.56" y1="-27.94" x2="63.5" y2="-27.94" width="0.1524" layer="91"/>
<wire x1="63.5" y1="-27.94" x2="81.28" y2="-27.94" width="0.1524" layer="91"/>
<wire x1="35.56" y1="-15.24" x2="35.56" y2="-27.94" width="0.1524" layer="91"/>
<junction x="35.56" y="-27.94"/>
<pinref part="JP12" gate="A" pin="4"/>
<pinref part="JP11" gate="A" pin="3"/>
<pinref part="JP14" gate="A" pin="2"/>
<pinref part="JP13" gate="A" pin="1"/>
<pinref part="JP1" gate="A" pin="3"/>
<pinref part="JP2" gate="A" pin="4"/>
<pinref part="JP3" gate="A" pin="1"/>
<pinref part="JP4" gate="A" pin="2"/>
<pinref part="JP5" gate="A" pin="3"/>
<pinref part="JP6" gate="A" pin="4"/>
<pinref part="JP16" gate="A" pin="2"/>
<pinref part="JP15" gate="A" pin="1"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports URNs for individual library
assets (packages, symbols, and devices). The URNs of those assets
will not be understood (or retained) with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports the association of 3D packages
with devices in libraries, schematics, and board files. Those 3D
packages will not be understood (or retained) with this version.
</note>
</compatibility>
</eagle>
