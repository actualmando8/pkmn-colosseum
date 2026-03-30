    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    mr r5, r4
    cmpwi r5, 0x0
    ble @L_801CAA94
    rlwinm r4, r3, 0, 16, 31
    rlwinm r5, r5, 0, 16, 31
    li r3, 0x0
    li r6, -0x1
    bl fn_80129A78
    mr r0, r3
    b @L_801CAAB4
@L_801CAA94:
    bge @L_801CAAB4
    neg r5, r5
    rlwinm r4, r3, 0, 16, 31
    rlwinm r5, r5, 0, 16, 31
    li r3, 0x0
    li r6, -0x1
    bl fn_801299C8
    mr r0, r3
@L_801CAAB4:
    mr r3, r0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
