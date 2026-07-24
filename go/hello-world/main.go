package main

import (
	"context"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"

	"go.mongodb.org/mongo-driver/v2/bson"
	"go.mongodb.org/mongo-driver/v2/mongo"
	"go.mongodb.org/mongo-driver/v2/mongo/options"
)

type Product struct {
	ID       bson.ObjectID   `bson:"_id,omitempty" json:"_id,omitempty"`
	Name     string          `bson:"name" json:"name"`
	Category string          `bson:"category" json:"category"`
	Price    bson.Decimal128 `bson:"price" json:"price"`
	Tags     []string        `bson:"tags" json:"tags"`
}

func mustDecimal128(value string) bson.Decimal128 {
	price, err := bson.ParseDecimal128(value)
	if err != nil {
		panic(err)
	}
	return price
}

// A few sample product documents seeded by this app so you can run it
// without loading an external dataset.
var sampleProducts = []Product{
	{
		Name:     "Wireless Mouse",
		Category: "Electronics",
		Price:    mustDecimal128("24.99"),
		Tags:     []string{"wireless", "usb", "ergonomic"},
	},
	{
		Name:     "Standing Desk",
		Category: "Furniture",
		Price:    mustDecimal128("349.99"),
		Tags:     []string{"adjustable", "office"},
	},
	{
		Name:     "Noise-Cancelling Headphones",
		Category: "Electronics",
		Price:    mustDecimal128("199.99"),
		Tags:     []string{"bluetooth", "wireless", "over-ear"},
	},
}

func main() {
	uri := os.Getenv("MONGODB_URI")
	if uri == "" {
		log.Fatal("Set the MONGODB_URI environment variable before running this app.")
	}

	ctx := context.Background()
	client, err := mongo.Connect(options.Client().ApplyURI(uri))
	if err != nil {
		log.Fatal(err)
	}
	defer func() {
		if err := client.Disconnect(ctx); err != nil {
			log.Print(err)
		}
	}()

	if err := client.Ping(ctx, nil); err != nil {
		log.Fatal(err)
	}

	products := client.Database("get_started").Collection("products")

	// Seed the collection so the app has data to query. Clearing the
	// collection first keeps results consistent across repeated runs.
	if _, err := products.DeleteMany(ctx, bson.D{}); err != nil {
		log.Fatal(err)
	}

	documents := make([]interface{}, len(sampleProducts))
	for i := range sampleProducts {
		documents[i] = sampleProducts[i]
	}
	if _, err := products.InsertMany(ctx, documents); err != nil {
		log.Fatal(err)
	}

	var product Product
	filter := bson.D{{Key: "name", Value: "Wireless Mouse"}}
	if err := products.FindOne(ctx, filter).Decode(&product); err != nil {
		log.Fatal(err)
	}

	fmt.Println(formatShellProduct(product))
}

func formatShellProduct(product Product) string {
	tags := make([]string, len(product.Tags))
	for i, tag := range product.Tags {
		tags[i] = strconv.Quote(tag)
	}

	return fmt.Sprintf(
		`{ "_id" : ObjectId("%s"), "name" : %s, "category" : %s, "price" : NumberDecimal("%s"), "tags" : [%s] }`,
		product.ID.Hex(),
		strconv.Quote(product.Name),
		strconv.Quote(product.Category),
		product.Price.String(),
		strings.Join(tags, ", "),
	)
}
