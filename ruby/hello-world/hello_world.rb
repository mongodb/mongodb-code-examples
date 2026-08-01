require 'mongo'
require 'json'

# Keep driver logging quiet so the app prints only the query result.
Mongo::Logger.logger.level = Logger::WARN

uri = ENV['MONGODB_URI']
if uri.nil? || uri.empty?
  warn 'Set the MONGODB_URI environment variable to your connection string'
  exit 1
end

# A few sample product documents seeded by this app so you can run
# it without loading an external dataset.
SAMPLE_PRODUCTS = [
  {
    name: 'Wireless Mouse',
    category: 'Electronics',
    price: 24.99,
    tags: %w[wireless usb ergonomic]
  },
  {
    name: 'Standing Desk',
    category: 'Furniture',
    price: 349.99,
    tags: %w[adjustable office]
  },
  {
    name: 'Noise-Cancelling Headphones',
    category: 'Electronics',
    price: 199.99,
    tags: %w[bluetooth wireless over-ear]
  }
].freeze

client = Mongo::Client.new(uri, database: 'get_started')

begin
  products = client[:products]

  # Seed the collection so the app has data to query. Clearing the
  # collection first keeps results consistent across repeated runs.
  products.delete_many({})
  products.insert_many(SAMPLE_PRODUCTS)

  product = products.find(name: 'Wireless Mouse').first
  puts JSON.generate(product.as_extended_json(mode: :relaxed))
ensure
  client.close
end
