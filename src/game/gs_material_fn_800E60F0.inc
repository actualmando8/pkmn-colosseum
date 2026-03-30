    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stfd f31, 0x18(r1)
    fmr f31, f1
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    stw r29, 0xc(r1)
    mr r29, r3
    lhz r0, 0x150(r3)
    cmplwi r0, 0x0
    bne @L_800E6128
    li r0, 0x0
    b @L_800E6158
    @L_800E6128:
    lwz r3, 0x14c(r29)
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    bne @L_800E6140
    li r0, 0x0
    b @L_800E6158
    @L_800E6140:
    bl fn_800DF240
    rlwinm. r0, r3, 0, 29, 29
    beq @L_800E6154
    li r0, 0x1
    b @L_800E6158
    @L_800E6154:
    li r0, 0x0
    @L_800E6158:
    clrlwi. r0, r0, 24
    beq @L_800E619C
    lhz r30, 0x150(r29)
    cmpwi r30, 0x0
    beq @L_800E619C
    lwz r29, 0x14c(r29)
    li r31, 0x0
    b @L_800E6194
    @L_800E6178:
    lwz r3, 0x0(r29)
    cmplwi r3, 0x0
    beq @L_800E618C
    fmr f1, f31
    bl fn_800DF1B8
    @L_800E618C:
    addi r31, r31, 0x1
    addi r29, r29, 0x4
    @L_800E6194:
    cmpw r31, r30
    blt @L_800E6178
    @L_800E619C:
    lwz r0, 0x24(r1)
    lfd f31, 0x18(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    lwz r29, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
