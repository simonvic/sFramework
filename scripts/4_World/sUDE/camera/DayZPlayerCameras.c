modded class DayZPlayerCameras {

	static const float DEFAULT_UNREGISTERED_TIME = 0.3;

	static const float TIME_TO_ADS = 0.1;
	static const float TIME_FROM_ADS = 0.1;
	static const float TIME_3RD = 0.2;
	static const float TIME_BACKUP_SIGHT = 0.1;

	override static void RegisterCameras(DayZPlayerType pType) {
		super.RegisterCameras(pType);

		////////////////////////////////////////////////////////////////////////
		// ENTERING: IRONSIGHT
		RegisterTransitionTime(DAYZCAMERA_1ST,            DAYZCAMERA_IRONSIGHTS,     TIME_TO_ADS,            false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC_RAISED, DAYZCAMERA_IRONSIGHTS,     TIME_TO_ADS + TIME_3RD, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_CRO_RAISED, DAYZCAMERA_IRONSIGHTS,     TIME_TO_ADS + TIME_3RD, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_PRO_RAISED, DAYZCAMERA_IRONSIGHTS,     TIME_TO_ADS + TIME_3RD, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC,        DAYZCAMERA_IRONSIGHTS,     TIME_TO_ADS + TIME_3RD, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_CRO,        DAYZCAMERA_IRONSIGHTS,     TIME_TO_ADS + TIME_3RD, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_PRO,        DAYZCAMERA_IRONSIGHTS,     TIME_TO_ADS + TIME_3RD, false);

		////////////////////////////////////////////////////////////////////////
		// EXITING: IRONSIGHT
		RegisterTransitionTime(DAYZCAMERA_IRONSIGHTS,     DAYZCAMERA_1ST,            TIME_FROM_ADS,          false);
		RegisterTransitionTime(DAYZCAMERA_IRONSIGHTS,     DAYZCAMERA_3RD_ERC_RAISED, TIME_FROM_ADS,          false);
		RegisterTransitionTime(DAYZCAMERA_IRONSIGHTS,     DAYZCAMERA_3RD_CRO_RAISED, TIME_FROM_ADS,          false);
		RegisterTransitionTime(DAYZCAMERA_IRONSIGHTS,     DAYZCAMERA_3RD_PRO_RAISED, TIME_FROM_ADS,          false);
		RegisterTransitionTime(DAYZCAMERA_IRONSIGHTS,     DAYZCAMERA_3RD_ERC,        TIME_FROM_ADS,          false);
		RegisterTransitionTime(DAYZCAMERA_IRONSIGHTS,     DAYZCAMERA_3RD_CRO,        TIME_FROM_ADS,          false);
		RegisterTransitionTime(DAYZCAMERA_IRONSIGHTS,     DAYZCAMERA_3RD_PRO,        TIME_FROM_ADS,          false);

		////////////////////////////////////////////////////////////////////////
		// ENTERING: OPTICS
		RegisterTransitionTime(DAYZCAMERA_1ST,            DAYZCAMERA_OPTICS,         TIME_TO_ADS,            false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC_RAISED, DAYZCAMERA_OPTICS,         TIME_TO_ADS + TIME_3RD, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_CRO_RAISED, DAYZCAMERA_OPTICS,         TIME_TO_ADS + TIME_3RD, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_PRO_RAISED, DAYZCAMERA_OPTICS,         TIME_TO_ADS + TIME_3RD, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_ERC,        DAYZCAMERA_OPTICS,         TIME_TO_ADS + TIME_3RD, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_CRO,        DAYZCAMERA_OPTICS,         TIME_TO_ADS + TIME_3RD, false);
		RegisterTransitionTime(DAYZCAMERA_3RD_PRO,        DAYZCAMERA_OPTICS,         TIME_TO_ADS + TIME_3RD, false);

		////////////////////////////////////////////////////////////////////////
		// EXITING: OPTICS
		RegisterTransitionTime(DAYZCAMERA_OPTICS,         DAYZCAMERA_1ST,            TIME_FROM_ADS,          false);
		RegisterTransitionTime(DAYZCAMERA_OPTICS,         DAYZCAMERA_3RD_ERC_RAISED, TIME_FROM_ADS,          false);
		RegisterTransitionTime(DAYZCAMERA_OPTICS,         DAYZCAMERA_3RD_CRO_RAISED, TIME_FROM_ADS,          false);
		RegisterTransitionTime(DAYZCAMERA_OPTICS,         DAYZCAMERA_3RD_PRO_RAISED, TIME_FROM_ADS,          false);
		RegisterTransitionTime(DAYZCAMERA_OPTICS,         DAYZCAMERA_3RD_ERC,        TIME_FROM_ADS,          false);
		RegisterTransitionTime(DAYZCAMERA_OPTICS,         DAYZCAMERA_3RD_CRO,        TIME_FROM_ADS,          false);
		RegisterTransitionTime(DAYZCAMERA_OPTICS,         DAYZCAMERA_3RD_PRO,        TIME_FROM_ADS,          false);

		////////////////////////////////////////////////////////////////////////
		// BACKUP IRONSIGHT
		RegisterTransitionTime(DAYZCAMERA_IRONSIGHTS,     DAYZCAMERA_OPTICS,         TIME_BACKUP_SIGHT,      true);

	}

}
