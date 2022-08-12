class SUserConfigOptionInfo : Managed {
	
	protected string m_title;
	protected string m_description;
		
	void SUserConfigOptionInfo(string title, string description) {
		m_title = title;
		m_description = description;
	}
	
	string getTitle() {
		return m_title;
	}
	
	void setTitle(string title) {
		m_title = title;
	}
	
	string getDescription() {
		return m_description;
	}
	
	void setDescription(string description) {
		m_description = description;
	}	
	
}