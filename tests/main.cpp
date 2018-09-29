#include <wlib/utility>
#include <cassert>

class Mover {
public:
    Mover() : m_ptr(new int) {}

    Mover(Mover &&m) : m_ptr(m.m_ptr) {
        m.m_ptr = nullptr;
    }

    ~Mover() {
        if (nullptr != m_ptr) {
            delete m_ptr;
        }
    }

    int *get() {
        return m_ptr;
    }
private:
    int *m_ptr;
};

int main(int argc, char *argv[]) {
    // wlp::move
    Mover m1;
    assert(nullptr != m1.get());
    *m1.get() = 10;
    Mover m2(wlp::move(m1));
    assert(nullptr == m1.get());
    assert(10 == *m2.get());
}
