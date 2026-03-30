    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    bne @L_801B70AC
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21416
    li r4, 0x127
    bl fn_80196E10
@L_801B70AC:
    li r3, 0x7c
    bl fn_80193B10
    mr. r31, r3
    bne @L_801B70CC
    addi r3, r2, -22576
    li r4, 0x3f
    addi r5, r2, -22544
    bl fn_80196E10
@L_801B70CC:
    li r4, 0xff
    li r5, 0x7c
    mr r3, r31
    bl memset
    li r0, 0x1
    li r8, 0x0
    stw r0, 0x0(r31)
    li r7, 0x0
    li r6, 0x0
    li r5, 0x1
    lwz r9, 0x0(r30)
    li r4, 0x2
    li r3, 0x3
    li r0, 0x0
    stw r9, 0x4(r31)
    li r9, 0x0
    stw r31, 0x0(r30)
    stw r8, 0x8(r31)
    stw r7, 0x14(r31)
    stw r6, 0x24(r31)
    stw r5, 0x28(r31)
    stw r4, 0x2c(r31)
    stw r3, 0x30(r31)
    stw r0, 0x74(r31)
    b @L_801B7154
@L_801B7130:
    rlwinm r3, r9, 3, 0, 28
    rlwinm r0, r9, 3, 0, 28
    add r3, r31, r3
    li r4, 0x0
    stw r4, 0x38(r3)
    add r3, r31, r0
    li r0, 0x0
    addi r9, r9, 0x1
    stw r0, 0x58(r3)
@L_801B7154:
    cmpwi r9, 0x4
    blt @L_801B7130
    lwz r0, 0x14(r1)
    mr r3, r31
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
