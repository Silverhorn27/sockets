#include "StringUtils.h"

StringVector StringUtils::cropToStrings(const string &str, char seperator)
{
    StringVector strList;
    string tmp;

    for(char ch : str) {
        if (ch == seperator) {
            if (!tmp.empty()) {
                strList.push_back(tmp);
                tmp.clear();
            }
        }
        else tmp += ch;
    }

    if (!tmp.empty()) {
        strList.push_back(tmp);
    }

    return strList;
}
