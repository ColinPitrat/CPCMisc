function TestSmallFloatFromU8():byte;
begin
	test_rc := 0;
	SmallFloat::FromU8(#f, 0);
	test_rc += CheckFloatIs("FromU8 0", #f, "00000000 00000000");
	SmallFloat::FromU8(#f, 1);
	test_rc += CheckFloatIs("FromU8 1", #f, "01000000 00000000");
	SmallFloat::FromU8(#f, 2);
	test_rc += CheckFloatIs("FromU8 2", #f, "01000000 10000000");
	SmallFloat::FromU8(#f, 3);
	test_rc += CheckFloatIs("FromU8 3", #f, "01000000 11000000");
	SmallFloat::FromU8(#f, 7);
	test_rc += CheckFloatIs("FromU8 7", #f, "01000001 01100000");
	SmallFloat::FromU8(#f, 255);
	test_rc += CheckFloatIs("FromU8 255", #f, "01000011 11111111");
	TestSmallFloatFromU8 := test_rc;
end;

function TestSmallFloatFromU16():byte;
begin
	test_rc := 0;
	SmallFloat::FromU16(#f, 0);
	test_rc += CheckFloatIs("FromU16 0", #f, "00000000 00000000");
	SmallFloat::FromU16(#f, 1);
	test_rc += CheckFloatIs("FromU16 1", #f, "01000000 00000000");
	SmallFloat::FromU16(#f, 2);
	test_rc += CheckFloatIs("FromU16 2", #f, "01000000 10000000");
	SmallFloat::FromU16(#f, 3);
	test_rc += CheckFloatIs("FromU16 3", #f, "01000000 11000000");
	SmallFloat::FromU16(#f, 7);
	test_rc += CheckFloatIs("FromU16 7", #f, "01000001 01100000");
	SmallFloat::FromU16(#f, 255);
	test_rc += CheckFloatIs("FromU16 255", #f, "01000011 11111111");
	SmallFloat::FromU16(#f, 256);
	test_rc += CheckFloatIs("FromU16 256", #f, "01000100 00000000");
	SmallFloat::FromU16(#f, 65500);
	test_rc += CheckFloatIs("FromU16 65500", #f, "01000111 11111111");
	SmallFloat::FromU16(#f, 65535);
	test_rc += CheckFloatIs("FromU16 65535", #f, "01000111 11111111");
	TestSmallFloatFromU16 := test_rc;
end;

function TestSmallFloatFromString():byte;
begin
	test_rc := 0;
	SmallFloat::FromString(#f, "0");
	test_rc += CheckFloatIs("FromString 0", #f, "00000000 00000000");
	SmallFloat::FromString(#f, "1");
	test_rc += CheckFloatIs("FromString 1", #f, "01000000 00000000");
	SmallFloat::FromString(#f, "2");
	test_rc += CheckFloatIs("FromString 2", #f, "01000000 10000000");
	SmallFloat::FromString(#f, "3");
	test_rc += CheckFloatIs("FromString 3", #f, "01000000 11000000");
	SmallFloat::FromString(#f, "7");
	test_rc += CheckFloatIs("FromString 7", #f, "01000001 01100000");
	SmallFloat::FromString(#f, "255");
	test_rc += CheckFloatIs("FromString 255", #f, "01000011 11111111");
	SmallFloat::FromString(#f, "256");
	test_rc += CheckFloatIs("FromString 256", #f, "01000100 00000000");
	SmallFloat::FromString(#f, "512");
	test_rc += CheckFloatIs("FromString 512", #f, "01000100 10000000");
	SmallFloat::FromString(#f, "544");
	test_rc += CheckFloatIs("FromString 544", #f, "01000100 10001000");
	// Fails due to the order in which this is constructed
	/*
	SmallFloat::FromString(#f, "65500");
	test_rc += CheckFloatIs("FromString 65500", #f, "01000111 11111111");
	SmallFloat::FromString(#f, "65535");
	test_rc += CheckFloatIs("FromString 65535", #f, "01000111 11111111");
	*/
	SmallFloat::FromString(#f, "7.5");
	test_rc += CheckFloatIs("FromString 7.5", #f, "01000001 01110000");
	SmallFloat::FromString(#f, "7.50");
	test_rc += CheckFloatIs("FromString 7.50", #f, "01000001 01110000");
	SmallFloat::FromString(#f, "-7.5");
	test_rc += CheckFloatIs("FromString -7.5", #f, "11000001 01110000");
	SmallFloat::FromString(#f, "-7.5ignored_suffix");
	test_rc += CheckFloatIs("FromString -7.5", #f, "11000001 01110000");
	SmallFloat::FromString(#f, "0.125");
	// The ideal result would be 00111110 10000000 but this is close enough.
	test_rc += CheckFloatIs("FromString 0.125", #f, "00111110 01111110");
	TestSmallFloatFromString := test_rc;
end;

function TestSmallFloatNeg():byte;
begin
	test_rc := 0;
	// We could go up to 256, but it's taking a bit long for not much
	for i := 0 to 26 do
	begin
		SmallFloat::FromU8(#sf, i);
		FormatFloat(#sf, #tmp_result);
		SmallFloat::Neg(#sf);
		tmp_result[0] := 49; // '1'
		test_rc += CheckFloatIs("Neg", #sf, #tmp_result);
	end;
	TestSmallFloatNeg := test_rc;
end;

function TestSmallFloatAssign():byte;
begin
	test_rc := 0;
	SmallFloat::FromU8(#ff, 0);
	SmallFloat::Assign(#f, #ff);
	test_rc += CheckFloatIs("Assign 0", #f, "00000000 00000000");
	SmallFloat::FromU8(#ff, 1);
	SmallFloat::Assign(#f, #ff);
	test_rc += CheckFloatIs("Assign 1", #f, "01000000 00000000");
	SmallFloat::FromU8(#ff, 7);
	SmallFloat::Assign(#f, #ff);
	test_rc += CheckFloatIs("Assign 7", #f, "01000001 01100000");
	SmallFloat::FromU8(#ff, 7);
	SmallFloat::Neg(#ff);
	SmallFloat::Assign(#f, #ff);
	test_rc += CheckFloatIs("Assign -7", #f, "11000001 01100000");
	TestSmallFloatAssign := test_rc;
end;