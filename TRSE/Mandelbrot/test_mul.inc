function TestOneSmallFloatMul(i, j: global byte, p: global pointer):byte;
begin
	Format::Strcpy("Mul ", #long_string);
	Format::Btos(i, #long_string+4);
	for k := 4 to 12 do
	begin
		if long_string[k] = 0 then break;
	end;
	Format::Strcpy("*", #long_string+k);
	k += 1;
	Format::Btos(j, #long_string+k);
	SmallFloat::FromU8(#sf1, i);
	SmallFloat::FromU8(#sf2, j);
	SmallFloat::Multiply(#sf, #sf1, #sf2);
	TestOneSmallFloatMul := CheckFloatIs(#long_string, #sf, p);
end;

function TestOneSmallFloatMulNegFirst(i, j: global byte, p: global pointer):byte;
begin
	Format::Strcpy("Mul -", #long_string);
	Format::Btos(i, #long_string+5);
	for k := 5 to 12 do
	begin
		if long_string[k] = 0 then break;
	end;
	Format::Strcpy("*", #long_string+k);
	k += 1;
	Format::Btos(j, #long_string+k);
	SmallFloat::FromU8(#sf1, i);
	SmallFloat::Neg(#sf1);
	SmallFloat::FromU8(#sf2, j);
	SmallFloat::Multiply(#sf, #sf1, #sf2);
	TestOneSmallFloatMulNegFirst := CheckFloatIs(#long_string, #sf, p);
end;

function TestOneSmallFloatMulNegSecond(i, j: global byte, p: global pointer):byte;
begin
	Format::Strcpy("Mul ", #long_string);
	Format::Btos(i, #long_string+4);
	for k := 4 to 12 do
	begin
		if long_string[k] = 0 then break;
	end;
	Format::Strcpy("*-", #long_string+k);
	k += 2;
	Format::Btos(j, #long_string+k);
	SmallFloat::FromU8(#sf1, i);
	SmallFloat::FromU8(#sf2, j);
	SmallFloat::Neg(#sf2);
	SmallFloat::Multiply(#sf, #sf1, #sf2);
	TestOneSmallFloatMulNegSecond := CheckFloatIs(#long_string, #sf, p);
end;


function TestOneSmallFloatMulNegBoth(i, j: global byte, p: global pointer):byte;
begin
	Format::Strcpy("Mul -", #long_string);
	Format::Btos(i, #long_string+5);
	for k := 5 to 12 do
	begin
		if long_string[k] = 0 then break;
	end;
	Format::Strcpy("*-", #long_string+k);
	k += 2;
	Format::Btos(j, #long_string+k);
	SmallFloat::FromU8(#sf1, i);
	SmallFloat::Neg(#sf1);
	SmallFloat::FromU8(#sf2, j);
	SmallFloat::Neg(#sf2);
	SmallFloat::Multiply(#sf, #sf1, #sf2);
	TestOneSmallFloatMulNegBoth := CheckFloatIs(#long_string, #sf, p);
end;

function TestSmallFloatMul():byte;
begin
	test_rc := 0;
	test_rc += TestOneSmallFloatMul(0, 0, "00000000 00000000");
	test_rc += TestOneSmallFloatMul(1, 1, "01000000 00000000");
	test_rc += TestOneSmallFloatMul(7, 2, "01000001 11100000");
	test_rc += TestOneSmallFloatMul(2, 80, "01000011 10100000");
	test_rc += TestOneSmallFloatMul(3, 80, "01000011 11110000");
	test_rc += TestOneSmallFloatMulNegFirst(0, 0, "00000000 00000000");
	test_rc += TestOneSmallFloatMulNegFirst(1, 1, "11000000 00000000");
	test_rc += TestOneSmallFloatMulNegFirst(7, 2, "11000001 11100000");
	test_rc += TestOneSmallFloatMulNegFirst(2, 80, "11000011 10100000");
	test_rc += TestOneSmallFloatMulNegFirst(3, 80, "11000011 11110000");
	test_rc += TestOneSmallFloatMulNegSecond(0, 0, "00000000 00000000");
	test_rc += TestOneSmallFloatMulNegSecond(1, 1, "11000000 00000000");
	test_rc += TestOneSmallFloatMulNegSecond(7, 2, "11000001 11100000");
	test_rc += TestOneSmallFloatMulNegSecond(2, 80, "11000011 10100000");
	test_rc += TestOneSmallFloatMulNegSecond(3, 80, "11000011 11110000");
	test_rc += TestOneSmallFloatMulNegBoth(0, 0, "00000000 00000000");
	test_rc += TestOneSmallFloatMulNegBoth(1, 1, "01000000 00000000");
	test_rc += TestOneSmallFloatMulNegBoth(7, 2, "01000001 11100000");
	test_rc += TestOneSmallFloatMulNegBoth(2, 80, "01000011 10100000");
	test_rc += TestOneSmallFloatMulNegBoth(3, 80, "01000011 11110000");
	TestSmallFloatMul := test_rc;
end;
