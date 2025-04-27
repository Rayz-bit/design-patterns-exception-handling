#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cctype>

const int MAX_PRODUCTS = 50;
const int MAX_ORDERS = 100;

//Product Class(Encapsulation, Abstraction)
class Product {
private:
    std::string id;
    std::string name;
    double price;
public:
    Product() : id(""), name(""), price(0.0) {}
    Product(const std::string& id, const std::string& name, double price)
        : id(id), name(name), price(price) {}

    std::string getId() const { return id; }
    std::string getName() const { return name; }
    double getPrice() const { return price; }
};

// Payment Strategy Abstract Class (Polymorphism)
class PaymentStrategy {
public:
    virtual void pay(double amount) = 0;
    virtual std::string getName() const = 0;
    virtual ~PaymentStrategy() = default;
};

// Concrete Payment Strategies
class CashPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        std::cout << "Paid " << amount << " using Cash.\n";
    }
    std::string getName() const override {
        return "Cash";
    }
};

class CardPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        std::cout << "Paid " << amount << " using Credit/Debit Card.\n";
    }
    std::string getName() const override {
        return "Credit / Debit Card";
    }
};

class GCashPayment : public PaymentStrategy {
public:
    void pay(double amount) override {
        std::cout << "Paid " << amount << " using GCash.\n";
    }
    std::string getName() const override {
        return "GCash";
    }
};

// Singleton Shopping Cart (Creational Pattern)
class SingletonCart {
private:
    Product cart[MAX_PRODUCTS];
    int quantity[MAX_PRODUCTS];
    int cartSize;
    SingletonCart() : cartSize(0) {}
    SingletonCart(const SingletonCart&) = delete;
    SingletonCart& operator=(const SingletonCart&) = delete;
public:
    static SingletonCart& getInstance() {
        static SingletonCart instance;
        return instance;
    }

    void addProduct(const Product& product) {
        for (int i = 0; i < cartSize; i++) {
            if (cart[i].getId() == product.getId()) {
                quantity[i]++;
                return;
            }
        }
        cart[cartSize] = product;
        quantity[cartSize] = 1;
        cartSize++;
    }

    void viewCart() const {
        if (cartSize == 0) {
            std::cout << "Cart is empty.\n";
            return;
        }
        std::cout << std::setw(10) << "ID" << std::setw(20) << "Name" 
                  << std::setw(10) << "Price" << std::setw(10) << "Qty" << "\n";
        for (int i = 0; i < cartSize; i++) {
            std::cout << std::setw(10) << cart[i].getId()
                      << std::setw(20) << cart[i].getName()
                      << std::setw(10) << cart[i].getPrice()
                      << std::setw(10) << quantity[i] << "\n";
        }
    }

    double getTotalAmount() const {
        double total = 0.0;
        for (int i = 0; i < cartSize; i++) {
            total += cart[i].getPrice() * quantity[i];
        }
        return total;
    }

    bool isEmpty() const {
        return cartSize == 0;
    }

    void clearCart() {
        cartSize = 0;
    }

    const Product* getProducts() const {
        return cart;
    }

    const int* getQuantities() const {
        return quantity;
    }

    int getCartSize() const {
        return cartSize;
    }
};

// Order Structure
struct Order {
    int orderId;
    double totalAmount;
    std::string paymentMethod;
    Product products[MAX_PRODUCTS];
    int quantities[MAX_PRODUCTS];
    int numProducts;
};

// Global Variables
Order orders[MAX_ORDERS];
int orderCount = 0;

// Utility Functions
std::string toUpperCase(const std::string& str) {
    std::string upper;
    for (char c : str) {
        upper += std::toupper(c);
    }
    return upper;
}

bool isValidProductId(const std::string& id) {
    std::string upperId = toUpperCase(id);
    return (upperId == "ABC" || upperId == "DEF" || upperId == "GHI" || upperId == "JKL" || upperId == "MNO");
}

bool isValidYesNo(const std::string& input) {
    std::string lowered;
    for (char c : input) lowered += std::tolower(c);
    return (lowered == "yes" || lowered == "no");
}

bool isYes(const std::string& input) {
    std::string lowered;
    for (char c : input) lowered += std::tolower(c);
    return lowered == "yes";
}

// Product List
Product products[] = {
    Product("ABC", "Tshirt", 600),
    Product("DEF", "Hoodie", 1200),
    Product("GHI", "Joggers", 700),
    Product("JKL", "Sweater", 1500),
    Product("MNO", "Jacket", 2000)
};
const int numProducts = sizeof(products)/sizeof(products[0]);

// --- Functions ---
void viewProducts() {
    std::cout << std::setw(10) << "Product ID" << std::setw(20) << "Name" << std::setw(10) << "Price" << "\n";
    for (int i = 0; i < numProducts; i++) {
        std::cout << std::setw(10) << products[i].getId()
                  << std::setw(20) << products[i].getName()
                  << std::setw(10) << products[i].getPrice() << "\n";
    }
}

Product* findProductById(const std::string& id) {
    std::string upperId = toUpperCase(id);
    for (int i = 0; i < numProducts; i++) {
        if (toUpperCase(products[i].getId()) == upperId)
            return &products[i];
    }
    return nullptr;
}

void checkout() {
    SingletonCart& cart = SingletonCart::getInstance();
    if (cart.isEmpty()) {
        std::cout << "Your cart is empty.\n";
        return;
    }
    cart.viewCart();
    double total = cart.getTotalAmount();
    std::cout << "Total Amount: " << total << "\n";

    std::cout << "Select Payment Method:\n";
    std::cout << "1. Cash\n2. Credit/Debit Card\n3. GCash\n";
    std::string option;
    PaymentStrategy* payment = nullptr;

    while (true) {
        std::cout << "Enter choice: ";
        std::cin >> option;
        if (option == "1") payment = new CashPayment();
        else if (option == "2") payment = new CardPayment();
        else if (option == "3") payment = new GCashPayment();

        if (payment != nullptr) break;
        std::cout << "Invalid choice. Try again.\n";
    }

    payment->pay(total);

    // log order
    std::ofstream logfile("log.txt", std::ios::app);
    logfile << "(LOG) -> Order ID: " << (orderCount + 1) << " has been successfully checked out and paid using " << payment->getName() << ".\n";
    logfile.close();

    // save order
    orders[orderCount].orderId = orderCount + 1;
    orders[orderCount].totalAmount = total;
    orders[orderCount].paymentMethod = payment->getName();
    orders[orderCount].numProducts = cart.getCartSize();
    for (int i = 0; i < cart.getCartSize(); i++) {
        orders[orderCount].products[i] = cart.getProducts()[i];
        orders[orderCount].quantities[i] = cart.getQuantities()[i];
    }
    orderCount++;

    delete payment;
    cart.clearCart();
    std::cout << "You have successfully checked out the products!\n";
}

void viewOrders() {
    if (orderCount == 0) {
        std::cout << "No orders have been made yet.\n";
        return;
    }
    for (int i = 0; i < orderCount; i++) {
        std::cout << "\nOrder ID: " << orders[i].orderId << "\n";
        std::cout << "Total Amount: " << orders[i].totalAmount << "\n";
        std::cout << "Payment Method: " << orders[i].paymentMethod << "\n";
        std::cout << "Order Details:\n";
        std::cout << std::setw(15) << "Product ID" 
                  << std::setw(20) << "Name"
                  << std::setw(10) << "Price"
                  << std::setw(10) << "Quantity" << "\n";
        for (int j = 0; j < orders[i].numProducts; j++) {
            std::cout << std::setw(15) << orders[i].products[j].getId()
                      << std::setw(20) << orders[i].products[j].getName()
                      << std::setw(10) << orders[i].products[j].getPrice()
                      << std::setw(10) << orders[i].quantities[j] << "\n";
        }
    }
}

// --- Main Function ---
int main() {
    while (true) {
        std::cout << "\n=== MENU ===\n";
        std::cout << "1. View Products\n2. View Shopping Cart\n3. View Orders\n4. Exit\n";
        std::cout << "Enter your choice: ";
        std::string choice;
        std::cin >> choice;

        if (choice == "1") {
            viewProducts();
            std::string addMore;
            do {
                std::cout << "Enter Product ID to add to cart: ";
                std::string prodId;
                std::cin >> prodId;
                if (!isValidProductId(prodId)) {
                    std::cout << "Invalid Product ID.\n";
                    continue;
                }
                Product* p = findProductById(prodId);
                if (p != nullptr) {
                    SingletonCart::getInstance().addProduct(*p);
                    std::cout << "Product added successfully!\n";
                }
                do {
                    std::cout << "Add another product? (YES/NO): ";
                    std::cin >> addMore;
                    if (!isValidYesNo(addMore)) {
                        std::cout << "Invalid input. Only YES or NO allowed.\n";
                    }
                } while (!isValidYesNo(addMore));
            } while (isYes(addMore));

        } else if (choice == "2") {
            SingletonCart::getInstance().viewCart();
            if (!SingletonCart::getInstance().isEmpty()) {
                std::string check;
                do {
                    std::cout << "Checkout all products? (YES/NO): ";
                    std::cin >> check;
                    if (!isValidYesNo(check)) {
                        std::cout << "Invalid input. Only YES or NO allowed.\n";
                    }
                } while (!isValidYesNo(check));
                if (isYes(check)) {
                    checkout();
                }
            }
        } else if (choice == "3") {
            viewOrders();
        } else if (choice == "4") {
            break;
        } else {
            std::cout << "Invalid option.\n";
        }
    }
    return 0;
}
