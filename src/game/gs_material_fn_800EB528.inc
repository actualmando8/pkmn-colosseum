    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r0, 0xc(r3)
    cmplwi r0, 0x0
    bne @L_800EB58C
    lwz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 14, 14
    bne @L_800EB58C
    lwz r3, 0x4(r31)
    lwz r3, 0x0(r3)
    bl fn_801A0FBC
    stw r3, 0xc(r31)
    lwz r3, 0x4(r31)
    lwz r3, 0x0(r3)
    bl fn_801A0FBC
    stw r3, 0x10(r31)
    lwz r3, 0x4(r31)
    lwz r3, 0x0(r3)
    bl fn_801A0FBC
    stw r3, 0x14(r31)
    lwz r3, 0xc(r31)
    bl fn_800EBEEC
    @L_800EB58C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
