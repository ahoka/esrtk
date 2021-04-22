int isupper(int c)
{
    return c >= 'A' && c <= 'Z';
}

int islower(int c)
{
    return c >= 'a' && c <= 'z';
}

int isalpha(int c)
{
    return isupper(c) || islower(c);
}

int isdigit(int c)
{
    switch (c)
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return 1;
    default:
        return 0;
    }
}

int isprint(int c)
{
    return c > 0x1f && c != 0x7f;
}

int isspace(int c)
{
    switch (c)
    {
    case ' ':
    case '\t':
    case '\n':
    case '\v':
    case '\f':
    case '\r':
        return 1;
    default:
        return 0;
    }
}

int isxdigit(int c)
{
    return (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || isdigit(c);
}
