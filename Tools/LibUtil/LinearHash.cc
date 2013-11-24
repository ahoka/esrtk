#include <vector>
#include <list>
#include <functional>
#include <algorithm>
#include <tuple>
#include <cmath>

template <class K, class V>
class LinearHash
{
public:
   LinearHash()
      : initialSize(8),
	position(0),
	iteration(1)
   {
      table.resize(initialSize);
   }

   bool insert(K key, V value)
   {
      auto item = std::make_tuple(key, value);

      auto index = h(key);
      if (table[index].size() > maxFill)
      {
	 split();
      }

      table[index].push_front(item);
   }

   V find(K key)
   {
      auto index = h(key);

      if (index < 
   }

private:
   void
   split()
   {
      std::size_t newSize = iteration * initialSize + position;
      std::size_t overflowIndex = newSize - 1;

      table.resize(iteration * initialSize + position);

      std::remove_copy_if(table[position].begin(), table[position].end(),
			  table[overflowIndex],
			  [this](const Item& item)
			  {
			     return h(std::get<Key>(item)) != position;
			  });

      position++;
      // XXX itereation!!!
   }
   
   std::size_t h(K key, int i)
   {
      return std::hash<K>()(key) % (std::size_t(pow(2, i)) * initialSize);
   }

   std::size_t h(K key)
   {
      return h(key, iteration);
   }

   using Item = std::tuple<K, V>;
   enum
   {
      Key = 0,
      Value = 1
   };

   using Values = std::list<Item>;
   using Hash = std::vector<Values>;

   enum
   {
      maxFill = 4
   };

   std::size_t initialSize;
   std::size_t position;
   int iteration;

   Hash table;
};

int
main()
{
}
