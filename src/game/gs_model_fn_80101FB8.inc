    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bge @L_80101FD4
    li r3, 0x1
@L_80101FD4:
    lis r4, fn_80101B34@ha
    stb r3, lbl_80478B28(r13)
    addi r3, r4, fn_80101B34@l
    bl fn_800B8FD8
    stw r3, lbl_8047ACF4(r13)
    li r3, 0x22
    li r4, 0x16
    bl fn_800BD91C
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

