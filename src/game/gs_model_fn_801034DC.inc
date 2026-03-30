    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r31, r4
    addi r30, r3, 0x1
    li r29, 0x0
    mr r3, r30
    bl fn_8008ABA0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_8010351C
    li r3, 0x0
    b @L_801035F8
@L_8010351C:
    mr r3, r30
    bl fn_8008AB8C
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r3, 0, 25, 25
    cmpwi r0, 0x0
    beq @L_8010353C
    ori r0, r29, 0x1
    rlwinm r29, r0, 0, 16, 31
@L_8010353C:
    rlwinm r0, r3, 0, 24, 24
    cmpwi r0, 0x0
    beq @L_80103550
    ori r0, r29, 0x2
    rlwinm r29, r0, 0, 16, 31
@L_80103550:
    rlwinm r0, r3, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_80103564
    ori r0, r29, 0x4
    rlwinm r29, r0, 0, 16, 31
@L_80103564:
    rlwinm r0, r3, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_80103578
    ori r0, r29, 0x8
    rlwinm r29, r0, 0, 16, 31
@L_80103578:
    rlwinm r0, r3, 0, 31, 31
    cmpwi r0, 0x0
    beq @L_8010358C
    ori r0, r29, 0x10
    rlwinm r29, r0, 0, 16, 31
@L_8010358C:
    rlwinm r0, r3, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_801035A0
    ori r0, r29, 0x20
    rlwinm r29, r0, 0, 16, 31
@L_801035A0:
    rlwinm r0, r3, 0, 29, 29
    cmpwi r0, 0x0
    beq @L_801035B4
    ori r0, r29, 0x100
    rlwinm r29, r0, 0, 16, 31
@L_801035B4:
    rlwinm r0, r3, 0, 22, 22
    cmpwi r0, 0x0
    beq @L_801035C8
    ori r0, r29, 0x200
    rlwinm r29, r0, 0, 16, 31
@L_801035C8:
    rlwinm r0, r3, 0, 23, 23
    cmpwi r0, 0x0
    beq @L_801035DC
    ori r0, r29, 0x400
    rlwinm r29, r0, 0, 16, 31
@L_801035DC:
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_801035F0
    ori r0, r29, 0x800
    rlwinm r29, r0, 0, 16, 31
@L_801035F0:
    sth r29, 0x0(r31)
    li r3, 0x1
@L_801035F8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
