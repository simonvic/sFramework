modded class MissionServer {
	
	override void OnInit() {
		super.OnInit();
		SUserConfigConstraints.getInstance().onPreload();
	}
	
	override PlayerBase OnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx) {
		PlayerBase player = super.OnClientNewEvent(identity, pos, ctx);
		syncSUserConfigConstraints(identity);
		return player;
	}
	
	override void OnClientReadyEvent(PlayerIdentity identity, PlayerBase player) {
		super.OnClientReadyEvent(identity, player);
		syncSUserConfigConstraints(identity);
	}
	
	protected void syncSUserConfigConstraints(PlayerIdentity playerIdentity) {
		SUserConfigConstraints.getInstance().syncWith(playerIdentity);
	}
	
}