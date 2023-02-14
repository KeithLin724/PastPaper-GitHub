#ifndef __POLYSEQ_HPP__
#define __POLYSEQ_HPP__

#include <unordered_map>
#include <unordered_set>
#include <tgmath.h>

class PolySeq {
private:
	using __BaseType = std::unordered_map<int32_t, int32_t>;
	using __PowerSet = std::unordered_set<int32_t>;

	PolySeq(const __BaseType& __copy)
		: __poly(__copy) {
		for (const auto& item : this->__poly) {
			this->__powerSet.insert(item.first);
		}
	}

private:

	inline static __BaseType __add(const __BaseType& obj1 , const __BaseType& obj2) {
		__BaseType res;
		__PowerSet tmpSavePowerSet;

		for (const auto& item : obj1) {
			auto& powerTemp = item.first;
			try {
				res.insert({ powerTemp , obj2.at(powerTemp) + item.second });
			}
			catch (const std::exception&) {
				res.insert(item);
			}
			tmpSavePowerSet.insert(powerTemp);
		}

		for (const auto& item : obj2) {
			if (tmpSavePowerSet.count(item.first)) {
				continue;
			}
			res.insert(item);
		}

		return res;
	}

	inline static std::pair<int32_t, int32_t> __multiplyItem(const std::pair<int32_t, int32_t>& node1 , const std::pair<int32_t, int32_t>& node2) {
		return { node1.first + node2.first , node1.second * node2.second };
	}

	inline static int32_t __getValueItem(const std::pair<int32_t,int32_t>& item , const int32_t& number) {
		return item.second * std::pow(number, item.first);
	}

	inline static std::pair<int32_t, int32_t> __integralItem(const std::pair<int32_t, int32_t>& item) {
		return { item.first + 1 , item.second / (item.first + 1) };
	}

public:
	PolySeq() = default;
	~PolySeq() = default;
	
	PolySeq(const PolySeq&) = default;
	PolySeq& operator=(const PolySeq&) = default;

	PolySeq(int32_t* coeff, int32_t* power, const int32_t number) {
		for (size_t i = 0; i < number; i++) {
			this->__poly.insert({ power[i],coeff[i] });
			this->__powerSet.insert(power[i]);
		}
	}

	inline PolySeq Add(const PolySeq& obj) const {
		return { PolySeq::__add(this->__poly , obj.__poly) };
	}

	inline PolySeq Multiply(const PolySeq& obj) const {
		__BaseType res;

		for (const auto& item : obj.__poly) {
			for (const auto& itemAtThis : this->__poly) {
				auto temp = PolySeq::__multiplyItem(itemAtThis, item);
				
				try {
					res.at(temp.first) += temp.second;
				}
				catch (const std::exception&) {
					res.insert(temp);
				}

			}
		}

		return { res };
	}

	inline PolySeq Derivative() const {
		__BaseType res;

		for (const auto& item : this->__poly) {
			// power is zero to skip
			if (item.first == 0 ) {
				continue;
			}

			res.insert({ item.first - 1  , item.second * item.first });
		}

		return { res };
	}

	inline int32_t Integral(const int32_t& start , const int32_t& end) const {
		int32_t endRes = 0, startRes = 0; 

		for (const auto& item : this->__poly) {
			auto integralItem = PolySeq::__integralItem(item);
			endRes += PolySeq::__getValueItem(integralItem, end);
			startRes += PolySeq::__getValueItem(integralItem, start);
		}

		return endRes - startRes; 
	}

	inline int32_t getValue(const int32_t& number) const {
		int32_t res = 0; 

		for (const auto& item : this->__poly) {
			res += PolySeq::__getValueItem(item, number);
		}

		return res; 
	}

private:
	__BaseType __poly;
	__PowerSet __powerSet; 
};


#endif // !__POLYSEQ_HPP__
