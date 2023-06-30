#include <memory>
#include "src05/Main050.h"


#define CLASS_TYPE Main050::Main050

int main() {

    std::unique_ptr<Main> m = std::make_unique<CLASS_TYPE>();

    return m->exec();
}
