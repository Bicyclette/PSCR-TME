#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <cstddef>
#include <forward_list>
#include <vector>

namespace pr {
	template<typename K, typename V>
	class HashMap{
		public:
			class Entry{
				public:
					const K key;
					V val;
					Entry(const K & key, V & val) : key(key), val(val) {}
			};
		private:
			typedef std::vector<std::forward_list<Entry>> buckets_t;
			buckets_t buckets;
			size_t sz;
		public:
			HashMap(size_t size) : sz(0), buckets(sz) {}
			
			V* get(const K & key) {
				size_t h = std::hash<K>()(key);
				size_t target = h % buckets.size();
				for(Entry & e : buckets[target]){
					if(e.key == key){
						return & e.val;
					}
				}
				return nullptr;
			}

			bool put(const K & key, const V & val){
				size_t h = std::hash<K>()(key);
				size_t target = h % buckets.size();
				for(Entry & e : buckets[target])
				{
					if(e.key == key){
						e.val = val;
						return true;
					}
				}
				sz++;
				buckets[target].emplace_front(key, val);
				return false;
			}

			size_t size() const {return sz;}
	};
};

#endif
