#include "pch.h"
#include "../MD5/MD5.cpp"

TEST(MD5, rotateTest) {
	{
		uint32_t value = 0;
		uint32_t Test = ~value;
		value = ~value;

		for (int i = 0; i < 64; i++) {
			value = rotate(value, i);
			EXPECT_EQ(Test, value);
		}
	}

	{
		uint32_t value = 0;
		uint32_t Test = 0;

		for (int i = 0; i < 64; i++) {
			value = rotate(value, i);
			EXPECT_EQ(Test, value);
		}
	}

	{
		for (int i = 0; i < 10; i++) {
			uint32_t value = rand() % 100'000;
			uint32_t Test = value;

			value = rotate(value, 32);
			EXPECT_EQ(Test, value);
		}
	}

	{
		uint32_t value = 23523523535;
		uint32_t Test = 23523523535;

		for (int i = 0; i < 32; i++) {
			value = rotate(value, 1);
		}
		EXPECT_EQ(Test, value);
	}
}

TEST(Utility, MakeInt32) {
	EXPECT_EQ(0, MakeInt32({0,0,0,0}));
	EXPECT_EQ(255, MakeInt32({ 0,0,0,char(255) }));
	EXPECT_EQ(256, MakeInt32({ 0,0,1,0 }));
	EXPECT_EQ(2565, MakeInt32({ 0,0,10,5 }));
	EXPECT_EQ(6579300, MakeInt32({ 0,100,100,100 }));
	EXPECT_EQ(1684300900, MakeInt32({ 100,100,100,100 }));
}

TEST(Utility, BreakString) {
	{
		std::string empty = "";
		EXPECT_TRUE(BreakString(empty, 1).empty());
	}

	{
		std::string anything = "SomeTestLineWithWord";
		auto vec = BreakString(anything, 4);
		EXPECT_EQ(vec[0], "Some");
		EXPECT_EQ(vec[1], "Test");
		EXPECT_EQ(vec[2], "Line");
		EXPECT_EQ(vec[3], "With");
		EXPECT_EQ(vec[4], "Word");
		EXPECT_EQ(vec.size(), 5);
	}

	{
		std::string anything = "SomeTestLineWithWordww";
		auto vec = BreakString(anything, 4);
		EXPECT_EQ(vec[0], "Some");
		EXPECT_EQ(vec[1], "Test");
		EXPECT_EQ(vec[2], "Line");
		EXPECT_EQ(vec[3], "With");
		EXPECT_EQ(vec[4], "Word");
		EXPECT_EQ(vec[5], "ww");
		EXPECT_EQ(vec.size(), 6);
	}

	{
		std::string anything = "SomeTestLineWithWordww";
		auto vec = BreakString(anything, 100000);
		EXPECT_EQ(vec[0], "SomeTestLineWithWordww");
		EXPECT_EQ(vec.size(), 1);
	}
}

TEST(MD5, align) {
	{
		std::string empty;
		empty = align(empty);
		EXPECT_EQ(empty.size(), 56);
		EXPECT_EQ(uint8_t(empty[0]), 128);
		for (int i = 1; i < empty.size(); i++) {
			EXPECT_EQ(empty[i], 0);
		}
	}

	{
		std::string big(56, 'a');
		big = align(big);
		EXPECT_EQ(big.size(), 120);
		EXPECT_EQ(uint8_t(big[56]), 128);
		for (int i = 57; i < big.size(); i++) {
			EXPECT_EQ(uint8_t(big[i]), 0);
		}
	}

	{
		std::string big(184, 'a');
		big = align(big);
		EXPECT_EQ(big.size(), 248);
		EXPECT_EQ(uint8_t(big[184]), 128);
		for (int i = 185; i < big.size(); i++) {
			EXPECT_EQ(uint8_t(big[i]), 0);
		}
	}
}

TEST(MD5, AddLength) {
	{
		std::string empty;
		empty = addLength(empty, empty.size());
		EXPECT_EQ(empty.size(), 8);
		for (std::size_t i = 0; i < empty.size(); i++) {
			EXPECT_EQ(empty[i], 0);
		}
	}

	{
		std::string empty(1, 'a');
		empty = addLength(empty, empty.size());
		EXPECT_EQ(empty.size(), 9);
		EXPECT_EQ(uint8_t(empty[1]), 8);
		for (std::size_t i = 2; i < empty.size(); i++) {
			EXPECT_EQ(empty[i], 0);
		}
	}

	{
		std::string empty(255, 'a');
		empty = addLength(empty, empty.size());
		EXPECT_EQ(empty.size(), 263);
		EXPECT_EQ(uint8_t(empty[255]), 248);
		EXPECT_EQ(uint8_t(empty[256]), 7);
		for (std::size_t i = 257; i < empty.size(); i++) {
			EXPECT_EQ(empty[i], 0);
		}
	}
}

TEST(Utility, ConvertHex) {
	EXPECT_EQ("00000000", convertHex(0));
	EXPECT_EQ("0f000000", convertHex(15));
	EXPECT_EQ("00000100", convertHex(65536));
	EXPECT_EQ("00001000", convertHex(1048576));
	EXPECT_EQ("00000001", convertHex(16777216));
	EXPECT_EQ("00000010", convertHex(268435456));
	EXPECT_EQ("000000f0", convertHex(4026531840));
	EXPECT_EQ("000000ff", convertHex(4278190080));
	EXPECT_EQ("ffffffff", convertHex(4294967295));
	EXPECT_EQ("ffffffff", convertHex(4294967295));
	EXPECT_EQ("89abcdef", convertHex(4023233417));
	EXPECT_EQ("01234567", convertHex(1732584193));
}

TEST(MD5, AlghoritmTest) {
	ASSERT_EQ(md5("Test"), "0cbc6611f5540bd0809a388dc95a615b");
	ASSERT_EQ(md5(""), "d41d8cd98f00b204e9800998ecf8427e");
	ASSERT_EQ(md5("md4"), "c93d3bf7a7c4afe94b64e30c2ce39f4f");
	ASSERT_EQ(md5("The quick brown fox jumps over the lazy dog"), "9e107d9d372bb6826bd81d3542a419d6");
	ASSERT_EQ(md5("The quick brown fox jumps over the lazy dog."), "e4d909c290d0fb1ca068ffaddf22cbd0");
}