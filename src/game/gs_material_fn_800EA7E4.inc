    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    beq @L_800EA810
    lwz r0, 0x14(r3)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_800EA810
    bl fn_800EA820
    @L_800EA810:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
