#pragma once

#ifndef __BASE_H__
#define __BASE_H__

#include <unordered_map>
#include <unordered_set>

#include <iostream>

#include "Graph.hpp"

namespace Tools {
	class Base {
	private:
		inline static bool __xorInt(const int32_t& a, const int32_t& b) {
			return static_cast<bool>(a) ^ static_cast<bool>(b);
		}

		inline static bool __nandInt(const int32_t& a, const int32_t& b) {
			return !(static_cast<bool>(a) && static_cast<bool>(b));
		}

		// Make a same point set function  
		inline static std::vector<std::string> __samePoint(const std::vector<std::string>& setPointA, const std::vector<std::string>& setPointB) {
			std::vector<std::string> samePoint;

			// get same subset 
			std::set_intersection(setPointA.begin(), setPointA.end(),
				setPointB.begin(), setPointB.end(),
				std::inserter(samePoint, samePoint.end()));
			return samePoint;
		}

		// check the object can merge together
		inline static bool __canMergeTogether(const BaseGraphData& aObj, const BaseGraphData& bObj) {

			std::vector<std::string> samePoint = Base::__samePoint(aObj.pointSet, bObj.pointSet);

			auto& aObjMapping = aObj.mappingGraph;
			auto& bObjMapping = bObj.mappingGraph;

			// find is all can put ? 
			return std::all_of(samePoint.begin(), samePoint.end(),
				// using the NAND to find is all can put the graph 
				[&aObjMapping, &bObjMapping](const std::string& str) {
					return Base::__nandInt(aObjMapping.at(str), bObjMapping.at(str));
				}
			);
		}

	public:
		Base() = default;

		Base(const int32_t& h, const int32_t& w) {
			this->setBaseSize(h, w);
		}
		~Base() = default;

		void setBaseSize(const int32_t& h, const int32_t& w) {
			this->__h = h;
			this->__w = w;


			// init the point set and the mapping graph 
			for (size_t yCnt = 0; yCnt < this->__h; yCnt++) {
				for (size_t xCnt = 0; xCnt < this->__w; xCnt++) {
					auto hashPoint = hashPointFunc({ xCnt , yCnt });

					this->__totalPointSet.push_back(hashPoint);
					this->__TotalMappingGraph.insert({ hashPoint  , 0 });
				}
			}
			std::sort(this->__totalPointSet.begin(), this->__totalPointSet.end());
		}

		// get Base Graph data 
		inline BaseGraphData getBaseGraphData() const {
			return { this->__totalPointSet , this->__TotalMappingGraph };
		}

		inline void addGraph(const Graph::Array2D_INT& mappingGraph) {
			this->__graphArray.push_back({ { mappingGraph,{0,0} } ,false });
		}
		// using AND() to find the graph can put

		inline Graph popGraph(const size_t& index) {
			std::pair<Graph, bool> findItemInGraphArray;
			try {
				findItemInGraphArray = this->__graphArray.at(index);
			}
			catch (const std::exception& e) {
				throw std::invalid_argument("popGraph : index out of range");
			}

			Graph popItem = findItemInGraphArray.first;
			findItemInGraphArray.second = false;

			auto findInMapItr = this->__baseGraph.find(index);

			// not in the base graph
			if (findInMapItr == this->__baseGraph.end()) {
				return popItem;
			}

			this->__baseGraph.erase(findInMapItr);

			this->refreshTotalMappingGraph();

			return popItem;
		}
	private:
		// check the graph have out of range // using graph 
		inline bool __outOfRange(const Graph& graphObj) {
			auto graphItemBaseData = graphObj.getBaseGraphData();
			auto& graphItemPointSet = graphItemBaseData.pointSet;

			std::vector<std::string> resPoint;

			// merge to gather and find there set is it large than last 
			std::set_union(graphItemPointSet.begin(), graphItemPointSet.end(),
				this->__totalPointSet.begin(), this->__totalPointSet.end(),
				std::inserter(resPoint, resPoint.end()));

			return resPoint.size() > this->__totalPointSet.size();
		}

	public:
		// check the graph have out of range // using index 
		inline bool outOfRange(const size_t& index) {
			return this->__outOfRange(this->__graphArray.at(index).first);
		}

		// return is it insert success 
		inline bool insertGraph(const size_t& index) {
			if (index >= this->__graphArray.size()) {
				return false;
			}

			auto& findGraphData = this->__graphArray.at(index);
			auto& findGraph = findGraphData.first;

			if (this->outOfRange(index) ||
				Base::__canMergeTogether(findGraph.getBaseGraphData(), this->getBaseGraphData()) == false) {

				return false;
			}

			this->__baseGraph.insert({ index , findGraph });
			findGraphData.second = true;

			this->refreshTotalMappingGraph();
			return true;
		}

		// using the __baseGraph update the __TotalMappingGraph // using NAND to update 
		void refreshTotalMappingGraph() {
			// clear graph 
			for (size_t yCnt = 0; yCnt < this->__h; yCnt++) {
				for (size_t xCnt = 0; xCnt < this->__w; xCnt++) {
					auto hashPoint = hashPointFunc({ xCnt , yCnt });
					this->__TotalMappingGraph.at(hashPoint) = 0;
				}
			}

			// add graph to the total graph 
			for (const auto& elementGraph : this->__baseGraph) {

				auto graphData = elementGraph.second.getBaseGraphData();

				// get same subset 
				std::vector<std::string> samePoint = Base::__samePoint(graphData.pointSet, this->__totalPointSet);

				// find the same subset point and update the bool using the NAND
				for (const auto& point : samePoint) {
					this->__TotalMappingGraph.at(point) = Base::__xorInt(this->__TotalMappingGraph.at(point), graphData.mappingGraph.at(point));
				}

			}

		}

		// this function for final board 
		inline Graph::Array2D_INT printOut() const {
			Graph::Array2D_INT displayBoard(this->__h, std::vector<int32_t>(this->__w, 0));
			for (const auto& item : this->__TotalMappingGraph) {
				auto point = unHashPointFunc(item.first);
				auto thing = item.second;
				auto& loc = displayBoard[point.second][point.first];

				loc = Base::__xorInt(loc, thing);
			}
			return displayBoard;
		}

		// is a function for change the graph using the command (Moving)
		inline bool changeGraph(const size_t& index, const Moving& command) {
			Graph popItem;
			try { popItem = this->popGraph(index); }
			catch (const std::exception& e) { throw e; }

			auto newGraph = popItem.movingChoose(command);
			if (this->__outOfRange(newGraph) ||
				Base::__canMergeTogether(newGraph.getBaseGraphData(), this->getBaseGraphData()) == false) {

				// put back 
				this->__graphArray[index] = { popItem , true };
				this->__baseGraph.insert({ index , popItem });
				this->refreshTotalMappingGraph();
				return false;
			}

			this->__graphArray[index] = { newGraph , true };
			this->__baseGraph.insert({ index , newGraph });
			this->refreshTotalMappingGraph();
			return true;
		}

	private:
		int32_t __w, __h;

		// first is index , second is graph 
		std::unordered_map<int32_t, Graph> __baseGraph;
		std::vector<std::pair<Graph, bool>> __graphArray;

		// total point set is all point in the set 
		std::vector<std::string> __totalPointSet;

		// what is the thing in the point 
		std::unordered_map<std::string, int32_t> __TotalMappingGraph;
	};
}


#endif //!__BASE_H__
