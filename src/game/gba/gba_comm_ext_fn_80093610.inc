    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    cmpwi r3, 0x0
    blt @L_80093634
    cmpwi r3, 0x3
    ble @L_8009363C
@L_80093634:
    lis r3, 0x1
    b @L_80093680
@L_8009363C:
    lis r4, -32704
    rlwinm r0, r3, 2, 0, 29
    addi r3, r4, -19672
    lwzx r30, r3, r0
    cmplwi r30, 0x0
    bne @L_8009365C
    li r3, 0x0
    b @L_80093680
@L_8009365C:
    mr r3, r30
    bl fn_8009F7B4
    lwz r31, 0x433c(r30)
    mr r3, r30
    bl fn_8009F890
    addi r3, r30, 0x20
    li r4, 0x8
    bl fn_800A257C
    mr r3, r31
@L_80093680:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
