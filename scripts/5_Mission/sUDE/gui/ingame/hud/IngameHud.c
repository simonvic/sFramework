modded class IngameHud {

	protected Widget m_overlaysRoot;
	protected SCameraOverlaysManager m_cameraOverlayManager;
	
	void IngameHud() {
		m_cameraOverlayManager = SCameraOverlaysManager.getInstance();
		m_overlaysRoot = GetGame().GetWorkspace().CreateWidgets( getOverlayLayoutName(), m_HudPanelWidget );
	}
	
	string getOverlayLayoutName() {
		return "MyMODS/sFramework/GUI/layouts/ingame/hud/camera_overlays_root.layout";
	}
	
	override void Update( float timeslice ) {
		super.Update( timeslice );
		m_cameraOverlayManager.onUpdate(timeslice, m_overlaysRoot);
	}
	
	override void RefreshHudVisibility() {
		super.RefreshHudVisibility();
		m_cameraOverlayManager.setIngameHUDVisibility(!m_HudHidePlayer);
	}


}