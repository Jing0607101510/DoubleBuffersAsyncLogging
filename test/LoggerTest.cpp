#include <iostream>

using namespace std;

template<int size>
class Buffer {
    public:
        void print() {
            cout << size << endl;
        }
};

int main(void) {
    const int size = 100;
    Buffer<size> buff;  // 传入的size需要编译时就确定值
    buff.print();

    cout << -100 % 3 << endl;
}