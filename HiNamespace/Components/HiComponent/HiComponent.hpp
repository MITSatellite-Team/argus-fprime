// ======================================================================
// \title  HiComponent.hpp
// \author liamh
// \brief  hpp file for HiComponent component implementation class
// ======================================================================

#ifndef HiNamespace_HiComponent_HPP
#define HiNamespace_HiComponent_HPP

#include "HiNamespace/Components/HiComponent/HiComponentComponentAc.hpp"

namespace HiNamespace {

class HiComponent final : public HiComponentComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct HiComponent object
    HiComponent(const char* const compName  //!< The component name
    );

    //! Destroy HiComponent object
    ~HiComponent();

  private:
    // ----------------------------------------------------------------------
    // Handler implementations for commands
    // ----------------------------------------------------------------------

    //! Handler implementation for command SAY_HI
    //!
    //! Command to issue greeting with maximum length of 20 characters
    void SAY_HI_cmdHandler(FwOpcodeType opCode,              //!< The opcode
                           U32 cmdSeq,                       //!< The command sequence number
                           const Fw::CmdStringArg& greeting  //!< Greeting to repeat in the SayHiEvent event
                           ) override;

    U32 m_greetingCount = 0;
};

}  // namespace HiNamespace

#endif
