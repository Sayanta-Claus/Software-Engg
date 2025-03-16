#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Product class
class Product {
public:
    int id;
    std::string name;
    double price;
    int quantity;

    Product(int id, std::string name, double price, int quantity) 
        : id(id), name(name), price(price), quantity(quantity) {}

    std::string toString() const {
        return std::to_string(id) + "," + name + "," + std::to_string(price) + "," + std::to_string(quantity);
    }
};

// Store class
class Store {
private:
    std::vector<Product> products;
    std::string filename = "products.csv";

public:
    // Load products from CSV file
    void loadProducts() {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file." << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string token;
            std::vector<std::string> tokens;

            while (std::getline(iss, token, ',')) {
                tokens.push_back(token);
            }

            if (tokens.size() == 4) {
                Product product(std::stoi(tokens[0]), tokens[1], std::stod(tokens[2]), std::stoi(tokens[3]));
                products.push_back(product);
            }
        }
        file.close();
    }

    // Save products to CSV file
    void saveProducts() const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file." << std::endl;
            return;
        }

        for (const auto& product : products) {
            file << product.toString() << "\n";
        }
        file.close();
    }

    // Add a new product
    void addProduct(int id, std::string name, double price, int quantity) {
        Product product(id, name, price, quantity);
        products.push_back(product);
        saveProducts();
    }

    // Remove a product by ID
    void removeProduct(int id) {
        for (auto it = products.begin(); it != products.end(); ++it) {
            if (it->id == id) {
                products.erase(it);
                saveProducts();
                return;
            }
        }
    }

    // Purchase a product
    void purchaseProduct(int id) {
        for (auto& product : products) {
            if (product.id == id && product.quantity > 0) {
                product.quantity--;
                saveProducts();
                logPurchase(product);
                return;
            }
        }
    }

    // Log purchase to a file
    void logPurchase(const Product& product) {
        std::ofstream logFile("purchase_log.txt", std::ios_base::app);
        if (logFile.is_open()) {
            logFile << "Product ID: " << product.id << ", Name: " << product.name << ", Price: " << product.price << std::endl;
            logFile.close();
        } else {
            std::cerr << "Failed to open log file." << std::endl;
        }
    }

    // Display all products
    void displayProducts() const {
        for (const auto& product : products) {
            std::cout << "ID: " << product.id << ", Name: " << product.name << ", Price: " << product.price << ", Quantity: " << product.quantity << std::endl;
        }
    }
};

int main() {
    Store store;
    store.loadProducts();

    while (true) {
        std::cout << "1. Add Product\n2. Remove Product\n3. Purchase Product\n4. Display Products\n5. Exit" << std::endl;
        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                int id;
                std::string name;
                double price;
                int quantity;

                std::cout << "Enter ID: ";
                std::cin >> id;
                std::cout << "Enter Name: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Enter Price: ";
                std::cin >> price;
                std::cout << "Enter Quantity: ";
                std::cin >> quantity;

                store.addProduct(id, name, price, quantity);
                break;
            }
            case 2: {
                int id;
                std::cout << "Enter ID to remove: ";
                std::cin >> id;
                store.removeProduct(id);
                break;
            }
            case 3: {
                int id;
                std::cout << "Enter ID to purchase: ";
                std::cin >> id;
                store.purchaseProduct(id);
                break;
            }
            case 4:
                store.displayProducts();
                break;
            case 5:
                return 0;
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    }

    return 0;
}
