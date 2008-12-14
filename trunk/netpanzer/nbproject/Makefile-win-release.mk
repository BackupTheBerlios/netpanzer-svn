# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE} -s -f nbproject/Makefile-win-release.mk dist/win-release/${PLATFORM}/netpanzer.exe

dist/win-release/${PLATFORM}/netpanzer.exe: ${OBJECTFILES}
	${MKDIR} -p dist/win-release/${PLATFORM}
	${LINK.cc} -o dist/win-release/${PLATFORM}/netpanzer ${OBJECTFILES} ${LDLIBSOPTIONS} 

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/win-release
	${RM} dist/win-release/${PLATFORM}/netpanzer.exe

# Subprojects
.clean-subprojects:
