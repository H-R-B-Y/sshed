
#ifndef FT_ENUM_MACRO_H
# define FT_ENUM_MACRO_H

# define ENUM_FACTORY(prefix, typename, list)\
	typedef enum e_##typename { \
		list(prefix, ENUM_FACTORY_ENUM) \
		prefix##_COUNT \
	} t_##typename; \
	\
	static const char *typename##_str[prefix##_COUNT] __attribute__((unused)) = { \
		list(prefix, ENUM_FACTORY_STR) \
	};

# define ENUM_FACTORY_ENUM(prefix, name) prefix##_##name,
# define ENUM_FACTORY_STR(prefix, name) #name,

#endif