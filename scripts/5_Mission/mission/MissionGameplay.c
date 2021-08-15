modded class MissionGameplay{
	
	void MissionGameplay() {
		PPEManager.onInit();		
	}
	
	void ~MissionGameplay(){
		SCameraOverlaysManager.getInstance().deactivateAll();
		PPEManager.deactivateAll();
		PPEManager.applyDefault();
	}
	
}