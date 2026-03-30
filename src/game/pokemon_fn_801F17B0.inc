    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r3
    li r27, 0x0
    b @L_801F1868
    @L_801F17CC:
    mr r3, r26
    mr r6, r27
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    mr. r31, r3
    beq @L_801F1864
    li r29, 0x0
    b @L_801F1858
    @L_801F17F0:
    mr r3, r31
    mr r6, r29
    li r4, 0x0
    li r5, 0x7
    bl fn_801F76B8
    mr. r30, r3
    beq @L_801F1854
    li r28, 0x0
    b @L_801F1848
    @L_801F1814:
    mr r3, r30
    mr r6, r28
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    cmplwi r3, 0x0
    beq @L_801F1844
    li r4, 0x0
    li r5, 0xfa
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    @L_801F1844:
    addi r28, r28, 0x1
    @L_801F1848:
    clrlwi r0, r28, 16
    cmplwi r0, 0x2
    blt @L_801F1814
    @L_801F1854:
    addi r29, r29, 0x1
    @L_801F1858:
    clrlwi r0, r29, 16
    cmplwi r0, 0x2
    blt @L_801F17F0
    @L_801F1864:
    addi r27, r27, 0x1
    @L_801F1868:
    clrlwi r0, r27, 16
    cmplwi r0, 0x2
    blt @L_801F17CC
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
