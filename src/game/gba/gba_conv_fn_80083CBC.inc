    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    cmplwi r3, 0x0
    beq @L_80083CD4
    b @L_80083CE0
    @L_80083CD4:
    li r3, 0x0
    li r4, 0xd
    bl fn_80129280
    @L_80083CE0:
    li r4, 0x0
    li r5, 0x49cc
    bl memset
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
