    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    mr r0, r3
    mulli r3, r0, 0x128
    stw r0, -23984(r13)
    bl fn_800E3534
    rlwinm. r0, r3, 0, 16, 31
    sth r3, -23992(r13)
    beq @L_800D2B34
    mr r3, r0
    bl fn_800E27B0
    li r5, 0x0
    stw r3, -23988(r13)
    mr r4, r5
    li r6, 0x0
    b @L_800D2B28
@L_800D2B18:
    lwz r3, -23988(r13)
    addi r6, r6, 0x1
    stbx r4, r3, r5
    addi r5, r5, 0x128
@L_800D2B28:
    lwz r0, -23984(r13)
    cmplw r6, r0
    blt @L_800D2B18
@L_800D2B34:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
