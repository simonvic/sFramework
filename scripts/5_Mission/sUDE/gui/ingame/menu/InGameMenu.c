modded class InGameMenu{

	protected ref SCOAnimationLogo m_coLogo = new SCOAnimationLogo();
	
	void InGameMenu(){
		SCameraOverlaysManager.getInstance().activate(m_coLogo);
	}
	
	void ~InGameMenu(){
		SCameraOverlaysManager.getInstance().deactivate(m_coLogo);
	}
}