#pragma once

class Move{
public:
    char x1;
    char y1;
    char x2;
    char y2;

    Move();

    Move(char x1, char y1, char x2, char y2);

    Move(const Move &m);

    char operator[](int idx) const;

    bool operator==(const Move &m) const;

    bool operator!=(const Move &m) const;

    bool operator<(const Move &m) const;
};

