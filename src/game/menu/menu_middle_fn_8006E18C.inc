    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    lis r3, lbl_80268574@ha
    li r30, 0x0
    addi r31, r3, lbl_80268574@l
    @L_8006E1B4:
    lhz r4, 0x0(r31)
    mr r3, r29
    bl fn_801046C8
    lbz r4, 0x95(r29)
    lbz r0, 0x2(r31)
    extsb r4, r4
    subf r0, r4, r0
    cntlzw r0, r0
    srwi r0, r0, 5
    clrlwi r4, r0, 24
    bl fn_80109220
    addi r31, r31, 0x4
    addi r30, r30, 0x1
    cmplwi r30, 0x6
    blt @L_8006E1B4
    lbz r0, 0x1(r29)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8006E204
    b @L_8006E22C
    @L_8006E204:
    bl fn_80071160
    cmpwi r3, 0x0
    beq @L_8006E22C
    lwz r3, 0x4(r29)
    li r4, 0x1ce
    bl fn_80107F38
    li r0, 0x1
    stb r0, 0x98(r29)
    stb r0, 0x99(r29)
    b @L_8006E23C
    @L_8006E22C:
    mr r3, r29
    li r4, 0x0
    li r5, 0x0
    bl fn_80070D84
    @L_8006E23C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
