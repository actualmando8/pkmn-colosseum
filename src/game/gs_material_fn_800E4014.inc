    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    clrlwi. r0, r4, 24
    beq @L_800E404C
    lwz r0, 0x0(r3)
    ori r0, r0, 0x2
    stw r0, 0x0(r3)
    lwz r3, 0x144(r3)
    cmplwi r3, 0x0
    beq @L_800E406C
    li r4, 0x1
    bl fn_80118C20
    b @L_800E406C
    @L_800E404C:
    lwz r0, 0x0(r3)
    rlwinm r0, r0, 0, 31, 29
    stw r0, 0x0(r3)
    lwz r3, 0x144(r3)
    cmplwi r3, 0x0
    beq @L_800E406C
    li r4, 0x0
    bl fn_80118C20
    @L_800E406C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
