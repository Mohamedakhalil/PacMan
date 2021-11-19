#ifndef VECTOR2_H
#define VECTOR2_H


//This is a struct that is nothing but two integers, to make it easier for sizes, positions, and other variables
struct Vector2 {
    int x;
    int y;
    Vector2()
        : x(0), y(0)
    {}
    Vector2(int x, int y)
        : x(x), y(y)
    {}
    Vector2(const Vector2& v2)
        : x(v2.x), y(v2.y)
    {}
};


#endif // VECTOR2_H
