    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0x43
    li r6, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    li r4, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x24
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802387B0
    mr r3, r31
    bl fn_80205BE8
    bl fn_80122DDC
    b @L_802387B4
@L_802387B0:
    li r3, 0x1
@L_802387B4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
