#include "filter.h"

int isVowel(char letter)
{
    if (letter >= 'A' && letter <= 'Z')
    {
        letter += +'a' - 'A';
    }
    if (letter == 'a' || letter == 'e' || letter == 'i' || letter == 'u' || letter == 'o')
    {
        return 1;
    }
    return -1;
}

void filter(int argc, char *argv[])
{
    if (argc != 1)
    {
        printError("Cantidad de parametros incorrecto.\n");
        return;
    }
    int c;
    while ((c = getChar()) != -1)
    {
        if (isVowel(c) == 1)
        {
            putChar(c);
        }
    }
    return;
}