#include "window.h"

#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>

#include "ui_window.h"

Window::Window(QWidget *parent) : QWidget(parent), ui(new Ui::Window)
{
    ui->setupUi(this);

    mBack.load(QStringLiteral("../../../data/picks/back.png"));
    mFon.load(QStringLiteral("../../../data/picks/fon.png"));

    for (int i = 0; i < 4; ++i) {
        mSuits[i].load(QString("../../../data/picks/%1.png").arg(i));
    }

    mStatus = mGame.status();
    startTimer(500);
}

Window::~Window()
{
    delete ui;
}

void Window::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    for (int w = 0; w < width(); w += mFon.width()) {
        for (int h = 0; h < height(); h += mFon.height()) {
            painter.drawPixmap(w, h, mFon.width(), mFon.height(), mFon);
        }
    }

    painter.setFont(QFont(QStringLiteral("Times new roman"), 17));
    paintBlock(painter);

    if (mGame.isGame()) {
        paintBackCarts(painter, UP_SPACE);
    } else {
        paintCarts(painter, mGame.cartsBot(), UP_SPACE, HEIGHT_CART + CART_SPACE);
    }

    paintCarts(painter, mGame.cartsGamer(),
               height() - DOWN_SPACE -
                   ((mGame.cartsGamer().size() - 1) / mCountCartsOfLine + 1) * HEIGHT_CART,
               HEIGHT_CART + CART_SPACE);
    paintCarts(painter, mGame.table1(),
               ((height() - HEIGHT_CART - DOWN_SPACE - UP_SPACE) >> 1) + UP_SPACE - 30,
               HEIGHT_CART + CART_SPACE + 30);
    paintCarts(painter, mGame.table2(),
               ((height() - HEIGHT_CART - DOWN_SPACE - UP_SPACE) >> 1) + UP_SPACE,
               HEIGHT_CART + CART_SPACE + 30);
}

void Window::mousePressEvent(QMouseEvent *ev)
{
    switch (ev->button()) {
        case Qt::LeftButton:
            for (int i = 0; i < mGame.cartsGamer().size(); ++i) {
                int x = (i % mCountCartsOfLine) * (WIDTH_CART + CART_SPACE) + LEFT_SPACE;
                int y =
                    height() - DOWN_SPACE -
                    ((mGame.cartsGamer().size() - 1) / mCountCartsOfLine + 1) * HEIGHT_CART +
                    (i / mCountCartsOfLine) * (HEIGHT_CART + CART_SPACE);
                if ((ev->x() >= x) && (ev->x() <= (x + WIDTH_CART)) && (ev->y() >= y) &&
                    (ev->y() <= (y + HEIGHT_CART))) {
                    mGame.clickCartGamer(i);
                }
            }

            break;

        case Qt::RightButton: mGame.clickRightGamer(); break;
    }

    mStatus = mGame.status();
    repaint();
}

void Window::resizeEvent(QResizeEvent *ev)
{
    mCountCartsOfLine =
        (ev->size().width() - LEFT_SPACE - RIGHT_SPACE) / (WIDTH_CART + CART_SPACE);
}

void Window::paintBlock(QPainter &painter)
{
    int x = width() - 100 - RIGHT_SPACE;
    int y = (height() >> 1) - 20;

    painter.setPen(Qt::black);
    painter.drawText(QRectF(x - 75, y - 45, 200, 30), mStatus, QTextOption(Qt::AlignHCenter));
    painter.drawText(QRectF(x, y - 23, 50, 23), QString::number(mGame.size()),
                     QTextOption(Qt::AlignHCenter));

    if (mGame.isEmpty()) {
        painter.drawPixmap(x + 17, y + 5, 18, 23, mSuits[mGame.trump().suit()]);
    } else {
        drawCart(painter, mGame.trump(), x, y);
    }

    if (mGame.size() > 1) {
        painter.drawPixmap(x - 8, y + 36, HEIGHT_CART, WIDTH_CART, mBack);

        if (mGame.size() > 10) {
            painter.drawPixmap(x - 6, y + 38, HEIGHT_CART, WIDTH_CART, mBack);

            if (mGame.size() > 20) {
                painter.drawPixmap(x - 4, y + 40, HEIGHT_CART, WIDTH_CART, mBack);

                if (mGame.size() > 30) {
                    painter.drawPixmap(x - 2, y + 42, HEIGHT_CART, WIDTH_CART, mBack);
                }
            }
        }
    }
}

void Window::paintCarts(QPainter &painter, const QList<Cart> &carts, int slip, int slipOfLine)
{
    for (int i = 0; i < carts.size(); ++i) {
        drawCart(painter, carts[i],
                 (i % mCountCartsOfLine) * (WIDTH_CART + CART_SPACE) + LEFT_SPACE,
                 slip + (i / mCountCartsOfLine) * slipOfLine);
    }
}

void Window::paintBackCarts(QPainter &painter, int slip)
{
    int count = (width() - LEFT_SPACE - RIGHT_SPACE) / (WIDTH_CART + CART_SPACE);

    for (int i = 0; i < mGame.sizeBot(); ++i) {
        painter.drawPixmap((i % count) * (WIDTH_CART + CART_SPACE) + LEFT_SPACE,
                           slip + (i / count) * (HEIGHT_CART + CART_SPACE), WIDTH_CART,
                           HEIGHT_CART, mBack);
    }
}

void Window::drawCart(QPainter &painter, const Cart &cart, int x, int y)
{
    if (cart.suit() & 1) {
        painter.setPen(Qt::black);
    } else {
        painter.setPen(QColor(0xdf, 0, 0));
    }

    painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
    painter.drawRect(x, y, WIDTH_CART, HEIGHT_CART);
    painter.drawText(QRectF(x + 2, y, 50, 23), valueString(cart.value()),
                     QTextOption(Qt::AlignLeft));
    painter.drawPixmap(x + 2, y + 22, 15, 18, mSuits[cart.suit()]);

    painter.rotate(180);
    painter.drawText(QRectF(-x - WIDTH_CART + 2, -y - HEIGHT_CART - 1, 50, 23),
                     valueString(cart.value()), QTextOption(Qt::AlignLeft));
    painter.drawPixmap(-x - WIDTH_CART + 2, -y - HEIGHT_CART + 22, 15, 18, mSuits[cart.suit()]);
    painter.rotate(180);

    painter.drawPixmap(x + WIDTH_CART / 2 - 18, y + HEIGHT_CART / 2 - 23, 36, 46,
                       mSuits[cart.suit()]);
}

void Window::timerEvent(QTimerEvent *)
{
    botActive();
    mStatus = mGame.status();
    repaint();
}

void Window::botActive()
{
    for (int i = 0; i < mGame.sizeBot(); ++i) {
        if (mGame.clickCartBot(i)) {
            return;
        }
    }

    mGame.clickRightBot();
}

QString Window::valueString(int val)
{
    if (mIsBigGame) {
        switch (val) {
            case 9: return QStringLiteral("В");
            case 10: return QStringLiteral("Д");
            case 11: return QStringLiteral("К");
            case 12: return QStringLiteral("Т");
            default: return QString::number(val + 2);
        }
    } else {
        switch (val) {
            case 5: return QStringLiteral("В");
            case 6: return QStringLiteral("Д");
            case 7: return QStringLiteral("К");
            case 8: return QStringLiteral("Т");
            default: return QString::number(val + 6);
        }
    }
}

void Window::start()
{
    int size = ui->comboBox->currentText().toInt();
    mGame.start(size, ui->spinBox->value());
    mIsBigGame = (size == 52);
}

void Window::on_newGameBtn_clicked()
{
    if (!mGame.isGame()) {
        start();
    } else {
        if (QMessageBox::question(this, QStringLiteral("Новая игра"),
                                  QStringLiteral("Начать игру заново?")) == QMessageBox::Yes) {
            start();
        }
    }
}

void Window::on_comboBox_activated(const QString &size)
{
    ui->spinBox->setMaximum(size.toInt() >> 1);
}
