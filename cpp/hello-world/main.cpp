#include <array>
#include <charconv>
#include <cstdlib>
#include <iostream>
#include <string>

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

// A few sample product documents seeded by this app so you can run it
// without loading an external dataset.
std::vector<bsoncxx::document::value> sample_products() {
    std::vector<bsoncxx::document::value> products;

    products.push_back(make_document(
        kvp("name", "Wireless Mouse"),
        kvp("category", "Electronics"),
        kvp("price", 24.99),
        kvp("tags", make_array("wireless", "usb", "ergonomic"))));

    products.push_back(make_document(
        kvp("name", "Standing Desk"),
        kvp("category", "Furniture"),
        kvp("price", 349.99),
        kvp("tags", make_array("adjustable", "office"))));

    products.push_back(make_document(
        kvp("name", "Noise-Cancelling Headphones"),
        kvp("category", "Electronics"),
        kvp("price", 199.99),
        kvp("tags", make_array("bluetooth", "wireless", "over-ear"))));

    return products;
}

int main() {
    const char* uri_env = std::getenv("MONGODB_URI");
    if (uri_env == nullptr) {
        std::cerr << "Set the MONGODB_URI environment variable to your "
                     "connection string.\n";
        return EXIT_FAILURE;
    }

    mongocxx::instance instance{};
    mongocxx::client client{mongocxx::uri{uri_env}};

    auto database = client["get_started"];
    auto products = database["products"];

    // Seed the collection so the app has data to query. Clearing the
    // collection first keeps results consistent across repeated runs.
    products.delete_many({});
    products.insert_many(sample_products());

    auto filter = make_document(kvp("name", "Wireless Mouse"));
    auto product = products.find_one(filter.view());
    if (product) {
        auto view = product->view();

        std::array<char, 32> buf;
        auto [ptr, ec] = std::to_chars(
            buf.data(), buf.data() + buf.size(), view["price"].get_double().value);
        std::string price(buf.data(), ptr);

        std::cout << "{ \"_id\" : { \"$oid\" : \""
                  << view["_id"].get_oid().value.to_string() << "\" }"
                  << ", \"name\" : \"" << view["name"].get_string().value << "\""
                  << ", \"category\" : \"" << view["category"].get_string().value
                  << "\""
                  << ", \"price\" : " << price << ", \"tags\" : [";
        bool first = true;
        for (auto tag : view["tags"].get_array().value) {
            std::cout << (first ? " " : ", ") << "\"" << tag.get_string().value
                      << "\"";
            first = false;
        }
        std::cout << " ] }\n";
    }

    return EXIT_SUCCESS;
}
