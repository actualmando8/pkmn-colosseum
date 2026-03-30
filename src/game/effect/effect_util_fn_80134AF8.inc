    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r5
    stw r30, 0x8(r1)
    mr r30, r4
    bne @L_80134B28
    li r3, 0x0
    li r4, 0x3
    bl fn_80129280
    @L_80134B28:
    extsb. r0, r30
    blt @L_80134B3C
    extsb r4, r30
    cmpwi r4, 0x3
    blt @L_80134B44
    @L_80134B3C:
    li r31, 0x0
    b @L_80134B74
    @L_80134B44:
    extsb. r0, r31
    blt @L_80134B58
    extsb r0, r31
    cmpwi r0, 0x1e
    blt @L_80134B60
    @L_80134B58:
    li r31, 0x0
    b @L_80134B74
    @L_80134B60:
    mulli r4, r4, 0x24a4
    mulli r0, r0, 0x138
    add r4, r4, r0
    addi r31, r4, 0x14
    add r31, r3, r31
    @L_80134B74:
    cmplwi r31, 0x0
    bne @L_80134B84
    li r3, 0x0
    b @L_80134BA8
    @L_80134B84:
    mr r3, r31
    bl fn_80123FBC
    clrlwi. r0, r3, 24
    bne @L_80134B9C
    li r3, 0x0
    b @L_80134BA8
    @L_80134B9C:
    mr r3, r31
    bl fn_80124A60
    li r3, 0x1
    @L_80134BA8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
