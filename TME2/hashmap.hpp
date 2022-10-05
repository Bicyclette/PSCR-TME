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
			const std::vector<std::forward_list<Entry>>& get_buckets() const {
				return buckets;
			}
		public:
			auto begin(){
				return buckets.begin();
			}
			auto end(){
				return buckets.end();
			}
/*
			struct Iterator{
				public: // tags for <algorithm>
					using iterator_category = std::forward_iterator_tag;
					using difference_type = std::ptrdiff_t;
					using value_type = Entry;
					using pointer = Entry*;
					using reference = Entry&;
				public:
					Iterator(Entry* ptr) : m_ptr(ptr) {}
					reference operator*() const {return *m_ptr;}
					pointer operator->() {return m_ptr;}
					//Iterator& operator++() {return }
					friend bool operator==(const Iterator& a, const Iterator& b){
						return a.m_ptr == b.m_ptr;
					}
					friend bool operator!=(const Iterator& a, const Iterator& b){
						return a.m_ptr != b.m_ptr;
					}
				private:
					//auto m_vec_it;
					//auto m_list_it;
					Entry* m_ptr;
			};

			Iterator begin(){
				return Iterator(*buckets[0].begin());
			}
			Iterator end(){
				return Iterator(*buckets[sz-1].end());
			}
*/
	template<typename my_iterator>
	size_t count(my_iterator begin, my_iterator end){
		size_t c = 0;
		while(begin != end){
			auto data = *begin;
			for(auto it = data.begin(); it != data.end(); ++it){
				c += 1;
			}
			++begin;
		}
		return c;
	}
	template<typename my_iterator, typename T>
	size_t count_if_equal(my_iterator begin, my_iterator end, const T & val){
		size_t c = 0;
		while(begin != end){
			auto data = *begin;
			for(auto it = data.begin(); it != data.end(); ++it){
				if((*it) == val){
					c += 1;
				}
			}
			++begin;
		}
		return c;
	}
	};
};

#endif
