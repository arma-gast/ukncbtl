# UKNCBTL.SDL #

Все настройки этой версии задаются через INI-файл UkncBtlSdl.ini -- обычный INI-файл с секциями SDL, VIDEO, UKNCBTL.

Клавиатура размаплена, но не до конца.

Выход по клавише Esc.

## Секция SDL ##
Все ключи начинаются с "SDL_", все пары ключ-значение без изменений передаются в SDL._

В частности, можно указать размещение окна на экране и выбрать видеодрайвер:
```
SDL_VIDEO_WINDOW_POS=300,200
SDL_VIDEODRIVER=directx
```

Полный набор опций описан в http://www.libsdl.org/docs/html/sdlenvvars.html

## Секция VIDEO ##
**Width** и **Height** -- ширина и высота окна, по умолчанию 800 x 600.

**BitsPerPixel** -- количество бит на пиксел, обычно 0 т.е. родное, можно поставить 32 или 16, но обычно не нужно.

**FullScreen** -- флаг показа на полный экран; при установленном флаге (1, yes, true) в Width и Height лучше ставить родное разрешение монитора.

**ScreenMode** -- число от 0 до 4, режим скалирования экрана УКНЦ, те же режимы что в win32-версии.

## Секция UKNCBTL ##

**Cartridge1** -- подключение картриджа 1..2

**Floppy0** -- подключение флоппи 0..3

**Hard1** -- подключение харда 1..2

## Пример INI-файла ##
Пример INI-файла -- показ в полный экран на FullHD мониторе:
```
[UKNCBTL]
Cartridge1=romctr_basic.bin
Floppy0=RT11A5.dsk

[SDL]
SDL_DEBUG=1
SDL_VIDEO_WINDOW_POS=300,200
SDL_VIDEODRIVER=directx

[VIDEO]
Width=1920
Height=1080
BitsPerPixel=0
FullScreen=1
ScreenMode=4
```