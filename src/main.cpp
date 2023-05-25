#include <memory>
#include "src04/Main042.h"


#define CLASS_TYPE Main042::Main042

int main() {

    std::unique_ptr<Main> m = std::make_unique<CLASS_TYPE>();

    return m->exec();
}
