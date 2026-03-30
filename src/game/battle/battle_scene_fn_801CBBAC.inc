    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    rlwinm r10, r5, 3, 0, 28
    lis r9, 0x6745
    stw r0, 0x84(r1)
    lis r8, 0xefce
    lis r7, 0x98bb
    lis r6, 0x1032
    stmw r27, 0x6c(r1)
    li r0, 0x0
    mr r31, r5
    lis r5, 0xc3d3
    addi r9, r9, 0x2301
    subi r8, r8, 0x5477
    subi r7, r7, 0x2302
    addi r6, r6, 0x5476
    subi r5, r5, 0x1e10
    cmplw r10, r10
    mr r29, r3
    mr r30, r4
    stw r0, 0x1c(r1)
    stw r8, 0xc(r1)
    rlwinm r8, r0, 29, 26, 31
    stw r9, 0x8(r1)
    stw r7, 0x10(r1)
    stw r6, 0x14(r1)
    stw r5, 0x18(r1)
    stw r0, 0x20(r1)
    stw r10, 0x1c(r1)
    bge @L_801CBC2C
    li r0, 0x1
    stw r0, 0x20(r1)
@L_801CBC2C:
    lwz r4, 0x20(r1)
    rlwinm r3, r31, 3, 29, 31
    add r0, r8, r31
    add r3, r4, r3
    cmplwi r0, 0x3f
    stw r3, 0x20(r1)
    ble @L_801CBCA0
    subfic r28, r8, 0x40
    addi r0, r1, 0x8
    addi r3, r8, 0x1c
    mr r4, r30
    mr r5, r28
    add r3, r0, r3
    bl memcpy
    addi r3, r1, 0x8
    addi r4, r1, 0x24
    bl fn_801CC380
    add r27, r30, r28
    b @L_801CBC8C
@L_801CBC78:
    mr r4, r27
    addi r3, r1, 0x8
    bl fn_801CC380
    addi r27, r27, 0x40
    addi r28, r28, 0x40
@L_801CBC8C:
    addi r0, r28, 0x3f
    cmplw r0, r31
    blt @L_801CBC78
    li r8, 0x0
    b @L_801CBCA4
@L_801CBCA0:
    li r28, 0x0
@L_801CBCA4:
    addi r0, r1, 0x8
    addi r3, r8, 0x1c
    add r3, r0, r3
    add r4, r30, r28
    subf r5, r28, r31
    bl memcpy
    mr r3, r29
    addi r4, r1, 0x8
    bl fn_801CBF64
    lmw r27, 0x6c(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
