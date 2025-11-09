/**
 * @file menu.h
 * @brief Menu system for UI navigation and selection
 * 
 * This header defines a flexible menu system for creating interactive
 * menus with selectable options. Menus support:
 * - Static and dynamic text
 * - Action callbacks
 * - Disabled states
 * - Selection callbacks (on_select/on_deselect)
 * - Keyboard navigation (up/down/enter)
 */

#ifndef MENU_H
# define MENU_H

# include <notcurses/notcurses.h>
# define FT_INCLUDE_ALL
# include "libft.h"

/* ========================================================================== */
/*                         FORWARD DECLARATIONS                               */
/* ========================================================================== */

struct s_menu;
struct s_menu_option;

/* ========================================================================== */
/*                         FUNCTION POINTER TYPES                             */
/* ========================================================================== */

/**
 * @brief Menu action function pointer type
 * 
 * Called when a menu option is activated (selected with Enter).
 * 
 * @param m The menu containing the option
 * @param nc The notcurses context
 * @return 0 on success, non-zero on error
 */
typedef int		(*t_menu_action_fn)(struct s_menu *m, struct notcurses *nc);

/**
 * @brief Dynamic text function pointer type
 * 
 * Called to generate dynamic text for a menu option each render.
 * Useful for displaying changing values (e.g., "Volume: 50%").
 * 
 * @param m The menu containing the option
 * @param option The menu option requesting text
 * @return Dynamically allocated string to display (caller must free)
 */
typedef char	*(*t_menu_dynamic_text_fn)(struct s_menu *m, struct s_menu_option *option);

/**
 * @brief Menu callback function pointer type
 * 
 * Called when an option is selected or deselected.
 * Useful for visual feedback or state changes.
 * 
 * @param m The menu containing the option
 * @param option The menu option affected
 * @return 0 on success, non-zero on error
 */
typedef int		(*t_menu_callback)(struct s_menu *m, struct s_menu_option *option);

/* ========================================================================== */
/*                         DATA STRUCTURES                                    */
/* ========================================================================== */

/**
 * @brief Menu option structure
 * 
 * Represents a single selectable option in a menu.
 * Supports static or dynamic text, actions, disabled states, and callbacks.
 */
/**
 * @brief Menu option structure
 * 
 * Represents a single selectable option in a menu.
 * Supports static or dynamic text, actions, disabled states, and callbacks.
 */
struct s_menu_option
{
	/// @brief Type of text for this option
	enum e_menu_text_type {
		STATIC_TEXT,              ///< Simple static string
		STATIC_TEXT_FUNCTION,     ///< Static string returned by function
		DYNAMIC_TEXT_FUNCTION,    ///< Dynamic string generated each render
	}	text_type;

	/// @brief Text content or function to get text (based on text_type)
	union
	{
		const char				*option_text;      ///< Static text string
		t_menu_dynamic_text_fn	get_option_text;   ///< Function to get text
	};

	/// @brief Action to perform when this option is activated
	t_menu_action_fn	option_action;
	/// @brief Whether this option is currently disabled
	int					disabled;
	/// @brief User-defined data associated with this option
	void				*user_data;

	/// @brief Callback when this option becomes selected
	t_menu_callback		on_select;
	/// @brief Callback when this option becomes deselected
	t_menu_callback		on_deselect;
};

/**
 * @brief Menu structure
 * 
 * Represents a navigable menu with multiple options.
 */
struct s_menu
{
	/// @brief Array of menu options
	struct s_menu_option	*options;
	/// @brief Number of options in the menu
	unsigned int			option_count;
	/// @brief Index of the currently selected option
	unsigned int			selected_index;
	/// @brief Notcurses plane for rendering the menu
	struct ncplane			*menu_plane;
	/// @brief Whether the menu is currently hidden
	t_u8					is_hidden;
	/// @brief Whether the menu needs to be re-rendered
	t_u8					is_dirty;
	/// @brief User-defined data associated with this menu
	void					*user_data;
};

/* ========================================================================== */
/*                       OPTION CREATION HELPERS                              */
/* ========================================================================== */

/**
 * @brief Create a static menu option
 * 
 * Helper function to create a menu option with static text.
 * 
 * @param text The text to display for this option
 * @param action The action to perform when activated
 * @return Initialized menu option structure
 */
struct s_menu_option static_menu_option(
	const char *text,
	int (*action)(struct s_menu *m, struct notcurses *nc)
);

/**
 * @brief Create a dynamic menu option
 * 
 * Helper function to create a menu option with dynamic text.
 * 
 * @param get_text Function to generate the text each render
 * @param action The action to perform when activated
 * @return Initialized menu option structure
 */
struct s_menu_option dynamic_menu_option(
	t_menu_dynamic_text_fn get_text,
	int (*action)(struct s_menu *m, struct notcurses *nc)
);

/* ========================================================================== */
/*                       MENU MANAGEMENT FUNCTIONS                            */
/* ========================================================================== */

/**
 * @brief Create a new menu
 * 
 * Allocates and initializes a menu with the given options.
 * 
 * @param menu Pointer to store the created menu
 * @param parent Parent notcurses plane for the menu
 * @param options Array of menu options
 * @param option_count Number of options in the array
 * @return 0 on success, non-zero on error
 */
int	menu_create(
	struct s_menu **menu,
	struct ncplane *parent,
	struct s_menu_option *options,
	unsigned int option_count
);

/**
 * @brief Destroy a menu and free resources
 * 
 * Frees all resources associated with the menu including the plane.
 * 
 * @param menu The menu to destroy
 */
void	menu_destroy(
	struct s_menu *menu
);

/**
 * @brief Add an option to an existing menu
 * 
 * Dynamically adds a new option to the menu.
 * 
 * @param menu The menu to add to
 * @param option_text Text for the new option
 * @param option_action Action for the new option
 * @return 0 on success, non-zero on error
 */
// int	menu_add_option(
// 	struct s_menu *menu,
// 	const char *option_text,
// 	int (*option_action)(struct s_menu *m, struct notcurses *nc)
// );

/* ========================================================================== */
/*                       MENU INTERACTION FUNCTIONS                           */
/* ========================================================================== */

/**
 * @brief Select the next menu option
 * 
 * Moves selection down, wrapping to top if at bottom.
 * Calls on_deselect/on_select callbacks as needed.
 * 
 * @param menu The menu to navigate
 * @return 0 on success, non-zero on error
 */
int	menu_select_next(
	struct s_menu *menu
);

/**
 * @brief Select the previous menu option
 * 
 * Moves selection up, wrapping to bottom if at top.
 * Calls on_deselect/on_select callbacks as needed.
 * 
 * @param menu The menu to navigate
 * @return 0 on success, non-zero on error
 */
int	menu_select_prev(
	struct s_menu *menu
);

/**
 * @brief Activate the currently selected option
 * 
 * Calls the option_action function of the selected option.
 * 
 * @param menu The menu containing the option
 * @return Result of the option's action function
 */
int	menu_activate_selected(
	struct s_menu *menu
);

/**
 * @brief Render the menu to the screen
 * 
 * Draws the menu and all its options to the associated plane.
 * Clears the dirty flag after rendering.
 * 
 * @param menu The menu to render
 * @return 0 on success, non-zero on error
 */
int	menu_render(
	struct s_menu *menu
);

/**
 * @brief Hide or show the menu
 * 
 * @param menu The menu to hide
 * @return int 
 */
int	menu_hide(
	struct s_menu *menu
);

/**
 * @brief Show the menu
 * 
 * @param menu The menu to show
 * @return int 
 */
int	menu_show(
	struct s_menu *menu
);

/* ========================================================================== */
/*                       DEFAULT KEY HANDLER                                  */
/* ========================================================================== */

/**
 * @brief Default menu key handler (inline helper)
 * 
 * Convenience function for handling standard menu keyboard input:
 * - NCKEY_DOWN: Select next option
 * - NCKEY_UP: Select previous option
 * - NCKEY_ENTER/\\n/\\r: Activate selected option
 * 
 * Usage: Call this from your stdin_handler function.
 * 
 * @param menu The menu to handle input for
 * @param nc The notcurses context
 * @return 0 on success, non-zero on error
 */
static inline int	_menu_key_handler(
	struct s_menu *menu,
	struct ncinput event
)
{
	if (!menu)
		return (1);
	if (event.id == NCKEY_DOWN)
		return (menu_select_next(menu));
	else if (event.id == NCKEY_UP)
		return (menu_select_prev(menu));
	else if (event.id == NCKEY_ENTER || event.id == '\n' || event.id == '\r')
		return (menu_activate_selected(menu));
	return (0);
}

#endif
