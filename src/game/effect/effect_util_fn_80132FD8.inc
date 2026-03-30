    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    b @L_80132FF0
    @L_80132FE8:
    bl fn_801E1810
    bl fn_800F0308
    @L_80132FF0:
    bl fn_801E1874
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80132FE8
    li r3, 0x2
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, -0x1
    beq @L_8013303C
    lis r4, lbl_80272AA8@ha
    mr r6, r3
    addi r4, r4, lbl_80272AA8@l
    addi r3, r1, 0x8
    la r5, lbl_8047D0E8(r2)
    crxor 6, 6, 6
    bl fn_800C8520
    addi r3, r1, 0x8
    li r4, 0x0
    bl fn_801E189C
    @L_8013303C:
    lwz r0, 0x24(r1)
    li r3, 0x0
    mtlr r0
    addi r1, r1, 0x20
    blr
