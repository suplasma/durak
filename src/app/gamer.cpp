#include "gamer.h"

Gamer::Gamer() {}

void Gamer::addCart(Cart cart)
{
    int i = 0;

    for (; i < mCarts.size(); ++i) {
        if (cart.suit() == mTrump) {
            if ((mCarts[i].suit() == mTrump) && (cart.value() < mCarts[i].value())) {
                break;
            }
        } else {
            if (mCarts[i].suit() == mTrump) {
                break;
            } else if (cart.value() < mCarts[i].value()) {
                break;
            }
        }
    }

    mCarts.insert(i, cart);
}

void Gamer::removeCart(int num)
{
    mCarts.removeAt(num);
}

Cart Gamer::atCart(int num) const
{
    return mCarts[num];
}

int Gamer::size() const
{
    return mCarts.size();
}

QList<Cart> Gamer::getCatrs() const
{
    return mCarts;
}

void Gamer::setTrump(int trump)
{
    mTrump = trump;
}

int Gamer::getMinTrump() const
{
    int min = 13;

    for (auto &cart : mCarts) {
        if ((cart.suit() == mTrump) && (min > cart.value())) {
            min = cart.value();
        }
    }

    return min;
}

void Gamer::clear()
{
    mCarts.clear();
}

bool Gamer::isEmpty()
{
    return mCarts.empty();
}
