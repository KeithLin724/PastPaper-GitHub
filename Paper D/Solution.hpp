#pragma once

#ifndef __SOLUTION_H__
#define __SOLUTION_H__

#include <fstream>
#include <sstream>
#include <iostream>

#include "Base.hpp"

namespace Tools {
	class Solution {
	private:
		inline static std::vector<int32_t> __splitCommandByNumber(const std::string& cmdStr) {
			std::stringstream  splitCmd(cmdStr);
			std::vector<int32_t> cmd;
			int32_t tmpNumber;

			while (splitCmd >> tmpNumber) {
				cmd.push_back(tmpNumber);
			}
			return cmd;
		}

	public:
		Solution() = default;
		~Solution() = default;

		void readFile(const std::string& fileName) {
			std::ifstream inFile;
			inFile.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit); 
			std::stringstream ssInFile;
			
			try {
				inFile.open(fileName); 
				ssInFile << inFile.rdbuf();
				inFile.close(); 
			}
			catch (const std::exception& e) {
				inFile.close(); 
				throw std::invalid_argument("cant open file"); 
			}

			int32_t h, w , numberOfPicture;

			//read base size 
			ssInFile >> h >> w;

			this->__baseGraph.setBaseSize(h, w);

			// read picture for each 
			ssInFile >> numberOfPicture;
			for (int32_t i = 0; i < numberOfPicture; i++) {
				int32_t picH, picW; 
				ssInFile >> picH >> picW; 

				// read picture by signal bits
				Graph::Array2D_INT graphArr; 
				for (int32_t y = 0; y < picH; y++) {
					graphArr.push_back({});
					auto& tempLineCarry = graphArr.back();

					int32_t picBit;
					for (int32_t x = 0; x < picW; x++) {
						ssInFile >> picBit;
						tempLineCarry.push_back(picBit);
					}
				}
				this->__totalGraphArray.push_back(graphArr);
			}

			std::string lineCmd; 

			while (std::getline(ssInFile, lineCmd , '\n')) {
				if (lineCmd.empty()) {
					continue;
				}

				auto cmdRes = Solution::__splitCommandByNumber(lineCmd);
				this->__commandArray.push_back(cmdRes);
			}
		}

		void runSolution(const std::string& outFileName) {
			std::stringstream ssOut; 

			// load graph 
			for (const auto& item : this->__totalGraphArray) {
				this->__baseGraph.addGraph(item);
			}

			// run solution 
			for (const auto& cmdLine : this->__commandArray) {
				if (cmdLine.size() == 1 && cmdLine.front() == 0) {
					break; 
				}

				int32_t cmdInt = cmdLine[0], graphIndex = cmdLine[1] - 1;
				bool success = false;
				if (cmdInt == 1) {
					success = this->__baseGraph.insertGraph(graphIndex);
				}
				else {
					success = this->__baseGraph.changeGraph(graphIndex, static_cast<Moving>(cmdInt));
				}

				if (success == false) {
					ssOut << "Error\n";
				}
			}
			
			auto printOutRes = this->__baseGraph.printOut();

			for (const auto& line : printOutRes) {
				for (const auto& elm : line) {
					ssOut << elm << " ";
				}
				ssOut << '\n';
			}

			std::ofstream outFile(outFileName);
			outFile << ssOut.rdbuf();
			outFile.close();
		}

	private:
		Base __baseGraph; 
		std::vector<Graph::Array2D_INT> __totalGraphArray; 
		std::vector<std::vector<int32_t>> __commandArray;

	} solutionTools;
}


#endif //!__SOLUTION_H__
