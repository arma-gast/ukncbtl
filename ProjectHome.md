# Download latest version #
[UKNCBTL-542-exe.zip](https://docs.google.com/uc?authuser=0&id=0B465kq_cVq2-VFpUV3lPdnY3UXc&export=download)

# UKNCBTL -- UKNC Back to Life! #
-- is cross-platform UKNC emulator for Windows/Linux/Mac OS X.
**UKNC** (**УКНЦ**, Электроника МС-0511) is soviet school computer based on two PDP-11 compatible processors KM1801VM2.

![http://ukncbtl.googlecode.com/svn/trunk/docs/MS0202.04.jpg](http://ukncbtl.googlecode.com/svn/trunk/docs/MS0202.04.jpg)
([Photo](http://ru.wikipedia.org/wiki/Файл:MC0202.04.jpg))

The emulator has two projects: UKNCBTL written for Win32 and works under Windows 2000/2003/2008/XP/Vista. UKNCBTL.Qt is based on Qt framework and works under Windows, Linux and Mac OS X.

Current status: Beta, under development. Most of software works fine.

Emulated:
  * CPU and PPU
  * Both memory controllers
  * Video controller
  * FDD controller (MZ standard)
  * ROM cartridges
  * Sound
  * Hard disk -- can read/write and boot
  * Tape cassette -- read/write WAV PCM files
  * Serial port (experimental)
  * Parallel port (experimental)

NOT emulated yet: network card, RAM disk.


---


# UKNCBTL -- UKNC Back to Life! #
-- это проект эмуляции советского школьного компьютера **УКНЦ**, построенного на двух процессорах КМ1801ВМ2, совместимых с семейством PDP-11.
Проект начат в июле 2006 года, переведён в open source в марте 2008 года.

На основе кода UKNCBTL начат проект эмулятора БК -- [BKBTL](http://code.google.com/p/bkbtl/).

В проект входят три ветки, все ветки используют одно и то же ядро эмуляции (см. папку emubase):
  * **UKNCBTL** -- написана под Win32, набор версий Windows -- 2000/2003/XP/Vista/7.
  * [UKNCBTL.Qt](QtUkncbtl.md) -- написана на основе Qt и работает под Windows, Linux и Mac OS X.
  * [UKNCBTL.SDL](UkncBtlSdl.md) -- использует для вывода SDL, работает под Windows

## Состояние эмулятора ##
Бета-версия. Работает RT-11 и его клоны, запускаются многие программы. Работает загрузка с дискет, из кассеты ПЗУ и с жёсткого диска.

Эмулируется:
  * оба процессора (тесты 791401, 791402, 691404 проходят)
  * контроллеры памяти
  * контроллер видео
  * контроллер дисковода (стандарт MZ)
  * кассеты ПЗУ
  * звук
  * винчестер (IDE-команды 91h, 20h, 30h, ech) -- загружается, чтение/запись
  * магнитофон -- чтение/запись через WAV PCM
  * стык С2 (последовательный порт) на реальном COM-порту PC -- экспериментально
  * параллельный порт -- экспериментально

НЕ эмулируются: локальная сеть; RAM-диск.
Проблемные места: тайминги процессора, контроллер прерываний процессора, таймер платы.

Есть возможность загрузки:
  * с образа кассеты ПЗУ (единственный имеющийся образ -- кассетный Бейсик Вильнюс)
  * с образа дискеты, поддерживаются форматы .dsk (только данные секторов, размером 819200 байт либо 409600 байт) и .rtd (то же самое плюс 256-байтный заголовок)
  * с образа винчестера (пока только с драйвером WD от "Электронные работы")
  * из WAV-файла -- эмуляция загрузки с магнитофона
  * через стык С2

Помимо эмулятора, в проект входят утилиты:
  * [RT11DSK](RT11DSK.md) -- для работы с образами дисков -- позволяет просматривать оглавление диска, извлекать и добавлять файлы (пока с ограничениями);
  * [SAV2WAV](SAV2WAV.md) -- конвертирует .SAV-файлы в формат WAV для загрузки через магнитофонный вход.
  * UkncComSender -- отправляет .SAV-файл на УКНЦ через Стык С2 (COM-порт)
  * [ESCParser](ESCParser.md) -- конвертирует файл с данными для принтера Robotron CM6329.01М (ESC/P) в формат SVG / PostScript

## Документация ##
  * [README](http://ukncbtl.googlecode.com/svn/trunk/docs/README.txt)
  * [Руководство пользователя](http://code.google.com/p/ukncbtl/wiki/UsersManual)
  * [Совместимость](http://code.google.com/p/ukncbtl/wiki/Compatibility)

## Скриншот ##

![http://ukncbtl.googlecode.com/svn/trunk/docs/screenshot/Uknc-Started.png](http://ukncbtl.googlecode.com/svn/trunk/docs/screenshot/Uknc-Started.png)

## Даты ##

  * Конец 2005 -- Александр Стрелец снял имидж ПЗУ УКНЦ
  * Июль 2006 -- первые попытки, реализация процессора на основе ВМ1 от Юрия Калмыкова
  * Февраль 2007 -- к проекту подключился Феликс Лазарев
  * Март 2007 -- к проекту подключился Алексей Кислый
  * 19 марта 2007 -- впервые увидели загрузочное меню
  * 21 июня 2007 -- первая бета-версия
  * 27 января 2008 -- beta-6, заработала запись на диск
  * 3 марта 2008 -- бета-9; проект переходит в опен-сорс на Google Code
  * 4 апреля 2009 -- бета-12 от Феликса, почти реалистичный звук
  * май 2010 -- появляется Qt-версия, под Windows, Linux и Mac OS X
  * июнь 2011 -- beta-20, утилита ESCparser
  * ноябрь 2011 -- появляется TestBench, выпуск beta-24
  * август 2012 -- появляется SDL-версия
  * март 2013 -- механизм отрисовки экрана переделан в новую архитектуру рендеров


---
