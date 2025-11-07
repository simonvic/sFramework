class SFileHelper {
		
	
	/**
	*	@brief Touch a file. If it doesn't exist, create it, and if necessary create its parent directories
	*	 @param path - File path to touch
	*/
	static void touch(string path) {
		if (FileExist(path)) return;

		//Trim file name		
		TStringArray dirs = new TStringArray;
		path.Split("\\",dirs);
		dirs.Remove(dirs.Count() - 1);
		string parentDir = "";
		for (int i = 0; i< dirs.Count(); i++) {
			parentDir += dirs[i] + "\\";
		}
		
		// Make parent folder
		mkdir(parentDir);
		
		FileHandle file = OpenFile(path, FileMode.WRITE);
		if (file != 0) {
			FPrint(file, "");
			CloseFile(file);
		}
	}
	
	/**
	*	@brief Make directory, and its parents if necessary
	*	 @param path - Directory path to create
	*/
	static void mkdir(string path) {
		TStringArray dirs = new TStringArray;
		path.Split("\\",dirs);
		string temp = dirs[0];
		MakeDirectory(temp);
		for (int i = 0; i<dirs.Count() - 1; i++) {
			temp += "\\" + dirs[i+1];
			MakeDirectory(temp);
		}
	}
	
	/**
	*	@brief Read the file
	*	 @return content of file, empty string on error
	*/
	static string cat(string path) {
		if (!FileExist(path)) {
			SLOG.e("SFileHelper::cat", "The file [ " + path + " ] doesn't exists");
			return string.Empty; 
		}
				
		FileHandle handle = OpenFile(path, FileMode.READ);
		if (handle == 0) {
			SLOG.e("SFileHelper::cat", "Error when reading [ " + path + " ]");
			return string.Empty;
		}
		
		string fileContent;
		string lineContent;
		while (FGets(handle, lineContent) >= 0) {
			fileContent += lineContent;
		}
		CloseFile(handle);
		return fileContent;
	}
	
	/**
	*	@brief Echo to the file
	*	 @param data \p string - data to echo
	*	 @param path \p string - path of file to be echoed to
	*/
	static void echo(string data, string path) {
		FileHandle handle = OpenFile(path, FileMode.WRITE);
		if (handle == 0) {
			SLOG.e("SFileHelper::echo", "Error when opening [ " + path + " ]");
			return;
		}
		FPrint(handle, data);
		CloseFile(handle);
	}
	
}
