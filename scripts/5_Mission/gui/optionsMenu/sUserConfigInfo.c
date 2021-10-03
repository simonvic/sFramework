class SUserConfigInfo : Managed {
	
	protected string m_title;
	protected string m_description;
	protected SConstraintBase m_constraints;
		
	void SUserConfigInfo(string title, string description, SConstraintBase constraints = null) {
		m_title = title;
		m_description = description;
		m_constraints = constraints;				
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
	
	void setConstraints(SConstraintBase constraints) {
		m_constraints = constraints;
	}
	
	void removeConstraints() {
		delete m_constraints;
	}
	
	string getWarningMessage() {
		if (m_constraints) return m_constraints.getMessage() + "\n\n" + m_constraints.toString();
		
		return string.Empty;
	}
	
	bool isConstrained() {
		return m_constraints && m_constraints.isEnabled();
	}
	
	
	
}