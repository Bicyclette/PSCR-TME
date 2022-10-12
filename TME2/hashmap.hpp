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
					Entry(const K & key, const V & val) : key(key), val(val) {}
			};
		private:
			typedef std::vector<std::forward_list<Entry>> buckets_t;
			buckets_t buckets;
			size_t sz;
		public:
			HashMap(size_t size) : buckets(size), sz(0) {}
			V* get(const K & key){
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
			const std::vector<std::forward_list<Entry>> & get_buckets() const {
				return buckets;
			}
		public:
			
			class Iterator{
				public:
					Iterator(HashMap<K,V>& hm, bool beg) : buckets(hm.buckets)
					{
						if(beg) 
						{
							vit = buckets.begin();
							while(vit->empty())
							{
								++vit;
							}
							lit = vit->begin();
						}
						else
						{
							vit = buckets.end();
							lit = vit->end();
						}
					}
					
					Entry& operator*() const
					{
						return *lit;
					}
					
					bool operator!=(const Iterator & other)
					{
						return vit != other.vit && lit != other.lit;
					}

					Iterator& operator++() {
						++lit;
						if(lit == vit->end())
						{
							do
							{
								++vit;
							}while(vit != buckets.end() && vit->empty());
							
							if(vit != buckets.end() && !vit->empty())
							{
								lit = vit->begin();
							}
						}
						return *this;
					}
				
				private:
					std::vector<std::forward_list<HashMap::Entry>> & buckets;
					typename std::vector<std::forward_list<HashMap::Entry>>::iterator vit;
					typename std::forward_list<HashMap::Entry>::iterator lit;
			};

			Iterator begin(){
				return Iterator(*this, true);
			}
			Iterator end(){
				return Iterator(*this, false);
			}
	};
	
	template<typename iterator>
	size_t count(iterator begin, iterator end){
		size_t c = 0;
		while(begin != end){
			++begin;
			++c;
		}
		return c;
	}

	template<typename iterator, typename T>
	size_t count_if_equal(iterator begin, iterator end, const T & val){
		size_t c = 0;
		while(begin != end){
			++begin;
			if(*begin == val){
				++c;
			}
		}
		return c;
	}
};

#endif
