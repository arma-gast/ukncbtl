
#include "ESCParser.h"

EscInterpreter::EscInterpreter(const void* pdata, long datalength, std::ostream& output) :
    m_output(output)
{
    m_pdata = pdata;
    m_pnext = (const unsigned char*) pdata;
    m_datalength = datalength;
    m_datapos = 0;

    //TODO: Настраивать режимы по DIP-переключателям
    m_widefont = m_doublestrike = false;

    m_marginleft = m_margintop = 160;

    PrinterReset();
}

unsigned char EscInterpreter::GetNextByte()
{
    unsigned char result = *m_pnext;
    m_pnext = m_pnext + 1;
    m_datapos++;
    return result;
}

void EscInterpreter::PrinterReset()
{
    m_x = m_y = 0;
    m_px = m_py = 3;
    m_printmode = DRAFTmode;
    m_shiftx = 720/10;  // 10 char/inch
    m_shifty = 720/6;   // 6 lines/inch
    m_widefont = false;
}

bool EscInterpreter::InterpretNext()
{
    if (IsEndOfFile()) return false;

    unsigned char ch = GetNextByte();
    switch (ch)
    {
        case 0/*NUL*/: case 7/*BEL*/: case 17/*DC1*/: case 19/*DC3*/: case 127/*DEL*/:
            break; // Игнорируемые коды
        case 24/*CAN*/:
            m_endofpage = true;
            return false; //Конец страницы
        case 8/*BS*/: //BackSpace - сдвиг на 1 символ назад
            m_x -= m_shiftx;  if (m_x < 0) m_x = 0;
            break;
        case 9/*HT*/: //Горизонтальная табуляция - реализован частный случай
            //переустановка позиций табуляции игнорируется
            m_x += m_shiftx * 8;
            m_x = (m_x / (m_shiftx * 8)) * (m_shiftx * 8);
            break;
        case 10/*LF*/: //LineFeed - сдвиг к следующей строке
            m_y += m_shifty;
            return true;
        case 11/*VT*/: //Вертикальная табуляция - в частном случае удовлетворяет описанию.
            //NOTE: Переустановка позиций табуляции игнорируется
            m_x = 0;  m_y += m_shifty;
            return true;
        case 12/*FF*/: // Form Feed - !!! доделать
            m_endofpage = true;
            return false;
        case 13/*CR*/: //CarriageReturn - возврат каретки
            m_x = 0;
            break;
        case 14/*SO*/: // Включение шрифта вразрядку
            m_widefont = true;
            m_shiftx = 720/10 * 2;
            break;
        case 15/*SI*/: //Выбрать 17.1 символов на дюйм
            m_shiftx = (720 * 10) / 171;
            break;
        case 18/*DC2*/: //Выбрать 10 символов на дюйм
            m_shiftx = 720/10;
            break;
        case 20/*DC4*/: // Выключение шрифта вразрядку
            m_widefont = false;
            m_shiftx = 720/10;
            break;
        case 27/*ESC*/:  //Expanded Function Codes
            return InterpretEscape();

        /* иначе "напечатать" символ */
        default:
            PrintCharacter(ch);
            m_x += m_shiftx;
            break;
    }

    return true;
}

bool EscInterpreter::InterpretEscape()
{
    unsigned char ch = GetNextByte();
    switch (ch)
    {
        case 'U': break; //Однонаправленная печать - игнорировать
        case 'x': // Выбор качества
            m_printmode = GetNextByte();
            break;

        // Группа функций character pitch
        case 'P':
            m_shiftx = 720/10; // 10 cpi
            break;
        case 'M':
            m_shiftx = 720/12; // 12 cpi
            break;
        case 15/*SI*/:
            m_shiftx = 720/17; // 17 cpi
            break;

        // Группа кодов line feed
        case '2':
            m_shifty = 720/6; /* set line spacing to 1/6 inch */
            break;
        case '0':
            m_shifty = 720/8; /* set line spacing to 1/8 inch */
            break;
        case 'A':   /* text line spacing */
            m_shifty = (720 * (int)GetNextByte() / 60);
            break;
        case '3':   /* graphics line spacing */
            m_shifty = (720 * (int)GetNextByte() / 180);
            break;
        case 'J': /* variable line spacing */
            m_y += (int)GetNextByte() * 720 / 180;
            return true;

        case 'C': //PageLength - игнорировать
            if (GetNextByte() == 0)
                GetNextByte();
            break;
        case 'N': //Skip perforation - игнорировать
            GetNextByte();
            break;
        case 'O': break;
        case 'B': //Set vertical tabs - игнорировать ???
            while (GetNextByte() != 0);
            break;
        case '/':
            GetNextByte();
            break;
        case 'D': //Set horizontal tabs - игнорировать ???
            while (GetNextByte() != 0);
            break;
        case 'Q': //Set right margin - игнорировать ???
            GetNextByte();
            break;
        //Bit image graphics mode !!! - недоделано
        case 'K': /* 8-bit single density graphics */
        case 'L': /* the same */
            printGR9(2*6);
            break;
        case 'Y': /* 8-bit double-speed double-density graphics */
            printGR9(2*3);
            break;
        case 'Z': /* 8-bit quadple-density graphics */
            printGR9(3 /* = 2*1.5 */);
            break;
        case '*': /* Bit Image Graphics Mode */
            switch(GetNextByte())
            {
                case 0: /* same as ESC K graphic command */
                case 1: /* same as ESC L graphic command */
                    printGR9(2*6);
                    break;
                case 2: /* same as ESC Y graphic command */
                    printGR9(2*3);
                    break;
                case 3: /* same as ESC Z graphic command */
                    printGR9(3);
                    break;
                case 4: /* CRT 1 */
                    printGR9(9);
                    break;
                case 6: /* CRT 2 */
                    printGR9(8);
                    break;
                case 32:  /* High-resolution for ESC K */
                    printGR24(2*6);
                    break;
                case 33:  /* High-resolution for ESC L */
                    printGR24(2*3);
                    break;
                case 38:  /* CRT 3 */
                    printGR24(2*4);
                    break;
                case 39:  /* High-resolution triple-density */
                    printGR24(2*2);
                    break;
                case 40:  /* high-resolution hex-density */
                    printGR24(2);
                    break;
            }
            break;
        /* reassign bit image command ??? */
        case '?': break;
        /* download - игнорировать (???) */
        case '&': break; /* this command downloads character sets to the printer */
        case '%': break; /* select/deselect download character code */
        case ':': /* this command copies the internal character set into the download area */
            GetNextByte();  GetNextByte();  GetNextByte();
            break;
        case 'R': /* international character set - игнорировать (???) */
            GetNextByte();
            break;
        /* MSB control - игнорорировать (???) */
        case '#': break; /* clear most significant bit */
        case '=': break; /* clear most significant bit */
        case '>': break; /* set most significant bit */
        /* print table control */
        case '6': break; /* select upper character set */
        case '7': break; /* select lower character set */
        /* home head */
        case '<':
            m_x = 0;    /* repositions the print head to the left most column */
            break;
        case 14/*SO*/: // Включение шрифта вразрядку
            m_widefont = true;
            m_shiftx = 720/10 * 2;
            break;
        /* inter character space */
        case 32/*SP*/:
            GetNextByte();
            break;
        /* absolute dot position */
        case '$':
            m_x = GetNextByte();
            m_x += 256*(int)GetNextByte();
            m_x = (int)((int)m_x * 720/60);
            break;
        /* relative dot position */
        case '\\':
        {
            int shift = GetNextByte();  shift += 256 * (int)GetNextByte();
            m_x += (int)((int)shift * 720 / (m_printmode ? 180 : 120));
            /* !!! учесть моду LQ или DRAFT */
        }
        break;
        
        /* CHARACTER CONTROL CODES */
        /* emphasized print */
        case 'E': /* !!! */
            break;
        case 'F': /* !!! */
            break;
        /* double strike print */
        case 'G':  // Включение двойной печати
            m_doublestrike = true;
            break;
        case 'H':  // Выключение двойной печати
            m_doublestrike = false;
            break;
        /* underline */
        case '-': /* !!! */
            GetNextByte();
            break;
        /* super/subscript character */
        case 'S': /* !!! */
            GetNextByte();
            break;
        case 'T': /* !!! */
            break;
        /* double width print */
        case 'W': /* !!! */
            GetNextByte();
            break;
        /* combination print */
        case '!': /* set print mode !!! */
            GetNextByte();
            break;
        /* italic print */
        case '4': /* set italics */
            break;
        case '5': /* clear itelics */
            break;
        /* character table */
        case 't': /* select character table ??? */
            GetNextByte(); /* игнорировать */
            break;
        /* double height */
        case 'w': /* select double height !!! */
            GetNextByte();
            break;

        /* SYSTEM CONTROL CODES */
        /* reset */
        case '@':
            PrinterReset();
            break;
        /* cut sheet feeder control */
        case 25/*EM*/:
            GetNextByte(); /* ??? - игнорировать */
            break;
    }

    return true;
}

void EscInterpreter::printGR9(int dx)
{
    int width = GetNextByte();  // Количество "кусочков" данных о изображении
    width += 256 * (int)GetNextByte();
    // Читать и выводить данные
    for (; width > 0; width--)
    {
        unsigned char fbyte = GetNextByte();
        unsigned char mask = 0x80;
        for (int i = 0; i < 8; i++)
        {
            if (fbyte & mask)
            {
                DrawStrike(float(m_x), float(m_y + i * 12));
                /* 12 соответствует 1/60 inch... На самом деле расстояние мажду иглами у
                9-pin dot matrix printers = 1/72 inch, но при эмуляции на 24-pin принимается 1/60 */
            }
            mask >>= 1;
        }
        m_x += dx;
    }
}

void EscInterpreter::printGR24(int dx)
{
    int width = GetNextByte(); //Количество "кусочков" данных о изображении
    width += 256 * (int)GetNextByte();
    //Читать и выводить данные
    for (; width > 0; width--)
    {
        for (unsigned char n = 0; n < 3; n++)
        {
            unsigned char fbyte = GetNextByte();
            unsigned char mask = 0x80;
            for (int i = 0; i < 8; i++)
            {
                if (fbyte & mask)
                {
                    DrawStrike(float(m_x), float((m_y + (n*4*8/*игл*/) + i*4)));
                    /* 4 соответствует 1/180 inch - расстояние мажду иглами у 24-pin dot matrix printers */
                }
                mask >>= 1;
            }
        }
        m_x += dx;
    }
}

void EscInterpreter::PrintCharacter(unsigned char ch)
{
    if (ch < 32 || ch > 204) return;

    const unsigned short* pchardata = RobotronFont + int(ch - 32) * 9;

    float step = float(m_shiftx) / 11.0f;
    for (int line = 0; line < 9; line++)
    {
        for (int col = 0; col < 9; col++)
        {
            unsigned short bit = (pchardata[line] >> col) & 1;
            if (!bit) continue;

            DrawStrike(m_x + col * step, float(m_y + line * 12));
            if (m_widefont)
                DrawStrike(m_x + (col + 1.0f) * step, float(m_y + line * 12));
            //TODO: Учитывать m_widefont
        }
    }
}

void EscInterpreter::DrawStrike(float x, float y)
{
    float cx = (float(m_marginleft) + x) / m_px;
    float cy = (float(m_margintop) + y) / m_py;
    float cr = 6.0f / m_px;
    m_output << "<circle cx=\"" << cx << "\" cy=\"" << cy << "\" r=\"" << cr << "\" />" << std::endl;
    //TODO: Учитывать m_doublestrike
}
