import { MongoClient } from 'mongodb';

// A few sample product documents seeded by this app so you can run it
// without loading an external dataset.
const sampleProducts = [
  {
    name: 'Wireless Mouse',
    category: 'Electronics',
    price: 24.99,
    tags: ['wireless', 'usb', 'ergonomic']
  },
  {
    name: 'Standing Desk',
    category: 'Furniture',
    price: 349.99,
    tags: ['adjustable', 'office']
  },
  {
    name: 'Noise-Cancelling Headphones',
    category: 'Electronics',
    price: 199.99,
    tags: ['bluetooth', 'wireless', 'over-ear']
  }
];

async function runGetStarted() {
  const uri = process.env.MONGODB_URI;
  const client = new MongoClient(uri);

  try {
    const database = client.db('get_started');
    const products = database.collection('products');

    // Seed the collection so the app has data to query. Clearing the
    // collection first keeps results consistent across repeated runs.
    await products.deleteMany({});
    await products.insertMany(sampleProducts);

    const query = { name: 'Wireless Mouse' };
    const product = await products.findOne(query);
    console.log(product);
  } finally {
    await client.close();
  }
}
runGetStarted().catch(console.dir);
