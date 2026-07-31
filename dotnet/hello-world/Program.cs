using MongoDB.Bson;
using MongoDB.Bson.IO;
using MongoDB.Driver;

// A few sample product documents seeded by this app so you can run it
// without loading an external dataset.
var sampleProducts = new[]
{
    new BsonDocument
    {
        { "name", "Wireless Mouse" },
        { "category", "Electronics" },
        { "price", 24.99m },
        { "tags", new BsonArray { "wireless", "usb", "ergonomic" } }
    },
    new BsonDocument
    {
        { "name", "Standing Desk" },
        { "category", "Furniture" },
        { "price", 349.99m },
        { "tags", new BsonArray { "adjustable", "office" } }
    },
    new BsonDocument
    {
        { "name", "Noise-Cancelling Headphones" },
        { "category", "Electronics" },
        { "price", 199.99m },
        { "tags", new BsonArray { "bluetooth", "wireless", "over-ear" } }
    }
};

var uri = Environment.GetEnvironmentVariable("MONGODB_URI");
var client = new MongoClient(uri);

var database = client.GetDatabase("get_started");
var products = database.GetCollection<BsonDocument>("products");

// Seed the collection so the app has data to query. Clearing the
// collection first keeps results consistent across repeated runs.
products.DeleteMany(Builders<BsonDocument>.Filter.Empty);
products.InsertMany(sampleProducts);

var filter = Builders<BsonDocument>.Filter.Eq("name", "Wireless Mouse");
var product = products.Find(filter).FirstOrDefault();
Console.WriteLine(product.ToJson(new JsonWriterSettings { OutputMode = JsonOutputMode.Shell }));
