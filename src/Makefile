#
# OMNeT++/OMNEST Makefile for omnetpp_obs
#
# This file was generated with the command:
#  opp_makemake -f --deep -I../../inet/src/networklayer/ipv4 -I../../inet/src/networklayer/common -I../../inet/src/networklayer/rsvp_te -I../../inet/src/networklayer/icmpv6 -I../../inet/src/transport/tcp -I../../inet/src/networklayer/mpls -I../../inet/src/base -I../../inet/src/networklayer/ted -I../../inet/src/util/headerserializers -I../../inet/src/networklayer/contract -I../../inet/src/util -I../../inet/src/transport/contract -I../../inet/src/linklayer/mfcore -I../../inet/src/networklayer/ipv6 -I../../inet/src/transport/sctp -I../../inet/src/world -I../../inet/src/applications/pingapp -I../../inet/src/linklayer/contract -I../../inet/src/networklayer/arp -I../../inet/src/networklayer/ldp -I../../inet/src/transport/udp -L../../inet/out/$(CONFIGNAME)/src -linet -KINET_PROJ=../../inet
#

# Name of target to be created (-o option)
TARGET = omnetpp_obs$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I../../inet/src/networklayer/ipv4 \
    -I../../inet/src/networklayer/common \
    -I../../inet/src/networklayer/rsvp_te \
    -I../../inet/src/networklayer/icmpv6 \
    -I../../inet/src/transport/tcp \
    -I../../inet/src/networklayer/mpls \
    -I../../inet/src/base \
    -I../../inet/src/networklayer/ted \
    -I../../inet/src/util/headerserializers \
    -I../../inet/src/networklayer/contract \
    -I../../inet/src/util \
    -I../../inet/src/transport/contract \
    -I../../inet/src/linklayer/mfcore \
    -I../../inet/src/networklayer/ipv6 \
    -I../../inet/src/transport/sctp \
    -I../../inet/src/world \
    -I../../inet/src/applications/pingapp \
    -I../../inet/src/linklayer/contract \
    -I../../inet/src/networklayer/arp \
    -I../../inet/src/networklayer/ldp \
    -I../../inet/src/transport/udp \
    -I. \
    -Icommon \
    -Icore \
    -Iedge \
    -Imessages

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L../../inet/out/$(CONFIGNAME)/src  -linet
LIBS += -Wl,-rpath,`abspath ../../inet/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = ../out
PROJECTRELATIVE_PATH = src
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/common/HorizonScheduler.o \
    $O/edge/AnsyncTimeSlicedDispatcher.o \
    $O/edge/BurstDisassembler.o \
    $O/edge/MixedTimerBurstlengthBasedQueue.o \
    $O/edge/OffsetTable.o \
    $O/edge/PacketClassifier.o \
    $O/edge/PacketGenerator.o \
    $O/edge/PacketSink.o \
    $O/edge/TimeSlicedDispatcher.o \
    $O/messages/Burst.o \
    $O/messages/BurstControlPacket.o \
    $O/messages/EnsureLengthBurst.o \
    $O/messages/Burst_m.o \
    $O/messages/BurstControlPacket_m.o \
    $O/messages/ConnectionEvent_m.o

# Message files
MSGFILES = \
    messages/Burst.msg \
    messages/BurstControlPacket.msg \
    messages/ConnectionEvent.msg

# Other makefile variables (-K)
INET_PROJ=../../inet

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET)

$(TARGET) : $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY:

.SUFFIXES: .cc

$O/%.o: %.cc
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f omnetpp_obs omnetpp_obs.exe libomnetpp_obs.so libomnetpp_obs.a libomnetpp_obs.dll libomnetpp_obs.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f common/*_m.cc common/*_m.h
	-rm -f core/*_m.cc core/*_m.h
	-rm -f edge/*_m.cc edge/*_m.h
	-rm -f messages/*_m.cc messages/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc common/*.cc core/*.cc edge/*.cc messages/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/common/HorizonScheduler.o: common/HorizonScheduler.cc \
	common/HorizonScheduler.h \
	common/IBurstScheduler.h
$O/edge/PacketGenerator.o: edge/PacketGenerator.cc \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo.h \
	edge/PacketGenerator.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h
$O/edge/MixedTimerBurstlengthBasedQueue.o: edge/MixedTimerBurstlengthBasedQueue.cc \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo.h \
	messages/Burst_m.h \
	messages/IBurst.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo_m.h \
	messages/Burst.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	edge/MixedTimerBurstlengthBasedQueue.h
$O/edge/PacketClassifier.o: edge/PacketClassifier.cc \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo.h \
	edge/PacketClassifier.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo_m.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h
$O/edge/BurstDisassembler.o: edge/BurstDisassembler.cc \
	messages/IBurst.h \
	messages/Burst.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	messages/BurstControlPacket_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo.h \
	messages/Burst_m.h \
	edge/BurstDisassembler.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo_m.h \
	messages/BurstControlPacket.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h
$O/edge/PacketSink.o: edge/PacketSink.cc \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo.h \
	edge/PacketSink.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h
$O/edge/TimeSlicedDispatcher.o: edge/TimeSlicedDispatcher.cc \
	messages/IBurst.h \
	messages/Burst.h \
	common/IBurstScheduler.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	messages/BurstControlPacket_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo.h \
	edge/TimeSlicedDispatcher.h \
	messages/Burst_m.h \
	$(INET_PROJ)/src/networklayer/contract/IPControlInfo_m.h \
	messages/BurstControlPacket.h \
	edge/OffsetTableAccess.h \
	common/BurstSchedulerAccess.h \
	$(INET_PROJ)/src/base/ModuleAccess.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	$(INET_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	edge/OffsetTable.h
$O/edge/AnsyncTimeSlicedDispatcher.o: edge/AnsyncTimeSlicedDispatcher.cc \
	edge/AnsyncTimeSlicedDispatcher.h \
	common/IBurstScheduler.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	edge/OffsetTable.h
$O/edge/OffsetTable.o: edge/OffsetTable.cc \
	$(INET_PROJ)/src/networklayer/contract/IPAddressResolver.h \
	$(INET_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	edge/OffsetTable.h
$O/messages/BurstControlPacket.o: messages/BurstControlPacket.cc \
	messages/BurstControlPacket.h \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	messages/BurstControlPacket_m.h
$O/messages/BurstControlPacket_m.o: messages/BurstControlPacket_m.cc \
	$(INET_PROJ)/src/base/INETDefs.h \
	$(INET_PROJ)/src/networklayer/contract/IPAddress.h \
	messages/BurstControlPacket_m.h
$O/messages/Burst.o: messages/Burst.cc \
	messages/Burst_m.h \
	messages/IBurst.h \
	messages/Burst.h
$O/messages/ConnectionEvent_m.o: messages/ConnectionEvent_m.cc \
	messages/ConnectionEvent_m.h
$O/messages/EnsureLengthBurst.o: messages/EnsureLengthBurst.cc \
	messages/Burst_m.h \
	messages/IBurst.h \
	messages/EnsureLengthBurst.h
$O/messages/Burst_m.o: messages/Burst_m.cc \
	messages/Burst_m.h
$O/messages/IBurst.o: messages/IBurst.cc \
	messages/IBurst.h

