#include <stdio.h>
#include <stdlib.h>
#include <mongoc/mongoc.h>

typedef struct {
   const char *name;
   const char *category;
   const char *price;
   const char *tags[8];
   size_t tag_count;
} sample_product_t;

/* A few sample product documents seeded by this app so you can run it
 * without loading an external dataset. */
static const sample_product_t sample_products[] = {
   {"Wireless Mouse", "Electronics", "24.99", {"wireless", "usb", "ergonomic"}, 3},
   {"Standing Desk", "Furniture", "349.99", {"adjustable", "office"}, 2},
   {"Noise-Cancelling Headphones", "Electronics", "199.99", {"bluetooth", "wireless", "over-ear"}, 3},
};

#define SAMPLE_PRODUCT_COUNT (sizeof (sample_products) / sizeof (sample_products[0]))

static bson_t *
product_to_bson (const sample_product_t *product)
{
   bson_t *doc = bson_new ();
   bson_decimal128_t price;
   bson_array_builder_t *tags;

   bson_decimal128_from_string (product->price, &price);
   BSON_APPEND_UTF8 (doc, "name", product->name);
   BSON_APPEND_UTF8 (doc, "category", product->category);
   BSON_APPEND_DECIMAL128 (doc, "price", &price);

   bson_append_array_builder_begin (doc, "tags", -1, &tags);
   for (size_t i = 0; i < product->tag_count; i++) {
      bson_array_builder_append_utf8 (tags, product->tags[i], -1);
   }
   bson_append_array_builder_end (doc, tags);

   return doc;
}

/* Prints a product document using MongoDB Shell-style formatting,
 * preserving types such as ObjectId and NumberDecimal. */
static void
print_shell_product (const bson_t *product)
{
   bson_iter_t iter;
   bson_oid_t oid;
   char oid_str[25];

   printf ("{ ");

   if (bson_iter_init_find (&iter, product, "_id")) {
      bson_oid_copy (bson_iter_oid (&iter), &oid);
      bson_oid_to_string (&oid, oid_str);
      printf ("\"_id\" : ObjectId(\"%s\"), ", oid_str);
   }

   if (bson_iter_init_find (&iter, product, "name")) {
      printf ("\"name\" : \"%s\", ", bson_iter_utf8 (&iter, NULL));
   }

   if (bson_iter_init_find (&iter, product, "category")) {
      printf ("\"category\" : \"%s\", ", bson_iter_utf8 (&iter, NULL));
   }

   if (bson_iter_init_find (&iter, product, "price")) {
      bson_decimal128_t price;
      char price_str[BSON_DECIMAL128_STRING];
      bson_iter_decimal128 (&iter, &price);
      bson_decimal128_to_string (&price, price_str);
      printf ("\"price\" : NumberDecimal(\"%s\"), ", price_str);
   }

   if (bson_iter_init_find (&iter, product, "tags")) {
      bson_iter_t tags_iter;
      bool first = true;

      printf ("\"tags\" : [");
      BSON_ASSERT (bson_iter_recurse (&iter, &tags_iter));
      while (bson_iter_next (&tags_iter)) {
         printf ("%s\"%s\"", first ? "" : ", ", bson_iter_utf8 (&tags_iter, NULL));
         first = false;
      }
      printf ("]");
   }

   printf (" }\n");
}

int
main (void)
{
   mongoc_client_t *client;
   mongoc_collection_t *products;
   const char *uri_string;
   bson_t empty_filter = BSON_INITIALIZER;
   bson_t name_filter = BSON_INITIALIZER;
   bson_t *inserts[SAMPLE_PRODUCT_COUNT];
   bson_error_t error;
   const bson_t *product;
   mongoc_cursor_t *cursor;
   int exit_code = EXIT_SUCCESS;

   uri_string = getenv ("MONGODB_URI");
   if (!uri_string) {
      fprintf (stderr, "Set the MONGODB_URI environment variable before running this app.\n");
      return EXIT_FAILURE;
   }

   mongoc_init ();

   client = mongoc_client_new (uri_string);
   if (!client) {
      fprintf (stderr, "Failed to parse MONGODB_URI.\n");
      mongoc_cleanup ();
      return EXIT_FAILURE;
   }

   products = mongoc_client_get_collection (client, "get_started", "products");

   /* Seed the collection so the app has data to query. Clearing the
    * collection first keeps results consistent across repeated runs. */
   if (!mongoc_collection_delete_many (products, &empty_filter, NULL, NULL, &error)) {
      fprintf (stderr, "Delete failed: %s\n", error.message);
      exit_code = EXIT_FAILURE;
      goto cleanup;
   }

   for (size_t i = 0; i < SAMPLE_PRODUCT_COUNT; i++) {
      inserts[i] = product_to_bson (&sample_products[i]);
   }
   if (!mongoc_collection_insert_many (
          products, (const bson_t **) inserts, SAMPLE_PRODUCT_COUNT, NULL, NULL, &error)) {
      fprintf (stderr, "Insert failed: %s\n", error.message);
      exit_code = EXIT_FAILURE;
   }
   for (size_t i = 0; i < SAMPLE_PRODUCT_COUNT; i++) {
      bson_destroy (inserts[i]);
   }
   if (exit_code == EXIT_FAILURE) {
      goto cleanup;
   }

   BSON_APPEND_UTF8 (&name_filter, "name", "Wireless Mouse");
   cursor = mongoc_collection_find_with_opts (products, &name_filter, NULL, NULL);
   if (mongoc_cursor_next (cursor, &product)) {
      print_shell_product (product);
   } else if (mongoc_cursor_error (cursor, &error)) {
      fprintf (stderr, "Query failed: %s\n", error.message);
      exit_code = EXIT_FAILURE;
   }
   mongoc_cursor_destroy (cursor);

cleanup:
   bson_destroy (&name_filter);
   mongoc_collection_destroy (products);
   mongoc_client_destroy (client);
   mongoc_cleanup ();

   return exit_code;
}
