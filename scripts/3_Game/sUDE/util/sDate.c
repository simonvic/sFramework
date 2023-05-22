class SDate {
	
	// TODO(perf): smash everything in a uint

	private int m_second;
	private int m_minute;
	private int m_hour;
	private int m_day;
	private int m_month;
	private int m_year;
	
	static SDate now() {
		SDate now = new SDate();
		GetYearMonthDayUTC(now.m_year, now.m_month, now.m_day);
		GetHourMinuteSecondUTC(now.m_hour, now.m_minute, now.m_second);
		return now;
	}
	
	bool after(SDate date) {
		return m_second > date.m_second || m_minute > date.m_minute || m_hour > date.m_hour || m_day > date.m_day || m_month > date.m_month || m_year > date.m_year;
	}
	
	bool before(SDate date) {
		return m_second < date.m_second || m_minute < date.m_minute || m_hour < date.m_hour || m_day < date.m_day || m_month < date.m_month || m_year < date.m_year;
	}
	
	bool equals(SDate date) {
		return m_second == date.m_second && m_minute == date.m_minute && m_hour == date.m_hour && m_day == date.m_day && m_month == date.m_month && m_year == date.m_year;
	}
	
	int getSecond() {
		return m_second;
	}
	
	void setSecond(int s) {
		m_second = s;
	}
	
	int getMinute() {
		return m_minute;
	}
	
	void setMinute(int m) {
		m_minute = m;
	}
	
	int getHour() {
		return m_hour;
	}
	
	void setHour(int h) {
		m_hour = h;
	}
	
	int getDay() {
		return m_day;
	}
	
	void setDay(int d) {
		m_day = d;
	}
	
	int getMonth() {
		return m_month;
	}
	
	void setMonth(int month) {
		m_month = month;
	}
	
	int getYear() {
		return m_year;
	}
	
	void setYear(int y) {
		m_year = y;
	}
	
	string toString() {
		return format("#Y/#m/#d #H:#M:#S");
	}
	
	string format(string format) {
		return SDate.format(this, format);
	}
	
	/**
	*/
	static string format(SDate date, string format) {
		int length = format.Length();
		if (length < 2) return format;
		string formatted;
		string char;
		int i;
		while (i < length - 1) {
			char = format[i++];
			if (char != "#") {
				formatted += char;
				continue;
			}
			string tempChar = format[i++];
			string unit = tempChar;
			while (tempChar == ":" && i < length) {
				tempChar = format[i++];
				unit += tempChar;
			}
			formatted += formatUnit(date, parseUnit(unit));
		}
		if (i < length) {
			formatted += format[i];
		}
		return formatted;
	}
	
	static SDateFormatUnit parseUnit(string unit) {
		switch (unit) {
			case "#": return SDateFormatUnit.HASHTAG;
			case "t": return SDateFormatUnit.TAB;
			case "n": return SDateFormatUnit.NEWLINE;
			case ":::z": return SDateFormatUnit.TIMEZONE;
			case "z": return SDateFormatUnit.TIMEZONE_MILITARY;
			case ":z": return SDateFormatUnit.TIMEZONE_HHMM;
			case "::z": return SDateFormatUnit.TIMEZONE_HHMMSS;
			case "Z": return SDateFormatUnit.TIMEZONE_ALPHABETIC;
			case "N": return SDateFormatUnit.NANOSECONDS;
			case "S": return SDateFormatUnit.SECONDS;
			case "s": return SDateFormatUnit.SECONDS_EPOCH;
			case "M": return SDateFormatUnit.MINUTE;
			case "H": return SDateFormatUnit.HOUR_24;
			case "I": return SDateFormatUnit.HOUR_12;
			case "k": return SDateFormatUnit.HOUR_24_PADDED;
			case "l": return SDateFormatUnit.HOUR_12_PADDED;
			case "j": return SDateFormatUnit.DAY_OF_YEAR;
			case "d": return SDateFormatUnit.DAY_OF_MONTH;
			case "e": return SDateFormatUnit.DAY_OF_MONTH_PADDED;
			case "u": return SDateFormatUnit.DAY_OF_WEEK;
			case "w": return SDateFormatUnit.DAY_OF_WEEK_SUNDAY;
			case "A": return SDateFormatUnit.WEEKDAY;
			case "a": return SDateFormatUnit.WEEKDAY_ABBREVIATED;
			case "g": return SDateFormatUnit.WEEKNUMBER_ISO_LAST_DIGITS;
			case "U": return SDateFormatUnit.WEEKNUMBER_OF_YEAR_SUNDAY;
			case "V": return SDateFormatUnit.WEEKNUMBER_ISO;
			case "W": return SDateFormatUnit.WEEKNUMBER_OF_YEAR;
			case "m": return SDateFormatUnit.MONTH;
			case "b": return SDateFormatUnit.MONTH_NAME_ABBREVIATED;
			case "h": return SDateFormatUnit.MONTH_NAME_ABBREVIATED_ALTERNATIVE;
			case "B": return SDateFormatUnit.MONTH_NAME_LOCALE;
			case "Y": return SDateFormatUnit.YEAR;
			case "y": return SDateFormatUnit.YEAR_LAST_DIGITS;
			case "q": return SDateFormatUnit.YEAR_QUARTER;
			case "C": return SDateFormatUnit.CENTURY;
			case "c": return SDateFormatUnit.DATE_TIME_LOCALE;
			case "D": return SDateFormatUnit.DATE;
			case "F": return SDateFormatUnit.DATE_FULL;
			case "G": return SDateFormatUnit.YEAR_ISO;
			case "p": return SDateFormatUnit.AM_PM;
			case "P": return SDateFormatUnit.AM_PM_LOWERCASE;
			case "r": return SDateFormatUnit.TIME_12_LOCALE;
			case "R": return SDateFormatUnit.TIME_24;
			case "T": return SDateFormatUnit.TIME;
			case "x": return SDateFormatUnit.DATE_LOCALE;
			case "X": return SDateFormatUnit.TIME_LOCALE;
		}
		return SDateFormatUnit.UNKOWN;
	}
	
	static string formatUnit(SDate date, SDateFormatUnit unit) {
		switch (unit) {
			case SDateFormatUnit.HASHTAG: return "#";
			case SDateFormatUnit.TAB: return "\t";
			case SDateFormatUnit.NEWLINE: return "\n";
			case SDateFormatUnit.TIMEZONE: return "";
			case SDateFormatUnit.TIMEZONE_MILITARY: return "";
			case SDateFormatUnit.TIMEZONE_HHMM: return "";
			case SDateFormatUnit.TIMEZONE_HHMMSS: return "";
			case SDateFormatUnit.TIMEZONE_ALPHABETIC: return "";
			case SDateFormatUnit.NANOSECONDS: return "";
			case SDateFormatUnit.SECONDS: return date.getSecond().ToStringLen(2);
			case SDateFormatUnit.SECONDS_EPOCH: return "";
			case SDateFormatUnit.MINUTE: return date.getMinute().ToStringLen(2);
			case SDateFormatUnit.HOUR_24: return date.getHour().ToStringLen(2);
			case SDateFormatUnit.HOUR_12: return pmToAmHour(date.getHour()).ToStringLen(2);
			case SDateFormatUnit.HOUR_24_PADDED: return date.getHour().ToStringLen(2);
			case SDateFormatUnit.HOUR_12_PADDED: return pmToAmHour(date.getHour()).ToStringLen(2);
			case SDateFormatUnit.DAY_OF_YEAR: return "";
			case SDateFormatUnit.DAY_OF_MONTH: return date.getDay().ToString();
			case SDateFormatUnit.DAY_OF_MONTH_PADDED: return date.getDay().ToStringLen(2);
			case SDateFormatUnit.DAY_OF_WEEK: return "";
			case SDateFormatUnit.DAY_OF_WEEK_SUNDAY: return "";
			case SDateFormatUnit.WEEKDAY: return "";
			case SDateFormatUnit.WEEKDAY_ABBREVIATED: return "";
			case SDateFormatUnit.WEEKNUMBER_ISO_LAST_DIGITS: return "?";
			case SDateFormatUnit.WEEKNUMBER_OF_YEAR_SUNDAY: return "";
			case SDateFormatUnit.WEEKNUMBER_ISO: return "";
			case SDateFormatUnit.WEEKNUMBER_OF_YEAR: return "";
			case SDateFormatUnit.MONTH: return date.getMonth().ToStringLen(2);
			case SDateFormatUnit.MONTH_NAME_ABBREVIATED:
			case SDateFormatUnit.MONTH_NAME_ABBREVIATED_ALTERNATIVE: return getMonthName(date.getMonth());
			case SDateFormatUnit.MONTH_NAME_LOCALE: return getMonthNameLocale(date.getMonth());
			case SDateFormatUnit.YEAR: return date.getYear().ToStringLen(2);
			case SDateFormatUnit.YEAR_LAST_DIGITS: return date.getYear().ToString().Substring(2, 2);
			case SDateFormatUnit.YEAR_QUARTER: return "";
			case SDateFormatUnit.CENTURY: return date.getYear().ToString().Substring(0, 2);
			case SDateFormatUnit.DATE_TIME_LOCALE: return "";
			case SDateFormatUnit.DATE: return "";
			case SDateFormatUnit.DATE_FULL: return "";
			case SDateFormatUnit.YEAR_ISO: return "";
			case SDateFormatUnit.AM_PM: return "";
			case SDateFormatUnit.AM_PM_LOWERCASE: return "";
			case SDateFormatUnit.TIME_12_LOCALE: return "";
			case SDateFormatUnit.TIME_24: return "";
			case SDateFormatUnit.TIME: return "";
			case SDateFormatUnit.DATE_LOCALE: return "";
			case SDateFormatUnit.TIME_LOCALE: return "";
		}
		return "";
	}
	
	static int pmToAmHour(int pm) {
		if (pm > 12) return pm - 12;
		return pm;
	}
	
	static string getMonthName(int month) {
		switch (month) {
			case 0: return "January";
			case 1: return "February";
			case 2: return "March";
			case 3: return "April";
			case 4: return "May";
			case 5: return "June";
			case 6: return "July";
			case 7: return "August";
			case 8: return "September";
			case 9: return "October";
			case 10: return "November";
			case 11: return "December";	
		}
		return "";
	}
	
	static string getMonthNameLocale(int month) {
		// TODO: use stringtable
		return getMonthName(month);
	}
	
	
}

enum SDateFormatUnit {
	HASHTAG,                               // ##      a literal # (hashtag sign)
	TAB,                                   // #t      a tab
	NEWLINE,                               // #n      a newline
	TIMEZONE,                              // #:::z   numeric time zone with : to necessary precisi
	TIMEZONE_MILITARY,                     // #z      +hhmm numeric time zone (e.g., -0400)
	TIMEZONE_HHMM,                         // #:z     +hh:mm numeric time zone (e.g., -04:00)
	TIMEZONE_HHMMSS,                       // #::z    +hh:mm:ss numeric time zone (e.g., -04:00:00)
	TIMEZONE_ALPHABETIC,                   // #Z      alphabetic time zone abbreviation (e.g., EDT)
	NANOSECONDS,                           // #N      nanoseconds (000000000..999999999)
	SECONDS,                               // #S      second (00..60)
	SECONDS_EPOCH,                         // #s      seconds since the Epoch (1970-01-01 00:00 UTC
	MINUTE,                                // #M      minute (00..59)
	HOUR_24,                               // #H      hour (00..23)
	HOUR_12,                               // #I      hour (01..12)
	HOUR_24_PADDED,                        // #k      hour, space padded ( 0..23); same as %_H
	HOUR_12_PADDED,                        // #l      hour, space padded ( 1..12); same as %_I
	DAY_OF_YEAR,                           // #j      day of year (001..366)
	DAY_OF_MONTH,                          // #d      day of month (e.g., 01)
	DAY_OF_MONTH_PADDED,                   // #e      day of month, space padded; same as %_d
	DAY_OF_WEEK,                           // #u      day of week (1..7); 1 is Monday
	DAY_OF_WEEK_SUNDAY,                    // #w      day of week (0..6); 0 is Sunday
	WEEKDAY,                               // #A      locale's full weekday name (e.g., Sunday)
	WEEKDAY_ABBREVIATED,                   // #a      locale's abbreviated weekday name (e.g., Sun)
	WEEKNUMBER_ISO_LAST_DIGITS,            // #g      last two digits of year of ISO week number (s
	WEEKNUMBER_OF_YEAR_SUNDAY,             // #U      week number of year, with Sunday as first day
	WEEKNUMBER_ISO,                        // #V      ISO week number, with Monday as first day of 
	WEEKNUMBER_OF_YEAR,                    // #W      week number of year, with Monday as first day
	MONTH,                                 // #m      month (01..12)
	MONTH_NAME_ABBREVIATED,                // #b      locale's abbreviated month name (e.g., Jan)
	MONTH_NAME_ABBREVIATED_ALTERNATIVE,    // #h      same as %b
	MONTH_NAME_LOCALE,                     // #B      locale's full month name (e.g., January)
	YEAR,                                  // #Y      year
	YEAR_LAST_DIGITS,                      // #y      last two digits of year (00..99)
	YEAR_QUARTER,                          // #q      quarter of year (1..4)
	CENTURY,                               // #C      century; like %Y, except omit last two digits
	DATE_TIME_LOCALE,                      // #c      locale's date and time (e.g., Thu Mar  3 23:0
	DATE,                                  // #D      date; same as %m/%d/%y
	DATE_FULL,                             // #F      full date; like %+4Y-%m-%d
	YEAR_ISO,                              // #G      year of ISO week number (see %V); normally us
	AM_PM,                                 // #p      locale's equivalent of either AM or PM; blank
	AM_PM_LOWERCASE,                       // #P      like %p, but lower case
	TIME_12_LOCALE,                        // #r      locale's 12-hour clock time (e.g., 11:11:04 P
	TIME_24,                               // #R      24-hour hour and minute; same as %H:%M
	TIME,                                  // #T      time; same as %H:%M:%S
	DATE_LOCALE,                           // #x      locale's date representation (e.g., 12/31/99)
	TIME_LOCALE,                           // #X      locale's time representation (e.g., 23:13:48)
	UNKOWN
}