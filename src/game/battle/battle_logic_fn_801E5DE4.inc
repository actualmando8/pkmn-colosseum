    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    lwz r4, -21108(r13)
    lwz r3, -21104(r13)
    addi r0, r4, 0x101
    stw r4, -21208(r13)
    addi r5, r3, 0x69c
    stw r0, -21204(r13)
    lwz r3, 0x69c(r3)
    lbz r4, 0x0(r3)
    addi r0, r3, 0x2
    lbz r3, 0x1(r3)
    rlwimi r3, r4, 8, 16, 23
    clrlwi r30, r3, 16
    stw r0, 0x0(r5)
    subi r30, r30, 0x2
@L_801E5E34:
    lwz r5, -21104(r13)
    li r29, 0x0
    li r3, 0x0
    lwz r4, 0x69c(r5)
    addi r0, r4, 0x1
    stw r0, 0x69c(r5)
    lbz r6, 0x0(r4)
    lwz r4, -21104(r13)
    srawi r0, r6, 4
    lwz r5, 0x69c(r4)
    clrlslwi r4, r6, 28, 1
    clrlwi r0, r0, 24
    add r0, r4, r0
    stw r5, -21212(r13)
    clrlwi r31, r0, 24
    b @L_801E5F38
@L_801E5E74:
    lwz r5, -21104(r13)
    addi r3, r3, 0x8
    lwz r4, 0x69c(r5)
    addi r0, r4, 0x1
    stw r0, 0x69c(r5)
    lwz r5, -21104(r13)
    lbz r0, 0x0(r4)
    lwz r4, 0x69c(r5)
    add r29, r29, r0
    addi r0, r4, 0x1
    stw r0, 0x69c(r5)
    lwz r5, -21104(r13)
    lbz r0, 0x0(r4)
    lwz r4, 0x69c(r5)
    add r29, r29, r0
    addi r0, r4, 0x1
    stw r0, 0x69c(r5)
    lwz r5, -21104(r13)
    lbz r0, 0x0(r4)
    lwz r4, 0x69c(r5)
    add r29, r29, r0
    addi r0, r4, 0x1
    stw r0, 0x69c(r5)
    lwz r5, -21104(r13)
    lbz r0, 0x0(r4)
    lwz r4, 0x69c(r5)
    add r29, r29, r0
    addi r0, r4, 0x1
    stw r0, 0x69c(r5)
    lwz r5, -21104(r13)
    lbz r0, 0x0(r4)
    lwz r4, 0x69c(r5)
    add r29, r29, r0
    addi r0, r4, 0x1
    stw r0, 0x69c(r5)
    lwz r5, -21104(r13)
    lbz r0, 0x0(r4)
    lwz r4, 0x69c(r5)
    add r29, r29, r0
    addi r0, r4, 0x1
    stw r0, 0x69c(r5)
    lwz r5, -21104(r13)
    lbz r0, 0x0(r4)
    lwz r4, 0x69c(r5)
    add r29, r29, r0
    addi r0, r4, 0x1
    stw r0, 0x69c(r5)
    lbz r0, 0x0(r4)
    add r29, r29, r0
@L_801E5F38:
    clrlwi r0, r3, 24
    cmplwi r0, 0x10
    blt @L_801E5E74
    mulli r3, r31, 0xe0
    lwz r5, -21104(r13)
    lwz r4, 0x69c(r5)
    addi r0, r3, 0x340
    stwx r4, r5, r0
    clrlwi r0, r29, 16
    lwz r4, -21104(r13)
    lwz r3, 0x69c(r4)
    add r0, r3, r0
    stw r0, 0x69c(r4)
    bl fn_801E5FC4
    bl fn_801E60B4
    mr r3, r31
    bl fn_801E611C
    lwz r5, -21104(r13)
    addi r0, r29, 0x11
    subf r30, r0, r30
    li r0, 0x1
    lbz r4, 0x6a8(r5)
    slw r3, r0, r31
    or r3, r4, r3
    clrlwi. r0, r30, 16
    stb r3, 0x6a8(r5)
    bne @L_801E5E34
    lwz r0, 0x24(r1)
    li r3, 0x0
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
