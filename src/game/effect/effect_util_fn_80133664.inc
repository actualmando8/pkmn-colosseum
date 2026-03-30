    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r3
    stw r29, 0x14(r1)
    bl fn_80105624
    lhz r29, 0x6(r3)
    mr r3, r30
    li r4, 0x0
    bl fn_80133B50
    extsb r31, r3
    lwz r3, 0x4(r30)
    bl fn_8005D9E4
    extsb r5, r3
    cmpw r31, r5
    bge @L_801336B0
    mr r5, r31
    @L_801336B0:
    lhz r3, 0x94(r30)
    clrlwi. r0, r29, 31
    clrlwi r0, r29, 16
    sth r3, 0x8(r1)
    beq @L_801336D4
    lbz r3, 0x9(r1)
    subi r0, r3, 0x1
    stb r0, 0x9(r1)
    b @L_801336E8
    @L_801336D4:
    rlwinm. r0, r0, 0, 30, 30
    beq @L_801336E8
    lbz r3, 0x9(r1)
    addi r0, r3, 0x1
    stb r0, 0x9(r1)
    @L_801336E8:
    lbz r4, 0x9(r1)
    extsb. r0, r4
    bge @L_8013372C
    lbz r3, 0x8(r1)
    li r0, 0x0
    stb r0, 0x9(r1)
    add r3, r3, r4
    clrlwi r0, r3, 24
    stb r3, 0x8(r1)
    extsb. r0, r0
    bge @L_8013377C
    extsb r4, r5
    subi r3, r4, 0x1
    subf r0, r4, r31
    stb r3, 0x9(r1)
    stb r0, 0x8(r1)
    b @L_8013377C
    @L_8013372C:
    extsb r0, r4
    extsb r3, r5
    cmpw r0, r3
    blt @L_8013377C
    subi r5, r3, 0x1
    lbz r3, 0x8(r1)
    subf r0, r5, r4
    stb r5, 0x9(r1)
    add r4, r3, r0
    clrlwi r0, r5, 24
    stb r4, 0x8(r1)
    clrlwi r3, r4, 24
    extsb r3, r3
    extsb r0, r0
    add r0, r3, r0
    cmpw r0, r31
    blt @L_8013377C
    li r0, 0x0
    stb r0, 0x8(r1)
    stb r0, 0x9(r1)
    @L_8013377C:
    lhz r0, 0x8(r1)
    sth r0, 0x94(r30)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
