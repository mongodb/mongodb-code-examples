import com.mongodb.client.model.Filters.eq
import com.mongodb.kotlin.client.MongoClient
import org.bson.Document

// A few sample product documents seeded by this app so you can run
// it without loading an external dataset.
private val SAMPLE_PRODUCTS = listOf(
    Document("name", "Wireless Mouse")
        .append("category", "Electronics")
        .append("price", 24.99)
        .append("tags", listOf("wireless", "usb", "ergonomic")),
    Document("name", "Standing Desk")
        .append("category", "Furniture")
        .append("price", 349.99)
        .append("tags", listOf("adjustable", "office")),
    Document("name", "Noise-Cancelling Headphones")
        .append("category", "Electronics")
        .append("price", 199.99)
        .append("tags", listOf("bluetooth", "wireless", "over-ear"))
)

fun main() {
    val uri = System.getenv("MONGODB_URI")

    MongoClient.create(uri).use { client ->
        val database = client.getDatabase("get_started")
        val products = database.getCollection<Document>("products")

        // Seed the collection so the app has data to query. Clearing
        // the collection first keeps results consistent across
        // repeated runs.
        products.deleteMany(Document())
        products.insertMany(SAMPLE_PRODUCTS)

        val product = products.find(eq("name", "Wireless Mouse")).firstOrNull()
        println(product?.toJson())
    }
}
