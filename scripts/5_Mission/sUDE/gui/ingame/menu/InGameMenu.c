modded class InGameMenu {

	protected ref SCOAnimationLogo m_coLogo = new SCOAnimationLogo();
	
	void InGameMenu() {
		m_coLogo.activate();
	}
	
	void ~InGameMenu() {
		m_coLogo.deactivate();
	}
}