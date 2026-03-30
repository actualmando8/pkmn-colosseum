    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    addi r4, r1, 0x8
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    lwz r3, -21580(r13)
    lwz r3, 0x14(r3)
    bl fn_800B5AE0
    lwz r30, 0x8(r1)
    li r3, 0x0
    lwz r31, 0xc(r1)
    li r4, 0x1
    bl fn_80129280
    bl fn_80135CD0
    mr r6, r31
    mr r5, r30
    bl fn_80135B8C
    lwz r7, -21580(r13)
    lis r3, 0x5555
    addi r3, r3, 0x5556
    li r5, 0x1
    lwz r6, 0x2c(r7)
    li r0, 0x0
    li r4, 0x1
    addi r6, r6, 0x1
    stw r6, 0x2c(r7)
    lwz r8, -21580(r13)
    lwz r6, 0x20(r8)
    addi r7, r6, 0x1
    mulhw r6, r3, r7
    rlwinm r3, r6, 1, 31, 31
    add r3, r6, r3
    mulli r3, r3, 0x3
    subf r3, r3, r7
    stw r3, 0x20(r8)
    lwz r3, -21580(r13)
    lwz r31, 0x50(r3)
    stb r5, 0x0(r31)
    addi r3, r31, 0x8
    stb r5, 0x1(r31)
    stb r0, 0x2(r31)
    stb r0, 0x3(r31)
    lwz r5, -21580(r13)
    lwz r0, 0x2c(r5)
    stw r0, 0x4(r31)
    lwz r30, 0x8(r1)
    lwz r29, 0xc(r1)
    bl fn_80129280
    bl fn_80135CD0
    mr r6, r29
    mr r5, r30
    bl fn_80135B8C
    lwz r3, -21580(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x8
    beq @L_801CF45C
    bge @L_801CF418
    cmpwi r0, 0x4
    beq @L_801CF45C
    b @L_801CF48C
@L_801CF418:
    cmpwi r0, 0xc
    bge @L_801CF48C
    cmpwi r0, 0xa
    bge @L_801CF45C
    addi r3, r31, 0x8
    li r4, 0xe
    bl fn_80129280
    mr r0, r3
    addi r3, r31, 0x8
    mr r29, r0
    bl fn_8006A718
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_801CF45C
    mr r3, r29
    li r4, 0x0
    bl fn_8006AF44
@L_801CF45C:
    addi r3, r31, 0x8
    li r4, 0x1
    bl fn_80129280
    li r4, 0x4
    bl fn_80135168
    mr r30, r3
    addi r3, r31, 0x8
    li r4, 0x1
    bl fn_80129280
    addi r5, r30, 0x1
    li r4, 0x4
    bl fn_80135030
@L_801CF48C:
    li r3, 0x0
    li r0, 0xc0
    stw r3, 0x20(r31)
    mtctr r0
@L_801CF49C:
    bdnz @L_801CF49C
    li r0, 0x0
    addis r5, r31, 0x2
    lis r4, 0x2
    stw r0, 0xc(r31)
    mr r3, r31
    li r6, 0x18
    subi r4, r4, 0x2028
    subi r5, r5, 0x2014
    bl fn_801CBE44
    lwz r4, 0x0(r31)
    addis r29, r31, 0x2
    lwz r0, 0x4(r31)
    li r30, 0x13
    lwz r3, 0x8(r31)
    subi r29, r29, 0x2028
    add r4, r4, r0
    lwz r0, 0xc(r31)
    add r4, r4, r3
    lwz r3, 0x10(r31)
    add r4, r4, r0
    lwz r0, 0x14(r31)
    add r4, r4, r3
    lwz r3, 0x18(r31)
    add r4, r4, r0
    lwz r0, 0x1c(r31)
    add r4, r4, r3
    add r4, r4, r0
    neg r0, r4
    stw r0, 0xc(r31)
@L_801CF514:
    bl fn_800E0C54
    cmpwi r30, 0x0
    stb r3, 0x0(r29)
    addi r29, r29, 0x1
    subi r30, r30, 0x1
    bne @L_801CF514
    lwz r3, -21580(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0xa
    bne @L_801CF548
    lwz r3, 0x18(r31)
    addi r0, r3, 0x1
    stw r0, 0x18(r31)
@L_801CF548:
    lwz r0, 0x24(r1)
    li r3, 0x26
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
