#include "Solution.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>

namespace KHAS {
	Solution::Solution(NumberOfUnknownVariables count, const std::string& filename) noexcept
		: window_name_{ "Алгоритмы и вычислительные методы оптимизации" }
		, filename_{ filename }
		, app_{ window_name_ , WinWidth{ .width = 80 }, WinHeight{ .height = 80 } }
		, data_{}
		, number_of_unknown_variables_{ count.size }
	{
		init();
	}

	void Solution::start() noexcept
	{

		if (!load(filename_)) {

			data_.clear();
			app_.clearData();
			app_.addDataItem("Ошибка загрузки данных из файла!");
		}
		if (!useMethodJordanGauss(data_)) {

			data_.clear();
			app_.clearData();
			app_.addDataItem("Ошибка формирования данных!");
		}
		//window.addMenuItem("hi", [] { return std::vector<std::string>{ {"qwerty"} };  }, IsExit::True);
		//window.addMenuItem("fgfgfgfghfghf:", std::nullopt);
		//window.addMenuItem("Выход:", std::nullopt );
		app_.loop();
	}

	bool Solution::load(const std::string& filename) noexcept
	{
		std::ifstream input_file{ filename, std::ios::in };
		if (!input_file.is_open()) return false;


		std::vector<double> tmp_data;
		tmp_data.reserve(number_of_unknown_variables_);

		for (size_t i{};; ++i) {
			double tmp{};
			if (!(input_file >> tmp)) break;

			tmp_data.emplace_back(tmp);

			if ((i + 1) % number_of_unknown_variables_ == 0) {
				data_.emplace_back(tmp_data);
				tmp_data.clear();
			}				
		}
		
		return true;
	}

	void Solution::printDataInTheInterface(std::vector<std::vector<double>>& data, const std::string& iter_header) noexcept
	{

		app_.addDataItem(app_.dividingLine());
		app_.addDataItem(iter_header);
		app_.addDataItem(app_.dividingLine());

		for (auto&& row: data) {
			std::ostringstream ss;
			for (auto&& col: row) {
				ss << std::fixed << std::setprecision(4) << std::setw(11) << col << " ";
			}
			app_.addDataItem(ss.str());
		}
	}

	bool Solution::useMethodJordanGauss(const std::vector<std::vector<double>>& actual_data) noexcept
	{
		std::vector<std::vector<double>> data{ actual_data };

		for (size_t i{}, ie{ data.size()}; i != ie; ++i) {

			auto fn{ i + 1 == ie ? i : findMaxAbs(data, i) };
			swap(data, fn, i);
			printDataInTheInterface(data, "Итерация № "+std::to_string((i+1)));
			transformationData(data, i);
		}

		printDataInTheInterface(data, "Итоговая матрица");
		return true;
	}

	size_t Solution::findMaxAbs(const std::vector<std::vector<double>>& data, size_t position) const noexcept
	{
		size_t max_position{};
		for (size_t i{ position }, ie{ data.size() }; i != ie; ++i) {

			if (i == 0) max_position = i;
			else max_position = std::fabs(data[i][position]) > std::fabs(data[max_position][position])
				? i : max_position;
		}
		return max_position;
	}

	void Solution::swap(std::vector<std::vector<double>>& data, size_t pos_left, size_t pos_right) const noexcept
	{
		assert(pos_left >= 0 && pos_left < data.size());
		assert(pos_right >= 0 && pos_right < data.size());
		if (pos_left == pos_right) return;
		std::swap(data[pos_left], data[pos_right]);
	}

	void Solution::transformationData(std::vector<std::vector<double>>& data, size_t diagonal_index) const noexcept
	{

		transformationLine(data, diagonal_index);
		for (size_t i{}, ie{ data.size() }; i != ie; ++i) {
			if (i == diagonal_index) continue;

			for (size_t j{ diagonal_index + 1 }, je{ data[0].size()}; j != je; ++j) {

				DiagonalCoord main_diagonal{
					Coord{.col = diagonal_index,	.row = diagonal_index },
					Coord{.col = j,					.row = i }
				};
				DiagonalCoord secondary_diagonal{
					Coord{.col = diagonal_index,	.row = i },
					Coord{.col = j,					.row = diagonal_index }
				};
				auto val{ calculateXY(data, main_diagonal, secondary_diagonal) };
				data[main_diagonal.second.row][main_diagonal.second.col] = val;
			}			
		}
		resetRow(data, diagonal_index);

	}

	void Solution::transformationLine(std::vector<std::vector<double>>& data, size_t diagonal_index) const noexcept
	{		
		assert(diagonal_index >= 0 && diagonal_index < data.size());

		auto tmp{ data[diagonal_index][diagonal_index] };

		for (size_t i{ diagonal_index }, ie{ data[diagonal_index].size() }; i != ie; ++i) {
			data[diagonal_index][i] /= tmp;
		}
		
	}

	void Solution::resetRow(std::vector<std::vector<double>>& data, size_t diagonal_index) const noexcept
	{
		assert(diagonal_index >= 0 && diagonal_index < data.size());

		for (size_t i{}, ie{ data.size() }; i != ie; ++i) {
			if (i != diagonal_index) data[i][diagonal_index] = 0;
		}
	}

	double Solution::calculateXY(std::vector<std::vector<double>>& data, const DiagonalCoord& main_coords, const DiagonalCoord& secondary_coords) const noexcept
	{

		auto&& [main_first_value, main_second_value] { getValueFromData(data, main_coords) };
		auto&& [secondary_first_value, secondary_second_value] { getValueFromData(data, secondary_coords) };

		auto pow{ secondary_first_value * secondary_second_value };

		return main_second_value - pow / main_first_value;
	}

	std::pair<double, double> Solution::getValueFromData(std::vector<std::vector<double>>& data, const DiagonalCoord& di_coord) const noexcept
	{
		auto&& [first_col, first_row] { di_coord.first };
		auto&& [second_col, second_row] { di_coord.second };

		return { data[first_row][first_col], data[second_row][second_col] };
	}

	void Solution::init() noexcept
	{
		using namespace std::string_literals;

		app_.addHeaderItem(window_name_);
		app_.addHeaderItem(app_.dividingLine());
		app_.addHeaderItem("Лабораторная работа:"s, "№ 1"s, Aligment::JUSTIFIED);
		app_.addHeaderItem("Группа:"s, "ПБ-11"s, Aligment::JUSTIFIED);
		app_.addHeaderItem("Студент:"s, "Хакимов А.С."s, Aligment::JUSTIFIED);
		auto pos{ filename_ .rfind('/') };
		if (pos == std::string::npos) pos = filename_.rfind('\\');
		if (pos != std::string::npos) 
			app_.addHeaderItem("Данные из файла:"s, "\""+std::string(filename_.begin()+pos+1, filename_.end()) + "\"", Aligment::JUSTIFIED);

	}
}