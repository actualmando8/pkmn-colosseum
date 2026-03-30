    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r28, r3
    li r29, 0x1
    li r30, 0x0
    b @L_801F22B4
    @L_801F223C:
    mr r3, r28
    mr r6, r30
    li r4, 0x0
    li r5, 0x59
    bl fn_801F54A4
    mr. r31, r3
    beq @L_801F22B0
    bl fn_802062FC
    clrlwi. r0, r3, 24
    beq @L_801F22B0
    mr r3, r31
    li r4, 0x0
    li r5, 0xfe
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_801F22B0
    bl fn_801F1170
    clrlwi. r0, r3, 24
    beq @L_801F22B0
    mr r3, r31
    li r4, 0x0
    li r5, 0x112
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_801F22B0
    li r29, 0x0
    b @L_801F22C0
    @L_801F22B0:
    addi r30, r30, 0x1
    @L_801F22B4:
    clrlwi r0, r30, 16
    cmplwi r0, 0x8
    blt @L_801F223C
    @L_801F22C0:
    mr r3, r29
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
