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
        <signal name="XLXN_25" />
        <signal name="XLXN_26(15:0)" />
        <port polarity="Output" name="dp" />
        <port polarity="Output" name="seg(6:0)" />
        <port polarity="Output" name="an(3:0)" />
        <port polarity="Input" name="clk" />
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
        <blockdef name="debugen">
            <timestamp>2010-3-30T3:20:10</timestamp>
            <rect width="256" x="64" y="-64" height="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <rect width="64" x="320" y="-44" height="24" />
            <line x2="384" y1="-32" y2="-32" x1="320" />
        </blockdef>
        <block symbolname="clkdiv" name="XLXI_11">
            <blockpin signalname="clk" name="clk" />
            <blockpin name="clk6" />
            <blockpin name="clk8" />
            <blockpin signalname="XLXN_25" name="clkps" />
            <blockpin signalname="XLXN_24" name="clkled" />
        </block>
        <block symbolname="debugen" name="XLXI_12">
            <blockpin signalname="XLXN_25" name="clk" />
            <blockpin signalname="XLXN_26(15:0)" name="data0(15:0)" />
        </block>
        <block symbolname="debug" name="XLXI_13">
            <blockpin signalname="XLXN_24" name="clk" />
            <blockpin signalname="dp" name="dot" />
            <blockpin signalname="seg(6:0)" name="seg(6:0)" />
            <blockpin signalname="XLXN_26(15:0)" name="data0(15:0)" />
            <blockpin name="data1(3:0)" />
            <blockpin signalname="an(3:0)" name="an(3:0)" />
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
        <branch name="clk">
            <wire x2="416" y1="688" y2="688" x1="272" />
        </branch>
        <iomarker fontsize="28" x="272" y="688" name="clk" orien="R180" />
        <branch name="XLXN_24">
            <wire x2="896" y1="880" y2="880" x1="800" />
            <wire x2="896" y1="880" y2="896" x1="896" />
            <wire x2="1008" y1="896" y2="896" x1="896" />
        </branch>
        <branch name="XLXN_26(15:0)">
            <wire x2="1008" y1="1152" y2="1152" x1="784" />
        </branch>
        <instance x="400" y="1184" name="XLXI_12" orien="R0">
        </instance>
        <branch name="XLXN_25">
            <wire x2="400" y1="1152" y2="1152" x1="336" />
            <wire x2="336" y1="1152" y2="1264" x1="336" />
            <wire x2="816" y1="1264" y2="1264" x1="336" />
            <wire x2="816" y1="816" y2="816" x1="800" />
            <wire x2="816" y1="816" y2="1264" x1="816" />
        </branch>
        <instance x="1008" y="1120" name="XLXI_13" orien="R0">
        </instance>
    </sheet>
</drawing>