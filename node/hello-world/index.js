import { MongoClient } from 'mongodb';

async function runGetStarted() {
  const uri = process.env.MONGODB_URI;
  const client = new MongoClient(uri);

  try {
    const database = client.db('sample_mflix');
    const movies = database.collection('movies');

    const query = { title: 'Back to the Future' };
    const movie = await movies.findOne(query);
    console.log(movie);
  } finally {
    await client.close();
  }
}
runGetStarted().catch(console.dir);

