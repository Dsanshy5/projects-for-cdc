#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip> // For std::quoted

class Product {
private:
    std::string productID;
    std::string productName;
    int quantity;
    double price;

public:
    Product(std::string id, std::string name, int qty, double pr)
        : productID(id), productName(name), quantity(qty), price(pr) {}

    std::string getProductID() const {
        return productID;
    }

    std::string getProductName() const {
        return productName;
    }

    int getQuantity() const {
        return quantity;
    }

    double getPrice() const {
        return price;
    }

    void setQuantity(int qty) {
        quantity = qty;
    }

    void displayProductDetails() const {
        std::cout << "Product ID: " << productID << std::endl;
        std::cout << "Product Name: " << productName << std::endl;
        std::cout << "Quantity: " << quantity << std::endl;
        std::cout << "Price: " << price << std::endl;
    }

    // Save product data to a file
    void saveToFile(std::ofstream &file) const {
        file << productID << "," << std::quoted(productName) << "," << quantity << "," << price << "\n";
    }

    // Load product data from a file
    void loadFromFile(std::ifstream &file) {
        std::getline(file, productID, ',');
        std::getline(file, productName, ',');
        file >> quantity;
        file.ignore(); // Consume the newline after the quantity
        file >> price;
        file.ignore(); // Consume the newline after the price
    }
};

void displayMenu() {
    std::cout << "Inventory Management System" << std::endl;
    std::cout << "1. Add a new product" << std::endl;
    std::cout << "2. View product details" << std::endl;
    std::cout << "3. Update product quantity" << std::endl;
    std::cout << "4. Delete a product" << std::endl;
    std::cout << "5. List all products" << std::endl;
    std::cout << "6. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

int main() {
    std::vector<Product> products;
    int choice;

    std::ifstream fin("products.txt");
    if (fin.is_open()) {
        std::string id, name;
        int qty;
        double price;
        while (fin >> id) {
            std::getline(fin >> std::ws, name, ',');
            fin >> qty >> price;
            fin.ignore(); // Consume the newline after the price
            products.emplace_back(id, name, qty, price);
        }
        fin.close();
    }

    do {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string id, name;
                int qty;
                double price;
                std::cout << "Enter product ID: ";
                std::cin >> id;
                std::cout << "Enter product name: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                std::cout << "Enter quantity: ";
                std::cin >> qty;
                std::cout << "Enter price: ";
                std::cin >> price;
                products.push_back(Product(id, name, qty, price));

                // Save to file
                std::ofstream fout("products.txt", std::ios::app);
                if (fout.is_open()) {
                    products.back().saveToFile(fout);
                    fout.close();
                } else {
                    std::cerr << "Error: Unable to open file for appending." << std::endl;
                }

                std::cout << "Product added successfully!" << std::endl;
                break;
            }
            case 2: {
                std::string id;
                std::cout << "Enter product ID: ";
                std::cin >> id;
                bool found = false;
                for (auto &product : products) {
                    if (product.getProductID() == id) {
                        product.displayProductDetails();
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    std::cout << "Product not found." << std::endl;
                }
                break;
            }
            case 3: {
                std::string id;
                int qty;
                std::cout << "Enter product ID: ";
                std::cin >> id;
                std::cout << "Enter new quantity: ";
                std::cin >> qty;
                bool found = false;
                for (auto &product : products) {
                    if (product.getProductID() == id) {
                        product.setQuantity(qty);

                        // Update file
                        std::ofstream fout("products.txt");
                        if (fout.is_open()) {
                            for (const auto &prod : products) {
                                prod.saveToFile(fout);
                            }
                            fout.close();
                        } else {
                            std::cerr << "Error: Unable to open file for writing." << std::endl;
                        }

                        std::cout << "Product quantity updated successfully!" << std::endl;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    std::cout << "Product not found." << std::endl;
                }
                break;
            }
            case 4: {
                std::string id;
                std::cout << "Enter product ID: ";
                std::cin >> id;
                bool found = false;
                for (auto it = products.begin(); it != products.end(); ++it) {
                    if (it->getProductID() == id) {
                        products.erase(it);

                        // Update file
                        std::ofstream fout("products.txt");
                        if (fout.is_open()) {
                            for (const auto &prod : products) {
                                prod.saveToFile(fout);
                            }
                            fout.close();
                        } else {
                            std::cerr << "Error: Unable to open file for writing." << std::endl;
                        }

                        std::cout << "Product deleted successfully!" << std::endl;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    std::cout << "Product not found." << std::endl;
                }
                break;
            }
            case 5: {
                if (products.empty()) {
                    std::cout << "No products available." << std::endl;
                } else {
                    for (const auto &product : products) {
                        product.displayProductDetails();
                        std::cout << "---------------------" << std::endl;
                    }
                }
                break;
            }
            case 6:
                std::cout << "Exiting the program." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 6);

    return 0;
}
