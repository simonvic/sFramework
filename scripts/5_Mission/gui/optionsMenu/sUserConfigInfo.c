class SUserConfigInfo : Managed {
	
	protected string m_title;
	protected string m_description;
	protected SConstraintBase m_constraint;
		
	void SUserConfigInfo(string title, string description, SConstraintBase constraint = null) {
		m_title = title;
		m_description = description;
		m_constraint = constraint;				
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
	
	void setConstraints(SConstraintBase constraint) {
		m_constraint = constraint;
	}
	
	void removeConstraints() {
		delete m_constraint;
	}
	
	string getWarningMessage() {
		if (!m_constraint) return string.Empty;
		
		return m_constraint.getMessage() + "\n\n" + m_constraint.toString();
	}
	
	bool isConstrained() {
		return m_constraint && m_constraint.isEnabled();
	}
	
	
	
}