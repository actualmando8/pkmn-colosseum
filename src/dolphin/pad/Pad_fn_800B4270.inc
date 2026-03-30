    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    mr r31, r4
    stw r30, 0x10(r1)
    addi r30, r3, 0x0
    lbz r0, 0x0(r4)
    cmplwi r0, 0xff
    bne @L_800B42A0
    li r3, -0x4
    b @L_800B42F0
    @L_800B42A0:
    lis r3, lbl_803FC840@ha
    lwz r4, 0x10c(r30)
    addi r0, r3, lbl_803FC840@l
    cmplw r4, r0
    beq @L_800B42E4
    addi r3, r31, 0x0
    li r5, 0x4
    bl fn_800C80D0
    cmpwi r3, 0x0
    bne @L_800B42EC
    lwz r4, 0x10c(r30)
    addi r3, r31, 0x4
    li r5, 0x2
    addi r4, r4, 0x4
    bl fn_800C80D0
    cmpwi r3, 0x0
    bne @L_800B42EC
    @L_800B42E4:
    li r3, 0x0
    b @L_800B42F0
    @L_800B42EC:
    li r3, -0xa
    @L_800B42F0:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
