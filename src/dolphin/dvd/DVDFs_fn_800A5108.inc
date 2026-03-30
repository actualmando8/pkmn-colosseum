    nofralloc
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x4(r1)
    stwu r1, -0x30(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    addi r30, r5, 0x0
    stw r29, 0x24(r1)
    addi r29, r4, 0x0
    stw r28, 0x20(r1)
    bne @L_800A513C
    li r3, 0x0
    b @L_800A5248
@L_800A513C:
    lwz r4, -24660(r13)
    mulli r3, r3, 0xc
    lwz r6, -24656(r13)
    addi r5, r4, 0x4
    lwzx r0, r4, r3
    lwzx r3, r5, r3
    rlwinm r0, r0, 0, 8, 31
    cmplwi r3, 0x0
    add r31, r6, r0
    bne @L_800A516C
    li r3, 0x0
    b @L_800A51E8
@L_800A516C:
    mulli r3, r3, 0xc
    lwzx r0, r4, r3
    mr r4, r29
    lwzx r3, r5, r3
    addi r5, r30, 0x0
    rlwinm r0, r0, 0, 8, 31
    add r28, r6, r0
    bl fn_800A5108
    cmplw r3, r30
    bne @L_800A5198
    b @L_800A51E8
@L_800A5198:
    addi r0, r3, 0x0
    addi r3, r3, 0x1
    li r4, 0x2f
    subf r6, r3, r30
    stbx r4, r29, r0
    addi r4, r6, 0x0
    add r5, r29, r3
    b @L_800A51CC
@L_800A51B8:
    lbz r0, 0x0(r28)
    addi r28, r28, 0x1
    subi r4, r4, 0x1
    stb r0, 0x0(r5)
    addi r5, r5, 0x1
@L_800A51CC:
    cmplwi r4, 0x0
    beq @L_800A51E0
    lbz r0, 0x0(r28)
    extsb. r0, r0
    bne @L_800A51B8
@L_800A51E0:
    subf r0, r4, r6
    add r3, r3, r0
@L_800A51E8:
    cmplw r3, r30
    bne @L_800A51F4
    b @L_800A5248
@L_800A51F4:
    addi r0, r3, 0x0
    addi r3, r3, 0x1
    li r4, 0x2f
    subf r7, r3, r30
    stbx r4, r29, r0
    addi r6, r31, 0x0
    addi r4, r7, 0x0
    add r5, r29, r3
    b @L_800A522C
@L_800A5218:
    lbz r0, 0x0(r6)
    addi r6, r6, 0x1
    subi r4, r4, 0x1
    stb r0, 0x0(r5)
    addi r5, r5, 0x1
@L_800A522C:
    cmplwi r4, 0x0
    beq @L_800A5240
    lbz r0, 0x0(r6)
    extsb. r0, r0
    bne @L_800A5218
@L_800A5240:
    subf r0, r4, r7
    add r3, r3, r0
@L_800A5248:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
