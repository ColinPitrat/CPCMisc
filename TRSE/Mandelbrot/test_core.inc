procedure PrintSmallFloat(printsmallfloat_f: pointer);
begin
	print_p := printsmallfloat_f+1;
	for j := 0 to 2 do
	begin
		Format::PaddedBinaryBtos(print_p^, #s);
		Text::Print(#s);
		Text::Print(" ");
		print_p -= 1;
	end;
	Text::Print("\r\n");
end;

procedure FormatFloat(formatfloat_f, formatfloat_s: pointer);
begin
	format_p := formatfloat_f+1;
	Format::PaddedBinaryBtos(format_p^, formatfloat_s);
	formatfloat_s[8] := $20;
	Format::PaddedBinaryBtos(formatfloat_f^, formatfloat_s+9);
	formatfloat_s[17] := 0;	
end;

function CheckFloatIs(test_case, test_value, test_expected: global pointer):byte;
begin
	FormatFloat(test_value, #test_result);
	test_failure := 0;
	for test_i := 0 to 18 do
	begin
		// Allows to have '?' for bits that do not matter
		if (test_result[test_i] <> test_expected[test_i] and test_expected[test_i] <> $3f) then
		begin
			test_failure := 1;
			break;
		end;
	end;
	if test_failure <> 0 then
	begin
		Text::Print("FAILURE: ");
		Text::Print(test_case);
		Text::Print("\r\n    expected: ");
		Text::Print(test_expected);
		Text::Print("\r\n         got: ");
		Text::Print(#test_result);
		Text::Print("\r\n");
	end;
	CheckFloatIs := test_failure;
end;

function CheckByteIs(test_byte_case: pointer, test_byte_value, test_byte_expected: global byte):byte;
begin
	test_failure := 0;
	if test_byte_value <> test_byte_expected then
	begin
		Text::Print("FAILURE: ");
		Text::Print(test_byte_case);
		Text::Print("\r\n    expected: ");
		Format::Btos(test_byte_expected, #test_byte_str);
		Text::Print(#test_byte_str);
		Text::Print("\r\n         got: ");
		Format::Btos(test_byte_value, #test_byte_str);
		Text::Print(#test_byte_str);
		Text::Print("\r\n");
		test_failure := 1;
	end;
	CheckByteIs := test_failure;
end;