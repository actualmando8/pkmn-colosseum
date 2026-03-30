    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r4, -32698
    li r5, 0x14
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    addi r28, r4, 31872
    mr r31, r3
    li r4, 0x0
    addi r3, r28, 0x40
    bl memset
    addi r3, r28, 0x54
    li r4, 0x0
    li r5, 0x20
    bl memset
    bl fn_80115BD8
    mr r26, r3
    bl fn_80115A80
    mr r0, r3
    mr r3, r26
    mr r27, r0
    bl fn_80115280
    li r0, 0x0
    mr r29, r3
    stw r0, -21516(r13)
    addi r30, r28, 0x0
    li r25, 0x0
    b @L_801DAFA0
@L_801DAF68:
    mr r3, r26
    mr r4, r25
    bl fn_8011538C
    mr r4, r3
    mr r3, r27
    bl fn_800F9318
    cmplwi r3, 0x0
    beq @L_801DAF9C
    lwz r5, -21516(r13)
    addi r4, r5, 0x1
    rlwinm r0, r5, 2, 0, 29
    stw r4, -21516(r13)
    stwx r3, r30, r0
@L_801DAF9C:
    addi r25, r25, 0x1
@L_801DAFA0:
    cmpw r25, r29
    blt @L_801DAF68
    mr r3, r26
    bl fn_8011542C
    mr. r4, r3
    beq @L_801DAFC4
    mr r3, r27
    bl fn_800F9318
    stw r3, -21512(r13)
@L_801DAFC4:
    cmpwi r31, 0x0
    beq @L_801DB04C
    bl fn_801019F8
    mulli r29, r31, 0x8c
    mr r3, r29
    bl fn_800E3534
    rlwinm. r0, r3, 0, 16, 31
    beq @L_801DB04C
    addi r4, r28, 0x40
    sth r3, 0x10(r4)
    sth r31, 0x4(r4)
    bl fn_800E27B0
    stw r3, 0x40(r28)
    mr r5, r29
    li r4, 0x0
    bl memset
    bl fn_801D301C
    li r3, 0x6f7
    li r4, 0x0
    bl fn_801DE598
    lis r4, 0x11ef
    li r3, 0x6f7
    addi r4, r4, 0x2400
    bl fn_800F9318
    addi r5, r28, 0x40
    lis r4, 0x11ee
    stw r3, 0x8(r5)
    addi r4, r4, 0x2400
    li r3, 0x6f7
    bl fn_800F9318
    addi r4, r28, 0x40
    li r0, 0x0
    stw r3, 0xc(r4)
    stb r0, 0x6(r4)
@L_801DB04C:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
