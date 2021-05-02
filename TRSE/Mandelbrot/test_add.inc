function TestOneSmallFloatAdd(i, j, negfirst, negsecond: global byte, p: global pointer):byte;
begin
	SmallFloat::FromU16(#sf1, i);
	SmallFloat::FromU16(#sf2, j);
	k := Format::Strcpy("Add ", #long_string);
	
	if negfirst <> 0 then
	begin
		SmallFloat::Neg(#sf1);
		k += Format::Strcpy("-", #long_string+k);
	end;
	k += Format::Itos(i, #long_string+k);
	k += Format::Strcpy("+", #long_string+k);
	if negsecond <> 0 then
	begin
		SmallFloat::Neg(#sf2);
		k += Format::Strcpy("-", #long_string+k);
	end;
	k += Format::Itos(j, #long_string+k);
	SmallFloat::Add(#sf, #sf1, #sf2);
	TestOneSmallFloatAdd := CheckFloatIs(#long_string, #sf, p);
end;

function TestSmallFloatAdd():byte;
begin
	test_rc := 0;

	// Add 2 positive numbers
	test_rc += TestOneSmallFloatAdd(0, 0, 0, 0, "?0000000 00000000");
	test_rc += TestOneSmallFloatAdd(1, 0, 0, 0, "01000000 00000000");
	test_rc += TestOneSmallFloatAdd(1, 1, 0, 0, "01000000 10000000");
	test_rc += TestOneSmallFloatAdd(1, 2, 0, 0, "01000000 11000000");
	test_rc += TestOneSmallFloatAdd(2, 1, 0, 0, "01000000 11000000");
	test_rc += TestOneSmallFloatAdd(2, 3, 0, 0, "01000001 00100000");
	test_rc += TestOneSmallFloatAdd(1, 5, 0, 0, "01000001 01000000");
	test_rc += TestOneSmallFloatAdd(5, 5, 0, 0, "01000001 10100000");
	test_rc += TestOneSmallFloatAdd(7, 2, 0, 0, "01000001 10010000");
	test_rc += TestOneSmallFloatAdd(7, 5, 0, 0, "01000001 11000000");
	test_rc += TestOneSmallFloatAdd(12, 5, 0, 0, "01000010 00001000");
	test_rc += TestOneSmallFloatAdd(5, 12, 0, 0, "01000010 00001000");

	// Add a negative and a positive
	test_rc += TestOneSmallFloatAdd(0, 0, 1, 0, "?0000000 00000000");
	test_rc += TestOneSmallFloatAdd(1, 0, 1, 0, "11000000 00000000");
	test_rc += TestOneSmallFloatAdd(1, 1, 1, 0, "00000000 00000000");
	test_rc += TestOneSmallFloatAdd(1, 2, 1, 0, "01000000 00000000");
	test_rc += TestOneSmallFloatAdd(2, 1, 1, 0, "11000000 00000000");
	test_rc += TestOneSmallFloatAdd(2, 3, 1, 0, "01000000 00000000");
	test_rc += TestOneSmallFloatAdd(1, 5, 1, 0, "01000001 00000000");
	test_rc += TestOneSmallFloatAdd(5, 5, 1, 0, "00000000 00000000");
	test_rc += TestOneSmallFloatAdd(7, 2, 1, 0, "11000001 00100000");
	test_rc += TestOneSmallFloatAdd(7, 5, 1, 0, "11000000 10000000");
	test_rc += TestOneSmallFloatAdd(12, 5, 1, 0, "11000001 01100000");
	test_rc += TestOneSmallFloatAdd(5, 12, 1, 0, "01000001 01100000");

	// Add a positive and a negative
	test_rc += TestOneSmallFloatAdd(0, 0, 0, 1, "?0000000 00000000");
	test_rc += TestOneSmallFloatAdd(1, 0, 0, 1, "01000000 00000000");
	test_rc += TestOneSmallFloatAdd(1, 1, 0, 1, "00000000 00000000");
	test_rc += TestOneSmallFloatAdd(1, 2, 0, 1, "11000000 00000000");
	test_rc += TestOneSmallFloatAdd(2, 1, 0, 1, "01000000 00000000");
	test_rc += TestOneSmallFloatAdd(2, 3, 0, 1, "11000000 00000000");
	test_rc += TestOneSmallFloatAdd(1, 5, 0, 1, "11000001 00000000");
	test_rc += TestOneSmallFloatAdd(5, 5, 0, 1, "00000000 00000000");
	test_rc += TestOneSmallFloatAdd(7, 2, 0, 1, "01000001 00100000");
	test_rc += TestOneSmallFloatAdd(7, 5, 0, 1, "01000000 10000000");
	test_rc += TestOneSmallFloatAdd(12, 5, 0, 1, "01000001 01100000");
	test_rc += TestOneSmallFloatAdd(5, 12, 0, 1, "11000001 01100000");

	// Add two negative numbers
	test_rc += TestOneSmallFloatAdd(0, 0, 1, 1, "?0000000 00000000");
	test_rc += TestOneSmallFloatAdd(1, 0, 1, 1, "11000000 00000000");
	test_rc += TestOneSmallFloatAdd(1, 1, 1, 1, "11000000 10000000");
	test_rc += TestOneSmallFloatAdd(1, 2, 1, 1, "11000000 11000000");
	test_rc += TestOneSmallFloatAdd(2, 1, 1, 1, "11000000 11000000");
	test_rc += TestOneSmallFloatAdd(2, 3, 1, 1, "11000001 00100000");
	test_rc += TestOneSmallFloatAdd(1, 5, 1, 1, "11000001 01000000");
	test_rc += TestOneSmallFloatAdd(5, 5, 1, 1, "11000001 10100000");
	test_rc += TestOneSmallFloatAdd(7, 2, 1, 1, "11000001 10010000");
	test_rc += TestOneSmallFloatAdd(7, 5, 1, 1, "11000001 11000000");
	test_rc += TestOneSmallFloatAdd(12, 5, 1, 1, "11000010 00001000");
	test_rc += TestOneSmallFloatAdd(5, 12, 1, 1, "11000010 00001000");

	TestSmallFloatAdd := test_rc;
end;
