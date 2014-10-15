/*
 * TLRelationMgr.h
 *
 *  Created on: Nov 12, 2013
 *      Author: ever
 */

#ifndef TLRELATIONMGR_H_
#define TLRELATIONMGR_H_
#include <map>
#include <set>
namespace tidp {
template<class NodeT = unsigned int>
class TL_RelationMgr {
public:
	typedef std::set<NodeT> node_set_t;
	typedef std::map<NodeT, node_set_t> relation_map_t;

	virtual ~TL_RelationMgr() {
	}
	void setNodesMap(const relation_map_t & rmap) {
		_relationMap = rmap;
	}
	void getNextLevel1Nodes(const NodeT& node, node_set_t& rset) {
		rset.clear();
		typename relation_map_t::const_iterator it = _relationMap.find(node);
		if (it != _relationMap.end()) {
			rset = it->second;
		}
	}
	void getNextAllNodes(const NodeT& node, node_set_t& rset) {
		rset.clear();
		getNextAllNodesRecursion(node, rset);
		rset.erase(node);
	}
	void getParentNodes(const NodeT& node, node_set_t& rset) {
		rset.clear();
		typename relation_map_t::const_iterator it = _relationMap.begin();
		while (it != _relationMap.end()) {
			if (it->second.find(node) != it->second.end()) {
				rset.insert(it->first);
			}
			++it;
		}
	}
private:
	void getNextAllNodesRecursion(const NodeT& node, node_set_t& rset) {
		//rset.clear();
		node_set_t rset2;
		getNextLevel1Nodes(node, rset2);
		/*typename relation_map_t::const_iterator it = _relationMap.find(node);
		if (it != _relationMap.end()) {
			rset2 = it->second;
		}*/
		rset.insert(node);
		typename node_set_t::const_iterator it2 = rset2.begin();
		while (it2 != rset2.end()) {
			if (rset.find(*it2) == rset.end()) {
				getNextAllNodesRecursion(*it2, rset);
			}
			++it2;
		}
	}
private:
	relation_map_t _relationMap;
};

} /* namespace tidp */

#endif /* TLRELATIONMGR_H_ */
