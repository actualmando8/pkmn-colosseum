    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r29, r4
    mr r28, r3
    li r3, 0x4
    mr r5, r29
    li r4, 0x0
    bl fn_801F02AC
    mr. r30, r3
    bne @L_801F016C
    li r3, 0x0
    b @L_801F01F0
    @L_801F016C:
    li r31, 0x0
    b @L_801F01D8
    @L_801F0174:
    clrlwi. r0, r31, 16
    beq @L_801F01D4
    lis r3, lbl_80375AC8@ha
    cmplw r5, r4
    clrlslwi r4, r31, 16, 3
    addi r0, r3, lbl_80375AC8@l
    add r3, r0, r4
    blt @L_801F0198
    li r3, 0x0
    @L_801F0198:
    cmplwi r3, 0x0
    beq @L_801F01D4
    bne @L_801F01AC
    li r0, 0x0
    b @L_801F01B0
    @L_801F01AC:
    lbz r0, 0x1(r3)
    @L_801F01B0:
    clrlwi. r0, r0, 24
    beq @L_801F01D4
    mr r3, r31
    mr r4, r30
    mr r5, r29
    bl fn_801F02AC
    cmplw r3, r28
    bne @L_801F01D4
    b @L_801F01EC
    @L_801F01D4:
    addi r31, r31, 0x1
    @L_801F01D8:
    lwz r4, lbl_80478D40(r13)
    clrlwi r5, r31, 16
    cmplw r5, r4
    blt @L_801F0174
    li r31, 0x0
    @L_801F01EC:
    mr r3, r31
    @L_801F01F0:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
