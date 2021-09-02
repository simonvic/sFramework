modded class MissionGameplay{
	
	void MissionGameplay() {
		SPPEManager.onInit();		
	}
	
	void ~MissionGameplay(){
		SCameraOverlaysManager.getInstance().deactivateAll();
		SPPEManager.deactivateAll();
		SPPEManager.applyDefault();
	}
	
}