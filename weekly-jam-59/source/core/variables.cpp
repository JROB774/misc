/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF String TrimVariablesFilePath (String _path)
{
	size_t pos = _path.FindLast("\\/");
	if (pos == String::UNDEFINED_POSITION) { pos = 0; }
	else { ++pos; }

	return _path.Substring(pos);
}

INLDEF void VariablesInit ()
{
	// Load custom user variables file if present, if not load the shared variables files.
	String user_variables_file = String("data/") + GetCurrentUsername() + String(".variables.dat");
	if (DoesFileExist(user_variables_file.c_string)) {
		TCE_DEBUG_LOG("VARIABLE", "Custom user variables file '%s' found.", user_variables_file.c_string);
		current_variables_path = user_variables_file;
	} else {
		current_variables_path = DEFAULT_VARIABLES_PATH;
	}

	variable_tweaks_enabled = true;
}

INLDEF void RegisterAllVariables ()
{
	RegisterVariable("enable_debug_logging",          VARIABLE_BOOLEAN, &debug_log_enabled);
	RegisterVariable("enable_hotloading",             VARIABLE_BOOLEAN, &hotloader_enabled);
	RegisterVariable("enable_console",                VARIABLE_BOOLEAN, &console_enabled);

	RegisterVariable("refresh_rate",                  VARIABLE_NUMBER,  &refresh_rate);

	RegisterVariable("window_title",                  VARIABLE_STRING,  &window_title);
	RegisterVariable("window_start_width",            VARIABLE_INTEGER, &window_start_width);
	RegisterVariable("window_start_height",           VARIABLE_INTEGER, &window_start_height);

	RegisterVariable("viewport_width",                VARIABLE_INTEGER, &viewport_width);
	RegisterVariable("viewport_height",               VARIABLE_INTEGER, &viewport_height);

	RegisterVariable("border_clear_color",            VARIABLE_COLOR,   &border_clear_color);
	RegisterVariable("viewport_clear_color",          VARIABLE_COLOR,   &viewport_clear_color);

	RegisterVariable("mixer_frequency",               VARIABLE_INTEGER, &mixer_frequency);
	RegisterVariable("mixer_channels",                VARIABLE_INTEGER, &mixer_channels);
	RegisterVariable("mixer_chunksize",               VARIABLE_INTEGER, &mixer_chunksize);
	RegisterVariable("mixer_tracks",                  VARIABLE_INTEGER, &mixer_tracks);

	RegisterVariable("sound_volume",                  VARIABLE_INTEGER, &sound_volume);
	RegisterVariable("music_volume",                  VARIABLE_INTEGER, &music_volume);

	RegisterVariable("console_mini_height",           VARIABLE_NUMBER,  &console_mini_height);
	RegisterVariable("console_large_height",          VARIABLE_NUMBER,  &console_large_height);
	RegisterVariable("console_left_padding",          VARIABLE_INTEGER, &console_left_padding);
	RegisterVariable("console_bottom_padding",        VARIABLE_INTEGER, &console_bottom_padding);
	RegisterVariable("console_input_color",           VARIABLE_COLOR,   &console_input_color);
	RegisterVariable("console_history_color",         VARIABLE_COLOR,   &console_history_color);
	RegisterVariable("console_back_color",            VARIABLE_COLOR,   &console_back_color);
	RegisterVariable("console_scroll_speed",          VARIABLE_NUMBER,  &console_scroll_speed);
	RegisterVariable("console_cursor_blink_interval", VARIABLE_NUMBER,  &console_cursor.blink_interval);
	RegisterVariable("console_text_scale",            VARIABLE_NUMBER,  &console_text_scale);
}

INLDEF void ParseStringVariable (const VariableInfo& _info, Tokenizer& _tokenizer, String* _string)
{
	*_string = TokenToString(_tokenizer);

	TCE_DEBUG_LOG("VARIABLE", "Loaded %s '%s' with value: '%s'.",
		VARIABLE_TYPE_NAME[_info.type], _info.name.c_string, _string->c_string);
}

INLDEF void ParseIntegerVariable (const VariableInfo& _info, Tokenizer& _tokenizer, int* _integer)
{
	*_integer = TokenToInteger(_tokenizer);

	TCE_DEBUG_LOG("VARIABLE", "Loaded %s '%s' with value: %d.",
		VARIABLE_TYPE_NAME[_info.type], _info.name.c_string, *_integer);
}

INLDEF void ParseNumberVariable (const VariableInfo& _info, Tokenizer& _tokenizer, float* _number)
{
	*_number = TokenToNumber(_tokenizer);

	TCE_DEBUG_LOG("VARIABLE", "Loaded %s '%s' with value: %f.",
		VARIABLE_TYPE_NAME[_info.type], _info.name.c_string, *_number);
}

INLDEF void ParseBooleanVariable (const VariableInfo& _info, Tokenizer& _tokenizer, bool* _boolean)
{
	*_boolean = TokenToBoolean(_tokenizer);

	TCE_DEBUG_LOG("VARIABLE", "Loaded %s '%s' with value: %s.",
		VARIABLE_TYPE_NAME[_info.type], _info.name.c_string, (*_boolean) ? "true" : "false");
}

INLDEF void ParseColorVariable (const VariableInfo& _info, Tokenizer& _tokenizer, SDL_Color* _color)
{
	*_color = TokenToColor(_tokenizer);

	TCE_DEBUG_LOG("VARIABLE", "Loaded %s '%s' with value: (%d,%d,%d,%d).",
		VARIABLE_TYPE_NAME[_info.type], _info.name.c_string, _color->r, _color->g, _color->b, _color->a);
}

STDDEF void LoadAllVariables ()
{
	// Do not bother if variable tweaking isn't enabled.
	if (!variable_tweaks_enabled) { return; }

	// Parse the file line-by-line and load the values into the registered variables.
	Array<String> lines = ReadEntireFileIntoLines(current_variables_path.c_string);
	if (!lines.data) {
		TCE_ERROR_LOG(ERROR_LEVEL_MED, ERROR_TYPE_STD, "Failed to load variable tweak file!");
		variable_tweaks_enabled = false;
		return;
	}

	Tokenizer tokenizer = {0};
	tokenizer.delim = " \n\r\t\v\f";
	for (size_t i=0; i<lines.count; ++i) {
		tokenizer.cursor = EatLeadingDelimiters(lines[i].c_string, tokenizer.delim);
		if (tokenizer.cursor[0] == '\0' || tokenizer.cursor[0] == '#') { // Ignore blanks and comments.
			continue;
		} else {
			GetToken(tokenizer);
			bool token_matched = false;
			// Search the registered variables and if there is a match load the value.
			for (size_t j=0; j<variable_list.count; ++j) {
				VariableInfo& variable_info = variable_list[j];
				if (strncmp(tokenizer.token, variable_info.name.c_string, tokenizer.token_length) == 0) {
					switch (variable_info.type) {
						case (VARIABLE_STRING): {
							ParseStringVariable(variable_info, tokenizer, CAST(String*, variable_info.value));
						} break;
						case (VARIABLE_INTEGER): {
							ParseIntegerVariable(variable_info, tokenizer, CAST(int*, variable_info.value));
						} break;
						case (VARIABLE_NUMBER): {
							ParseNumberVariable(variable_info, tokenizer, CAST(float*, variable_info.value));
						} break;
						case (VARIABLE_BOOLEAN): {
							ParseBooleanVariable(variable_info, tokenizer, CAST(bool*, variable_info.value));
						} break;
						case (VARIABLE_COLOR): {
							ParseColorVariable(variable_info, tokenizer, CAST(SDL_Color*, variable_info.value));
						} break;

						// Inform of unknown variable types.
						default: {
							TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_STD, "Attempt to load variable '%s' of unknown type '%d'!",
								variable_info.name.c_string, variable_info.type);
						} break;
					}

					// We can end the search for the particular variable.
					token_matched = true;
					break;
				}
			}

			if (!token_matched) {
				TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_STD, "Cannot load non-registered variable '%.*s'!",
					tokenizer.token_length, tokenizer.token);
			}
		}
	}

	// Updates all the potentially changed window values.
	if (window_ready) { UpdateWindowValues(); }
	// Updates all the potentially changes mixer values.
	if (mixer_ready) { UpdateMixerValues(); }
}

STDDEF void RegisterVariable (const char* _name, VariableType _type, void* _value)
{
	// Don't bother if variable tweaking isn't enabled.
	if (!variable_tweaks_enabled) { return; }
	// Don't bother registering a variable a second time.
	if (VariableIsRegistered(_name)) { return; }

	// Add the variable info to the list of registered variables.
	VariableInfo variable_info;

	variable_info.name  = _name;
	variable_info.type  = _type;
	variable_info.value = _value;

	variable_list.AddElement(variable_info);

	// Informs that a variable was successfully registered
	TCE_DEBUG_LOG("VARIABLE", "Registered variable '%s' of type '%s'!", _name, VARIABLE_TYPE_NAME[_type]);
}

STDDEF bool VariableIsRegistered (const char* _name)
{
	// Don't bother if variable tweaking isn't enabled.
	if (variable_tweaks_enabled) {
		for (size_t i=0; i<variable_list.count; ++i) {
			if (_name == variable_list[i].name) {
				return true;
			}
		}
	}
	return false;
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
