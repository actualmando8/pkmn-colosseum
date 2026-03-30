    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x19
    li r4, 0x0
    stw r0, 0x14(r1)
    bl fn_801F025C
    li r4, 0x14
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80214B3C
    lis r3, lbl_80379F58@ha
    addi r3, r3, lbl_80379F58@l
    addis r3, r3, 0x1
    lbz r0, 0x609b(r3)
    ori r0, r0, 0x80
    stb r0, 0x609b(r3)
@L_80214B3C:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
