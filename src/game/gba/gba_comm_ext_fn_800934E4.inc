    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    cmpwi r3, 0x0
    blt @L_80093508
    cmpwi r3, 0x3
    ble @L_80093510
@L_80093508:
    li r3, 0x0
    b @L_8009355C
@L_80093510:
    lis r4, -32704
    rlwinm r0, r3, 2, 0, 29
    addi r3, r4, -19672
    lwzx r30, r3, r0
    cmplwi r30, 0x0
    beq @L_80093554
    mr r3, r30
    bl fn_8009F7B4
    lwz r0, 0x4340(r30)
    mr r3, r30
    cntlzw r0, r0
    rlwinm r31, r0, 27, 5, 31
    bl fn_8009F890
    addi r3, r30, 0x20
    li r4, 0x8
    bl fn_800A257C
    b @L_80093558
@L_80093554:
    li r31, 0x1
@L_80093558:
    mr r3, r31
@L_8009355C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
