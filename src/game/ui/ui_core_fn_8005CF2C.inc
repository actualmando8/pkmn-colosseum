    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r31, r3
    mr r30, r4
    li r3, 0xbe
    li r4, 0x0
    li r5, 0x0
    li r6, 0x10
    li r7, 0x0
    li r8, 0x1
    li r9, 0xf5
    crxor 6, 6, 6
    bl fn_801026A4
    bl fn_8006B420
    stw r3, 0x8(r1)
    li r0, 0x0
    mr r9, r31
    mr r10, r30
    stw r0, 0xc(r1)
    li r3, 0xda
    li r4, 0x0
    li r5, 0x0
    li r6, 0x10
    li r7, 0x0
    li r8, 0x4
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0xda
    li r4, 0x0
    li r5, -0x28
    bl fn_80102868
    bl fn_8006B420
    mr r4, r3
    mr r3, r31
    bl fn_80076054
    mr r31, r3
    clrlwi r0, r31, 16
    cmplwi r0, 0x0
    bne @L_8005CFEC
    lis r3, lbl_802678D8@ha
    lis r5, lbl_80267A20@ha
    addi r3, r3, lbl_802678D8@l
    li r4, 0x1bb
    addi r5, r5, lbl_80267A20@l
    bl fn_80196E10
@L_8005CFEC:
    li r3, 0x26
    bl fn_80166A28
    clrlwi r4, r31, 16
    li r3, 0x7
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    cmpwi r30, 0x0
    beq @L_8005D01C
    blt @L_8005D034
    cmpwi r30, 0x3
    b @L_8005D034
@L_8005D01C:
    li r3, 0x7
    li r4, 0x440a
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    b @L_8005D048
@L_8005D034:
    li r3, 0x7
    li r4, 0x3c4e
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
@L_8005D048:
    li r3, 0xda
    li r4, 0x0
    li r5, 0x0
    bl fn_80102568
    li r3, 0xbe
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0xda
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    bl fn_8006E0CC
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
