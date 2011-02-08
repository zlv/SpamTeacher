#include "u82w1251.hpp"
#include <string>
//перевести строку в cp1251 / translate to cp1251
std::string utf8_2_win1251(std::string& s)
{
    //результат обработки s
    std::string result;
    //устанавливается в 1 или 2, когда a>127 / set to 1 or 2, then a is larger than 127
    int status = 0;

    for (int i=0; i<(int)s.length(); i++)
    {
        unsigned char a = s[i];

        if (status)
        {
            if (status==1)
            {
                if (a==129)
                {
                    result += (unsigned char)(168);
                }
                else
                {
                    result += (a+48);
                }
            }
            else if (status==2)
            {
                if (a==209 || a==145)
                {
                    result += (unsigned char)(184);
                }
                else
                {
                    result += (a+112);
                }
            }
            status = 0;
        }
        else
        {
            if (a<=127)
            {
                result += a;
            }
            else if (a==208)
            {
                status = 1;
            }
            else if (a==209)
            {
                status = 2;
            }
        }
    }

    return result;
}
