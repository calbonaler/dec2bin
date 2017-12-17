// コンパイル手順
// VC++
//     Visual Studio 開発者コマンドプロンプトより
//     > cl /EHsc DecimalToBinary.cpp
// GCC/G++
//     $ g++ -std=c++11 DecimalToBinary.cpp

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <unordered_map>

// 2 進化された十進有限小数を表す
struct BinarizedDecimal
{
	std::vector<bool> integer; // 整数部 (小さな位が先，大きな位が後)
	std::vector<bool> fraction; // 小数部
	size_t repeatingSectionLength = 0; // 循環節の長さ (位置は fraction.size() - repeatingSectionLength で求められる)

	// 循環節まで表した文字列表現を返す
	std::string toString() const
	{
		std::string sb;
		for (size_t i = integer.size() - 1; i < integer.size(); i--)
			sb.append(integer[i] ? "1" : "0");
		// 小数部が存在する場合
		if (fraction.size() > 0)
		{
			sb.append(".");
			for (size_t i = 0; i < fraction.size(); i++)
			{
				if (repeatingSectionLength != 0 && i + repeatingSectionLength == fraction.size())
					sb.append("("); // 循環節の始まり
				sb.append(fraction[i] ? "1" : "0");
			}
			if (repeatingSectionLength != 0)
				sb.append(")"); // 循環節の終わり
		}
		return std::move(sb);
	}

	// 指定された文字列で始まっているかどうかを調べる
	// (循環節は必要に応じて展開される)
	bool startsWith(const std::string& text) const
	{
		auto it = text.cbegin();
		// 整数部の判定
		for (size_t i = integer.size() - 1; i < integer.size(); i--)
		{
			// 途中で文字列が終了した場合は不一致と判断
			if (it == text.cend() || *it++ != (integer[i] ? '1' : '0'))
				return false;
		}
		// 文字列が終わりならば一致
		if (it == text.cend())
			return true;
		if (*it++ != '.')
			return false;
		// 循環節の手前までの判定
		for (size_t i = 0; i < fraction.size() - repeatingSectionLength; i++)
		{
			// 途中で文字列が終了した場合は一致と判断
			if (it == text.cend())
				return true;
			if (*it++ != (fraction[i] ? '1' : '0'))
				return false;
		}
		if (repeatingSectionLength == 0)
		{
			// 循環節が存在しない (有限小数である) ，または，小数部が存在しない場合，
			// 残りの小数部は無限に続く 0 で埋められているとして判定
			while (true)
			{
				if (it == text.cend())
					return true;
				if (*it++ != '0')
					return false;
			}
		}
		else
		{
			// 循環節が存在する場合，必要な分だけ循環節をコピーし，それとの一致を判定
			while (true)
			{
				for (size_t i = fraction.size() - repeatingSectionLength; i < fraction.size(); i++)
				{
					if (it == text.cend())
						return true;
					if (*it++ != (fraction[i] ? '1' : '0'))
						return false;
				}
			}
		}
	}
};

// 指定された十進有限小数を表す 2 進数を返す
BinarizedDecimal toBinary(const std::string& value)
{
	BinarizedDecimal binDec;
	const auto dotIndex = value.find('.');
	// 整数部 ('.' より前) を処理
	for (auto ip = std::stoull(value.substr(0u, dotIndex)); ;)
	{
		// 整数部に桁を追加
		binDec.integer.push_back(ip % 2 != 0);
		if ((ip /= 2) <= 0) break;
	}
	const auto afterDot = value.substr(dotIndex + 1);
	// '.' がないか '.' 以降の文字列の長さが 0 ならば現在の結果を返す
	if (dotIndex == std::string::npos || afterDot.length() == 0)
		return std::move(binDec);
	// 小数部の桁数 + 1 の桁数をもつ 10 のべき乗を用意
	const auto fpThreshold = static_cast<uint64_t>(pow(10, afterDot.length()));
	// 計算過程から小数部の位置へのマッピングを作成
	std::unordered_map<uint64_t, size_t> fps;
	// 小数部をそのまま整数へ変換 (例: 12.345 -> 345)
	auto fp = std::stoull(afterDot);
	while (true)
	{
		fp *= 2;
		// 現在の計算過程と一致する計算過程をマッピングから調べる
		auto it = fps.find(fp);
		if (it != fps.end())
		{
			// 見つかった場合は循環が発生したと判断し，循環節の長さを設定
			binDec.repeatingSectionLength = binDec.fraction.size() - it->second;
			break;
		}
		// 現在の計算過程と対応する小数部の位置を保存
		fps[fp] = binDec.fraction.size();
		// 小数部に桁を追加
		binDec.fraction.push_back(fp / fpThreshold != 0);
		// 小数部がなくなった場合は終了
		if ((fp %= fpThreshold) <= 0) break;
	}
	return std::move(binDec);
}

int main()
{
	while (true)
	{
		std::string line;
		std::getline(std::cin, line);
		// EOF が入力された場合は終了
		if (line == "") break;
		// 半角スペースで分割
		const auto sepIndex = line.find(' ');
		// 半角スペース以前を 2 進数に変換
		const auto binary = toBinary(line.substr(0u, sepIndex));
		if (sepIndex == std::string::npos)
		{
			// 半角スペースがない場合は 2 進数を表示
			std::cout << binary.toString() << std::endl;
		}
		else
		{
			// 半角スペースがある場合は半角スペース以降との一致を判定
			const auto second = line.substr(sepIndex + 1);
			const auto dix = second.find('.');
			const auto fractionDigits = dix == std::string::npos ? 0 : second.length() - dix - 1;
			if (binary.startsWith(second))
				std::cout << "Matched! ";
			else
				std::cout << "Unmatched... ";
			// 2 番目の文字列の小数部の長さを出力
			std::cout << fractionDigits << std::endl;
		}
	}
}
