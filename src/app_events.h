# pragma once

enum AppEvent
 { // General
	evQuit,
	evEditParameters,
	evIndividualJointAdjust,
	evJointListSelect,
	evLoadConfig,
	evSaveConfig,
	evRunOde,
	evOdeReset,
	evDropBalls,
	evSkelAdjust,
	evLoadDefault,
	evTorqueMult,
	evControlMethods,
	evRest,
	evDoubleStance,
	evLeftStance,
	evRightStance,
	evToeStance,
	evHeelStance,
	evFixedStance,
	evWalkAdjust,
	evWalk,
	evManip,
	evParmListSelected,
	evSaveParm,
	evUpdateParm,
	evLoadParm,
	evDeleteParm,
	evStartStep,
	// End
	evLastOneNotUsed
 };

