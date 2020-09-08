namespace Hotkey
{
	namespace
	{
		struct Shortcut
		{
			int mod, key;
			void(*action)();
		};

		std::vector<Shortcut> shortcuts;
	}

	bool CheckKeyMods (int _mod)
	{
		// checks to see if the passed key mods are currently active
		// note: this function doesn't care about left or right difference
		int keymod = SDL_GetModState();
		// we need to strip out keys like capslock and numlock (they cause problems)
		keymod &= ~(KMOD_NUM|KMOD_CAPS);
		// @OPTIMISE: THERE SHOULD BE A MUCH BETTER WAY OF HANDLING THIS...
		// we also need to set both left and right keymods (annoying...)
		if (keymod & KMOD_LCTRL  || keymod & KMOD_RCTRL)  { keymod |= KMOD_CTRL; }
		if (keymod & KMOD_LSHIFT || keymod & KMOD_RSHIFT) { keymod |= KMOD_SHIFT; }
		if (keymod & KMOD_LALT   || keymod & KMOD_RALT)   { keymod |= KMOD_ALT; }
		if (keymod & KMOD_LGUI   || keymod & KMOD_RGUI)   { keymod |= KMOD_GUI; }
		// does the actual comparison
		return (keymod == _mod);
	}

	void Handle ()
	{
		switch (App::event.type) {
			case (SDL_KEYDOWN): {
				// we don't want hotkeys running repeatedly
				if (App::event.key.repeat == 0) {
					for (auto& shortcut: shortcuts) {
						// check to see if both the key and keymods are right, then run the func
						if (App::event.key.keysym.sym == shortcut.key && CheckKeyMods(shortcut.mod)) {
							// we don't want to run an invalid function
							if (shortcut.action) {
								shortcut.action();
								break;
							}
						}
					}
				}
			} break;
		}
	}

	void Register (int _mod, int _key, void(*_action)())
	{
		// add a new shortcut/hotkey
		shortcuts.push_back(Shortcut());
		auto& shortcut = shortcuts.back();

		// set the data for the shortcut, used in the handler
		shortcut.mod = _mod, shortcut.key = _key;
		shortcut.action = _action;
	}
}