    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    bl fn_800FF52C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8000C008
    li r3, 0x26
    bl fn_80166A28
    li r30, 0x0
    lis r3, lbl_802666B0@ha
    addi r31, r3, lbl_802666B0@l
    @L_8000BFD8:
    mr r6, r31
    li r3, 0xc8
    li r4, 0xf0
    li r5, -0x1
    crxor 6, 6, 6
    bl fn_800FAEF8
    bl fn_800F0308
    addi r30, r30, 0x1
    cmpwi r30, 0xf
    blt @L_8000BFD8
    li r3, 0x0
    b @L_8000C054
    @L_8000C008:
    li r3, 0x1
    bl fn_800F7BC4
    rlwinm r0, r3, 0, 26, 26
    cmplwi r0, 0x0
    beq @L_8000C050
    addi r4, r1, 0x8
    li r3, 0x0
    bl fn_8001E3E0
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8000C03C
    li r3, -0x1
    b @L_8000C054
    @L_8000C03C:
    lwz r3, 0x8(r1)
    li r4, 0x0
    bl fn_80113828
    li r3, 0x0
    b @L_8000C054
    @L_8000C050:
    li r3, 0x1
    @L_8000C054:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
