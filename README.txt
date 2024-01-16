# JFTT - Kompilator

Autor: Jakub Musiał 268442

Projekt: Kompilator prostego języka imperatywnego



Użyte narzędzia:
    * GNU Make 4.3
    * GNU G++ 12.3.0
    * GNU Flex 2.6.4
    * GNU Bison 3.8.2

    Projekt był testowany na systemie WSL Ubuntu 22.04.3 LTS. Użyty standard języka C++: C++17



Kompilacja:
    * By skompilować projekt należy użyć polecenia `make` lub `make compiler` z poziomu głównego katalogu projektu.
    * Po skutecznej kompilacji w głównym katalogu projektu powinien pojawić się plik wykonywalny o nazwie `compiler`.



Użycie kompilatora:
    Kompilator uruchamia się za pomocą polecenia:
    ```
    ./compiler <input> <output>
    ```
    Gdzie `<input>` to ścieżka pliku wejściowego zawierającego kod zadanego języka oraz `<output>`
    to ścieżka pliku wyjściowego, do którego zostanie zapisany kod assemblera wygenerowany przez kompilator.



Pliki projektu:
    * app/main.cpp - plik główny projektu
    * {include,source}/architecture/* - implementacja struktur obsługujacych architektury maszyny wirtualnej
    * {include,source}/assembly/* - implementacja struktur obsługujących generowanie kodu assemblera
    * {include,source}/condition/* - implementacja struktury manadżera gałęzi instrukcji warunkowych oraz struktur pomocniczych
    * {include,source}/identifier/* - implementacja struktur definiujących identyfikatory
    * {include,source}/loop/* - implementacja struktur definiujących pętle
    * include/parser/token.hpp - implementacja struktury `token` użytej jako typ tokenów parsera gramatyki
    * {include,source}/compiler.{hpp,cpp} - implementacja struktury `compiler` obsługującej kompilację z poziomu parsera
    * {include,source}/identifier_manager.{hpp,cpp} - implementacja struktury manadżera identyfikatorów
    * include/identifier.hpp - pomocniczy plik nagłówkowy dołączający pliki `include/identifier/*`
    * {include,source}/io.{hpp,cpp} - implementacja funckji służących do czytania oraz zapisywania do plików
    * {include,source}/loop_manager.{hpp,cpp} - implementacja struktury manadżera pętli
    * include/loop.hpp - pomocniczy plik nagłówkowy dołączający pliki `include/loop/*`
    * source/lexer.l - plik analizatora leksykalnego kodu zadanego języka
    * source/parser.y - plik parsera gramatyki zadanego języka
    * Makefile
    * README.txt
