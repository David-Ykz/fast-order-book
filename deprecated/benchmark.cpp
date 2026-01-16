#include "book.h"
#include <chrono>

int main() {
    Book book = Book();
    auto start = chrono::high_resolution_clock::now();
    for (int i = 1; i <= 1000; i++) {
        for (int j = 1; j <= 1000; j++) {
            book.addBid(j, i);
        }
    }
    for (int i = 1000; i >= 1; i--) {
        for (int j = 1; j <= 1000; j++) {
            book.addAsk(j, i);
        }
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Elapsed time: " << elapsed.count() << " seconds" << endl;
    cout << "bid: " << book.getBidPrice() << ", ask: " << book.getAskPrice() << endl;
    book.cleanup();
    return 0;
}