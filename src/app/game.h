#pragma once

#include <QVector>

#include <cart.h>
#include <gamer.h>

class Game {
public:
    Game();
    void start(int size, int sizeCart); //Начать игру
    QList<Cart> cartsGamer() const;     //Карты игрока
    QList<Cart> cartsBot() const;       //Карты бота
    void clickCartGamer(int num);       //Выбор карты игрока
    bool clickCartBot(int num);         //Выбор карты бота
    void clickRightGamer();             //Клик правой кнопкой мыши
    void clickRightBot();       //Клик правой кнопкой мыши ботом
    QString status();           //Статус игры
    Cart trump() const;         //Козырная карта
    int size() const;           //Количество карт в колоде
    int sizeBot() const;        //Количество карт у бота
    QList<Cart> table1() const; //Подкинутые карты
    QList<Cart> table2() const; //Кроющие карты
    bool isEmpty();             //Есть ли карты
    bool isGame();              //Сейчас идет игра

private:
    QList<Cart> mBlock;  //Карты в колоде
    QList<Cart> mTable1; //Карты подкинутые
    QList<Cart> mTable2; //Карты кроющие
    Gamer mGamer;        //Игрок
    Gamer mBot;          //Бот
    Cart mTrump;         //Козырная карта
    int mFollow;         //Кто сейчас ходит 0 - бот, 1 - игрок
    bool mIsTakeCarts;   //Режим беру
    bool mIsGame;        //Сейчас идет игра
    int mSizeCart;       //Количество карт в раздаче

    void tap(Gamer &gamer, Gamer &coper);              //Бито
    void take(Gamer &gamer, Gamer &coper);             //Бери
    bool walk(Gamer &gamer, Gamer &coper, int num);    //Ходить
    bool cope(Gamer &gamer, int num);                  //Крыть
    bool enclose(Gamer &gamer, Gamer &coper, int num); //Подкинуть
};
