    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    rlwinm r0, r4, 0, 16, 31
    cmplwi r0, 0x27e
    ble @L_800D21E4
    li r4, 0x27e
@L_800D21E4:
    rlwinm r0, r5, 0, 16, 31
    cmplwi r0, 0x1de
    ble @L_800D21F4
    li r5, 0x1de
@L_800D21F4:
    rlwinm r0, r6, 0, 16, 31
    cmplwi r0, 0x27f
    ble @L_800D2204
    li r6, 0x27f
@L_800D2204:
    rlwinm r0, r7, 0, 16, 31
    cmplwi r0, 0x1df
    ble @L_800D2214
    li r7, 0x1df
@L_800D2214:
    addi r6, r6, 0x1
    addi r0, r7, 0x1
    sth r4, 0x8(r1)
    addi r4, r1, 0x8
    sth r5, 0xc(r1)
    sth r6, 0xa(r1)
    sth r0, 0xe(r1)
    lwz r3, 0xc(r3)
    bl fn_80194400
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
