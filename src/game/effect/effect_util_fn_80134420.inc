    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r4
    bne @L_8013445C
    li r3, 0x0
    li r4, 0x3
    bl fn_80129280
    mr r31, r3
    @L_8013445C:
    mr r3, r28
    bl fn_801440A0
    cmplwi r3, 0x0
    bne @L_80134474
    li r3, 0x0
    b @L_80134564
    @L_80134474:
    mr r29, r31
    clrlwi r28, r28, 16
    li r30, 0x0
    @L_80134480:
    addi r3, r29, 0x6dec
    bl fn_801429E8
    clrlwi. r0, r3, 24
    beq @L_801344AC
    addi r3, r29, 0x6dec
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    cmpw r28, r3
    beq @L_801344BC
    @L_801344AC:
    addi r30, r30, 0x1
    addi r29, r29, 0x4
    cmpwi r30, 0xeb
    blt @L_80134480
    @L_801344BC:
    cmpwi r30, 0xeb
    li r3, -0x1
    bge @L_801344CC
    extsh r3, r30
    @L_801344CC:
    extsh. r0, r3
    blt @L_80134558
    extsh. r0, r3
    blt @L_801344E8
    extsh r0, r3
    cmpwi r0, 0xeb
    blt @L_801344F0
    @L_801344E8:
    li r30, 0x0
    b @L_801344FC
    @L_801344F0:
    slwi r3, r0, 2
    addi r30, r3, 0x6dec
    add r30, r31, r30
    @L_801344FC:
    cmplwi r30, 0x0
    bne @L_80134510
    lis r3, 0x1
    subi r3, r3, 0x1
    b @L_80134544
    @L_80134510:
    mr r3, r30
    bl fn_801429E8
    clrlwi. r0, r3, 24
    bne @L_8013452C
    lis r3, 0x1
    subi r3, r3, 0x1
    b @L_80134544
    @L_8013452C:
    mr r3, r30
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142CF4
    clrlwi r3, r3, 16
    @L_80134544:
    clrlwi r0, r3, 16
    cmplwi r0, 0x3e7
    ble @L_8013455C
    li r3, 0x0
    b @L_8013455C
    @L_80134558:
    li r3, 0x0
    @L_8013455C:
    subfic r0, r3, 0x3e7
    clrlwi r3, r0, 16
    @L_80134564:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
