import os

from pymongo import MongoClient

# A few sample product documents seeded by this app so you can run it
# without loading an external dataset.
sample_products = [
    {
        "name": "Wireless Mouse",
        "category": "Electronics",
        "price": 24.99,
        "tags": ["wireless", "usb", "ergonomic"],
    },
    {
        "name": "Standing Desk",
        "category": "Furniture",
        "price": 349.99,
        "tags": ["adjustable", "office"],
    },
    {
        "name": "Noise-Cancelling Headphones",
        "category": "Electronics",
        "price": 199.99,
        "tags": ["bluetooth", "wireless", "over-ear"],
    },
]


def run_get_started():
    uri = os.environ["MONGODB_URI"]
    client = MongoClient(uri)

    try:
        database = client["get_started"]
        products = database["products"]

        # Seed the collection so the app has data to query. Clearing the
        # collection first keeps results consistent across repeated runs.
        products.delete_many({})
        products.insert_many(sample_products)

        query = {"name": "Wireless Mouse"}
        product = products.find_one(query)
        print(product)
    finally:
        client.close()


if __name__ == "__main__":
    run_get_started()
