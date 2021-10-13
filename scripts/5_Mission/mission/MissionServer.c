modded class MissionServer {
	
	override void OnEvent(EventType eventTypeId, Param params) {
		super.OnEvent(eventTypeId, params);	
			
		switch (eventTypeId) {
			
			case ClientNewEventTypeID:						
			case ClientReadyEventTypeID:
			
			ClientReadyEventParams readyParams = ClientReadyEventParams.Cast(params);
			PlayerBase player;
			if (readyParams && Class.CastTo(player, readyParams.param2)) {
				syncSUserConfigConstraints(player);
			}
			
			break;
			
		}
	}
	
	protected void syncSUserConfigConstraints(PlayerBase player) {
		SUserConfigConstraints.getInstance().syncWith(player.GetIdentity());
	}
	
}