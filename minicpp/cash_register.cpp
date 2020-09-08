/*******************************************************************************
 * Determines the change needed for an item in standard US currency.
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

#include <iostream>

int main (int _argc, char* _argv[])
{
    constexpr int QUARTER = 25;
    constexpr int DIME    = 10;
    constexpr int NICKEL  =  5;

    double price, amount, change;
    int coins, dollars, quarters, dimes, nickels, cents;

    std::cout << "Enter item price: ";
    std::cin >> price;
    std::cout << "Enter amount given: ";
    std::cin >> amount;

    change  = (amount - price);
    dollars = change; // Implicit truncation of cents.
    change *= 100;

    coins    =  change - (dollars * 100);
    quarters =   coins / QUARTER;
    dimes    = ( coins % QUARTER) / DIME;
    nickels  = ((coins % QUARTER) % DIME) / NICKEL;
    cents    = ((coins % QUARTER) % DIME) % NICKEL;

    std::cout
    << "Dollars:  " << dollars  << "\n"
    << "Quarters: " << quarters << "\n"
    << "Dimes:    " << dimes    << "\n"
    << "Nickels:  " << nickels  << "\n"
    << "Cents:    " << cents    << "\n";

    std::cin.get();
    return 0;
}

/*////////////////////////////////////////////////////////////////////////////*/

/*******************************************************************************
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
*******************************************************************************/
