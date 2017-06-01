// RestrictedMap: emphasizes the insertion and retrieval of keys
// with access to the values.
#pragma once
#include "BinSearchTree.h"
#include "OrderedPair.h"


// implements a map containing key/value pairs.
// a map does not contain multiple copies of the same item.
// types T and Key must have a default constructor
template <typename Key, typename T>
class RestrictedMap
{
public:

	typedef typename BinarySearchTree< OrderedPair<const Key, T> >::iterator map_iterator;
	typedef typename BinarySearchTree< OrderedPair<const Key, T> >::const_iterator const_map_iterator;
	// RestrictedMap iterators are simply BinarySearchTree iterators. an iterator cannot
	// change the key in a tree node, since the key attribute
	// of a OrderedPair object in the tree is const

	typedef OrderedPair<const Key, T> value_type;
	// for programmer convenience

	RestrictedMap() {};
	// default constructor. create an empty map

	RestrictedMap(value_type *first, value_type *last) { return t(first, last); };
	// build a map whose key/value pairs are determined by pointer
	// values (first, last)

	bool empty() const
	{
		return t.empty();
	}
	// is the map empty?

	int size() const
	{
		return t.size();
	}
	// return the number of elements in the map

	map_iterator find(const Key& key)
	// search for item in the map with the given key
	// and return an iterator pointing at it, or end()
	// if it is not found
	{
		return map_iterator(t.find(value_type(key,T())));
	}

	const_map_iterator find(const Key& key) const
	{
		// constant version of find()

		return const_map_iterator(t.find(value_type(key,T())));
	}

	T& operator[] (const Key& key)
	{
		// if no value is associated with key, create a new
		// map entry with the default value T() and return a
		// reference to the default value; otherwise,
		// return a reference to the value already associated
		// with the key

		/*value_type tempNode(key, T());
		map_iterator iter = t.insert(tempNode).first;
		return (*iter).second;*/
		map_iterator miFind = t.find(OrderedPair<const Key, T>(key, T()));
		
		if (t.end() == miFind)
		{
			OrderedPair<map_iterator, bool> result = t.insert(value_type(key, T()));
			return(*(result.first)).second;
		}
		else
		{
			
			return(*miFind).second;
		}
	}

	OrderedPair<map_iterator, bool> insert(const value_type& kvpair)
	{
		// if the map does not contain a key/value pair whose
		// key matches that of kvpair, insert a copy of kvpair
		// and return a OrderedPair object whose first element is an
		// map_iterator positioned at the new key/value pair and whose second
		// element is true. if the map already contains a key/value
		// pair whose key matches that of kvpair, return a OrderedPair
		// object whose first element is an map_iterator positioned at the
		// existing key/value pair and whose second element is false
		
		return t.insert(kvpair);

	}

	int erase(const Key& key)
	{
		// erase the key/value pair with the specified key
		// from the map and return the number
		// of items erased (1 or 0)
		if (t.find(value_type(key,T())) == t.end())
		{
			return 0;
		}
		else
		{
			t.erase(value_type(key,T()));
			return 1;
		}
	}

	void erase(map_iterator pos)
	{
		// erase the map key/value pair pointed by to pos
		t.erase(pos);
	}

	void erase(map_iterator first, map_iterator last)
	{
		t.erase(first, last);
	}
	// erase the key/value pairs in the range [first, last)

	map_iterator begin()
	{
		return map_iterator(t.begin());
	}
	// return an map_iterator pointing at the first member
	// in the map
	const_map_iterator begin() const
	{
		return const_map_iterator(t.begin());
	}
	// constant version of begin()

	map_iterator end()
	{
		return map_iterator(t.end());
	}
	// return an map_iterator pointing just past the last
	// member in the map
	const_map_iterator end() const
	{
		return const_map_iterator(t.end());
	}
	// constant version of end()

private:
	// RestrictedMap implemented using an BinarySearchTree of key-value pairs
	BinarySearchTree<OrderedPair<const Key, T> > t;
};
