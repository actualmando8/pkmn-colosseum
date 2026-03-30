    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    lis r3, lbl_80408378@ha
    addi r31, r3, lbl_80408378@l
    lbz r0, 0x51(r31)
    cmplwi r0, 0x0
    beq @L_80114028
    lwz r30, 0x48(r31)
    lwz r29, 0x4c(r31)
    lbz r28, 0x50(r31)
    b @L_80114060
@L_80114028:
    li r3, 0x0
    li r4, 0x5
    bl fn_80135168
    mr r0, r3
    li r3, 0x0
    mr r30, r0
    li r4, 0x7
    bl fn_80135168
    mr r0, r3
    li r3, 0x0
    mr r29, r0
    li r4, 0x8
    bl fn_80135168
    clrlwi r28, r3, 24
@L_80114060:
    cmplwi r30, 0x0
    beq @L_80114094
    bl fn_800FF56C
    lis r4, lbl_80408378@ha
    clrlwi r0, r28, 24
    addi r5, r4, lbl_80408378@l
    li r4, 0x1
    stw r3, 0x0(r5)
    mr r3, r30
    stw r30, 0x4(r5)
    stb r4, 0x8(r5)
    stw r0, 0xc(r5)
    bl fn_800FF58C
@L_80114094:
    lis r3, lbl_80408378@ha
    li r0, 0x0
    addi r3, r3, lbl_80408378@l
    stb r0, 0x51(r31)
    stw r29, 0x0(r3)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
