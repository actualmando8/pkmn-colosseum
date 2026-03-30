    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x1
    li r4, 0x0
    stw r0, 0x14(r1)
    bl fn_800E0E14
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801332B4
    lis r3, lbl_80272AE0@ha
    addi r3, r3, lbl_80272AE0@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801332C4
    @L_801332B4:
    lis r3, lbl_80272AF0@ha
    addi r3, r3, lbl_80272AF0@l
    crxor 6, 6, 6
    bl fn_800DD970
    @L_801332C4:
    lwz r0, 0x14(r1)
    li r3, 0x0
    mtlr r0
    addi r1, r1, 0x10
    blr
