typedef void (*DtorFunc)(void);

extern void __destroy_global_chain(void);

#pragma section ".dtors$10"
__declspec(section ".dtors$10") DtorFunc const __destroy_global_chain_reference =
    __destroy_global_chain;
