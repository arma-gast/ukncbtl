Работа эмулятора основана на выполнении фреймов. Основной цикл приложения -- это цикл обработки сообщений Windows. Сначала выполняется фрейм и отрисовывается экран, затем обрабатываются все накопившиеся оконные сообщения:
```
    MSG msg;
    while (true)
    {
        Emulator_SystemFrame();
        ScreenView_RedrawScreen();

        // Process all queue
        while (::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ))
        {
            if (msg.message == WM_QUIT)
                goto endprog;

            if (::TranslateAccelerator(g_hwnd, hAccelTable, &msg))
                continue;

            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }
endprog:
```

Каждый фрейм равен 1/25 секунды = 40 мс = 20000 тиков, 1 тик = 2 мкс.
За каждый фрейм происходит (см. CMotherboard::SystemFrame):
  * 20000 тиков системного таймера - на каждый 1-й тик
  * 2 сигнала EVNT, в 0-й и 10000-й тик фрейма
  * 320000 тиков ЦП - 16 раз за один тик
  * 250000 тиков ПП - 12,5 раз за один тик
  * 625 тиков FDD - каждый 32-й тик