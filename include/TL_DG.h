/*
 * TL_DG.h
 *
 *  Created on: Sep 20, 2014
 *      Author: ever
 */

#ifndef TL_DG_H_
#define TL_DG_H_

#include <iostream>
#include <sstream>

#include <set>
#include <map>

#if USE_BOOST
#include <boost/shared_ptr.hpp>
#else
#include <memory>
#endif

using namespace std;

namespace tidp {
template<class T>
class TL_DG {
public:
	struct Node {
		set<T> prevs;
		set<T> nexts;
		T node;
		Node(const T& n)
				: node(n) {
		}
		bool operator <(const Node& other) const {
			return node < other.node;
		}
	};
	typedef shared_ptr<Node> NodePtr;
	struct NodePtrCmp {
		bool operator()(const NodePtr& n1, const NodePtr& n2) const {
			return *n1 < *n2;
		}
	};
	struct NodeNoDel {
		void operator()(Node * n) const {
			//no actions
		}
	};

	TL_DG() {
	}
	~TL_DG() {
	}

	NodePtr find(const T& n);
	void addNode(const T& only);
	void addNode(const T& prev, const T & next);
	void rmNode(const T&);

	void getAll(set<T>& nodes);
	void getByDeep(set<T>& nodes, int dep);

	void getNextNodes(const T& n, set<T>& nodes);
	void getPrevNodes(const T& n, set<T>& nodes);

	void getNextAllNodes(const T& n, set<T>& nodes);
	void getPrevAllNodes(const T& n, set<T>& nodes);

	void getPrevNextNodes(const T& n, set<T>&prevs, set<T>&nexts);
	//找出图中入度为0的节点
	void getFirstLevelNodes(set<T>& nodes);
	//找出所有出度为0的节点
	void getLastLevelNodes(set<T>& nodes);

	bool topologySort(vector<set<T> >& vsort);
	void toString();

	static NodeNoDel undel;
private:
	set<NodePtr, NodePtrCmp> _nodes;
};
#define DGCLASS TL_DG<T>

template<class T> typename DGCLASS::NodeNoDel DGCLASS::undel;

template<class T>
typename DGCLASS::NodePtr DGCLASS::find(const T& n) {
	typename DGCLASS::Node node(n);
	typename set<NodePtr,NodePtrCmp>::iterator it = _nodes.find(NodePtr(&node,undel));
	if(it != _nodes.end()) {
		return *it;
	} else {
		return NodePtr();
	}
}
template<class T>
void DGCLASS::addNode(const T& only){
	//NodePtr pt = find(only);
	_nodes.insert(NodePtr(new Node(only)));
}
template<class T>
void DGCLASS::addNode(const T& prev, const T & next) {
	NodePtr pt = find(prev);
	if(pt) {
		pt->nexts.insert(next);
	} else {
		_nodes.insert(NodePtr(new Node(prev)));
		pt = find(prev);
		pt->nexts.insert(next);
	}

	pt = find(next);
	if(pt) {
		pt->prevs.insert(prev);
	} else {
		_nodes.insert(NodePtr(new Node(next)));
		pt = find(next);
		pt->prevs.insert(prev);
	}
}
//删除一个节点 以及其前后的关系链
template<class T>
void DGCLASS::rmNode(const T& n) {
	typename DGCLASS::Node node(n);
	typename set<NodePtr,NodePtrCmp>::iterator it = _nodes.find(NodePtr(&node,undel));
	if(it != _nodes.end()) {
		const T& n = *it->node;
		set<T>& prevs = *it->prevs;
		set<T>& next = *it->next;
		//父节点中去除本子节点关系链
		typename set<T>::iterator it2 = prevs.begin();
		while(it2 != prevs.end()) {
			node.node = *it2;
			it = _nodes.find(NodePtr(&node,undel));
			if(it != _nodes.end()) {
				NodePtr &pt = *it;
				pt->nexts.erase(n);
			}
			++it2;
		}
		//子节点中去除关系链
		it2 = next.begin();
		while(it2 != next.end()) {
			node.node = *it2;
			it = _nodes.find(NodePtr(&node,undel));
			if(it != _nodes.end()) {
				NodePtr &pt = *it;
				pt->prevs.erase(n);
			}
			++it2;
		}
	}
}

template<class T>
void DGCLASS::toString() {
	typename set<NodePtr,NodePtrCmp>::iterator it = _nodes.begin();
	while(it != _nodes.end()) {
		const NodePtr & pt = *it;
		cout << pt->node << " ----" << pt->prevs.size() << "," << pt->nexts.size() << endl;
		typename set<T>::iterator itp = pt->prevs.begin();
		while(itp != pt->prevs.end()) {
			cout << pt->node << " <-- " << *itp << " ";
			++itp;
		}

		typename set<T>::iterator itn = pt->nexts.begin();
		while(itn != pt->nexts.end()) {
			cout << pt->node << " -> " << *itn << " ";
			++itn;
		}
		cout << std::endl;
		++it;
	}
}

template<class T>
void DGCLASS::getAll(set<T>& nodes) {
	nodes.clear();
	typename set<NodePtr,NodePtrCmp>::iterator it = _nodes.begin();
	while(it != _nodes.end()) {
		nodes.insert((*it)->node);
		++it;
	}
}

template<class T>
void DGCLASS::getByDeep(set<T>& nodes,int dep) {
	//维护一份入出度,排序过程中动态更新。

}

template<class T>
void DGCLASS::getPrevNextNodes(const T& n,set<T>&prevs, set<T>&nexts) {
	NodePtr pt = find(n);
	if (pt) {
		prevs = pt->prevs;
		nexts = pt->nexts;
	} else {
		prevs.clear();
		nexts.clear();
	}
}

template<class T>
void DGCLASS::getNextNodes(const T& n,set<T>& nodes) {
	NodePtr pt = find(n);
	if(pt) {
		nodes = pt->nexts;
	} else {
		nodes.clear();
	}
}

template<class T>
void DGCLASS::getPrevNodes(const T& n,set<T>& nodes) {
	NodePtr pt = find(n);
	if(pt) {
		nodes = pt->prevs;
	} else {
		nodes.clear();
	}
}
template<class T>
void DGCLASS::getNextAllNodes(const T& n,set<T>& nodes) {
	NodePtr pt = find(n);
	nodes.clear();
	if(pt) {
		set<T> needchk = pt->nexts;
		typename set<T>::iterator it = needchk.begin();
		while(it != needchk.end()) {
			//防止环
			if(nodes.find(*it) == nodes.end()) {
				pt = find(*it);
				if(pt->nexts.size() > 0) {
					needchk.insert(pt->nexts.begin(),pt->nexts.end());
				}
				nodes.insert(*it);
			}
			needchk.erase(it);
			it = needchk.begin();
		}
	}
}

template<class T>
void DGCLASS::getPrevAllNodes(const T& n,set<T>& nodes) {
	NodePtr pt = find(n);
	nodes.clear();
	if(pt) {
		set<T> needchk = pt->prevs;
		typename set<T>::iterator it = needchk.begin();
		while(it != needchk.end()) {
			//防止环
			if(nodes.find(*it) == nodes.end()) {
				//新的Prev
				pt = find(*it);
				if(pt->nexts.size() > 0) {
					needchk.insert(pt->prevs.begin(),pt->prevs.end());
				}
				nodes.insert(*it);
			}
			needchk.erase(it);
			it = needchk.begin();
		}
	}
}
template<class T>
void findZeroInDegree(const map<T, pair<int, int> > &index, set<T>& nodes) {
	nodes.clear();
	typename map<T, pair<int, int> >::const_iterator it = index.begin();
	while (it != index.end()) {
		if (it->second.first == 0) {
			nodes.insert(it->first);
		}
		++it;
	}
}

template<class T>
void DGCLASS::getFirstLevelNodes(set<T>& nodes) {
	nodes.clear();
	typename set<NodePtr,NodePtrCmp>::iterator it = _nodes.begin();
	while(it != _nodes.end()) {
		const NodePtr& pt = *it;
		if(pt->prevs.size() == 0) {
			nodes.insert(pt->node);
		}
		++it;
	}
}

template<class T>
void DGCLASS::getLastLevelNodes(set<T>& nodes) {
	nodes.clear();
	typename set<NodePtr,NodePtrCmp>::iterator it = _nodes.begin();
	while(it != _nodes.end()) {
		const NodePtr& pt = *it;
		if(pt->nexts.size() == 0) {
			nodes.insert(pt->node);
		}
		++it;
	}
}
//拓扑排序，如果产生来环，（排序无法进行下去) 则返回true
template<class T>
bool DGCLASS::topologySort(vector<set<T> >& vsort) {
	//维护一份入度出度，排序过程中动态更新
	map<T,pair<int,int> > io_degree;
	typename set<NodePtr,NodePtrCmp>::iterator it = _nodes.begin();
	while(it != _nodes.end()) {
		const NodePtr& pt = *it;
		io_degree.insert(make_pair(pt->node,make_pair(pt->prevs.size(),pt->nexts.size())));
		++it;
	}
	//查找入度为0的节点
	vsort.clear();
	set<T> nodes;
	do {
		findZeroInDegree(io_degree,nodes);
		if(nodes.size() > 0) {
			vsort.push_back(nodes);
			typename set<T>::const_iterator it2 = nodes.begin();
			while(it2 != nodes.end()) {
				set<T> nexts;
				getNextNodes(*it2,nexts);
				//io_index
				typename set<T>::iterator it3 = nexts.begin();
				while(it3 != nexts.end()) {
					-- io_degree[*it3].first;
					++it3;
				}
				io_degree.erase(*it2);
				++it2;
			}
			//for()
		} else {
			//无法找到入度为0的节点
			return io_degree.size() > 0;
		}
	}while(nodes.size() > 0);
	return io_degree.size() > 0;
}

#undef DGCLASS

}
 //end namespace
#endif /* TL_DG_H_ */
