    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    li r4, 0x0
    li r3, -0x1
    b @L_80207A10
@L_80207A04:
    clrlslwi r0, r4, 16, 2
    addi r4, r4, 0x1
    stwx r3, r31, r0
@L_80207A10:
    rlwinm r0, r4, 0, 16, 31
    cmplwi r0, 0x12
    blt @L_80207A04
    li r27, 0x0
    li r28, 0x0
    b @L_80207ABC
@L_80207A28:
    mr r3, r29
    li r4, 0x0
    li r5, 0xff
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r28, 0, 16, 31
    cmplw r0, r3
    beq @L_80207A70
    mr r3, r29
    li r4, 0x0
    li r5, 0xff
    li r6, 0x1
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r28, 0, 16, 31
    cmplw r0, r3
    bne @L_80207A78
@L_80207A70:
    li r0, 0x1
    b @L_80207A7C
@L_80207A78:
    li r0, 0x0
@L_80207A7C:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80207AB8
    mr r3, r30
    mr r4, r28
    bl fn_8010C74C
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x42
    beq @L_80207AA8
    cmplwi r0, 0x43
    bne @L_80207AB8
@L_80207AA8:
    clrlslwi r0, r27, 16, 2
    rlwinm r3, r28, 0, 16, 31
    stwx r3, r31, r0
    addi r27, r27, 0x1
@L_80207AB8:
    addi r28, r28, 0x1
@L_80207ABC:
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x12
    blt @L_80207A28
    mr r3, r27
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
