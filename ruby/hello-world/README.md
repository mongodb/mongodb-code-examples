# Get Started with the MongoDB Ruby Driver

This sample application connects to a MongoDB deployment, seeds a small
set of sample product documents, and retrieves one of them. Because the
app inserts its own data, you don't need to load an external dataset.

## Prerequisites

Before you begin, complete the [Atlas Get Started guide](https://www.mongodb.com/docs/get-started/)
to create a free Atlas deployment and save your database user
credentials.

You also need the following components installed in your development environment:

- Ruby version 3.3 or later
- Bundler

The Ruby driver is not officially supported on Windows.

## Installation

Clone this repository:

```bash
git clone https://github.com/mongodb/mongodb-code-examples
```

Navigate into the `ruby/hello-world` project directory and install the
`mongo` gem with Bundler:

```bash
cd mongodb-code-examples/ruby/hello-world
bundle install
```

## Connect to MongoDB

Set your connection string as an environment variable, replacing
`<connection string uri>` with your connection string:

```bash
export MONGODB_URI="<connection string uri>"
```

## Run the Application

```bash
bundle exec ruby hello_world.rb
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
