    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    li r31, 0x0
@L_80076350:
    mr r3, r30
    mr r4, r31
    bl fn_80076398
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80076370
    li r3, 0x0
    b @L_80076380
@L_80076370:
    addi r31, r31, 0x1
    cmpwi r31, 0x6
    blt @L_80076350
    li r3, 0x1
@L_80076380:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
