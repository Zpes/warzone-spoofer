#pragma once

#pragma warning (disable : 6066)
#pragma warning (disable : 4804)
#pragma warning (disable : 4018)

#define print_success_message( ... ) DbgPrintEx(0,0, "[+] " __VA_ARGS__);
#define print_error_message( ... ) DbgPrintEx(0,0, "[-] " __VA_ARGS__);