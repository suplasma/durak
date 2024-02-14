#pragma once

#include <QWidget>

#include <game.h>

namespace Ui {
class Window;
}

class Window : public QWidget {
    Q_OBJECT

public:
    explicit Window(QWidget *parent = 0);
    ~Window();

private slots:
    void on_newGameBtn_clicked();
    void on_comboBox_activated(const QString &size);

private:
    Ui::Window *ui;
    Game mGame;
    QString mStatus;
    bool mIsBigGame;
    QPixmap mBack;
    QPixmap mFon;
    QPixmap mSuits[4];
    int mCountCartsOfLine;

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *ev);
    void resizeEvent(QResizeEvent *ev);
    void paintBlock(QPainter &painter);
    void paintCarts(QPainter &painter, const QList<Cart> &carts, int slip, int slipOfLine);
    void paintBackCarts(QPainter &painter, int slip);
    void drawCart(QPainter &painter, const Cart &cart, int x, int y);
    void timerEvent(QTimerEvent *);
    void botActive();
    QString valueString(int val);
    void start();

    constexpr static int LEFT_SPACE = 20;
    constexpr static int RIGHT_SPACE = 10;
    constexpr static int DOWN_SPACE = 20;
    constexpr static int UP_SPACE = 50;
    constexpr static int CART_SPACE = 5;
    constexpr static int WIDTH_CART = 71;
    constexpr static int HEIGHT_CART = 96;
};
