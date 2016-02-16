# The dict_gx extension
PROJECT("Gx application (3GPP TS 29.212) dictionary definitions" C)

# Compile as a module
FD_ADD_EXTENSION(dict_gx dict_gx.c)

####
## INSTALL section ##
INSTALL(TARGETS dict_gx
	LIBRARY DESTINATION ${INSTALL_EXTENSIONS_SUFFIX}
	COMPONENT freeDiameter-dictionary-Gx)
