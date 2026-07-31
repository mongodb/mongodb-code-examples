# Get Started with the MongoDB PHP Library

This sample application connects to a MongoDB deployment, seeds a small
set of sample product documents, and retrieves one of them. Because the
app inserts its own data, you don't need to load an external dataset.

## Prerequisites

Before you begin, complete the [Atlas Get Started guide](https://www.mongodb.com/docs/get-started/)
to create a free Atlas deployment and save your database user
credentials.

You also need the following components installed in your development environment:

- PHP version 8.2 or later
- Composer version 2.0 or later
- The MongoDB PHP extension (`ext-mongodb`)

Follow the [Get Started with the PHP Library](https://www.mongodb.com/docs/php-library/current/get-started/)
guide to install the PHP extension for your platform.

## Installation

Clone this repository:

```bash
git clone https://github.com/mongodb/mongodb-code-examples
```

Navigate into the `php/hello-world` project directory and install the
MongoDB PHP library with Composer:

```bash
cd mongodb-code-examples/php/hello-world
composer install
```

## Connect to MongoDB

Set your connection string as an environment variable, replacing
`<connection string uri>` with your connection string:

```bash
export MONGODB_URI="<connection string uri>"
```

## Run the Application

```bash
php src/HelloWorld.php
```

When you run the app, it inserts a few product documents into the
`get_started.products` collection, then queries and prints one of them:

```
{"_id":{"$oid":"..."},"name":"Wireless Mouse","category":"Electronics","price":24.99,"tags":["wireless","usb","ergonomic"]}
```

You can run the app more than once. It clears the collection before
each run, so the results stay consistent.

If you encounter an error or see no output, verify that you set the
`MONGODB_URI` environment variable correctly.
