// ======================================================================
// \title  FirstDeploymentTopology.cpp
// \brief cpp file containing the topology instantiation code
//
// ======================================================================
// Provides access to autocoded functions
#include <HiNamespace/FirstDeployment/Top/FirstDeploymentTopologyAc.hpp>
// Note: Uncomment when using Svc:TlmPacketizer
//#include <HiNamespace/FirstDeployment/Top/FirstDeploymentPacketsAc.hpp>

// Necessary project-specified types
#include <Fw/Types/MallocAllocator.hpp>

// Zephyr devicetree access for the UART used by comDriver
#include <zephyr/device.h>
#include <zephyr/devicetree.h>

// Public functions for use in main program are namespaced with deployment module HiNamespace
// This is also the namespace where the topology components are instantiated by FPP.
namespace HiNamespace {

// Instantiate a malloc allocator for cmdSeq buffer allocation
Fw::MallocAllocator mallocator;

// The reference topology divides the incoming clock signal (1Hz) into sub-signals: 1Hz, 1/2Hz, and 1/4Hz with 0 offset
Svc::RateGroupDriver::DividerSet rateGroupDivisorsSet{{{1, 0}, {2, 0}, {4, 0}}};

// Rate groups may supply a context token to each of the attached children whose purpose is set by the project. The
// reference topology sets each token to zero as these contexts are unused in this project.
Svc::ActiveRateGroup::ContextArray rateGroup1Context(0);
Svc::ActiveRateGroup::ContextArray rateGroup2Context(0);
Svc::ActiveRateGroup::ContextArray rateGroup3Context(0);

enum TopologyConstants {
    COMM_BAUD_RATE = 115200,
};

// Set false by stopRateGroups() to end the startRateGroups() loop
static volatile bool s_cycling = true;

/**
 * \brief configure/setup components in project-specific way
 *
 * This is a *helper* function which configures/sets up each component requiring project specific input. This includes
 * allocating resources, passing-in arguments, etc. This function may be inlined into the topology setup function if
 * desired, but is extracted here for clarity.
 */
void configureTopology() {
    // Rate group driver needs a divisor list
    rateGroupDriver.configure(rateGroupDivisorsSet);

    // Rate groups require context arrays.
    rateGroup1.configure(rateGroup1Context);
    rateGroup2.configure(rateGroup2Context);
    rateGroup3.configure(rateGroup3Context);

    // Command sequencer needs to allocate memory to hold contents of command sequences
    cmdSeq.allocateBuffer(0, mallocator, 5 * 1024);

    // PrmDb file name must be supplied by the using topology
    FileHandling::prmDb.configure("PrmDb.dat");
}

void setupTopology(const TopologyState& state) {
    // Autocoded initialization. Function provided by autocoder.
    initComponents(state);
    // Autocoded id setup. Function provided by autocoder.
    setBaseIds();
    // Autocoded connection wiring. Function provided by autocoder.
    connectComponents();
    // Autocoded command registration. Function provided by autocoder.
    regCommands();
    // Autocoded configuration. Function provided by autocoder.
    configComponents(state);
    // UART for ground communication. Uses the devicetree's chosen console (USB CDC-ACM on the Pico 2 stand-in).
    // Must run after connectComponents() since configure() signals readiness on the `ready` port.
    comDriver.configure(DEVICE_DT_GET(DT_CHOSEN(zephyr_console)), COMM_BAUD_RATE);
    // Project-specific component configuration. Function provided above. May be inlined, if desired.
    configureTopology();
    // Autocoded parameter read from file. Function provided by autocoder.
    readParameters();
    // Autocoded parameter loading. Function provided by autocoder.
    loadParameters();
    // Autocoded task kick-off (active components). Function provided by autocoder.
    startTasks(state);
}

void startRateGroups(const Fw::TimeInterval& interval) {
    // The timer component drives the fundamental tick rate of the system.
    // Svc::RateGroupDriver will divide this down to the slower rate groups.
    // Each cycle() call blocks until the next timer expiry, then emits one tick.
    // This call will block until the stopRateGroups() call is made.
    timer.configure(interval.getSeconds() * 1000 + interval.getUSeconds() / 1000);
    timer.start();
    while (s_cycling) {
        timer.cycle();
    }
    timer.stop();
}

void stopRateGroups() {
    s_cycling = false;
}

void teardownTopology(const TopologyState& state) {
    // Autocoded (active component) task clean-up. Functions provided by topology autocoder.
    stopTasks(state);
    freeThreads(state);

    // Resource deallocation
    cmdSeq.deallocateBuffer(mallocator);

    tearDownComponents(state);
    deinitComponents(state);
}
};  // namespace HiNamespace
