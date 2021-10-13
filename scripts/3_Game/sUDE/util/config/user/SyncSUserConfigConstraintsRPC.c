class SyncSUserConfigConstraintsRPC : Managed {
	
	protected ref ScriptRPC m_rpc;
	
	void SyncSUserConfigConstraintsRPC() {
		m_rpc = new ScriptRPC();
		setup();
	}
	
	void setup() {
		m_rpc.Reset();
		onSetup(m_rpc);
	}
	
	protected void onSetup(ParamsWriteContext ctx);
	
	ScriptRPC getScriptRPC() {
		return m_rpc;
	}
	
	void sendTo(PlayerIdentity playerIdentity = null, Object target = null) {
		m_rpc.Send(target, SRPCIDs.SYNC_USER_CONFIG_CONSTRAINTS, true, playerIdentity);
	}
}