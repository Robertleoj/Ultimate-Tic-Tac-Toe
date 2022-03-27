#include <move.h>
#include <stdexcept>


char Move::operator[](int idx) const {
    switch(idx){
        case 0:
            return x1;
        case 1:
            return y1;
        case 2:
            return x2;
        case 3:
            return y2;
    };
    throw std::runtime_error("Index out of range");
}
Move::Move() {
    x1 = 0;
    y1 = 0;
    x2 = 0;
    y2 = 0;
}
bool Move::operator!=(const Move &m) const {
    return !(*this == m);
}

bool Move::operator==(const Move &m) const {
    return (x1 == m.x1 && y1 == m.y1 && x2 == m.x2 && y2 == m.y2);
}

Move::Move(char x1, char y1, char x2, char y2){
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
}

Move::Move(const Move &m){
    this->x1 = m.x1;
    this->y1 = m.y1;
    this->x2 = m.x2;
    this->y2 = m.y2;
}

bool Move::operator<(const Move &m) const {
    if(x1 != m.x1){
        return x1 < m.x1;
    }
    if(y1 != m.y1){
        return y1 < m.y1;
    }
    if(x2 != m.x2){
        return x2 < m.x2;
    }
    return y2 < m.y2;
}

