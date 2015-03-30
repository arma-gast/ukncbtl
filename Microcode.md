# Вступление #

Здесь будут выкладки RE микрокода. Микрокод анализируется оптическим методом "на-глаз", глядя в микроскоп. Данный анализ не претендует на 100% работоспособное воссоздание оригинального микрокода.


# Заметки #

Общий формат микрокода - ~~34 бита~~ 37 бит.
```
MSB

321098 76543210987654321 0
333322 22222222111111111 1987 654 3210   пропущено 3 бита НЗ
AAAAAA LLLLLLLLLLLLLLLLL BBBB CCC TTTT

```

Где

AAAAAATTTТ  образуют адрес следующего микрослова --

TTTT - номер такта микрокоманды -- идет в счетчик микрокоманд


LLLLLLLLLLLLLLLLL + TTTT идет в АЛУ

LLLLLLLLLLLLLLLLL + BBBB+ TTTT идет в блок переходов

LLLLLLLLLLLLLLLLL + CCC + TTTT идет в блок управления

для удобночтения микрокода -- буду его писать мнемонически как связку:

пример
NEXT=10|[R7](https://code.google.com/p/ukncbtl/source/detail?r=7)=OP|NOP|NOP|STEP=12  -- будет означать
001000 LLLLLLLLLLLLLLLLL 0000 000 1010   --- ([R7](https://code.google.com/p/ukncbtl/source/detail?r=7)=OP пока placeholder)

тоесть
СЛЕДМИКР| АЛУ | БП| БУ | ТАКТ

# АЛУ #

## Устройство АЛУ ##
Как ни странно в самом АЛУ есть 16 регистров общего назначения,Буферный регистр, Аккумулятор, 8 флагов признаков

обозначим их как uR0-uR15, CNVZ (уже знакомые нам) EFGH(пока непонятные) обозначим как uRS. Буферный регистр обозначим как uRB, аккумулятор как uA

## Микрокоманды АЛУ ##

бит 11 = признак одно-операндной микрокомманды -- (1 - 1операнд, 0- 2операнда)

биты 12-23 образуют микрокомманду АЛУ (обозначим как uc1-uc11)

### Микрокоманды с одним операндом ###
Загрузка
```
 uc11 uc10 uc9 uc8 uc7 uc6 uc5 uc4 uc3 uc2 uc1    Описание

  0    X   0   1   0   1   uR  uR uR   uR   x     MOV uRB, uR
  1    0   0   1   0   1   x   x   x   x    0     MOV uC, uA[3..0]  (грузим uc4-uc2 в uA)
  1    0   0   1   0   1   x   x   x   x    1     MOV uC, uA[7..4]
  1    1   0   1   0   1   x   x   x   x    0     MOV uC, uA[11..8]
  1    1   0   1   0   1   x   x   x   x    1     MOV uC, uA[15..12]
```


Коды операций для АЛУ (А -- операнд, Dst -- регистр назначения)
```
  op3 op2 op1 op0           Описание
  0   0   0   0             LSL
  0   0   0   1             A-1 -> Dst
  0   0   1   0             A+C -> Dst
  0   0   1   1             A+1 -> Dst (не меняет флаг C!!!)
  0   1   0   0             NOT A -> Dst
  0   1   0   1             CLR Dst
  0   1   1   0             LSR A -> Dst (без С)
  0   1   1   1             NOP
  1   0   0   0             ROL
  1   0   0   1             A+1 -> Dst
  1   0   1   0             A-C -> Dst
  1   0   1   1             LSLC A -> Dst (сдвиг влево c С)
  1   1   0   0             LSRC A -> Dst (логический сдвиг вправо с С)
  1   1   0   1             A -> Dst 
  1   1   1   0             ASR A -> Dst (без С)
  1   1   1   1             Set uRS (в соответствии с Dst)
```

Микрокоманды Алу
```
 uc11 uc10 uc9 uc8 uc7 uc6 uc5 uc4 uc3 uc2 uc1    Описание

  0    X   op3 op2 op1 op0 uR  uR  uR  uR   x     ALU(uR)->uR  (op cannot be x101 or x111)                                                  
  0    1   1   1   1   1   op3 op2 op1 op0  x     ALU(uRS)->uRS
  0    X   0   1   1   1   op3 op2 op1 op0  x     ALU(uRB)->uA
  1    X   0   1   1   1   op3 op2 op1 op0  x     ALU(DB)->uA,uRB
                                                  В следующих коммандах недопустимо
                                                  применение опкода 1101 (Mov)
  1    X   1   1   1   1   op3 op2 op1 op0  x     ALU(uRS)->uRS,uRB (одновременно в оба)
  0    X   1   1   0   1   op3 op2 op1 op0  x     ALU(uA)->uA
  1    X   1   1   0   1   op3 op2 op1 op0  x     ALU(uA)->uA,uRB
```




### Микрокоманды с двумя операндами ###
Коды операций для АЛУ (А,B -- операнды, Dst -- регистр назначения)
```
    uc11 uc10  op3 op2 op1 op0           Описание
    X    X     X   0   0   0             A&B->Dst
    X    X     X   0   0   1             A-B->Dst
    X    X     X   0   1   0             A^B->Dst
    X    X     0   0   1   1             A+B->Dst
    X    X     X   1   1   0             A|B->Dst
    А вот тут второй операнд игнорируется -- хотя и приходит
    0    0     1   0   1   1             A[7..0]->Dst[15..8]  
    1    0     1   0   1   1             A[7..0]->Dst[15..8]
    0    1     1   0   1   1             A[15..8]->Dst[7..0]
    1    1     1   0   1   1             A[15..8]->Dst[7..0]
```

Микрокоманды Алу с двумя операндами
```
 uc11 uc10 uc9 uc8 uc7 uc6 uc5 uc4 uc3 uc2     Описание

 0    1    0   1   1   1   op3 op2 op1 op0     ALU(uRB,uA)->uRS
 0    1    1   1   1   1   op3 op2 op1 op0     ALU(uRS,uA)->uA
 1    0    1   1   1   1   op3 op2 op1 op0     ALU(uRS,uA)->uRS
 1    1    0   1   1   1   op3 op2 op1 op0     ALU(uRB,uA)->uA,uRB
 1    1    1   1   1   1   op3 op2 op1 op0     ALU(uRS,uA)->uA,uRB
 0    0    X   1   1   1   X   X   X   X       INIT   
                                               в следующих микрокомандах
                                               не используется код x111
 0    0    op3 op2 op1 op0 uR  uR  uR  uR      ALU(uR,uA)->uRS
 0    1    op3 op2 op1 op0 uR  uR  uR  uR      ALU(uR,uA)->uA
 1    0    op3 op2 op1 op0 uR  uR  uR  uR      ALU(uR,uA)->uR
 1    1    op3 op2 op1 op0 uR  uR  uR  uR      ALU(uR,uA)->uA,uRB 
```

# Микрокоманды БП #

0000  - NOP

# Микрокоманды БУ #

000 - NOP