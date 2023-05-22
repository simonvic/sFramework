#ifdef DIAG_DEVELOPER

// TODO(tests): add more
class TestUnit_SDate : STestUnit {
	override void init() {
		registerTestCases({
			"test_second"
			"test_minute"
			"test_hour"
			"test_day"
			"test_month"
			"test_now"
			"test_equal_newInstances"
			"test_equal_now"
			"datesWithDifferentSeconds_shouldNotBeEquals"
			"differentDates_shouldNotBeEquals"
			"test_before"
			"test_after"
			"test_format"
		});
	}
	
	void test_second() {
		SDate d = new SDate();
		d.setSecond(42);
		assertEqual(42, d.getSecond());
	}
	
	void test_minute() {
		SDate d = new SDate();
		d.setMinute(42);
		assertEqual(42, d.getMinute());
	}
	
	void test_hour() {
		SDate d = new SDate();
		d.setHour(16);
		assertEqual(16, d.getHour());
	}
	
	void test_day() {
		SDate d = new SDate();
		d.setDay(6);
		assertEqual(6, d.getDay());
	}
	
	void test_month() {
		SDate d = new SDate();
		d.setMonth(9);
		assertEqual(9, d.getMonth());
	}

	void test_now() {
		SDate d = SDate.now();
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;
		GetYearMonthDayUTC(year, month, day);
		GetHourMinuteSecondUTC(hour, minute, second);
		if (d.getYear() != year) fail(year.ToString(), d.getYear().ToString(), "year");
		else if (d.getMonth() != month) fail(month.ToString(), d.getMonth().ToString(), "month");
		else if (d.getDay() != day) fail(day.ToString(), d.getDay().ToString(), "day");
		else if (d.getHour() != hour) fail(hour.ToString(), d.getHour().ToString(), "hour");
		else if (d.getMinute() != minute) fail(minute.ToString(), d.getMinute().ToString(), "minute");
		else if (d.getSecond() != second) fail(second.ToString(), d.getSecond().ToString(), "second");
		else pass("", "", "Date is correct");
	}
	
	void test_equal_newInstances() {
		SDate a = new SDate();
		SDate b = new SDate();
		assertTrue(a.equals(b));
	}
	
	void test_equal_now() {
		SDate a = SDate.now();
		a.setSecond(0);		
		
		SDate b = SDate.now();
		b.setSecond(0);
		
		assertTrue(a.equals(b));
	}
	
	void datesWithDifferentSeconds_shouldNotBeEquals() {
		SDate a = new SDate();
		a.setSecond(0);
		
		SDate b = new SDate();
		b.setSecond(1);
		assertFalse(a.equals(b));
	}
	
	void differentDates_shouldNotBeEquals() {
		SDate a = new SDate();
		a.setSecond(0);
		a.setHour(5);
		
		SDate b = new SDate();
		b.setSecond(1);
		b.setHour(2);
		assertFalse(a.equals(b));
	}
	
	void test_after() {
		SDate a = new SDate();
		a.setMinute(1);
		a.setSecond(5);
		
		SDate b = new SDate();
		b.setMinute(0);
		b.setSecond(0);
		
		assertTrue(a.after(b));
	}
	
	void test_before() {
		SDate a = new SDate();
		a.setMinute(1);
		a.setSecond(5);
		
		SDate b = new SDate();
		b.setMinute(0);
		b.setSecond(0);
		
		assertTrue(b.before(a));
	}
	
	void test_format() {
		SDate now = SDate.now();
		string expected = string.Format("%1/%2/%3 # %4:%5:%6", now.getYear(), now.getMonth(), now.getDay(), now.getHour(), now.getMinute(), now.getSecond());
		string actual = now.format("#Y/#m/#d ## #H:#M:#S");
		assertEqual(expected, actual);
	}
	
	
	
	
	
	
	
}

#endif