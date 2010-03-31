<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan3e" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="dp" />
        <signal name="seg(6:0)" />
        <signal name="an(3:0)" />
        <signal name="clk" />
        <signal name="XLXN_24" />
        <signal name="XLXN_27(7:0)" />
        <signal name="RsRx" />
        <signal name="RsTx" />
        <signal name="MemAdr(23:1)" />
        <signal name="XLXN_36" />
        <signal name="MemWR" />
        <signal name="MemOE" />
        <signal name="MemDB(15:0)" />
        <signal name="XLXN_44(15:0)" />
        <signal name="btn0" />
        <signal name="FlashRp" />
        <signal name="RamCS" />
        <signal name="FlashCS" />
        <signal name="btn1" />
        <signal name="btn2" />
        <signal name="Led0" />
        <signal name="XLXN_77" />
        <signal name="XLXN_79" />
        <port polarity="Output" name="dp" />
        <port polarity="Output" name="seg(6:0)" />
        <port polarity="Output" name="an(3:0)" />
        <port polarity="Input" name="clk" />
        <port polarity="Input" name="RsRx" />
        <port polarity="Output" name="RsTx" />
        <port polarity="Output" name="MemAdr(23:1)" />
        <port polarity="Output" name="MemWR" />
        <port polarity="Output" name="MemOE" />
        <port polarity="Input" name="MemDB(15:0)" />
        <port polarity="Input" name="btn0" />
        <port polarity="Output" name="FlashRp" />
        <port polarity="Output" name="RamCS" />
        <port polarity="Output" name="FlashCS" />
        <port polarity="Input" name="btn1" />
        <port polarity="Input" name="btn2" />
        <port polarity="Output" name="Led0" />
        <blockdef name="debug">
            <timestamp>2010-3-30T3:17:58</timestamp>
            <rect width="64" x="0" y="20" height="24" />
            <line x2="0" y1="32" y2="32" x1="64" />
            <rect width="64" x="0" y="84" height="24" />
            <line x2="0" y1="96" y2="96" x1="64" />
            <rect width="64" x="320" y="20" height="24" />
            <line x2="384" y1="32" y2="32" x1="320" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
            <rect width="64" x="320" y="-140" height="24" />
            <line x2="384" y1="-128" y2="-128" x1="320" />
            <rect width="256" x="64" y="-256" height="384" />
        </blockdef>
        <blockdef name="clkdiv">
            <timestamp>2010-3-30T2:53:57</timestamp>
            <rect width="256" x="64" y="-256" height="256" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
            <line x2="384" y1="-160" y2="-160" x1="320" />
            <line x2="384" y1="-96" y2="-96" x1="320" />
            <line x2="384" y1="-32" y2="-32" x1="320" />
        </blockdef>
        <blockdef name="Rs232RefComp">
            <timestamp>2010-3-30T23:59:34</timestamp>
            <rect width="256" x="64" y="-448" height="448" />
            <line x2="0" y1="-416" y2="-416" x1="64" />
            <line x2="0" y1="-352" y2="-352" x1="64" />
            <line x2="0" y1="-288" y2="-288" x1="64" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <rect width="64" x="0" y="-108" height="24" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="384" y1="-416" y2="-416" x1="320" />
            <line x2="384" y1="-352" y2="-352" x1="320" />
            <line x2="384" y1="-288" y2="-288" x1="320" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
            <rect width="64" x="320" y="-172" height="24" />
            <line x2="384" y1="-160" y2="-160" x1="320" />
            <line x2="384" y1="-96" y2="-96" x1="320" />
            <line x2="384" y1="-32" y2="-32" x1="320" />
        </blockdef>
        <blockdef name="debugen">
            <timestamp>2010-3-31T2:51:8</timestamp>
            <line x2="0" y1="32" y2="32" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <rect width="64" x="0" y="-44" height="24" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="432" y1="-160" y2="-160" x1="368" />
            <rect width="64" x="368" y="-108" height="24" />
            <line x2="432" y1="-96" y2="-96" x1="368" />
            <rect width="64" x="368" y="-44" height="24" />
            <line x2="432" y1="-32" y2="-32" x1="368" />
            <rect width="304" x="64" y="-192" height="256" />
        </blockdef>
        <blockdef name="debounce">
            <timestamp>2010-3-31T2:55:41</timestamp>
            <rect width="256" x="64" y="-128" height="128" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-96" y2="-96" x1="320" />
        </blockdef>
        <blockdef name="constant">
            <timestamp>2006-1-1T10:10:10</timestamp>
            <rect width="112" x="0" y="0" height="64" />
            <line x2="112" y1="32" y2="32" x1="144" />
        </blockdef>
        <blockdef name="buf">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-32" y2="-32" x1="0" />
            <line x2="128" y1="-32" y2="-32" x1="224" />
            <line x2="128" y1="0" y2="-32" x1="64" />
            <line x2="64" y1="-32" y2="-64" x1="128" />
            <line x2="64" y1="-64" y2="0" x1="64" />
        </blockdef>
        <block symbolname="clkdiv" name="XLXI_11">
            <blockpin signalname="clk" name="clk" />
            <blockpin name="clk6" />
            <blockpin signalname="XLXN_77" name="clk8" />
            <blockpin signalname="XLXN_79" name="clkps" />
            <blockpin signalname="XLXN_24" name="clkled" />
        </block>
        <block symbolname="debug" name="XLXI_13">
            <blockpin signalname="XLXN_24" name="clk" />
            <blockpin signalname="dp" name="dot" />
            <blockpin signalname="seg(6:0)" name="seg(6:0)" />
            <blockpin signalname="XLXN_44(15:0)" name="data0(15:0)" />
            <blockpin name="data1(3:0)" />
            <blockpin signalname="an(3:0)" name="an(3:0)" />
        </block>
        <block symbolname="Rs232RefComp" name="XLXI_14">
            <blockpin signalname="RsRx" name="RXD" />
            <blockpin signalname="clk" name="CLK" />
            <blockpin name="RD" />
            <blockpin signalname="XLXN_36" name="WR" />
            <blockpin name="RST" />
            <blockpin signalname="XLXN_27(7:0)" name="DBIN(7:0)" />
            <blockpin signalname="RsTx" name="TXD" />
            <blockpin name="PE" />
            <blockpin name="FE" />
            <blockpin name="OE" />
            <blockpin signalname="XLXN_27(7:0)" name="DBOUT(7:0)" />
            <blockpin signalname="XLXN_36" name="RDA" />
            <blockpin name="TBE" />
        </block>
        <block symbolname="debugen" name="XLXI_32">
            <blockpin signalname="XLXN_77" name="clk" />
            <blockpin signalname="XLXN_79" name="latch" />
            <blockpin signalname="MemDB(15:0)" name="MemDB(15:0)" />
            <blockpin signalname="MemOE" name="MemOE" />
            <blockpin signalname="XLXN_44(15:0)" name="data0(15:0)" />
            <blockpin signalname="MemAdr(23:1)" name="MemAdr(23:1)" />
        </block>
        <block symbolname="debounce" name="XLXI_33">
            <blockpin signalname="clk" name="clk" />
            <blockpin signalname="btn0" name="insig" />
            <blockpin name="outsig" />
        </block>
        <block symbolname="constant" name="XLXI_36">
            <attr value="1" name="CValue">
                <trait delete="all:1 sym:0" />
                <trait editname="all:1 sch:0" />
                <trait valuetype="BitVector 32 Hexadecimal" />
            </attr>
            <blockpin signalname="RamCS" name="O" />
        </block>
        <block symbolname="constant" name="XLXI_38">
            <attr value="1" name="CValue">
                <trait delete="all:1 sym:0" />
                <trait editname="all:1 sch:0" />
                <trait valuetype="BitVector 32 Hexadecimal" />
            </attr>
            <blockpin signalname="FlashRp" name="O" />
        </block>
        <block symbolname="constant" name="XLXI_39">
            <attr value="1" name="CValue">
                <trait delete="all:1 sym:0" />
                <trait editname="all:1 sch:0" />
                <trait valuetype="BitVector 32 Hexadecimal" />
            </attr>
            <blockpin signalname="MemWR" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_45">
            <blockpin signalname="MemOE" name="I" />
            <blockpin signalname="FlashCS" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_46">
            <blockpin signalname="XLXN_79" name="I" />
            <blockpin signalname="Led0" name="O" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="3520" height="2720">
        <branch name="dp">
            <wire x2="1568" y1="896" y2="896" x1="1392" />
        </branch>
        <branch name="seg(6:0)">
            <wire x2="1568" y1="992" y2="992" x1="1392" />
        </branch>
        <iomarker fontsize="28" x="1568" y="896" name="dp" orien="R0" />
        <iomarker fontsize="28" x="1568" y="992" name="seg(6:0)" orien="R0" />
        <instance x="416" y="912" name="XLXI_11" orien="R0">
        </instance>
        <branch name="an(3:0)">
            <wire x2="1600" y1="1152" y2="1152" x1="1392" />
        </branch>
        <iomarker fontsize="28" x="1600" y="1152" name="an(3:0)" orien="R0" />
        <iomarker fontsize="28" x="272" y="688" name="clk" orien="R180" />
        <branch name="XLXN_24">
            <wire x2="896" y1="880" y2="880" x1="800" />
            <wire x2="896" y1="880" y2="896" x1="896" />
            <wire x2="1008" y1="896" y2="896" x1="896" />
        </branch>
        <instance x="1008" y="1120" name="XLXI_13" orien="R0">
        </instance>
        <instance x="1296" y="1968" name="XLXI_14" orien="R0">
        </instance>
        <branch name="XLXN_27(7:0)">
            <wire x2="1296" y1="1872" y2="1872" x1="1232" />
            <wire x2="1232" y1="1872" y2="2032" x1="1232" />
            <wire x2="1760" y1="2032" y2="2032" x1="1232" />
            <wire x2="1760" y1="1808" y2="1808" x1="1680" />
            <wire x2="1760" y1="1808" y2="2032" x1="1760" />
        </branch>
        <branch name="RsRx">
            <wire x2="1296" y1="1552" y2="1552" x1="1072" />
        </branch>
        <branch name="RsTx">
            <wire x2="1968" y1="1552" y2="1552" x1="1680" />
        </branch>
        <iomarker fontsize="28" x="1072" y="1552" name="RsRx" orien="R180" />
        <iomarker fontsize="28" x="1968" y="1552" name="RsTx" orien="R0" />
        <branch name="MemAdr(23:1)">
            <wire x2="944" y1="1232" y2="1232" x1="848" />
            <wire x2="944" y1="1232" y2="1280" x1="944" />
            <wire x2="1376" y1="1280" y2="1280" x1="944" />
        </branch>
        <iomarker fontsize="28" x="288" y="1216" name="MemDB(15:0)" orien="R180" />
        <iomarker fontsize="28" x="1376" y="1280" name="MemAdr(23:1)" orien="R0" />
        <branch name="XLXN_36">
            <wire x2="1296" y1="1744" y2="1744" x1="1248" />
            <wire x2="1248" y1="1744" y2="2016" x1="1248" />
            <wire x2="1744" y1="2016" y2="2016" x1="1248" />
            <wire x2="1744" y1="1872" y2="1872" x1="1680" />
            <wire x2="1744" y1="1872" y2="2016" x1="1744" />
        </branch>
        <branch name="MemDB(15:0)">
            <wire x2="320" y1="1216" y2="1216" x1="288" />
            <wire x2="320" y1="1216" y2="1232" x1="320" />
            <wire x2="416" y1="1232" y2="1232" x1="320" />
        </branch>
        <branch name="XLXN_44(15:0)">
            <wire x2="928" y1="1168" y2="1168" x1="848" />
            <wire x2="1008" y1="1152" y2="1152" x1="928" />
            <wire x2="928" y1="1152" y2="1168" x1="928" />
        </branch>
        <instance x="416" y="1264" name="XLXI_32" orien="R0">
        </instance>
        <instance x="192" y="544" name="XLXI_33" orien="R0">
        </instance>
        <branch name="btn0">
            <wire x2="192" y1="512" y2="512" x1="96" />
        </branch>
        <iomarker fontsize="28" x="96" y="512" name="btn0" orien="R180" />
        <branch name="FlashRp">
            <wire x2="960" y1="1856" y2="1856" x1="800" />
        </branch>
        <iomarker fontsize="28" x="976" y="1696" name="RamCS" orien="R0" />
        <branch name="RamCS">
            <wire x2="976" y1="1696" y2="1696" x1="800" />
        </branch>
        <instance x="656" y="1664" name="XLXI_36" orien="R0">
        </instance>
        <instance x="656" y="1824" name="XLXI_38" orien="R0">
        </instance>
        <iomarker fontsize="28" x="960" y="1776" name="FlashCS" orien="R0" />
        <iomarker fontsize="28" x="960" y="1856" name="FlashRp" orien="R0" />
        <instance x="656" y="1904" name="XLXI_39" orien="R0">
        </instance>
        <branch name="MemWR">
            <wire x2="960" y1="1936" y2="1936" x1="800" />
        </branch>
        <iomarker fontsize="28" x="960" y="1936" name="MemWR" orien="R0" />
        <branch name="btn1">
            <wire x2="272" y1="1808" y2="1808" x1="224" />
        </branch>
        <iomarker fontsize="28" x="224" y="1808" name="btn1" orien="R180" />
        <iomarker fontsize="28" x="160" y="2208" name="btn2" orien="R180" />
        <branch name="Led0">
            <wire x2="1040" y1="1472" y2="1472" x1="960" />
        </branch>
        <iomarker fontsize="28" x="1040" y="1472" name="Led0" orien="R0" />
        <branch name="XLXN_77">
            <wire x2="384" y1="992" y2="1104" x1="384" />
            <wire x2="416" y1="1104" y2="1104" x1="384" />
            <wire x2="848" y1="992" y2="992" x1="384" />
            <wire x2="848" y1="752" y2="752" x1="800" />
            <wire x2="848" y1="752" y2="992" x1="848" />
        </branch>
        <branch name="XLXN_79">
            <wire x2="416" y1="1296" y2="1296" x1="336" />
            <wire x2="336" y1="1296" y2="1392" x1="336" />
            <wire x2="720" y1="1392" y2="1392" x1="336" />
            <wire x2="728" y1="1392" y2="1392" x1="720" />
            <wire x2="912" y1="1392" y2="1392" x1="728" />
            <wire x2="720" y1="1392" y2="1472" x1="720" />
            <wire x2="736" y1="1472" y2="1472" x1="720" />
            <wire x2="912" y1="816" y2="816" x1="800" />
            <wire x2="912" y1="816" y2="1392" x1="912" />
        </branch>
        <branch name="FlashCS">
            <attrtext style="alignment:SOFT-TCENTER" attrname="Name" x="800" y="1776" type="branch" />
            <wire x2="608" y1="1904" y2="1904" x1="560" />
            <wire x2="640" y1="1904" y2="1904" x1="608" />
            <wire x2="640" y1="1904" y2="2016" x1="640" />
            <wire x2="880" y1="2016" y2="2016" x1="640" />
            <wire x2="800" y1="1744" y2="1776" x1="800" />
            <wire x2="960" y1="1776" y2="1776" x1="800" />
            <wire x2="880" y1="1744" y2="1744" x1="800" />
            <wire x2="880" y1="1744" y2="2016" x1="880" />
        </branch>
        <instance x="384" y="1936" name="XLXI_45" orien="R0" />
        <iomarker fontsize="28" x="992" y="2304" name="MemOE" orien="R0" />
        <branch name="btn2">
            <wire x2="272" y1="2208" y2="2208" x1="160" />
        </branch>
        <branch name="clk">
            <wire x2="192" y1="448" y2="448" x1="112" />
            <wire x2="112" y1="448" y2="608" x1="112" />
            <wire x2="320" y1="608" y2="608" x1="112" />
            <wire x2="320" y1="608" y2="688" x1="320" />
            <wire x2="368" y1="688" y2="688" x1="320" />
            <wire x2="416" y1="688" y2="688" x1="368" />
            <wire x2="368" y1="688" y2="1616" x1="368" />
            <wire x2="1296" y1="1616" y2="1616" x1="368" />
            <wire x2="320" y1="688" y2="688" x1="272" />
        </branch>
        <instance x="736" y="1504" name="XLXI_46" orien="R0" />
        <branch name="MemOE">
            <wire x2="256" y1="1552" y2="1904" x1="256" />
            <wire x2="336" y1="1904" y2="1904" x1="256" />
            <wire x2="384" y1="1904" y2="1904" x1="336" />
            <wire x2="336" y1="1904" y2="2064" x1="336" />
            <wire x2="688" y1="2064" y2="2064" x1="336" />
            <wire x2="688" y1="2064" y2="2304" x1="688" />
            <wire x2="992" y1="2304" y2="2304" x1="688" />
            <wire x2="688" y1="1552" y2="1552" x1="256" />
            <wire x2="896" y1="1408" y2="1408" x1="672" />
            <wire x2="672" y1="1408" y2="1472" x1="672" />
            <wire x2="688" y1="1472" y2="1472" x1="672" />
            <wire x2="688" y1="1472" y2="1552" x1="688" />
            <wire x2="896" y1="1104" y2="1104" x1="848" />
            <wire x2="896" y1="1104" y2="1408" x1="896" />
        </branch>
    </sheet>
</drawing>