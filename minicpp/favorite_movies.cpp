/*******************************************************************************
 * Creates a list of a person's favourite movies then displays the list.
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

class Movie
{
public:

    Movie (): name("N/A"), rating("U"), year(0) {}
    Movie (std::string _name, std::string _rating, int _year)
    {
        name = _name;
        rating = _rating;
        SetYear(_year);
    }

    void SetName (std::string _name) { name = _name; }
    void SetRating (std::string _rating) { rating = _rating; }
    void SetYear (int _year)
    {
        std::time_t rawtime = std::time(nullptr);
        std::tm* timeinfo = std::localtime(&rawtime);
        int curyear = 1900 + timeinfo->tm_year;

        while (_year > curyear) {
            std::cout << "Invalid year, enter again: ";
            std::cin >> _year;
            std::cin.ignore();
        }

        year = _year;
    }

    std::string GetName () const { return name; }
    std::string GetRating () const { return rating; }
    int GetYear () const { return year; }

private:

    std::string name;
    std::string rating;
    int year;

}; // Movie

int main (int _argc, char* _argv[])
{
    std::vector<Movie> movies;
    char control = 'Y';

    while (std::toupper(control) == 'Y') {
        char name[256], rating[16];
        int year;

        std::cout << "Name of movie: ";
        std::cin.getline(name, 256);
        std::cout << "Rating of movie: ";
        std::cin.getline(rating, 16);
        std::cout << "Release year: ";
        std::cin >> year;
        std::cin.ignore();

        movies.push_back(Movie(name, rating, year));

        std::cout << "Add another movie [Y/N]: ";
        std::cin >> control;
    }

    for (auto& it: movies) {
        std::cout
        << "Name: " << it.GetName() << "\n"
        << "Rating: " << it.GetRating() << "\n"
        << "Year: " << it.GetYear() << "\n";
    }

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
