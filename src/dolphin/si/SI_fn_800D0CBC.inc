    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    addi r31, r4, 0x0
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    addi r28, r3, 0x0
    bl OSDisableInterrupts
    addi r30, r3, 0x0
    addi r3, r28, 0x0
    bl SIGetType
    lis r4, -32719
    rlwinm r5, r28, 2, 0, 29
    addi r0, r4, 16288
    add r4, r0, r5
    lwz r0, 0x0(r4)
    addi r29, r3, 0x0
    rlwinm. r0, r0, 0, 24, 24
    beq @L_800D0DB8
    lis r3, -32704
    rlwinm r4, r28, 4, 0, 27
    addi r0, r3, 272
    add r4, r0, r4
    lwz r0, 0x0(r4)
    li r5, 0x0
    cmplw r0, r31
    beq @L_800D0DCC
    cmplwi r0, 0x0
    bne @L_800D0D44
    rlwinm r0, r5, 2, 0, 29
    stwx r31, r4, r0
    b @L_800D0DCC
@L_800D0D44:
    addi r3, r4, 0x4
    lwz r0, 0x4(r4)
    li r5, 0x1
    cmplw r0, r31
    beq @L_800D0DCC
    cmplwi r0, 0x0
    bne @L_800D0D6C
    rlwinm r0, r5, 2, 0, 29
    stwx r31, r4, r0
    b @L_800D0DCC
@L_800D0D6C:
    lwzu r0, 0x4(r3)
    li r5, 0x2
    cmplw r0, r31
    beq @L_800D0DCC
    cmplwi r0, 0x0
    bne @L_800D0D90
    rlwinm r0, r5, 2, 0, 29
    stwx r31, r4, r0
    b @L_800D0DCC
@L_800D0D90:
    lwz r0, 0x4(r3)
    li r5, 0x3
    cmplw r0, r31
    beq @L_800D0DCC
    cmplwi r0, 0x0
    bne @L_800D0DCC
    rlwinm r0, r5, 2, 0, 29
    stwx r31, r4, r0
    b @L_800D0DCC
    b @L_800D0DCC
@L_800D0DB8:
    addi r12, r31, 0x0
    mtlr r12
    addi r3, r28, 0x0
    addi r4, r29, 0x0
    blrl
@L_800D0DCC:
    mr r3, r30
    bl OSRestoreInterrupts
    mr r3, r29
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
