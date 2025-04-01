#include <iostream>
// #include "../SQLiteCpp/include/SQLiteCpp/SQLiteCpp.h"
#include "SQLiteCpp/SQLiteCpp.h"
using namespace std;

void initializeDatabase() {
    try {
        SQLite::Database db("store.db", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
        
        db.exec(R"(
            CREATE TABLE IF NOT EXISTS products (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                price REAL NOT NULL,
                stock INTEGER NOT NULL
            );
        )");

        db.exec(R"(
            CREATE TABLE IF NOT EXISTS purchases (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                product_id INTEGER,
                quantity INTEGER,
                total_price REAL,
                timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
                FOREIGN KEY(product_id) REFERENCES products(id)
            );
        )");
        
    } catch (const std::exception& e) {
        cerr << "Database error: " << e.what() << endl;
    }
}

//  add a product
void addProduct(const string& name, double price, int stock) {
    try {
        SQLite::Database db("store.db", SQLite::OPEN_READWRITE);
        SQLite::Statement query(db, "INSERT INTO products (name, price, stock) VALUES (?, ?, ?);");
        query.bind(1, name);
        query.bind(2, price);
        query.bind(3, stock);
        query.exec();
        cout << "Product added successfully!" << endl;
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

//  update product details
void updateProduct(int id, double price, int stock) {
    try {
        SQLite::Database db("store.db", SQLite::OPEN_READWRITE);
        SQLite::Statement query(db, "UPDATE products SET price = ?, stock = ? WHERE id = ?;");
        query.bind(1, price);
        query.bind(2, stock);
        query.bind(3, id);
        query.exec();
        cout << "Product updated successfully!" << endl;
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

//  purchase a product
void purchaseProduct(int id, int quantity) {
    try {
        SQLite::Database db("store.db", SQLite::OPEN_READWRITE);
        SQLite::Statement query(db, "SELECT price, stock FROM products WHERE id = ?;");
        query.bind(1, id);
        if (query.executeStep()) {
            double price = query.getColumn(0).getDouble();
            int stock = query.getColumn(1).getInt();
            if (stock < quantity) {
                cout << "Not enough stock available!" << endl;
                return;
            }
            double total_price = price * quantity;
            db.exec("INSERT INTO purchases (product_id, quantity, total_price) VALUES (" + to_string(id) + ", " + to_string(quantity) + ", " + to_string(total_price) + ");");
            db.exec("UPDATE products SET stock = stock - " + to_string(quantity) + " WHERE id = " + to_string(id) + ";");
            cout << "Purchase successful! Total: $" << total_price << endl;
        } else {
            cout << "Product not found!" << endl;
        }
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

//  display all products
void displayProducts() {
    try {
        SQLite::Database db("store.db", SQLite::OPEN_READONLY);
        SQLite::Statement query(db, "SELECT * FROM products;");
        cout << "\nID | Name | Price | Stock" << endl;
        cout << "----------------------------------" << endl;
        while (query.executeStep()) {
            cout << query.getColumn(0).getInt() << " | "
                 << query.getColumn(1).getText() << " | $"
                 << query.getColumn(2).getDouble() << " | "
                 << query.getColumn(3).getInt() << endl;
        }
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

int main() {
    initializeDatabase();
    int choice;
    while (true) {
        cout << "\nStore Management System" << endl;
        cout << "1. Add Product" << endl;
        cout << "2. Update Product" << endl;
        cout << "3. Display Products" << endl;
        cout << "4. Purchase Product" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        if (choice == 1) {
            string name;
            double price;
            int stock;
            cout << "Enter product name: ";
            cin >> name;
            cout << "Enter price: ";
            cin >> price;
            cout << "Enter stock quantity: ";
            cin >> stock;
            addProduct(name, price, stock);
        } else if (choice == 2) {
            int id, stock;
            double price;
            cout << "Enter product ID: ";
            cin >> id;
            cout << "Enter new price: ";
            cin >> price;
            cout << "Enter new stock quantity: ";
            cin >> stock;
            updateProduct(id, price, stock);
        } else if (choice == 3) {
            displayProducts();
        } else if (choice == 4) {
            int id, quantity;
            cout << "Enter product ID: ";
            cin >> id;
            cout << "Enter quantity to purchase: ";
            cin >> quantity;
            purchaseProduct(id, quantity);
        } else if (choice == 5) {
            break;
        } else {
            cout << "Invalid choice! Try again." << endl;
        }
    }
    return 0;
}
