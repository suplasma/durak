#pragma once

#include <QString>

class Cart {
public:
    Cart();
    explicit Cart(int num);
    int suit() const;  //Получить масть
    int value() const; //Получить значение

private:
    int mVal;  //Значение
    int mSuit; //Масть
};
