    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x34
    stw r0, 0x14(r1)
    li r6, 0x0
    stw r31, 0xc(r1)
    bl fn_801F54A4
    clrlwi r31, r3, 24
    bl fn_80077B84
    cmplwi r31, 0x1
    bne @L_801F1740
    cmpwi r3, 0x0
    ble @L_801F1740
    li r3, 0x1
    b @L_801F1744
    @L_801F1740:
    li r3, 0x0
    @L_801F1744:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
