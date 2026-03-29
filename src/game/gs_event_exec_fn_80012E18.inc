    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r30, r3
    bl fn_80105624
    lhz r29, 0x6(r3)
    mr r3, r30
    li r4, 0x2
    bl fn_801040D0
    extsb r31, r3
    lwz r3, 0x4(r30)
    bl fn_8005D9E4
    extsb r6, r3
    xor r0, r31, r6
    srawi r3, r0, 1
    and r0, r0, r31
    subf r0, r0, r3
    srwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_80012E78
    mr r31, r6
    @L_80012E78:
    cmpwi r0, 0x0
    bne @L_80012E84
    mr r6, r31
    @L_80012E84:
    clrlwi r4, r29, 16
    lhz r3, 0x94(r30)
    clrlwi r0, r4, 31
    cmpwi r0, 0x0
    sth r3, 0x8(r1)
    beq @L_80012EAC
    lbz r3, 0x9(r1)
    subi r0, r3, 0x1
    stb r0, 0x9(r1)
    b @L_80012EC4
    @L_80012EAC:
    rlwinm r0, r4, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80012EC4
    lbz r3, 0x9(r1)
    addi r0, r3, 0x1
    stb r0, 0x9(r1)
    @L_80012EC4:
    lbz r5, 0x9(r1)
    extsb r0, r5
    cmpwi r0, 0x0
    bge @L_80012F18
    lbz r3, 0x8(r1)
    li r0, 0x0
    stb r0, 0x9(r1)
    add r3, r3, r5
    clrlwi r0, r3, 24
    stb r3, 0x8(r1)
    extsb r0, r0
    cmpwi r0, 0x0
    bge @L_80012F70
    extsb r4, r6
    subi r3, r4, 0x1
    subf r0, r4, r31
    extsb r3, r3
    extsb r0, r0
    stb r3, 0x9(r1)
    stb r0, 0x8(r1)
    b @L_80012F70
    @L_80012F18:
    extsb r3, r6
    cmpw r0, r3
    blt @L_80012F70
    subi r6, r3, 0x1
    lbz r4, 0x8(r1)
    subf r3, r6, r5
    extsb r0, r31
    add r4, r4, r3
    extsb r3, r6
    extsb r5, r4
    stb r3, 0x9(r1)
    clrlwi r4, r5, 24
    clrlwi r3, r3, 24
    extsb r4, r4
    stb r5, 0x8(r1)
    extsb r3, r3
    add r3, r4, r3
    cmpw r3, r0
    blt @L_80012F70
    li r0, 0x0
    stb r0, 0x8(r1)
    stb r0, 0x9(r1)
    @L_80012F70:
    lhz r5, 0x8(r1)
    li r3, 0x0
    lbz r4, 0x8(r1)
    lbz r0, 0x9(r1)
    sth r5, 0x94(r30)
    extsb r4, r4
    extsb r0, r0
    add r0, r4, r0
    stw r0, 0x80(r30)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
