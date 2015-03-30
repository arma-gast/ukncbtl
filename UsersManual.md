Данное руководство относится в основном к Windows-версии UKNCBTL.

# Введение #

**UKNCBTL** -- это эмулятор советского компьютера **Электроника МС 0511**, также известного как **УКНЦ** («учебный компьютер Научного центра»).
УКНЦ выпускался с конца 1987 года на нескольких заводах, суммарный объём выпуска превысил 300 000 машин. Основное назначение -- использование в школах на уроках информатики: на базе УКНЦ создавались учебные классы (КУВТ) по 2-12 рабочих мест учащихся (РМУ), с одним рабочим местом преподавателя (РМП). В состав РМП, в отличие от РМУ, входил дисковод и матричный принтер.

Задача эмулятора UKNCBTL -- насколько это возможно полная эмуляция компьютера и его периферии, с тем чтобы сохранить знания об УКНЦ и возможность запуска программ для него на современных машинах (прежде всего PC), не имея реальных экземпляров устройства.


# Установка #

Скачайте последнюю версию UKNCBTL. Список дистрибутивов для скачивания находится по адресу: http://code.google.com/p/ukncbtl/downloads/list. Выберите и скачайте файл UKNCBTL-xxx-exe.zip с наибольшим номером версии. Номер версии соответствует ревизии репозитория Subversion.

Эмулятор поставляется в виде архива с бинарными файлами. Разархивируйте архив в пустую папку.

Для запуска эмулятора запустите файл UKNCBTL.exe.

Эмулятор предназначен для работы под управлением "настольных" версий Windows, поддерживающих Unicode: Windows XP, Windows Vista, Windows 7 и выше.
Использование оперативной памяти при работе зависит от используемого рендера и режима экрана -- от 4,5 МБ до 10 МБ.

# Использование #

## Первые шаги ##

Запуск УКНЦ в работу выполняется командой меню **Emulator** > **Run**. Для удобства эта команда вынесена на панель инструментов. Та же команда выполняет останов эмулятора; при следующей команде Run выполнение продолжится с того же места.

**Внимание!** При работе эмулятор потребляет значительную часть процессорного времени, поэтому рекомендуется останавливать эмулятор, когда его работа не требуется.

Сброс эмулятора в исходное состояние выполняется командой **Emulator** > **Reset**. Это соответствует нажатию кнопки СБРОС на реальной УКНЦ. Для выполнения сброса не требуется останавливать эмулятор.

Спустя несколько секунд после запуска вы увидите на экране загрузочное меню УКНЦ:

![http://ukncbtl.googlecode.com/svn/trunk/docs/screenshot/Uknc-Started.png](http://ukncbtl.googlecode.com/svn/trunk/docs/screenshot/Uknc-Started.png)

Переход к нужной команде меню выполняется клавишами-стрелками, выбор пункта -- клавишей Enter.

Для загрузки с кассеты ПЗУ, выберите пункт меню **Drives** > **Cartridge 1**, укажите .bin-файл с образом кассеты ПЗУ (обычно это **romctr\_basic.bin** -- единственный образ кассеты ПЗУ, поставляемый с эмулятором), затем выберите в меню УКНЦ пункт **2 - кассета ПЗУ** и нажмите Enter.

Для загрузки с диска, выберите пункт меню **Drives** > **Floppy MZ0:**, укажите файл с образом диска. Эмулятор поддерживает два формата образов дискет -- .dsk (образ обычного диска) и .rtd (образ диска NET-RT11). После этого выберите в меню УКНЦ пункт **1 - диск** и нажмите Enter. Если выбранный диск -- загрузочный, то вы увидите процесс загрузки с диска. Если диск оказался не загрузочным, на экране УКНЦ появится сообщение об ошибке вида **?BOOT-U-No boot on volume** (**?BOOT-U-!Файл монитора не найден**).

Процесс загрузки с диска может включать в себя ввод текущей даты и времени -- при появлении подсказки укажите дату и время в предложенном формате и нажмите Enter.

По окончании загрузки появится приглашение к вводу команды -- обычно оно выглядит как точка **.** или надпись **MZ0>** в начале строки.

Для получения списка файлов на диске, введите команду **DIR** и нажмите Enter:
> . DIR

Файлы с расширением .SAV -- это исполняемые файлы, их можно запустить, введя имя и нажав клавишу Enter, например:
> . TEST

Некоторые диски являются незагрузочными, т.е. не содержат загрузчика и операционной системы. Для запуска программ с таких дисков нужно сделать следующее. Загрузиться с какого-либо загрузочного диска. Установить незагрузочный диск в качестве второго диска -- командой **Emulator** > **Floppy MZ1:**. Сделать диск MZ1: текущим командой:
> . ASS MZ1 DK
После этого можно получить список файлов командой DIR:
> . DIR `*`.SAV
Запускать программы на выполнение нужно командой RUN:
> . RU DIGGER

Операционная система, используемая на УКНЦ -- это RT-11 либо её советские клоны (ФОДОС, РАФОС). Поэтому, для более глубокого знакомства с работой на УКНЦ имеет смысл изучать руководства по работе в RT-11, в частности, [«RT-11, Step by Step» А.Г.Архангельского](http://www.az-libr.ru/Persons/0GN/fe16506c/Books/000/Toc.shtml).
См. также список литературы и ссылки в Приложении.

**TODO**: где можно получить помощь

## Интерфейс ##

### Меню ###
Меню **File**:
  * **Save State** -- сохранение состояния эмулятора в файл (пока не работает)
  * **Load State** -- загрузка состояния эмулятора из файла (пока не работает)
  * **Screenshot** -- сохранение скриншота экрана в формате PNG; файл сохраняется в текущую папку
  * **Save Screenshot As** -- сохранение скриншота экрана в выбранном формате -- PNG либо BMP
  * **Save Animation** -- начать сохранение последовательности кадров в формате Animated PNG
  * **Create Disk** -- создание файла образа пустой дискеты для использования в эмуляторе; после выполнения команды диск нужно подключить в эмулятор и инициализировать его из RT-11 командой INIT
  * **Settings** -- настройки программы
  * **Exit** -- завершение работы эмулятора

Меню **View**:
  * **Toolbar** -- показ/скрытие панели инструментов
  * **Keyboard** -- показ/скрытие экранной клавиатуры
  * **Tape** -- показ/скрытие панели магнитофона (чтение/запись пока не работает)
  * **RGB Screen** -- режим цветного экрана
  * **GRB Screen** -- режим цветного экрана, с перепутанными каналами красного и зеленого
  * **Grayscale Screen** -- режим черно-белого экрана
  * **Fullscreen** -- включение/отключение режима "полный экран"

Меню **Render**: каждый пункт меню соответствует одному режиму работы рендера

Меню **Emulator**:
  * **Run** -- запуск/останов эмулятора
  * **Reset** -- сброс эмулятора в исходное состояние
  * **Autostart** -- при включенном флаге, при запуске программы UKNCBTL сразу же запускает и эмулятор, без необходимости дополнительно вызывать команду **Run**
  * **Sound** -- включение/отключение звука; при включенном звуке скорость выполнения выравнивается до 100%
  * **Serial** -- включение/отключение привязки Стык С2 к последовательному порту PC
  * **Parallel** -- включение/отключение режима эмуляции порта принтера
  * **Network** -- включение/отключение режима эмуляции локальной сети

Меню **Configuration**:
  * **Floppy MZ0/1/2/3:** -- четыре команды для подключения/отключения дисков
  * **Cartridge 1/2** -- команды для подключения/отключения картриджей ПЗУ
  * **Hard 1/2** -- подключение/отключение образов жёсткого диска

Меню **Debug**:
  * **Debug Views** -- включение/отключение отладочного режима
  * **Switch CPU/PPU** -- переключение отладчика между ЦП и ПП
  * **Step Into** -- шаг эмулятора, используется в отладочном режиме -- выполняет команду 's' в отладочной консоли
  * **Step Over** -- шаг эмулятора, используется в отладочном режиме -- выполняет команду 'so' в отладочной консоли

## Загрузка ##

### Загрузка с кассеты ПЗУ ###
Кассета ПЗУ для УКНЦ -- это отдельный блок, вставляемый в один из слотов в верхней правой части корпуса.

В эмуляторе "вставка" кассеты ПЗУ выполняется командой **Configuration > Cartridge**, при этом выбирается файл образа кассеты ПЗУ.
Единственным известным картриджем для УКНЦ был картридж Вильнюс-Бейсика на 24 КБ -- файл образа имеет имя `romctr_basic.bin`.

Для загрузки с кассеты ПЗУ, после запуска эмулятора и появления загрузочного меню выбираем пункт **2 - кассета ПЗУ**.

### Загрузка с дискеты ###
**TODO**: "Вставка" дискеты, выбор пункта меню

### Загрузка с винчестера ###
Жёсткий диск ("винчестер") для УКНЦ -- это устройство, подключаемое в разъём расширения. ПЗУ винчестера видно системе как кассета ПЗУ. Поэтому, для подключения винчестера в эмуляторе нужно, во-первых, командой **Configuration > Cartridge** подключить файл ПЗУ винчестера. Можно использовать как первый так и второй слот. Имеющихся вариантов ПЗУ всего два: это `ide_hdbootv0400.bin` ("Электронные Работы", устройства с префиксом ID) и `ide_wdromv0110.bin` (Олег Ховайко, префиксы WD/WE/WF). После установки ПЗУ винчестера в качестве кассеты ПЗУ, выполняем выбор образа винчестера командой **Drives > Hard Drive**.

Для загрузки с винчестера, сразу после включения выбираем в меню пункт **2 - кассета ПЗУ**.

### Загрузка с ленты ###
**TODO**: Открытие WAV, выбор пункта меню, Play

## Встроенный отладчик ##

Встроенный отладчик включается/отключается по команде **View** > **Debug**.

Назначение отладочного режима -- двоякое. Во-первых, это инструмент для разработчиков эмулятора, позволяющий дойти до нужного места кода и пройти его по шагам, контроллируя что все функции работают правильно. Во-вторых, это возможность для пользователей эмулятора в деталях посмотреть работу программы.

Слева вверху -- экран УКНЦ. Слева внизу -- отладочная консоль, в которой можно вводить команды; краткий список команд выдается по команде h. Справа вверху -- окно состояния текущего процессора: вверху в боксе его регистры, справа от них содержимое стека, ниже дизассемблированный блок возле адреса PC; правее вверху -- состояние портов, внизу -- состояние каналов обмена данными между процессорами. Справа внизу -- окно состояния памяти.

Красным цветом везде отмечены ячейки памяти, которые изменились после очередного шага или прогона. Перед шагом/запуском сохраняется состояние всей памяти и регистров процессоров, что позволяет о каждой ячейке сказать, изменилась ли она.

Список команд отладочной консоли выдается по команде 'h':
```
  c          Clear console log
  dXXXXXX    Disassemble from address XXXXXX
  g          Go; free run
  gXXXXXX    Go; run processor until breakpoint at address XXXXXX
  m          Memory dump at current address
  mXXXXXX    Memory dump at address XXXXXX
  mrN        Memory dump at address from register N; N=0..7
  p          Switch to other processor
  r          Show register values
  rN         Show value of register N; N=0..7,ps
  rN XXXXXX  Set register N to value XXXXXX; N=0..7,ps
  s          Step; executes one instruction (F8)
  u          Save memory dump to file memdumpXPU.bin
```


# Приложения #

## Раскладка клавиатуры ##
Раскладка:
  * A..Z — соответствующие клавиши УКНЦ, раскладка QWERTY
  * К1...К5 = F1...F5
  * ТАБ = TAB
  * AP2 = Esc
  * УПР = левый Ctrl
  * НР = левый Shift
  * ГРАФ = правый Ctrl
  * АЛФ = правый Shift
  * ФИКС = Caps Lock
  * ПОМ = Delete
  * УСТ = End
  * ИСП = PageDown
  * СТОП = Pause/Break, дублировано на PageUp
  * СБРОС = Insert

## Литература ##
  * Архангельский Андрей Германович. «RT-11, Step by Step». — Москва, 1989. http://www.az-libr.ru/Persons/0GN/fe16506c/Books/000/Toc.shtml
  * Л. И. Валикова, Г. В. Вигдорчик, А. Ю. Воробьев, А. А. Лукин. «Операционная система СМ ЭВМ РАФОС. Справочник». — М.: Финансы и статистика, 1984. — 207 с.
  * [Документация на DEC RT-11 (PDF), англ.](http://www.bitsavers.org/pdf/dec/pdp11/rt11/)

## Ссылки ##
  * Страница проекта UKNCBTL: http://code.google.com/p/ukncbtl/
  * УКНЦ в Википедии: http://ru.wikipedia.org/wiki/УКНЦ
  * UKNC в Wikipedia: http://en.wikipedia.org/wiki/UKNC
  * УКНЦ & PDP-11 -- страница Арсения Гордина: http://uknc.narod.ru/
  * PDP-11.ORG.RU: http://pdp-11.org.ru/
  * Документация на RT-11 5.6 на BitSavers.org: http://www.bitsavers.org/pdf/dec/pdp11/rt11/v5.6_Aug91/