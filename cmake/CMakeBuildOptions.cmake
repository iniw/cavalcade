# --- compiler options ---

# used by every build option
set(OPTIONS_GENERAL 

    /MP
    /bigobj

)
    
set(OPTIONS_DEBUG

    /MTd

)

set(OPTIONS_RELWITHDEBINFO 

    /O2
    /Ob2
    /Oi
    /Ot
    /Zi
    /MT
    /DNDEBUG

)

set(OPTIONS_RELEASE 

    /O2
    /Ob2
    /Oi
    /Ot
    /GL
    /DNDEBUG
    /MT

)

# --- linker options ---

set(LINKER_OPTIONS_DEBUG

    /debug
    /INCREMENTAL

)

set(LINKER_OPTIONS_RELWITHDEBINFO 

    /debug
    /INCREMENTAL

)

set(LINKER_OPTIONS_RELEASE 

    /LTCG:incremental
    /INCREMENTAL:NO
    /OPT:REF
    /OPT:ICF

)