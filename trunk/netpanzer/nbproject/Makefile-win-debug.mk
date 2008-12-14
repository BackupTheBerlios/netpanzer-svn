# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-win-debug.mk ./netpanzer-debug.exe

./netpanzer-debug.exe: ${OBJECTFILES}
	${MKDIR} -p .
	${LINK.cc} -mwindows -o ./netpanzer-debug ${OBJECTFILES} ${LDLIBSOPTIONS} 

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/win-debug
	${RM} ./netpanzer-debug.exe

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
