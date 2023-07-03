#include <memory>
#include "src05/Main050.h"
#include "gui/Exhibition.h"


#define CLASS_TYPE Main050::Painter050

int main() {
//    std::unique_ptr<Main> m = std::make_unique<CLASS_TYPE>();
//
//    return m->exec();

    std::unique_ptr<CLASS_TYPE> p = std::make_unique<CLASS_TYPE>();
    Exhibition e;
    e.AddPainter(std::move(p));
    e.Exhibit();
}
