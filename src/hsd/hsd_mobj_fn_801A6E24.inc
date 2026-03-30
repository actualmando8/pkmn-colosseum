    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    mr r30, r3
    stw r29, 0x24(r1)
    bl fn_801B3884
    lwz r6, 0xc(r30)
    addi r5, r1, 0x8
    lwz r31, 0x4(r30)
    addi r4, r1, 0xc
    lwz r0, 0x8(r6)
    addi r3, r1, 0x10
    stw r0, 0x8(r1)
    lwz r6, 0xc(r30)
    lwz r0, 0x4(r6)
    stw r0, 0xc(r1)
    lwz r6, 0xc(r30)
    lwz r0, 0x0(r6)
    stw r0, 0x10(r1)
    lwz r6, 0xc(r30)
    lfs f1, 0xc(r6)
    bl fn_801B28C8
    rlwinm. r0, r31, 0, 28, 28
    beq @L_801A6E98
    lwz r3, 0xc(r30)
    lfs f1, 0x10(r3)
    bl fn_801B28B8
    @L_801A6E98:
    lwz r3, 0x8(r30)
    rlwinm. r0, r31, 0, 5, 5
    li r29, 0x0
    stw r3, 0x14(r1)
    beq @L_801A6EDC
    lwz r0, lbl_8047B2DC(r13)
    cmplwi r0, 0x0
    beq @L_801A6EDC
    addi r29, r1, 0x14
    b @L_801A6EC8
    @L_801A6EC0:
    lwz r3, 0x0(r29)
    addi r29, r3, 0x8
    @L_801A6EC8:
    lwz r0, 0x0(r29)
    cmplwi r0, 0x0
    bne @L_801A6EC0
    lwz r0, lbl_8047B2DC(r13)
    stw r0, 0x0(r29)
    @L_801A6EDC:
    rlwinm. r0, r31, 0, 19, 19
    beq @L_801A6F14
    lwz r0, lbl_8047B2D8(r13)
    cmplwi r0, 0x0
    beq @L_801A6F14
    lwz r3, lbl_8047B2D8(r13)
    lwz r0, 0x58(r3)
    cmplwi r0, 0x0
    beq @L_801A6F14
    lwz r0, 0x14(r1)
    lwz r3, lbl_8047B2D8(r13)
    stw r0, 0x8(r3)
    lwz r0, lbl_8047B2D8(r13)
    stw r0, 0x14(r1)
    @L_801A6F14:
    lwz r3, 0x14(r1)
    bl fn_801BBFE4
    lwz r3, 0x14(r1)
    bl fn_801BDA58
    lwz r6, 0x0(r30)
    mr r3, r30
    mr r5, r31
    lwz r4, 0x14(r1)
    lwz r12, 0x48(r6)
    mtctr r12
    bctrl
    lwz r4, 0x10(r30)
    mr r3, r31
    bl fn_801B294C
    cmplwi r29, 0x0
    beq @L_801A6F5C
    li r0, 0x0
    stw r0, 0x0(r29)
    @L_801A6F5C:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
