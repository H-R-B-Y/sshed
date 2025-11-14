#include "game/game_data.h"
#include "game/game_manager.h"

# define DEFAULT_REFRESH 0
# define DEFAULT_SORT 1
# define DEFAULT_TIMEOUT 10
# define DEFAULT_BACKGROUND NCCHANNEL_INITIALIZER(0,0,0)
# define DEFAULT_DISPLAY_NAME NULL
# define DEFAULT_KEY_MODE 0

int	settings_set_refresh(struct s_game_manager *manager, unsigned int refresh)
{
	if (!manager)
		return (1);
	if (refresh > 10000)
		refresh = 10000; // Cap at 10 seconds
	manager->settings.refresh_timeout = refresh;
	return (0);
}

int	settings_set_auto_sort(struct s_game_manager *manager, t_u8 auto_sort)
{
	if (!manager)
		return (1);
	manager->settings.auto_sort = auto_sort ? 1 : 0;
	return (0);
}

int	settings_set_ai_timeout(struct s_game_manager *manager, unsigned int ai_timeout)
{
	if (!manager)
		return (1);
	if (ai_timeout < 1)
		ai_timeout = 1; // Minimum 1 
	if (ai_timeout > 10000)
		ai_timeout = 10000; // Cap at 10000
	manager->settings.ai_timeout = ai_timeout;
	return (0);
}

int	settings_set_background(struct s_game_manager *manager, t_u32 channel)
{
	if (!manager)
		return (1);
	if (channel == DEFAULT_BACKGROUND)
		ncchannel_set_alpha(&channel, NCALPHA_TRANSPARENT);
	manager->settings.background_channel = channel;
	return (0);
}

int	settings_set_display_name(struct s_game_manager *manager, char *display_name)
{
	if (!manager)
		return (1);
	if (manager->settings.display_name)
		free(manager->settings.display_name);
	manager->settings.display_name = display_name;
	return (0);
}

int	settings_set_key_mode(struct s_game_manager *manager, t_u8 key_mode)
{
	if (!manager)
		return (1);
	manager->settings.key_mode = key_mode ? 1 : 0;
	return (0);
}
t_u32 _parse_background(const char *value)
{
	// Parse a hex color string (e.g., "0xRRGGBB") into a t_u32 channel
	// This is a placeholder implementation
	unsigned int r = 0, g = 0, b = 0;
	if (sscanf(value, "0x%02x%02x%02x", &r, &g, &b) == 3)
		return (NCCHANNEL_INITIALIZER(r, g, b));
	return (DEFAULT_BACKGROUND);
}

int	setup_settings(struct s_game_manager *manager)
{
	char	*value;

	if (!manager)
		return (1);
	value = getenv("SSHED_REFRESH");
	if (!value)
		settings_set_refresh(manager, DEFAULT_REFRESH);
	else
	{
		int refresh = atoi(value);
		if (refresh < 0)
			refresh = DEFAULT_REFRESH;
		settings_set_refresh(manager, (unsigned int)refresh);
	}
	value = getenv("SSHED_SORT_HAND");
	if (!value)
		settings_set_auto_sort(manager, DEFAULT_SORT);
	else
	{
		if (ft_strcmp(value, "1") == 0 || ft_strcmp(value, "true") == 0)
			settings_set_auto_sort(manager, 1);
		else if (ft_strcmp(value, "0") == 0 || ft_strcmp(value, "false") == 0)
			settings_set_auto_sort(manager, 0);
		else
			settings_set_auto_sort(manager, DEFAULT_SORT);
	}
	value = getenv("SSHED_AI_TIMEOUT");
	if (!value)
		settings_set_ai_timeout(manager, DEFAULT_TIMEOUT);
	else
	{
		int ai_timeout = atoi(value);
		if (ai_timeout < 0)
			ai_timeout = DEFAULT_TIMEOUT;
		settings_set_ai_timeout(manager, (unsigned int)ai_timeout);
	}
	value = getenv("SSHED_BACKGROUND");
	if (!value)
		settings_set_background(manager, DEFAULT_BACKGROUND);
	else
	{
		t_u32 channel = _parse_background(value);
		settings_set_background(manager, channel);
	}
	value = getenv("SSHED_DISPLAY_NAME");
	if (!value)
		settings_set_display_name(manager, DEFAULT_DISPLAY_NAME);
	else
	{
		settings_set_display_name(manager, ft_strdup(value));
	}
	value = getenv("SSHED_KEY_MODE");
	if (!value)
		settings_set_key_mode(manager, DEFAULT_KEY_MODE);
	else
	{
		int key_mode = atoi(value);
		if (key_mode < 0 || key_mode > 1)
			key_mode = DEFAULT_KEY_MODE;
		settings_set_key_mode(manager, (t_u8)key_mode);
	}
	return (0);
}
