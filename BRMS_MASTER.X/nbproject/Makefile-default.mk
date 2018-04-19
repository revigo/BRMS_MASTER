#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/BRMS_MASTER.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/BRMS_MASTER.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../SOURCE/uMain.c ../SOURCE/uI2C.c ../SOURCE/uDelay.c ../SOURCE/uInit.c ../SOURCE/uCommon.c ../SOURCE/uLCD.c ../SOURCE/uPBUS.c ../SOURCE/uMBcrc.c ../SOURCE/uModbus.c ../SOURCE/uRS232.c ../SOURCE/uSDcard.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1758838966/uMain.o ${OBJECTDIR}/_ext/1758838966/uI2C.o ${OBJECTDIR}/_ext/1758838966/uDelay.o ${OBJECTDIR}/_ext/1758838966/uInit.o ${OBJECTDIR}/_ext/1758838966/uCommon.o ${OBJECTDIR}/_ext/1758838966/uLCD.o ${OBJECTDIR}/_ext/1758838966/uPBUS.o ${OBJECTDIR}/_ext/1758838966/uMBcrc.o ${OBJECTDIR}/_ext/1758838966/uModbus.o ${OBJECTDIR}/_ext/1758838966/uRS232.o ${OBJECTDIR}/_ext/1758838966/uSDcard.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1758838966/uMain.o.d ${OBJECTDIR}/_ext/1758838966/uI2C.o.d ${OBJECTDIR}/_ext/1758838966/uDelay.o.d ${OBJECTDIR}/_ext/1758838966/uInit.o.d ${OBJECTDIR}/_ext/1758838966/uCommon.o.d ${OBJECTDIR}/_ext/1758838966/uLCD.o.d ${OBJECTDIR}/_ext/1758838966/uPBUS.o.d ${OBJECTDIR}/_ext/1758838966/uMBcrc.o.d ${OBJECTDIR}/_ext/1758838966/uModbus.o.d ${OBJECTDIR}/_ext/1758838966/uRS232.o.d ${OBJECTDIR}/_ext/1758838966/uSDcard.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1758838966/uMain.o ${OBJECTDIR}/_ext/1758838966/uI2C.o ${OBJECTDIR}/_ext/1758838966/uDelay.o ${OBJECTDIR}/_ext/1758838966/uInit.o ${OBJECTDIR}/_ext/1758838966/uCommon.o ${OBJECTDIR}/_ext/1758838966/uLCD.o ${OBJECTDIR}/_ext/1758838966/uPBUS.o ${OBJECTDIR}/_ext/1758838966/uMBcrc.o ${OBJECTDIR}/_ext/1758838966/uModbus.o ${OBJECTDIR}/_ext/1758838966/uRS232.o ${OBJECTDIR}/_ext/1758838966/uSDcard.o

# Source Files
SOURCEFILES=../SOURCE/uMain.c ../SOURCE/uI2C.c ../SOURCE/uDelay.c ../SOURCE/uInit.c ../SOURCE/uCommon.c ../SOURCE/uLCD.c ../SOURCE/uPBUS.c ../SOURCE/uMBcrc.c ../SOURCE/uModbus.c ../SOURCE/uRS232.c ../SOURCE/uSDcard.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/BRMS_MASTER.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=33EP256MU806
MP_LINKER_FILE_OPTION=,--script=p33EP256MU806.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1758838966/uMain.o: ../SOURCE/uMain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uMain.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uMain.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uMain.c  -o ${OBJECTDIR}/_ext/1758838966/uMain.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uMain.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uMain.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uI2C.o: ../SOURCE/uI2C.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uI2C.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uI2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uI2C.c  -o ${OBJECTDIR}/_ext/1758838966/uI2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uI2C.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uI2C.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uDelay.o: ../SOURCE/uDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uDelay.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uDelay.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uDelay.c  -o ${OBJECTDIR}/_ext/1758838966/uDelay.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uDelay.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uDelay.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uInit.o: ../SOURCE/uInit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uInit.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uInit.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uInit.c  -o ${OBJECTDIR}/_ext/1758838966/uInit.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uInit.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uInit.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uCommon.o: ../SOURCE/uCommon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uCommon.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uCommon.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uCommon.c  -o ${OBJECTDIR}/_ext/1758838966/uCommon.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uCommon.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uCommon.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uLCD.o: ../SOURCE/uLCD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uLCD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uLCD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uLCD.c  -o ${OBJECTDIR}/_ext/1758838966/uLCD.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uLCD.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uLCD.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uPBUS.o: ../SOURCE/uPBUS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uPBUS.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uPBUS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uPBUS.c  -o ${OBJECTDIR}/_ext/1758838966/uPBUS.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uPBUS.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uPBUS.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uMBcrc.o: ../SOURCE/uMBcrc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uMBcrc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uMBcrc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uMBcrc.c  -o ${OBJECTDIR}/_ext/1758838966/uMBcrc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uMBcrc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uMBcrc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uModbus.o: ../SOURCE/uModbus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uModbus.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uModbus.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uModbus.c  -o ${OBJECTDIR}/_ext/1758838966/uModbus.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uModbus.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uModbus.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uRS232.o: ../SOURCE/uRS232.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uRS232.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uRS232.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uRS232.c  -o ${OBJECTDIR}/_ext/1758838966/uRS232.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uRS232.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uRS232.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uSDcard.o: ../SOURCE/uSDcard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uSDcard.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uSDcard.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uSDcard.c  -o ${OBJECTDIR}/_ext/1758838966/uSDcard.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uSDcard.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -mno-eds-warn  -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uSDcard.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1758838966/uMain.o: ../SOURCE/uMain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uMain.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uMain.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uMain.c  -o ${OBJECTDIR}/_ext/1758838966/uMain.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uMain.o.d"      -mno-eds-warn  -g -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uMain.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uI2C.o: ../SOURCE/uI2C.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uI2C.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uI2C.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uI2C.c  -o ${OBJECTDIR}/_ext/1758838966/uI2C.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uI2C.o.d"      -mno-eds-warn  -g -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uI2C.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uDelay.o: ../SOURCE/uDelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uDelay.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uDelay.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uDelay.c  -o ${OBJECTDIR}/_ext/1758838966/uDelay.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uDelay.o.d"      -mno-eds-warn  -g -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uDelay.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uInit.o: ../SOURCE/uInit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uInit.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uInit.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uInit.c  -o ${OBJECTDIR}/_ext/1758838966/uInit.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uInit.o.d"      -mno-eds-warn  -g -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uInit.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uCommon.o: ../SOURCE/uCommon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uCommon.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uCommon.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uCommon.c  -o ${OBJECTDIR}/_ext/1758838966/uCommon.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uCommon.o.d"      -mno-eds-warn  -g -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uCommon.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uLCD.o: ../SOURCE/uLCD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uLCD.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uLCD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uLCD.c  -o ${OBJECTDIR}/_ext/1758838966/uLCD.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uLCD.o.d"      -mno-eds-warn  -g -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uLCD.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uPBUS.o: ../SOURCE/uPBUS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uPBUS.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uPBUS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uPBUS.c  -o ${OBJECTDIR}/_ext/1758838966/uPBUS.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uPBUS.o.d"      -mno-eds-warn  -g -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uPBUS.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uMBcrc.o: ../SOURCE/uMBcrc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uMBcrc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uMBcrc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uMBcrc.c  -o ${OBJECTDIR}/_ext/1758838966/uMBcrc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uMBcrc.o.d"      -mno-eds-warn  -g -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uMBcrc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uModbus.o: ../SOURCE/uModbus.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uModbus.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uModbus.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uModbus.c  -o ${OBJECTDIR}/_ext/1758838966/uModbus.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uModbus.o.d"      -mno-eds-warn  -g -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uModbus.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uRS232.o: ../SOURCE/uRS232.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uRS232.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uRS232.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uRS232.c  -o ${OBJECTDIR}/_ext/1758838966/uRS232.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uRS232.o.d"      -mno-eds-warn  -g -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uRS232.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1758838966/uSDcard.o: ../SOURCE/uSDcard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1758838966 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uSDcard.o.d 
	@${RM} ${OBJECTDIR}/_ext/1758838966/uSDcard.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SOURCE/uSDcard.c  -o ${OBJECTDIR}/_ext/1758838966/uSDcard.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1758838966/uSDcard.o.d"      -mno-eds-warn  -g -omf=elf -O0 -I"../INCLUDE" -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1758838966/uSDcard.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/BRMS_MASTER.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/BRMS_MASTER.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf  -mreserve=data@0x1000:0x101B -mreserve=data@0x101C:0x101D -mreserve=data@0x101E:0x101F -mreserve=data@0x1020:0x1021 -mreserve=data@0x1022:0x1023 -mreserve=data@0x1024:0x1027 -mreserve=data@0x1028:0x104F   -Wl,--local-stack,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/BRMS_MASTER.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/BRMS_MASTER.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,--local-stack,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/BRMS_MASTER.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
