<?xml version="1.0" encoding="UTF-8"?>
<drawing version="7">
    <attr value="spartan3e" name="DeviceFamilyName">
        <trait delete="all:0" />
        <trait editname="all:0" />
        <trait edittrait="all:0" />
    </attr>
    <netlist>
        <signal name="sw(7:0)" />
        <signal name="btn(3:0)" />
        <signal name="XLXN_90" />
        <signal name="dp" />
        <signal name="seg(6:0)" />
        <signal name="an(3:0)" />
        <signal name="clk" />
        <signal name="Led(7:0)" />
        <signal name="XLXN_98" />
        <signal name="XLXN_99" />
        <signal name="XLXN_101" />
        <signal name="XLXN_105" />
        <signal name="XLXN_106" />
        <signal name="XLXN_107(15:0)" />
        <signal name="XLXN_108" />
        <signal name="XLXN_109" />
        <signal name="XLXN_110" />
        <signal name="XLXN_111(15:0)" />
        <signal name="XLXN_114" />
        <signal name="XLXN_116" />
        <signal name="MemOE" />
        <signal name="MemWR" />
        <signal name="FlashCS" />
        <signal name="RamCS" />
        <signal name="MemDB(15:0)" />
        <signal name="MemAdr(23:1)" />
        <signal name="FlashRp" />
        <signal name="leda(7:0)" />
        <signal name="leda(0)" />
        <signal name="leda(1)" />
        <signal name="leda(2)" />
        <signal name="Led(0)" />
        <signal name="Led(1)" />
        <signal name="Led(2)" />
        <signal name="Led(7)" />
        <signal name="Led(6)" />
        <signal name="Led(5)" />
        <signal name="XLXN_146" />
        <signal name="XLXN_147" />
        <signal name="XLXN_148(15:0)" />
        <port polarity="Input" name="sw(7:0)" />
        <port polarity="Input" name="btn(3:0)" />
        <port polarity="Output" name="dp" />
        <port polarity="Output" name="seg(6:0)" />
        <port polarity="Output" name="an(3:0)" />
        <port polarity="Input" name="clk" />
        <port polarity="Output" name="Led(7:0)" />
        <port polarity="Output" name="MemOE" />
        <port polarity="Output" name="MemWR" />
        <port polarity="Output" name="FlashCS" />
        <port polarity="Output" name="RamCS" />
        <port polarity="BiDirectional" name="MemDB(15:0)" />
        <port polarity="Output" name="MemAdr(23:1)" />
        <port polarity="Output" name="FlashRp" />
        <blockdef name="clkdiv">
            <timestamp>2010-3-31T22:4:3</timestamp>
            <line x2="384" y1="32" y2="32" x1="320" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
            <line x2="384" y1="-160" y2="-160" x1="320" />
            <line x2="384" y1="-96" y2="-96" x1="320" />
            <line x2="384" y1="-32" y2="-32" x1="320" />
            <rect width="256" x="64" y="-256" height="320" />
        </blockdef>
        <blockdef name="cpuvm2">
            <timestamp>2010-3-31T20:37:10</timestamp>
            <rect width="64" x="320" y="212" height="24" />
            <line x2="384" y1="224" y2="224" x1="320" />
            <line x2="0" y1="32" y2="32" x1="64" />
            <rect width="64" x="0" y="84" height="24" />
            <line x2="0" y1="96" y2="96" x1="64" />
            <rect width="64" x="0" y="148" height="24" />
            <line x2="0" y1="160" y2="160" x1="64" />
            <line x2="384" y1="32" y2="32" x1="320" />
            <rect width="64" x="320" y="84" height="24" />
            <line x2="384" y1="96" y2="96" x1="320" />
            <rect width="64" x="320" y="148" height="24" />
            <line x2="384" y1="160" y2="160" x1="320" />
            <line x2="0" y1="-672" y2="-672" x1="64" />
            <line x2="0" y1="-608" y2="-608" x1="64" />
            <line x2="0" y1="-544" y2="-544" x1="64" />
            <line x2="0" y1="-480" y2="-480" x1="64" />
            <line x2="0" y1="-416" y2="-416" x1="64" />
            <line x2="0" y1="-352" y2="-352" x1="64" />
            <line x2="0" y1="-288" y2="-288" x1="64" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="384" y1="-672" y2="-672" x1="320" />
            <line x2="384" y1="-592" y2="-592" x1="320" />
            <line x2="384" y1="-512" y2="-512" x1="320" />
            <line x2="384" y1="-432" y2="-432" x1="320" />
            <line x2="384" y1="-352" y2="-352" x1="320" />
            <line x2="384" y1="-272" y2="-272" x1="320" />
            <line x2="384" y1="-192" y2="-192" x1="320" />
            <rect width="64" x="320" y="-124" height="24" />
            <line x2="384" y1="-112" y2="-112" x1="320" />
            <rect width="256" x="64" y="-704" height="960" />
        </blockdef>
        <blockdef name="pullup">
            <timestamp>2000-1-1T10:10:10</timestamp>
            <line x2="64" y1="-108" y2="-128" x1="64" />
            <line x2="64" y1="-104" y2="-108" x1="80" />
            <line x2="80" y1="-88" y2="-104" x1="48" />
            <line x2="48" y1="-72" y2="-88" x1="80" />
            <line x2="80" y1="-56" y2="-72" x1="48" />
            <line x2="48" y1="-48" y2="-56" x1="64" />
            <line x2="64" y1="-32" y2="-48" x1="64" />
            <line x2="64" y1="-56" y2="-48" x1="48" />
            <line x2="48" y1="-72" y2="-56" x1="80" />
            <line x2="80" y1="-88" y2="-72" x1="48" />
            <line x2="48" y1="-104" y2="-88" x1="80" />
            <line x2="80" y1="-108" y2="-104" x1="64" />
            <line x2="64" y1="0" y2="-32" x1="64" />
            <line x2="32" y1="-128" y2="-128" x1="96" />
        </blockdef>
        <blockdef name="ppumemctr">
            <timestamp>2010-3-31T22:5:30</timestamp>
            <line x2="0" y1="96" y2="96" x1="64" />
            <rect width="64" x="320" y="20" height="24" />
            <line x2="384" y1="32" y2="32" x1="320" />
            <line x2="0" y1="-416" y2="-416" x1="64" />
            <line x2="0" y1="-352" y2="-352" x1="64" />
            <line x2="0" y1="-288" y2="-288" x1="64" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <line x2="384" y1="-416" y2="-416" x1="320" />
            <line x2="384" y1="-352" y2="-352" x1="320" />
            <line x2="384" y1="-288" y2="-288" x1="320" />
            <line x2="384" y1="-224" y2="-224" x1="320" />
            <rect width="64" x="320" y="-108" height="24" />
            <line x2="384" y1="-96" y2="-96" x1="320" />
            <rect width="64" x="320" y="-44" height="24" />
            <line x2="384" y1="-32" y2="-32" x1="320" />
            <rect width="256" x="64" y="-448" height="576" />
        </blockdef>
        <blockdef name="memmux">
            <timestamp>2010-3-31T21:56:26</timestamp>
            <rect width="336" x="64" y="-640" height="640" />
            <line x2="0" y1="-608" y2="-608" x1="64" />
            <line x2="0" y1="-544" y2="-544" x1="64" />
            <line x2="0" y1="-480" y2="-480" x1="64" />
            <line x2="0" y1="-416" y2="-416" x1="64" />
            <line x2="0" y1="-352" y2="-352" x1="64" />
            <line x2="0" y1="-288" y2="-288" x1="64" />
            <line x2="0" y1="-224" y2="-224" x1="64" />
            <line x2="0" y1="-160" y2="-160" x1="64" />
            <rect width="64" x="0" y="-108" height="24" />
            <line x2="0" y1="-96" y2="-96" x1="64" />
            <rect width="64" x="0" y="-44" height="24" />
            <line x2="0" y1="-32" y2="-32" x1="64" />
            <line x2="464" y1="-608" y2="-608" x1="400" />
            <line x2="464" y1="-544" y2="-544" x1="400" />
            <line x2="464" y1="-480" y2="-480" x1="400" />
            <line x2="464" y1="-416" y2="-416" x1="400" />
            <line x2="464" y1="-352" y2="-352" x1="400" />
            <line x2="464" y1="-288" y2="-288" x1="400" />
            <rect width="64" x="400" y="-236" height="24" />
            <line x2="464" y1="-224" y2="-224" x1="400" />
            <rect width="64" x="400" y="-172" height="24" />
            <line x2="464" y1="-160" y2="-160" x1="400" />
            <rect width="64" x="400" y="-108" height="24" />
            <line x2="464" y1="-96" y2="-96" x1="400" />
            <rect width="64" x="400" y="-44" height="24" />
            <line x2="464" y1="-32" y2="-32" x1="400" />
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
        <block symbolname="clkdiv" name="XLXI_49">
            <blockpin signalname="clk" name="clk" />
            <blockpin name="clk6" />
            <blockpin name="clk8" />
            <blockpin signalname="XLXN_147" name="clkps" />
            <blockpin signalname="XLXN_90" name="clkled" />
            <blockpin signalname="XLXN_146" name="clkmem" />
        </block>
        <block symbolname="cpuvm2" name="XLXI_50">
            <blockpin signalname="XLXN_147" name="clk" />
            <blockpin name="virq" />
            <blockpin name="evnt" />
            <blockpin name="halt" />
            <blockpin name="aclo" />
            <blockpin name="dclo" />
            <blockpin name="ar" />
            <blockpin name="dmr" />
            <blockpin name="sack" />
            <blockpin signalname="XLXN_101" name="rply" />
            <blockpin name="sel" />
            <blockpin signalname="XLXN_90" name="dbgclk" />
            <blockpin signalname="sw(7:0)" name="sw(7:0)" />
            <blockpin signalname="btn(3:0)" name="btn(3:0)" />
            <blockpin signalname="XLXN_107(15:0)" name="ad(15:0)" />
            <blockpin name="dmgo" />
            <blockpin name="iako" />
            <blockpin name="init" />
            <blockpin signalname="XLXN_105" name="sync" />
            <blockpin signalname="XLXN_98" name="din" />
            <blockpin signalname="XLXN_99" name="dout" />
            <blockpin signalname="XLXN_106" name="wtbt" />
            <blockpin signalname="dp" name="dot" />
            <blockpin signalname="seg(6:0)" name="seg(6:0)" />
            <blockpin signalname="an(3:0)" name="an(3:0)" />
            <blockpin signalname="leda(7:0)" name="led(7:0)" />
        </block>
        <block symbolname="pullup" name="XLXI_51">
            <blockpin signalname="XLXN_99" name="O" />
        </block>
        <block symbolname="pullup" name="XLXI_52">
            <blockpin signalname="XLXN_98" name="O" />
        </block>
        <block symbolname="pullup" name="XLXI_53">
            <blockpin signalname="XLXN_105" name="O" />
        </block>
        <block symbolname="pullup" name="XLXI_54">
            <blockpin signalname="XLXN_101" name="O" />
        </block>
        <block symbolname="ppumemctr" name="XLXI_56">
            <blockpin signalname="XLXN_146" name="clk" />
            <blockpin signalname="XLXN_98" name="din" />
            <blockpin signalname="XLXN_99" name="dout" />
            <blockpin signalname="XLXN_105" name="sync" />
            <blockpin signalname="XLXN_106" name="wtbt" />
            <blockpin name="halt" />
            <blockpin signalname="XLXN_107(15:0)" name="ad(15:0)" />
            <blockpin signalname="XLXN_148(15:0)" name="memdata(15:0)" />
            <blockpin signalname="XLXN_101" name="rply" />
            <blockpin signalname="XLXN_108" name="flashcs" />
            <blockpin signalname="XLXN_109" name="memoe" />
            <blockpin signalname="XLXN_110" name="memrw" />
            <blockpin signalname="XLXN_111(15:0)" name="memaddr(15:0)" />
            <blockpin signalname="XLXN_116" name="memrdy" />
        </block>
        <block symbolname="memmux" name="XLXI_57">
            <blockpin signalname="XLXN_108" name="ppu_flcs" />
            <blockpin signalname="XLXN_114" name="ppu_ramcs" />
            <blockpin signalname="XLXN_109" name="ppu_memoe" />
            <blockpin signalname="XLXN_110" name="ppu_memrw" />
            <blockpin name="cpu_ramcs" />
            <blockpin name="cpu_memoe" />
            <blockpin name="cpu_memrw" />
            <blockpin signalname="XLXN_146" name="clk" />
            <blockpin signalname="XLXN_111(15:0)" name="ppu_addr(15:0)" />
            <blockpin name="cpu_addr(15:0)" />
            <blockpin signalname="XLXN_116" name="ppu_rdy" />
            <blockpin name="cpu_rdy" />
            <blockpin signalname="MemOE" name="memoe" />
            <blockpin signalname="MemWR" name="memrw" />
            <blockpin signalname="FlashCS" name="memflcs" />
            <blockpin signalname="RamCS" name="memramcs" />
            <blockpin signalname="MemAdr(23:1)" name="memaddr(23:1)" />
            <blockpin signalname="XLXN_148(15:0)" name="ppu_data(15:0)" />
            <blockpin name="cpu_data(15:0)" />
            <blockpin signalname="MemDB(15:0)" name="memdata(15:0)" />
        </block>
        <block symbolname="constant" name="XLXI_58">
            <attr value="1" name="CValue">
                <trait delete="all:1 sym:0" />
                <trait editname="all:1 sch:0" />
                <trait valuetype="BitVector 32 Hexadecimal" />
            </attr>
            <blockpin signalname="XLXN_114" name="O" />
        </block>
        <block symbolname="constant" name="XLXI_59">
            <attr value="1" name="CValue">
                <trait delete="all:1 sym:0" />
                <trait editname="all:1 sch:0" />
                <trait valuetype="BitVector 32 Hexadecimal" />
            </attr>
            <blockpin signalname="FlashRp" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_60">
            <blockpin signalname="leda(0)" name="I" />
            <blockpin signalname="Led(0)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_61">
            <blockpin signalname="leda(1)" name="I" />
            <blockpin signalname="Led(1)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_62">
            <blockpin signalname="leda(2)" name="I" />
            <blockpin signalname="Led(2)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_63">
            <blockpin signalname="MemOE" name="I" />
            <blockpin signalname="Led(7)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_64">
            <blockpin signalname="FlashCS" name="I" />
            <blockpin signalname="Led(6)" name="O" />
        </block>
        <block symbolname="buf" name="XLXI_65">
            <blockpin signalname="MemWR" name="I" />
            <blockpin signalname="Led(5)" name="O" />
        </block>
    </netlist>
    <sheet sheetnum="1" width="3520" height="2720">
        <instance x="624" y="608" name="XLXI_49" orien="R0">
        </instance>
        <instance x="624" y="1504" name="XLXI_50" orien="R0">
        </instance>
        <branch name="sw(7:0)">
            <wire x2="624" y1="1600" y2="1600" x1="416" />
        </branch>
        <branch name="btn(3:0)">
            <wire x2="624" y1="1664" y2="1664" x1="416" />
        </branch>
        <branch name="XLXN_90">
            <wire x2="624" y1="1536" y2="1536" x1="608" />
            <wire x2="608" y1="1536" y2="1760" x1="608" />
            <wire x2="1088" y1="1760" y2="1760" x1="608" />
            <wire x2="1088" y1="576" y2="576" x1="1008" />
            <wire x2="1088" y1="576" y2="1760" x1="1088" />
        </branch>
        <branch name="dp">
            <wire x2="1360" y1="1536" y2="1536" x1="1008" />
        </branch>
        <branch name="seg(6:0)">
            <wire x2="1360" y1="1600" y2="1600" x1="1008" />
        </branch>
        <branch name="an(3:0)">
            <wire x2="1360" y1="1664" y2="1664" x1="1008" />
        </branch>
        <iomarker fontsize="28" x="1360" y="1536" name="dp" orien="R0" />
        <iomarker fontsize="28" x="1360" y="1600" name="seg(6:0)" orien="R0" />
        <iomarker fontsize="28" x="1360" y="1664" name="an(3:0)" orien="R0" />
        <iomarker fontsize="28" x="416" y="1664" name="btn(3:0)" orien="R180" />
        <iomarker fontsize="28" x="416" y="1600" name="sw(7:0)" orien="R180" />
        <iomarker fontsize="28" x="288" y="384" name="clk" orien="R180" />
        <instance x="1392" y="928" name="XLXI_51" orien="R0" />
        <instance x="1280" y="928" name="XLXI_52" orien="R0" />
        <instance x="1168" y="928" name="XLXI_53" orien="R0" />
        <branch name="XLXN_98">
            <wire x2="624" y1="1904" y2="1904" x1="544" />
            <wire x2="544" y1="1904" y2="2384" x1="544" />
            <wire x2="1344" y1="2384" y2="2384" x1="544" />
            <wire x2="1344" y1="1152" y2="1152" x1="1008" />
            <wire x2="1344" y1="1152" y2="2384" x1="1344" />
            <wire x2="1344" y1="928" y2="1152" x1="1344" />
        </branch>
        <branch name="XLXN_99">
            <wire x2="624" y1="1968" y2="1968" x1="560" />
            <wire x2="560" y1="1968" y2="2336" x1="560" />
            <wire x2="1632" y1="2336" y2="2336" x1="560" />
            <wire x2="1456" y1="1232" y2="1232" x1="1008" />
            <wire x2="1632" y1="1232" y2="1232" x1="1456" />
            <wire x2="1632" y1="1232" y2="2336" x1="1632" />
            <wire x2="1456" y1="928" y2="1232" x1="1456" />
        </branch>
        <instance x="192" y="1024" name="XLXI_54" orien="R0" />
        <branch name="XLXN_101">
            <wire x2="256" y1="1024" y2="1408" x1="256" />
            <wire x2="624" y1="1408" y2="1408" x1="256" />
            <wire x2="256" y1="1408" y2="1792" x1="256" />
            <wire x2="1088" y1="1792" y2="1792" x1="256" />
            <wire x2="1088" y1="1792" y2="1840" x1="1088" />
            <wire x2="1088" y1="1840" y2="1840" x1="1008" />
        </branch>
        <instance x="624" y="2256" name="XLXI_56" orien="R0">
        </instance>
        <branch name="XLXN_105">
            <wire x2="624" y1="2032" y2="2032" x1="576" />
            <wire x2="576" y1="2032" y2="2368" x1="576" />
            <wire x2="1072" y1="2368" y2="2368" x1="576" />
            <wire x2="1232" y1="1072" y2="1072" x1="1008" />
            <wire x2="1072" y1="1056" y2="2368" x1="1072" />
            <wire x2="1232" y1="1056" y2="1056" x1="1072" />
            <wire x2="1232" y1="1056" y2="1072" x1="1232" />
            <wire x2="1232" y1="928" y2="1056" x1="1232" />
        </branch>
        <branch name="XLXN_106">
            <wire x2="608" y1="1776" y2="2096" x1="608" />
            <wire x2="624" y1="2096" y2="2096" x1="608" />
            <wire x2="1056" y1="1776" y2="1776" x1="608" />
            <wire x2="1056" y1="1312" y2="1312" x1="1008" />
            <wire x2="1056" y1="1312" y2="1776" x1="1056" />
        </branch>
        <branch name="XLXN_107(15:0)">
            <wire x2="1024" y1="1392" y2="1392" x1="1008" />
            <wire x2="1024" y1="1392" y2="2160" x1="1024" />
            <wire x2="1024" y1="2160" y2="2160" x1="1008" />
        </branch>
        <instance x="1808" y="2384" name="XLXI_57" orien="R0">
        </instance>
        <branch name="XLXN_108">
            <wire x2="1408" y1="1904" y2="1904" x1="1008" />
            <wire x2="1408" y1="1776" y2="1904" x1="1408" />
            <wire x2="1808" y1="1776" y2="1776" x1="1408" />
        </branch>
        <branch name="XLXN_109">
            <wire x2="1424" y1="1968" y2="1968" x1="1008" />
            <wire x2="1424" y1="1904" y2="1968" x1="1424" />
            <wire x2="1808" y1="1904" y2="1904" x1="1424" />
        </branch>
        <branch name="XLXN_110">
            <wire x2="1440" y1="2032" y2="2032" x1="1008" />
            <wire x2="1440" y1="1968" y2="2032" x1="1440" />
            <wire x2="1808" y1="1968" y2="1968" x1="1440" />
        </branch>
        <branch name="XLXN_111(15:0)">
            <wire x2="1808" y1="2288" y2="2288" x1="1008" />
        </branch>
        <instance x="1456" y="1808" name="XLXI_58" orien="R0">
        </instance>
        <branch name="XLXN_114">
            <wire x2="1808" y1="1840" y2="1840" x1="1600" />
        </branch>
        <branch name="clk">
            <wire x2="624" y1="384" y2="384" x1="288" />
        </branch>
        <branch name="XLXN_116">
            <wire x2="624" y1="2352" y2="2448" x1="624" />
            <wire x2="2336" y1="2448" y2="2448" x1="624" />
            <wire x2="2336" y1="1776" y2="1776" x1="2272" />
            <wire x2="2336" y1="1776" y2="2448" x1="2336" />
        </branch>
        <branch name="MemOE">
            <wire x2="2448" y1="1904" y2="1904" x1="2272" />
            <wire x2="2576" y1="1904" y2="1904" x1="2448" />
            <wire x2="2448" y1="1280" y2="1280" x1="2320" />
            <wire x2="2448" y1="1280" y2="1904" x1="2448" />
        </branch>
        <branch name="MemWR">
            <wire x2="2448" y1="1968" y2="1968" x1="2272" />
            <wire x2="2576" y1="1968" y2="1968" x1="2448" />
            <wire x2="2448" y1="1968" y2="1984" x1="2448" />
            <wire x2="2400" y1="1408" y2="1408" x1="2320" />
            <wire x2="2400" y1="1408" y2="1984" x1="2400" />
            <wire x2="2448" y1="1984" y2="1984" x1="2400" />
        </branch>
        <branch name="FlashCS">
            <wire x2="2416" y1="2032" y2="2032" x1="2272" />
            <wire x2="2576" y1="2032" y2="2032" x1="2416" />
            <wire x2="2416" y1="1344" y2="1344" x1="2320" />
            <wire x2="2416" y1="1344" y2="2032" x1="2416" />
        </branch>
        <branch name="RamCS">
            <wire x2="2576" y1="2096" y2="2096" x1="2272" />
        </branch>
        <iomarker fontsize="28" x="2576" y="1904" name="MemOE" orien="R0" />
        <iomarker fontsize="28" x="2576" y="1968" name="MemWR" orien="R0" />
        <iomarker fontsize="28" x="2576" y="2032" name="FlashCS" orien="R0" />
        <iomarker fontsize="28" x="2576" y="2096" name="RamCS" orien="R0" />
        <branch name="MemDB(15:0)">
            <wire x2="2576" y1="2352" y2="2352" x1="2272" />
        </branch>
        <iomarker fontsize="28" x="2576" y="2352" name="MemDB(15:0)" orien="R0" />
        <branch name="MemAdr(23:1)">
            <wire x2="2576" y1="2160" y2="2160" x1="2272" />
        </branch>
        <iomarker fontsize="28" x="2576" y="2160" name="MemAdr(23:1)" orien="R0" />
        <instance x="2064" y="2576" name="XLXI_59" orien="R0">
        </instance>
        <branch name="FlashRp">
            <wire x2="2480" y1="2608" y2="2608" x1="2208" />
        </branch>
        <iomarker fontsize="28" x="2480" y="2608" name="FlashRp" orien="R0" />
        <iomarker fontsize="28" x="2016" y="1504" name="Led(7:0)" orien="R0" />
        <branch name="Led(7:0)">
            <wire x2="1568" y1="1504" y2="1504" x1="1520" />
            <wire x2="1600" y1="1504" y2="1504" x1="1568" />
            <wire x2="1680" y1="1504" y2="1504" x1="1600" />
            <wire x2="1728" y1="1504" y2="1504" x1="1680" />
            <wire x2="1760" y1="1504" y2="1504" x1="1728" />
            <wire x2="1792" y1="1504" y2="1504" x1="1760" />
            <wire x2="2016" y1="1504" y2="1504" x1="1792" />
        </branch>
        <bustap x2="1792" y1="1504" y2="1600" x1="1792" />
        <bustap x2="1728" y1="1504" y2="1600" x1="1728" />
        <bustap x2="1568" y1="1504" y2="1600" x1="1568" />
        <branch name="leda(7:0)">
            <wire x2="1104" y1="1728" y2="1728" x1="1008" />
            <wire x2="1104" y1="1680" y2="1728" x1="1104" />
            <wire x2="1168" y1="1680" y2="1680" x1="1104" />
            <wire x2="1232" y1="1680" y2="1680" x1="1168" />
            <wire x2="1280" y1="1680" y2="1680" x1="1232" />
            <wire x2="1280" y1="1680" y2="1728" x1="1280" />
            <wire x2="1296" y1="1728" y2="1728" x1="1280" />
            <wire x2="1440" y1="1728" y2="1728" x1="1296" />
        </branch>
        <bustap x2="1232" y1="1680" y2="1776" x1="1232" />
        <bustap x2="1680" y1="1504" y2="1600" x1="1680" />
        <bustap x2="1760" y1="1504" y2="1408" x1="1760" />
        <bustap x2="1600" y1="1504" y2="1408" x1="1600" />
        <bustap x2="1296" y1="1728" y2="1824" x1="1296" />
        <instance x="1616" y="1120" name="XLXI_60" orien="R0" />
        <instance x="1616" y="1184" name="XLXI_61" orien="R0" />
        <instance x="1616" y="1248" name="XLXI_62" orien="R0" />
        <instance x="2320" y="1248" name="XLXI_63" orien="R180" />
        <instance x="2320" y="1312" name="XLXI_64" orien="R180" />
        <instance x="2320" y="1376" name="XLXI_65" orien="R180" />
        <branch name="leda(0)">
            <wire x2="1152" y1="1088" y2="1776" x1="1152" />
            <wire x2="1168" y1="1776" y2="1776" x1="1152" />
            <wire x2="1616" y1="1088" y2="1088" x1="1152" />
        </branch>
        <branch name="leda(1)">
            <wire x2="1232" y1="1776" y2="1840" x1="1232" />
            <wire x2="1264" y1="1840" y2="1840" x1="1232" />
            <wire x2="1264" y1="1696" y2="1840" x1="1264" />
            <wire x2="1536" y1="1696" y2="1696" x1="1264" />
            <wire x2="1536" y1="1152" y2="1696" x1="1536" />
            <wire x2="1616" y1="1152" y2="1152" x1="1536" />
        </branch>
        <branch name="leda(2)">
            <wire x2="1296" y1="1824" y2="1888" x1="1296" />
            <wire x2="1328" y1="1888" y2="1888" x1="1296" />
            <wire x2="1328" y1="1216" y2="1888" x1="1328" />
            <wire x2="1616" y1="1216" y2="1216" x1="1328" />
        </branch>
        <branch name="Led(0)">
            <wire x2="1568" y1="1600" y2="1680" x1="1568" />
            <wire x2="2000" y1="1680" y2="1680" x1="1568" />
            <wire x2="2000" y1="1088" y2="1088" x1="1840" />
            <wire x2="2000" y1="1088" y2="1680" x1="2000" />
        </branch>
        <branch name="Led(1)">
            <wire x2="1600" y1="1328" y2="1408" x1="1600" />
            <wire x2="1904" y1="1328" y2="1328" x1="1600" />
            <wire x2="1904" y1="1152" y2="1152" x1="1840" />
            <wire x2="1904" y1="1152" y2="1328" x1="1904" />
        </branch>
        <branch name="Led(2)">
            <wire x2="1680" y1="1600" y2="1664" x1="1680" />
            <wire x2="1888" y1="1664" y2="1664" x1="1680" />
            <wire x2="1888" y1="1216" y2="1216" x1="1840" />
            <wire x2="1888" y1="1216" y2="1664" x1="1888" />
        </branch>
        <branch name="Led(7)">
            <wire x2="2096" y1="1280" y2="1280" x1="1760" />
            <wire x2="1760" y1="1280" y2="1408" x1="1760" />
        </branch>
        <branch name="Led(6)">
            <wire x2="1648" y1="1344" y2="1648" x1="1648" />
            <wire x2="1728" y1="1648" y2="1648" x1="1648" />
            <wire x2="2096" y1="1344" y2="1344" x1="1648" />
            <wire x2="1728" y1="1600" y2="1648" x1="1728" />
        </branch>
        <branch name="Led(5)">
            <wire x2="1792" y1="1600" y2="1648" x1="1792" />
            <wire x2="1824" y1="1648" y2="1648" x1="1792" />
            <wire x2="1824" y1="1408" y2="1648" x1="1824" />
            <wire x2="2096" y1="1408" y2="1408" x1="1824" />
        </branch>
        <bustap x2="1168" y1="1680" y2="1776" x1="1168" />
        <branch name="XLXN_146">
            <wire x2="528" y1="752" y2="1840" x1="528" />
            <wire x2="624" y1="1840" y2="1840" x1="528" />
            <wire x2="1024" y1="752" y2="752" x1="528" />
            <wire x2="1024" y1="640" y2="640" x1="1008" />
            <wire x2="1040" y1="640" y2="640" x1="1024" />
            <wire x2="1040" y1="640" y2="2224" x1="1040" />
            <wire x2="1808" y1="2224" y2="2224" x1="1040" />
            <wire x2="1024" y1="640" y2="752" x1="1024" />
        </branch>
        <branch name="XLXN_147">
            <wire x2="560" y1="736" y2="832" x1="560" />
            <wire x2="624" y1="832" y2="832" x1="560" />
            <wire x2="1072" y1="736" y2="736" x1="560" />
            <wire x2="1072" y1="512" y2="512" x1="1008" />
            <wire x2="1072" y1="512" y2="736" x1="1072" />
        </branch>
        <branch name="XLXN_148(15:0)">
            <wire x2="1024" y1="2224" y2="2224" x1="1008" />
            <wire x2="1024" y1="2224" y2="2432" x1="1024" />
            <wire x2="2320" y1="2432" y2="2432" x1="1024" />
            <wire x2="2320" y1="2224" y2="2224" x1="2272" />
            <wire x2="2320" y1="2224" y2="2432" x1="2320" />
        </branch>
    </sheet>
</drawing>