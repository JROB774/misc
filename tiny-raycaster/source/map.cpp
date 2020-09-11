static constexpr const char MAP[] =
"0000222222220000"
"1              0"
"1      11111   0"
"1     0        0"
"0     0  1110000"
"0     3        0"
"0   10000      0"
"0   3   11100  0"
"5   4   0      0"
"5   4   1  00000"
"0       1      0"
"2       1      0"
"0       0      0"
"0 0000000      0"
"0              0"
"0002222222200000";

Map::Map (): w(16), h(16)
{
    assert(sizeof(MAP) == ((w*h)+1)); // +1 for the null-terminator.
}

int Map::get_tile (int x, int y) const
{
    assert((x<w) && (y<h) && (sizeof(MAP) == ((w*h)+1)));
    return (MAP[y*w+x] - '0');
}

bool Map::is_empty (int x, int y) const
{
    assert((x<w) && (y<h) && (sizeof(MAP) == ((w*h)+1)));
    return (MAP[y*w+x] == ' ');
}
