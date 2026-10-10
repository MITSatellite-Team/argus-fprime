// ======================================================================
// \title  Main.cpp
// \brief main program for the F' application. Intended for Zephyr RTOS targets
//
// ======================================================================
// Used to access topology functions
#include <HiNamespace/FirstDeployment/Top/FirstDeploymentTopology.hpp>
// OSAL initialization
#include <Os/Os.hpp>
// Used for logging to the console
#include <Fw/Logger/Logger.hpp>

/**
 * \brief execute the program
 *
 * This F´ program is designed to run on an embedded Zephyr target. There is no command line, and the program does not
 * exit: the rate group loop runs forever.
 *
 * @return: 0 on success, something else on failure
 */
int main() {
    Os::init();

    // Object for communicating state to the topology
    HiNamespace::TopologyState inputs;

    // Setup and cycle topology
    HiNamespace::setupTopology(inputs);
    HiNamespace::startRateGroups(Fw::TimeInterval(1,0));  // Program loop cycling rate groups at 1Hz

    // Only reached if stopRateGroups() is called
    HiNamespace::teardownTopology(inputs);
    Fw::Logger::log("Exiting...\n");
    return 0;
}
