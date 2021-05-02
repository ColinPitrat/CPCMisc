function TestOneSmallFloatDiv(i, j: global byte, p: global pointer):byte;
begin
	k := Format::Strcpy("Div ", #long_string);
	k += Format::Btos(i, #long_string+k);
	k += Format::Strcpy("/", #long_string+k);
	k += Format::Btos(j, #long_string+k);
	SmallFloat::FromU8(#sf1, i);
	SmallFloat::FromU8(#sf2, j);
	SmallFloat::Divide(#sf, #sf1, #sf2);
	TestOneSmallFloatDiv := CheckFloatIs(#long_string, #sf, p);
end;

function TestOneSmallFloatDivNegFirst(i, j: global byte, p: global pointer):byte;
begin
	k := Format::Strcpy("Div -", #long_string);
	k += Format::Btos(i, #long_string+k);
	k += Format::Strcpy("/", #long_string+k);
	k += Format::Btos(j, #long_string+k);
	SmallFloat::FromU8(#sf1, i);
	SmallFloat::Neg(#sf1);
	SmallFloat::FromU8(#sf2, j);
	SmallFloat::Divide(#sf, #sf1, #sf2);
	TestOneSmallFloatDivNegFirst := CheckFloatIs(#long_string, #sf, p);
end;

function TestOneSmallFloatDivNegSecond(i, j: global byte, p: global pointer):byte;
begin
	k := Format::Strcpy("Div ", #long_string);
	k += Format::Btos(i, #long_string+k);
	k += Format::Strcpy("/-", #long_string+k);
	k += Format::Btos(j, #long_string+k);
	SmallFloat::FromU8(#sf1, i);
	SmallFloat::FromU8(#sf2, j);
	SmallFloat::Neg(#sf2);
	SmallFloat::Divide(#sf, #sf1, #sf2);
	TestOneSmallFloatDivNegSecond := CheckFloatIs(#long_string, #sf, p);
end;


function TestOneSmallFloatDivNegBoth(i, j: global byte, p: global pointer):byte;
begin
	k := Format::Strcpy("Div -", #long_string);
	k += Format::Btos(i, #long_string+k);
	k += Format::Strcpy("/-", #long_string+k);
	k += Format::Btos(j, #long_string+k);
	SmallFloat::FromU8(#sf1, i);
	SmallFloat::Neg(#sf1);
	SmallFloat::FromU8(#sf2, j);
	SmallFloat::Neg(#sf2);
	SmallFloat::Divide(#sf, #sf1, #sf2);
	TestOneSmallFloatDivNegBoth := CheckFloatIs(#long_string, #sf, p);
end;

// Exponents
// 1111111 - .5
// 1111110 - .25
// 1111101 - .125
// 1111100 - .0625
// 1111011 - .03125
// 1111010 - .015625
// 1111001 - .0078125
// For example:
// 00111101 10011000
// .03125 * (1+.125+.0625)
// 00111101 10011001
// .03125 * (1+.125+.0625+.0078125) = .037353515625
// 00111101 01100110
// .015625 * (1+.5+.25+.03125+.015625) = .028076171875
// 00111101 01001100
// .015625 * (1+.5+.0625+.03125) = .02490234375
// 00111101 01000000
// .015625 * (1+.5) = .0234375
	
function TestSmallFloatDiv():byte;
begin
	test_rc := 0;
	// What should be 0/0?
	//test_rc += TestOneSmallFloatDiv(0, 0, "00000000 00000000");
	test_rc += TestOneSmallFloatDiv(0, 1, "00000000 00000000");
	test_rc += TestOneSmallFloatDiv(0, 200, "00000000 00000000");
	test_rc += TestOneSmallFloatDiv(1, 1, "01000000 00000000");
	test_rc += TestOneSmallFloatDiv(7, 2, "01000000 11100000");
	test_rc += TestOneSmallFloatDiv(16, 1, "01000010 00000000");
	test_rc += TestOneSmallFloatDiv(16, 2, "01000001 10000000");
	test_rc += TestOneSmallFloatDiv(16, 4, "01000001 00000000");
	test_rc += TestOneSmallFloatDiv(16, 8, "01000000 10000000");
	test_rc += TestOneSmallFloatDiv(16, 16, "01000000 00000000");
	test_rc += TestOneSmallFloatDiv(16, 32, "00111111 10000000");
	test_rc += TestOneSmallFloatDiv(16, 64, "00111111 00000000");
	test_rc += TestOneSmallFloatDiv(16, 128, "00111110 10000000");
	test_rc += TestOneSmallFloatDiv(15, 1, "01000001 11110000");
	test_rc += TestOneSmallFloatDiv(15, 5, "01000000 11000000");
	test_rc += TestOneSmallFloatDiv(15, 3, "01000001 00100000");
	test_rc += TestOneSmallFloatDiv(15, 4, "01000000 11110000");
	test_rc += TestOneSmallFloatDiv(15, 8, "01000000 01110000");
	test_rc += TestOneSmallFloatDiv(15, 16, "00111111 11110000");
	test_rc += TestOneSmallFloatDiv(20, 2, "01000001 10100000");
	// should be 6.667
	// with  8 bits div: 01000001 01010000: 6.5
	// with 16 bits div: 
	test_rc += TestOneSmallFloatDiv(20, 3, "01000001 01010101");
	test_rc += TestOneSmallFloatDiv(20, 4, "01000001 00100000");
	// should be 1.333
	// with  8 bits div: 01000000 00100000: 1.25
	// with 16 bits div: 
	test_rc += TestOneSmallFloatDiv(20, 15, "01000000 00101010");
	// Test last bit precision
	test_rc += TestOneSmallFloatDiv(254, 1, "01000011 11111110");
	test_rc += TestOneSmallFloatDiv(254, 8, "01000010 01111110");
	test_rc += TestOneSmallFloatDiv(254, 32, "01000001 01111110");
	test_rc += TestOneSmallFloatDiv(255, 1, "01000011 11111111");
	test_rc += TestOneSmallFloatDiv(255, 8, "01000010 01111111");
	test_rc += TestOneSmallFloatDiv(255, 32, "01000001 01111111");
	// should be 0.025
	// with  8 bits div: 00111101 01000000: .0234375
	// with 16 bits div: 00111101 01001100: .02490234375
	test_rc += TestOneSmallFloatDiv(2, 80, "00111101 01001100");
	// should be 0.0375
	// with  8 bits div: 00111101 10011000: .037109375
	// with 16 bits div: 00111101 10011001: .037353515625
	test_rc += TestOneSmallFloatDiv(3, 80, "00111101 10011001");
	
	//test_rc += TestOneSmallFloatDivNegFirst(0, 0, "00000000 00000000");
	test_rc += TestOneSmallFloatDivNegFirst(0, 1, "00000000 00000000");
	test_rc += TestOneSmallFloatDivNegFirst(1, 1, "11000000 00000000");
	test_rc += TestOneSmallFloatDivNegFirst(7, 2, "11000000 11100000");
	//test_rc += TestOneSmallFloatDivNegFirst(2, 80, "11000011 10100000");
	//test_rc += TestOneSmallFloatDivNegFirst(3, 80, "11000011 11110000");
	
	//test_rc += TestOneSmallFloatDivNegSecond(0, 0, "00000000 00000000");
	test_rc += TestOneSmallFloatDivNegSecond(0, 1, "00000000 00000000");
	test_rc += TestOneSmallFloatDivNegSecond(1, 1, "11000000 00000000");
	test_rc += TestOneSmallFloatDivNegSecond(7, 2, "11000000 11100000");
	//test_rc += TestOneSmallFloatDivNegSecond(2, 80, "11000011 10100000");
	//test_rc += TestOneSmallFloatDivNegSecond(3, 80, "11000011 11110000");
	
	//test_rc += TestOneSmallFloatDivNegBoth(0, 0, "00000000 00000000");
	test_rc += TestOneSmallFloatDivNegBoth(0, 1, "00000000 00000000");
	test_rc += TestOneSmallFloatDivNegBoth(1, 1, "01000000 00000000");
	test_rc += TestOneSmallFloatDivNegBoth(7, 2, "01000000 11100000");
	//test_rc += TestOneSmallFloatDivNegBoth(2, 80, "01000011 10100000");
	//test_rc += TestOneSmallFloatDivNegBoth(3, 80, "01000011 11110000");
	TestSmallFloatDiv := test_rc;
end;
