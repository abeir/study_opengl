#include <memory>
#include "src04/Main049.h"


#define CLASS_TYPE Main049::Main049

int main() {

    std::unique_ptr<Main> m = std::make_unique<CLASS_TYPE>();

    return m->exec();
}
