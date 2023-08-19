#pragma once
#include "ConsoleInterface.h"
#include <vector>
#include <string>
#include <optional>

namespace KHAS {

	struct NumberOfUnknownVariables final { size_t size;  };
	struct Coord final { size_t col; size_t row; };
	struct DiagonalCoord final { Coord first; Coord second; };

	class Solution final
	{
	private:
		std::string window_name_;
		const std::string filename_;
		ConsoleInterface app_;
		std::vector<std::vector<double>> data_;
		size_t number_of_unknown_variables_;
	public:
		explicit Solution(NumberOfUnknownVariables count, const std::string& filename) noexcept;

		void start() noexcept;

	private:

		bool load(const std::string& filename) noexcept;

		void printDataInTheInterface(std::vector<std::vector<double>>& data, const std::string& iter_header) noexcept;

		bool useMethodJordanGauss(const std::vector<std::vector<double>>& data) noexcept;

		size_t findMaxAbs(const std::vector<std::vector<double>>& data, size_t position) const noexcept;
		void swap(std::vector<std::vector<double>>& data, size_t pos_left, size_t pos_right) const noexcept;
		void transformationData(std::vector<std::vector<double>>& data, size_t diagonal_index) const noexcept;
		void transformationLine(std::vector<std::vector<double>>& data, size_t diagonal_index) const noexcept;
		void resetRow(std::vector<std::vector<double>>& data, size_t diagonal_index) const noexcept;
		double calculateXY(std::vector<std::vector<double>>& data, const DiagonalCoord& main_coords, const DiagonalCoord& secondary_coords) const noexcept;
		std::pair<double, double> getValueFromData(std::vector<std::vector<double>>& data, const DiagonalCoord& di_coord) const noexcept;
	private:
		void init() noexcept;
	};

}