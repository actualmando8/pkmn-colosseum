    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    bl fn_80101A70
    mr r31, r3
    mr r3, r30
    bl fn_80101A9C
    mr r30, r3
    cmpw r31, r30
    bne @L_80101AFC
    b @L_80101B1C
@L_80101AFC:
    bl fn_800E0C54
    subf r4, r30, r31
    rlwinm r5, r3, 0, 16, 31
    addi r3, r4, 0x1
    divw r0, r5, r3
    mullw r0, r0, r3
    subf r0, r0, r5
    add r3, r30, r0
@L_80101B1C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
