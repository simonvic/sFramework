class SConstraintBase : Managed {
	
	protected bool enabled = false;
	protected string message = " ( #STR_SUDE_LAYOUT_OPTIONS_LOCKED )";
				
	bool isEnabled() {
		return enabled;
	}
	
	void setEnabled(bool enable) {
		enabled = enable;
	}
	
	string getMessage() {
		return message;
	}
	
	void setMessage(string msg) {
		message = msg;
	}
}