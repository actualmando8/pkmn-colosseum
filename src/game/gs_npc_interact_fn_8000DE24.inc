    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_80102ED4
    li r3, 0x0
    bl fn_801F18DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000DE94
    li r3, 0x0
    bl fn_801F1700
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8000DE7C
    bl fn_80265924
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8000DE7C
    li r0, 0x1
    b @L_8000DE98
    @L_8000DE7C:
    bl fn_801EF634
    clrlwi r0, r3, 16
    cmplwi r0, 0x1
    bne @L_8000DE94
    li r0, 0x1
    b @L_8000DE98
    @L_8000DE94:
    li r0, 0x0
    @L_8000DE98:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    beq @L_8000DEB0
    li r0, 0x1
    stb r0, 0x98(r31)
    stb r0, 0x99(r31)
    @L_8000DEB0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
