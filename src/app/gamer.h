#pragma once

#include <QList>

#include <cart.h>

class Gamer {
public:
    Gamer();
    void addCart(Cart cart);      //Добавить карту
    Cart atCart(int num) const;   //Получить карту
    void removeCart(int num);     //Забрать карту
    int size() const;             //Получить количество карт
    QList<Cart> getCatrs() const; //Получить все карты
    void setTrump(int trump);     //Установить масть козыря
    int getMinTrump() const; //Получить наименьший козырь 0-12, 13 - отсутствует козырь
    void clear();            //Забрать все карты
    bool isEmpty();          //Есть ли карты

private:
    QList<Cart> mCarts; //Карты игрока
    int mTrump;         //Масть козыря
};
