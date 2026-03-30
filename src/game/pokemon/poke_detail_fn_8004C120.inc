    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    li r30, 0x0
    bl fn_801D1F7C
    mr r28, r3
    cmpwi r28, 0x0
    ble @L_8004C288
    rlwinm r3, r28, 1, 0, 30
    bl fn_800E3534
    mr r29, r3
    bl fn_800E27B0
    stw r3, -25376(r13)
    mr r31, r3
    bl fn_801D1B4C
    cmpwi r3, 0x2
    beq @L_8004C1B8
    bge @L_8004C17C
    cmpwi r3, 0x0
    beq @L_8004C258
    bge @L_8004C188
    b @L_8004C258
@L_8004C17C:
    cmpwi r3, 0x4
    bge @L_8004C258
    b @L_8004C208
@L_8004C188:
    li r27, 0x0
    b @L_8004C1A8
@L_8004C190:
    mr r3, r27
    bl fn_801D1F0C
    rlwinm r0, r3, 0, 16, 31
    addi r27, r27, 0x1
    sth r0, 0x0(r31)
    addi r31, r31, 0x2
@L_8004C1A8:
    bl fn_801D1F7C
    cmpw r27, r3
    blt @L_8004C190
    b @L_8004C290
@L_8004C1B8:
    bl fn_801D1F7C
    mr r27, r31
    mr r25, r3
    li r26, 0x0
    b @L_8004C1E4
@L_8004C1CC:
    mr r3, r26
    bl fn_801D1F0C
    rlwinm r0, r3, 0, 16, 31
    addi r26, r26, 0x1
    sth r0, 0x0(r27)
    addi r27, r27, 0x2
@L_8004C1E4:
    cmpw r26, r25
    blt @L_8004C1CC
    lis r4, -32763
    mr r3, r31
    addi r6, r4, -16608
    li r5, 0x2
    mr r4, r25
    bl fn_800CA620
    b @L_8004C290
@L_8004C208:
    bl fn_801D1F7C
    mr r25, r31
    mr r27, r3
    li r26, 0x0
    b @L_8004C234
@L_8004C21C:
    mr r3, r26
    bl fn_801D1F0C
    rlwinm r0, r3, 0, 16, 31
    addi r26, r26, 0x1
    sth r0, 0x0(r25)
    addi r25, r25, 0x2
@L_8004C234:
    cmpw r26, r27
    blt @L_8004C21C
    lis r4, -32763
    mr r3, r31
    addi r6, r4, -16752
    li r5, 0x2
    mr r4, r27
    bl fn_800CA620
    b @L_8004C290
@L_8004C258:
    bl fn_801D1F7C
    subi r25, r3, 0x1
    b @L_8004C27C
@L_8004C264:
    mr r3, r25
    bl fn_801D1F0C
    rlwinm r0, r3, 0, 16, 31
    subi r25, r25, 0x1
    sth r0, 0x0(r31)
    addi r31, r31, 0x2
@L_8004C27C:
    cmpwi r25, 0x0
    bge @L_8004C264
    b @L_8004C290
@L_8004C288:
    li r0, 0x0
    stw r0, -25376(r13)
@L_8004C290:
    mr r3, r30
    bl fn_8004D34C
    cmpwi r3, 0x0
    blt @L_8004C2AC
    bl fn_8004D9C0
    mr r30, r3
    b @L_8004C290
@L_8004C2AC:
    cmpwi r28, 0x0
    ble @L_8004C2C4
    mr r3, r29
    bl fn_800E24B0
    mr r3, r29
    bl fn_800E209C
@L_8004C2C4:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
