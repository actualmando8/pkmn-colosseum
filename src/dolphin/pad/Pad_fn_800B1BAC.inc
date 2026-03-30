    nofralloc
    mflr r0
    mulli r7, r3, 0x110
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    lis r6, lbl_803FC620@ha
    addi r0, r6, lbl_803FC620@l
    stw r31, 0x1c(r1)
    add r9, r0, r7
    stw r30, 0x18(r1)
    lwz r0, 0x0(r9)
    cmpwi r0, 0x0
    bne @L_800B1BE4
    li r3, -0x3
    b @L_800B1CAC
    @L_800B1BE4:
    lwz r8, 0x88(r9)
    lhz r0, 0x6(r8)
    cmplw r0, r4
    bge @L_800B1BFC
    li r3, -0x9
    b @L_800B1CAC
    @L_800B1BFC:
    subf r0, r4, r0
    sth r0, 0x6(r8)
    lis r6, 0x1
    subi r12, r6, 0x1
    lhz r11, 0x8(r8)
    addi r0, r12, 0x0
    li r30, 0x0
    b @L_800B1C94
    @L_800B1C1C:
    lhz r10, 0x10(r9)
    addi r30, r30, 0x1
    clrlwi r6, r30, 16
    subi r7, r10, 0x5
    cmpw r7, r6
    bge @L_800B1C3C
    li r3, -0x6
    b @L_800B1CAC
    @L_800B1C3C:
    addi r11, r11, 0x1
    clrlwi r6, r11, 16
    cmplwi r6, 0x5
    blt @L_800B1C54
    cmplw r6, r10
    blt @L_800B1C58
    @L_800B1C54:
    li r11, 0x5
    @L_800B1C58:
    clrlslwi r6, r11, 16, 1
    add r7, r8, r6
    lhz r6, 0x0(r7)
    cmplwi r6, 0x0
    bne @L_800B1C94
    clrlwi r6, r12, 16
    cmplwi r6, 0xffff
    bne @L_800B1C80
    mr r12, r11
    b @L_800B1C88
    @L_800B1C80:
    clrlslwi r6, r31, 16, 1
    sthx r11, r8, r6
    @L_800B1C88:
    sth r0, 0x0(r7)
    addi r31, r11, 0x0
    subi r4, r4, 0x1
    @L_800B1C94:
    cmplwi r4, 0x0
    bne @L_800B1C1C
    sth r11, 0x8(r8)
    mr r4, r8
    sth r12, 0xbe(r9)
    bl fn_800B1D60
    @L_800B1CAC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
