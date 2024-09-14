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
<package name="1X06" urn="urn:adsk.eagle:footprint:22361/1" locally_modified="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="0.635" y1="1.27" x2="1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="1.27" x2="2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="0.635" x2="2.54" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-0.635" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="2.54" y1="0.635" x2="3.175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="1.27" x2="4.445" y2="1.27" width="0.1524" layer="21"/>
<wire x1="4.445" y1="1.27" x2="5.08" y2="0.635" width="0.1524" layer="21"/>
<wire x1="5.08" y1="0.635" x2="5.08" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="5.08" y1="-0.635" x2="4.445" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="4.445" y1="-1.27" x2="3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-1.27" x2="2.54" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0.635" x2="-1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="1.27" x2="-0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="1.27" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="0.635" x2="0" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="-0.635" x2="-0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-1.27" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-1.27" x2="-2.54" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="0.635" y1="1.27" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="-0.635" x2="0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-1.27" x2="0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-6.985" y1="1.27" x2="-5.715" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="1.27" x2="-5.08" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="0.635" x2="-5.08" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="-0.635" x2="-5.715" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="0.635" x2="-4.445" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="1.27" x2="-3.175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="1.27" x2="-2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0.635" x2="-2.54" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-0.635" x2="-3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="-1.27" x2="-4.445" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="-1.27" x2="-5.08" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="0.635" x2="-7.62" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-6.985" y1="1.27" x2="-7.62" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="-0.635" x2="-6.985" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="-1.27" x2="-6.985" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="5.715" y1="1.27" x2="6.985" y2="1.27" width="0.1524" layer="21"/>
<wire x1="6.985" y1="1.27" x2="7.62" y2="0.635" width="0.1524" layer="21"/>
<wire x1="7.62" y1="0.635" x2="7.62" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="7.62" y1="-0.635" x2="6.985" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="5.715" y1="1.27" x2="5.08" y2="0.635" width="0.1524" layer="21"/>
<wire x1="5.08" y1="-0.635" x2="5.715" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="6.985" y1="-1.27" x2="5.715" y2="-1.27" width="0.1524" layer="21"/>
<pad name="1" x="-6.35" y="0" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="2" x="-3.81" y="0" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="3" x="-1.27" y="0" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="4" x="1.27" y="0" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="5" x="3.81" y="0" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="6" x="6.35" y="0" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<text x="-7.6962" y="1.8288" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-7.62" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="3.556" y1="-0.254" x2="4.064" y2="0.254" layer="51"/>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51"/>
<rectangle x1="-1.524" y1="-0.254" x2="-1.016" y2="0.254" layer="51"/>
<rectangle x1="-4.064" y1="-0.254" x2="-3.556" y2="0.254" layer="51"/>
<rectangle x1="-6.604" y1="-0.254" x2="-6.096" y2="0.254" layer="51"/>
<rectangle x1="6.096" y1="-0.254" x2="6.604" y2="0.254" layer="51"/>
</package>
<package name="1X05" urn="urn:adsk.eagle:footprint:22354/1" locally_modified="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="1.905" y1="1.27" x2="3.175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="1.27" x2="3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="3.81" y1="0.635" x2="3.81" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="3.81" y1="-0.635" x2="3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="0.635" x2="-0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="1.27" x2="0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="1.27" x2="1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="0.635" x2="1.27" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-0.635" x2="0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="-1.27" x2="-0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-1.27" x2="-1.27" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="1.905" y1="1.27" x2="1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-0.635" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-1.27" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="1.27" x2="-4.445" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="1.27" x2="-3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="0.635" x2="-3.81" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="-0.635" x2="-4.445" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="0.635" x2="-3.175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="1.27" x2="-1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="1.27" x2="-1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="0.635" x2="-1.27" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-0.635" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-1.27" x2="-3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="-1.27" x2="-3.81" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-6.35" y1="0.635" x2="-6.35" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="1.27" x2="-6.35" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-6.35" y1="-0.635" x2="-5.715" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="-1.27" x2="-5.715" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="4.445" y1="1.27" x2="5.715" y2="1.27" width="0.1524" layer="21"/>
<wire x1="5.715" y1="1.27" x2="6.35" y2="0.635" width="0.1524" layer="21"/>
<wire x1="6.35" y1="0.635" x2="6.35" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="6.35" y1="-0.635" x2="5.715" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="4.445" y1="1.27" x2="3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="3.81" y1="-0.635" x2="4.445" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="5.715" y1="-1.27" x2="4.445" y2="-1.27" width="0.1524" layer="21"/>
<pad name="1" x="-5.08" y="0" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="2" x="-2.54" y="0" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="3" x="0" y="0" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="4" x="2.54" y="0" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="5" x="5.08" y="0" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<text x="-6.4262" y="1.8288" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-6.35" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="2.286" y1="-0.254" x2="2.794" y2="0.254" layer="51"/>
<rectangle x1="-0.254" y1="-0.254" x2="0.254" y2="0.254" layer="51"/>
<rectangle x1="-2.794" y1="-0.254" x2="-2.286" y2="0.254" layer="51"/>
<rectangle x1="-5.334" y1="-0.254" x2="-4.826" y2="0.254" layer="51"/>
<rectangle x1="4.826" y1="-0.254" x2="5.334" y2="0.254" layer="51"/>
</package>
<package name="1_05X2MM" urn="urn:adsk.eagle:footprint:22356/1">
<description>CON-M-1X5-200</description>
<text x="-4.5" y="1.5" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-4.75" y="-2.75" size="1.27" layer="27">&gt;VALUE</text>
<wire x1="-5" y1="0.5" x2="-4.5" y2="1" width="0.1524" layer="21"/>
<wire x1="-4.5" y1="1" x2="-3.5" y2="1" width="0.1524" layer="21"/>
<wire x1="-3.5" y1="1" x2="-3" y2="0.5" width="0.1524" layer="21"/>
<wire x1="-3" y1="-0.5" x2="-3.5" y2="-1" width="0.1524" layer="21"/>
<wire x1="-3.5" y1="-1" x2="-4.5" y2="-1" width="0.1524" layer="21"/>
<wire x1="-4.5" y1="-1" x2="-5" y2="-0.5" width="0.1524" layer="21"/>
<wire x1="-5" y1="0.5" x2="-5" y2="-0.5" width="0.1524" layer="21"/>
<wire x1="-3" y1="0.5" x2="-2.5" y2="1" width="0.1524" layer="21"/>
<wire x1="-2.5" y1="1" x2="-1.5" y2="1" width="0.1524" layer="21"/>
<wire x1="-1.5" y1="1" x2="-1" y2="0.5" width="0.1524" layer="21"/>
<wire x1="-1" y1="-0.5" x2="-1.5" y2="-1" width="0.1524" layer="21"/>
<wire x1="-1.5" y1="-1" x2="-2.5" y2="-1" width="0.1524" layer="21"/>
<wire x1="-2.5" y1="-1" x2="-3" y2="-0.5" width="0.1524" layer="21"/>
<wire x1="-3" y1="0.5" x2="-3" y2="-0.5" width="0.1524" layer="21"/>
<wire x1="-1" y1="0.5" x2="-0.5" y2="1" width="0.1524" layer="21"/>
<wire x1="-0.5" y1="1" x2="0.5" y2="1" width="0.1524" layer="21"/>
<wire x1="0.5" y1="1" x2="1" y2="0.5" width="0.1524" layer="21"/>
<wire x1="1" y1="-0.5" x2="0.5" y2="-1" width="0.1524" layer="21"/>
<wire x1="0.5" y1="-1" x2="-0.5" y2="-1" width="0.1524" layer="21"/>
<wire x1="-0.5" y1="-1" x2="-1" y2="-0.5" width="0.1524" layer="21"/>
<wire x1="-1" y1="0.5" x2="-1" y2="-0.5" width="0.1524" layer="21"/>
<wire x1="1" y1="0.5" x2="1.5" y2="1" width="0.1524" layer="21"/>
<wire x1="1.5" y1="1" x2="2.5" y2="1" width="0.1524" layer="21"/>
<wire x1="2.5" y1="1" x2="3" y2="0.5" width="0.1524" layer="21"/>
<wire x1="3" y1="-0.5" x2="2.5" y2="-1" width="0.1524" layer="21"/>
<wire x1="2.5" y1="-1" x2="1.5" y2="-1" width="0.1524" layer="21"/>
<wire x1="1.5" y1="-1" x2="1" y2="-0.5" width="0.1524" layer="21"/>
<wire x1="1" y1="0.5" x2="1" y2="-0.5" width="0.1524" layer="21"/>
<wire x1="3" y1="0.5" x2="3.5" y2="1" width="0.1524" layer="21"/>
<wire x1="3.5" y1="1" x2="4.5" y2="1" width="0.1524" layer="21"/>
<wire x1="4.5" y1="1" x2="5" y2="0.5" width="0.1524" layer="21"/>
<wire x1="5" y1="0.5" x2="5" y2="-0.5" width="0.1524" layer="21"/>
<wire x1="5" y1="-0.5" x2="4.5" y2="-1" width="0.1524" layer="21"/>
<wire x1="4.5" y1="-1" x2="3.5" y2="-1" width="0.1524" layer="21"/>
<wire x1="3.5" y1="-1" x2="3" y2="-0.5" width="0.1524" layer="21"/>
<wire x1="3" y1="0.5" x2="3" y2="-0.5" width="0.1524" layer="21"/>
<pad name="1" x="-4" y="0" drill="1.016" diameter="1.3" shape="square" rot="R90"/>
<pad name="3" x="0" y="0" drill="1.016" diameter="1.3" rot="R90"/>
<pad name="2" x="-2" y="0" drill="1.016" diameter="1.3" rot="R90"/>
<pad name="4" x="2" y="0" drill="1.016" diameter="1.3" rot="R90"/>
<pad name="5" x="4" y="0" drill="1.016" diameter="1.3" rot="R90"/>
<rectangle x1="-4.254" y1="-0.254" x2="-3.746" y2="0.254" layer="51"/>
<rectangle x1="-2.254" y1="-0.254" x2="-1.746" y2="0.254" layer="51"/>
<rectangle x1="-0.254" y1="-0.254" x2="0.254" y2="0.254" layer="51"/>
<rectangle x1="1.746" y1="-0.254" x2="2.254" y2="0.254" layer="51"/>
<rectangle x1="3.746" y1="-0.254" x2="4.254" y2="0.254" layer="51"/>
</package>
<package name="1X5_AUTO">
<wire x1="1.905" y1="1.27" x2="3.175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="1.27" x2="3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="3.81" y1="0.635" x2="3.81" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="3.81" y1="-0.635" x2="3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="0.635" x2="-0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="1.27" x2="0.635" y2="1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="1.27" x2="1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="0.635" x2="1.27" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-0.635" x2="0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0.635" y1="-1.27" x2="-0.635" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-1.27" x2="-1.27" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="1.905" y1="1.27" x2="1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-0.635" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-1.27" x2="1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="1.27" x2="-4.445" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="1.27" x2="-3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="0.635" x2="-3.81" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="-0.635" x2="-4.445" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="0.635" x2="-3.175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="1.27" x2="-1.905" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="1.27" x2="-1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="0.635" x2="-1.27" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-0.635" x2="-1.905" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-1.27" x2="-3.175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="-1.27" x2="-3.81" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-6.35" y1="0.635" x2="-6.35" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="-5.715" y1="1.27" x2="-6.35" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-6.35" y1="-0.635" x2="-5.715" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-4.445" y1="-1.27" x2="-5.715" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="4.445" y1="1.27" x2="5.715" y2="1.27" width="0.1524" layer="21"/>
<wire x1="5.715" y1="1.27" x2="6.35" y2="0.635" width="0.1524" layer="21"/>
<wire x1="6.35" y1="0.635" x2="6.35" y2="-0.635" width="0.1524" layer="21"/>
<wire x1="6.35" y1="-0.635" x2="5.715" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="4.445" y1="1.27" x2="3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="3.81" y1="-0.635" x2="4.445" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="5.715" y1="-1.27" x2="4.445" y2="-1.27" width="0.1524" layer="21"/>
<pad name="1" x="-5.08" y="0" drill="1.016" shape="octagon" rot="R90"/>
<pad name="2" x="-2.54" y="0" drill="1.016" shape="octagon" rot="R90"/>
<pad name="3" x="0" y="0" drill="1.016" shape="octagon" rot="R90"/>
<pad name="4" x="2.54" y="0" drill="1.016" shape="octagon" rot="R90"/>
<pad name="5" x="5.08" y="0" drill="1.016" shape="octagon" rot="R90"/>
<text x="-6.4262" y="1.8288" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-6.35" y="-3.175" size="1.27" layer="27">&gt;VALUE</text>
<rectangle x1="2.286" y1="-0.254" x2="2.794" y2="0.254" layer="51"/>
<rectangle x1="-0.254" y1="-0.254" x2="0.254" y2="0.254" layer="51"/>
<rectangle x1="-2.794" y1="-0.254" x2="-2.286" y2="0.254" layer="51"/>
<rectangle x1="-5.334" y1="-0.254" x2="-4.826" y2="0.254" layer="51"/>
<rectangle x1="4.826" y1="-0.254" x2="5.334" y2="0.254" layer="51"/>
</package>
<package name="1X05_1.27">
<wire x1="0.9525" y1="1.27" x2="1.5875" y2="1.27" width="0.1524" layer="21"/>
<wire x1="1.5875" y1="1.27" x2="1.905" y2="0.9525" width="0.1524" layer="21"/>
<wire x1="1.905" y1="0.9525" x2="1.905" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-0.9525" x2="1.5875" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.905" y1="0.9525" x2="2.2225" y2="1.27" width="0.1524" layer="21"/>
<wire x1="2.2225" y1="1.27" x2="2.8575" y2="1.27" width="0.1524" layer="21"/>
<wire x1="2.8575" y1="1.27" x2="3.175" y2="0.9525" width="0.1524" layer="21"/>
<wire x1="3.175" y1="0.9525" x2="3.175" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="3.175" y1="-0.9525" x2="2.8575" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="2.8575" y1="-1.27" x2="2.2225" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="2.2225" y1="-1.27" x2="1.905" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="0.9525" x2="-0.3175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.3175" y1="1.27" x2="0.3175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="0.3175" y1="1.27" x2="0.635" y2="0.9525" width="0.1524" layer="21"/>
<wire x1="0.635" y1="0.9525" x2="0.635" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="0.635" y1="-0.9525" x2="0.3175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0.3175" y1="-1.27" x2="-0.3175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.3175" y1="-1.27" x2="-0.635" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="0.9525" y1="1.27" x2="0.635" y2="0.9525" width="0.1524" layer="21"/>
<wire x1="0.635" y1="-0.9525" x2="0.9525" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.5875" y1="-1.27" x2="0.9525" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-2.8575" y1="1.27" x2="-2.2225" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-2.2225" y1="1.27" x2="-1.905" y2="0.9525" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="0.9525" x2="-1.905" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="-0.9525" x2="-2.2225" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.905" y1="0.9525" x2="-1.5875" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-1.5875" y1="1.27" x2="-0.9525" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.9525" y1="1.27" x2="-0.635" y2="0.9525" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="0.9525" x2="-0.635" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="-0.635" y1="-0.9525" x2="-0.9525" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.9525" y1="-1.27" x2="-1.5875" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.5875" y1="-1.27" x2="-1.905" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="0.9525" x2="-3.175" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="-2.8575" y1="1.27" x2="-3.175" y2="0.9525" width="0.1524" layer="21"/>
<wire x1="-3.175" y1="-0.9525" x2="-2.8575" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-2.2225" y1="-1.27" x2="-2.8575" y2="-1.27" width="0.1524" layer="21"/>
<pad name="1" x="-2.54" y="0" drill="0.65" diameter="1.016" shape="long" rot="R90"/>
<pad name="2" x="-1.27" y="0" drill="0.65" diameter="1.016" shape="long" rot="R90"/>
<pad name="3" x="0" y="0" drill="0.65" diameter="1.016" shape="long" rot="R90"/>
<pad name="4" x="1.27" y="0" drill="0.65" diameter="1.016" shape="long" rot="R90"/>
<pad name="5" x="2.54" y="0" drill="0.65" diameter="1.016" shape="long" rot="R90"/>
<text x="-3.5687" y="1.8288" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-3.81" y="-2.8575" size="1.27" layer="27">&gt;VALUE</text>
</package>
<package name="1X05_1.27SMT">
<smd name="P$1" x="0" y="1.524" dx="3" dy="0.76" layer="1" rot="R90"/>
<smd name="P$2" x="1.27" y="-1.524" dx="3" dy="0.76" layer="1" rot="R90"/>
<smd name="P$3" x="2.54" y="1.524" dx="3" dy="0.76" layer="1" rot="R90"/>
<smd name="P$4" x="3.81" y="-1.524" dx="3" dy="0.76" layer="1" rot="R90"/>
<smd name="P$5" x="5.08" y="1.524" dx="3" dy="0.76" layer="1" rot="R90"/>
<wire x1="0.381" y1="-1.27" x2="-0.381" y2="-1.27" width="0.127" layer="51"/>
<wire x1="-0.635" y1="-1.016" x2="-0.635" y2="0.889" width="0.127" layer="51"/>
<wire x1="-0.635" y1="0.889" x2="-0.635" y2="1.016" width="0.127" layer="51"/>
<wire x1="-0.381" y1="1.27" x2="-0.254" y2="1.27" width="0.127" layer="51"/>
<wire x1="-0.254" y1="1.27" x2="0.254" y2="1.27" width="0.127" layer="51"/>
<wire x1="0.254" y1="1.27" x2="0.381" y2="1.27" width="0.127" layer="51"/>
<wire x1="0.635" y1="1.016" x2="0.635" y2="0.889" width="0.127" layer="51"/>
<wire x1="0.635" y1="0.889" x2="0.635" y2="-1.016" width="0.127" layer="51"/>
<wire x1="-0.381" y1="-1.27" x2="-0.635" y2="-1.016" width="0.127" layer="51"/>
<wire x1="0.381" y1="-1.27" x2="0.635" y2="-1.016" width="0.127" layer="51"/>
<wire x1="-0.635" y1="1.016" x2="-0.381" y2="1.27" width="0.127" layer="51"/>
<wire x1="0.635" y1="1.016" x2="0.381" y2="1.27" width="0.127" layer="51"/>
<wire x1="1.651" y1="-1.27" x2="0.889" y2="-1.27" width="0.127" layer="51"/>
<wire x1="0.635" y1="-1.016" x2="0.635" y2="0.889" width="0.127" layer="51"/>
<wire x1="0.635" y1="0.889" x2="0.635" y2="1.016" width="0.127" layer="51"/>
<wire x1="0.889" y1="1.27" x2="1.016" y2="1.27" width="0.127" layer="51"/>
<wire x1="1.016" y1="1.27" x2="1.524" y2="1.27" width="0.127" layer="51"/>
<wire x1="1.524" y1="1.27" x2="1.651" y2="1.27" width="0.127" layer="51"/>
<wire x1="1.905" y1="1.016" x2="1.905" y2="0.889" width="0.127" layer="51"/>
<wire x1="1.905" y1="0.889" x2="1.905" y2="-1.016" width="0.127" layer="51"/>
<wire x1="0.889" y1="-1.27" x2="0.635" y2="-1.016" width="0.127" layer="51"/>
<wire x1="1.651" y1="-1.27" x2="1.905" y2="-1.016" width="0.127" layer="51"/>
<wire x1="0.635" y1="1.016" x2="0.889" y2="1.27" width="0.127" layer="51"/>
<wire x1="1.905" y1="1.016" x2="1.651" y2="1.27" width="0.127" layer="51"/>
<wire x1="2.921" y1="-1.27" x2="2.159" y2="-1.27" width="0.127" layer="51"/>
<wire x1="1.905" y1="-1.016" x2="1.905" y2="0.889" width="0.127" layer="51"/>
<wire x1="1.905" y1="0.889" x2="1.905" y2="1.016" width="0.127" layer="51"/>
<wire x1="2.159" y1="1.27" x2="2.286" y2="1.27" width="0.127" layer="51"/>
<wire x1="2.286" y1="1.27" x2="2.794" y2="1.27" width="0.127" layer="51"/>
<wire x1="2.794" y1="1.27" x2="2.921" y2="1.27" width="0.127" layer="51"/>
<wire x1="3.175" y1="1.016" x2="3.175" y2="0.889" width="0.127" layer="51"/>
<wire x1="3.175" y1="0.889" x2="3.175" y2="-1.016" width="0.127" layer="51"/>
<wire x1="2.159" y1="-1.27" x2="1.905" y2="-1.016" width="0.127" layer="51"/>
<wire x1="2.921" y1="-1.27" x2="3.175" y2="-1.016" width="0.127" layer="51"/>
<wire x1="1.905" y1="1.016" x2="2.159" y2="1.27" width="0.127" layer="51"/>
<wire x1="3.175" y1="1.016" x2="2.921" y2="1.27" width="0.127" layer="51"/>
<wire x1="4.191" y1="-1.27" x2="3.429" y2="-1.27" width="0.127" layer="51"/>
<wire x1="3.175" y1="-1.016" x2="3.175" y2="0.889" width="0.127" layer="51"/>
<wire x1="3.175" y1="0.889" x2="3.175" y2="1.016" width="0.127" layer="51"/>
<wire x1="3.429" y1="1.27" x2="3.556" y2="1.27" width="0.127" layer="51"/>
<wire x1="3.556" y1="1.27" x2="4.064" y2="1.27" width="0.127" layer="51"/>
<wire x1="4.064" y1="1.27" x2="4.191" y2="1.27" width="0.127" layer="51"/>
<wire x1="4.445" y1="1.016" x2="4.445" y2="0.889" width="0.127" layer="51"/>
<wire x1="4.445" y1="0.889" x2="4.445" y2="-1.016" width="0.127" layer="51"/>
<wire x1="3.429" y1="-1.27" x2="3.175" y2="-1.016" width="0.127" layer="51"/>
<wire x1="4.191" y1="-1.27" x2="4.445" y2="-1.016" width="0.127" layer="51"/>
<wire x1="3.175" y1="1.016" x2="3.429" y2="1.27" width="0.127" layer="51"/>
<wire x1="4.445" y1="1.016" x2="4.191" y2="1.27" width="0.127" layer="51"/>
<wire x1="5.461" y1="-1.27" x2="4.699" y2="-1.27" width="0.127" layer="51"/>
<wire x1="4.445" y1="-1.016" x2="4.445" y2="0.889" width="0.127" layer="51"/>
<wire x1="4.445" y1="0.889" x2="4.445" y2="1.016" width="0.127" layer="51"/>
<wire x1="4.699" y1="1.27" x2="4.826" y2="1.27" width="0.127" layer="51"/>
<wire x1="4.826" y1="1.27" x2="5.334" y2="1.27" width="0.127" layer="51"/>
<wire x1="5.334" y1="1.27" x2="5.461" y2="1.27" width="0.127" layer="51"/>
<wire x1="5.715" y1="1.016" x2="5.715" y2="0.889" width="0.127" layer="51"/>
<wire x1="5.715" y1="0.889" x2="5.715" y2="-1.016" width="0.127" layer="51"/>
<wire x1="4.699" y1="-1.27" x2="4.445" y2="-1.016" width="0.127" layer="51"/>
<wire x1="5.461" y1="-1.27" x2="5.715" y2="-1.016" width="0.127" layer="51"/>
<wire x1="4.445" y1="1.016" x2="4.699" y2="1.27" width="0.127" layer="51"/>
<wire x1="5.715" y1="1.016" x2="5.461" y2="1.27" width="0.127" layer="51"/>
<rectangle x1="-0.254" y1="-0.254" x2="0.254" y2="0.254" layer="51"/>
<rectangle x1="1.016" y1="-0.254" x2="1.524" y2="0.254" layer="51"/>
<rectangle x1="2.286" y1="-0.254" x2="2.794" y2="0.254" layer="51"/>
<rectangle x1="3.556" y1="-0.254" x2="4.064" y2="0.254" layer="51"/>
<rectangle x1="4.826" y1="-0.254" x2="5.334" y2="0.254" layer="51"/>
</package>
<package name="1X6_1.27">
<wire x1="0.3175" y1="1.27" x2="0.9525" y2="1.27" width="0.1524" layer="21"/>
<wire x1="0.9525" y1="1.27" x2="1.27" y2="0.9525" width="0.1524" layer="21"/>
<wire x1="1.27" y1="0.9525" x2="1.27" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-0.9525" x2="0.9525" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.27" y1="0.9525" x2="1.5875" y2="1.27" width="0.1524" layer="21"/>
<wire x1="1.5875" y1="1.27" x2="2.2225" y2="1.27" width="0.1524" layer="21"/>
<wire x1="2.2225" y1="1.27" x2="2.54" y2="0.9525" width="0.1524" layer="21"/>
<wire x1="2.54" y1="0.9525" x2="2.54" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-0.9525" x2="2.2225" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="2.2225" y1="-1.27" x2="1.5875" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="1.5875" y1="-1.27" x2="1.27" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="0.9525" x2="-0.9525" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.9525" y1="1.27" x2="-0.3175" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-0.3175" y1="1.27" x2="0" y2="0.9525" width="0.1524" layer="21"/>
<wire x1="0" y1="0.9525" x2="0" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="0" y1="-0.9525" x2="-0.3175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.3175" y1="-1.27" x2="-0.9525" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-0.9525" y1="-1.27" x2="-1.27" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="0.3175" y1="1.27" x2="0" y2="0.9525" width="0.1524" layer="21"/>
<wire x1="0" y1="-0.9525" x2="0.3175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="0.9525" y1="-1.27" x2="0.3175" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-3.4925" y1="1.27" x2="-2.8575" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-2.8575" y1="1.27" x2="-2.54" y2="0.9525" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0.9525" x2="-2.54" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-0.9525" x2="-2.8575" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0.9525" x2="-2.2225" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-2.2225" y1="1.27" x2="-1.5875" y2="1.27" width="0.1524" layer="21"/>
<wire x1="-1.5875" y1="1.27" x2="-1.27" y2="0.9525" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="0.9525" x2="-1.27" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-0.9525" x2="-1.5875" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-1.5875" y1="-1.27" x2="-2.2225" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-2.2225" y1="-1.27" x2="-2.54" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="0.9525" x2="-3.81" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="-3.4925" y1="1.27" x2="-3.81" y2="0.9525" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="-0.9525" x2="-3.4925" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="-2.8575" y1="-1.27" x2="-3.4925" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="2.8575" y1="1.27" x2="3.4925" y2="1.27" width="0.1524" layer="21"/>
<wire x1="3.4925" y1="1.27" x2="3.81" y2="0.9525" width="0.1524" layer="21"/>
<wire x1="3.81" y1="0.9525" x2="3.81" y2="-0.9525" width="0.1524" layer="21"/>
<wire x1="3.81" y1="-0.9525" x2="3.4925" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="2.8575" y1="1.27" x2="2.54" y2="0.9525" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-0.9525" x2="2.8575" y2="-1.27" width="0.1524" layer="21"/>
<wire x1="3.4925" y1="-1.27" x2="2.8575" y2="-1.27" width="0.1524" layer="21"/>
<pad name="1" x="-3.175" y="0" drill="0.65" diameter="1.016" shape="long" rot="R90"/>
<pad name="2" x="-1.905" y="0" drill="0.65" diameter="1.016" shape="long" rot="R90"/>
<pad name="3" x="-0.635" y="0" drill="0.65" diameter="1.016" shape="long" rot="R90"/>
<pad name="4" x="0.635" y="0" drill="0.65" diameter="1.016" shape="long" rot="R90"/>
<pad name="5" x="1.905" y="0" drill="0.65" diameter="1.016" shape="long" rot="R90"/>
<pad name="6" x="3.175" y="0" drill="0.65" diameter="1.016" shape="long" rot="R90"/>
<text x="-4.2037" y="1.8288" size="1.27" layer="25" ratio="10">&gt;NAME</text>
<text x="-4.445" y="-2.8575" size="1.27" layer="27">&gt;VALUE</text>
</package>
<package name="1X05/90" urn="urn:adsk.eagle:footprint:22355/1" locally_modified="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-6.35" y1="-1.905" x2="-3.81" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="-1.905" x2="-3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="0.635" x2="-6.35" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-6.35" y1="0.635" x2="-6.35" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="6.985" x2="-5.08" y2="1.27" width="0.762" layer="21"/>
<wire x1="-3.81" y1="-1.905" x2="-1.27" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="-1.905" x2="-1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="0.635" x2="-3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="6.985" x2="-2.54" y2="1.27" width="0.762" layer="21"/>
<wire x1="-1.27" y1="-1.905" x2="1.27" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-1.905" x2="1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="0.635" x2="-1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="6.985" x2="0" y2="1.27" width="0.762" layer="21"/>
<wire x1="1.27" y1="-1.905" x2="3.81" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="3.81" y1="-1.905" x2="3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="3.81" y1="0.635" x2="1.27" y2="0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="6.985" x2="2.54" y2="1.27" width="0.762" layer="21"/>
<wire x1="3.81" y1="-1.905" x2="6.35" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="6.35" y1="-1.905" x2="6.35" y2="0.635" width="0.1524" layer="21"/>
<wire x1="6.35" y1="0.635" x2="3.81" y2="0.635" width="0.1524" layer="21"/>
<wire x1="5.08" y1="6.985" x2="5.08" y2="1.27" width="0.762" layer="21"/>
<pad name="1" x="-5.08" y="-3.81" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="2" x="-2.54" y="-3.81" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="3" x="0" y="-3.81" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="4" x="2.54" y="-3.81" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="5" x="5.08" y="-3.81" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<text x="-6.985" y="-3.81" size="1.27" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="8.255" y="-3.81" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-5.461" y1="0.635" x2="-4.699" y2="1.143" layer="21"/>
<rectangle x1="-2.921" y1="0.635" x2="-2.159" y2="1.143" layer="21"/>
<rectangle x1="-0.381" y1="0.635" x2="0.381" y2="1.143" layer="21"/>
<rectangle x1="2.159" y1="0.635" x2="2.921" y2="1.143" layer="21"/>
<rectangle x1="4.699" y1="0.635" x2="5.461" y2="1.143" layer="21"/>
<rectangle x1="-5.461" y1="-2.921" x2="-4.699" y2="-1.905" layer="21"/>
<rectangle x1="-2.921" y1="-2.921" x2="-2.159" y2="-1.905" layer="21"/>
<rectangle x1="-0.381" y1="-2.921" x2="0.381" y2="-1.905" layer="21"/>
<rectangle x1="2.159" y1="-2.921" x2="2.921" y2="-1.905" layer="21"/>
<rectangle x1="4.699" y1="-2.921" x2="5.461" y2="-1.905" layer="21"/>
</package>
<package name="1X06/90" urn="urn:adsk.eagle:footprint:22362/1" locally_modified="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<wire x1="-7.62" y1="-1.905" x2="-5.08" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="-1.905" x2="-5.08" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-5.08" y1="0.635" x2="-7.62" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-7.62" y1="0.635" x2="-7.62" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-6.35" y1="6.985" x2="-6.35" y2="1.27" width="0.762" layer="21"/>
<wire x1="-5.08" y1="-1.905" x2="-2.54" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="-1.905" x2="-2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0.635" x2="-5.08" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-3.81" y1="6.985" x2="-3.81" y2="1.27" width="0.762" layer="21"/>
<wire x1="-2.54" y1="-1.905" x2="0" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="0" y1="-1.905" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="0" y1="0.635" x2="-2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="-1.27" y1="6.985" x2="-1.27" y2="1.27" width="0.762" layer="21"/>
<wire x1="0" y1="-1.905" x2="2.54" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="2.54" y1="-1.905" x2="2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="2.54" y1="0.635" x2="0" y2="0.635" width="0.1524" layer="21"/>
<wire x1="1.27" y1="6.985" x2="1.27" y2="1.27" width="0.762" layer="21"/>
<wire x1="2.54" y1="-1.905" x2="5.08" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="5.08" y1="-1.905" x2="5.08" y2="0.635" width="0.1524" layer="21"/>
<wire x1="5.08" y1="0.635" x2="2.54" y2="0.635" width="0.1524" layer="21"/>
<wire x1="3.81" y1="6.985" x2="3.81" y2="1.27" width="0.762" layer="21"/>
<wire x1="5.08" y1="-1.905" x2="7.62" y2="-1.905" width="0.1524" layer="21"/>
<wire x1="7.62" y1="-1.905" x2="7.62" y2="0.635" width="0.1524" layer="21"/>
<wire x1="7.62" y1="0.635" x2="5.08" y2="0.635" width="0.1524" layer="21"/>
<wire x1="6.35" y1="6.985" x2="6.35" y2="1.27" width="0.762" layer="21"/>
<pad name="1" x="-6.35" y="-3.81" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="2" x="-3.81" y="-3.81" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="3" x="-1.27" y="-3.81" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="4" x="1.27" y="-3.81" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="5" x="3.81" y="-3.81" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<pad name="6" x="6.35" y="-3.81" drill="1.016" diameter="1.5748" shape="octagon" rot="R90"/>
<text x="-8.255" y="-3.81" size="1.27" layer="25" ratio="10" rot="R90">&gt;NAME</text>
<text x="9.525" y="-3.81" size="1.27" layer="27" rot="R90">&gt;VALUE</text>
<rectangle x1="-6.731" y1="0.635" x2="-5.969" y2="1.143" layer="21"/>
<rectangle x1="-4.191" y1="0.635" x2="-3.429" y2="1.143" layer="21"/>
<rectangle x1="-1.651" y1="0.635" x2="-0.889" y2="1.143" layer="21"/>
<rectangle x1="0.889" y1="0.635" x2="1.651" y2="1.143" layer="21"/>
<rectangle x1="3.429" y1="0.635" x2="4.191" y2="1.143" layer="21"/>
<rectangle x1="5.969" y1="0.635" x2="6.731" y2="1.143" layer="21"/>
<rectangle x1="-6.731" y1="-2.921" x2="-5.969" y2="-1.905" layer="21"/>
<rectangle x1="-4.191" y1="-2.921" x2="-3.429" y2="-1.905" layer="21"/>
<rectangle x1="-1.651" y1="-2.921" x2="-0.889" y2="-1.905" layer="21"/>
<rectangle x1="0.889" y1="-2.921" x2="1.651" y2="-1.905" layer="21"/>
<rectangle x1="3.429" y1="-2.921" x2="4.191" y2="-1.905" layer="21"/>
<rectangle x1="5.969" y1="-2.921" x2="6.731" y2="-1.905" layer="21"/>
</package>
</packages>
<packages3d>
<package3d name="1X06" urn="urn:adsk.eagle:package:22472/2" type="model">
<description>PIN HEADER</description>
<packageinstances>
<packageinstance name="1X06"/>
</packageinstances>
</package3d>
<package3d name="1X05" urn="urn:adsk.eagle:package:22469/2" type="model">
<description>PIN HEADER</description>
<packageinstances>
<packageinstance name="1X05"/>
</packageinstances>
</package3d>
<package3d name="1_05X2MM" urn="urn:adsk.eagle:package:22466/2" type="model">
<description>CON-M-1X5-200</description>
<packageinstances>
<packageinstance name="1_05X2MM"/>
</packageinstances>
</package3d>
<package3d name="1X05/90" urn="urn:adsk.eagle:package:22467/2" type="model">
<description>PIN HEADER</description>
<packageinstances>
<packageinstance name="1X05/90"/>
</packageinstances>
</package3d>
<package3d name="1X06/90" urn="urn:adsk.eagle:package:22475/2" type="model">
<description>PIN HEADER</description>
<packageinstances>
<packageinstance name="1X06/90"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="PINHD6">
<wire x1="-6.35" y1="-7.62" x2="1.27" y2="-7.62" width="0.4064" layer="94"/>
<wire x1="1.27" y1="-7.62" x2="1.27" y2="10.16" width="0.4064" layer="94"/>
<wire x1="1.27" y1="10.16" x2="-6.35" y2="10.16" width="0.4064" layer="94"/>
<wire x1="-6.35" y1="10.16" x2="-6.35" y2="-7.62" width="0.4064" layer="94"/>
<text x="-6.35" y="10.795" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-10.16" size="1.778" layer="96">&gt;VALUE</text>
<pin name="1" x="-2.54" y="7.62" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="2" x="-2.54" y="5.08" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="3" x="-2.54" y="2.54" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="4" x="-2.54" y="0" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="5" x="-2.54" y="-2.54" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="6" x="-2.54" y="-5.08" visible="pad" length="short" direction="pas" function="dot"/>
</symbol>
<symbol name="PINHD5">
<wire x1="-6.35" y1="-7.62" x2="1.27" y2="-7.62" width="0.4064" layer="94"/>
<wire x1="1.27" y1="-7.62" x2="1.27" y2="7.62" width="0.4064" layer="94"/>
<wire x1="1.27" y1="7.62" x2="-6.35" y2="7.62" width="0.4064" layer="94"/>
<wire x1="-6.35" y1="7.62" x2="-6.35" y2="-7.62" width="0.4064" layer="94"/>
<text x="-6.35" y="8.255" size="1.778" layer="95">&gt;NAME</text>
<text x="-6.35" y="-10.16" size="1.778" layer="96">&gt;VALUE</text>
<pin name="1" x="-2.54" y="5.08" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="2" x="-2.54" y="2.54" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="3" x="-2.54" y="0" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="4" x="-2.54" y="-2.54" visible="pad" length="short" direction="pas" function="dot"/>
<pin name="5" x="-2.54" y="-5.08" visible="pad" length="short" direction="pas" function="dot"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="PINHD-1X6" prefix="JP" uservalue="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<gates>
<gate name="A" symbol="PINHD6" x="0" y="-2.54"/>
</gates>
<devices>
<device name="" package="1X06">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
<connect gate="A" pin="4" pad="4"/>
<connect gate="A" pin="5" pad="5"/>
<connect gate="A" pin="6" pad="6"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:22472/2"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="79" constant="no"/>
</technology>
</technologies>
</device>
<device name="/90" package="1X06/90">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
<connect gate="A" pin="4" pad="4"/>
<connect gate="A" pin="5" pad="5"/>
<connect gate="A" pin="6" pad="6"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:22475/2"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="10" constant="no"/>
</technology>
</technologies>
</device>
<device name="1.27" package="1X6_1.27">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
<connect gate="A" pin="4" pad="4"/>
<connect gate="A" pin="5" pad="5"/>
<connect gate="A" pin="6" pad="6"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="PINHD-1X5" prefix="JP" uservalue="yes">
<description>&lt;b&gt;PIN HEADER&lt;/b&gt;</description>
<gates>
<gate name="A" symbol="PINHD5" x="0" y="0"/>
</gates>
<devices>
<device name="" package="1X05">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
<connect gate="A" pin="4" pad="4"/>
<connect gate="A" pin="5" pad="5"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:22469/2"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="69" constant="no"/>
</technology>
</technologies>
</device>
<device name="/90" package="1X05/90">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
<connect gate="A" pin="4" pad="4"/>
<connect gate="A" pin="5" pad="5"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:22467/2"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="9" constant="no"/>
</technology>
</technologies>
</device>
<device name="5X2MM" package="1_05X2MM">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
<connect gate="A" pin="4" pad="4"/>
<connect gate="A" pin="5" pad="5"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:22466/2"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="0" constant="no"/>
</technology>
</technologies>
</device>
<device name="1X05_1.27" package="1X05_1.27">
<connects>
<connect gate="A" pin="1" pad="1"/>
<connect gate="A" pin="2" pad="2"/>
<connect gate="A" pin="3" pad="3"/>
<connect gate="A" pin="4" pad="4"/>
<connect gate="A" pin="5" pad="5"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="1X05_1.27_SMT" package="1X05_1.27SMT">
<connects>
<connect gate="A" pin="1" pad="P$1"/>
<connect gate="A" pin="2" pad="P$2"/>
<connect gate="A" pin="3" pad="P$3"/>
<connect gate="A" pin="4" pad="P$4"/>
<connect gate="A" pin="5" pad="P$5"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="PINHD-1X5_AUTO">
<gates>
<gate name="G$1" symbol="PINHD5" x="2.54" y="0"/>
</gates>
<devices>
<device name="" package="1X5_AUTO">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
</connects>
<technologies>
<technology name=""/>
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
<part name="JP2" library="pinhead2" deviceset="PINHD-1X6" device="" package3d_urn="urn:adsk.eagle:package:22472/2"/>
<part name="JP11" library="pinhead2" deviceset="PINHD-1X5" device="" package3d_urn="urn:adsk.eagle:package:22469/2"/>
<part name="U$1" library="pinhead2" deviceset="PINHD-1X5_AUTO" device=""/>
<part name="U$2" library="pinhead2" deviceset="PINHD-1X5_AUTO" device=""/>
<part name="U$3" library="pinhead2" deviceset="PINHD-1X5_AUTO" device=""/>
<part name="U$4" library="pinhead2" deviceset="PINHD-1X5_AUTO" device=""/>
<part name="U$5" library="pinhead2" deviceset="PINHD-1X5_AUTO" device=""/>
<part name="U$6" library="pinhead2" deviceset="PINHD-1X5_AUTO" device=""/>
<part name="U$7" library="pinhead2" deviceset="PINHD-1X5_AUTO" device=""/>
<part name="U$8" library="pinhead2" deviceset="PINHD-1X5_AUTO" device=""/>
<part name="JP1" library="pinhead2" deviceset="PINHD-1X6" device="" package3d_urn="urn:adsk.eagle:package:22472/2"/>
<part name="JP3" library="pinhead2" deviceset="PINHD-1X5" device="" package3d_urn="urn:adsk.eagle:package:22469/2"/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="JP2" gate="A" x="63.5" y="33.02" smashed="yes" rot="R180">
<attribute name="NAME" x="69.85" y="22.225" size="1.778" layer="95" rot="R180"/>
<attribute name="VALUE" x="69.85" y="43.18" size="1.778" layer="96" rot="R180"/>
</instance>
<instance part="JP11" gate="A" x="124.46" y="35.56" smashed="yes">
<attribute name="NAME" x="118.11" y="43.815" size="1.778" layer="95"/>
<attribute name="VALUE" x="118.11" y="25.4" size="1.778" layer="96"/>
</instance>
<instance part="U$1" gate="G$1" x="93.98" y="45.72" smashed="yes" rot="R90">
<attribute name="NAME" x="85.725" y="39.37" size="1.778" layer="95" rot="R90"/>
<attribute name="VALUE" x="104.14" y="39.37" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="U$2" gate="G$1" x="93.98" y="38.1" smashed="yes" rot="R90">
<attribute name="NAME" x="85.725" y="31.75" size="1.778" layer="95" rot="R90"/>
<attribute name="VALUE" x="104.14" y="31.75" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="U$3" gate="G$1" x="93.98" y="30.48" smashed="yes" rot="R90">
<attribute name="NAME" x="85.725" y="24.13" size="1.778" layer="95" rot="R90"/>
<attribute name="VALUE" x="104.14" y="24.13" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="U$4" gate="G$1" x="93.98" y="22.86" smashed="yes" rot="R90">
<attribute name="NAME" x="85.725" y="16.51" size="1.778" layer="95" rot="R90"/>
<attribute name="VALUE" x="104.14" y="16.51" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="U$5" gate="G$1" x="93.98" y="15.24" smashed="yes" rot="R90">
<attribute name="NAME" x="85.725" y="8.89" size="1.778" layer="95" rot="R90"/>
<attribute name="VALUE" x="104.14" y="8.89" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="U$6" gate="G$1" x="93.98" y="7.62" smashed="yes" rot="R90">
<attribute name="NAME" x="85.725" y="1.27" size="1.778" layer="95" rot="R90"/>
<attribute name="VALUE" x="104.14" y="1.27" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="U$7" gate="G$1" x="93.98" y="0" smashed="yes" rot="R90">
<attribute name="NAME" x="85.725" y="-6.35" size="1.778" layer="95" rot="R90"/>
<attribute name="VALUE" x="104.14" y="-6.35" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="U$8" gate="G$1" x="93.98" y="-7.62" smashed="yes" rot="R90">
<attribute name="NAME" x="85.725" y="-13.97" size="1.778" layer="95" rot="R90"/>
<attribute name="VALUE" x="104.14" y="-13.97" size="1.778" layer="96" rot="R90"/>
</instance>
<instance part="JP1" gate="A" x="71.12" y="33.02" smashed="yes" rot="R180">
<attribute name="NAME" x="77.47" y="22.225" size="1.778" layer="95" rot="R180"/>
<attribute name="VALUE" x="77.47" y="43.18" size="1.778" layer="96" rot="R180"/>
</instance>
<instance part="JP3" gate="A" x="116.84" y="35.56" smashed="yes">
<attribute name="NAME" x="110.49" y="43.815" size="1.778" layer="95"/>
<attribute name="VALUE" x="110.49" y="25.4" size="1.778" layer="96"/>
</instance>
</instances>
<busses>
</busses>
<nets>
<net name="GND" class="0">
<segment>
<pinref part="JP11" gate="A" pin="4"/>
<pinref part="JP3" gate="A" pin="4"/>
<wire x1="121.92" y1="33.02" x2="114.3" y2="33.02" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$1" class="0">
<segment>
<pinref part="JP2" gate="A" pin="5"/>
<pinref part="JP1" gate="A" pin="5"/>
<wire x1="66.04" y1="35.56" x2="73.66" y2="35.56" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="JP1" gate="A" pin="4"/>
<pinref part="JP2" gate="A" pin="4"/>
<wire x1="73.66" y1="33.02" x2="66.04" y2="33.02" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="JP2" gate="A" pin="3"/>
<pinref part="JP1" gate="A" pin="3"/>
<wire x1="66.04" y1="30.48" x2="73.66" y2="30.48" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="JP1" gate="A" pin="2"/>
<pinref part="JP2" gate="A" pin="2"/>
<wire x1="73.66" y1="27.94" x2="66.04" y2="27.94" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="JP2" gate="A" pin="1"/>
<pinref part="JP1" gate="A" pin="1"/>
<wire x1="66.04" y1="25.4" x2="73.66" y2="25.4" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="JP3" gate="A" pin="5"/>
<pinref part="JP11" gate="A" pin="5"/>
<wire x1="114.3" y1="30.48" x2="121.92" y2="30.48" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<pinref part="JP11" gate="A" pin="2"/>
<pinref part="JP3" gate="A" pin="2"/>
<wire x1="121.92" y1="38.1" x2="114.3" y2="38.1" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="JP3" gate="A" pin="1"/>
<pinref part="JP11" gate="A" pin="1"/>
<wire x1="114.3" y1="40.64" x2="121.92" y2="40.64" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<pinref part="JP11" gate="A" pin="3"/>
<wire x1="114.3" y1="35.56" x2="121.92" y2="35.56" width="0.1524" layer="91"/>
<pinref part="JP3" gate="A" pin="3"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<pinref part="JP2" gate="A" pin="6"/>
<pinref part="JP1" gate="A" pin="6"/>
<wire x1="73.66" y1="38.1" x2="66.04" y2="38.1" width="0.1524" layer="91"/>
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
