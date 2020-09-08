/*******************************************************************************
 * VARIABLES
 * Loads the hotloaded variable tweak file for quick value changes.
 *
*******************************************************************************/

#ifndef __TCE_VARIABLES_H__ /*////////////////////////////////////////////////*/
#define __TCE_VARIABLES_H__

namespace TCE
{

enum VariableType
{
	VARIABLE_STRING,
	VARIABLE_INTEGER,
	VARIABLE_NUMBER,
	VARIABLE_BOOLEAN,
	VARIABLE_COLOR,

	VARIABLE_TOTAL
};

struct VariableInfo
{
	String name;

	VariableType type;
	void* value;
};

GLOBAL constexpr const char* VARIABLE_TYPE_NAME[VARIABLE_TOTAL] =
{
	"STRING",
	"INTEGER",
	"NUMBER",
	"BOOLEAN",
	"COLOR"
};

GLOBAL constexpr const char* DEFAULT_VARIABLES_PATH = "data/variables.dat";

GLOBAL String current_variables_path;

GLOBAL Array<VariableInfo> variable_list;
GLOBAL bool variable_tweaks_enabled;

INLDEF void VariablesInit ();

INLDEF void RegisterAllVariables ();
STDDEF void LoadAllVariables ();

STDDEF void RegisterVariable (const char* _name, VariableType _type, void* _value);
STDDEF bool VariableIsRegistered (const char* _name);

} // TCE

#endif /* __TCE_VARIABLES_H__ ////////////////////////////////////////////////*/
