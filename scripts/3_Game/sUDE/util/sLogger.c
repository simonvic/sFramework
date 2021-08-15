enum SLoggerLevels{
	DEBUG = 0,
	INFO,
	WARNING,
	ERROR,
	CRITICAL
}

class SLog{
		
	static bool overrideEnabled = true;
	static bool filePrintEnabled = true;
	
	/**
	*	$profile
	*	$saves
	*/
	static const string LOG_PATH = "$profile:sUDE\\logs";
	
	/**
	*	%1 : level
	*	%2 : indentation
	*	%3 : caller
	*	%4 : text
	*	%5 : timestamp
	*	%6 : caller prefix
	*	%7 : text prefix
	*/
	static const string PRINT_FORMAT = "%5 [%1] %2%6%3%7%4";
	
	static const string CALLER_PREFIX = "::";
	static const string TEXT_PREFIX = " : ";
	
	/**
	*	%1 : year
	*	%2 : month
	*	%3 : day
	*	%4 : hour
	*	%5 : minutes
	*	%6 : seconds
	*/
	static const string DATE_PRINT_FORMAT = "%4:%5:%6";
	
	/**
	*	%1 : typename
	*	%2 : variable value
	*/
	static const string VARIABLE_PRINT_FORMAT = "< %1 > %2";
	
	/**
	* @brief Print a debug message to script log
	* 	@param text \p string Text to print
	* 	@param caller \p string Caller
	* 	@param indentation \p int Indentation of the print
	* 	@param enabled \p bool Enable this print
	*/
	static void d(string text, string caller = "", int indentation = 0, bool enabled = true){
		SLog.log(text,SLoggerLevels.DEBUG,caller,indentation, enabled);
	}
	
	static void d(Class variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.d(toString(variable), caller, indentation, enabled);
	}
	
	static void d(typename variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.d(toString(variable), caller, indentation, enabled);
	}
	
	static void d(float variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.d(toString(variable), caller, indentation, enabled);
	}
	
	static void d(int variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.d(toString(variable), caller, indentation, enabled);
	}
	
	static void d(bool variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.d(toString(variable), caller, indentation, enabled);
	}
	
	static void d(vector variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.d(toString(variable), caller, indentation, enabled);
	}
	
	/**
	* @brief Print an informative message to script log
	* 	@param text \p string Text to print
	* 	@param caller \p string Caller
	* 	@param indentation \p int Indentation of the print
	* 	@param enabled \p bool Enable this print
	*/
	static void i(string text,string caller = "", int indentation = 0, bool enabled = true){
		SLog.log(text,SLoggerLevels.INFO,caller,indentation, enabled);
	}
	
	static void i(Class variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.i(toString(variable), caller, indentation, enabled);
	}
	
	static void i(typename variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.i(toString(variable), caller, indentation, enabled);
	}
	
	static void i(float variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.i(toString(variable), caller, indentation, enabled);
	}
	
	static void i(int variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.i(toString(variable), caller, indentation, enabled);
	}
	
	static void i(bool variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.i(toString(variable), caller, indentation, enabled);
	}
	
	static void i(vector variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.i(toString(variable), caller, indentation, enabled);
	}
	
		
	/**
	* @brief Print a warning message to script log
	* 	@param text \p string Text to print
	* 	@param caller \p string Caller
	* 	@param indentation \p int Indentation of the print
	* 	@param enabled \p bool Enable this print
	*/
	static void w(string text,string caller = "", int indentation = 0, bool enabled = true){
		SLog.log(text,SLoggerLevels.WARNING,caller,indentation, enabled);
	}
	
	static void w(Class variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.w(toString(variable), caller, indentation, enabled);
	}
	
	static void w(typename variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.w(toString(variable), caller, indentation, enabled);
	}
	
	static void w(float variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.w(toString(variable), caller, indentation, enabled);
	}
	
	static void w(int variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.w(toString(variable), caller, indentation, enabled);
	}
	
	static void w(bool variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.w(toString(variable), caller, indentation, enabled);
	}
	
	static void w(vector variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.w(toString(variable), caller, indentation, enabled);
	}
	
	/**
	* @brief Print an eerror message to script log
	* 	@param text \p string Text to print
	* 	@param caller \p string Caller
	*	@param indentation \p int Indentation of the print
	* 	@param enabled \p bool Enable this print
	*/
	static void e(string text,string caller = "", int indentation = 0, bool enabled = true){
		SLog.log(text,SLoggerLevels.ERROR,caller,indentation, enabled);
	}
	
	static void e(Class variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.e(toString(variable), caller, indentation, enabled);
	}
	
	static void e(typename variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.e(toString(variable), caller, indentation, enabled);
	}
	
	static void e(float variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.e(toString(variable), caller, indentation, enabled);
	}
	
	static void e(int variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.e(toString(variable), caller, indentation, enabled);
	}
	
	static void e(bool variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.e(toString(variable), caller, indentation, enabled);
	}
	
	static void e(vector variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.e(toString(variable), caller, indentation, enabled);
	}
	
	/**
	* @brief Print a critical e message to script log
	* 	@param text \p string Text to print
	* 	@param caller \p string Caller
	* 	@param indentation \p int Indentation of the print
	* 	@param enabled \p bool Enable this print
	*/
	static void c(string text, string caller = "", int indentation = 0, bool enabled = true){
		SLog.log(text,SLoggerLevels.CRITICAL,caller,indentation, enabled);
	}
	
	static void c(Class variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.c(toString(variable), caller, indentation, enabled);
	}
	
	static void c(typename variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.c(toString(variable), caller, indentation, enabled);
	}
	
	static void c(float variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.c(toString(variable), caller, indentation, enabled);
	}
	
	static void c(int variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.c(toString(variable), caller, indentation, enabled);
	}
	
	static void c(bool variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.c(toString(variable), caller, indentation, enabled);
	}
	
	static void c(vector variable, string caller = "", int indentation = 0, bool enabled = true){
		SLog.c(toString(variable), caller, indentation, enabled);
	}
	
	
	/**
	* @brief Print a message to script log
	* 	@param text \p string Text to print
	* 	@param level \p int Level type of the message
	* 	@param caller \p string Caller
	* 	@param indentation \p int Indentation of the print
	* 	@param enabled \p bool Enable this print
	*/
	static void log(string text, SLoggerLevels level = SLoggerLevels.INFO, string caller = "", int indentation = 0, bool enabled = true){
		if(overrideEnabled && enabled){
			string output = buildOutput(text, level, caller, indentation);
			PrintFormat(output);
			if(filePrintEnabled){
				SLog.logToFile(output);
			}
		}
	}
	
	/**
	*	@brief Build a string with formatted output
	* 	 @return formatted string
	*/
	static string buildOutput(string text, SLoggerLevels level, string caller, int indentation){
		string callerPrefix;
		string textPrefix;
		if(caller == string.Empty) {
			callerPrefix = string.Empty;
			textPrefix = string.Empty;
		}else{
			callerPrefix = CALLER_PREFIX;
			textPrefix = TEXT_PREFIX;
		}
		return string.Format(PRINT_FORMAT, getLevel(level), getIndentation(indentation), caller, text, buildTimestamp(), callerPrefix, textPrefix);
	}
	
	/**
	*	@brief Build a string containing current timestamp
	* 	 @return formatted string
	*/
	static string buildTimestamp(string format = DATE_PRINT_FORMAT){
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;
		GetYearMonthDayUTC(year,month,day);
		GetHourMinuteSecondUTC(hour, minute, second);
		string monthS = ""+month;
		string dayS = ""+day;
		string hourS = ""+hour;
		string minuteS = ""+minute;
		string secondS  = ""+second;
		if(month < 10) monthS = "0"+monthS;
		if(day < 10) dayS = "0"+dayS;
		if(hour < 10) hourS = "0"+hourS;
		if(minute < 10) minuteS = "0"+minuteS;
		if(second < 10) secondS = "0"+secondS;
		return string.Format(format, year, monthS, dayS, hourS, minuteS, secondS);
	}
	
	static void log(Class variable, SLoggerLevels level = SLoggerLevels.INFO, string caller = "", int indentation = 0, bool enabled = true){
		SLog.log(toString(variable), level, caller, indentation, enabled);
	}
	
	static void log(typename variable, SLoggerLevels level = SLoggerLevels.INFO, string caller = "", int indentation = 0, bool enabled = true){
		SLog.log(toString(variable), level,  caller, indentation, enabled);
	}
	
	static void log(float variable, SLoggerLevels level = SLoggerLevels.INFO, string caller = "", int indentation = 0, bool enabled = true){
		SLog.log(toString(variable), level,  caller, indentation, enabled);
	}
	
	static void log(int variable, SLoggerLevels level = SLoggerLevels.INFO, string caller = "", int indentation = 0, bool enabled = true){
		SLog.log(toString(variable), level,  caller, indentation, enabled);
	}
	
	static void log(bool variable, SLoggerLevels level = SLoggerLevels.INFO, string caller = "", int indentation = 0, bool enabled = true){
		SLog.log(toString(variable), level,  caller, indentation, enabled);
	}
	
	static void log(vector variable, SLoggerLevels level = SLoggerLevels.INFO, string caller = "", int indentation = 0, bool enabled = true){
		SLog.log(toString(variable), level,  caller, indentation, enabled);
	}
	
	private static void logToFile(string text){
		int year;
		int month;
		int day;
		GetYearMonthDayUTC(year,month,day);
		string today = ""+year+"-"+month+"-"+day;
		string filePath = LOG_PATH+"\\"+today+".slog";
		
		if(!FileExist(filePath)){
			SFileHelper.touch(filePath);
			text = buildHeader() + text;
		}
		
		FileHandle file = OpenFile(filePath, FileMode.APPEND);
		if (file != 0){
			FPrintln(file, text);
		}
		CloseFile(file);
		
	}
		
	static string toString(Class variable){
		return string.Format(VARIABLE_PRINT_FORMAT, variable.Type().ToString(), variable.ToString());
	}
	
	static string toString(typename variable){
		return string.Format(VARIABLE_PRINT_FORMAT, "typename", variable.ToString());
	}
	
	static string toString(float variable){
		return string.Format(VARIABLE_PRINT_FORMAT, "float", variable.ToString());
	}
	
	static string toString(int variable){
		return string.Format(VARIABLE_PRINT_FORMAT, "int", variable.ToString());
	}
	
	static string toString(bool variable){
		return string.Format(VARIABLE_PRINT_FORMAT, "bool", variable.ToString());
	}
	
	static string toString(vector variable){
		return string.Format(VARIABLE_PRINT_FORMAT, "vector", variable.ToString());
	}	
	
	
	static string buildHeader(){
		string playerName;
		float avgFPS = -1;
		float tickTime = -1;
		string worldName;
		string version;
		string profileName;
		string machineName;
		if(GetGame()){
			GetGame().GetPlayerName(playerName);			
			avgFPS = GetGame().GetFps();			
			tickTime = GetGame().GetTickTime();
			GetGame().GetWorldName(worldName);
			GetGame().GetVersion(version);
		}
		profileName = GetProfileName();
		machineName = GetMachineName();
		
		string mods;
		if(GetCLIParam("mod", mods)){
			TStringArray modList = new TStringArray;
			mods.Split(";",modList);
			mods = "";
			foreach(string mod : modList){
				mods += "\n                  \t- " + mod;
			}
		}
		
		string header;
		header += string.Format("====================================================================\n");
		header += string.Format("                      %1 \n",buildTimestamp("%1/%2/%3 %4:%5:%6"));
		header += string.Format("--------------------------------------------------------------------\n\n");
		header += string.Format("                  Profile name  : %1\n", profileName);
		header += string.Format("                  Machine name  : %1\n", machineName);
		header += string.Format("                  Player name   : %1\n", playerName);
		header += string.Format("                  Average FPS   : %1\n", avgFPS);
		header += string.Format("                  Tick time     : %1\n", tickTime);
		header += string.Format("                  World name    : %1\n", worldName);
		header += string.Format("                  Game version  : %1\n", version);
		header += string.Format("                  Mods          : %1\n", mods);
		header += string.Format("--------------------------------------------------------------------\n\n");
		
		return header;
	}
	
	private static string getIndentation(int indentation){
		string temp = "";
		for(int i=0; i<indentation; i++){
			temp += "│\t";
		}
		return temp;
	}
	
	private static string getLevel(SLoggerLevels level){
		switch(level){
			case SLoggerLevels.DEBUG: return "dd";
			case SLoggerLevels.INFO: return "ii";
			case SLoggerLevels.WARNING: return "ww";
			case SLoggerLevels.ERROR: return "ee";
			case SLoggerLevels.CRITICAL: return "cc";
		}
		return "xx";
	}
	
}

//can we have an amen for OOP ?