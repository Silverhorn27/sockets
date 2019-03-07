#ifndef DEFINIONS_H
#define DEFINIONS_H

#include <string>
#include <vector>
#include <map>
#include <list>

using std::string;
using std::vector;
using std::map;
using std::list;

using StringList = list<string>;
using StringVector = vector<string>;

using Buffer = vector<char>;

#define MAX_BUFFER_SIZE 4096
#define DEFAULT_BUFFER_SIZE 256

static const string CONFIG_FILE = "config.ini";

#endif // DEFINIONS_H
