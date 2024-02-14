#include "cart.h"

Cart::Cart() {}

Cart::Cart(int num) : mVal(num >> 2), mSuit(num & 3) {}

int Cart::suit() const
{
    return mSuit;
}

int Cart::value() const
{
    return mVal;
}
