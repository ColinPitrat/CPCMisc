function TestOneSmallFloatCompareByte(i, j, pass_byte_expected, negfirst, negsecond: global byte):byte;
begin
	SmallFloat::FromU16(#sf1, i);
	SmallFloat::FromU16(#sf2, j);
	k := Format::Strcpy("Cmp ", #long_string);
	if negfirst <> 0 then
	begin
		SmallFloat::Neg(#sf1);
		k += Format::Strcpy("-", #long_string+k);
	end;
	k += Format::Btos(i, #long_string+k);
	k += Format::Strcpy("?", #long_string+k);
	if negsecond <> 0 then
	begin
		SmallFloat::Neg(#sf2);
		k += Format::Strcpy("-", #long_string+k);
	end;
	k += Format::Btos(j, #long_string+k);
	pass_byte_value := SmallFloat::Compare(#sf1, #sf2);
	TestOneSmallFloatCompareByte := CheckByteIs(#long_string, pass_byte_value, pass_byte_expected);
end;

function TestSmallFloatCompareByte():byte;
begin
	test_rc := 0;
	// Compare two positive values
	test_rc += TestOneSmallFloatCompareByte(  0,   0,  0, 0, 0);
	test_rc += TestOneSmallFloatCompareByte(  1,   0,  1, 0, 0);
	test_rc += TestOneSmallFloatCompareByte(  1,   1,  0, 0, 0);
	test_rc += TestOneSmallFloatCompareByte(  1,   2, -1, 0, 0);
	test_rc += TestOneSmallFloatCompareByte(  1,   5, -1, 0, 0);
	test_rc += TestOneSmallFloatCompareByte(  5,   5,  0, 0, 0);
	test_rc += TestOneSmallFloatCompareByte( 12,   5,  1, 0, 0);
	test_rc += TestOneSmallFloatCompareByte(255,   1,  1, 0, 0);
	test_rc += TestOneSmallFloatCompareByte(  1, 255, -1, 0, 0);

	// Compare negative with positive value. Always smaller except for 0.
	test_rc += TestOneSmallFloatCompareByte(  0,   0,  0, 1, 0);
	test_rc += TestOneSmallFloatCompareByte(  1,   0, -1, 1, 0);
	test_rc += TestOneSmallFloatCompareByte(  1,   1, -1, 1, 0);
	test_rc += TestOneSmallFloatCompareByte(  1,   2, -1, 1, 0);
	test_rc += TestOneSmallFloatCompareByte(  1,   5, -1, 1, 0);
	test_rc += TestOneSmallFloatCompareByte(  5,   5, -1, 1, 0);
	test_rc += TestOneSmallFloatCompareByte( 12,   5, -1, 1, 0);
	test_rc += TestOneSmallFloatCompareByte(255,   1, -1, 1, 0);
	test_rc += TestOneSmallFloatCompareByte(  1, 255, -1, 1, 0);
	
	// Compare positive with negative value. Always greater except for 0.
	test_rc += TestOneSmallFloatCompareByte(  0,   0,  0, 0, 1);
	test_rc += TestOneSmallFloatCompareByte(  1,   0,  1, 0, 1);
	test_rc += TestOneSmallFloatCompareByte(  1,   1,  1, 0, 1);
	test_rc += TestOneSmallFloatCompareByte(  1,   2,  1, 0, 1);
	test_rc += TestOneSmallFloatCompareByte(  1,   5,  1, 0, 1);
	test_rc += TestOneSmallFloatCompareByte(  5,   5,  1, 0, 1);
	test_rc += TestOneSmallFloatCompareByte( 12,   5,  1, 0, 1);
	test_rc += TestOneSmallFloatCompareByte(255,   1,  1, 0, 1);
	test_rc += TestOneSmallFloatCompareByte(  1, 255,  1, 0, 1);
	
	// Compare two negative values
	test_rc += TestOneSmallFloatCompareByte(  0,   0,  0, 1, 1);
	test_rc += TestOneSmallFloatCompareByte(  1,   0, -1, 1, 1);
	test_rc += TestOneSmallFloatCompareByte(  1,   1,  0, 1, 1);
	test_rc += TestOneSmallFloatCompareByte(  1,   2,  1, 1, 1);
	test_rc += TestOneSmallFloatCompareByte(  1,   5,  1, 1, 1);
	test_rc += TestOneSmallFloatCompareByte(  5,   5,  0, 1, 1);
	test_rc += TestOneSmallFloatCompareByte( 12,   5, -1, 1, 1);
	test_rc += TestOneSmallFloatCompareByte(255,   1, -1, 1, 1);
	test_rc += TestOneSmallFloatCompareByte(  1, 255,  1, 1, 1);
	TestSmallFloatCompareByte := test_rc;
end;

function TestOneSmallFloatCompareFloat(pf1, pf2: global pointer, j: global byte):byte;
begin
	k := SmallFloat::Compare(pf1, pf2);
	TestOneSmallFloatCompareFloat := CheckByteIs("Compare Float", k, j);
end;

function TestSmallFloatCompareFloat():byte;
begin
	test_rc := 0;
	sf1[0] := 2;
	sf1[1] := 63;
	sf2[0] := 0;
	sf2[1] := 65;
	test_rc += TestOneSmallFloatCompareFloat(#sf1, #sf2, -1);
	TestSmallFloatCompareFloat := test_rc;
end;

function TestSmallFloatCompare():byte;
begin
	test_rc2 := TestSmallFloatCompareByte();
	Format::Btos(test_rc2, #test_result);
    test_rc2 += TestSmallFloatCompareFloat();
    TestSmallFloatCompare := test_rc2;
end;
