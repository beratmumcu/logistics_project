@echo off
echo =================================================
echo        Lojistik Projesi Derleniyor...
echo =================================================

:: Derleyici ve bagimli DLL'lerin bulunabilmesi icin bin klasorunu PATH'e ekliyoruz
set PATH=C:\msys64\ucrt64\bin;%PATH%
gcc main.c hash_table.c graph.c -o lojistik_sistemi.exe

if %errorlevel% neq 0 (
    echo.
    echo [HATA] Derleme basarisiz!
    echo Lutfen main.c, hash_table.c ve graph.c dosyalarinin klasorde oldugundan emin olun.
) else (
    echo.
    echo [BASARILI] Proje "lojistik_sistemi.exe" olarak derlendi!
)
echo =================================================
pause
