#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main() {
    setlocale(LC_ALL, ""); // Set the locale to the user's default

    wchar_t thai_baht_symbol = L'\u0E3F';

    wprintf(L"Thai Baht symbol: %lc\n", thai_baht_symbol);

    return 0;
}