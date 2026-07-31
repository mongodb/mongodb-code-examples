<?php

require __DIR__ . '/../vendor/autoload.php';

use MongoDB\Client;

$uri = getenv('MONGODB_URI');
if ($uri === false || $uri === '') {
    fwrite(STDERR, "Set the MONGODB_URI environment variable to your connection string\n");
    exit(1);
}

// A few sample product documents seeded by this app so you can run
// it without loading an external dataset.
$sampleProducts = [
    [
        'name' => 'Wireless Mouse',
        'category' => 'Electronics',
        'price' => 24.99,
        'tags' => ['wireless', 'usb', 'ergonomic'],
    ],
    [
        'name' => 'Standing Desk',
        'category' => 'Furniture',
        'price' => 349.99,
        'tags' => ['adjustable', 'office'],
    ],
    [
        'name' => 'Noise-Cancelling Headphones',
        'category' => 'Electronics',
        'price' => 199.99,
        'tags' => ['bluetooth', 'wireless', 'over-ear'],
    ],
];

$client = new Client($uri);
$products = $client->get_started->products;

// Seed the collection so the app has data to query. Clearing the
// collection first keeps results consistent across repeated runs.
$products->deleteMany([]);
$products->insertMany($sampleProducts);

$product = $products->findOne(['name' => 'Wireless Mouse']);

echo json_encode($product), "\n";
