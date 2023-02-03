#pragma once
#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <vector>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <functional>

namespace Tools {

	struct BaseGraphData {
		std::vector<std::string> pointSet;
		std::unordered_map<std::string, int32_t> mappingGraph;
	};

	inline static std::string hashPointFunc(const std::pair<int32_t, int32_t>& point) {
		return std::to_string(point.first) + " " + std::to_string(point.second);
	}

	inline static std::pair<int32_t, int32_t> unHashPointFunc(const std::string& hashPoint) {
		std::stringstream ss(hashPoint);
		std::pair<int32_t, int32_t> res;

		ss >> res.first >> res.second;

		return res;
	}

	enum class Moving: int32_t {
		UP = 2, DOWN = 3, LEFT = 4, RIGHT = 5,
		CLOCK_WISE_90 = 6, COUNTER_CLOCK_WISE_90 = 7,
		VERTICALLY = 8, HORIZONTALLY = 9
	};

	class Graph {
	public:
		using Array2D_INT = std::vector<std::vector<int32_t>>;
	private:

		inline static bool __same_size(const Array2D_INT& matrixA, const Array2D_INT& matrixB) {
			return (matrixA.size() != matrixB.size() ? false : matrixA.back().size() == matrixB.back().size());
		}

	public:
		Graph() = default;
		~Graph() = default;

		inline Graph(const Graph&) = default;
		inline Graph& operator=(const Graph&) = default;

		// return the size using the tuple // weight and height 
		inline std::tuple<int32_t, int32_t> getSize() const {
			return Graph::__getSize(this->__graph);
		}

		inline Array2D_INT getGraph() const {
			return this->__graph;
		}

		inline std::string displayStr() const {
			std::string res;
			for (const auto& line : this->__graph) {
				for (const auto& item : line) {
					res += std::to_string(item) + " ";
				}
				res.back() = '\n';
			}

			return res;
		}

		Graph(const std::vector<std::vector<int32_t>>& mappingGraph, const std::pair<int32_t, int32_t>& point) {
			this->__graph = mappingGraph;
			this->__x = point.first;
			this->__y = point.second;

			// make the mapping graph // using unordered set 
			size_t graphHeight, graphWeight;
			std::tie(graphHeight, graphWeight) = Graph::__getSize(mappingGraph);

			for (size_t yCnt = 0; yCnt < graphHeight; yCnt++) {
				for (size_t xCnt = 0; xCnt < graphWeight; xCnt++) {
					// zero to skip 
					if (this->__graph[yCnt][xCnt] == 0) {
						continue;
					}
					auto hashPoint = hashPointFunc({ xCnt + this->__x, yCnt + this->__y });

					this->__setOfPoint.push_back(hashPoint);
					this->__mappingOfGraph.insert({ hashPoint  , this->__graph[yCnt][xCnt] });
				}
			}
			std::sort(this->__setOfPoint.begin(), this->__setOfPoint.end());
		}

		// this function is using for change the graph post and return a new obj
		inline Graph movingChoose(const Moving& movingEnum) const {
			switch (movingEnum) {
			case Moving::UP:    return Graph(this->__graph, { this->__x , this->__y - 1 });
			case Moving::DOWN:  return Graph(this->__graph, { this->__x , this->__y + 1 });
			case Moving::LEFT:  return Graph(this->__graph, { this->__x - 1 , this->__y });
			case Moving::RIGHT: return Graph(this->__graph, { this->__x + 1 , this->__y });

			case Moving::VERTICALLY:   return Graph(Graph::__vertically(this->__graph), { this->__x , this->__y });
			case Moving::HORIZONTALLY: return Graph(Graph::__horizontal(this->__graph), { this->__x , this->__y });

			case Moving::CLOCK_WISE_90:         return Graph(Graph::__clock_wise(this->__graph), { this->__x , this->__y });
			case Moving::COUNTER_CLOCK_WISE_90: return Graph(Graph::__counter_clock_wise(this->__graph), { this->__x , this->__y });

			default: return Graph();
			}
		}

		inline BaseGraphData getBaseGraphData() const {
			return { this->__setOfPoint , this->__mappingOfGraph };
		}

	private:
		inline static std::tuple<int32_t, int32_t> __getSize(const Array2D_INT& matrix) {
			return std::make_tuple(matrix.size(), matrix.back().size());
		}

		// return the graph 
		inline static Array2D_INT __vertically(const Array2D_INT& matrix) {
			Array2D_INT copyGraph;
			std::reverse_copy(matrix.begin(), matrix.end(), std::back_inserter(copyGraph));
			return copyGraph;
		}

		// return the graph 
		inline static Array2D_INT __horizontal(const Array2D_INT& matrix) {
			Array2D_INT copyGraph = matrix;

			for (auto& line : copyGraph) {
				std::reverse(line.begin(), line.end());
			}

			return copyGraph;
		}

		// this function is using for transpose the matrix 
		inline static Array2D_INT __clock_wise(const Array2D_INT& matrix) {
			Array2D_INT res;

			size_t oldHeight, oldWeight;
			std::tie(oldHeight, oldWeight) = Graph::__getSize(matrix);

			// TODO: 
			for (int64_t i = 0; i < oldWeight; i++) {
				res.push_back({});
				auto& tempBackLine = res.back();

				for (int64_t j = oldHeight - 1; j >= 0; j--) {
					tempBackLine.push_back(matrix[j][i]);
				}
			}

			return res;
		}

		// this function is using for transpose the matrix 
		inline static Array2D_INT __counter_clock_wise(const Array2D_INT& matrix) {
			Array2D_INT res;

			size_t oldHeight, oldWeight;
			std::tie(oldHeight, oldWeight) = Graph::__getSize(matrix);

			// TODO: 
			for (int64_t i = oldWeight - 1; i >= 0; i--) {
				res.push_back({});
				auto& tempBackLine = res.back();

				for (int64_t j = 0; j < oldHeight; j++) {
					tempBackLine.push_back(matrix[j][i]);
				}
			}

			return res;
		}

	private:
		int32_t __x, __y;
		Array2D_INT __graph;
		std::vector<std::string> __setOfPoint; // point x, y
		std::unordered_map<std::string, int32_t> __mappingOfGraph; // point with the graph 
	};
}

#endif //!__GRAPH_H__

