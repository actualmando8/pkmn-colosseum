    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r11, 0xffff
    stw r0, 0x24(r1)
    addi r11, r11, 0x33d0
    stmw r27, 0xc(r1)
    mr r31, r1
    lwz r0, 0x0(r1)
    stwux r0, r1, r11
    lwz r3, -21580(r13)
    li r4, 0x1
    lwz r3, 0x50(r3)
    addi r30, r3, 0x8
    mr r3, r30
    bl fn_80129280
    mr r0, r3
    mr r3, r30
    mr r29, r0
    li r4, 0x2
    bl fn_80129280
    mr r0, r3
    mr r3, r30
    mr r27, r0
    li r4, 0xe
    bl fn_80129280
    mr r0, r3
    li r3, 0x0
    mr r28, r0
    li r4, 0xe
    bl fn_80129280
    lwz r4, -21580(r13)
    mr r5, r3
    lwz r0, 0x0(r4)
    cmpwi r0, 0x9
    beq @L_801CF61C
    bge @L_801CF610
    cmpwi r0, 0x6
    beq @L_801CF70C
    bge @L_801CF78C
    cmpwi r0, 0x5
    bge @L_801CF6DC
    b @L_801CF78C
@L_801CF610:
    cmpwi r0, 0xd
    beq @L_801CF6F0
    b @L_801CF78C
@L_801CF61C:
    subis r3, r31, 0x1
    li r0, 0x1985
    addi r5, r3, 0x33d8
    subi r4, r28, 0x4
    mtctr r0
@L_801CF630:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801CF630
    lwz r0, 0x4(r4)
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x4(r5)
    bl fn_80129280
    li r0, 0x3bfa
    subi r5, r30, 0x4
    subi r4, r3, 0x4
    mtctr r0
@L_801CF668:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801CF668
    subis r3, r31, 0x1
    li r0, 0x1985
    subi r5, r28, 0x4
    addi r4, r3, 0x33d8
    mtctr r0
@L_801CF690:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801CF690
    lwz r3, 0x4(r4)
    li r0, 0x3bfa
    subi r4, r30, 0x4
    stw r3, 0x4(r5)
    lwz r3, -21580(r13)
    lwz r3, 0x58(r3)
    subi r5, r3, 0x4
    mtctr r0
@L_801CF6C4:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801CF6C4
    b @L_801CF78C
@L_801CF6DC:
    addis r4, r5, 0x1
    mr r3, r28
    subi r4, r4, 0x4cd8
    bl fn_8006AF44
    b @L_801CF78C
@L_801CF6F0:
    addis r4, r5, 0x1
    addis r3, r28, 0x1
    li r5, 0x1f8
    subi r4, r4, 0x3624
    subi r3, r3, 0x3624
    bl memcpy
    b @L_801CF78C
@L_801CF70C:
    li r3, 0x0
    li r4, 0xd
    li r5, 0x0
    bl fn_8012A5B0
    mr r0, r3
    mr r3, r27
    mr r5, r0
    li r4, 0xd
    bl fn_8012A450
    li r3, 0x0
    li r4, 0xe
    li r5, 0x0
    bl fn_8012A5B0
    mr r0, r3
    mr r3, r27
    mr r5, r0
    li r4, 0xe
    bl fn_8012A450
    li r3, 0x0
    li r4, 0xe
    bl fn_80129280
    li r0, 0x1985
    subi r5, r28, 0x4
    subi r4, r3, 0x4
    mtctr r0
@L_801CF770:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_801CF770
    lwz r0, 0x4(r4)
    stw r0, 0x4(r5)
@L_801CF78C:
    li r3, 0x0
    li r4, 0x9
    bl fn_80135168
    rlwinm r5, r3, 0, 24, 31
    mr r3, r29
    li r4, 0x9
    bl fn_80135030
    li r3, 0x0
    li r4, 0xa
    bl fn_80135168
    rlwinm r5, r3, 0, 24, 31
    mr r3, r29
    li r4, 0xa
    bl fn_80135030
    mr r10, r31
    li r3, 0x25
    lmw r27, 0xc(r10)
    lwz r10, 0x0(r1)
    lwz r0, 0x4(r10)
    mr r1, r10
    mtlr r0
    blr
