#include <string>

#include "uci.h"

using namespace std;

namespace mockfish {

string uci::square(Square s) {
    return std::string{ char('a' + file_of(s)), char('1' + rank_of(s)) };
}

}