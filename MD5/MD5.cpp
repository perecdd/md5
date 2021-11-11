#include "MD5.h"
#include <vector>
#include <string>

void Round(std::function<uint32_t(uint32_t, uint32_t, uint32_t)> fun, uint32_t& A, uint32_t& B, uint32_t& C, uint32_t& D, uint32_t X, int i)
{
	A = B + rotate(A + fun(B, C, D) + X + Consts[i], S[i]);
}

std::vector<std::string_view> BreakString(std::string_view line, int count)
{
	std::vector<std::string_view> vector;

	while (!line.empty()) {
		vector.push_back(line.substr(0, count));

		if (count <= line.size()) {
			line.remove_prefix(count);
		}
		else {
			line.remove_prefix(line.size());
		}
	}

	return vector;
}

inline uint32_t rotate(uint32_t data, int value) {
	return (data >> (32 - value)) | (data << value);
}

std::string md5(std::string text)
{
	std::size_t first_size = text.size();
	text = align(std::move(text));
	text = addLength(std::move(text), first_size);

	uint32_t A = 0x67452301;
	uint32_t B = 0xEFCDAB89;
	uint32_t C = 0x98BADCFE;
	uint32_t D = 0x10325476;

	for (std::string_view slice : BreakString(text, 64)) {
		std::array<uint32_t, 16> M;

		for (int i = 0; i < 16; i++) {
			M[i] = MakeInt32({slice[4 * i + 3], slice[4 * i + 2], slice[4 * i + 1], slice[4 * i]});
		}

		uint32_t AA = A;
		uint32_t BB = B;
		uint32_t CC = C;
		uint32_t DD = D;

		for (int i = 0; i < 4; i++) { // Cycle for each phase of md5 work
			for (int j = 16 * i; j < (i + 1) * 16; j++) { // Cycle for each round
				Round(F[i], AA, BB, CC, DD, M[X[j]], j);
				// F - array of functions
				// X - array of values for round

				uint32_t F = CC;
				CC = BB;
				BB = AA;
				AA = DD;
				DD = F;
				// Swaping values to do operatoins in cycle
			}
		}

		A += AA;
		B += BB;
		C += CC;
		D += DD;
	}

	return convertHex(A) + convertHex(B) + convertHex(C) + convertHex(D);
}

/// <summary>
/// Align stream to size multiple of 64 + 56. In tail of the stream 8 symbols are need to storage stream size.
/// </summary>
/// <param name="data">Some string to align</param>
/// <returns>Aligned stream, size % 64 = 56</returns>
std::string align(std::string data)
{
	data.push_back(128);
	int remain = data.size() % 64;
	if (remain > 56) {
		data.resize(data.size() + 64 - remain, 0);
	}
	data.resize(56 - (data.size() % 64) + data.size(), 0);
	return data;
}

uint32_t MakeInt32(std::array<char, 4> data)
{
	uint32_t value = 0;
	for (char c : data) {
		value <<= 8;
		value += unsigned char(c);
	}
	return value;
}

/// <summary>
/// Add length of data stream to end of stream
/// </summary>
/// <param name="data">A string containing 64 * N + 56 characters.</param>
/// <returns>A string containing a multiple of 64 characters, which is prepared for md5 operation</returns>
std::string addLength(std::string data, std::uint64_t size)
{
	std::uint64_t length = size * 8;
	for (int i = 0; i < 8; i++) {
		data.push_back(length);
		length >>= 8;
	}
	return data;
}

inline char intToChar(int value) {
	if (value >= 0 && value <= 9) {
		return '0' + value;
	}
	else {
		return 'a' + value - 10;
	}
}

std::string convertHex(uint32_t value)
{
	std::string str;
	for (int i = 0; i < 4; i++) {
		str.push_back(intToChar((value / 16) % 16));
		str.push_back(intToChar(value % 16));
		value >>= 8;
	}
	return str;
}

inline uint32_t FunF(uint32_t X, uint32_t Y, uint32_t Z)
{
	return (X & Y) | (~X & Z);
}

inline uint32_t FunG(uint32_t X, uint32_t Y, uint32_t Z)
{
	return (X & Z) | (Y & ~Z);
}

inline uint32_t FunH(uint32_t X, uint32_t Y, uint32_t Z)
{
	return X ^ Y ^ Z;
}

inline uint32_t FunI(uint32_t X, uint32_t Y, uint32_t Z)
{
	return Y ^ (~Z | X);
}