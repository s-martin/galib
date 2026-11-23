/* ----------------------------------------------------------------------------
  mbwall 25feb95
  Copyright 1995 Massachusetts Institute of Technology
 ---------------------------------------------------------------------------- */

#pragma once

#include <GAListBASE.h>
#include <gaerror.h>

#include <iterator>
#include <list>
#include <utility>

template <class T> class GAListIter;

/** List of objects, implemented on top of std::list.
 *
 * You never get nodes back, you get pointers to the contents of the nodes.
 * We include an iterator in this object so that you can navigate through the
 * list.  You can create another iterator (GAListIter) and assign it to your list
 * so you can have multiple iterators.
 * All of the actions take place relative to the current location of the
 * embedded iterator.  Be careful so that you don't end up with an iterator
 * dangling at an element that no longer exists.
 *
 * The list behaves as a circular list: next() at the tail wraps to the head and
 * prev() at the head wraps to the tail.
 *
 * The contents must be copy constructible.
 */
template <class T> class GAList : public GAListBASE
{
  public:
	GAList() : iter(*this) {}
	explicit GAList(const T &t) : items(1, t), iter(*this) { iter.head(); }
	GAList(const GAList<T> &orig) : items(orig.items), iter(*this) { iter.head(); }
	GAList<T> &operator=(const GAList<T> &orig)
	{
		if (&orig != this)
			copy(orig);
		return *this;
	}
	virtual ~GAList() = default;

	/** Number of elements in the list. */
	int size() const { return static_cast<int>(items.size()); }

	/** Make a copy of a list and return the pointer to the new list.
	 *
	 * This routine has no effect on the iterator in the original list. The
	 * iterator of the new list is at its head.
	 *
	 * @param i 0: will clone entire list; other values will clone the list from the index to the end of the list.
	 * @return GAList<T>* the caller owns the returned list
	 */
	GAList<T> *clone(unsigned int i = 0) const
	{
		auto *t = new GAList<T>;
		if (i >= items.size())
			return t; // empty or out of range: empty list
		auto first = items.begin();
		std::advance(first, i);
		t->items.assign(first, items.end());
		t->iter.head();
		return t;
	}

	/** Make a complete copy of the list.
	 *
	 * The iterator is set to the head of the list, not to the node corresponding to the iterator of orig.
	 */
	void copy(const GAList<T> &orig)
	{
		items = orig.items;
		iter.head();
	}

	/** Remove the current node from the list and free the memory it was using.
	 *
	 * Once the node is gone, the current node is the previous node. If the node
	 * that was destroyed was the head then the current node is the new head.
	 * @return error code (always NO_ERR)
	 */
	int destroy()
	{
		if (!iter.valid)
			return NO_ERR;
		auto doomed = iter.node;
		if (items.size() == 1)
			iter.valid = false;
		else if (doomed == items.begin())
			iter.node = std::next(doomed);
		else
			iter.node = std::prev(doomed);
		items.erase(doomed);
		return NO_ERR;
	}

	/** Swap two nodes in the list.
	 *
	 * This has no effect on the size or the iterator (the iterator stays on the
	 * same node). If either index is out of bounds, or both are the same, then
	 * we don't do anything.
	 *
	 * @return error code (always NO_ERR)
	 */
	int swap(unsigned int a, unsigned int b)
	{
		if (a == b || a >= items.size() || b >= items.size())
			return NO_ERR;
		if (a > b)
			std::swap(a, b);
		auto ia = items.begin();
		std::advance(ia, a);
		auto ib = ia;
		std::advance(ib, b - a);
		auto afterb = std::next(ib);
		items.splice(ia, items, ib);
		items.splice(afterb, items, ia);
		return NO_ERR;
	}

	/** Remove the current node from the list.
	 *
	 * The iterator gets set to the previous node (the tail if the head was removed).
	 *
	 * @return copy of the contents of the removed node, or nullptr if there is no current node. The caller owns the returned object.
	 */
	T *remove()
	{
		if (!iter.valid)
			return nullptr;
		auto doomed = iter.node;
		T *contents = new T(*doomed);
		if (items.size() == 1)
			iter.valid = false;
		else
			iter.node = (doomed == items.begin()) ? std::prev(items.end()) : std::prev(doomed);
		items.erase(doomed);
		return contents;
	}

	/** Inserts the contents of list in to the current list and removes it from the original list
	 *
	 * The original list is not deleted but is empty afterwards. The iterator is
	 * set to the first inserted element.
	 *
	 * @param t List to be inserted
	 * @param where Location where list is to be inserted (default: AFTER)
	 * @return NO_ERR or ERR
	 */
	int insert(GAList<T> *t, Location where = AFTER)
	{
		if (this == t)
		{
			GAErr(GA_LOC, "GAList", "insert", GAError::CannotInsertIntoSelf);
			return ERR;
		}
		if (t->items.empty())
			return NO_ERR;
		auto pos = insertPosition(where);
		if (pos.second == ERR)
			return ERR;
		auto first = t->items.begin();
		items.splice(pos.first, t->items);
		iter.node = first;
		iter.valid = true;
		t->iter.valid = false;
		return NO_ERR;
	}

	/** Insert the object into the list at the specified place relative to the current location of the embedded iterator
	 *
	 * The iterator is set to the new element.
	 *
	 * @param t Object to be inserted into list
	 * @param where Insert location (default: AFTER)
	 * @return NO_ERR or ERR
	 */
	int insert(const T &t, Location where = AFTER)
	{
		auto pos = insertPosition(where);
		if (pos.second == ERR)
			return ERR;
		iter.node = items.insert(pos.first, t);
		iter.valid = true;
		return NO_ERR;
	}

	// typesafes on iteration methods.  These call the built-in iterator then
	// return the contents of the now-current node.  They do not affect the
	// state of the list.
	T *head() { return iter.head(); }
	T *tail() { return iter.tail(); }
	T *current() { return iter.current(); }
	T *next() { return iter.next(); }
	T *prev() { return iter.prev(); }
	T *warp(unsigned int i) { return iter.warp(i); }
	T *warp(const GAListIter<T> &i)
	{
		return ((i.list == this) ? iter.warp(i) : nullptr);
	}
	T *operator[](unsigned int i) { return iter.warp(i); }

  private:
	// Translate the insert location into a position in the std::list.  An empty
	// list always inserts at the head.  BEFORE the head inserts at the end.
	std::pair<typename std::list<T>::iterator, int> insertPosition(Location where)
	{
		if (items.empty())
			where = HEAD;
		else if (!iter.valid && where != HEAD && where != TAIL)
		{
			GAErr(GA_LOC, "GAList", "insert", GAError::CannotInsertOnNilNode);
			return {items.end(), ERR};
		}
		switch (where)
		{
		case HEAD:
			return {items.begin(), NO_ERR};
		case TAIL:
			return {items.end(), NO_ERR};
		case BEFORE:
			return {iter.node == items.begin() ? items.end() : iter.node, NO_ERR};
		case AFTER:
			return {std::next(iter.node), NO_ERR};
		}
		GAErr(GA_LOC, "GAList", "insert", GAError::BadWhereIndicator);
		return {items.end(), ERR};
	}

	std::list<T> items;
	GAListIter<T> iter; // the embedded iterator
	friend class GAListIter<T>;
};

/** Iterator for GAList.
 *
 * An iterator is independent of the iterator embedded in the list and of other
 * iterators. Navigation is circular: next() at the tail wraps to the head and
 * prev() at the head wraps to the tail. The iterator is invalid (current()
 * returns nullptr) until it is moved onto an element, and when the list is
 * empty.
 *
 * When we create a new list iterator from another iterator, it defaults to the
 * same node. If it is created with a list as its argument, it defaults to the
 * list's iterator's current position.
 */
template <class T> class GAListIter
{
  public:
	GAListIter() = default;
	explicit GAListIter(const GAList<T> &t) { (*this)(t); }
	GAListIter(const GAListIter<T> &i) = default;
	GAListIter<T> &operator=(const GAListIter<T> &i) = default;

	void operator()(const GAList<T> &t)
	{
		list = const_cast<GAList<T> *>(&t);
		node = t.iter.node;
		valid = t.iter.valid;
	}

	T *current() { return valid ? &*node : nullptr; }
	T *head()
	{
		if (list == nullptr)
			return nullptr;
		valid = !list->items.empty();
		node = list->items.begin();
		return current();
	}
	T *tail()
	{
		if (list == nullptr)
			return nullptr;
		valid = !list->items.empty();
		node = valid ? std::prev(list->items.end()) : list->items.end();
		return current();
	}
	T *next()
	{
		if (!valid)
			return nullptr;
		if (++node == list->items.end())
			node = list->items.begin();
		return &*node;
	}
	T *prev()
	{
		if (!valid)
			return nullptr;
		if (node == list->items.begin())
			node = std::prev(list->items.end());
		else
			--node;
		return &*node;
	}

	/** Move the iterator to the element referenced by index.
	 *
	 * The head is element 0. If the index is out of bounds nullptr is returned
	 * and the iterator is left at the head.
	 */
	T *warp(unsigned int i)
	{
		if (head() == nullptr || i >= list->items.size())
			return nullptr;
		std::advance(node, i);
		return &*node;
	}
	/** Move the iterator to the current position of the list's embedded iterator. */
	T *warp(const GAList<T> &t)
	{
		(*this)(t);
		return current();
	}
	/** Move the iterator to the position of another iterator. */
	T *warp(const GAListIter<T> &i)
	{
		list = i.list;
		node = i.node;
		valid = i.valid;
		return current();
	}

	int size() const { return list != nullptr ? list->size() : 0; }

  private:
	GAList<T> *list = nullptr;
	typename std::list<T>::iterator node{};
	bool valid = false;
	friend class GAList<T>;
};
