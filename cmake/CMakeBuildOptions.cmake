# --- compiler options ---

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