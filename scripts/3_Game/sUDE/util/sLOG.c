/**
 * @brief Log entry severity level
 * DEBUG: for debugging purposes, should not be used on retail
 * INFO: for information purposes
 * WARNING: for something that the reader should be aware of; not necessarily an error
 * ERROR: for something that wasn't expected
 * CRITICAL: for something that wasn't expected and may cause other problems
 */
enum SLoggerLevels {
	DEBUG = 0,
	INFO,
	WARNING,
	ERROR,
	CRITICAL,

	COUNT
}

class SLOG {

	static bool headerPrinted;
	static float time;

	static void onUpdate(float dt) {
		time += dt;
	}

	/**
	 * @brief Whether or not to print to log file
	 */
	static bool PRINT_TO_FILE = true;

	/**
	 * @brief Path to logs directory
	 * Allowed prefixes: $profile, $saves
	 */
	static string LOG_PATH = "$profile:sUDE\\logs";

	/**
	 * @brief Extension of file (will be suffixed to the file name)
	 */
	static string LOGFILE_EXTENSION = ".slog";

	/**
	 * @brief Severity level threshold.
	 * Log entries with lower severity won't be printed
	 */
	static SLoggerLevels LEVEL = SLoggerLevels.DEBUG;
	
	/**
	 * @brief Text tag for each SLoggerLevels
	 */
	static string LEVEL_TAGS[] = {
		"[DD]",
		"[II]",
		"[WW]",
		"[EE]",
		"[CC]"
	};

	/**
	 * @brief String format for a log entry.
	 * It will be used in string::Format
	 * %1 : timestamp
	 * %2 : level
	 * %3 : indentation
	 * %4 : caller
	 * %5 : text
	 */
	static string PRINT_FORMAT = "%1 %2 | %3%4%5";

	/**
	 * @brief String format for timestamps.
	 * It will be used in string::Format
	 * %1 : year
	 * %2 : month
	 * %3 : day
	 * %4 : hour
	 * %5 : minutes
	 * %6 : seconds
	 * %7 : milliseconds (approx. based on delta time; remains the same on same frame)
	 */
	static string DATE_PRINT_FORMAT = "%4:%5:%6.%7";

	/**
	 * @brief String format for variables.
	 * It will be used in string::Format
	 * %1 : typename
	 * %2 : variable value
	 */
	static string VARIABLE_PRINT_FORMAT = "<%1> %2";

	/**
	 * @brief String format for log entry caller.
	 * It will be used in string::Format
	 * %1 : caller
	 */
	static string CALLER_FORMAT = "::%1 => ";

	/**
	 * @brief Whether or not to print floats using "simple" format
	 * @see float::ToString
	 */
	static bool SIMPLE_FLOAT = true;

	/**
	 * @brief Indentation guide; one for each indentation level - 1
	 */
	static string INDENT_GUIDE = "\t|";

	/**
	 * @brief Last indentation guide; only for indentation level >= 1
	 */
	static string INDENT_GUIDE_LAST = "\t|- ";

	/**
	 * @brief Get formatted indentation prefix
	 * @param indentation depth
	 * @return formatted indentation
	 */
	private static string getIndentation(int indentation) {
		string temp = "";
		if (indentation >= 1) {
			for (int i = 0; i < indentation - 1; i++) {
				temp += INDENT_GUIDE;
			}
			temp += INDENT_GUIDE_LAST;
		}
		return temp;
	}

	/**
	 * @brief Build a string containing current timestamp
	 * @param format
	 * @return formatted string
	 */
	static string buildTimestamp(string format = DATE_PRINT_FORMAT) {
		int year, month, day, hour, minute, second, ms;
		GetYearMonthDayUTC(year, month, day);
		GetHourMinuteSecondUTC(hour, minute, second);
		ms = (SLOG.time - ((int) SLOG.time)) * 1000;
		string monthS = month.ToStringLen(2);
		string dayS = day.ToStringLen(2);
		string hourS = hour.ToStringLen(2);
		string minuteS = minute.ToStringLen(2);
		string secondS  = second.ToStringLen(2);
		string msS = ms.ToStringLen(3);
		return string.Format(format, year, monthS, dayS, hourS, minuteS, secondS, msS);
	}

	/**
	 * @brief Build a string with formatted output
	 * @param level severity level
	 * @param int indentation level
	 * @param caller who invoked the log
	 * @param text to print
	 * @return formatted string
	 */
	static string buildOutput(SLoggerLevels level, int indentation, string caller, string text) {
		if (caller != string.Empty) {
			caller = string.Format(CALLER_FORMAT, caller);
		}
		return string.Format(PRINT_FORMAT, buildTimestamp(DATE_PRINT_FORMAT), LEVEL_TAGS[level], getIndentation(indentation), caller, text);
	}

	/**
	 * @brief Print log header to file
	 * @param file handle
	 */
	static void printHeader(FileHandle file) {
		string playerName;
		string worldName;
		string version;
		string profileName;
		string machineName;
		if (GetGame()) {
			GetGame().GetPlayerName(playerName);			
			GetGame().GetWorldName(worldName);
			GetGame().GetVersion(version);
		}
		profileName = GetProfileName();
		machineName = GetMachineName();
		string header;
		header += string.Format("====================================================================\n");
		header += string.Format("                      %1 \n",buildTimestamp("%1/%2/%3 %4:%5:%6"));
		header += string.Format("--------------------------------------------------------------------\n");
		header += string.Format("                  Profile name  : %1\n", profileName);
		header += string.Format("                  Machine name  : %1\n", machineName);
		header += string.Format("                  Player name   : %1\n", playerName);
		header += string.Format("                  World name    : %1\n", worldName);
		header += string.Format("                  Game version  : %1\n", version);
		header += string.Format("                  Mods          :");
		FPrintln(file, header);
		string mods;
		if (GetCLIParam("mod", mods)) {
			array<string> modList = {};
			mods.Split(";", modList);
			foreach (string mod : modList) {
				// NOTE: mod list may be too big for a single print
				FPrintln(file, "                      - " + mod);
			}
		}
		FPrintln(file, "--------------------------------------------------------------------");
	}

	/**
	 * @brief Print log footer
	 */
	static void printFooter() {
		float avgFrametime = -1;
		float tickTime = -1;
		float maxFPS = -1;
		float avgFPS = -1;
		float minFPS = -1;
		if (GetGame()) {
			avgFrametime = GetGame().GetFps();
			tickTime = GetGame().GetTickTime();
			maxFPS = GetGame().GetMaxFPS();
			avgFPS = GetGame().GetAvgFPS();
			minFPS = GetGame().GetMinFPS();
		}
		string footer;
		footer += string.Format("--------------------------------------------------------------------\n");
		footer += string.Format("                  Tick time : %1\n", tickTime);
		footer += string.Format("                    max FPS : %1\n", maxFPS);
		footer += string.Format("                    avg FPS : %1\n", maxFPS);
		footer += string.Format("          avg frametime(ms) : %1\n", avgFrametime);
		footer += string.Format("                    min FPS : %1\n", maxFPS);
		footer += string.Format("--------------------------------------------------------------------\n");
		footer += string.Format("                  %1 \n",buildTimestamp("%1/%2/%3 %4:%5:%6"));
		footer += string.Format("====================================================================\n");
		printToFile(footer);
	}

	/**
	 * @brief Print log entry to file
	 * @param text to print
	 */
	private static void printToFile(string text) {
		string today = buildTimestamp("%1-%2-%3");
		string filePath = LOG_PATH + "\\" + today + LOGFILE_EXTENSION;
		SFileHelper.touch(filePath);
		FileHandle file = OpenFile(filePath, FileMode.APPEND);
		if (file != 0) {
			if (!headerPrinted) {
				printHeader(file);
				headerPrinted = true;
			}
			FPrintln(file, text);
			CloseFile(file);
		}
	}

	/**
	 * @brief Convert a variable to string
	 * Primitive values will be converted using their ToString() method.
	 * Array of primitives will also print its content. Array of non primitives will not.
	 * @param variable to print
	 */
	static string toString(Class variable) {
		if (variable == null) {
			return string.Format(VARIABLE_PRINT_FORMAT, "?", "NULL");
		}
		if (variable.IsInherited(array)) {
			string entries;
			// TODO: replace with StaticType() ?
			string arrayType = variable.Type().ToString();
			switch (arrayType) {

				case "array<typename>": 
				array<typename> typenameArray = array<typename>.Cast(variable);
				foreach (typename typenameEntry : typenameArray) entries += typenameEntry.ToString() + ", ";
				break;

				case "array<float>": 
				array<float> floatArray = array<float>.Cast(variable);
				foreach (float floatEntry : floatArray) entries += floatEntry.ToString(SIMPLE_FLOAT) + ", ";
				break;

				case "array<int>": 
				array<int> intArray = array<int>.Cast(variable);
				foreach (int intEntry : intArray) entries += intEntry.ToString() + ", ";
				break;

				case "array<bool>": 
				array<bool> boolArray = array<bool>.Cast(variable);
				foreach (bool boolEntry : boolArray) entries += boolEntry.ToString() + ", ";
				break;

				case "array<vector>": 
				array<vector> vectorArray = array<vector>.Cast(variable);
				foreach (vector vectorEntry : vectorArray) entries += vectorEntry.ToString() + ", ";
				break;

				case "array<string>": 
				array<string> stringArray = array<string>.Cast(variable);
				foreach (string stringEntry : stringArray) entries += stringEntry + ", ";
				break;

				default:
				return string.Format(VARIABLE_PRINT_FORMAT, "", variable.ToString());

			}
			return string.Format(VARIABLE_PRINT_FORMAT, "", variable.ToString() + " {" + entries + "}");	
		}
		return string.Format(VARIABLE_PRINT_FORMAT, "", variable.ToString());
	}

	static string toString(typename variable) {
		return string.Format(VARIABLE_PRINT_FORMAT, "typename", variable.ToString());
	}

	static string toString(float variable) {
		return string.Format(VARIABLE_PRINT_FORMAT, "float", variable.ToString(SIMPLE_FLOAT));
	}

	static string toString(int variable) {
		return string.Format(VARIABLE_PRINT_FORMAT, "int", variable.ToString());
	}

	static string toString(bool variable) {
		return string.Format(VARIABLE_PRINT_FORMAT, "bool", variable.ToString());
	}

	static string toString(vector variable) {
		return string.Format(VARIABLE_PRINT_FORMAT, "vector", variable.ToString());
	}

	static string toString(string variable) {
		return variable;
	}

	static void log(SLoggerLevels level, int indentation, string caller, string text) {
		if (level < LEVEL) return;
		string output = buildOutput(level, indentation, caller, text);
		PrintFormat("%1", output);
		if (PRINT_TO_FILE) {
			SLOG.printToFile(output);
		}
	}

	//@formatter:off

	static void log(int indentation, string caller, Class variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, indentation, caller, toString(variable)); }
	static void log(int indentation, string caller, typename variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, indentation, caller, toString(variable)); }
	static void log(int indentation, string caller, float variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, indentation, caller, toString(variable)); }
	static void log(int indentation, string caller, int variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, indentation, caller, toString(variable)); }
	static void log(int indentation, string caller, bool variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, indentation, caller, toString(variable)); }
	static void log(int indentation, string caller, vector variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, indentation, caller, toString(variable)); }
	static void log(int indentation, string caller, string variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, indentation, caller, toString(variable)); }
	static void log(string caller, Class variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, 0, caller, toString(variable)); }
	static void log(string caller, typename variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, 0, caller, toString(variable)); }
	static void log(string caller, float variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, 0, caller, toString(variable)); }
	static void log(string caller, int variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, 0, caller, toString(variable)); }
	static void log(string caller, bool variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, 0, caller, toString(variable)); }
	static void log(string caller, vector variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, 0, caller, toString(variable)); }
	static void log(string caller, string variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, 0, caller, toString(variable)); }
	static void log(int indentation, Class variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, indentation, "", toString(variable)); }
	static void log(int indentation, typename variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, indentation, "", toString(variable)); }
	static void log(int indentation, float variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, indentation, "", toString(variable)); }
	static void log(int indentation, int variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, indentation, "", toString(variable)); }
	static void log(int indentation, bool variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, indentation, "", toString(variable)); }
	static void log(int indentation, vector variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, indentation, "", toString(variable)); }
	//static void log(int indentation, string variable, SLoggerLevels level = SLoggerLevels.INFO) { SLOG.log(level, indentation, "", toString(variable)); }

	static void d(int indentation, string caller, Class variable) { SLOG.log(SLoggerLevels.DEBUG, indentation, caller, toString(variable)); }
	static void d(int indentation, string caller, typename variable) { SLOG.log(SLoggerLevels.DEBUG, indentation, caller, toString(variable)); }
	static void d(int indentation, string caller, float variable) { SLOG.log(SLoggerLevels.DEBUG, indentation, caller, toString(variable)); }
	static void d(int indentation, string caller, int variable) { SLOG.log(SLoggerLevels.DEBUG, indentation, caller, toString(variable)); }
	static void d(int indentation, string caller, bool variable) { SLOG.log(SLoggerLevels.DEBUG, indentation, caller, toString(variable)); }
	static void d(int indentation, string caller, vector variable) { SLOG.log(SLoggerLevels.DEBUG, indentation, caller, toString(variable)); }
	static void d(int indentation, string caller, string variable) { SLOG.log(SLoggerLevels.DEBUG, indentation, caller, toString(variable)); }

	static void d(int indentation, Class variable) { SLOG.log(SLoggerLevels.DEBUG, indentation, "", toString(variable)); }
	static void d(int indentation, typename variable) { SLOG.log(SLoggerLevels.DEBUG, indentation, "", toString(variable)); }
	static void d(int indentation, float variable) { SLOG.log(SLoggerLevels.DEBUG, indentation, "", toString(variable)); }
	static void d(int indentation, int variable) { SLOG.log(SLoggerLevels.DEBUG, indentation, "", toString(variable)); }
	static void d(int indentation, bool variable) { SLOG.log(SLoggerLevels.DEBUG, indentation, "", toString(variable)); }
	static void d(int indentation, vector variable) { SLOG.log(SLoggerLevels.DEBUG, indentation, "", toString(variable)); }
	static void d(int indentation, string variable) { SLOG.log(SLoggerLevels.DEBUG, indentation, "", toString(variable)); }

	static void d(string caller, Class variable) { SLOG.log(SLoggerLevels.DEBUG, 0, caller, toString(variable)); }
	static void d(string caller, typename variable) { SLOG.log(SLoggerLevels.DEBUG, 0, caller, toString(variable)); }
	static void d(string caller, float variable) { SLOG.log(SLoggerLevels.DEBUG, 0, caller, toString(variable)); }
	static void d(string caller, int variable) { SLOG.log(SLoggerLevels.DEBUG, 0, caller, toString(variable)); }
	static void d(string caller, bool variable) { SLOG.log(SLoggerLevels.DEBUG, 0, caller, toString(variable)); }
	static void d(string caller, vector variable) { SLOG.log(SLoggerLevels.DEBUG, 0, caller, toString(variable)); }
	static void d(string caller, string variable) { SLOG.log(SLoggerLevels.DEBUG, 0, caller, toString(variable)); }

	static void d(Class variable) { SLOG.log(SLoggerLevels.DEBUG, 0, "", toString(variable)); }
	static void d(typename variable) { SLOG.log(SLoggerLevels.DEBUG, 0, "", toString(variable)); }
	static void d(float variable) { SLOG.log(SLoggerLevels.DEBUG, 0, "", toString(variable)); }
	static void d(int variable) { SLOG.log(SLoggerLevels.DEBUG, 0, "", toString(variable)); }
	static void d(bool variable) { SLOG.log(SLoggerLevels.DEBUG, 0, "", toString(variable)); }
	static void d(vector variable) { SLOG.log(SLoggerLevels.DEBUG, 0, "", toString(variable)); }
	static void d(string variable) { SLOG.log(SLoggerLevels.DEBUG, 0, "", toString(variable)); }

	static void i(int indentation, string caller, Class variable) { SLOG.log(SLoggerLevels.INFO, indentation, caller, toString(variable)); }
	static void i(int indentation, string caller, typename variable) { SLOG.log(SLoggerLevels.INFO, indentation, caller, toString(variable)); }
	static void i(int indentation, string caller, float variable) { SLOG.log(SLoggerLevels.INFO, indentation, caller, toString(variable)); }
	static void i(int indentation, string caller, int variable) { SLOG.log(SLoggerLevels.INFO, indentation, caller, toString(variable)); }
	static void i(int indentation, string caller, bool variable) { SLOG.log(SLoggerLevels.INFO, indentation, caller, toString(variable)); }
	static void i(int indentation, string caller, vector variable) { SLOG.log(SLoggerLevels.INFO, indentation, caller, toString(variable)); }
	static void i(int indentation, string caller, string variable) { SLOG.log(SLoggerLevels.INFO, indentation, caller, toString(variable)); }

	static void i(int indentation, Class variable) { SLOG.log(SLoggerLevels.INFO, indentation, "", toString(variable)); }
	static void i(int indentation, typename variable) { SLOG.log(SLoggerLevels.INFO, indentation, "", toString(variable)); }
	static void i(int indentation, float variable) { SLOG.log(SLoggerLevels.INFO, indentation, "", toString(variable)); }
	static void i(int indentation, int variable) { SLOG.log(SLoggerLevels.INFO, indentation, "", toString(variable)); }
	static void i(int indentation, bool variable) { SLOG.log(SLoggerLevels.INFO, indentation, "", toString(variable)); }
	static void i(int indentation, vector variable) { SLOG.log(SLoggerLevels.INFO, indentation, "", toString(variable)); }
	static void i(int indentation, string variable) { SLOG.log(SLoggerLevels.INFO, indentation, "", toString(variable)); }

	static void i(string caller, Class variable) { SLOG.log(SLoggerLevels.INFO, 0, caller, toString(variable)); }
	static void i(string caller, typename variable) { SLOG.log(SLoggerLevels.INFO, 0, caller, toString(variable)); }
	static void i(string caller, float variable) { SLOG.log(SLoggerLevels.INFO, 0, caller, toString(variable)); }
	static void i(string caller, int variable) { SLOG.log(SLoggerLevels.INFO, 0, caller, toString(variable)); }
	static void i(string caller, bool variable) { SLOG.log(SLoggerLevels.INFO, 0, caller, toString(variable)); }
	static void i(string caller, vector variable) { SLOG.log(SLoggerLevels.INFO, 0, caller, toString(variable)); }
	static void i(string caller, string variable) { SLOG.log(SLoggerLevels.INFO, 0, caller, toString(variable)); }

	static void i(Class variable) { SLOG.log(SLoggerLevels.INFO, 0, "", toString(variable)); }
	static void i(typename variable) { SLOG.log(SLoggerLevels.INFO, 0, "", toString(variable)); }
	static void i(float variable) { SLOG.log(SLoggerLevels.INFO, 0, "", toString(variable)); }
	static void i(int variable) { SLOG.log(SLoggerLevels.INFO, 0, "", toString(variable)); }
	static void i(bool variable) { SLOG.log(SLoggerLevels.INFO, 0, "", toString(variable)); }
	static void i(vector variable) { SLOG.log(SLoggerLevels.INFO, 0, "", toString(variable)); }
	static void i(string variable) { SLOG.log(SLoggerLevels.INFO, 0, "", toString(variable)); }

	static void w(int indentation, string caller, Class variable) { SLOG.log(SLoggerLevels.WARNING, indentation, caller, toString(variable)); }
	static void w(int indentation, string caller, typename variable) { SLOG.log(SLoggerLevels.WARNING, indentation, caller, toString(variable)); }
	static void w(int indentation, string caller, float variable) { SLOG.log(SLoggerLevels.WARNING, indentation, caller, toString(variable)); }
	static void w(int indentation, string caller, int variable) { SLOG.log(SLoggerLevels.WARNING, indentation, caller, toString(variable)); }
	static void w(int indentation, string caller, bool variable) { SLOG.log(SLoggerLevels.WARNING, indentation, caller, toString(variable)); }
	static void w(int indentation, string caller, vector variable) { SLOG.log(SLoggerLevels.WARNING, indentation, caller, toString(variable)); }
	static void w(int indentation, string caller, string variable) { SLOG.log(SLoggerLevels.WARNING, indentation, caller, toString(variable)); }

	static void w(int indentation, Class variable) { SLOG.log(SLoggerLevels.WARNING, indentation, "", toString(variable)); }
	static void w(int indentation, typename variable) { SLOG.log(SLoggerLevels.WARNING, indentation, "", toString(variable)); }
	static void w(int indentation, float variable) { SLOG.log(SLoggerLevels.WARNING, indentation, "", toString(variable)); }
	static void w(int indentation, int variable) { SLOG.log(SLoggerLevels.WARNING, indentation, "", toString(variable)); }
	static void w(int indentation, bool variable) { SLOG.log(SLoggerLevels.WARNING, indentation, "", toString(variable)); }
	static void w(int indentation, vector variable) { SLOG.log(SLoggerLevels.WARNING, indentation, "", toString(variable)); }
	static void w(int indentation, string variable) { SLOG.log(SLoggerLevels.WARNING, indentation, "", toString(variable)); }

	static void w(string caller, Class variable) { SLOG.log(SLoggerLevels.WARNING, 0, caller, toString(variable)); }
	static void w(string caller, typename variable) { SLOG.log(SLoggerLevels.WARNING, 0, caller, toString(variable)); }
	static void w(string caller, float variable) { SLOG.log(SLoggerLevels.WARNING, 0, caller, toString(variable)); }
	static void w(string caller, int variable) { SLOG.log(SLoggerLevels.WARNING, 0, caller, toString(variable)); }
	static void w(string caller, bool variable) { SLOG.log(SLoggerLevels.WARNING, 0, caller, toString(variable)); }
	static void w(string caller, vector variable) { SLOG.log(SLoggerLevels.WARNING, 0, caller, toString(variable)); }
	static void w(string caller, string variable) { SLOG.log(SLoggerLevels.WARNING, 0, caller, toString(variable)); }

	static void w(Class variable) { SLOG.log(SLoggerLevels.WARNING, 0, "", toString(variable)); }
	static void w(typename variable) { SLOG.log(SLoggerLevels.WARNING, 0, "", toString(variable)); }
	static void w(float variable) { SLOG.log(SLoggerLevels.WARNING, 0, "", toString(variable)); }
	static void w(int variable) { SLOG.log(SLoggerLevels.WARNING, 0, "", toString(variable)); }
	static void w(bool variable) { SLOG.log(SLoggerLevels.WARNING, 0, "", toString(variable)); }
	static void w(vector variable) { SLOG.log(SLoggerLevels.WARNING, 0, "", toString(variable)); }
	static void w(string variable) { SLOG.log(SLoggerLevels.WARNING, 0, "", toString(variable)); }

	static void e(int indentation, string caller, Class variable) { SLOG.log(SLoggerLevels.ERROR, indentation, caller, toString(variable)); }
	static void e(int indentation, string caller, typename variable) { SLOG.log(SLoggerLevels.ERROR, indentation, caller, toString(variable)); }
	static void e(int indentation, string caller, float variable) { SLOG.log(SLoggerLevels.ERROR, indentation, caller, toString(variable)); }
	static void e(int indentation, string caller, int variable) { SLOG.log(SLoggerLevels.ERROR, indentation, caller, toString(variable)); }
	static void e(int indentation, string caller, bool variable) { SLOG.log(SLoggerLevels.ERROR, indentation, caller, toString(variable)); }
	static void e(int indentation, string caller, vector variable) { SLOG.log(SLoggerLevels.ERROR, indentation, caller, toString(variable)); }
	static void e(int indentation, string caller, string variable) { SLOG.log(SLoggerLevels.ERROR, indentation, caller, toString(variable)); }

	static void e(int indentation, Class variable) { SLOG.log(SLoggerLevels.ERROR, indentation, "", toString(variable)); }
	static void e(int indentation, typename variable) { SLOG.log(SLoggerLevels.ERROR, indentation, "", toString(variable)); }
	static void e(int indentation, float variable) { SLOG.log(SLoggerLevels.ERROR, indentation, "", toString(variable)); }
	static void e(int indentation, int variable) { SLOG.log(SLoggerLevels.ERROR, indentation, "", toString(variable)); }
	static void e(int indentation, bool variable) { SLOG.log(SLoggerLevels.ERROR, indentation, "", toString(variable)); }
	static void e(int indentation, vector variable) { SLOG.log(SLoggerLevels.ERROR, indentation, "", toString(variable)); }
	static void e(int indentation, string variable) { SLOG.log(SLoggerLevels.ERROR, indentation, "", toString(variable)); }

	static void e(string caller, Class variable) { SLOG.log(SLoggerLevels.ERROR, 0, caller, toString(variable)); }
	static void e(string caller, typename variable) { SLOG.log(SLoggerLevels.ERROR, 0, caller, toString(variable)); }
	static void e(string caller, float variable) { SLOG.log(SLoggerLevels.ERROR, 0, caller, toString(variable)); }
	static void e(string caller, int variable) { SLOG.log(SLoggerLevels.ERROR, 0, caller, toString(variable)); }
	static void e(string caller, bool variable) { SLOG.log(SLoggerLevels.ERROR, 0, caller, toString(variable)); }
	static void e(string caller, vector variable) { SLOG.log(SLoggerLevels.ERROR, 0, caller, toString(variable)); }
	static void e(string caller, string variable) { SLOG.log(SLoggerLevels.ERROR, 0, caller, toString(variable)); }

	static void e(Class variable) { SLOG.log(SLoggerLevels.ERROR, 0, "", toString(variable)); }
	static void e(typename variable) { SLOG.log(SLoggerLevels.ERROR, 0, "", toString(variable)); }
	static void e(float variable) { SLOG.log(SLoggerLevels.ERROR, 0, "", toString(variable)); }
	static void e(int variable) { SLOG.log(SLoggerLevels.ERROR, 0, "", toString(variable)); }
	static void e(bool variable) { SLOG.log(SLoggerLevels.ERROR, 0, "", toString(variable)); }
	static void e(vector variable) { SLOG.log(SLoggerLevels.ERROR, 0, "", toString(variable)); }
	static void e(string variable) { SLOG.log(SLoggerLevels.ERROR, 0, "", toString(variable)); }

	static void c(int indentation, string caller, Class variable) { SLOG.log(SLoggerLevels.CRITICAL, indentation, caller, toString(variable)); }
	static void c(int indentation, string caller, typename variable) { SLOG.log(SLoggerLevels.CRITICAL, indentation, caller, toString(variable)); }
	static void c(int indentation, string caller, float variable) { SLOG.log(SLoggerLevels.CRITICAL, indentation, caller, toString(variable)); }
	static void c(int indentation, string caller, int variable) { SLOG.log(SLoggerLevels.CRITICAL, indentation, caller, toString(variable)); }
	static void c(int indentation, string caller, bool variable) { SLOG.log(SLoggerLevels.CRITICAL, indentation, caller, toString(variable)); }
	static void c(int indentation, string caller, vector variable) { SLOG.log(SLoggerLevels.CRITICAL, indentation, caller, toString(variable)); }
	static void c(int indentation, string caller, string variable) { SLOG.log(SLoggerLevels.CRITICAL, indentation, caller, toString(variable)); }

	static void c(int indentation, Class variable) { SLOG.log(SLoggerLevels.CRITICAL, indentation, "", toString(variable)); }
	static void c(int indentation, typename variable) { SLOG.log(SLoggerLevels.CRITICAL, indentation, "", toString(variable)); }
	static void c(int indentation, float variable) { SLOG.log(SLoggerLevels.CRITICAL, indentation, "", toString(variable)); }
	static void c(int indentation, int variable) { SLOG.log(SLoggerLevels.CRITICAL, indentation, "", toString(variable)); }
	static void c(int indentation, bool variable) { SLOG.log(SLoggerLevels.CRITICAL, indentation, "", toString(variable)); }
	static void c(int indentation, vector variable) { SLOG.log(SLoggerLevels.CRITICAL, indentation, "", toString(variable)); }
	static void c(int indentation, string variable) { SLOG.log(SLoggerLevels.CRITICAL, indentation, "", toString(variable)); }

	static void c(string caller, Class variable) { SLOG.log(SLoggerLevels.CRITICAL, 0, caller, toString(variable)); }
	static void c(string caller, typename variable) { SLOG.log(SLoggerLevels.CRITICAL, 0, caller, toString(variable)); }
	static void c(string caller, float variable) { SLOG.log(SLoggerLevels.CRITICAL, 0, caller, toString(variable)); }
	static void c(string caller, int variable) { SLOG.log(SLoggerLevels.CRITICAL, 0, caller, toString(variable)); }
	static void c(string caller, bool variable) { SLOG.log(SLoggerLevels.CRITICAL, 0, caller, toString(variable)); }
	static void c(string caller, vector variable) { SLOG.log(SLoggerLevels.CRITICAL, 0, caller, toString(variable)); }
	static void c(string caller, string variable) { SLOG.log(SLoggerLevels.CRITICAL, 0, caller, toString(variable)); }

	static void c(Class variable) { SLOG.log(SLoggerLevels.CRITICAL, 0, "", toString(variable)); }
	static void c(typename variable) { SLOG.log(SLoggerLevels.CRITICAL, 0, "", toString(variable)); }
	static void c(float variable) { SLOG.log(SLoggerLevels.CRITICAL, 0, "", toString(variable)); }
	static void c(int variable) { SLOG.log(SLoggerLevels.CRITICAL, 0, "", toString(variable)); }
	static void c(bool variable) { SLOG.log(SLoggerLevels.CRITICAL, 0, "", toString(variable)); }
	static void c(vector variable) { SLOG.log(SLoggerLevels.CRITICAL, 0, "", toString(variable)); }
	static void c(string variable) { SLOG.log(SLoggerLevels.CRITICAL, 0, "", toString(variable)); }


	//@formatter:on

}
