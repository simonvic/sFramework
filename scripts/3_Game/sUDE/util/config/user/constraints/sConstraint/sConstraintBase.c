class SConstraintBase : Managed {
	
	protected bool enabled = false;
	protected string message = "#STR_SUDE_LAYOUT_OPTIONS_CONSTRAINED";
				
	bool isEnabled() {
		return enabled;
	}
	
	void setEnabled(bool enable) {
		enabled = enable;
	}
	
	string getMessage() {
		return message;
	}
	
	//@todo move this, it doesn't belong here
	string toString();
	
	void setMessage(string msg) {
		message = msg;
	}
}