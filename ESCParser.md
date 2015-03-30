ESCParser -- это утилита с интерфейсом командной строки, эмулятор принтера с системой команд ESC/P. Текущая реализация ближе всего к принтеру **Robotron CM 6329.01M**. Для печати символов используется шрифт, полученный из ПЗУ этого принтера.

ESCParser может выдавать результат в двух форматах:
  * PostScript -- поддерживается многостраничность. Для просмотра и конвертации в другие форматы можно использовать GSView + Ghostscript.
  * SVG -- многостраничность не поддерживается. Результат можно просматривать в любом современном веб-браузере.

Пример использования утилиты:
```
  ESCParser -ps printer.log > DOC.ps
  ESCParser -svg printer.log > DOC.svg
```
Результат прогона тестового примера через ESCParser:
![http://ukncbtl.googlecode.com/svn/trunk/docs/screenshot/ESCParser-test3.png](http://ukncbtl.googlecode.com/svn/trunk/docs/screenshot/ESCParser-test3.png)