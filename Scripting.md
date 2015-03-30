# Скрипты в UKNCBTL.Qt #

Начиная с UKNCBTL.Qt версии beta-32 в оболочку эмулятора добавлена поддержка скриптинга. Командой **File > Run Script** выбирается .JS-файл и запускается на выполнение. Язык файла -- ECMAScript, по сути это JavaScript.

Справочник по ECMAScript: http://qt-project.org/doc/qt-4.8/ecmascript.html

В тексте программы может использоваться объект `emulator`, методы которого используются для управления эмулятором UKNCBTL.

## Методы объекта emulator ##

<dt><code>emulator.reset();</code> // since beta 32</dt>
<dd>Сброс эмулируемой машины.</dd>

<dt><code>emulator.run(100);</code> // since beta 32</dt>
<dd>Работа эмулятора в течение 100 фреймов, каждый фрейм это 1/25 секунды.</dd>

<dt><code>emulator.saveScreenshot("test.png");</code> // since beta 32</dt>
<dd>Сохранение скриншота в формате PNG.</dd>

<dt><code>emulator.attachCartridge(1, 'romctr_basic.bin');</code> // since beta 33</dt>
<dd>Подключение картриджа.</dd>

<dt><code>emulator.detachCartridge(1);</code> // since beta 33<br>
<dd>Отключение картриджа.</dd>

<dt><code>emulator.attachFloppy(0, 'sys1002.dsk');</code> // since beta 33</dt>
<dd>Подключение образа диска.</dd>

<dt><code>emulator.detachFloppy(0);</code> // since beta 33</dt>
<dd>Отключение диска.</dd>

<dt><code>emulator.attachHard(1, 'WDC170inv.img');</code> // since beta 33</dt>
<dd>Подключение образа жёсткого диска, работает только если уже подключена прошивка через emulator.attachCartridge</dd>

<dt><code>emulator.detachHard(1);</code> // since beta 33</dt>
<dd>Отключение жёсткого диска.</dd>

<dt><code>emulator.getUptime()</code> либо свойство <code>emulator.uptime</code> // since beta 33</dt>
<dd>Возвращает тип number -- количество секунд работы эмулируемой машины, по emulator.reset() сбрасывается в 0.</dd>

<dt><code>emulator.keyScan(0153);</code> // since beta 33</dt>
<dd>Нажимает клавишу (задаётся скан-код), прогон 3 фрейма, отпускает, прогон 3 фрейма.</dd>

<dt><code>emulator.keyScan(0153, 6);</code> // since beta 33</dt>
<dd>То же, но задаётся интервал (фреймов) от нажатия до отжатия.</dd>

<dt><code>emulator.keyScanShift(0013);</code> // since beta 33</dt>
<dd>Нажатие SHIFT, 2 фрейма, нажатие клавиши, 3 фрейма, отжатие клавиши, 2 фрейма, отжатие SHIFT, 3 фрейма -- т.е. комбинация для ввода символа в другом регистре.</dd>

<dt><code>emulator.keyString('1\n');</code> // since beta 33</dt>
<dd>Ввод серии символов; для каждого ASCII-символа ищется скан-код, если нужно используется нажатие SHIFT.</dd>

<dt><code>emulator.getCPU()</code> либо <code>emulator.cpu</code> // since beta 33</dt>
<dd>Получение объекта для ЦП.</dd>

<dt><code>emulator.getPPU()</code> либо <code>emulator.ppu</code> // since beta 33</dt>
<dd>Получение объекта для ПП.</dd>

<dt><code>emulator.consolePrint('message')</code> // since beta ??</dt>
<dd>Вывод сообщения в окно отладочной консоли; это окно видно если показаны отладочные окна.</dd>

<h2>Методы объектов emulator.cpu и emulator.ppu</h2>
(В примерах используется <code>emulator.ppu</code>, для <code>emulator.cpu</code> работает точно также.)<br>
<br>
<dt><code>emulator.ppu.getName()</code> либо <code>emulator.ppu.name</code> // since beta 33</dt>
<dd>Название процессора, "CPU" либо "PPU".</dd>

<dt><code>emulator.ppu.getReg(0)</code> либо <code>emulator.ppu.r(0)</code> // since beta 33</dt>
<dd>Значение в регистре 0..7.</dd>

<dt><code>emulator.ppu.getPC()</code> либо <code>emulator.ppu.pc</code> // since beta 33</dt>
<dd>Значение в регистре 7.</dd>

<dt><code>emulator.ppu.getSP()</code> либо <code>emulator.ppu.sp</code> // since beta 33</dt>
<dd>Значение в регистре 6.</dd>

<dt><code>emulator.ppu.getPSW()</code> либо <code>emulator.ppu.psw</code> // since beta 33</dt>
<dd>Значение в PSW.</dd>

<dt><code>emulator.ppu.setReg(0, 0123456)</code> // since beta ??</dt>
<dd>Изменить значение в регистре 0..7.</dd>

<dt><code>emulator.ppu.setPC(0123456)</code> либо <code>emulator.ppu.pc = 0123456</code> // since beta ??</dt>
<dd>Изменить значение в регистре 7.</dd>

<dt><code>emulator.ppu.setSP(0123456)</code> либо <code>emulator.ppu.sp = 0123456</code> // since beta ??</dt>
<dd>Изменить значение в регистре 6.</dd>

<dt><code>emulator.ppu.setPSW(0123456)</code> либо <code>emulator.ppu.psw = 0123456</code> // since beta ??</dt>
<dd>Изменить значение в регистре PSW.</dd>

<dt><code>emulator.ppu.isHalt()</code> либо <code>emulator.ppu.halt</code> // since beta 33</dt>
<dd>Признак HALT/USER режима.</dd>

<dt><code>emulator.ppu.readWord(0160304)</code> // since beta 33</dt>
<dd>Прочитать слово из памяти процессора.</dd>

<dt><code>emulator.ppu.readByte(0160304)</code> // since beta ??</dt>
<dd>Прочитать байт из памяти процессора.</dd>

<dt><code>emulator.ppu.disassemble(0160304)</code> // since beta 33</dt>
<dd>Дизассемблирует одну инструкцию по заданному адресу, возвращает массив из четырёх элементов: { address, instruction, arguments, instruction length }.</dd>

<h2>Пример скрипта</h2>
Сбрасываем машинку, записываем процесс загрузки в виде скриншотов через каждые 10 фреймов:<br>
<pre><code>emulator.reset();<br>
<br>
for (i = 0; i &lt; 10; i++)<br>
{<br>
  emulator.run(10);<br>
  emulator.saveScreenshot('test' + i + '.png');<br>
}<br>
</code></pre>

<h2>Советы и находки</h2>

Для перевода числа в строку в восьмеричной системе используем <code>toString(8)</code>.<br>
<br>
<h2>Ссылки</h2>
Подобный скриптинг в других эмуляторах:<br>
<ul><li><a href='http://code.google.com/p/snes9x-rr/wiki/LuaScriptingFunctions'>http://code.google.com/p/snes9x-rr/wiki/LuaScriptingFunctions</a>
</li><li><a href='http://code.google.com/p/psxjin/wiki/LuaScriptingFunctions'>http://code.google.com/p/psxjin/wiki/LuaScriptingFunctions</a>
</li><li><a href='http://code.google.com/p/vba-rerecording/wiki/LuaScriptingFunctions'>http://code.google.com/p/vba-rerecording/wiki/LuaScriptingFunctions</a>
</li><li><a href='http://www.fceux.com/web/help/fceux.html'>http://www.fceux.com/web/help/fceux.html</a>