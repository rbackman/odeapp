# pragma once


#include "common.h"

enum config_parms{
		CONFIG_START_VAL, //this is a temp fix to make sure that all the values are considered float by forcing the first value to have a decimal
		CONFIG_PID_P,
		CONFIG_PID_I,
		CONFIG_PID_D,
		CONFIG_GRAV,
		CONFIG_PID,
		CONFIG_PID_MAXT,
		CONFIG_PID_T_MULT,
		CONFIG_X,
		CONFIG_Y,
		CONFIG_Z,
		CONFIG_VF,
		CONFIG_VFM_X,
		CONFIG_VFM_Y,
		CONFIG_VFM_Z
	};

class AppFileManager
 { public :
    AppFileManager ();
    GsVars* vars;

	GsArray<GsString> configs; 

	void loadParameterFiles();
	
	//load the specified physical human(.ph) file and apply it to the scene
	void loadParameterNamed(const char* n);  

	
	//this will save the specified config file and overwrite it if it exists
	//the string format should be the name alone.. "jumping" and the .ph and directory will be added
	void saveParameterNamed(const char* cfgfile);

	//save the current changes to the current.ph file
	//to change previously loaded one it must be "updated"
	void saveCurrentState();	//save info from this session.. to save the human Parameters.
	void loadCurrentState();
	void loadDefault();

 private:
    void loadVars (GsVars* v, const char* cfgfile );
	void read_files ( const char* cfgfile );
	
	GsString directory, fileType;
	GsString defaultFile;
	GsVars* files; //this is the main directory that keeps track of all the other config files.. 
				   //only the file manager has access 
	
 };



