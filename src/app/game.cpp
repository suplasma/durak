#include "game.h"

#include <QDateTime>
#include <QRandomGenerator>

Game::Game() : mIsTakeCarts(false), mTrump(0) {}

void Game::start(int size, int sizeCart)
{
    QRandomGenerator rand(QDateTime::currentMSecsSinceEpoch());
    QList<Cart> mCarts;
    mBlock.clear();
    mGamer.clear();
    mBot.clear();
    mTable1.clear();
    mTable2.clear();

    for (int i = 0; i < size; ++i) {
        mCarts.append(Cart(i));
    }

    while (!mCarts.isEmpty()) {
        mBlock.append(mCarts.takeAt(rand() % mCarts.size()));
    }

    mTrump = mBlock.last();
    mGamer.setTrump(mTrump.suit());
    mBot.setTrump(mTrump.suit());

    for (int i = 0; i < sizeCart; ++i) {
        mGamer.addCart(mBlock.takeFirst());
        mBot.addCart(mBlock.takeFirst());
    }

    mFollow = (mGamer.getMinTrump() < mBot.getMinTrump());
    mIsGame = true;
    mSizeCart = sizeCart;
}

QList<Cart> Game::cartsGamer() const
{
    return mGamer.getCatrs();
}

QList<Cart> Game::cartsBot() const
{
    return mBot.getCatrs();
}

void Game::clickCartGamer(int num)
{
    if (mIsGame) {
        if ((num >= 0) && (num < mGamer.size())) {
            if (mFollow) {
                if (mIsTakeCarts) {
                    enclose(mGamer, mBot, num);
                } else {
                    walk(mGamer, mBot, num);
                }
            } else {
                cope(mGamer, num);
            }
        }
    }
}

bool Game::clickCartBot(int num)
{
    if ((num >= 0) && (num < mBot.size()) && mIsGame) {
        if (mFollow) {
            return cope(mBot, num);
        }

        if (mIsTakeCarts) {
            return enclose(mBot, mGamer, num);
        }

        return walk(mBot, mGamer, num);
    }

    return false;
}

void Game::clickRightGamer()
{
    if (mIsGame) {
        if (!mTable1.isEmpty()) {
            if (mFollow) {                              //Ходил игрок
                if (mTable1.size() == mTable2.size()) { //Бито
                    tap(mGamer, mBot);
                    mFollow = 0;
                } else if (mIsTakeCarts) { //Бери
                    take(mGamer, mBot);
                }
            } else if (mTable1.size() > mTable2.size()) { //Беру
                mIsTakeCarts = true;
            }
        }
    }
}

void Game::clickRightBot()
{
    if (mIsGame) {
        if (!mTable1.isEmpty()) {
            if (mFollow == 0) {                         //Ходил игрок
                if (mTable1.size() == mTable2.size()) { //Бито
                    tap(mBot, mGamer);
                    mFollow = 1;
                } else if (mIsTakeCarts) { //Бери
                    take(mBot, mGamer);
                }

            } else if (mTable1.size() > mTable2.size()) { //Беру
                mIsTakeCarts = true;
            }
        }
    }
}

QString Game::status()
{
    if (mTable1.isEmpty()) {
        if (mGamer.isEmpty()) {
            if (mBot.isEmpty()) {
                mIsGame = false;
                return QStringLiteral("Ничья");
            } else {
                mIsGame = false;
                return QStringLiteral("Игрок выйграл");
            }
        }

        if (mBot.isEmpty()) {
            mIsGame = false;
            return QStringLiteral("Бот выйграл");
        }
    }

    if (mFollow) {
        if (mIsTakeCarts) {
            return QStringLiteral("Бот берет");
        }

        return QStringLiteral("Ходит игрок");
    }

    if (mIsTakeCarts) {
        return QStringLiteral("Игрок берет");
    }

    return QStringLiteral("Ходит бот");
}

Cart Game::trump() const
{
    return mTrump;
}

int Game::size() const
{
    return mBlock.size();
}

int Game::sizeBot() const
{
    return mBot.size();
}

QList<Cart> Game::table1() const
{
    return mTable1;
}

QList<Cart> Game::table2() const
{
    return mTable2;
}

bool Game::isEmpty()
{
    return mBlock.isEmpty();
}

bool Game::isGame()
{
    return mIsGame;
}

void Game::tap(Gamer &gamer, Gamer &coper)
{
    mTable1.clear();
    mTable2.clear();

    while ((gamer.size() < mSizeCart) && !mBlock.isEmpty()) {
        gamer.addCart(mBlock.takeFirst());
    }

    while ((coper.size() < mSizeCart) && !mBlock.isEmpty()) {
        coper.addCart(mBlock.takeFirst());
    }
}

void Game::take(Gamer &gamer, Gamer &coper)
{
    while (!mTable1.isEmpty()) {
        coper.addCart(mTable1.takeFirst());
    }

    while (!mTable2.isEmpty()) {
        coper.addCart(mTable2.takeFirst());
    }

    while ((gamer.size() < mSizeCart) && !mBlock.isEmpty()) {
        gamer.addCart(mBlock.takeFirst());
    }

    mIsTakeCarts = false;
}

bool Game::walk(Gamer &gamer, Gamer &coper, int num)
{
    const Cart &cart = gamer.atCart(num);

    if (mTable1.size() == mTable2.size()) {
        if (mTable1.isEmpty()) {
            mTable1.append(cart);
            gamer.removeCart(num);

            return true;

        } else if ((mTable1.size() < mSizeCart) &&
                   (coper.size() > (mTable1.size() - mTable2.size()))) {
            bool isFind = false;

            for (auto &cartTable : mTable1) {
                if (cart.value() == cartTable.value()) {
                    isFind = true;
                    break;
                }
            }

            for (auto &cartTable : mTable2) {
                if (cart.value() == cartTable.value()) {
                    isFind = true;
                    break;
                }
            }

            if (isFind) {
                mTable1.append(cart);
                gamer.removeCart(num);

                return true;
            }
        }
    }

    return false;
}

bool Game::cope(Gamer &gamer, int num)
{
    if (!mIsTakeCarts) {
        const Cart &cart = gamer.atCart(num);

        if (mTable1.size() > mTable2.size()) {
            auto lastCart = mTable1.last();

            if (cart.suit() != lastCart.suit()) {
                if (cart.suit() == mTrump.suit()) {
                    mTable2.append(cart);
                    gamer.removeCart(num);

                    return true;
                }
            } else if (cart.value() > lastCart.value()) {
                mTable2.append(cart);
                gamer.removeCart(num);

                return true;
            }
        }
    }

    return false;
}

bool Game::enclose(Gamer &gamer, Gamer &coper, int num)
{
    if ((mTable1.size() < mSizeCart) && (coper.size() > (mTable1.size() - mTable2.size()))) {
        const Cart &cart = gamer.atCart(num);

        bool isFind = false;

        for (auto &cartTable : mTable1) {
            if (cart.value() == cartTable.value()) {
                isFind = true;
                break;
            }
        }

        for (auto &cartTable : mTable2) {
            if (cart.value() == cartTable.value()) {
                isFind = true;
                break;
            }
        }

        if (isFind) {
            mTable1.append(cart);
            gamer.removeCart(num);

            return true;
        }
    }

    return false;
}
