    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r4
    lwz r3, 0x60(r3)
    lwz r29, 0x4(r3)
    mr r3, r29
    bl fn_801D16F0
    bl fn_8017B2CC
    cmpwi r3, 0x1
    bne @L_8004E478
    li r0, 0x0
    b @L_8004E47C
    @L_8004E478:
    li r0, 0x1
    @L_8004E47C:
    cmpwi r0, 0x0
    bne @L_8004E48C
    li r29, -0x1
    b @L_8004E4D4
    @L_8004E48C:
    mr r3, r29
    bl fn_801D16F0
    mr r28, r3
    bl fn_8017B448
    li r29, 0x0
    mr r31, r3
    mr r30, r29
    b @L_8004E4CC
    @L_8004E4AC:
    mr r3, r28
    mr r4, r30
    bl fn_8017B4BC
    bl fn_8017B5A4
    cmplwi r3, 0x9
    bne @L_8004E4C8
    addi r29, r29, 0x1
    @L_8004E4C8:
    addi r30, r30, 0x1
    @L_8004E4CC:
    cmpw r30, r31
    blt @L_8004E4AC
    @L_8004E4D4:
    cmpwi r29, 0x0
    bgt @L_8004E4EC
    mr r3, r27
    li r4, 0x0
    bl fn_80109220
    b @L_8004E4F8
    @L_8004E4EC:
    mr r3, r27
    li r4, 0x1
    bl fn_80109220
    @L_8004E4F8:
    li r3, 0x0
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
