#include "benchmark_latency.h"

ClientOrder* generateOrders(GBMGenerator& gbmGenerator, uint32_t spreadWidth, uint32_t minQuantity, uint32_t maxQuantity, size_t numOrders, double bidNotAskTendency) {
    uint32_t targetPrice = gbmGenerator.generatePrice();
    uint32_t minPrice = targetPrice > spreadWidth ? targetPrice - spreadWidth : 1;
    uint32_t maxPrice = min(targetPrice + spreadWidth, static_cast<uint32_t>(4095));

    if (minPrice > maxPrice) {
        minPrice = maxPrice - spreadWidth;
    }

    mt19937 priceGenerator(random_device{}());
    mt19937 quantityGenerator(random_device{}());
    mt19937 bidNotAskGenerator(random_device{}());

    uniform_int_distribution<uint32_t> priceDistribution(minPrice, maxPrice);
    uniform_int_distribution<uint32_t> quantityDistribution(minQuantity, maxQuantity);
    uniform_real_distribution<double> bidNotAskDistribution(0, 1);

    ClientOrder* orders = new ClientOrder[numOrders];

    for (size_t i = 0; i < numOrders; i++) {
        uint32_t price = priceDistribution(priceGenerator);
        uint32_t quantity = quantityDistribution(quantityGenerator);
        orders[i] = ClientOrder{i, price, quantity, bidNotAskDistribution(bidNotAskGenerator) < bidNotAskTendency};
    }

    return orders;
}

void executeTrades(Book &book, ClientOrder* orders, size_t numOrders) {
    for (int i = 0; i < numOrders; i++) {
        uint64_t clientId = orders[i].client;
        uint32_t price = orders[i].price;
        uint32_t volume = orders[i].quantity;

        if (orders[i].bidNotAsk) {
            book.addOrder<true>(clientId, price, volume);
        } else {
            book.addOrder<false>(clientId, price, volume);
        }
    }
}

double test() {
    StockParameters p1{1, 0.0001, 0.01, 1};
    StockParameters p2{1, 0.0001, 0.02, 1};

    GBMGenerator generator1(p1);
    GBMGenerator generator2(p2);

    Book book = Book(0);

    constexpr uint64_t numIterations = 10000;
    constexpr size_t numOrders = 1000;
    uint64_t timeTaken = 0;

    for (int i = 0; i < numIterations; i++) {
        // cout << static_cast<uint32_t>(round(generator1.computeNextPrice() * 100)) << endl;
        ClientOrder* orders = generateOrders(generator1, 50, 1, 10000, numOrders, 0.5);
        uint64_t startTime = tick();
        executeTrades(book, orders, numOrders);
        uint64_t endTime = tick();

        timeTaken += endTime - startTime;
        delete[] orders;
    }

    // cout << "Time per order: " << timeTaken/(numOrders * numIterations * 1.0) << " ns" << endl;
    // cout << "Total time: " << timeTaken/(1000000000 * 1.0) << " s" << endl;
    
    book.cleanup();
    
    return timeTaken/(numOrders * numIterations * 1.0);
}

// int main() {
//     constexpr int numTests = 100;
//     double totalTime = 0;

//     for (int i = 0; i < numTests; i++) {
//         double time = test();
//         cout << time << endl;
//         totalTime += time;
//     }
//     cout << endl;
//     cout << totalTime / (numTests * 1.0) << endl;

//     return 0;
// }

