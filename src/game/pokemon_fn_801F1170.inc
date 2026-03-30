    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_801F1190
    li r3, 0x0
    b @L_801F11B8
    @L_801F1190:
    bl fn_8020D950
    clrlwi. r0, r3, 16
    bne @L_801F11A4
    li r3, 0x0
    b @L_801F11B8
    @L_801F11A4:
    mr r3, r31
    bl fn_8020D920
    neg r3, r3
    subic r0, r3, 0x1
    subfe r3, r0, r3
    @L_801F11B8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
