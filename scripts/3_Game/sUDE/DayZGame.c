modded class DayZGame {

	override void OnUpdate(bool doSim, float timeslice) {
		super.OnUpdate(doSim, timeslice);
		SLOG.onUpdate(timeslice);
	}

	private void ~DayZGame() {
		if (SLOG.headerPrinted) {
			SLOG.printFooter();
		}
	}
}
