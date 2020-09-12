namespace Editor
{

namespace internal
{
FILDEF void reset_ui_state ();
}

FILDEF void init_editor ();
FILDEF void quit_editor ();

FILDEF void do_editor   ();

FILDEF void handle_editor_events ();

FILDEF bool is_editor_initialized ();

} // Editor
