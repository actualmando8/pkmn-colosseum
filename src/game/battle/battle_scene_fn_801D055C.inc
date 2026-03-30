    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    lwz r6, -21580(r13)
    lwz r0, 0x0(r6)
    cmpwi r0, 0x0
    beq @L_801D0680
@L_801D0588:
    bl fn_800F0308
    lwz r3, -21580(r13)
    lwz r0, 0xc(r3)
    cmpwi r0, 0x30
    bne @L_801D05BC
    lwz r3, 0x4(r3)
    bl fn_801D0090
    lwz r4, -21580(r13)
    rlwinm r0, r3, 0, 24, 31
    stw r0, 0x4(r4)
    lwz r3, -21580(r13)
    lwz r0, 0x10(r3)
    stw r0, 0xc(r3)
@L_801D05BC:
    lwz r3, -21580(r13)
    lwz r0, 0xc(r3)
    cmpwi r0, 0x32
    beq @L_801D05D4
    li r0, 0x0
    b @L_801D05D8
@L_801D05D4:
    lwz r0, 0x8(r3)
@L_801D05D8:
    cmpwi r0, 0x0
    beq @L_801D0588
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_801D0674
    lwz r0, 0x0(r3)
    cmpwi r0, 0x8
    beq @L_801D0600
    li r3, 0x1
    bl fn_801069FC
@L_801D0600:
    lwz r3, -21580(r13)
    lwz r3, 0x64(r3)
    bl fn_800FE714
    lwz r3, -21580(r13)
    lwz r27, 0x54(r3)
    mr r3, r27
    bl fn_800E202C
    mr r28, r3
    bl fn_800E24B0
    mr r3, r28
    bl fn_800E209C
    cmpwi r3, 0x0
    bne @L_801D0638
    li r27, 0x0
@L_801D0638:
    lwz r3, -21580(r13)
    stw r27, 0x54(r3)
    lwz r3, -21580(r13)
    lwz r28, 0x50(r3)
    mr r3, r28
    bl fn_800E202C
    mr r27, r3
    bl fn_800E24B0
    mr r3, r27
    bl fn_800E209C
    cmpwi r3, 0x0
    bne @L_801D066C
    li r28, 0x0
@L_801D066C:
    lwz r3, -21580(r13)
    stw r28, 0x50(r3)
@L_801D0674:
    lwz r3, -21580(r13)
    li r0, 0x0
    stw r0, 0x0(r3)
@L_801D0680:
    cmpwi r30, 0x0
    bne @L_801D068C
    li r30, 0x2
@L_801D068C:
    lwz r5, -21580(r13)
    lis r3, 0x2
    li r0, 0x0
    li r4, 0x20
    stw r29, 0x0(r5)
    subi r3, r3, 0x1a00
    lwz r5, -21580(r13)
    stw r31, 0x14(r5)
    lwz r5, -21580(r13)
    stw r30, 0x5c(r5)
    lwz r5, -21580(r13)
    stw r0, 0x8(r5)
    lwz r5, -21580(r13)
    stw r0, 0xc(r5)
    lwz r5, -21580(r13)
    stw r0, 0x4(r5)
    lwz r5, -21580(r13)
    stb r0, 0x3e(r5)
    bl fn_800E2C04
    bl fn_800E27B0
    lwz r6, -21580(r13)
    lis r5, 0x1
    li r4, 0x20
    stw r3, 0x50(r6)
    subi r3, r5, 0x6000
    bl fn_800E2C04
    bl fn_800E27B0
    lwz r5, -21580(r13)
    lis r4, -32739
    addi r6, r4, -9468
    stw r3, 0x54(r5)
    li r4, 0x17
    li r3, 0x1
    li r5, 0x0
    bl fn_800FE834
    lwz r5, -21580(r13)
    li r4, 0x0
    stw r3, 0x64(r5)
    li r3, 0x0
    bl fn_80129280
    lwz r4, -21580(r13)
    stw r3, 0x58(r4)
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
