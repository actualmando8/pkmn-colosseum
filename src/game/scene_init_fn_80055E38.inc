    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    lis r4, lbl_8026768C@ha
    cmpwi r3, 0x0
    addi r6, r4, lbl_8026768C@l
    lwz r5, 0x0(r6)
    lwz r4, 0x4(r6)
    lwz r0, 0x8(r6)
    stw r5, 0x8(r1)
    stw r4, 0xc(r1)
    stw r0, 0x10(r1)
    blt @L_80055E74
    cmpwi r3, 0x3
    blt @L_80055E7C
    @L_80055E74:
    li r3, -0x1
    b @L_80055E88
    @L_80055E7C:
    slwi r0, r3, 2
    addi r3, r1, 0x8
    lwzx r3, r3, r0
    @L_80055E88:
    cmpwi r3, 0x0
    bge @L_80055E98
    li r3, 0x1
    b @L_80055EA8
    @L_80055E98:
    bl fn_801070F4
    clrlwi r0, r3, 24
    cntlzw r0, r0
    srwi r3, r0, 5
    @L_80055EA8:
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
