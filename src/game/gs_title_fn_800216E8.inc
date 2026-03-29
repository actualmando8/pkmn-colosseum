    nofralloc
    stwu r1, -0x120(r1)
    mflr r0
    stw r0, 0x124(r1)
    stmw r27, 0x10c(r1)
    mr r27, r3
    mr r28, r4
    lis r3, lbl_80266C7C@ha
    li r0, 0x1f
    addi r3, r3, lbl_80266C7C@l
    addi r8, r1, 0x4
    subi r4, r3, 0x4
    mtctr r0
    @L_80021718:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r8)
    stwu r0, 0x8(r8)
    bdnz @L_80021718
    extsh r9, r6
    lwz r0, 0x4(r4)
    cmpwi r9, 0x0
    stw r0, 0x4(r8)
    bgt @L_80021754
    mr r3, r27
    addi r4, r28, 0x1
    li r5, 0x4261
    bl fn_800F96E4
    b @L_800218A8
    @L_80021754:
    addi r4, r1, 0x8
    li r10, 0x0
    @L_8002175C:
    lwz r6, 0x0(r4)
    mr r3, r5
    li r8, 0x0
    mtctr r9
    cmpwi r9, 0x0
    ble @L_8002178C
    @L_80021774:
    lwz r0, 0x0(r3)
    cmpw r6, r0
    beq @L_8002178C
    addi r3, r3, 0x8
    addi r8, r8, 0x1
    bdnz @L_80021774
    @L_8002178C:
    cmpw r8, r9
    blt @L_800217A4
    addi r4, r4, 0xc
    addi r10, r10, 0x1
    cmpwi r10, 0x15
    blt @L_8002175C
    @L_800217A4:
    cmpwi r10, 0x15
    blt @L_800217C0
    mr r3, r27
    addi r4, r28, 0x1
    li r5, 0x4261
    bl fn_800F96E4
    b @L_800218A8
    @L_800217C0:
    mulli r6, r10, 0xc
    addi r3, r1, 0x10
    slwi r0, r8, 3
    addi r4, r1, 0xc
    lwzx r11, r3, r6
    add r3, r5, r0
    lwzx r29, r4, r6
    cmpwi r11, 0x0
    lha r31, 0x4(r3)
    lha r30, 0x6(r3)
    blt @L_80021868
    addi r10, r10, 0x1
    addi r4, r1, 0x8
    mulli r0, r10, 0xc
    add r4, r4, r0
    b @L_80021854
    @L_80021800:
    lwz r0, 0x8(r4)
    cmpwi r0, 0x0
    blt @L_8002184C
    cmpw r11, r0
    beq @L_8002184C
    lwz r6, 0x0(r4)
    mr r3, r5
    li r8, 0x0
    mtctr r9
    cmpwi r9, 0x0
    ble @L_80021844
    @L_8002182C:
    lwz r0, 0x0(r3)
    cmpw r6, r0
    beq @L_80021844
    addi r3, r3, 0x8
    addi r8, r8, 0x1
    bdnz @L_8002182C
    @L_80021844:
    cmpw r8, r9
    blt @L_8002185C
    @L_8002184C:
    addi r4, r4, 0xc
    addi r10, r10, 0x1
    @L_80021854:
    cmpwi r10, 0x15
    blt @L_80021800
    @L_8002185C:
    cmpwi r10, 0x15
    bge @L_80021868
    li r29, 0x4201
    @L_80021868:
    mr r3, r7
    bl fn_8011F4F0
    mr r0, r3
    li r3, 0x32
    mr r4, r0
    bl fn_80132A38
    mr r4, r31
    li r3, 0x2f
    bl fn_80132A38
    mr r4, r30
    li r3, 0x30
    bl fn_80132A38
    mr r3, r27
    mr r5, r29
    addi r4, r28, 0x1
    bl fn_800F96E4
    @L_800218A8:
    lmw r27, 0x10c(r1)
    lwz r0, 0x124(r1)
    mtlr r0
    addi r1, r1, 0x120
    blr
