# Get Started with the MongoDB Node.js Driver

This sample application connects to a MongoDB Atlas deployment and
retrieves a document from the sample dataset.

## Prerequisites

Before you begin, complete the [Atlas Get Started guide](https://www.mongodb.com/docs/get-started/)
to create a free Atlas deployment, load sample data, and save your
database user credentials.

You also need the following components installed in your development environment:

- Node.js v16.20.1 or later
- npm

## Installation

Clone this repository:

```bash
git clone https://github.com/mongodb/mongodb-code-examples
```

Navigate into the `node/hello-world` project directory and install dependencies:

```bash
cd mongodb-code-examples/node/hello-world
npm install
```

## Connect to MongoDB

Set your Atlas connection string as an environment variable, replacing
`<connection string uri>` with your connection string:

```bash
export MONGODB_URI="<connection string uri>"
```

## Run the Application

```bash
node index.js
```

The output includes details about the retrieved movie document:

```
{
  _id: ...,
  plot: 'A young man is accidentally sent 30 years into the past...',
  genres: [ 'Adventure', 'Comedy', 'Sci-Fi' ],
  ...
  title: 'Back to the Future',
  ...
}
```

If you encounter an error or see no output, verify that you set the
`MONGODB_URI` environment variable correctly and that you loaded the
sample data.
