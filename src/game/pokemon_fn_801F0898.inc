    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_801F08B8
    li r0, 0x0
    b @L_801F08E8
    @L_801F08B8:
    bl fn_8020D950
    clrlwi. r0, r3, 16
    bne @L_801F08CC
    li r0, 0x0
    b @L_801F08E8
    @L_801F08CC:
    mr r3, r31
    bl fn_8020D920
    cmplwi r3, 0x0
    bne @L_801F08E4
    li r0, 0x0
    b @L_801F08E8
    @L_801F08E4:
    li r0, 0x1
    @L_801F08E8:
    clrlwi. r0, r0, 24
    bne @L_801F08F8
    li r3, 0x0
    b @L_801F0914
    @L_801F08F8:
    mr r3, r31
    bl fn_8020D920
    cmplwi r3, 0x0
    bne @L_801F0910
    li r3, 0x0
    b @L_801F0914
    @L_801F0910:
    bl fn_8020D82C
    @L_801F0914:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
