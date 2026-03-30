    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    mr r7, r3
    stw r0, 0x24(r1)
    li r0, 0x0
    mulli r3, r7, 0x14
    stw r31, 0x1c(r1)
    mr r31, r6
    stw r30, 0x18(r1)
    mr r30, r5
    stw r29, 0x14(r1)
    mr r29, r4
    stw r0, -23376(r13)
    stw r0, -23372(r13)
    stw r7, -23416(r13)
    bl fn_800E3534
    rlwinm. r0, r3, 0, 16, 31
    sth r3, -23424(r13)
    beq @L_800FF954
    mr r3, r0
    bl fn_800E27B0
    add r0, r30, r31
    stw r3, -23420(r13)
    add r0, r29, r0
    mulli r3, r0, 0x24
    stw r29, -23404(r13)
    stw r30, -23400(r13)
    stw r31, -23396(r13)
    stw r0, -23392(r13)
    bl fn_800E3534
    rlwinm. r0, r3, 0, 16, 31
    sth r3, -23412(r13)
    beq @L_800FF954
    mr r3, r0
    bl fn_800E27B0
    li r5, 0x0
    stw r3, -23408(r13)
    mr r4, r5
    li r6, 0x0
    b @L_800FF8DC
@L_800FF8C8:
    lwz r3, -23408(r13)
    addi r0, r5, 0x8
    addi r5, r5, 0x24
    addi r6, r6, 0x1
    stwx r4, r3, r0
@L_800FF8DC:
    lwz r0, -23392(r13)
    cmplw r6, r0
    blt @L_800FF8C8
    lwz r9, -23420(r13)
    li r10, 0x0
    lis r3, -32752
    stw r10, -23380(r13)
    addi r8, r3, -1680
    li r0, -0x1
    stw r10, -23388(r13)
    li r3, 0x0
    li r4, 0x7d0
    li r5, 0x4000
    stw r9, -23384(r13)
    li r6, 0x1
    li r7, 0x1
    stw r10, 0x0(r9)
    lwz r9, -23384(r13)
    stw r10, 0x4(r9)
    lwz r9, -23384(r13)
    sth r10, 0x8(r9)
    lwz r9, -23384(r13)
    stb r10, 0xa(r9)
    lwz r9, -23384(r13)
    stb r10, 0xb(r9)
    lwz r9, -23384(r13)
    stw r10, 0x10(r9)
    stw r10, -23368(r13)
    stw r0, -32008(r13)
    bl fn_800F07A8
@L_800FF954:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
