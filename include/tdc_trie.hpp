/*
* Copyright 2012 Tino Didriksen <tino@didriksen.cc>
* http://tinodidriksen.com/
*/

/*
References:
- http://en.wikipedia.org/wiki/Trie
- http://www.wutka.com/dawg.html
*/

#pragma once
#ifndef __TDC_TRIE_HPP_28c53c53a48d38efafee7fb7004a01faaac9e22f
#define __TDC_TRIE_HPP_28c53c53a48d38efafee7fb7004a01faaac9e22f

#include <stdint.h>
#include <map>
#include <string>
#include <deque>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <limits>

namespace tdc {

template<typename T>
struct trie_serializer {
	void serialize(std::ostream& out, const T& value) const {
		out.write(reinterpret_cast<const char*>(&value), sizeof(T));
	}

	T unserialize(std::istream& in) const {
		T value;
		in.read(reinterpret_cast<char*>(&value), sizeof(T));
		return value;
	}
};

template<typename T, typename Y>
typename T::iterator findchild(T& t, const Y& y) {
	typename T::iterator it;
	for (it = t.begin() ; it != t.end() ; ++it) {
		if (it->first == y) {
			break;
		}
	}
	return it;
}

template<typename T, typename Y>
typename T::const_iterator findchild(const T& t, const Y& y) {
	typename T::const_iterator it;
	for (it = t.begin() ; it != t.end() ; ++it) {
		if (it->first == y) {
			break;
		}
	}
	return it;
}

template<typename T, typename Y>
void insertchild(T& t, const Y& y) {
	typename T::iterator it;
	for (it = t.begin() ; it != t.end() ; ++it) {
		if (it->first == y.first) {
			return;
		}
		if (it->first > y.first) {
			break;
		}
	}
	t.insert(it, y);
}

template<typename String, typename Serializer=trie_serializer<typename String::value_type> >
class trie {
private:

	class trie_node {
	protected:
		friend class trie;

		typedef trie_node node_type;
		typedef std::vector<std::pair<typename String::value_type, size_t> > children_type;
		typedef std::vector<const node_type*> query_path_type;
		typedef std::map<String,size_t> query_type;
		typedef trie root_type;

		bool terminal;
		typename String::value_type self;
		size_t children_depth;
		size_t parent;
		children_type children;

		void buildString(const query_path_type& qp, String& in) const {
			in.reserve(qp.size());
			for (typename query_path_type::const_iterator it = qp.begin() ; it != qp.end() ; ++it) {
				in.push_back((*it)->self);
			}
		}

	public:

		trie_node() :
		parent(std::numeric_limits<size_t>::max())
		{
		}

		trie_node(size_t parent, typename String::value_type self) :
		terminal(false),
		self(self),
		children_depth(0),
		parent(parent)
		{
		}

		bool add(root_type& root, const String& entry, size_t pos=0, size_t depth=0) {
			if (pos < entry.size()) {
				typename children_type::iterator child = findchild(children, entry[pos]);
				if (child != children.end()) {
					node_type& node = root.nodes[child->second];
					return node.add(root, entry, pos+1, depth+1);
				}
				else {
					size_t p = this - &root.nodes[0];
					size_t z = root.nodes.size();
					insertchild(children, std::make_pair(entry[pos], z));
					root.nodes.resize(z+1);
					root.nodes.back() = node_type(p, entry[pos]);
					root.nodes.back().add(root, entry, pos+1, depth+1);
					return true;
				}
			}
			else {
				if (!terminal) {
					terminal = true;
					updateChildrenDepth(root);
					return true;
				}
			}
			return false;
		}

		void query(const root_type& root, const String& entry, size_t pos, query_type& collected, query_path_type& qp, size_t maxdist=0, size_t curdist=0, size_t depth=0) const {
			qp.push_back(this);

			if (pos < entry.size()) {
				typename children_type::const_iterator child = findchild(children, entry[pos]);
				if (child != children.end()) {
					root.nodes[child->second].query(root, entry, pos+1, collected, qp, maxdist, curdist, depth+1);
				}
			}

			if (curdist < maxdist) {
				for (typename children_type::const_iterator child = children.begin() ; child != children.end() ; ++child) {
					if (pos >= entry.size() || child->first != entry[pos]) {
						root.nodes[child->second].query(root, entry, pos, collected, qp, maxdist, curdist+1, depth+1);
						root.nodes[child->second].query(root, entry, pos+1, collected, qp, maxdist, curdist+1, depth+1);
					}
					for (size_t i = 1 ; pos+i < entry.size() ; ++i) {
						if (child->first == entry[pos+i]) {
							root.nodes[child->second].query(root, entry, pos+i+1, collected, qp, maxdist, curdist+i, depth+1);
						}
					}
				}
			}

			if (terminal) {
				size_t dist = curdist;
				if (pos < entry.size()) {
					dist += entry.size() - pos;
				}
				else {
					dist += pos - entry.size();
				}
				if (dist <= maxdist) {
					String out;
					buildString(qp, out);
					typename query_type::iterator ins = collected.insert(std::make_pair(out, dist)).first;
					ins->second = std::min(ins->second, dist);
				}
			}

			qp.pop_back();
		}

	private:
		void updateChildrenDepth(root_type& root, size_t depth=0) {
			children_depth = std::max(children_depth, depth);
			if (parent != std::numeric_limits<size_t>::max()) {
				root.nodes[parent].updateChildrenDepth(root, depth+1);
			}
		}

		bool equals(const node_type* second) const {
			if (self != second->self) {
				return false;
			}
			if (children_depth != second->children_depth) {
				return false;
			}
			if (terminal != second->terminal) {
				return false;
			}
			if (children.size() != second->children.size()) {
				return false;
			}
			for (typename children_type::const_iterator mine = children.begin(), other = second->children.begin()
				; mine != children.end() && other != second->children.end() ; ++mine, ++other) {
				const node_type *nmine = mine->second;
				const node_type *nother = other->second;
				if (!nmine->equals(nother)) {
					return false;
				}
			}
			return true;
		}
	};

	friend class trie_node;

	typedef trie_node node_type;
	typedef std::vector<node_type> node_container_type;
	typedef std::vector<std::pair<typename String::value_type, size_t> > children_type;
	typedef std::vector<const node_type*> query_path_type;

	Serializer serializer;
	bool compressed;
	node_container_type nodes;
	children_type children;

public:
	typedef std::map<String,size_t> query_type;

	trie() : compressed(false) {
	}

	/*
	void serialize(std::ostream& out) const {
		out.write(reinterpret_cast<const char*>(&compressed), sizeof(compressed));

		size_t value = nodes.size();
		out.write(reinterpret_cast<const char*>(&value), sizeof(value));
		for (typename node_container_type::const_iterator node = nodes.begin() ; node != nodes.end() ; ++node) {
			out.write(reinterpret_cast<const char*>(&(*node)->terminal), sizeof((*node)->terminal));
			serializer.serialize(out, (*node)->self);
		}

		for (typename node_container_type::const_iterator node = nodes.begin() ; node != nodes.end() ; ++node) {
			size_t value = (*node)->children.size();
			out.write(reinterpret_cast<const char*>(&value), sizeof(value));
			for (typename children_type::const_iterator child = (*node)->children.begin() ; child != (*node)->children.end() ; ++child) {
				const node_type *node = child->second;
				value = node->number;
				out.write(reinterpret_cast<const char*>(&value), sizeof(value));
			}
		}

		value = children.size();
		out.write(reinterpret_cast<const char*>(&value), sizeof(value));
		for (typename children_type::const_iterator child = children.begin() ; child != children.end() ; ++child) {
			const node_type *node = child->second;
			value = node->number;
			out.write(reinterpret_cast<const char*>(&value), sizeof(value));
		}
	}
	//*/

	bool is_compressed() const {
		return compressed;
	}

	size_t size() const {
		return nodes.size();
	}

	bool add(const String& entry) {
		if (entry.empty()) {
			return false;
		}
		if (compressed) {
			return false;
		}
		typename children_type::iterator child = findchild(children, entry[0]);
		if (child != children.end()) {
			node_type& node = nodes[child->second];
			return node.add(*this, entry, 1, 1);
		}
		size_t z = nodes.size();
		insertchild(children, std::make_pair(entry[0], z));
		nodes.resize(z+1);
		nodes.back() = node_type(std::numeric_limits<size_t>::max(), entry[0]);
		nodes.back().add(*this, entry, 1, 1);
		return true;
	}

	query_type query(const String& entry, size_t maxdist = 0) const {
		query_type matches;
		if (!entry.empty()) {
			query_path_type qp;
			qp.reserve(entry.size()+maxdist+2);

			typename children_type::const_iterator child = findchild(children, entry[0]);
			if (child != children.end()) {
				nodes[child->second].query(*this, entry, 1, matches, qp, maxdist);
			}
			if (maxdist) {
				for (typename children_type::const_iterator child = children.begin() ; child != children.end() ; ++child) {
					if (child->first != entry[0]) {
						nodes[child->second].query(*this, entry, 0, matches, qp, maxdist, 1);
						nodes[child->second].query(*this, entry, 1, matches, qp, maxdist, 1);
					}
					for (size_t i = 1 ; i < entry.size() ; ++i) {
						if (child->first == entry[i]) {
							nodes[child->second].query(*this, entry, i+1, matches, qp, maxdist, i);
						}
					}
				}
			}
		}
		return matches;
	}

	/*
	void compress() {
		if (compressed) {
			return;
		}

		typedef std::multimap<typename String::value_type,node_type*> multichild_type;
		typedef std::map<size_t,multichild_type> depths_type;
		depths_type depths;

		for (typename node_container_type::iterator it = nodes.begin() ; it != nodes.end() ; ++it) {
			node_type *node = *it;
			depths[node->children_depth].insert(std::make_pair(node->self, node));
		}

		std::cerr << "Compressing " << nodes.size() << " nodes..." << std::endl;

		size_t removed = 0;

		for (size_t i=0 ; ; ++i) {
			if (depths.find(i) == depths.end()) {
				break;
			}
			std::cerr << "Handling depth " << i << "..." << std::flush;

			multichild_type& mchild = depths[i];
			bool did_compress = false;

			typename multichild_type::iterator onode;
			for (onode = mchild.begin() ; onode != mchild.end() ; ++onode) {
				typename multichild_type::iterator inode = onode;
				++inode;
				for ( ; inode != mchild.end() ; ) {
					node_type *first = onode->second;
					node_type *second = inode->second;
					if (first->self != second->self) {
						break;
					}
					if (!second->parent || !first->equals(second)) {
						++inode;
						continue;
					}

					findchild(second->parent->children, first->self)->second = first;
					second->parent = 0;
					second->self = typename String::value_type();
					second->children.clear();
					second->children_depth = 0;
					typename multichild_type::iterator die = inode;
					++inode;
					mchild.erase(die);
					did_compress = true;
					++removed;
				}
			}

			if (did_compress) {
				compressed = true;
				std::cerr << "compressed down to " << nodes.size()-removed << " nodes." << std::endl;
			}
			else {
				std::cerr << "nothing more to do." << std::endl;
				break;
			}
		}

		node_container_type tosave;
		for (typename node_container_type::iterator it = nodes.begin() ; it != nodes.end() ; ++it) {
			node_type *node = *it;
			if (node->parent == 0 && node->self == typename String::value_type() && node->children.size() == 0 && node->children_depth == 0) {
				delete node;
			}
			else {
				node->number = tosave.size();
				tosave.push_back(node);
			}
		}
		nodes = tosave;
		std::cerr << std::endl;
	}
	//*/
};

}

#endif
